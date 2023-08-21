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

u16_t riscv_dsp_min_u16(const u16_t * FUNC_RESTRICT src, uint32_t size, uint32_t * FUNC_RESTRICT index)
{
    u16_t min_val, temp_val;
    uint32_t min_index, i;

    //First, check if input size is zero?
    if (size <= 0)
    {
        *index = 0;
        return 0xffff;
    }

    i = min_index = 0U;
    min_val = (q31_t) * src++;

    while (i < (size - 1))
    {
        temp_val = *src++;
        i++;
        if (temp_val < min_val)
        {
            min_index = i;
            min_val = temp_val;
        }
    }

    *index = min_index;
    return min_val;

}
