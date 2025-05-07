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

/* function description */
void riscv_dsp_mat_mul_mxv_f32(const float32_t * FUNC_RESTRICT src1, const float32_t * FUNC_RESTRICT src2,
                               float32_t * FUNC_RESTRICT dst, uint32_t row, uint32_t col)
{
    const float32_t *InA = src1;
    float32_t sum;
    uint32_t colcnt, rowcnt;

    rowcnt = row;
    do
    {
        const float32_t *A = InA;
        const float32_t *B = src2;

        sum = 0.0f;

        /* column loop */
        colcnt = col;
        do
        {
            sum += (*A++) * *B;
            B++;
            colcnt--;
        }
        while (colcnt != 0u);

        *dst++ = sum;
        InA = InA + col;
        rowcnt--;
    }
    while (rowcnt != 0u);
}

/**
 * @} end of matrixmul
 */
