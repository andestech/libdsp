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
#include <stdio.h>  //include this is because __ULong defined in math.h when using BSP320.
//BSP400 doesn't need this file.
#include <math.h>
#include "internal_utils_math.h"
extern float32_t riscv_dsp_var_f32(const float32_t *src, uint32_t size);

/**
 * @ingroup statistics
 */

/**
 * @defgroup std Standard deviation
 *
 * In statistics and probability theory, the standard deviation(σ) shows
 * how much variation or dispersion from the average exists. This function
 * calculates the standard deviation of the elements in the input vectors.
 *
 * <pre>
 *      dst = sqrt((sos - sum ^ 2 / size) / (size - 1)),
 *      where sos(sum of the square) = src[0] * src[0] + src[1] * src[1]
 *              + src[2] * src[2] + ... + src[size-1] * src[size-1],
 *            sum = src[0] + src[1] + src[2] + ... + src[size-1].
 * </pre>
 */

/**
 * @addtogroup std
 * @{
 */

/**
 * @brief Standard deviation of the floating-potint vector.
 * @param[in]       *src    points to the input vector.
 * @param[in]       size    size of the vectors.
 * @return Standard deviation value.
 */

/* function description */
float32_t riscv_dsp_std_f32(const float32_t *src, uint32_t size)
{
    float32_t dst = 0.0f;
    float32_t tmp2 = riscv_dsp_var_f32(src, size);
    dst = (tmp2 >= 0) ? sqrtf(tmp2) : -1.0f;
    return dst;
}

/**
 * @} end of std.
 */
