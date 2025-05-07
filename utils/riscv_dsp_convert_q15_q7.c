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

#include "internal_isa.h"
#include "internal_math_types.h"

void riscv_dsp_convert_q15_q7(q15_t * FUNC_RESTRICT src, q7_t * FUNC_RESTRICT dst, uint32_t size)
{
    uint32_t size2 = size >> 1;
    uint32_t rest_count = size & 1UL;

    while (size2 != 0u)
    {
        q15_t tempa, tempb;

        tempa = *src++;
        tempb = *src++;

        *dst++ = dsp_convert_q15_to_q7(tempa);
        *dst++ = dsp_convert_q15_to_q7(tempb);

        size2--;
    }

    if (rest_count != 0)
    {
        *dst = dsp_convert_q15_to_q7(*src);
    }
}

/**
 * @} end of convertq15
 */
