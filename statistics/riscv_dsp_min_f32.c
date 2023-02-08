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

/**
 * @ingroup statistics
 */

/**
 * @defgroup min Minimum
 *
 * Minimum is definied as the smallest value of a array or a vector. This
 * function returns the smallest value and its position index in the vector.
 */

/**
 * @addtogroup min
 * @{
 */

/**
 * @brief Minimum value of the floating-potint vector.
 * @param[in]       *src    points to the input vector.
 * @param[in]       size    size of the vectors.
 * @param[out]      *index  index of the minimum value.
 * @return minimum value.
 */

/* function description */
float32_t riscv_dsp_min_f32(const float32_t * FUNC_RESTRICT src, uint32_t size,
                            uint32_t * FUNC_RESTRICT index)
{
    float32_t min_val, temp_val;
    uint32_t min_index, i;

    //First, check if input size is zero?
    if (size <= 0)
    {
        *index = 0;
        return (float32_t)0.0;
    }

    i = min_index = 0;
    min_val = *src++;
#ifdef NDS_CPU_64
    uint32_t cnt = (size - 1) >> 2;
    while (cnt != 0)
    {
        temp_val = *src++;
        if (temp_val < min_val)
        {
            min_index = i + 1;
            min_val = temp_val;
        }
        temp_val = *src++;
        if (temp_val < min_val)
        {
            min_index = i + 2;
            min_val = temp_val;
        }
        temp_val = *src++;
        if (temp_val < min_val)
        {
            min_index = i + 3;
            min_val = temp_val;
        }
        temp_val = *src++;
        if (temp_val < min_val)
        {
            min_index = i + 4;
            min_val = temp_val;
        }
        i += 4;
        cnt--;
    }
    uint32_t rest_cnt =  (size - 1) & 3;
    while (rest_cnt != 0)
    {
        temp_val = *src++;
        if (temp_val < min_val)
        {
            min_index = i + 1;
            min_val = temp_val;
        }
        i++;
        rest_cnt--;
    }

#else
    //Using one if
    //Performance (V3+O3): 23569/30749, use __gesf2
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
#endif
    *index = min_index;
    return min_val;

}

/**
 * @} end of min.
 */
