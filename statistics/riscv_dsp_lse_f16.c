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
#include <math.h> // for logf

/* function description */
FUNC_ATTR_FFAST_MATH
float16_t riscv_dsp_lse_f16(const float16_t * FUNC_RESTRICT src, uint32_t size)
{
    float16_t max, tmp;
    float16_t acc, out;
    const float16_t *in = src;

    uint32_t cnt = size;
    max = *in++;
    cnt--;
    while (cnt != 0)
    {
        tmp = *in++;
        max = (max < tmp) ? tmp : max;
        cnt--;
    }

    cnt = size;
    in = src;
    acc = 0.0f;
    while (cnt != 0)
    {
        tmp = *in++;
        acc += expf(tmp - max);
        cnt--;
    }
    out = max + logf(acc);
    return out;
}
