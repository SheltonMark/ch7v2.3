/* 
 * ni_ans.h -- ans模块对外提供的头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */

#ifndef NI_ANS_H
#define NI_ANS_H


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include <stddef.h>
#include <stdint.h>


/* ========================================================================== */
/*                         宏和类型定义区                                     */      
/* ========================================================================== */
typedef void *      AnsHandle;

AnsHandle *Ni_Ans_Create();

 /* This function initializes a NS instance and has to be called before any other
 * processing is made.
 *
 * Input:
 *      - NS_inst       : Instance that should be initialized
 *      - fs            : sampling frequency
 *
 * Output:
 *      - NS_inst       : Initialized instance
 *
 * Return value         :  0 - Ok
 *                        -1 - Error
 */
int Ni_Ans_Init(AnsHandle *NS_inst, uint32_t fs);


/*
 * This changes the aggressiveness of the noise suppression method.
 *
 * Input:
 *      - NS_inst       : Noise suppression instance.
 *      - mode          : 0: Mild, 1: Medium , 2: Aggressive
 *
 * Output:
 *      - NS_inst       : Updated instance.
 *
 * Return value         :  0 - Ok
 *                        -1 - Error
 */
int Ni_Ans_set_policy(AnsHandle *NS_inst, int mode);


/*
 * This functions estimates the background noise for the inserted speech frame.
 * The input and output signals should always be 10ms (80 or 160 samples).
 *
 * Input
 *      - NS_inst       : Noise suppression instance.
 *      - spframe       : Pointer to speech frame buffer for L band
 *
 * Output:
 *      - NS_inst       : Updated NS instance
 */
void Ni_Ans_Analyze(AnsHandle *NS_inst, const int16_t *spframe);


/*
 * This functions does Noise Suppression for the inserted speech frame. The
 * input and output signals should always be 10ms (80 or 160 samples).
 *
 * Input
 *      - NS_inst       : Noise suppression instance.
 *      - spframe       : Pointer to speech frame buffer for each band
 *      - num_bands     : Number of bands
 *
 * Output:
 *      - NS_inst       : Updated NS instance
 *      - outframe      : Pointer to output frame for each band
 */
void Ni_Ans_Process(AnsHandle *NS_inst,
                      const int16_t *const *spframe,
                      int num_bands,
                      int16_t *const *outframe);
					  
					  
					  
/*
 * This function frees the dynamic memory of a specified noise suppression
 * instance.
 *
 * Input:
 *      - NS_inst       : Pointer to NS instance that should be freed
 */
void Ni_Ans_Free(AnsHandle *NS_inst);


#ifdef __cplusplus
}
#endif

#endif  // NI_NS_H
