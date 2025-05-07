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

/**
 * @ingroup statistics
 */

/**
 * @defgroup mean Mean
 *
 * The mean of a sample is sum the sampled values then divided by the
 * number of items in the sample.
 *
 * <pre>
 *      dst = (src[0] + src[1] + src[2] + ... + src[size - 1]) / size
 * </pre>
 */

/**
 * @addtogroup mean
 * @{
 */

/**
 * @brief Mean value of the floating-potint vector.
 * @param[in]       *src    points to the input vector.
 * @param[in]       size    size of the vectors.
 * @return mean value.
 */

/* function description */
float32_t riscv_dsp_mean_f32(const float32_t *src, uint32_t size)
{
    float32_t dst = 0.0f;

    if (size != 0)
    {
        float32_t sum = 0.0f;
        uint32_t i = size;

        do
        {
            sum += *src++;
        }
        while (--i);
        dst = sum / (float32_t) size;
    }
    return dst;

}

/**
 * @} end of mean.
 */
