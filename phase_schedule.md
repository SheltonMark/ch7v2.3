# Video Pipeline 模块化重构完整方案

## 一、架构设计原则

1. **单一职责原则** - 每个模块只负责一个子系统
2. **接口抽象** - 所有SDK调用必须通过adapter
3. **可复用性** - 模块间可以互相调用公共接口
4. **向后兼容** - Video.h保持不变，应用层无需修改
5. **数据驱动** - 用配置数据消除特殊case

---

## 二、最终目录结构

```
libmedia/Src/video/
├── adapter/                           ← 平台抽象层
│   ├── ax/
│   │   └── ax_adapter.c              ← SDK调用实现（AX520CE平台）
│   ├── include/
│   │   └── platform_adapter.h        ← adapter接口定义
│   └── adapter_factory.c             ← adapter工厂（已有）
│
├── core/                              ← 核心管理层（新建）
│   ├── video_pipeline.c              ← 视频管道初始化（管理流程）
│   └── include/
│       └── video_pipeline.h          ← 初始化接口定义
│
├── modules/                           ← 业务功能模块（新建）
│   ├── video_encoder.c               ← VENC编码模块
│   ├── video_vps.c                   ← VPS缩放/处理模块
│   ├── video_osd.c                   ← OSD叠加模块（重命名自video_overlay.c）
│   ├── video_input.c                 ← VI/ISP输入模块
│   ├── video_adaptive.c              ← 自适应降帧模块
│   └── include/
│       ├── video_encoder.h
│       ├── video_vps.h
│       ├── video_osd.h
│       ├── video_input.h
│       └── video_adaptive.h
│
├── video.c                            ← 原有文件，逐步变成转发层
├── video_api.c                        ← 最终的纯转发层（Phase 7创建）
└── include/
    └── video_internal.h               ← 内部数据结构定义
```

### 目录职责说明

| 目录/文件 | 职责 | 特点 |
|----------|------|------|
| `adapter/` | 平台抽象层 | 隔离SDK调用，平台移植只改这里 |
| `core/video_pipeline.c` | 初始化流程管理 | 协调各模块初始化顺序，管理VI→VPS→VENC创建和绑定 |
| `modules/video_encoder.c` | VENC编码功能 | 可被adaptive模块复用，提供动态参数接口 |
| `modules/video_osd.c` | OSD叠加功能 | 独立功能模块，时间戳/通道名/图形叠加 |
| `modules/video_adaptive.c` | 自适应降帧 | 复用encoder/vps接口，不直接调用SDK |

---

## 三、模块划分与职责

### Phase 2: video_encoder.c - VENC编码模块

**负责**: 视频编码相关的所有操作

**公共接口**（可被其他模块复用）:

```c
// ========== video_encoder.h ==========

/* 初始化接口 */
int VideoEncoder_CreateChannel(int VencChn, VENC_CHN_ATTR_S* pAttr);
int VideoEncoder_DestroyChannel(int VencChn);
int VideoEncoder_Start(int VencChn);
int VideoEncoder_Stop(int VencChn);

/* 动态参数设置（可被adaptive模块复用） */
int VideoEncoder_SetFrameRate(int VencChn, int fps);
int VideoEncoder_SetBitRate(int VencChn, int bitrate_kbps);
int VideoEncoder_SetQP(int VencChn, int minQp, int maxQp, int minIQp, int maxIQp);
int VideoEncoder_SetGOP(int VencChn, int gop);
int VideoEncoder_SetRcMode(int VencChn, VENC_RC_MODE_E rcMode);
int VideoEncoder_SetResolution(int VencChn, int width, int height);

/* 编码控制 */
int VideoEncoder_RequestIDR(int VencChn);
int VideoEncoder_EnableIntraRefresh(int VencChn, int enable);

/* 属性获取 */
int VideoEncoder_GetChannelAttr(int VencChn, VENC_CHN_ATTR_S* pAttr);
int VideoEncoder_GetStreamInfo(int VencChn, VENC_STREAM_S* pStream);

/* 旋转/镜像（特殊功能） */
int VideoEncoder_SetRotate(int VencChn, ROTATION_E enRotation);
int VideoEncoder_SetMirror(int VencChn, int bMirror, int bFlip);
```

