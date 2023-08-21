/******************************************************************************
 * Copyright (C) 2010-2023 Arm Limited or its affiliates. All rights reserved.*
 * Copyright (C) 2018-2023 Andes Technology Corporation. All rights reserved. *
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

#include "internal_isa.h"

extern q31_t riscv_dsp_sqrt_q31(q31_t src);

void riscv_dsp_cmag_hp_q15(const q15_t * FUNC_RESTRICT src, q15_t * FUNC_RESTRICT dst, uint32_t size)
{

    q15_t a, b;
    do
    {
        /* y = sqrt(x[(2 * n) + 0] ^ 2 + x[(2 * n) + 1] ^ 2) */
        a = *src++;
        b = *src++;
        /* store the output in 2.14 format */
        q31_t acc0, acc1;
        acc0 = ((q31_t) a * a);
        acc1 = ((q31_t) b * b);
        q31_t out = riscv_dsp_sqrt_q31((acc0 + acc1) >> 1);
        *dst++ = out >> 16;
        size--;
    }
    while (size != 0u);

}
