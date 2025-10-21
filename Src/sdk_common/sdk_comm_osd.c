/* 
 * sdk_comm_osd.c -- osd sample用例 
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
     
#include "mdk_osd.h"
#include "mdk_sys.h"
#include "sdk_comm.h"
#include "TdCommon.h"
enum
{
 OSD_ALIGN_1BIT = 64, 
 OSD_ALIGN_2BIT = 32,
 OSD_ALIGN_4BIT = 16,
};

#define	SMB_V_STEP		4
#define SMB_H_STEP		16
#define	MB_V_STEP		16
#define MB_H_STEP		16
#define BMB_V_STEP		32
#define BMB_H_STEP		32

#ifndef NI_Align
#define NI_Align(value, align)   ((((value) + ((align) - 1)) \
                                     / (align)) * (align))
#endif

//static NI_U32 u32PhyAddr = 0;
//static  NI_VOID *pVirAddr = NI_NULL;

/* 初始化调色板内容 */
NI_S32 SampleOsdPaletteInit(OSD_PALETTE_S *pstOsdPalette)
{
    NI_U32 i;
    
    for(i = 0 ; i < OSD_MAX_PALETTE_NUM; i++)
    {
       pstOsdPalette->astPalette[i].u8Alpha = 31;
    }
	pstOsdPalette->astPalette[0].u8Alpha = 0;
    pstOsdPalette->astPalette[4].u8Alpha = 24;
    /*黑*/
    pstOsdPalette->astPalette[0].stYUVColor.u8Y = 0;
    pstOsdPalette->astPalette[0].stYUVColor.u8U = 128;
    pstOsdPalette->astPalette[0].stYUVColor.u8V = 128;

    /*白*/
    pstOsdPalette->astPalette[1].stYUVColor.u8Y = 255;
    pstOsdPalette->astPalette[1].stYUVColor.u8U = 127;
    pstOsdPalette->astPalette[1].stYUVColor.u8V = 127;

    /*红*/
    //pstOsdPalette->astPalette[2].stYUVColor.u8Y = 76;
    //pstOsdPalette->astPalette[2].stYUVColor.u8U = 84;
    //pstOsdPalette->astPalette[2].stYUVColor.u8V = 255;

    /*粉*/
    //pstOsdPalette->astPalette[3].stYUVColor.u8Y = 212;
    //pstOsdPalette->astPalette[3].stYUVColor.u8U = 122;
    //pstOsdPalette->astPalette[3].stYUVColor.u8V = 158;

    pstOsdPalette->astPalette[2].stYUVColor.u8Y = 0;
    pstOsdPalette->astPalette[2].stYUVColor.u8U = 128;
    pstOsdPalette->astPalette[2].stYUVColor.u8V = 128;

    pstOsdPalette->astPalette[3].stYUVColor.u8Y = 0;
    pstOsdPalette->astPalette[3].stYUVColor.u8U = 128;
    pstOsdPalette->astPalette[3].stYUVColor.u8V = 128;

    /*蓝*/
    pstOsdPalette->astPalette[4].stYUVColor.u8Y = 255;
    pstOsdPalette->astPalette[4].stYUVColor.u8U = 127;
    pstOsdPalette->astPalette[4].stYUVColor.u8V = 127;

    /*绿*/
    pstOsdPalette->astPalette[5].stYUVColor.u8Y = 75;
    pstOsdPalette->astPalette[5].stYUVColor.u8U = 84;
    pstOsdPalette->astPalette[5].stYUVColor.u8V = 74;

    /*紫*/
    pstOsdPalette->astPalette[6].stYUVColor.u8Y = 52;
    pstOsdPalette->astPalette[6].stYUVColor.u8U = 170;
    pstOsdPalette->astPalette[6].stYUVColor.u8V = 181;

    /*橙*/
    pstOsdPalette->astPalette[7].stYUVColor.u8Y = 173;
    pstOsdPalette->astPalette[7].stYUVColor.u8U = 30;
    pstOsdPalette->astPalette[7].stYUVColor.u8V = 186;

    /*青*/
    pstOsdPalette->astPalette[8].stYUVColor.u8Y = 178;
    pstOsdPalette->astPalette[8].stYUVColor.u8U = 171;
    pstOsdPalette->astPalette[8].stYUVColor.u8V = 0;

    /*海军蓝*/
    pstOsdPalette->astPalette[9].stYUVColor.u8Y = 14;
    pstOsdPalette->astPalette[9].stYUVColor.u8U = 192;
    pstOsdPalette->astPalette[9].stYUVColor.u8V = 117;

    /*灰*/
    pstOsdPalette->astPalette[10].stYUVColor.u8Y = 127;
    pstOsdPalette->astPalette[10].stYUVColor.u8U = 127;
    pstOsdPalette->astPalette[10].stYUVColor.u8V = 128;

    /*棕*/
    pstOsdPalette->astPalette[11].stYUVColor.u8Y = 78;
    pstOsdPalette->astPalette[11].stYUVColor.u8U = 107;
    pstOsdPalette->astPalette[11].stYUVColor.u8V = 189;

    /*金*/
    pstOsdPalette->astPalette[12].stYUVColor.u8Y = 202;
    pstOsdPalette->astPalette[12].stYUVColor.u8U = 13;
    pstOsdPalette->astPalette[12].stYUVColor.u8V = 165;

    /*橄榄*/
    pstOsdPalette->astPalette[13].stYUVColor.u8Y = 113;
    pstOsdPalette->astPalette[13].stYUVColor.u8U = 63;
    pstOsdPalette->astPalette[13].stYUVColor.u8V = 138;

    /*洋红*/
    pstOsdPalette->astPalette[14].stYUVColor.u8Y = 105;
    pstOsdPalette->astPalette[14].stYUVColor.u8U = 212;
    pstOsdPalette->astPalette[14].stYUVColor.u8V = 234;

    /*黄*/
    pstOsdPalette->astPalette[15].stYUVColor.u8Y = 225;
    pstOsdPalette->astPalette[15].stYUVColor.u8U = 0;
    pstOsdPalette->astPalette[15].stYUVColor.u8V = 148;

    return NI_SUCCESS;
}

