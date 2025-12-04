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
#define UNROLL

/* function description */
float32_t riscv_dsp_accum_f32(const float32_t *src, uint32_t size)
{
    float32_t sum = 0.0f;
    uint32_t loop = size;
#ifdef UNROLL
    (void) loop;
    uint32_t loop2 = size >> 2;
    uint32_t rmder = size & 3;
    float32_t sum1 = 0.0f, sum2 = 0.0f;
    while (loop2 > 0U)
    {
        sum1 += *src++;
        sum2 += *src++;
        sum1 += *src++;
        sum2 += *src++;
        loop2 = loop2 - 1;
    }
    while(rmder > 0U)
    {
        sum1 += *src++;
        rmder = rmder - 1;
    }
    sum = sum1 + sum2;
#else
    while (loop > 0U)
    {
        sum += *src++;
        loop--;
    }
#endif
    return sum;
}
