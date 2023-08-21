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

/* function description */
void riscv_dsp_mse_q31(const q31_t * FUNC_RESTRICT src1, const q31_t * FUNC_RESTRICT src2, uint32_t size, q31_t * result)
{
    uint32_t loop;
    q63_t sum = 0;

    loop = size;
    q31_t tmp1, tmp2;
    while (loop != 0u)
    {
        tmp1 = *src1++ >> 1;
        tmp2 = *src2++ >> 1;
        tmp1 = NDS_ISA_KSUBW(tmp1, tmp2);
        sum += ((q63_t)tmp1 * tmp1) >> 14u;
        loop--;
    }
    *result = (q31_t) ((sum / size) >> 15);
}