**内部实现要点**:
- 所有操作通过`adapter->venc_*`接口
- 参数变更使用`Stop → Get → Modify → Set → Start`模式
- 不直接调用任何NI_MDK_*或SDK_COMM_*函数

**对应原video.c接口**:
- `VideoSetFormat()` → `VideoEncoder_SetFrameRate/BitRate/Resolution`
- `VideoSetQP()` → `VideoEncoder_SetQP`
- `VideoSetRotate()` → `VideoEncoder_SetRotate`
- `VideoSetMirrorAndflip()` → `VideoEncoder_SetMirror`
- `VideoSetGop()` → `VideoEncoder_SetGOP`
- `VideoRequestIDR()` → `VideoEncoder_RequestIDR`

---

### Phase 3: video_osd.c - OSD叠加模块

**负责**: 时间戳、通道名、自定义图形叠加

**公共接口**:

```c
// ========== video_osd.h ==========

/* 区域管理 */
int VideoOSD_CreateRegion(int RegionHandle, OSD_REGION_ATTR_S* pAttr);
int VideoOSD_DestroyRegion(int RegionHandle);
int VideoOSD_AttachToChannel(int RegionHandle, int VencChn);
int VideoOSD_DetachFromChannel(int RegionHandle, int VencChn);

/* 显示控制 */
int VideoOSD_ShowRegion(int RegionHandle);
int VideoOSD_HideRegion(int RegionHandle);
int VideoOSD_SetDisplayAttr(int RegionHandle, OSD_DISP_ATTR_S* pAttr);
int VideoOSD_GetDisplayAttr(int RegionHandle, OSD_DISP_ATTR_S* pAttr);

/* 内容绘制 */
int VideoOSD_DrawText(int RegionHandle, const char* text, OSD_TEXT_ATTR_S* pAttr);
int VideoOSD_DrawBitmap(int RegionHandle, BITMAP_S* pBitmap);
int VideoOSD_Clear(int RegionHandle);

/* 时间戳（高层封装） */
int VideoOSD_EnableTimestamp(int VencChn, OSD_TIMESTAMP_CONFIG_S* pConfig);
int VideoOSD_DisableTimestamp(int VencChn);
int VideoOSD_UpdateTimestamp(int VencChn); // 定时刷新

/* 通道名称（高层封装） */
int VideoOSD_SetChannelName(int VencChn, const char* name, OSD_TEXT_ATTR_S* pAttr);
```

**内部实现要点**:
- 所有操作通过`adapter->osd_*`接口
- 时间戳刷新可以独立线程，也可以被adaptive模块调用
- 支持多个Region同时叠加到一个通道

**对应原video.c接口**:
- `VideoSetTimeOSDEnable()` → `VideoOSD_EnableTimestamp`
- `VideoSetTimeOsdPosition()` → `VideoOSD_SetDisplayAttr`
- `VideoSetChannelNameOSDEnable()` → `VideoOSD_SetChannelName`
- `VideoDrawGraphToOSD()` → `VideoOSD_DrawBitmap`

---

### Phase 3: video_vps.c - VPS缩放处理模块

**负责**: 视频缩放、裁剪、格式转换

**公共接口**:

