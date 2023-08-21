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
 * @addtogroup mean
 * @{
 */

/**
 * @brief Mean value of the u8 vector.
 * @param[in]       *src    points to the input vector.
 * @param[in]       size    size of the vectors.
 * @return mean value.
 *
 * The 8-bit format input is accumulated in a 32-bit accumulator
 * and then truncated to yield a result of 8-bit format.
 */

/* function description */
uint8_t riscv_dsp_mean_u8(const uint8_t *src, uint32_t size)
{
    uint32_t sum = 0, dst;
    uint32_t i = size;

    while (i != 0u)
    {
        sum += *src++;
        i--;
    }

    dst = (uint8_t)(sum / size);
    return dst;

}

/**
 * @} end of mean.
 */
