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
 * @addtogroup basicsub
 * @{
 */

/**
 * @brief Subtraction of q31 vectors.
 * @param[in]       *src1 points to the first input vector.
 * @param[in]       *src2 points to the second input vector.
 * @param[out]      *dst  points to the output vector.
 * @param[in]       size  size of the vectors.
 * @return none.
 *
 * Ouput results will be saturated in Q31 range [0x80000000 0x7FFFFFFF].
 */

/* function description */
void riscv_dsp_sub_q31(q31_t * FUNC_RESTRICT src1, q31_t * FUNC_RESTRICT src2, q31_t * FUNC_RESTRICT dst, uint32_t size)
{
    while (size != 0u)
    {
        /* y = x1 + x2 and then saturated in Q31 range. */
    *dst++ = (q31_t) NDS_ISA_KSUBW((*src1++), (*src2++));

        size--;
    }
}

/**
 * @} end of basicsub
 */
