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
void riscv_dsp_mat_add_f16(const float16_t * FUNC_RESTRICT src1, const float16_t * FUNC_RESTRICT src2,
                           float16_t * FUNC_RESTRICT dst, uint32_t row, uint32_t col)
{
    uint32_t size = row * col;
    const float16_t *dsttop = dst + size;

    do
    {
        /* C(m,n) = A(m,n) + B(m,n) */
        *dst++ = (*src1++) + (*src2++);
    }
    while (dst != dsttop);
}
