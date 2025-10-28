# osd_lock 深入分析报告

## 问题发现

通过分析所有 `pthread_mutex_lock/unlock(&osd_lock)` 的使用位置，发现了**严重的设计问题**：

### 加锁位置（Lock）

| 文件 | 行号 | 函数 | 说明 |
|------|------|------|------|
| video_encoder.c | 724 | VideoEncoder_SetFormat | ❌ **编码器函数加锁OSD** |
| video_osd.c | 112 | VideoOSD_Destroy | ✅ OSD模块内部加锁 |
| video_osd.c | 313 | VideoOSD_SetTitle | ✅ OSD模块内部加锁 |
| video_osd.c | 550 | VideoOSD_SetLogo | ✅ OSD模块内部加锁 |
| video_osd.c | 911 | _osd_xxx (内部函数) | ✅ 内部加锁 |
| video_osd.c | 1112 | _osd_xxx (内部函数) | ✅ 内部加锁 |
| video_osd.c | 1349 | _osd_xxx (内部函数) | ✅ 内部加锁 |

### 解锁位置（Unlock）

| 文件 | 行号 | 函数 | 配对的Lock在哪？ |
|------|------|------|----------------|
| video_encoder.c | 728, 744, 754, 760 | VideoEncoder_SetFormat | ✅ 同函数内的 line 724 |
| video_osd.c | **804** | **VideoOSD_SetTitleRegion** | ❌ **没有配对的lock！** |
| video_osd.c | 130 | VideoOSD_Destroy | ✅ 同函数内的 line 112 |
| video_osd.c | 483 | VideoOSD_SetTitle | ✅ 同函数内的 line 313 |
| video_osd.c | 668 | VideoOSD_SetLogo | ✅ 同函数内的 line 550 |
| video_osd.c | 929, 1210, 1235, 1247, 1258, 1268, 1276, 1282 | _osd_xxx (内部) | ✅ 配对的lock在同函数内 |
| video_osd.c | 1366, 1426, 1433, 1442, 1452, 1459, 1467 | _osd_xxx (内部) | ✅ 配对的lock在同函数内 |

---

## 核心问题：VideoOSD_SetTitleRegion

```c
int VideoOSD_SetTitleRegion()
{
    // ...
    VideoOSD_Create();

    pthread_mutex_unlock(&osd_lock);  // ❌ 这里unlock，但没有lock！

    for (int i = 0; i < TD_OSD_MAX_NUM; i++)
    {
        // ...
        ret |= VideoOSD_SetTitle(0, &pParam);  // 这里会lock+unlock
    }

    return ret;
}
```

**为什么会有这个孤立的 unlock？**

看调用链：
```
VideoEncoder_SetFormat (line 724: lock)
    ↓
    VideoEncoder_UpdateChannelConfig
        ↓
        _rebuild_overlays
            ↓
            VideoOSD_SetTitleRegion (line 804: unlock)  ← 这里解锁上层的锁！
                ↓
                VideoOSD_SetTitle (内部 lock + unlock)
```

**原始设计意图：**
1. VideoEncoder_SetFormat 加锁（保护整个编码设置流程）
2. 中途调用 VideoOSD_SetTitleRegion 解锁（因为 VideoOSD_SetTitle 内部会重新加锁）
3. VideoOSD_SetTitle 内部加锁+解锁（保护OSD操作）
4. VideoEncoder_SetFormat 结束时再次解锁

**这是非常糟糕的设计！违反了"谁加锁谁解锁"的原则。**

---

## 模块化重构后的正确方案

### 原则

1. **职责分离**：
   - **osd_lock 只保护 OSD 操作**（OSD Create/Destroy/SetTitle/SetLogo）
   - **编码器模块不应该加 osd_lock**

2. **谁加锁谁解锁**：
   - OSD模块内部的函数自己管理锁
   - 不允许跨模块传递锁的责任

3. **最小锁范围**：
   - 锁只保护必要的临界区
   - 不要在整个 SetFormat 流程中持有锁

### 修改方案

#### 1. 删除 VideoEncoder_SetFormat 中的 osd_lock

**原因：**
- VideoEncoder_SetFormat 主要是编码器参数设置（VPS + VENC）
- 只有在分辨率变化时才会调用 `_rebuild_overlays`
- `_rebuild_overlays` 内部调用的 OSD 函数已经自己加锁了

