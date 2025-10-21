/* * ni_private_venc.h ---- 私有数据定义头文件 */
#ifndef __NI_PRIVATE_VENC_H__
#define __NI_PRIVATE_VENC_H__

#include "ni_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef struct tagVencCuinfo {
    NI_U32 u32PicMbWidth;  /* 对齐后的图像16宏块宽度，图像对齐方式HEVC:64px，H264:16px */
    NI_U32 u32PicMbHeight; /* 对齐后的图像16宏块高度，图像对齐方式HEVC:64px，H264:16px */

    /* 每8个bytes代表32x32(hevc)or16x16(h264)的MMS,MV_Y[63:56],MV_X[55:47],STD[46:39],MAD1[38:32],MAD0[31:29],Inter[27],MSE[26:17],SATD[16:0] */
    NI_U32  u32MmsInfoPhyAddr;  /* MMS信息所在的物理地址 */
    NI_U32 *pu32MmsInfoVirAddr; /* MMS信息所在的虚拟地址 */
    NI_U32  u32MmsInfoSize;

    /* 每2个bytes代表32x32(hevc)or16x16(h264)的bitcost,内存中以小端格式摆放 */
    NI_U32  u32BitsInfoPhyAddr;  /* bitcost信息所在的物理地址 */
    NI_U16 *pu16BitsInfoVirAddr; /* bitcost信息所在的虚拟地址 */
    NI_U32  u32BitsInfoSize;

    /* 每1个bytes代表16x16的qp值 */
    NI_U32 u32QpInfoPhyAddr; /* qp信息所在的物理地址 */
    NI_U8 *pu8QpInfoVirAddr; /* qp信息所在的虚拟地址 */
    NI_U32 u32QpInfoSize;
} VENC_CU_INFO_S;

/* Cuinfo组成结构体 */
typedef struct tagVencCuOutData {
    NI_BOOL        bValid;  /* 当前码流CuInfo是否有效 */
    NI_BOOL        bIsH264; /* 是否是H264编码 */
    VENC_CU_INFO_S stCuinfo;
} VENC_CU_OUT_DATA_S;

typedef struct tagVencAdditionalInfo {
    NI_U32 u32PicQP;                    /* 输出帧级QP值。*/
    NI_U32 u32BsSizeAve;                /* 当前帧(包含)之前sliding_times时间内的平均帧大小，当设置venc模块
                                          参数bitrate_win_times为相应时间(秒)值后有效。 */
    VENC_CU_OUT_DATA_S stCuOutDataInfo; /* Cuinfo信息描述。 */
} VENC_ADDITIONAL_INFO_S;

/* 定义H26X RC高级属性参数 */
typedef struct tagRcParamH26x {
    NI_S32  s32InitIQp;               /* 初始第一个I帧QP,默认值-1，表示rc自动计算初始QP */
    NI_BOOL bHrdEn;                   /* Hypothetical Reference Decoder model, [0,1]. restricts the instantaneous bitrate and
                                       * total bit amount of every coded picture. Enabling HRD will cause tight constrains
                                       * on the operation of the rate control
                                       */
    NI_U32 u32HrdCpbSize;             /* Size of Coded Picture Buffer in HRD (bits) */
    NI_S32 s32QpDeltaIP;              /* 用于设置qp_delta_i_p，通道未使能的情况下可修改配置。默认值0，表示由码控算法自动调节。*/
    NI_S32 s32MinMbQp;                /* CTB RC宏块QP最小值, 通道未使能的情况下可修改配置。有效值范围为[0,51]，
                                        默认值-1，表示由码控算法自动调节。 */
    NI_S32 s32MaxMbQp;                /* CTB RC宏块QP最大值, 通道未使能的情况下可修改配置。有效值范围为[0,51]，默认值48。 */
    NI_S32 s32MaxISize;               /* 用于设置最大I帧Size(字节单位),D1分辨率及以下默认值75000，720P分辨率及以下默认值150000，
                                        1080P分辨率及以下默认值437500，其他分辨率默认值0，表示由码控算法自动调节。 */
    NI_S32 s32TuneVisual;             /* Pursue subjective optimization rather than BD-rate。通道未使能的情况下可修改配置。
                                        CBR下默认值226,其他模式下默认值216。该值默认已调优，不建议用户修改。*/
    NI_U32 u32StaticSceneIbitPercent; /* i、p之间的size比，通道未使能的情况下可修改配置。默认值0，表示由码控算法自动调节。*/
    NI_S32 s32IpenaltyAngular;        /* 通道未使能的情况下可修改配置。有效值范围为[0,31]，默认值-1，表示由码控算法自动调节。 */
    NI_S32 s32IpenaltyPlanar;         /* 通道未使能的情况下可修改配置。有效值范围为[0,31]，默认值-1，表示由码控算法自动调节。 */
    NI_S32 s32IpenaltyDc;             /* 通道未使能的情况下可修改配置。有效值范围为[0,31]，默认值-1，表示由码控算法自动调节。 */
    NI_S32 s32Ipenalty8Accum;         /* 通道未使能的情况下可修改配置。有效值范围为[0,31]，默认值-1，表示由码控算法自动调节。 */
    NI_S32 s32Ipenalty16Accum;        /* 通道未使能的情况下可修改配置。有效值范围为[0,31]，默认值-1，表示由码控算法自动调节。 */
    NI_S32 s32Ipenalty32Full;         /* 通道未使能的情况下可修改配置。有效值范围为[0,31]，H264编码时默认值8，H265编码时默认值-1，
                                         表示由码控算法自动调节。 */
    NI_S32 s32MaqQpMax;               /* set qp adjust limit。通道未使能的情况下可修改配置。有效值范围为[0,51],默认值-1，
                                        表示由码控算法自动调节。 */
    NI_BOOL bScdEn;                   /* 默认值FALSE，ax520该参数无效。 */
    NI_S32  s32ScdPercent;            /* ax520该参数无效。 */
    NI_S32  s32ScdThreshold;          /* ax520该参数无效。 */
    NI_S32  s32AqSsim;                /* Enable SSIM driven adaptive_quantisation。通道未使能的情况下可修改配置。
                                        有效值范围为[0,3],默认值3。*/
    NI_S32 s32AqNegRatio;             /* set decrease qp speed, ratio larger, speed faster。通道未使能的情况下可修改配置。
                                        当s32AqSsim非0时有效，有效值范围为[0,63]，默认值-1，表示由码控算法自动调节。*/
    NI_S32 s32AqPosRatio;             /* set increase qp speed, ratio larger, speed faster。通道未使能的情况下可修改配置。
                                        当s32AqSsim非0时有效，有效值范围为[0,63]，默认值-1，表示由码控算法自动调节。*/
    NI_S32 s32AqQpDeltaLmt;           /* set qp adjust limit。通道未使能的情况下可修改配置。当s32AqSsim非0时有效，
                                        有效值范围为[0,15]，默认值-1，表示由码控算法自动调节。*/
    NI_S32 s32AqInitFrmAvgSvar;       /* initial first or scenechange case frm avg svar。通道未使能的情况下可修改配置。
                                        当s32AqSsim非0时有效，有效值范围为[0,15]，CBR下默认值1，其他模式下默认值6。*/
    NI_S32 s32AqSrc;                  /* AQ fixed parameters。通道未使能的情况下可修改配置。当s32AqSsim非0时有效，有效值范围为[0,63]，默认值0。*/
    NI_S32 s32CbQpOffset;             /* QP offset between CB and Luma(HEVC only)。有效值范围为[-12,12]，H265默认值-6，H264该值无意义。
                                        通道未使能的情况下可修改配置。*/
    NI_S32 s32CrQpOffset;             /* QP offset between CR and Luma(HEVC only)。有效值范围为[-12,12]，H265默认值-6，H264该值无意义。
                                        通道未使能的情况下可修改配置。*/
    NI_S32 s32ChromaDqp;              /* chroma dqp for h.264。有效值范围为[-12,12]，H264默认值-6，H265该值无意义。通道未使能的情况下可修改配置。*/
    NI_S32 s32TemporalMvp;            /* Enable/Disable temporal motion vector predicition (HEVC only)。有效值范围为0（关）或
                                        者1（开），默认值1。通道未使能的情况下可修改配置。*/
} RC_PARAM_H26X_S;

