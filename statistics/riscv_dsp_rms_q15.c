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
 * @addtogroup rms
 * @{
 */

/**
 * @brief RMS of the q15 vector.
 * @param[in]       *src    points to the input vector.
 * @param[in]       size    size of the vectors.
 * @return RMS value.
 *
 * <b>Function notes:</b>
 *
 * The 1.15 format input is multiplied yields a 2.30 format, and then added
 * without saturation to a 64-bit accumulator in 34.30 format. Finally,
 * the added output is truncated to 34.15 format by discarding the lower 15
 * bits, and then saturated to yield a result in 1.15 format.
 */

/* function description */
q15_t riscv_dsp_rms_q15(const q15_t *src, uint32_t size)
{
    q63_t sum = 0;
    q15_t dst;
    q31_t tmp;
    uint32_t i;

    if (size > 0)
    {
        i = size;

        while (i != 0u)
        {
            tmp = *src++;
            //Only V3M does not have mulsr64 inst.
            //Using this instr. can save one cycle.
            sum += (q63_t) tmp * tmp;

            i--;
        }

        tmp = (sum >> 15);
        tmp = (int32_t)((uint32_t) tmp / (uint32_t) size);

        //It is well for only checking if larger than (INT32_MAX >> 1) due to the value of sum is always positive.

        tmp = (q31_t) tmp > INT16_MAX ? INT16_MAX : (q31_t)tmp;
        dst = riscv_dsp_sqrt_q15((q15_t)tmp);
    }
    else
    {
        dst = 0;
    }
    return dst;
}

/**
 * @} end of rms.
 */
