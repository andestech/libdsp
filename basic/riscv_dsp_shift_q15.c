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
#include "internal_math_types.h"

#include "internal_isa.h"

/**
 * @ingroup basic
 */

/**
 * @addtogroup basicshift
 * @{
 */

/**
 * @brief Shifts a q15 vector for a specified shift number.
 * @param[in]       *src the input vector.
 * @param[in]      shift number of shift bits. If (shift > 0) means shifts
 *                       left; (shift < 0) means shifts right.
 * @param[out]      *dst the output vector.
 * @param[in]       size size of the vectors.
 * @return none.
 *
 * The input and output are all saturated to q15 range [0x8000 0x7FFF].
 */

/* function description */
void riscv_dsp_shift_q15(q15_t * FUNC_RESTRICT src, int8_t shift, q15_t * FUNC_RESTRICT dst, uint32_t size)
{

    if (shift < 0)
    {
        shift =  -shift;
        while (size != 0u)
        {
            *dst++ = *src++ >> shift;
            size--;
        }
    }
    else
    {

        while (size != 0u)
        {
            *dst++ = (q15_t) NDS_ISA_SATS(((q31_t) * src++ << shift), 16u);
            size--;
        }
    }

}

/**
 * @} end of basicabs
 */
