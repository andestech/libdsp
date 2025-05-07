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
#include <stdio.h>  //include this is because __ULong defined in math.h when using BSP320.
//BSP400 doesn't need this file.
#include <math.h>
#include "internal_utils_math.h"

/**
 * @ingroup statistics
 */

/**
 * @defgroup rms Root Mean Square(RMS)
 *
 * RMS is a statistical measure of the magnitude of a varying quantity.
 * This function calculates the Root Mean Square of the elements in the
 * input vectors.
 *
 * <pre>
 *      dst = sqrt((src[0] * src[0] + src[1] * src[1] + src[2] * src[2]
 *          + ... + src[size-1] * src[size-1]) / size);
 * </pre>
 */

/**
 * @addtogroup rms
 * @{
 */

/**
 * @brief RMS of the floating-potint vector.
 * @param[in]       *src    points to the input vector.
 * @param[in]       size    size of the vectors.
 * @return RMS value.
 */

/* function description */
float32_t riscv_dsp_rms_f32(const float32_t *src, uint32_t size)
{
    float32_t sum = 0.0f, tmp, dst;
    uint32_t i = size;

    while (i != 0u)
    {
        tmp = *src++;
        sum += tmp * tmp;
        i--;
    }

    dst = sqrtf(sum / (float32_t) size);
    return dst;

}

/**
 * @} end of rms.
 */
