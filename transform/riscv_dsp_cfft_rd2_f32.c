/******************************************************************************
 * Copyright (C) 2010-2025 Arm Limited or its affiliates. All rights reserved.*
 * Copyright (C) 2018-2025 Andes Technology Corporation. All rights reserved. *
 *                                                                            *
 * SPDX-License-Identifier: Apache-2.0                                        *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the License); you may      *
 * not use this file except in compliance with the License.                   *
 * You may obtain a copy of the License at                                    *
 *                                                                            *
 * www.apache.org/licenses/LICENSE-2.0                                        *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT    *
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.           *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 ******************************************************************************/

#include <config.h>
#include "internal_transform_math.h"

/**
 * @ingroup transform
 */

/**
 * @defgroup cfft_radix2 Radix-2 Complex FFT Functions
 *
 * Complex Fast Fourier Transform(CFFT) and Complex Inverse Fast Fourier
 * Transformm(CIFFT) is an efficient algorithm to compute Discrete Fourier
 * Transform(DFT) and Inverse Discrete Fourier Transform(IDFT). This
 * function is the implementaion of a very simple Cooley-Tukey Radix-2
 * DIF Complex FFT.
 *
 */

/**
 * @addtogroup cfft_radix2
 * @{
 */

/**
 * @brief Function to implement the floating-point Radix-2 Complex FFT
 *        Functions
 * @param[in]       *src     points to the input block data src[2 * k] and
 *                           src[2 * k + 1] contain the real and imaginary
 *                           parts of the k-th data.
 * @param[in]       m        If not defined STATIC_VERSION, the data buffer
 *                           is of size 2 * n while n = 2 ** m.
 * @return valaue   0  success.
 *                  -1 invalid range.
 */

/* function description */
/**
 * @brief Function to implement the floating-point Radix-2 Complex IFFT
 *        Functions
 * @param[in]       *src     points to the input block data src[2 * k] and
 *                           src[2 * k + 1] contain the real and imaginary
 *                           parts of the k-th data.
 * @param[in]       m        If not defined STATIC_VERSION, the data buffer
 *                           is of size 2 * n while n = 2 ** m.
 * @return valaue   0  success.
 *                  -1 invalid range.
 */