```c
// ========== video_vps.h ==========

/* Group管理 */
int VideoVPS_CreateGroup(int VpsGrp, VPS_GRP_ATTR_S* pAttr);
int VideoVPS_DestroyGroup(int VpsGrp);
int VideoVPS_StartGroup(int VpsGrp);
int VideoVPS_StopGroup(int VpsGrp);

/* Channel管理 */
int VideoVPS_CreateChannel(int VpsGrp, int VpsChn, VPS_CHN_ATTR_S* pAttr);
int VideoVPS_DestroyChannel(int VpsGrp, int VpsChn);
int VideoVPS_EnableChannel(int VpsGrp, int VpsChn);
int VideoVPS_DisableChannel(int VpsGrp, int VpsChn);

/* 动态参数（可被adaptive模块复用） */
int VideoVPS_SetChannelAttr(int VpsGrp, int VpsChn, VPS_CHN_ATTR_S* pAttr);
int VideoVPS_GetChannelAttr(int VpsGrp, int VpsChn, VPS_CHN_ATTR_S* pAttr);
int VideoVPS_SetOutputFrameRate(int VpsGrp, int VpsChn, int fps);
int VideoVPS_SetOutputResolution(int VpsGrp, int VpsChn, int width, int height);

/* 裁剪功能 */
int VideoVPS_SetCropRect(int VpsGrp, int VpsChn, RECT_S* pCropRect);
int VideoVPS_GetCropRect(int VpsGrp, int VpsChn, RECT_S* pCropRect);
```

**内部实现要点**:
- 所有操作通过`adapter->vps_*`接口
- AX520CE平台固定使用VpsGrp=0
- 提供给adaptive模块的帧率/分辨率调整接口

**对应原video.c接口**:
- `_video_set_vps_out_param()` → `VideoVPS_SetChannelAttr`
- `_video_set_vps_in_frame_rate()` → `VideoVPS_SetOutputFrameRate`

---

### Phase 4: video_input.c - VI/ISP输入模块

**负责**: 视频输入、ISP控制、Sensor配置

**公共接口**:

```c
// ========== video_input.h ==========

/* VI通道管理 */
int VideoInput_CreateChannel(int ViChn, VI_CHN_ATTR_S* pAttr);
int VideoInput_DestroyChannel(int ViChn);
int VideoInput_EnableChannel(int ViChn);
int VideoInput_DisableChannel(int ViChn);

/* ISP控制 */
int VideoInput_SetISPAttr(int ViChn, ISP_ATTR_S* pAttr);
int VideoInput_GetISPAttr(int ViChn, ISP_ATTR_S* pAttr);
int VideoInput_SetAutoFPS(int ViChn, int enable);
int VideoInput_GetAutoFPS(int ViChn, int* pEnable);

/* Sensor控制 */
int VideoInput_SetSensorFrameRate(int ViChn, int fps);
int VideoInput_GetSensorFrameRate(int ViChn, int* pFps);
int VideoInput_SetVideoFormat(int ViChn, VIDEO_FMT_ATTR_S* pFmt);
int VideoInput_GetVideoFormat(int ViChn, VIDEO_FMT_ATTR_S* pFmt);

/* 图像增强 */
int VideoInput_SetBrightness(int ViChn, int value);
int VideoInput_SetContrast(int ViChn, int value);
int VideoInput_SetSaturation(int ViChn, int value);
int VideoInput_SetSharpness(int ViChn, int value);
```

**内部实现要点**:
- 所有操作通过`adapter->vi_*`和`adapter->isp_*`接口
- ISP参数修改可能影响所有通道（共享sensor）

**对应原video.c接口**:
- `VideoInSetAutoFps()` → `VideoInput_SetAutoFPS`
- `VideoInGetAutoFps()` → `VideoInput_GetAutoFPS`
- `VideoInSetVideoFmtAttr()` → `VideoInput_SetVideoFormat`
- `VideoInGetVideoFmtAttr()` → `VideoInput_GetVideoFormat`

---

### Phase 3-4: video_adaptive.c - 自适应降帧模块

**负责**: 夜间降帧、运动检测、用户参数变更

**公共接口**:

