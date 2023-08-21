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

/* function description */
void riscv_dsp_mat_mul_mxv_q31(const q31_t * FUNC_RESTRICT src1, const q31_t * FUNC_RESTRICT src2,
                               q31_t * FUNC_RESTRICT dst, uint32_t row, uint32_t col)
{
    const q31_t *InA = src1;
    q63_t sum;
    uint32_t colcnt, rowcnt;

    rowcnt = row;
    do
    {
        const q31_t *A = InA;
        const q31_t *B = src2;

        sum = 0;

        /* column loop */
        colcnt = col;
        do
        {
            sum += (q63_t) * A++ * *B;
            B++;
            colcnt--;
        }
        while (colcnt != 0u);

        *dst++ = (q31_t)(sum >> 31);
        InA = InA + col;
        rowcnt--;
    }
    while (rowcnt != 0u);
}

/**
 * @} end of matrixmul
 */