int32_t riscv_dsp_cfft_8pt_f32(float32_t *src, uint32_t m)
{
    (void) m;
    float32_t tmp_cal_float_r[8];
    float32_t tmp_cal_float_i[8];
    float32_t TW_81 = 0.70710678f;
    float32_t tmp_cpx_r[16];
    float32_t tmp_cpx_i[16];
    
    tmp_cal_float_r[0] = src[0] + src[8];
    tmp_cal_float_i[0] = src[1] + src[9];
    tmp_cal_float_r[1] = src[0] - src[8];
    tmp_cal_float_i[1] = src[1] - src[9];
    tmp_cal_float_r[2] = src[2] + src[10];
    tmp_cal_float_i[2] = src[3] + src[11];
    tmp_cal_float_r[3] = src[2] - src[10];
    tmp_cal_float_i[3] = src[3] - src[11];
    tmp_cal_float_r[4] = src[4] + src[12];
    tmp_cal_float_i[4] = src[5] + src[13];
    tmp_cal_float_r[5] = src[4] - src[12];
    tmp_cal_float_i[5] = src[5] - src[13];
    tmp_cal_float_r[6] = src[6] + src[14];
    tmp_cal_float_i[6] = src[7] + src[15];
    tmp_cal_float_r[7] = src[6] - src[14];
    tmp_cal_float_i[7] = src[7] - src[15];

    tmp_cpx_r[0] = tmp_cal_float_r[0];
    tmp_cpx_i[0] = tmp_cal_float_i[0];
    tmp_cpx_r[1] = tmp_cal_float_r[1];
    tmp_cpx_i[1] = tmp_cal_float_i[1];
    tmp_cpx_r[2] = tmp_cal_float_r[2];
    tmp_cpx_i[2] = tmp_cal_float_i[2];
    tmp_cpx_r[4] = tmp_cal_float_r[4];
    tmp_cpx_i[4] = tmp_cal_float_i[4];
    tmp_cpx_r[6] = tmp_cal_float_r[6];
    tmp_cpx_i[6] = tmp_cal_float_i[6];
    tmp_cpx_r[5] = tmp_cal_float_i[5];
    tmp_cpx_i[5] = -tmp_cal_float_r[5];
    tmp_cpx_r[3] = (tmp_cal_float_r[3] + tmp_cal_float_i[3]) * TW_81;
    tmp_cpx_i[3] = (tmp_cal_float_i[3] - tmp_cal_float_r[3]) * TW_81;
    tmp_cpx_r[7] = (tmp_cal_float_r[7] - tmp_cal_float_i[7]) * TW_81;
    tmp_cpx_i[7] = (tmp_cal_float_i[7] + tmp_cal_float_r[7]) * TW_81;
 
    tmp_cpx_r[8] = tmp_cpx_r[0] + tmp_cpx_r[4];
    tmp_cpx_i[8] = tmp_cpx_i[0] + tmp_cpx_i[4];
    tmp_cpx_r[9] = tmp_cpx_r[1] + tmp_cpx_r[5];
    tmp_cpx_i[9] = tmp_cpx_i[1] + tmp_cpx_i[5];
    tmp_cpx_r[10] = tmp_cpx_r[0] - tmp_cpx_r[4];
    tmp_cpx_i[10] = tmp_cpx_i[0] - tmp_cpx_i[4];
    tmp_cpx_r[11] = tmp_cpx_r[1] - tmp_cpx_r[5];
    tmp_cpx_i[11] = tmp_cpx_i[1] - tmp_cpx_i[5];
 
    tmp_cpx_r[12] = tmp_cpx_r[2] + tmp_cpx_r[6];
    tmp_cpx_i[12] = tmp_cpx_i[2] + tmp_cpx_i[6];
    tmp_cpx_r[13] = tmp_cpx_r[3] - tmp_cpx_r[7];
    tmp_cpx_i[13] = tmp_cpx_i[3] - tmp_cpx_i[7];
    tmp_cpx_r[14] = tmp_cpx_r[2] - tmp_cpx_r[6];
    tmp_cpx_i[14] = tmp_cpx_i[2] - tmp_cpx_i[6];
    tmp_cpx_r[15] = tmp_cpx_r[3] + tmp_cpx_r[7];
    tmp_cpx_i[15] = tmp_cpx_i[3] + tmp_cpx_i[7];

    src[0]  = tmp_cpx_r[8] + tmp_cpx_r[12];
    src[1]  = tmp_cpx_i[8] + tmp_cpx_i[12];
    src[2]  = tmp_cpx_r[9] + tmp_cpx_r[13];
    src[3]  = tmp_cpx_i[9] + tmp_cpx_i[13];
    src[4]  = tmp_cpx_r[10] + tmp_cpx_i[14];
    src[5]  = tmp_cpx_i[10] - tmp_cpx_r[14];
    src[6]  = tmp_cpx_r[11] + tmp_cpx_i[15];
    src[7]  = tmp_cpx_i[11] - tmp_cpx_r[15];
    src[8]  = tmp_cpx_r[8] - tmp_cpx_r[12];
    src[9]  = tmp_cpx_i[8] - tmp_cpx_i[12];
    src[10] = tmp_cpx_r[9] - tmp_cpx_r[13];
    src[11] = tmp_cpx_i[9] - tmp_cpx_i[13];
    src[12] = tmp_cpx_r[10] - tmp_cpx_i[14];
    src[13] = tmp_cpx_i[10] + tmp_cpx_r[14];
    src[14] = tmp_cpx_r[11] - tmp_cpx_i[15];
    src[15] = tmp_cpx_i[11] + tmp_cpx_r[15];

    return 0;
}

