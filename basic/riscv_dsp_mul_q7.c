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
 * @addtogroup basicmul
 * @{
 */

/**
 * @brief Multiplication of q7 vectors.
 * @param[in]       *src1 points to the first input vector.
 * @param[in]       *src2 points to the second input vector.
 * @param[out]      *dst  points to the output vector.
 * @param[in]       size  size of the vectors.
 * @return none.
 *
 * Ouput results will be saturated in Q7 range [0x80 0x7F].
 */

/* function description */
void riscv_dsp_mul_q7(q7_t * FUNC_RESTRICT src1, q7_t * FUNC_RESTRICT src2, q7_t * FUNC_RESTRICT dst, uint32_t size)
{

    int size2 = size >> 0x1L;

    size = size & 0x1L;
    while (size2 != 0u)
    {
    *dst++ = (q7_t) NDS_ISA_SATS((((q15_t) (*src1++) * (*src2++)) >> 7), 8u);
    *dst++ = (q7_t) NDS_ISA_SATS((((q15_t) (*src1++) * (*src2++)) >> 7), 8u);
        size2--;
    }

    if (size != 0 )
    {
    *dst++ = (q7_t) NDS_ISA_SATS((((q15_t) (*src1++) * (*src2++)) >> 7), 8u);
    }
}

/**
 * @} end of basicmul
 */
