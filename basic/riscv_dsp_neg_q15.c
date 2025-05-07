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
#include "internal_isa.h"

/**
 * @ingroup basic
 */

/**
 * @addtogroup basicneg
 * @{
 */

/**
 * @brief Q15 neg
 * @param[in]       *src points to the input vector.
 * @param[out]      *dst points to the output vector.
 * @param[in]       size size of the vectors.
 * @return none.
 *
 * The Q15 value INT16_MIN (0x8000) will be saturated to the maximum
 * allowable positive value INT16_MAX.
 */

/* function description */
void riscv_dsp_neg_q15(q15_t * FUNC_RESTRICT src, q15_t * FUNC_RESTRICT dst, uint32_t size)
{
    q15_t a;

    while (size != 0u)
    {
        a = *src++;
        //For v3m, if ENA_EXT_SAT_INST is on, the instructions and cycles are 5129/9227. (Keil's cycle is 13344.)
        //Otherwise, the instructions and cycles are 11279/15382.
        //For v3m, if using NDS_ISA_SATS instead of NDS_ISA_KSUBH, the instructions and cycles are 11279/15382.
        *dst++ = (q15_t) NDS_ISA_KSUBH(0, a);

        size--;
    }
}

/**
 * @} end of basicneg
 */
