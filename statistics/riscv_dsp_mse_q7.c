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
#ifndef NDS_CPU_64
#include "internal_utils_math.h"
#endif

/* function description */
void riscv_dsp_mse_q7(const q7_t * FUNC_RESTRICT src1, const q7_t * FUNC_RESTRICT src2, uint32_t size, q7_t * result)
{
    uint32_t loop;
    q31_t sum = 0;
    q7_t tmp1, tmp2;
    loop = size;

    while (loop != 0u)
    {
        tmp1 = *src1++ >> 1;
        tmp2 = *src2++ >> 1;
        tmp1 = (q7_t) NDS_ISA_SATS(((q15_t)tmp1 - (q15_t)tmp2), 8);
        sum += ((q15_t) tmp1 * tmp1);
        loop--;
    }
    *result = (q7_t) NDS_ISA_SATS((q15_t) (sum / size) >> 5, 8);

}
