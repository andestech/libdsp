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
#include <math.h> //fabs

/**
 * @ingroup statistics
 */

/**
 * @defgroup min Minimum
 *
 * Computes the minimum value of absolute values of an array of data.
 * Minimum is definied as the smallest value of a array or a vector. This
 * function returns the smallest value and its position index in the vector.
 */

/**
 * @addtogroup AbsMin
 * @{
 */

/**
 * @brief           Minimum value of absolute values of a floating-point vector.
 * @param[in]       *src    points to the input vector.
 * @param[in]       size    size of the vectors.
 * @param[out]      *index  index of the minimum value.
 * @return minimum value.
 */

/* function description */
float64_t riscv_dsp_absmin_f64(const float64_t * FUNC_RESTRICT src, uint32_t size,
                               uint32_t * FUNC_RESTRICT index)
{
    float64_t min_val, temp_val;
    uint32_t min_index, i;

    //First, check if input size is zero?
    if (size <= 0)
    {
        *index = 0;
        return (float64_t) -1.0;
    }
    i = min_index = 0;
    min_val = fabs(*src++);

    uint32_t cnt = (size - 1) >> 1;
    float64_t temp_val2;
    while (cnt != 0)
    {
        temp_val = fabs(*src++);
        temp_val2 = fabs(*src++);
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
        cnt--;
    }
    uint32_t rest_cnt =  (size - 1) & 1;
    while (rest_cnt != 0)
    {
        temp_val = fabs(*src++);
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