static NI_S32 SampleOsdDataLineToMb(OSD_ACCESS_FORMAT_E enFormat, NI_U8 *pcInData, NI_U8 *pcOutData, NI_U32 u32Wdith, NI_U32 u32Height) 
{
    NI_U32 i, j, m;
    NI_CHAR* pcTmpOutData = (NI_CHAR*)pcOutData;
    
    switch(enFormat)
    {
         case OSD_FORMAT_BIT:
         {
            for(i = 0; i < u32Height; i += 16)
            {
                for(j = 0; j < u32Wdith; j += 16)
                {
                    for(m = 0; m < 16; m++)
                    {
                        memcpy(pcTmpOutData + m * 2, pcInData + (i + m) * u32Wdith / 8 + j / 8, 2);
                    }

                    pcTmpOutData += (16 * 2);
                }
            }

            break;
         }
         case OSD_FORMAT_BYTE:
         {  
            for(i = 0; i < u32Height; i += 16)
            {
                for(j = 0; j < u32Wdith; j += 16)
                {
                    for(m = 0; m < 16; m++)
                    {
                        memcpy(pcTmpOutData + m * 16, pcInData + (i + m) * u32Wdith + j, 16);
                    }
                    
                    pcTmpOutData += (16 * 16);
                }
            }
            break;
        }
        default:
            break;
    }
    return 0;
}

NI_VOID OSD_TEST_ConvertToLittleEndian2(NI_VOID *pOsdData, OSD_PIXEL_FORMAT_E enPixelFormat, NI_U32 u32DataSize)
{    
    NI_U32 i = 0;
    NI_U8 *ptemp = NI_NULL;

    if (NI_NULL == pOsdData)
    {
        printf("////  Osd data ptr is NULL     ////.\n");
        return;
    }

    if (enPixelFormat >= OSD_PIXEL_FORMAT_BUTT)
    {
        printf("////  Osd pixel format is illegal     ////.\n");
        return;
    }

    ptemp = (NI_U8 *)pOsdData;
    for (i = 0; (i < u32DataSize) && (ptemp != NI_NULL); i++)
    {
        if (OSD_PIXEL_FORMAT_1BIT == enPixelFormat)
        {
            *(ptemp + i)= ((*(ptemp + i) & 0x80) >> 7)
                    | ((*(ptemp + i) & 0x40) >> 5)
                    | ((*(ptemp + i) & 0x20) >> 3)
                    | ((*(ptemp + i) & 0x10) >> 1)
                    | ((*(ptemp + i) & 0x08) << 1)
                    | ((*(ptemp + i) & 0x04) << 3)
                    | ((*(ptemp + i) & 0x02) << 5)
                    | ((*(ptemp + i) & 0x01) << 7);
        }
        else if (OSD_PIXEL_FORMAT_2BIT == enPixelFormat)
        {
            *(ptemp + i) = ((*(ptemp + i) & 0x80) >> 6)
                    | ((*(ptemp + i) & 0x40) >> 6)
                    | ((*(ptemp + i) & 0x20) >> 2)
                    | ((*(ptemp + i) & 0x10) >> 2)
                    | ((*(ptemp + i) & 0x08) << 2)
                    | ((*(ptemp + i) & 0x04) << 2)
                    | ((*(ptemp + i) & 0x02) << 6)
                    | ((*(ptemp + i) & 0x01) << 6);
        }
        else if (OSD_PIXEL_FORMAT_4BIT == enPixelFormat)
        {
            *(ptemp + i) = ((*(ptemp + i) & 0x80) >> 4)
                    | ((*(ptemp + i) & 0x40) >> 4)
                    | ((*(ptemp + i) & 0x20) >> 4)
                    | ((*(ptemp + i) & 0x10) >> 4)
                    | ((*(ptemp + i) & 0x08) << 4)
                    | ((*(ptemp + i) & 0x04) << 4)
                    | ((*(ptemp + i) & 0x02) << 4)
                    | ((*(ptemp + i) & 0x01) << 4);
        }
        else
        {
            //do nothing
        }
    }
}


