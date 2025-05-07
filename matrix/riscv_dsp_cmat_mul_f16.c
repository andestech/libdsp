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

#include "internal_math_types.h"		//for union64_t
#include "internal_isa.h"

typedef union
{
    struct
    {
        float16_t re;
        float16_t im;
    };
} union_clx_f16_t;

/* function description */
void riscv_dsp_cmat_mul_f16(const float16_t * FUNC_RESTRICT src1, const float16_t * FUNC_RESTRICT src2,
                            float16_t * FUNC_RESTRICT dst, uint32_t row, uint32_t col, uint32_t col2)
{
    uint32_t j;
    const float16_t *Aend = src1 + (row * col * 2);
    float16_t sum_re;
    float16_t sum_im;
    do
    {
        const union_clx_f16_t *B = (union_clx_f16_t *)src2;
        /* Dot product of each row in src1 with each column in src2 */
        j = col2;
        do
        {
            const union_clx_f16_t *A = (union_clx_f16_t *)src1;
            const union_clx_f16_t *A2 = (union_clx_f16_t *)(A + col);
            sum_re = 0;
            sum_im = 0;
            /* column loop */
            union_clx_f16_t a, b;
            do
            {
                //  If V3_O3 and unrolling, the insts as well as cycles are 42882/45203,
                //  and the performance is better than Keil.(Keil's cycle for CM3_O3 is 45388.)
                a = *A++;
                b = *B;
                sum_re += a.re * b.re - a.im * b.im;
                sum_im += a.re * b.im + a.im * b.re;
                B += (col2);
            }
            while (A != A2);

            *dst++ = sum_re;
            *dst++ = sum_im;
            B = (union_clx_f16_t *)(src2 + 2 * (col2 - (--j)));
        }
        while (j != 0u);

        src1 += (col * 2);
    }
    while (src1 != Aend);
}
