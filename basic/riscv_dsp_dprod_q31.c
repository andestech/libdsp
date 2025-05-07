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
 * @addtogroup dot_prod
 * @{
 */

/**
 * @brief Dot production of q31 vectors.
 * @param[in]       *src1 points to the first input vector.
 * @param[in]       *src2 points to the second input vector.
 * @param[in]       size  size of the vectors.
 * @return dot product of two input vectors.
 *
 * The output of multiplications is truncated from 2.62 to 2.48 format and
 * then added without saturation to a 64-bit accumulator. The return value
 * is in 16.48 format. When the size of the vectors less than 2^16, there is
 * no risk to overflow.
 */

/* function description */
q63_t riscv_dsp_dprod_q31(q31_t * FUNC_RESTRICT src1, q31_t * FUNC_RESTRICT src2, uint32_t size)
{
    q63_t sum = 0;
    while (size != 0u)
    {
        /* y = x1[0] * x2[0] + x1[1] * x2[1] + x1[2] * x2[2]
         ** ..... + x1[size-1] * x2[size-1] */
        sum += ((q63_t)(*src1++) * (*src2++)) >> 14u;

        size--;
    }
    return (sum);
}

/**
 * @} end of dot_prod
 */
