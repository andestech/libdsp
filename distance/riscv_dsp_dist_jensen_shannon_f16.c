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
#include <math.h>

extern float16_t riscv_dsp_sqrt_f16(float16_t src);

float16_t riscv_dsp_dist_jensen_shannon_f16(const float16_t * FUNC_RESTRICT src1, const float16_t * FUNC_RESTRICT src2, uint32_t size)
{
    float16_t tmp1, tmp2, out, tmp;
    float16_t in1, in2;
    tmp1 = 0.0f;
    tmp2 = 0.0f;

    while (size != 0)
    {
        in1 = *src1++;
        in2 = *src2++;
        tmp = (in1 + in2) / 2.0f;
        tmp1 += (in1 * logf(in1 / tmp));
        tmp2 += (in2 * logf(in2 / tmp));
        size--;
    }
    tmp = tmp1 + tmp2;
    out = riscv_dsp_sqrt_f16(tmp / 2.0);
    return out;
}
