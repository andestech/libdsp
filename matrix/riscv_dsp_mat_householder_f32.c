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

#include "internal_nds_types.h"
#include <string.h>
#include <math.h>
#include "riscv_dsp_basic_math.h"

/* function description */
float32_t riscv_dsp_mat_householder_f32(const float32_t * FUNC_RESTRICT src, const float32_t threshold, uint32_t size, float32_t * FUNC_RESTRICT dst)
{
    uint32_t i;
    float32_t epsilon;
    float32_t x1norm2, alpha;
    float32_t beta, tau, r;

    epsilon = threshold;

    alpha = src[0];

    for(i = 1; i < size; i++)
    {
        dst[i] = src[i];
    }
    dst[0] = 1.0f;

    x1norm2 = riscv_dsp_dprod_f32((float32_t *)(src + 1), (float32_t *)(src + 1), (size - 1));

    if (x1norm2 <= epsilon)
    {
        tau = 0.0f;
        memset(dst, 0, size * sizeof(float32_t));
    }
    else
    {
        beta =  alpha * alpha + x1norm2;
        beta = sqrtf(beta);

        /* To avoid subtraction and possible cancelation error */
        if (alpha > 0.0f)
        {
            beta = -beta;
        }

        r = 1.0f / (alpha - beta);
        riscv_dsp_scale_f32((float32_t *)src, r, dst, size);
        dst[0] = 1.0f;

        tau = (beta - alpha) / beta;
    }

    return(tau);
}
