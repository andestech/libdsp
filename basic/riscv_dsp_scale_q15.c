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
 * @addtogroup scale
 * @{
 */

/**
 * @brief To multiply a q15 vectors by a q15 scale.
 * @param[in]  *src points to the input vector.
 * @param[in]  scalefract: is the fractional portion value to be multiplied.
 * @param[in]  shift: number of bits to shift.
 * @param[out] *dst points to the output vector.
 * @param[in]  size size of the vectors.
 * @return none.
 *
 * The inputs are multiplied to yield a 2.30 output and then are shift with
 * saturation to 1.15 formats.
 */

/* function description */
void riscv_dsp_scale_q15(q15_t * FUNC_RESTRICT src, q15_t scalefract, int8_t shift, q15_t * FUNC_RESTRICT dst,
                   uint32_t size)
{
    int8_t shift1 = 15 - shift;
    while (size != 0)
    {
        *dst++ = (q15_t) NDS_ISA_SATS(((q31_t) (*src++) * scalefract) >> shift1, 16u);
        size--;
    }
}

/**
 * @} end of scale
 */
