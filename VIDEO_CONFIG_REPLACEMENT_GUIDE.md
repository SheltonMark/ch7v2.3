# video_config 模块替换操作指南

## ✅ 已完成的工作

### 1. osd_lock 修复（已完成）
- ✅ 删除 video_encoder.c 中所有 osd_lock 操作（5处）
- ✅ 删除 video_osd.c:804 的孤立 unlock
- ✅ 删除 video_encoder.c:60 的 `extern pthread_mutex_t osd_lock;`

---

## 📋 待完成的替换工作

### 文件1: capture/capture.c

**状态：**
- ✅ 已添加 `#include "modules/include/video_config.h"` (line 9)
- ✅ 已删除 `extern unsigned int gFrameRate;`
- ✅ 已删除 `extern unsigned int sensor_fps;`

**待替换：15处 gFrameRate 使用**

#### 方案1：函数级缓存（推荐，性能最优）

在函数 `_capture_head_to_packets` (line 109) 开头添加：

```c
int _capture_head_to_packets(int Chn, StreamType_e Streamtype, AV_SLICE_TYPE_e AVType, void *buffer,int StreamLen
	, PRI_STREAM_PACK_p PriStreamPack,AV_SLICE_HEADER_p pAvHeader,AV_SLICE_TAILS_p pAvTail, int PacketSize, int *PacketLength, int *DataLength, SLICE_LOCATION_t *pSliceLocation
	,uint64 pts)
{
	// 在函数开头添加这一行
	unsigned int current_fps = VideoConfig_GetFrameRate();

	// 原有代码...
	int PriStreamPackCnt = 1;
	SLICE_LOCATION_t SliceLocation;
	// ...
```

然后使用查找替换功能（注意：只在 `_capture_head_to_packets` 函数内替换）：
- 查找：`gFrameRate`
- 替换为：`current_fps`
- 范围：line 109 - line 1200（函数结束位置）

#### 方案2：直接替换（简单但性能略差）

使用查找替换功能（全文件范围）：
- 查找：`gFrameRate`
- 替换为：`VideoConfig_GetFrameRate()`

**受影响的行号：**
```
260, 262, 279, 281, 355, 357, 372, 374, 482, 484, 499, 501, 570, 572, 588, 590
```

**替换示例：**
```c
// 替换前（line 260-262）
if ((gFrameRate < pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].Format.FramesPerSecond) && (0 != gFrameRate))
{
    pAvHeader->ExternHeader.H264PSlice.VFps = gFrameRate;
}

// 方案1替换后
if ((current_fps < pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].Format.FramesPerSecond) && (0 != current_fps))
{
    pAvHeader->ExternHeader.H264PSlice.VFps = current_fps;
}

// 方案2替换后
if ((VideoConfig_GetFrameRate() < pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].Format.FramesPerSecond) && (0 != VideoConfig_GetFrameRate()))
{
    pAvHeader->ExternHeader.H264PSlice.VFps = VideoConfig_GetFrameRate();
}
```

---

### 文件2: video/modules/video_encoder.c

**状态：**
- ✅ 已删除 `extern pthread_mutex_t osd_lock;` (原line 60)
- ❌ 未添加 `#include "modules/include/video_config.h"`
- ❌ 还有2个 extern 未删除

#### Step 1: 添加头文件

在 line 25 后添加：
```c
#include "../modules/include/video_osd.h"
#include "platform_adapter.h"
#include "modules/include/video_config.h"  // ← 添加这一行
#include "PrintGrade.h"
```

#### Step 2: 删除 extern 声明

删除 line 50-57：
```c
// 删除这些行
extern SIZE_S imageSize[2][VIDEO_SIZE_NR];

// QP tables from video.c
extern CaptureImageQuality_t CaptureQtTable[6];
extern CaptureImageQuality_t CHL_2END_T_CaptureQtTable[6];

// Sensor frame rate
extern unsigned int sensor_fps;
```

#### Step 3: 替换使用点

**3.1 搜索 `imageSize[` 的所有使用**

命令：`grep -n "imageSize\[" video_encoder.c`

对于每处使用：
```c
// 替换前
imageSize[vstd][size].u32Width

// 替换后
VideoConfig_GetImageSize(vstd, size)->u32Width
```

