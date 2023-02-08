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
 * @defgroup pwr Sum of the Squares
 *
 * This function calculates the sum of the squares (power) of the elements in the
 * input vector.
 *
 * <pre>
 *      dst = src[0] * src[0] + src[1] * src[1] + src[2] * src[2]
 *          + ... + src[size-1] * src[size-1];
 * </pre>
 */

/**
 * @addtogroup pwr
 * @{
 */

/**
 * @brief Sum of the squares of the floating-potint vector.
 * @param[in]       *src    points to the input vector.
 * @param[in]       size    size of the vectors.
 * @return Sum of the squares value.
 */

/* function description */
float32_t riscv_dsp_pwr_f32(const float32_t *src, uint32_t size)
{
    float32_t sum = 0.0f, tmp;

    while (size != 0u)
    {
        tmp = *src++;
        sum += tmp * tmp;
        size--;
    }

    return sum;

}

/**
 * @} end of pwr.
 */