void SampleOsdDataYuv2Mb(int img_width, int img_height, unsigned char *p_y_line_buf, unsigned char *p_u_line_buf, unsigned char *p_v_line_buf, unsigned char *p_yuv_mb_buf)
{
	unsigned char *p_yuv_mb_buf_tmp = p_yuv_mb_buf;
	int i, j, k;
	
	
	for (i = 0; i < img_height; i += MB_V_STEP)
	{
		for (j = 0; j < img_width; j += MB_H_STEP)
		{
			// Y
			for (k = 0; k < MB_V_STEP; k++)
				memcpy(p_yuv_mb_buf_tmp + k * MB_H_STEP, p_y_line_buf + (i + k) * img_width + j, MB_H_STEP);
			// Y offset
			p_yuv_mb_buf_tmp += (MB_V_STEP * MB_H_STEP);
			// U
			for (k = 0; k < MB_V_STEP / 2; k++)
				memcpy(p_yuv_mb_buf_tmp + k * (MB_H_STEP / 2), p_u_line_buf + ((i / 2) + k) * (img_width / 2) + (j / 2), (MB_H_STEP / 2));
			// U offset
			p_yuv_mb_buf_tmp += (MB_V_STEP * MB_H_STEP / 4);
			// V
			for (k = 0; k < MB_V_STEP / 2; k++)
				memcpy(p_yuv_mb_buf_tmp + k * (MB_H_STEP / 2), p_v_line_buf + ((i / 2) + k) * (img_width / 2) + (j / 2), (MB_H_STEP / 2));
			// V offset
			p_yuv_mb_buf_tmp += (MB_V_STEP * MB_H_STEP / 4);
		}
	}

	return;
}

