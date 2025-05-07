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

/**
 * @ingroup complex
 */

/**
 * @addtogroup clx_mul_real
 * @{
 */

/**
 * @brief Multiply the q31 complex vector by a real vector.
 * @param[in]       *src  the input complex vector.
 * @param[in]       *real the input real vector.
 * @param[out]      *dst  output complex vector.
 * @param[in]       size size of the vectors.
 * @return none.
 *
 * <b>Function notes:</b>
 *
 * The function uses saturating arithmetic.
 * Output results will be saturated in Q31 range[0x80000000 0x7FFFFFFF].
 */

/* function description */
void riscv_dsp_cmul_real_q31(const q31_t * FUNC_RESTRICT src, const q31_t * FUNC_RESTRICT real,
                       q31_t * FUNC_RESTRICT dst, uint32_t size)
{

    const q31_t min_q31 = (q31_t) INT32_MIN;
    const q31_t max_q31 = (q31_t) INT32_MAX;

    q31_t a, b;

    while (size != 0u)
    {
        /* y[2 * i] = x[2 * i] * real[i] */
        /* y[2 * i + 1] = x[2 * i + 1] * real[i] */
        //only when both are 0x80000000 will cause overflow.
        a = *real++;

        b = *src++;

#ifdef ENA_HIGHER_PERFORMANCE
        //The performance is better than 3cm with same error (1 bit error).
        *dst++ = ((min_q31 ^ a) | (min_q31 ^ b)) ?  (q31_t)((((q63_t) a * b) >> 32)) << 1 : max_q31;
#else
        *dst++ = ((min_q31 ^ a) | (min_q31 ^ b)) ?  ((q63_t) a * b) >> 31 : max_q31;
#endif

        b = *src++;
#ifdef ENA_HIGHER_PERFORMANCE
        //The performance is better than 3cm with same error (1 bit error).
        *dst++ = ((min_q31 ^ a) | (min_q31 ^ b)) ?  (q31_t)((((q63_t) a * b) >> 32)) << 1 : max_q31;
#else
        *dst++ = ((min_q31 ^ a) | (min_q31 ^ b)) ?  ((q63_t) a * b) >> 31 : max_q31;
#endif

        size--;
    }
}

/**
 * @} end of clx_mul_real
 */