int32_t riscv_dsp_cfft_4pt_f32(float32_t *src, uint32_t m)
{
    // used for 3pts rfft
    (void) m;
    float32_t tmp_cal_float_r[4];
    float32_t tmp_cal_float_i[4];

    tmp_cal_float_r[0] = src[0] + src[4];
    tmp_cal_float_i[0] = src[1] + src[5];
    tmp_cal_float_r[1] = src[0] - src[4];
    tmp_cal_float_i[1] = src[1] - src[5];

    tmp_cal_float_r[2] = src[2] + src[6];
    tmp_cal_float_i[2] = src[3] + src[7];
    tmp_cal_float_r[3] = src[2] - src[6];
    tmp_cal_float_i[3] = src[3] - src[7];

    src[0] = tmp_cal_float_r[0] + tmp_cal_float_r[2];
    src[1] = tmp_cal_float_i[0] + tmp_cal_float_i[2];

    src[2] = tmp_cal_float_r[1] + tmp_cal_float_i[3];
    src[3] = tmp_cal_float_i[1] - tmp_cal_float_r[3];

    src[4] = tmp_cal_float_r[0] - tmp_cal_float_r[2];
    src[5] = tmp_cal_float_i[0] - tmp_cal_float_i[2];

    src[6] = tmp_cal_float_r[1] - tmp_cal_float_i[3];
    src[7] = tmp_cal_float_i[1] + tmp_cal_float_r[3];

    return 0;

}

int32_t riscv_dsp_cifft_8pt_f32(float32_t *src, uint32_t m)
{
    (void) m;
    float32_t tmp_cal_float_r[8];
    float32_t tmp_cal_float_i[8];
    float32_t TW_81 = 0.70710678f;
    float32_t tmp_cpx_r[16];
    float32_t tmp_cpx_i[16];
    float32_t inv_factor = (float32_t)1.0f/ 8.0f ;

    tmp_cal_float_r[0] = src[0] + src[8];
    tmp_cal_float_i[0] = src[1] + src[9];
    tmp_cal_float_r[1] = src[0] - src[8];
    tmp_cal_float_i[1] = src[1] - src[9];
    tmp_cal_float_r[2] = src[2] + src[10];
    tmp_cal_float_i[2] = src[3] + src[11];
    tmp_cal_float_r[3] = src[2] - src[10];
    tmp_cal_float_i[3] = src[3] - src[11];
    tmp_cal_float_r[4] = src[4] + src[12];
    tmp_cal_float_i[4] = src[5] + src[13];
    tmp_cal_float_r[5] = src[4] - src[12];
    tmp_cal_float_i[5] = src[5] - src[13];
    tmp_cal_float_r[6] = src[6] + src[14];
    tmp_cal_float_i[6] = src[7] + src[15];
    tmp_cal_float_r[7] = src[6] - src[14];
    tmp_cal_float_i[7] = src[7] - src[15];

    tmp_cpx_r[0] = tmp_cal_float_r[0];
    tmp_cpx_i[0] = tmp_cal_float_i[0];
    tmp_cpx_r[1] = tmp_cal_float_r[1];
    tmp_cpx_i[1] = tmp_cal_float_i[1];
    tmp_cpx_r[2] = tmp_cal_float_r[2];
    tmp_cpx_i[2] = tmp_cal_float_i[2];
    tmp_cpx_r[4] = tmp_cal_float_r[4];
    tmp_cpx_i[4] = tmp_cal_float_i[4];
    tmp_cpx_r[6] = tmp_cal_float_r[6];
    tmp_cpx_i[6] = tmp_cal_float_i[6];
    tmp_cpx_r[5] = -tmp_cal_float_i[5];
    tmp_cpx_i[5] = tmp_cal_float_r[5];
    tmp_cpx_r[3] = (tmp_cal_float_r[3] - tmp_cal_float_i[3]) * TW_81;
    tmp_cpx_i[3] = (tmp_cal_float_i[3] + tmp_cal_float_r[3]) * TW_81;
    tmp_cpx_r[7] = (tmp_cal_float_r[7] + tmp_cal_float_i[7]) * TW_81;
    tmp_cpx_i[7] = (tmp_cal_float_i[7] - tmp_cal_float_r[7]) * TW_81;

    tmp_cpx_r[8] = tmp_cpx_r[0] + tmp_cpx_r[4];
    tmp_cpx_i[8] = tmp_cpx_i[0] + tmp_cpx_i[4];
    tmp_cpx_r[9] = tmp_cpx_r[1] + tmp_cpx_r[5];
    tmp_cpx_i[9] = tmp_cpx_i[1] + tmp_cpx_i[5];
    tmp_cpx_r[10] = tmp_cpx_r[0] - tmp_cpx_r[4];
    tmp_cpx_i[10] = tmp_cpx_i[0] - tmp_cpx_i[4];
    tmp_cpx_r[11] = tmp_cpx_r[1] - tmp_cpx_r[5];
    tmp_cpx_i[11] = tmp_cpx_i[1] - tmp_cpx_i[5];

    tmp_cpx_r[12] = tmp_cpx_r[2] + tmp_cpx_r[6];
    tmp_cpx_i[12] = tmp_cpx_i[2] + tmp_cpx_i[6];
    tmp_cpx_r[13] = tmp_cpx_r[3] - tmp_cpx_r[7];
    tmp_cpx_i[13] = tmp_cpx_i[3] - tmp_cpx_i[7];
    tmp_cpx_r[14] = tmp_cpx_r[2] - tmp_cpx_r[6];
    tmp_cpx_i[14] = tmp_cpx_i[2] - tmp_cpx_i[6];
    tmp_cpx_r[15] = tmp_cpx_r[3] + tmp_cpx_r[7];
    tmp_cpx_i[15] = tmp_cpx_i[3] + tmp_cpx_i[7];

    src[0]  = (tmp_cpx_r[8] + tmp_cpx_r[12]) * inv_factor;
    src[1]  = (tmp_cpx_i[8] + tmp_cpx_i[12]) * inv_factor;
    src[2]  = (tmp_cpx_r[9] + tmp_cpx_r[13]) * inv_factor;
    src[3]  = (tmp_cpx_i[9] + tmp_cpx_i[13]) * inv_factor;
    src[4]  = (tmp_cpx_r[10] - tmp_cpx_i[14]) * inv_factor;
    src[5]  = (tmp_cpx_i[10] + tmp_cpx_r[14]) * inv_factor;
    src[6]  = (tmp_cpx_r[11] - tmp_cpx_i[15]) * inv_factor;
    src[7]  = (tmp_cpx_i[11] + tmp_cpx_r[15]) * inv_factor;
    src[8]  = (tmp_cpx_r[8] - tmp_cpx_r[12]) * inv_factor;
    src[9]  = (tmp_cpx_i[8] - tmp_cpx_i[12]) * inv_factor;
    src[10] = (tmp_cpx_r[9] - tmp_cpx_r[13]) * inv_factor;
    src[11] = (tmp_cpx_i[9] - tmp_cpx_i[13]) * inv_factor;
    src[12] = (tmp_cpx_r[10] + tmp_cpx_i[14]) * inv_factor;
    src[13] = (tmp_cpx_i[10] - tmp_cpx_r[14]) * inv_factor;
    src[14] = (tmp_cpx_r[11] + tmp_cpx_i[15]) * inv_factor;
    src[15] = (tmp_cpx_i[11] - tmp_cpx_r[15]) * inv_factor;

    return 0;
}

