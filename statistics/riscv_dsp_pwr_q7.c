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
 * @addtogroup pwr
 * @{
 */

/**
 * @brief Sum of the squares of the q7 vector.
 * @param[in]       *src    points to the input vector.
 * @param[in]       size    size of the vectors.
 * @return Sum of the squares value.
 *
 * <b>Function notes:</b>
 *
 * The 1.7 format input is multiplied yields a 2.14 format, and then added
 * without saturation to a 32-bit accumulator in 18.14 format. Finally,
 * the return result is in 18.14 format.
 */

/* function description */
q31_t riscv_dsp_pwr_q7(const q7_t *src, uint32_t size)
{
    q31_t sum = 0;
    q7_t tmp;

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
