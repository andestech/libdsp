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
#include "internal_math_types.h"
#define UNROLL2

/**
 * @ingroup utils
 */

/**
 * @defgroup convertq15 Convert q15 vector
 *
 * Convert a Q15 vector.
 */

/**
 * @addtogroup convertq15
 * @{
 */

/**
 * @brief Convert a Q15 vector to floating.
 * @param[in]       *src the input vector point.
 * @param[out]      *dst the output vector point.
 * @param[in]       size size of vector.
 * @return none.
 *
 * <pre>
 *    while (size > 0)
 *    {
 *        dst = (float32_t) (src * UNIT_Q15);
 *        size--;
 *    }
 * </pre>
 */

void riscv_dsp_convert_q15_f32(q15_t * FUNC_RESTRICT src, float32_t * FUNC_RESTRICT dst, uint32_t size)
{
#ifdef UNROLL2
    float32_t r1, r2;
    while (size > 1u)
    {
        r1 =  dsp_convert_q15_to_float(*src++);
        r2 =  dsp_convert_q15_to_float(*src++);
        *dst++ = r1;
        *dst++ = r2;
        size -= 2;
    }
    while (size > 0u)
    {
        *dst++ = dsp_convert_q15_to_float(*src++);
        size --;
    }
#else
    while (size != 0u)
    {
        *dst++ = dsp_convert_q15_to_float(*src++);
        size--;
    }
#endif
}

/**
 * @} end of convertq15
 */
