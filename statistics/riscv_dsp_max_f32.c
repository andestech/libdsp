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
 * @defgroup max Maximum
 *
 * Maximum is definied as the largest value of a array or a vector. This
 * function returns the largest value and its position index in the vector.
 */

/**
 * @addtogroup max
 * @{
 */

/**
 * @brief Max value of the floating-potint vector.
 * @param[in]       *src    points to the input vector.
 * @param[in]       size    size of the vectors.
 * @param[out]      *index  index of the maximum value.
 * @return max value.
 */

/* function description */
float32_t riscv_dsp_max_f32(const float32_t * FUNC_RESTRICT src, uint32_t size,
                            uint32_t * FUNC_RESTRICT index)
{
    float32_t max_val, temp_val;
    uint32_t max_index, i;

    //First, check if input size is zero?
    if (size <= 0)
    {
        *index = 0;
        return (float32_t)0.0;
    }

    i = max_index = 0;
    max_val = *src++;
#ifdef NDS_CPU_64
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

#else
    //Using one if
    //Performance (V3+O3): 22534/27668, use __cmpsf2
    while (i < (size - 1))
    {
        temp_val = *src++;
        i++;
        if (max_val < temp_val)
        {
            max_index = i;
            max_val = temp_val;
        }
    }
#endif

    *index = max_index;
    return max_val;

}

/**
 * @} end of max.
 */
