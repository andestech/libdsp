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

#include "config.h"

#include "internal_isa.h"

/**
 * @ingroup matrix
 */

/**
 * @addtogroup matrixmul
 * @{
 */

#define __SMMLA(a, b, c) NDS_DSP_KMMAC(c, a, b)
/* function description */
void riscv_dsp_mat_mul_fast_q31(const q31_t * FUNC_RESTRICT src1, const q31_t * FUNC_RESTRICT src2,
                          q31_t * FUNC_RESTRICT dst, uint32_t row, uint32_t col, uint32_t col2)
{
    uint32_t j;
    const q31_t *Aend = src1 + row * col;
    do
    {
        const q31_t *B = src2;

        j = col2;
        do
        {
            const q31_t *A = src1;
            const q31_t *A2 = A + col;
            q63_t sum = 0;

            do
            {
                sum = (q31_t) ((((q63_t) sum << 32) + ((q63_t) * A++ * *B)) >> 32);
                B += col2;

            }
            while (A != A2);

            *dst++ = (sum << 1);
            B = src2 + col2 - (--j);
        }
        while (j != 0);

        src1 += col;
    }
    while (src1 != Aend);
}

/**
 * @} end of matrixmul
 */