**注意：** `VideoConfig_GetImageSize()` 返回 `const SIZE_S*`，所以：
- `imageSize[vstd][size].u32Width` → `VideoConfig_GetImageSize(vstd, size)->u32Width`
- `imageSize[vstd][size].u32Height` → `VideoConfig_GetImageSize(vstd, size)->u32Height`

**3.2 搜索 `sensor_fps` 的所有使用**

命令：`grep -n "sensor_fps" video_encoder.c`

对于每处使用：
```c
// 读取
sensor_fps → VideoConfig_GetSensorFps()

// 写入（如果有）
sensor_fps = 30; → VideoConfig_SetSensorFps(30);
```

**3.3 搜索 `CaptureQtTable` 和 `CHL_2END_T_CaptureQtTable`**

这两个是质量表，已经在 video_config.c 中定义，使用接口：

**主码流质量表替换：**
```c
// 替换前
CaptureQtTable[level].ImaxQP

// 替换后
VideoConfig_GetQualityTable(level)->ImaxQP
```

**子码流质量表替换：**
```c
// 替换前
CHL_2END_T_CaptureQtTable[level].ImaxQP

// 替换后
VideoConfig_GetSubstreamQualityTable(level)->ImaxQP
```

**接口说明：**
1. `VideoConfig_GetQualityTable(level)` - 主码流质量表，返回 `const CaptureImageQuality_t*`
2. `VideoConfig_GetSubstreamQualityTable(level)` - 子码流质量表（✅ 已补充），返回 `const CaptureImageQuality_t*`
3. 两者都支持 level 范围 0-5，返回值使用 `->` 访问成员（ImaxQP, IminQP, PmaxQP, PminQP）

---

### 文件3: video/modules/video_frame_rate.c

#### Step 1: 添加头文件 + 删除 extern

查找：
```c
extern unsigned int gFrameRate;
extern unsigned int sensor_fps;
```

替换为：
```c
#include "include/video_config.h"
```

#### Step 2: 替换所有使用

**2.1 查找 `gFrameRate`（读取）**
```c
// 替换前
u32CurFrame = (unsigned int)FrameRate;
if (u32CurFrame != gFrameRate)

// 替换后
u32CurFrame = (unsigned int)FrameRate;
if (u32CurFrame != VideoConfig_GetFrameRate())
```

**2.2 查找 `gFrameRate =`（写入）**
```c
// 替换前
gFrameRate = u32CurFrame;

// 替换后
VideoConfig_SetFrameRate(u32CurFrame);
```

**2.3 查找 `sensor_fps`（读取）**
```c
sensor_fps → VideoConfig_GetSensorFps()
```

**2.4 查找 `sensor_fps =`（写入）**
```c
sensor_fps = xxx; → VideoConfig_SetSensorFps(xxx);
```

**快速替换命令（使用sed或编辑器）：**
```bash
# 读取替换
s/\bgFrameRate\b/VideoConfig_GetFrameRate()/g
s/\bsensor_fps\b/VideoConfig_GetSensorFps()/g

# 写入替换（需要手动检查）
s/VideoConfig_GetFrameRate() = /VideoConfig_SetFrameRate(/g
s/VideoConfig_GetSensorFps() = /VideoConfig_SetSensorFps(/g
```

---

### 文件4: video/modules/video_osd.c

#### Step 1: 添加头文件 + 删除 extern

查找：
```c
extern pthread_mutex_t osd_lock;
extern SIZE_S imageSize[2][VIDEO_SIZE_NR];
```

替换为：
```c
#include "include/video_config.h"
```

#### Step 2: 替换 pthread_mutex_lock/unlock

**查找所有 `pthread_mutex_lock(&osd_lock)`：**
```bash
grep -n "pthread_mutex_lock(&osd_lock)" video_osd.c
```

替换为：
```c
VideoConfig_LockOsd();
```

**查找所有 `pthread_mutex_unlock(&osd_lock)`：**
```bash
grep -n "pthread_mutex_unlock(&osd_lock)" video_osd.c
```

替换为：
```c
VideoConfig_UnlockOsd();
```