```c
// ========== video_adaptive.h ==========

/* 配置结构 */
typedef struct {
    int enabled;

    /* 夜间降帧配置 */
    struct {
        int enabled;
        int night_fps;      // 夜间帧率（如5fps）
        int day_fps;        // 白天帧率（如25fps）
        int start_hour;     // 降帧开始时间（如22点）
        int end_hour;       // 降帧结束时间（如6点）
    } night_mode;

    /* 运动检测配置 */
    struct {
        int enabled;
        int idle_fps;       // 无运动时帧率（如10fps）
        int active_fps;     // 有运动时帧率（如25fps）
        int idle_timeout;   // 无运动多久降帧（秒）
    } motion_detect;

} VideoAdaptive_Config_S;

/* 初始化 */
int VideoAdaptive_Init(VideoAdaptive_Config_S* pConfig);
int VideoAdaptive_Start(void);
int VideoAdaptive_Stop(void);
int VideoAdaptive_Deinit(void);

/* 参数变更队列 */
int VideoAdaptive_QueueFrameRateChange(int channel, int fps);
int VideoAdaptive_QueueBitRateChange(int channel, int bitrate);
int VideoAdaptive_QueueResolutionChange(int channel, int width, int height);

/* 运行时配置更新 */
int VideoAdaptive_UpdateConfig(VideoAdaptive_Config_S* pConfig);
int VideoAdaptive_GetStatus(VideoAdaptive_Status_S* pStatus);
```

**内部实现要点**:
- **复用video_encoder.c接口**: `VideoEncoder_SetFrameRate`, `VideoEncoder_SetBitRate`
- **复用video_vps.c接口**: `VideoVPS_SetOutputFrameRate`
- 独立线程处理，使用队列避免并发问题
- 不直接调用adapter，全部通过上层模块接口

**对应原video.c接口**:
- `_venc_adaptive_process()` → `VideoAdaptive_Process()`（内部线程）
- `_video_set_vps_out_param()` → 复用`VideoVPS_*`接口
- `_video_set_venc_param()` → 复用`VideoEncoder_*`接口

---

## 四、adapter接口扩展

需要在`platform_adapter.h`中添加以下接口支持动态参数修改：

```c
typedef struct {
    // ===== 已有接口（Phase 1） =====
    int (*venc_create)(int VencChn, VENC_CHN_ATTR_S* pAttr);
    int (*venc_destroy)(int VencChn);
    int (*venc_start)(int VencChn);
    int (*venc_stop)(int VencChn);
    // ... 其他已有接口

    // ===== Phase 2: VENC动态参数接口 =====
    int (*venc_get_chn_attr)(int VencChn, VENC_CHN_ATTR_S* pAttr);
    int (*venc_set_chn_attr)(int VencChn, VENC_CHN_ATTR_S* pAttr);
    int (*venc_get_rc_param)(int VencChn, VENC_RC_PARAM_S* pParam);
    int (*venc_set_rc_param)(int VencChn, VENC_RC_PARAM_S* pParam);
    int (*venc_request_idr)(int VencChn);
    int (*venc_set_rotate)(int VencChn, ROTATION_E enRotation);

    // ===== Phase 3: VPS动态参数接口 =====
    int (*vps_get_grp_attr)(int VpsGrp, VPS_GRP_ATTR_S* pAttr);
    int (*vps_set_grp_attr)(int VpsGrp, VPS_GRP_ATTR_S* pAttr);
    int (*vps_get_chn_attr)(int VpsGrp, int VpsChn, VPS_CHN_ATTR_S* pAttr);
    int (*vps_set_chn_attr)(int VpsGrp, int VpsChn, VPS_CHN_ATTR_S* pAttr);
    int (*vps_set_crop)(int VpsGrp, int VpsChn, RECT_S* pCropRect);
    int (*vps_get_crop)(int VpsGrp, int VpsChn, RECT_S* pCropRect);

    // ===== Phase 3: OSD接口 =====
    int (*osd_create_region)(int Handle, OSD_REGION_ATTR_S* pAttr);
    int (*osd_destroy_region)(int Handle);
    int (*osd_attach_to_chn)(int Handle, MDK_CHN_S* pChn);
    int (*osd_detach_from_chn)(int Handle, MDK_CHN_S* pChn);
    int (*osd_set_disp_attr)(int Handle, OSD_DISP_ATTR_S* pAttr);
    int (*osd_get_disp_attr)(int Handle, OSD_DISP_ATTR_S* pAttr);
    int (*osd_update_content)(int Handle, void* pContent, int size);

    // ===== Phase 4: VI/ISP接口 =====
    int (*vi_get_chn_attr)(int ViChn, VI_CHN_ATTR_S* pAttr);
    int (*vi_set_chn_attr)(int ViChn, VI_CHN_ATTR_S* pAttr);
    int (*isp_get_auto_fps)(int ViChn, int* pEnable);
    int (*isp_set_auto_fps)(int ViChn, int enable);
    int (*isp_get_video_fmt)(int ViChn, VIDEO_FMT_ATTR_S* pFmt);
    int (*isp_set_video_fmt)(int ViChn, VIDEO_FMT_ATTR_S* pFmt);

} PlatformAdapter;
```