NI_U32 OSD_TEST_Matrix2ExtMatrix(NI_U8 *pu8SrcMatrixAddr, SRC_OSD_ATTR *pstSrcOsdAttr, NI_U8 *pu8ExtMatrixAddr)
{
    NI_U32 u32OsdSize = 0;
    //NI_U32 align_width = 0;
    NI_U32 u32MatrixWidth = 0, u32MatrixHeight = 0;
    NI_U32 u32Multiplier = 0;
    OSD_ACCESS_FORMAT_E enFormat;
    NI_U8 u8FgIdx = 0,u8BgIdx = 0;
    NI_U32 i, j, k;
    NI_U8 *dest, *src, *tmp, *pu8YSrc, *pu8USrc, *pu8VSrc, *tmp1;
    NI_U32 u32SrcOffset;
    //NI_U32 dest_offset;

    if((NI_NULL == pu8SrcMatrixAddr) || (NI_NULL == pu8ExtMatrixAddr))
    {
        NI_ERROR("input parameter error!!\r\n");
        
        return NI_FAILURE;
    }
    
    enFormat = pstSrcOsdAttr->enFormat;
    u32MatrixWidth = pstSrcOsdAttr->stSize.u32Width;
    u32MatrixHeight = pstSrcOsdAttr->stSize.u32Height;

    if (NI_TRUE == pstSrcOsdAttr->stOsdRegion.bMacroFormat)
    {
        /* 原始点阵宽度必须至少为16的整数倍 */
        if ((u32MatrixWidth & 0xf) || (u32MatrixHeight & 0xf))
        {
            NI_ERROR("matrix_width(%d) or u32MatrixHeight(%d) is not multiple of 16!!\r\n", u32MatrixWidth, u32MatrixHeight);
            
            return NI_FAILURE;
        }
    }
    else
    {
        /* 原始点阵宽度必须至少为16的整数倍 */
        if ((u32MatrixWidth & 0x7) || (u32MatrixHeight & 0x1))
        {
            NI_ERROR("matrix_width(%d) is not multiple of 8, or matrix_heighet(%d) is not multiple of 2!!\r\n", u32MatrixWidth, u32MatrixHeight);
            
            return NI_FAILURE;
        }
    }

    switch (pstSrcOsdAttr->stOsdRegion.enPixelFormat)
    {
        case OSD_PIXEL_FORMAT_1BIT:
            //align_width = NI_align(u32MatrixWidth, OSD_ALIGN_1BIT);
            u8FgIdx = pstSrcOsdAttr->u8FgIdx & 0x1;
            u8BgIdx = pstSrcOsdAttr->u8BgIdx & 0x1;
            u32Multiplier = 1;
            break;
            
        case OSD_PIXEL_FORMAT_2BIT :
            //align_width = NI_align(u32MatrixWidth, OSD_ALIGN_1BIT);
            u8FgIdx = pstSrcOsdAttr->u8FgIdx & 0x3;
            u8BgIdx = pstSrcOsdAttr->u8BgIdx & 0x3;
            u32Multiplier = 2;
            break;
            
        case OSD_PIXEL_FORMAT_4BIT :
            //align_width = NI_align(u32MatrixWidth, OSD_ALIGN_1BIT);
            u8FgIdx = pstSrcOsdAttr->u8FgIdx & 0xf;
            u8BgIdx = pstSrcOsdAttr->u8BgIdx & 0xf;
            u32Multiplier = 4;
            break;
            
        case  OSD_PIXEL_FORMAT_BYTE:
            u32Multiplier = 8 * 3 / 2;
            break;
            
        default:
            NI_ERROR("enPixelFormt %d parameter error!!\r\n", pstSrcOsdAttr->stOsdRegion.enPixelFormat);
            
        return NI_FAILURE;
    }

    /* 传给驱动的扩展点阵数据大小*/
    u32OsdSize = u32MatrixWidth * u32MatrixHeight * u32Multiplier / 8;

    /* 清除老的OSD数据，必需*/
    memset(pu8ExtMatrixAddr, 0, u32OsdSize);

    //dest_offset = align_width * u32Multiplier / 8;
    if(enFormat == OSD_FORMAT_BIT)
    {
        u32SrcOffset = u32MatrixWidth / 8;
    }
    else
    {
        u32SrcOffset = u32MatrixWidth;
    }

    dest = pu8ExtMatrixAddr;
    src = pu8SrcMatrixAddr;
    tmp = (NI_U8*)malloc(u32OsdSize);
    tmp1 = tmp;
    memset(tmp, 0, u32OsdSize);

    //NI_INFO("enPixelFormat = %d, bMacroFormat = %d, enFormat = %d\n"
    // , pstSrcOsdAttr->stOsdRegion.enPixelFormat, pstSrcOsdAttr->stOsdRegion.bMacroFormat, enFormat);
    switch (pstSrcOsdAttr->stOsdRegion.enPixelFormat)
    {
        case OSD_PIXEL_FORMAT_1BIT:
        {
            if(enFormat == OSD_FORMAT_BIT)
            {
                if (NI_TRUE == pstSrcOsdAttr->stOsdRegion.bMacroFormat)
                {
                    SampleOsdDataLineToMb(OSD_FORMAT_BIT, src, dest, u32MatrixWidth, u32MatrixHeight);
                }
                else
                {
                    memcpy(dest, src, u32OsdSize);
                }
            }
            else if(enFormat == OSD_FORMAT_BYTE)
            {
                if (NI_TRUE == pstSrcOsdAttr->stOsdRegion.bMacroFormat)
                {
                    SampleOsdDataLineToMb(OSD_FORMAT_BYTE, src, tmp, u32MatrixWidth, u32MatrixHeight);
                }
                else
                {
                    tmp = src;
                }

                for(j = 0; j < u32MatrixHeight; j++)
                {
                    for(i = 0; i < u32MatrixWidth; i++)
                    {
                        if(1 == tmp[i])
                        {
                            dest[i / 8] |= u8FgIdx << (7 - (i % 8));
                        }
                        else
                        {
                            dest[i / 8] |= u8BgIdx << (7 - (i % 8));
                        }
                    }
                    dest += u32SrcOffset / 8;
                    tmp += u32SrcOffset;
                }
            }
            else
            {
                NI_ERROR("eFormat parameter error!!\r\n");

                free(tmp1);
                tmp1 = NULL; 
                return NI_FAILURE;
            }
            break;
        }
            
        case OSD_PIXEL_FORMAT_2BIT :
        {
            if(enFormat == OSD_FORMAT_BIT)
            {
                if (NI_TRUE == pstSrcOsdAttr->stOsdRegion.bMacroFormat)
                {
                    SampleOsdDataLineToMb(OSD_FORMAT_BIT, src, tmp, u32MatrixWidth, u32MatrixHeight);
                }
                else
                {
                    tmp = src;
                }
                
                for(i = 0; i < u32MatrixHeight; i++)
                {
                    for(j = 0; j < u32SrcOffset; j++)
                    {   
                        for(k = 0; k < 8; k++)
                        {
                            if((tmp[j] >> ( 7 - k )) & 0x1)
                            {
                                dest[2 * j + k / 4] |= (u8FgIdx << (2 * ((7 - k) & 0x3)));
                            }
                            else
                            {
                                dest[2 * j + k / 4] |= (u8BgIdx << (2 * ((7 - k) & 0x3)));
                            }
                        }
                    }
                    dest += u32SrcOffset * 2;
                    tmp += u32SrcOffset;
                }
            }
            else if(enFormat == OSD_FORMAT_BYTE)
            {
                if (NI_TRUE == pstSrcOsdAttr->stOsdRegion.bMacroFormat)
                {
                    SampleOsdDataLineToMb(OSD_FORMAT_BYTE, src, tmp, u32MatrixWidth, u32MatrixHeight);     
                }
                else
                {
                    tmp = src;
                }    
                
                for(j = 0; j < u32MatrixHeight; j++)
                {
                    for(i = 0; i < u32MatrixWidth; i++)
                    {
                        if(1 == tmp[i])
                        {
                            dest[i / 4] |= u8FgIdx << (2 * (3 - (i % 4)));
                        }
                        else   
                        {
                            dest[i / 4] |= u8BgIdx << (2 * (3 - (i % 4)));
                        }
                    }
                    dest += u32SrcOffset / 4;
                    tmp += u32SrcOffset;
                }
            }
            else
            {
                NI_ERROR("eFormat parameter error!!\r\n");

                free(tmp1);
                tmp1 = NULL;
                return NI_FAILURE;
            }
            break;
        }
            
        case OSD_PIXEL_FORMAT_4BIT:
        {
            if(enFormat == OSD_FORMAT_BIT)
            {       
                if (NI_TRUE == pstSrcOsdAttr->stOsdRegion.bMacroFormat)
                {
                    SampleOsdDataLineToMb(OSD_FORMAT_BIT, src, tmp, u32MatrixWidth, u32MatrixHeight);
                }
                else
                {
                    tmp = src;
                }
                
                for(i = 0; i < u32MatrixHeight; i++)
                {
                    for(j = 0; j < u32SrcOffset; j++)
                    {   
                        for(k = 0; k < 8; k++)
                        {
                            if((tmp[j] >> ( 7 - k )) & 0x1)
                            {
                                dest[4 * j + k / 2] |= (u8FgIdx << (4 * ((7 - k) & 0x1)));
                            }
                            else
                            {
                                dest[4 * j + k / 2] |= (u8BgIdx << (4 * ((7 - k) & 0x1)));
                            }
                        }
                    }
                    dest += u32SrcOffset * 4;
                    tmp += u32SrcOffset;
                }
            }
            else if(enFormat == OSD_FORMAT_BYTE)
            {
                if (NI_TRUE == pstSrcOsdAttr->stOsdRegion.bMacroFormat)
                {
                    SampleOsdDataLineToMb(OSD_FORMAT_BYTE, src, tmp, u32MatrixWidth, u32MatrixHeight);
                }
                else
                {
                    tmp = src;
                }
               
                for(j = 0; j < u32MatrixHeight; j++)
                {
                    for(i = 0; i < u32MatrixWidth; i++)
                    {
                        if(1 == tmp[i])
                        {
                            dest[i / 2] |= u8FgIdx << (4 * (1 - (i % 2)));
                        }
                        else  
                        {
                            dest[i / 2] |= u8BgIdx << (4 * (1 - (i % 2)));
                        }               
                    }
                    dest += u32SrcOffset / 2;
                    tmp += u32SrcOffset;
                }
            }
            else
            {
                NI_ERROR("eFormat parameter error!!\r\n");

                free(tmp1);
                tmp1 = NULL;
                return NI_FAILURE;
            }
            break;
        }
        case OSD_PIXEL_FORMAT_BYTE:
        {
            if (NI_TRUE == pstSrcOsdAttr->stOsdRegion.bMacroFormat)
            {
                pu8YSrc = src;
                pu8USrc = pu8YSrc + u32MatrixWidth * u32MatrixHeight;
                pu8VSrc = pu8USrc + u32MatrixWidth * u32MatrixHeight / 4;
                
                SampleOsdDataYuv2Mb(u32MatrixWidth, u32MatrixHeight, pu8YSrc, pu8USrc, pu8VSrc, dest);
            }
            else
            {
                memcpy(dest, src, u32OsdSize);
            }

            break;
        }
        default:

            free(tmp1);
            tmp1 = NULL;
            return NI_FAILURE;
    }

    free(tmp1);
    tmp1 = NULL;

    return NI_SUCCESS;
}
#if 0
NI_S32 VSF_drvNarrowOsd(VSF_DrvOsdSdkData *pOsd, OSD_BUFFER_INFO_S *psrcOsd, OSD_BUFFER_INFO_S *pdestOsd, NI_U32 u32NarrowNumber, NI_U32 OsdDispHandle)
{
    NI_S32 status;
    OSD_BUFFER_INFO_S stOsdBufInfo;
    NI_U32 i, j;
    NI_U32 u32DivNum = 1;
    NI_U8 *pu8SrcVirAddr = NULL, *pu8DestVirAddr = NULL;

    if (u32NarrowNumber != 2)
    {
        LOG_ERROR("nott support this narrow num %d\n", u32NarrowNumber);
        return OSA_EFAIL;
    }

    pdestOsd->u32Len = psrcOsd->u32Len / u32NarrowNumber / u32NarrowNumber;

    status = NI_MDK_OSD_GetBuffer(OsdDispHandle, pdestOsd);
    if (status != OSA_SOK)
    {
        LOG_ERROR("NI_MDK_OSD_GetBuffer failed with %#x!\n", status);
        return OSA_EFAIL;
    }
    if (VSF_DRV_OSD_TYPE_PIC == pOsd->dataType)
    {
        u32DivNum = 1;
    }
    else if (VSF_DRV_OSD_TYPE_BIT4 == pOsd->dataType)
    {
        u32DivNum = 2;
    }
    else if (VSF_DRV_OSD_TYPE_BIT1 == pOsd->dataType)
    {
        u32DivNum = 8;
    }

    pu8SrcVirAddr = (NI_U8 *)psrcOsd->pu32VirtAddr;
    pu8DestVirAddr = (NI_U8 *)pdestOsd->pu32VirtAddr;

    for (i = 0; i < pOsd->height; i += u32NarrowNumber)
    {
        for (j = 0; j < (pOsd->width / u32DivNum); j += u32NarrowNumber)
        {
            if (VSF_DRV_OSD_TYPE_PIC == pOsd->dataType)
            {
                *pu8DestVirAddr = *pu8SrcVirAddr;
                pu8DestVirAddr++;
                pu8SrcVirAddr += u32NarrowNumber;
            }
            else if (VSF_DRV_OSD_TYPE_BIT4 == pOsd->dataType)
            {
                *pu8DestVirAddr = *pu8SrcVirAddr & 0xf0;
                pu8SrcVirAddr += (u32NarrowNumber / 2);
                *pu8DestVirAddr |= (*pu8SrcVirAddr & 0xf0 >> 4);
                pu8SrcVirAddr += (u32NarrowNumber / 2);
                pu8DestVirAddr++;
            }
            else if (VSF_DRV_OSD_TYPE_BIT1 == pOsd->dataType)
            {
                *pu8DestVirAddr = (*pu8SrcVirAddr & 0x80) | (*pu8SrcVirAddr & 0x20 << 1) | (*pu8SrcVirAddr & 0x08 << 2) | (*pu8SrcVirAddr & 0x02 << 3);
                pu8SrcVirAddr += (u32NarrowNumber / 2);
                *pu8DestVirAddr |= (*pu8SrcVirAddr & 0x80 >> 4) | (*pu8SrcVirAddr & 0x20 >> 3) | (*pu8SrcVirAddr & 0x08 >> 2) | (*pu8SrcVirAddr & 0x02 >> 1);
                pu8SrcVirAddr += (u32NarrowNumber / 2);
                pu8DestVirAddr++;
            }
        }
        pu8SrcVirAddr += pOsd->width / u32DivNum * (u32NarrowNumber - 1);
    }

    return status;
}
#endif
NI_S32 SDK_COMM_OSD_Creat(NI_U32 u32OsdHandle, MDK_CHN_S *pstMdkChn, SAMPLE_OSD_CONFIG_S *pstOsdCfg)
{
    //OSD_PALETTE_S stOsdPalette;
    OSD_REGION_S stOsdRegion;
    CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
    //OSD_BUFFER_INFO_S stOsdBufInfo;
    //SRC_OSD_ATTR stSrcOsdAttr;
    //OSD_DISP_ATTR_S stOsdDispAttr;
    //NI_U32 u32OsdDataSize;
    //FILE *pfd;
    //NI_CHAR szOsdCharFileName[128] = "/opt/app/ABCD.bin";
    //NI_U32 i, j;
    //NI_U8 *pSrcVirt = NI_NULL;
    //NI_U8 *pDesVirt = NI_NULL;
    //NI_U8 *temp = NI_NULL;

    if (NI_ID_VPS == pstMdkChn->enModId)
    {
        stOsdRegion.bMacroFormat = NI_FALSE;
		stOsdRegion.enOsdRgn = OSD_RGN;
    }

    if(NI_ID_VENC == pstMdkChn->enModId)
    {
        stOsdRegion.bMacroFormat = NI_FALSE;
		stOsdRegion.enOsdRgn = OSD_EX_RGN;
    }

    stOsdRegion.enPixelFormat = OSD_PIXEL_FORMAT_2BIT;
    //stSrcOsdAttr.stOsdRegion = stOsdRegion;
	//stSrcOsdAttr.stSize.u32Width = 64;
	//stSrcOsdAttr.stSize.u32Height = 32;
    //stSrcOsdAttr.u8FgIdx = 1;
    //stSrcOsdAttr.u8BgIdx = 0;
    //stSrcOsdAttr.enFormat = OSD_FORMAT_BIT;
	
	//stOsdBufInfo.u32Len = 4096;

	//stOsdDispAttr.bShow = NI_FALSE;
	//stOsdDispAttr.stSize.u32Width = 64;	
	//stOsdDispAttr.stSize.u32Height = 32; 
    //stOsdDispAttr.stStartPiont.s32X = pstOsdCfg->stStartPiont.s32X;
    //stOsdDispAttr.stStartPiont.s32Y = pstOsdCfg->stStartPiont.s32Y;
   // stOsdDispAttr.u32Layer = pstOsdCfg->u32Layer;
    //stOsdDispAttr.enAlphaMode = OSD_ALPHA_MODE_PALETTE;
    //stOsdDispAttr.u32RegionAlpha = 31;
   // stOsdDispAttr.bInvert = NI_TRUE;
   // stOsdDispAttr.bSpecialFrmId = pstOsdCfg->bSpecialFrmId;
    //stOsdDispAttr.u32TargetFrameId = pstOsdCfg->u32TargetFrameId;


    //u32OsdDataSize = stSrcOsdAttr.stSize.u32Width * stSrcOsdAttr.stSize.u32Height;
    
    //pfd = fopen(szOsdCharFileName, "rb");
    //if (NULL == pfd)
   // {
    //    printf("fopen err.\n");
    //    return NI_FAILURE;
    //}

    //NI_MDK_SYS_MmzAlloc(&u32PhyAddr, &pVirAddr, u32OsdDataSize, "osd_src", NULL);
    //printf("=====malloc u32PhyAddr:0x%x===u32OsdDataSize is %d ==\n", u32PhyAddr, u32OsdDataSize);

    //fread(pVirAddr, u32OsdDataSize, 1, pfd);
    
    //SampleOsdPaletteInit(&stOsdPalette);
    //int handle = 0;
    for (int i = 0; i < pCaptureDevice->EncDevice[0].StreamCount; i++)
    {
        //跳过编码通道
        if(i == CHL_3IRD_T)
        {
            break;
        }
        for(int j = 0; j < 4;j++)
        {
            NI_MDK_OSD_Create(u32OsdHandle, &stOsdRegion);
            u32OsdHandle++;
        }

#if 0
        stOsdBufInfo.u32Len = stOsdDispAttr.stSize.u32Width * stOsdDispAttr.stSize.u32Height / 2;
        NI_MDK_OSD_GetBuffer(u32OsdHandle, &stOsdBufInfo);

        pSrcVirt = (NI_U8 *)pVirAddr;
        pDesVirt = (NI_U8 *)stOsdBufInfo.pu32VirtAddr;
        temp = pDesVirt + stOsdDispAttr.stSize.u32Width / 4;
        for (i = 0; i < stSrcOsdAttr.stSize.u32Height; i++)
        {
            for (j = 0; j < stOsdDispAttr.stSize.u32Width / 8; j++)
            {
                *pDesVirt = (*pSrcVirt & 0x80) | ((*pSrcVirt & 0x80) >> 1) | ((*pSrcVirt & 0x40) >> 1) | ((*pSrcVirt & 0x40) >> 2) | ((*pSrcVirt & 0x20) >> 2) | ((*pSrcVirt & 0x20) >> 3) | ((*pSrcVirt & 0x10) >> 3) | ((*pSrcVirt & 0x10) >> 4);
                *temp = (*pSrcVirt & 0x80) | ((*pSrcVirt & 0x80) >> 1) | ((*pSrcVirt & 0x40) >> 1) | ((*pSrcVirt & 0x40) >> 2) | ((*pSrcVirt & 0x20) >> 2) | ((*pSrcVirt & 0x20) >> 3) | ((*pSrcVirt & 0x10) >> 3) | ((*pSrcVirt & 0x10) >> 4);
                pDesVirt++;
                temp++;

                *pDesVirt = ((*pSrcVirt & 0x8) << 4) | ((*pSrcVirt & 0x8) << 3) | ((*pSrcVirt & 0x4) << 3) | ((*pSrcVirt & 0x4) << 2) | ((*pSrcVirt & 0x2) << 2) | ((*pSrcVirt & 0x2) << 1) | ((*pSrcVirt & 0x1) << 1) | (*pSrcVirt & 0x1);
                *temp = ((*pSrcVirt & 0x8) << 4) | ((*pSrcVirt & 0x8) << 3) | ((*pSrcVirt & 0x4) << 3) | ((*pSrcVirt & 0x4) << 2) | ((*pSrcVirt & 0x2) << 2) | ((*pSrcVirt & 0x2) << 1) | ((*pSrcVirt & 0x1) << 1) | (*pSrcVirt & 0x1);
                pDesVirt++;
                pSrcVirt++;
                temp++;
            }
            pDesVirt += stOsdDispAttr.stSize.u32Width / 4;
            temp += stOsdDispAttr.stSize.u32Width / 4;
        }
 
        OSD_TEST_Matrix2ExtMatrix((NI_U8*)pVirAddr, &stSrcOsdAttr, (NI_U8*)stOsdBufInfo.pu32VirtAddr);
        OSD_TEST_ConvertToLittleEndian2((NI_U8*)stOsdBufInfo.pu32VirtAddr, stOsdRegion.enPixelFormat, stOsdBufInfo.u32Len);
        pstMdkChn->s32ChnId = 0;
        NI_MDK_OSD_SetPalette(pstMdkChn, &stOsdPalette);

        NI_MDK_OSD_PaintToChn(u32OsdHandle, pstMdkChn, &stOsdDispAttr);

        pstMdkChn->s32ChnId = 1;
        NI_MDK_OSD_SetPalette(pstMdkChn, &stOsdPalette);
        NI_MDK_OSD_PaintToChn(u32OsdHandle, pstMdkChn, &stOsdDispAttr);
#endif
    }

    return NI_SUCCESS;
}


NI_S32 SDK_COMM_OSD_Destroy(NI_U32 u32OsdHandle, MDK_CHN_S *pstMdkChn)
{
    CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
    for (int i = 0; i < pCaptureDevice->EncDevice[0].StreamCount; i++)
    {
        //跳过编码通道
        if (i == CHL_3IRD_T)
        {
            break;
        }
        for (int j = 0; j < 4; j++)
        {
            NI_MDK_OSD_ClearFromChn(u32OsdHandle, pstMdkChn, 500);
            NI_MDK_OSD_Destroy(u32OsdHandle);
            u32OsdHandle++;
        }
        pstMdkChn->s32ChnId++;
    }
    //NI_MDK_SYS_MmzFree(u32PhyAddr,pVirAddr);
	
    return NI_SUCCESS;
}