**注意：** line 804 的孤立 unlock 已经删除，其他的应该都是配对的。

#### Step 3: 替换 imageSize

查找所有 `imageSize[` 的使用：
```bash
grep -n "imageSize\[" video_osd.c
```

替换规则：
```c
// 替换前
imageSize[vstd][size].u32Width

// 替换后
VideoConfig_GetImageSize(vstd, size)->u32Width
```

---

### 文件5: video/modules/video_vps.c

#### Step 1: 添加头文件 + 删除 extern

查找：
```c
extern SIZE_S imageSize[2][VIDEO_SIZE_NR];
```

替换为：
```c
#include "include/video_config.h"
```

#### Step 2: 替换 imageSize

查找所有 `imageSize[` 的使用：
```bash
grep -n "imageSize\[" video_vps.c
```

替换规则：
```c
imageSize[vstd][size].u32Width → VideoConfig_GetImageSize(vstd, size)->u32Width
imageSize[vstd][size].u32Height → VideoConfig_GetImageSize(vstd, size)->u32Height
```

---

### 文件6: video/video.c - 删除全局变量定义

#### 删除以下内容：

**1. 帧率变量（line 12-13）**
```c
unsigned int gFrameRate = 30;
unsigned int sensor_fps = 30;
```

**2. OSD锁（line 15）**
```c
pthread_mutex_t osd_lock;
```

**3. 字体表（line 19）**
```c
int FontSiZeTable[] = {64, 32, 16};
```

**4. 质量表（line 22-60，约40行）**
```c
CaptureImageQuality_t CaptureQtTable[6] = {
    // ... 删除整个数组定义
};
```

**5. 子码流质量表（line 62-100，约40行）**
```c
CaptureImageQuality_t CHL_2END_T_CaptureQtTable[6] = {
    // ... 删除整个数组定义
};
```

**6. 分辨率查找表（line 102-178，约76行）**
```c
SIZE_S imageSize[2][VIDEO_SIZE_NR] = {
    // ... 删除整个数组定义
};
```

**删除范围：大约 line 12 - line 178（共约166行）**

**保留的内容：**
- 头文件 include
- 函数实现（VideoCreate, VideoDestroy, VideoSetFormat 等）

---

### 文件7: media_com/td_common.c - 添加初始化

#### Step 1: 添加头文件

在文件开头的 include 区域添加：
```c
#include "../video/modules/include/video_config.h"
```

#### Step 2: 在 MediaHardWareInit() 中添加初始化

找到 `MediaHardWareInit()` 函数，在 `DeviceHalInit()` 之后添加：

```c
void MediaHardWareInit(int Normal)
{
    PRINT_TRACE("Normal  =%d\n",Normal);

    DeviceHalInit();

    // GPIO初始化...
    // ...

    /* ========== 初始化 video_config 模块 ========== */
    if (VideoConfig_Init() != 0) {
        PRINT_ERROR("VideoConfig_Init failed\n");
        return;
    }
    PRINT_INFO("VideoConfig_Init success\n");
    /* ============================================== */

    // 视频管道初始化
    if(VideoPipeline_Init() != HI_SUCCESS) {
        PRINT_ERROR("VideoPipeline_Init failed\n");
        return;
    }

    // 后续初始化...
}
```

**注意：** `VideoConfig_Init()` 必须在任何使用 video_config 接口的模块初始化之前调用。

---

## 🔧 快速替换技巧

### 使用 sed 批量替换（Linux/Git Bash）

```bash
cd Z:/ch7v2.3/libmedia/Src

# 文件1: capture.c - 方案2（直接替换）
sed -i 's/\bgFrameRate\b/VideoConfig_GetFrameRate()/g' capture/capture.c

# 文件3: video_frame_rate.c
sed -i 's/\bgFrameRate\b/VideoConfig_GetFrameRate()/g' video/modules/video_frame_rate.c
sed -i 's/\bsensor_fps\b/VideoConfig_GetSensorFps()/g' video/modules/video_frame_rate.c
# 手动检查写入操作（= 赋值）

# 文件4: video_osd.c
sed -i 's/pthread_mutex_lock(&osd_lock)/VideoConfig_LockOsd()/g' video/modules/video_osd.c
sed -i 's/pthread_mutex_unlock(&osd_lock)/VideoConfig_UnlockOsd()/g' video/modules/video_osd.c
```

