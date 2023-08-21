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

extern q31_t riscv_dsp_sqrt_q31(q31_t src);

/**
 * @ingroup statistics
 */

/**
 * @addtogroup rms
 * @{
 */

/**
 * @brief RMS of the q31 vector.
 * @param[in]       *src    points to the input vector.
 * @param[in]       size    size of the vectors.
 * @return RMS value.
 *
 * <b>Function notes:</b>
 *
 * The 1.31 format input is multiplied yields a 2.62 format. In order to
 * avoid overflows, the input signal must be scaled down by
 * <code>log2(size)</code> bits, Finally, the 2.62 accumulator is right
 * shifted  by 31 bits to yield a 1.31 format value.
 */

/* function description */
q31_t riscv_dsp_rms_q31(const q31_t *src, uint32_t size)
{
    q63_t sum = 0;
    q31_t tmp, dst;
    uint32_t i;

    if (size > 0)
    {
        i = size;

        while (i != 0u)
        {
            tmp = *src++;
            sum += (q63_t) tmp * tmp;
            i--;
        }

        //It is well for only checking if larger than (INT32_MAX >> 1) due to the value of sum is always positive.
        //The format of tmp is Q31.
        tmp = (q31_t)(sum >> 31) > (INT32_MAX >> 1) ? (INT32_MAX >> 1) : (q31_t)(sum >> 31);
        tmp = (int32_t)((uint32_t) tmp / (uint32_t) size);
        dst = riscv_dsp_sqrt_q31((q31_t)tmp);
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