/* 定义MJPEG RC高级属性参数 */
typedef struct tagRcParamMjpeg {
    NI_U32 u32Qpmin;         /* Minimum QP for picture, [0,51] */
    NI_U32 u32Qpmax;         /* Maximum QP for picture, [u32MinIQp,51] */
    NI_S32 s32PicQpDeltaMin; /* minimum pic qp delta, range:[-10,-1],default:-5 */
    NI_S32 s32PicQpDeltaMax; /* maximum pic qp delta, range:[1,10],default:5 */
} RC_PARAM_MJPEG_S;

/* the block parameter record specifies the various properties of a quad */
typedef struct tagVencQpMapBlockParamRecord {
    /* 负值的时候高位为符号位， 补码。 -32到31之间 */
    NI_U32 u32QpDeltaTopLeft : 6;   //[0-5]     /*deltaqp取值范围[-32,31]*/
    NI_U32 u32QpDeltaTopRight : 6;  //[6-11]
    NI_U32 u32QpDeltaBotLeft : 6;   //[12-17]
    NI_U32 u32QpDeltaBotRight : 6;  //[18-23]

    NI_U32 u32Enable : 1;      //[24]
    NI_U32 u32Resvd0 : 4;      //[25-28]
    NI_U32 u32ForceIntra : 1;  //[29]
    NI_U32 u32AbsQp : 1;       //[30]      /*绝对qp使能位，需先将SelQpDelta置1，absqp=baseqp[32]+deltaqp[-32,31]*/
    NI_U32 u32Skip : 1;        //[31]

    NI_U32 u32MinQpTopLeft : 6;   //[32:37]   /*设置qp值下限，如不设置则为默认值0*/
    NI_U32 u32MinQpTopRight : 6;  //[38:43]
    NI_U32 u32MinQpBotLeft : 6;   //[44:49]
    NI_U32 u32MinQpBotRight : 6;  //[50:55]
    NI_U32 u32SelQpDelta : 1;     //[56]      /*相对qp使能位，deltaqp取值范围[-32,31]，相应减少或增加图像qp值*/
    NI_U32 u32Resvd1 : 7;         //[57-63]
} VENC_QPMAP_BLOCK_PARAM_RECORD;

typedef struct tagVencQpMapExt {
    NI_BOOL bQpMapDirect; /* 可以通过使用mbuf机制实现qpmap内存无拷贝机制，
                            此时需要将QpMapDirect置为True,且用户将申请的mbuf poolid赋值给u32PoolId */
    NI_U32  u32PoolId;
    NI_BOOL bActOnIFrm; /* QpMap是否作用在i帧上，默认i帧不起作用，仅支持CBR、VBR */
    NI_BOOL bAbsQp;     /* 用户给的QpMap是否启用了绝对qp功能，驱动中需要set相应parameter到fw中 */
} VENC_QPMAP_EXT_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* End of #ifndef __NI_PRIVATE_VENC_H__ */
