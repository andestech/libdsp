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
#include "stdio.h"

/* function description */
void riscv_dsp_cmul_real_f16(const float16_t * FUNC_RESTRICT  src, const float16_t * FUNC_RESTRICT real,
                             float16_t * FUNC_RESTRICT dst, uint32_t size)
{
    float16_t a;

    do
    {
        /* y[2 * i] = x[2 * i] * real[i] */
        /* y[2 * i + 1] = x[2 * i + 1] * real[i] */
        a = *real++;
        *dst++ = (*src++) * a;
        *dst++ = (*src++) * a;

        size--;
    }
    while (size != 0u);
}