---

## 五、公共API层转发（video_api.c）

替代原来4107行的video.c，只做纯转发：

```c
// ========== video_api.c ==========
#include "video_encoder.h"
#include "video_vps.h"
#include "video_osd.h"
#include "video_input.h"
#include "video_adaptive.h"

/* Video.h对外接口实现 - 纯转发 */

int VideoSetFormat(int channel, DWORD dwType, VIDEO_FORMAT *pFormat)
{
    int ret = 0;

    // 转发到对应模块
    if (pFormat->BitRate > 0) {
        ret |= VideoEncoder_SetBitRate(channel, pFormat->BitRate);
    }

    if (pFormat->FramesPerSecond > 0) {
        ret |= VideoEncoder_SetFrameRate(channel, pFormat->FramesPerSecond);
    }

    if (pFormat->Width > 0 && pFormat->Height > 0) {
        ret |= VideoEncoder_SetResolution(channel, pFormat->Width, pFormat->Height);
    }

    return ret;
}

int VideoSetQP(int channel, DWORD dwType, int minQp, int maxQp,
               int minIQp, int maxIQp)
{
    return VideoEncoder_SetQP(channel, minQp, maxQp, minIQp, maxIQp);
}

int VideoSetRotate(int channel, DWORD dwType, int nRotate)
{
    return VideoEncoder_SetRotate(channel, (ROTATION_E)nRotate);
}

int VideoSetMirrorAndflip(int channel, DWORD dwType, int bMirror, int bFlip)
{
    return VideoEncoder_SetMirror(channel, bMirror, bFlip);
}

int VideoSetTimeOSDEnable(int channel, DWORD dwType, int bEnable)
{
    if (bEnable) {
        OSD_TIMESTAMP_CONFIG_S config = {
            .format = "%Y-%m-%d %H:%M:%S",
            .x = 10, .y = 10,
            .font_size = 32,
            .color = 0xFFFFFF
        };
        return VideoOSD_EnableTimestamp(channel, &config);
    } else {
        return VideoOSD_DisableTimestamp(channel);
    }
}

// ... 其他42个接口类似转发
```

---

## 六、Phase 2-7 实施路线图

### **Phase 2: VENC模块重构**

**目标**: 创建video_encoder.c，消灭所有VENC相关的直接SDK调用

**工作内容**:
1. 创建`modules/video_encoder.c`和`modules/include/video_encoder.h`
2. 在`ax_adapter.c`中实现VENC动态参数接口（13个函数）
3. 迁移以下video.c接口到video_encoder.c:
   - `VideoSetFormat()` - 最常用，优先级最高
   - `VideoSetQP()`
   - `VideoSetRotate()`
   - `VideoSetMirrorAndflip()`
   - `VideoSetGop()`
   - `VideoRequestIDR()`
   - `VideoSetIntraRefreshEnable()`
