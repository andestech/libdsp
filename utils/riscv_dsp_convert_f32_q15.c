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
#include "internal_isa.h"
#include "internal_utils_math.h"

/**
 * @ingroup utils
 */

/**
 * @defgroup convertf32 Convert f32 vector
 *
 * Convert a floating-point vector.
 */

/**
 * @addtogroup convertf32
 * @{
 */

/**
 * @brief Convert a floating-point vector to Q15.
 * @param[in]       *src the input vector point.
 * @param[out]      *dst yhe output vector point.
 * @param[in]       size size of vector.
 * @return none.
 *
 * <pre>
 *    while (size > 0)
 *    {
 *        dst = (q15_t) (src * INT16_MAX);
 *        size--;
 *    }
 * </pre>
 */

void riscv_dsp_convert_f32_q15(float32_t * FUNC_RESTRICT src, q15_t * FUNC_RESTRICT dst, uint32_t size)
{
    float32_t in;
    q31_t rst;
#ifdef NDS_CPU_64
    uint32_t cnt = size >> 2;
    float32_t in2, in3, in4;
    q31_t rst2, rst3, rst4;
    while (cnt != 0)
    {
        in = *src++;
        in2 = *src++;
        in3 = *src++;
        in4 = *src++;
        in *= (float32_t)(32768.0);
        in2 *= (float32_t)(32768.0);
        in3 *= (float32_t)(32768.0);
        in4 *= (float32_t)(32768.0);
        if (in >= (float32_t)0.0f)
            in += (float32_t)0.5;
        else
            in -= (float32_t)0.5;
        rst = (q31_t)in;
        if (in2 >= (float32_t)0.0f)
            in2 += (float32_t)0.5;
        else
            in2 -= (float32_t)0.5;
        rst2 = (q31_t)in2;
        if (in3 >= (float32_t)0.0f)
            in3 += (float32_t)0.5;
        else
            in3 -= (float32_t)0.5;
        rst3 = (q31_t)in3;
        if (in4 >= (float32_t)0.0f)
            in4 += (float32_t)0.5;
        else
            in4 -= (float32_t)0.5;
        rst4 = (q31_t)in4;
        *dst++ = (q15_t) NDS_ISA_SATS(rst, 16u);
        *dst++ = (q15_t) NDS_ISA_SATS(rst2, 16u);
        *dst++ = (q15_t) NDS_ISA_SATS(rst3, 16u);
        *dst++ = (q15_t) NDS_ISA_SATS(rst4, 16u);
        cnt --;
    }
    uint32_t rest_cnt = size & 3;
    while (rest_cnt != 0u)
    {
        in = *src++;
        in *= (float32_t)(32768.0);
        if (in >= (float32_t)0.0f)
            in += (float32_t)0.5;
        else
            in -= (float32_t)0.5;
        rst = (q31_t)in;
        *dst++ = (q15_t) NDS_ISA_SATS(rst, 16u);
        rest_cnt--;
    }


#else
    while (size != 0u)
    {
        in = *src++;
        in *= (float32_t)(32768.0);
        if (in >= (float32_t)0.0f)
            in += (float32_t)0.5;
        else
            in -= (float32_t)0.5;
        rst = (q31_t)in;
    *dst++ = (q15_t) NDS_ISA_SATS(rst, 16u);
        size--;
    }
#endif
}

/**
 * @} end of convertf32
 */
