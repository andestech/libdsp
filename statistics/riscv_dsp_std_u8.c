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
extern q15_t riscv_dsp_sqrt_q15(q15_t val_in);

/**
 * @ingroup statistics
 */

/**
 * @addtogroup std
 * @{
 */

/**
 * @brief Standard deviation of the u8 vector.
 * @param[in]       *src    points to the input vector.
 * @param[in]       size    size of the vectors.
 * @return Standard deviation value.
 *
 * <b>Function notes:</b>
 * The 8-bit format input is multiplied yields a 16-bit format, and then added
 * saturation to a 32-bit accumulator in 16.16 format. Finally,
 * the added output is truncated to 34.15 format by discarding the lower 1
 * bits, and then saturated to yield a result in 1.15 format.
 */
/* function description */
q15_t riscv_dsp_std_u8(const uint8_t *src, uint32_t size)
{
    q31_t sum = 0;
    q31_t sos = 0;
    q31_t meansqr, sqrmean;
    q31_t tmp1;
    q15_t dst;
    uint32_t i = size;
    while (i != 0u)
    {
        tmp1 = *src++;
        //Only V3M does not have mulsr64 inst.
        //Using this instr. can save one cycle.
        sos += (q31_t) tmp1 * tmp1;
        sum += tmp1;
        i--;
    }
    // To improve accuracy, replace the original with the following code.
    meansqr = sos / (size - 1);
    sqrmean = (q31_t)(((q63_t) sum * sum) / (q63_t)(size * (size - 1)));
    tmp1 = (q15_t) NDS_ISA_SATS(((meansqr - sqrmean) >> 1), 16u);

    dst = (q15_t) riscv_dsp_sqrt_q15(tmp1);
    return dst;
}

/**
* @} end of std.
*/
