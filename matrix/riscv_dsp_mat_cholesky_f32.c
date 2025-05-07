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
#include <math.h> //sqrtf

int riscv_dsp_mat_cholesky_f32(float32_t* FUNC_RESTRICT src, float32_t* FUNC_RESTRICT dst, uint32_t row, uint32_t col)
{
    //check if the input row or col is zero, and should be square matrix.
    if ((row <= 0) || (col <= 0) || (row != col))
    {
        return -1;
    }
    int i, j, k;
    int n = row;
    float32_t invSqrtVj, sum;
    const float32_t* pA;
    float32_t* pG;

    pA = src;
    pG = dst;
    for (i = 0; i < n; i++)
    {
        for (j = i; j < n; j++)
        {
            sum = 0.0f;
            for (k = 0; k < i; k++)
            {
                sum += pG[i * n + k] * pG[j * n + k];
            }
            pG[j * n + i] = pA[j * n + i] - sum;
        }

        if (pG[i * n + i] <= 0.0f)
        {
            return -1;
        }

        invSqrtVj = 1.0f / sqrtf(pG[i * n + i]);
        for (j = i; j < n; j++)
        {
            pG[j * n + i] = pG[j * n + i] * invSqrtVj;
        }
    }
    return 0;
}
