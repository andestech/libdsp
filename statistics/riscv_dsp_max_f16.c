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
float16_t riscv_dsp_max_f16(const float16_t * FUNC_RESTRICT src, uint32_t size, uint32_t * FUNC_RESTRICT index)
{
    float16_t max_val, temp_val;
    uint32_t max_index, i;

    //First, check if input size is zero?
    if (size <= 0)
    {
        *index = 0;
        return (float16_t) -1.0;
    }

    i = max_index = 0;
    max_val = *src++;
    uint32_t cnt = (size - 1) >> 2;
    while (cnt != 0)
    {
        temp_val = *src++;
        if (max_val < temp_val)
        {
            max_index = i + 1;
            max_val = temp_val;
        }
        temp_val = *src++;
        if (max_val < temp_val)
        {
            max_index = i + 2;
            max_val = temp_val;
        }
        temp_val = *src++;
        if (max_val < temp_val)
        {
            max_index = i + 3;
            max_val = temp_val;
        }
        temp_val = *src++;
        if (max_val < temp_val)
        {
            max_index = i + 4;
            max_val = temp_val;
        }
        i += 4;
        cnt--;
    }
    uint32_t rest_cnt =  (size - 1) & 3;
    while (rest_cnt != 0)
    {
        temp_val = *src++;
        if (max_val < temp_val)
        {
            max_index = i + 1;
            max_val = temp_val;
        }
        i++;
        rest_cnt--;
    }
    *index = max_index;
    return max_val;

}