**修改：**
```c
// video_encoder.c:VideoEncoder_SetFormat
int VideoEncoder_SetFormat(...)
{
    // ...

    // Step 1: Stop encoder
    // ❌ 删除这行：pthread_mutex_lock(&osd_lock);
    ret = VideoEncoder_Stop(VencChn);
    if (ret != RETURN_OK)
    {
        // ❌ 删除这行：pthread_mutex_unlock(&osd_lock);
        PRINT_ERROR("...");
        return -1;
    }

    // Step 2: Set VPS
    ret = VideoVPS_SetOutputParam(...);
    if (ret != RETURN_OK)
    {
        VideoEncoder_Start(VencChn);
        // ❌ 删除这行：pthread_mutex_unlock(&osd_lock);
        return -1;
    }

    // Step 3: Set VENC
    ret = VideoEncoder_UpdateChannelConfig(...);
    if (ret != RETURN_OK)
    {
        VideoEncoder_Start(VencChn);
        // ❌ 删除这行：pthread_mutex_unlock(&osd_lock);
        return -1;
    }

    // Step 4: Start encoder
    ret = VideoEncoder_Start(VencChn);
    // ❌ 删除这行：pthread_mutex_unlock(&osd_lock);
    // ...
}
```

#### 2. 修复 VideoOSD_SetTitleRegion

**删除孤立的 unlock：**
```c
int VideoOSD_SetTitleRegion()
{
    // ...
    VideoOSD_Create();

    // ❌ 删除这行：pthread_mutex_unlock(&osd_lock);

    for (int i = 0; i < TD_OSD_MAX_NUM; i++)
    {
        // ...
        ret |= VideoOSD_SetTitle(0, &pParam);  // 内部已经有lock+unlock
    }

    return ret;
}
```

#### 3. 确认 OSD 模块内部的锁是完整的

**检查清单：**
- ✅ VideoOSD_SetTitle：有 lock (line 313) + unlock (line 483)
- ✅ VideoOSD_SetLogo：有 lock (line 550) + unlock (line 668)
- ✅ VideoOSD_Destroy：有 lock (line 112) + unlock (line 130)
- ✅ VideoOSD_Create：不需要锁（只是创建region，不修改显示状态）

---

## 为什么原来要加那么多 unlock？

**答案：因为原来的设计是"外部加锁，内部多次解锁"**

```
VideoEncoder_SetFormat:
    lock                           ← 一次加锁
    ↓
    Step 1: Stop encoder
    if error: unlock; return       ← 错误退出解锁
    ↓
    Step 2: Set VPS
    if error: unlock; return       ← 错误退出解锁
    ↓
    Step 3: Set VENC
        ↓
        VideoOSD_SetTitleRegion:
            unlock                 ← 中途解锁（让 SetTitle 重新加锁）
            VideoOSD_SetTitle:
                lock
                ...
                unlock
    if error: unlock; return       ← 错误退出解锁
    ↓
    Step 4: Start encoder
    unlock                         ← 正常退出解锁
```

**问题：**
1. 锁的粒度太大（整个 SetFormat 流程）
2. 锁的职责混乱（编码器操作为什么要锁OSD？）
3. 跨模块传递锁的责任（OSD模块被迫知道外层有锁）

---

## 模块化后不需要那么多 unlock 的原因

**原因：职责分离 + 最小锁范围**

```
VideoEncoder_SetFormat:
    (无锁)
    ↓
    Step 1: Stop encoder
    ↓
    Step 2: Set VPS
    ↓
    Step 3: Set VENC
        ↓
        if (resolution_changed):
            _rebuild_overlays:
                ↓
                VideoOSD_SetTitleRegion:
                    ↓
                    VideoOSD_SetTitle:
                        lock       ← OSD模块自己加锁
                        ...
                        unlock     ← OSD模块自己解锁
                ↓
                VideoVPS_SetCoverRegion:
                    (内部也应该自己加锁，保护Cover操作)
                ↓
                VideoOSD_SetLogoRegion:
                    ↓
                    VideoOSD_SetLogo:
                        lock       ← OSD模块自己加锁
                        ...
                        unlock     ← OSD模块自己解锁
    ↓
    Step 4: Start encoder
```

**优点：**
1. ✅ 锁的粒度最小（只锁OSD操作）
2. ✅ 职责清晰（OSD锁只保护OSD，编码器不关心）
3. ✅ 模块独立（OSD模块不需要知道外层是否有锁）
4. ✅ 易于理解（谁加锁谁解锁）

---

## 总结

### 原来的设计（糟糕）
- ❌ 外部加锁（VideoEncoder_SetFormat）
- ❌ 内部多次解锁（VideoOSD_SetTitleRegion + 错误路径）
- ❌ 职责混乱（编码器锁OSD）
- ❌ 跨模块传递锁责任

### 重构后的设计（正确）
- ✅ 每个模块自己管理锁
- ✅ 谁加锁谁解锁（在同一个函数内）
- ✅ 最小锁范围（只锁必要的临界区）
- ✅ 职责清晰（osd_lock只保护OSD操作）

### 需要修改的地方
1. ❌ 删除 `video_encoder.c:724` 的 `pthread_mutex_lock(&osd_lock)`
2. ❌ 删除 `video_encoder.c:728, 744, 754, 760` 的 `pthread_mutex_unlock(&osd_lock)`
3. ❌ 删除 `video_osd.c:804` 的孤立 `pthread_mutex_unlock(&osd_lock)`
4. ✅ 保留 OSD 模块内部的所有 lock/unlock 配对
