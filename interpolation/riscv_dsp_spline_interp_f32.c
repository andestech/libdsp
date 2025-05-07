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
#include "riscv_dsp_interp.h"

/* function description */

void riscv_dsp_spline_interp_f32(riscv_dsp_spline_interp_f32_t * FUNC_RESTRICT instance, const float32_t * FUNC_RESTRICT src, float32_t * FUNC_RESTRICT dst, uint32_t size)
{
    const float32_t * x = instance->x;
    const float32_t * y = instance->y;
    int32_t n = instance->num_x;

    /* Coefficients (a==y for i<=n-1) */
    float32_t * b = (instance->coeffs);
    float32_t * c = (instance->coeffs) + (n - 1);
    float32_t * d = (instance->coeffs) + (2 * (n - 1));

    const float32_t * pXq = src;
    int32_t blkCnt = (int32_t)size;
    int32_t blkCnt2;
    int32_t i;
    float32_t x_sc;
    float32_t hi, hip1;

    /* Create output for x(i)<x<x(i+1) */
    for (i = 0; i < n - 1; i++)
    {
        while ( *pXq <= x[i + 1] && blkCnt > 0 )
        {
            x_sc = *pXq++;
            hi = x_sc - x[i];
            *dst = y[i] + b[i] * hi + c[i] * hi * hi + d[i] * hi * hi * hi;
            dst++;
            blkCnt--;
        }
    }

    /* Create output for remaining samples (x>=x(n)) */
    blkCnt2 = blkCnt;

    while (blkCnt2 > 0)
    {
        x_sc = *pXq++;
        hip1 = x_sc - x[i - 1];
        *dst = y[i - 1] + b[i - 1] * hip1 + c[i - 1] * hip1 * hip1 + d[i - 1] * hip1 * hip1 * hip1;
        dst++;
        blkCnt2--;
    }
}