### 使用 VS Code 查找替换

1. 打开 VS Code
2. 按 `Ctrl+H` 打开查找替换
3. 启用正则表达式模式（图标：`.*`）
4. 在"要包含的文件"中指定文件路径

**示例：**
- 查找：`\bgFrameRate\b`
- 替换为：`VideoConfig_GetFrameRate()`
- 文件：`capture/capture.c`

---

## 📊 替换工作量统计

| 文件 | extern 删除 | 使用点替换 | 头文件添加 | 预计时间 |
|------|-------------|-----------|-----------|---------|
| capture.c | ✅ 已完成 | 15处 gFrameRate | ✅ 已完成 | 5分钟 |
| video_encoder.c | 3个 | imageSize(多处) + sensor_fps(多处) + QTable(多处) | 1个 | 10分钟 |
| video_frame_rate.c | 2个 | gFrameRate(4-6处) + sensor_fps(2-3处) | 1个 | 5分钟 |
| video_osd.c | 2个 | osd_lock(10-15处) + imageSize(多处) | 1个 | 10分钟 |
| video_vps.c | 1个 | imageSize(3-5处) | 1个 | 3分钟 |
| video.c | 删除166行 | - | - | 2分钟 |
| td_common.c | - | 添加Init调用 | 1个 | 2分钟 |
| **总计** | **8处** | **约50处** | **5处** | **37分钟** |

---

## ⚠️ 注意事项

### 1. imageSize 替换特别注意

```c
// 错误：忘记改为指针访问
VideoConfig_GetImageSize(vstd, size).u32Width  // ❌ 编译错误

// 正确：返回指针，使用 -> 访问
VideoConfig_GetImageSize(vstd, size)->u32Width  // ✅
```

### 2. 赋值操作需要手动检查

```c
// 读取（可以自动替换）
int x = gFrameRate;  →  int x = VideoConfig_GetFrameRate();

// 写入（需要手动改为 Set 函数）
gFrameRate = 30;  →  VideoConfig_SetFrameRate(30);  // 不能自动替换！
```

### 3. 编译错误排查

如果编译时报错 `undefined reference to gFrameRate`：
- 说明还有文件使用了 extern，但未替换
- 使用 `grep -r "extern unsigned int gFrameRate" .` 查找

如果编译时报错 `osd_lock undeclared`：
- 说明还有文件使用了 osd_lock，但未替换
- 使用 `grep -r "osd_lock" .` 查找（排除 video_config.c）

### 4. 运行时错误排查

如果运行时段错误（Segmentation fault）：
- 检查是否调用了 `VideoConfig_Init()`
- 检查调用顺序：VideoConfig_Init() 必须在第一次使用前调用

---

## ✅ 完成后的验证

### 1. 编译检查
```bash
cd Z:/ch7v2.3/libmedia
make clean
make
```

### 2. 静态检查（确认没有残留）
```bash
# 检查是否还有 extern gFrameRate
grep -r "extern unsigned int gFrameRate" Src/

# 检查是否还有 extern sensor_fps
grep -r "extern unsigned int sensor_fps" Src/

# 检查是否还有 extern osd_lock
grep -r "extern pthread_mutex_t osd_lock" Src/

# 检查是否还有 extern imageSize
grep -r "extern SIZE_S imageSize" Src/
```

**预期结果：** 所有命令应该返回空（没有任何匹配）

### 3. 功能测试
- 测试视频编码是否正常
- 测试 OSD 显示是否正常
- 测试帧率切换是否正常
- 测试分辨率切换是否正常

---

## 📚 参考资料

- [video_config.h](Z:\ch7v2.3\libmedia\Src\video\modules\include\video_config.h) - 接口定义
- [video_config.c](Z:\ch7v2.3\libmedia\Src\video\modules\video_config.c) - 实现代码
- [OSD_LOCK_DEEP_ANALYSIS.md](Z:\ch7v2.3\libmedia\OSD_LOCK_DEEP_ANALYSIS.md) - osd_lock 分析
