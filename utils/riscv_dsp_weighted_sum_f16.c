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
float16_t riscv_dsp_weighted_sum_f16(const float16_t * FUNC_RESTRICT src, const float16_t * FUNC_RESTRICT weight, uint32_t size)
{
    uint32_t cnt = size;
    float16_t acc1, acc2, out;

    acc1 = 0.0f;
    acc2 = 0.0f;
    while (cnt != 0)
    {
        acc1 += *src++ * *weight;
        acc2 += *weight++;
        cnt--;
    }
    out = acc1 / acc2;
    return out;
}
