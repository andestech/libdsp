/******************************************************************************
 * Copyright (C) 2010-2023 Arm Limited or its affiliates. All rights reserved.*
 * Copyright (C) 2018-2023 Andes Technology Corporation. All rights reserved. *
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

#include "internal_isa.h"

/**
 * @ingroup basic
 */

/**
 * @addtogroup scale
 * @{
 */

/**
 * @brief To multiply a q31 vectors by a q31 scale.
 * @param[in]  *src points to the input vector.
 * @param[in]  scalefract: is the fractional portion value to be multiplied.
 * @param[in]  shift: number of bits to shift.
 * @param[out] *dst points to the output vector.
 * @param[in]  size size of the vectors.
 * @return none.
 *
 * The inputs are multiplied to yield a 2.62 output and then are shift with
 * saturation to 1.31 formats.
 */

/* function description */
void riscv_dsp_scale_q31(q31_t * FUNC_RESTRICT src, q31_t scalefract, int8_t shift, q31_t * FUNC_RESTRICT dst,
                         uint32_t size)
{
    int32_t shift1 = 31 - shift;
    int32_t shift2 = shift + 1;

    if (shift2 >= 0)
    {
        uint32_t size2 = size >> 1;
        uint32_t rest_count = size & 1UL;
        int32_t cur_max = 1L << (shift1 - 1);
        int32_t cur_min = -cur_max;

        while (size2 != 0)
        {
            q63_t out1 = (q63_t) (*src++) * scalefract;
            q63_t out2 = (q63_t) (*src++) * scalefract;
            q31_t tmp1 = (q31_t)(out1 >> shift1);
            q31_t tmp2 = (q31_t)(out2 >> shift1);
#if defined(NDS_CPU_32) && defined(__riscv)
            q31_t tmp11 = (q31_t)(out1 >> 32);
            q31_t tmp21 = (q31_t)(out2 >> 32);
            if (tmp11 != (tmp1 >> shift2))
                tmp1 = 0x7FFFFFFF ^ (tmp11 >> 31);
            if (tmp21 != (tmp2 >> shift2))
                tmp2 = 0x7FFFFFFF ^ (tmp21 >> 31);
#else
            if ((out1 > 0) && ((q31_t)(out1 >> 32) >= cur_max))
            {
                tmp1 = INT32_MAX;
            }
            else if ((out1 < 0) && ((q31_t)(out1 >> 32) <= cur_min))
            {
                tmp1 = INT32_MIN;
            }

            if ((out2 > 0) && ((q31_t)(out2 >> 32) >= cur_max))
            {
                tmp2 = INT32_MAX;
            }
            else if ((out2 < 0) && ((q31_t)(out2 >> 32) <= cur_min))
            {
                tmp2 = INT32_MIN;
            }
#endif
            *dst++ = tmp1;
            *dst++ = tmp2;
            size2--;
        }

        if (rest_count)
        {
            q63_t out1 = (q63_t) (*src) * scalefract;
            q31_t tmp1 = (q31_t)(out1 >> shift1);
            if ((out1 > 0) && ((q31_t)(out1 >> 32) >= cur_max))
            {
                tmp1 = INT32_MAX;
            }
            else if ((out1 < 0) && ((q31_t)(out1 >> 32) <= cur_min))
            {
                tmp1 = INT32_MIN;
            }
            *dst = tmp1;
        }
    }
    else
    {
        while (size != 0u)
        {
            /* y = x * scale then saturated in Q31 range */
            q31_t tmp = ((q63_t) * src++ * scalefract) >> 32;
            *dst++ = tmp >> shift1;
            size--;
        }
    }
}

/**
 * @} end of scale
 */
