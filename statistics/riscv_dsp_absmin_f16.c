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
#include <math.h> //fabsf

/* function description */
float16_t riscv_dsp_absmin_f16(const float16_t * FUNC_RESTRICT src, uint32_t size,
                               uint32_t * FUNC_RESTRICT index)
{
    float16_t min_val, temp_val;
    uint32_t min_index, i;

    //First, check if input size is zero?
    if (size <= 0)
    {
        *index = 0;
        return (float16_t) -1.0;
    }
    i = min_index = 0;
    min_val = fabsf(*src++);

    uint32_t cnt = (size - 1) >> 2;
    float16_t temp_val2, temp_val3, temp_val4;
    while (cnt != 0)
    {
        temp_val = fabsf(*src++);
        temp_val2 = fabsf(*src++);
        temp_val3 = fabsf(*src++);
        temp_val4 = fabsf(*src++);
        i++;
        if (min_val > temp_val)
        {
            min_index = i;
            min_val = temp_val;
        }
        i++;
        if (min_val > temp_val2)
        {
            min_index = i;
            min_val = temp_val2;
        }
        i++;
        if (min_val > temp_val3)
        {
            min_index = i;
            min_val = temp_val3;
        }
        i++;
        if (min_val > temp_val4)
        {
            min_index = i;
            min_val = temp_val4;
        }
        cnt--;
    }
    uint32_t rest_cnt =  (size - 1) & 3;
    while (rest_cnt != 0)
    {
        temp_val = fabsf(*src++);
        i++;
        if (min_val > temp_val)
        {
            min_index = i;
            min_val = temp_val;
        }
        rest_cnt--;
    }
    *index = min_index;
    return min_val;

}

/**
 * @} end of min.
 */
