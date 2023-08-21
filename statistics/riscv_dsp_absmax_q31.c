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

/**
 * @ingroup statistics
 */

/**
 * @addtogroup AbsMax
 * @{
 */

/**
 * @brief           Maximum value of absolute values of a Q31 vector.
 * @param[in]       *src    points to the input vector.
 * @param[in]       size    size of the vectors.
 * @param[out]      *index  index of the maximum value.
 * @return max value.
 */

/* function description */
q31_t riscv_dsp_absmax_q31(const q31_t * FUNC_RESTRICT src, uint32_t size, uint32_t * FUNC_RESTRICT index)
{
    //First, check if input size is zero?
    if (size <= 0)
    {
        *index = 0;
        return (q31_t)0x0;
    }
    uint32_t max_index, i;
    i = max_index = 0U;

    //pureC
    q31_t max_val, temp_val;
    max_val = (*src > 0) ? *src : ((*src == INT32_MIN) ? INT32_MAX : -*src);
    src++;

    while (i < (size - 1))
    {
        temp_val = (*src > 0) ? *src : ((*src == INT32_MIN) ? INT32_MAX : -*src);
        src++;
        i++;
        if (max_val < temp_val)
        {
            max_index = i;
            max_val = temp_val;
        }
    }

    *index = max_index;
    return max_val;

}

/**
 * @} end of max.
 */
