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
 * @ingroup basic
 */

/**
 * @addtogroup basicneg
 * @{
 */

/**
 * @brief Q7 neg
 * @param[in]       *src points to the input vector.
 * @param[out]      *dst points to the output vector.
 * @param[in]       size size of the vectors.
 * @return none.
 *
 * The Q7 value INT8_MIN (0x80) will be saturated to the maximum allowable
 * positive value INT8_MAX.
 */

void riscv_dsp_neg_q7(q7_t * FUNC_RESTRICT src, q7_t * FUNC_RESTRICT dst, uint32_t size)
{
    q7_t a;

    while (size != 0u)
    {
        a = *src++;
        *dst++ = (q7_t) NDS_ISA_SATS((0 - a), 8u);

        size--;
    }
}

/**
 * @} end of basicneg
 */