4. 修改video.c对应函数为转发调用
5. 编译测试，确保功能正常

**验收标准**:
- video_encoder.c中无任何NI_MDK_*或SDK_COMM_*调用
- 所有VENC相关接口测试通过
- video.c减少约600行

---

### **Phase 3: OSD + VPS模块重构**

**目标**: 创建video_osd.c和video_vps.c

**工作内容**:
1. 创建`modules/video_osd.c`和对应头文件
2. 在`ax_adapter.c`中实现OSD接口（7个函数）
3. 迁移OSD相关接口:
   - `VideoSetTimeOSDEnable()`
   - `VideoSetTimeOsdPosition()`
   - `VideoSetChannelNameOSDEnable()`
   - `VideoDrawGraphToOSD()`
   - ... (共18个OSD接口)
4. 创建`modules/video_vps.c`和对应头文件
5. 在`ax_adapter.c`中实现VPS动态参数接口（6个函数）
6. 迁移VPS相关内部函数:
   - `_video_set_vps_out_param()` → `VideoVPS_SetChannelAttr`
   - `_video_set_vps_in_frame_rate()` → `VideoVPS_SetOutputFrameRate`

**验收标准**:
- video_osd.c和video_vps.c中无SDK调用
- OSD显示正常（时间戳、通道名）
- video.c减少约1000行

---

### **Phase 4: VI/ISP模块 + Adaptive模块重构**

**目标**: 创建video_input.c和video_adaptive.c

**工作内容**:
1. 创建`modules/video_input.c`
2. 在`ax_adapter.c`中实现VI/ISP接口（5个函数）
3. 迁移VI/ISP接口:
   - `VideoInSetAutoFps()`
   - `VideoInGetAutoFps()`
   - `VideoInSetVideoFmtAttr()`
   - `VideoInGetVideoFmtAttr()`
4. 创建`modules/video_adaptive.c`
5. 重构`_venc_adaptive_process()`线程:
   - 使用`VideoEncoder_*`接口替代直接SDK调用
   - 使用`VideoVPS_*`接口
   - 实现参数变更队列机制

**验收标准**:
- video_input.c和video_adaptive.c中无SDK调用
- 夜间降帧功能正常
- 用户参数变更功能正常
- video.c减少约800行

---

### **Phase 5: 数据结构重组**

**目标**: 消除GlobalDevice滥用，建立清晰的数据所有权

**工作内容**:
1. 定义模块级数据结构:
   ```c
   // video_encoder.c
   typedef struct {
       VENC_CHN_ATTR_S attr;
       int running;
       pthread_mutex_t lock;
   } EncoderChannel;

   static EncoderChannel g_encoders[MAX_VENC_CHN];
   ```
2. 将GlobalDevice中的字段迁移到对应模块
3. 提供数据访问接口而非直接暴露全局变量
4. 添加锁保护避免并发问题

**验收标准**:
- GlobalDevice仅在初始化时使用
- 每个模块有独立的数据管理
- 无数据竞争问题

---

### **Phase 6: 特殊case消除**

**目标**: 用数据驱动配置替代硬编码的if-else

**工作内容**:
1. 识别所有平台特定代码（如VpsGrp=0的硬编码）
2. 创建平台配置结构:
   ```c
   typedef struct {
       int vps_grp_shared;  // AX520CE: 1, 其他平台: 0
       int max_venc_chn;
       int support_rotate;
       // ...
   } PlatformConfig;
   ```
3. 用配置表替代switch-case
4. 简化if-else嵌套

**验收标准**:
- 减少50%以上的条件分支
- 平台差异由配置表驱动
- 代码可读性显著提升

---

### **Phase 7: 测试与清理**

**目标**: 删除video.c，完成重构

**工作内容**:
1. 将video.c剩余的转发函数迁移到video_api.c
2. 删除video.c文件
3. 全功能回归测试:
   - 初始化流程
   - 编码出图
   - 参数动态修改
   - OSD显示
   - 降帧功能
