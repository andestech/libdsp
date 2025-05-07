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
 * @addtogroup mean
 * @{
 */

/**
 * @brief Mean value of the q15 vector.
 * @param[in]       *src    points to the input vector.
 * @param[in]       size    size of the vectors.
 * @return mean value.
 *
 * <b>Function notes:</b>
 *
 * The 1.15 format input is accumulated in a 32-bit accumulator in 17.15
 * format and then truncated to yield a result of 1.15 format.
 */

/* function description */
q15_t riscv_dsp_mean_q15(const q15_t *src, uint32_t size)
{
    q31_t sum = 0, dst;
    uint32_t i = size;

    while (i != 0u)
    {
        sum += *src++;
        i--;
    }

    dst = (q15_t)(sum / (int32_t)size);
    return dst;
}

/**
 * @} end of mean.
 */
