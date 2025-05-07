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
int riscv_dsp_mat_solve_ut_f16(float16_t* FUNC_RESTRICT ut, float16_t* FUNC_RESTRICT A, float16_t* FUNC_RESTRICT dst, uint32_t row, uint32_t col)
{
    //check if the input row or col is zero, and should be square matrix.
    if ((row != col))
    {
        return -1;
    }
    int32_t i, j, k;
    float16_t* pX = dst;
    float16_t* pUT = ut;
    float16_t* pA = A;

    float16_t* ut_row;
    float16_t* a_col;
    for (j = 0; j < (int32_t)col; j++)
    {
        a_col = &pA[j];

        for (i = row - 1; i >= 0; i--)
        {
            float16_t tmp = a_col[i * col];

            ut_row = &pUT[row * i];

            for (k = row - 1; k > i; k--)
            {
                tmp -= ut_row[k] * pX[col * k + j];
            }

            if (ut_row[i] == 0.0f)
            {
                return -1; //The matrix is singular
            }
            tmp = tmp / ut_row[i];
            pX[i * col + j] = tmp;
        }
    }
    return 0;
}