4. 性能测试（对比重构前后）
5. 内存泄漏检查
6. 更新文档

**验收标准**:
- video.c已删除
- 所有功能测试通过
- 性能无明显下降
- 无内存泄漏
- 代码行数减少50%以上

---

## 七、模块间调用关系图

```
┌─────────────────────────────────────────────────────────┐
│  应用层 (Apps/)                                          │
│  VideoSetFormat(), VideoSetQP(), ...                    │
└─────────────────────────────────────────────────────────┘
                          ↓
┌─────────────────────────────────────────────────────────┐
│  video_api.c (转发层)                                    │
│  纯转发，无业务逻辑                                       │
└─────────────────────────────────────────────────────────┘
                          ↓
┌────────────────┬────────────────┬────────────────┬───────┐
│ video_encoder  │  video_vps     │  video_osd     │ video │
│      .c        │      .c        │      .c        │ _input│
│                │                │                │   .c  │
│ - SetFrameRate │ - SetChnAttr   │ - DrawText     │ - Set │
│ - SetBitRate   │ - SetCrop      │ - DrawBitmap   │  Auto │
│ - SetQP        │ - SetFps       │ - EnableTime   │  Fps  │
└────────┬───────┴────────┬───────┴────────┬───────┴───┬───┘
         │                │                │           │
         └────────────────┼────────────────┼───────────┘
                          ↓                ↓
                  ┌──────────────────────────────┐
                  │  video_adaptive.c            │
                  │  复用上面模块的接口:          │
                  │  - VideoEncoder_SetFrameRate │
                  │  - VideoVPS_SetOutputFps     │
                  │  - VideoEncoder_SetBitRate   │
                  └──────────────────────────────┘
                          ↓
         ┌────────────────┴────────────────┐
         ↓                                 ↓
┌─────────────────┐              ┌──────────────────┐
│ platform_adapter│              │ video_pipeline   │
│       .h        │              │       .c         │
│ (接口定义)       │              │ (初始化)          │
└─────────────────┘              └──────────────────┘
         ↓
┌─────────────────────────────────────────┐
│  ax_adapter.c                           │
│  唯一调用SDK的地方:                      │
│  - NI_MDK_VENC_*                        │
│  - SDK_COMM_VENC_*                      │
│  - NI_MDK_VPS_*                         │
│  - ...                                  │
└─────────────────────────────────────────┘
```

---

## 八、重构前后对比

| 指标 | 重构前 | 重构后 |
|------|--------|--------|
| video.c行数 | 4107行 | 删除（替换为video_api.c ~200行） |
| SDK直接调用 | 遍布video.c | 仅在ax_adapter.c |
| 模块数量 | 1个巨型文件 | 6个独立模块 |
| 可测试性 | 困难（高耦合） | 容易（模块独立） |
| 代码复用 | 大量重复 | 模块间可复用 |
| 平台移植 | 修改video.c | 仅修改ax_adapter.c |
| 特殊case | 50+个if-else | 配置表驱动 |
| 数据结构 | GlobalDevice滥用 | 模块私有数据 |
| 向后兼容 | - | 100%兼容 |

---

## 九、风险控制

1. **增量迁移** - 每个Phase独立，可随时回退
2. **双路径验证** - 保留原video.c，新旧代码并行运行对比
3. **自动化测试** - 每个Phase后运行完整测试套件
4. **代码审查** - 每个模块完成后review
5. **性能监控** - 对比重构前后的CPU/内存占用

---

## 十、核心思想总结

1. **Video.h对外不变** - 应用层不需要改一行代码
2. **video.c → video_api.c** - 变成薄薄的转发层
3. **业务逻辑全部在modules/** - 6个独立模块各司其职
4. **模块间可复用** - adaptive调用encoder/vps的接口
5. **SDK调用只在ax_adapter.c** - 平台移植只改这一个文件

最终实现"**Never break userspace**"同时把4107行怪兽彻底消灭！
