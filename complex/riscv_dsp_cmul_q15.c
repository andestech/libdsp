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

#include "internal_isa.h"

/**
 * @ingroup complex
 */

/**
 * @addtogroup clx_mul
 * @{
 */

/**
 * @brief Multiply two q15 complex vector.
 * @param[in]       *src1 the first input complex vector.
 * @param[in]       *src2 the second input complex vector.
 * @param[out]      *dst  output complex vector.
 * @param[in]       size size of the vectors.
 * @return none.
 *
 * <b>Function notes:</b>
 *
 * The function implements 1.15 by 1.15 multiplications and finally output
 * is converted into 3.13 format.
 */

/* function description */
void riscv_dsp_cmul_q15(const q15_t * FUNC_RESTRICT src1, const q15_t * FUNC_RESTRICT src2,
                  q15_t * FUNC_RESTRICT dst, uint32_t size)
{

    q15_t a, b, c, d;
    q31_t mul1, mul2, mul3, mul4;

    while (size != 0u)
    {
        /* y[2 * i] = x1[2 * i] * x2[2 * i]
         **- x1[2 * i + 1] * x2[2 * i + 1]. */
        /* y[2 * i + 1] = x1[2 * i] * x2[2 * i + 1]
         ** + x1[2 * i + 1] * x2[2 * i]. */
        a = *src1++;
        b = *src1++;
        c = *src2++;
        d = *src2++;

        mul1 = ((q31_t) a * c); // >> 17;
        mul2 = ((q31_t) b * d); // >> 17;
        mul3 = ((q31_t) a * d); // >> 17;
        mul4 = ((q31_t) b * c); // >> 17;

        *dst++ = ((q31_t) mul1 - mul2) >> 17;
        *dst++ = ((q31_t) mul3 + mul4) >> 17;

        size--;

    }

}

/**
 * @} end of clx_mul
 */