int32_t riscv_dsp_cifft_4pt_f32(float32_t *src, uint32_t m)
{
    // used for 3pts rfft
    (void) m;
    float32_t inv_factor = 1.0f / 4.0f;
    float32_t tmp_cal_float_r[4];
    float32_t tmp_cal_float_i[4];

    tmp_cal_float_r[0] = src[0] + src[4];
    tmp_cal_float_i[0] = src[1] + src[5];
    tmp_cal_float_r[1] = src[0] - src[4];
    tmp_cal_float_i[1] = src[1] - src[5];

    tmp_cal_float_r[2] = src[2] + src[6];
    tmp_cal_float_i[2] = src[3] + src[7];
    tmp_cal_float_r[3] = src[2] - src[6];
    tmp_cal_float_i[3] = src[3] - src[7];

    src[0] = (tmp_cal_float_r[0] + tmp_cal_float_r[2]) * inv_factor;
    src[1] = (tmp_cal_float_i[0] + tmp_cal_float_i[2]) * inv_factor;

    src[2] = (tmp_cal_float_r[1] - tmp_cal_float_i[3]) * inv_factor;
    src[3] = (tmp_cal_float_i[1] + tmp_cal_float_r[3]) * inv_factor;

    src[4] = (tmp_cal_float_r[0] - tmp_cal_float_r[2]) * inv_factor;
    src[5] = (tmp_cal_float_i[0] - tmp_cal_float_i[2]) * inv_factor;

    src[6] = (tmp_cal_float_r[1] + tmp_cal_float_i[3]) * inv_factor;
    src[7] = (tmp_cal_float_i[1] - tmp_cal_float_r[3]) * inv_factor;

    return 0;

}




/**
 * @} end of cfft_radix2
 */
