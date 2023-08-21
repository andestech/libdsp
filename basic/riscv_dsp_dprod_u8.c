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
 * @addtogroup dot_prod
 * @{
 */

/**
 * @brief Dot production of U8 vectors.
 * @param[in]       *src1 points to the first input vector.
 * @param[in]       *src2 points to the second input vector.
 * @param[in]       size  size of the vectors.
 * @return dot product of two input vectors.
 *
 * The output of multiplications is in 0.16 format and then added to an
 * accumulator in 16.16 format. The return result is in 16.16 format.
 * When the size of the vectors less than (2^15)-1, there is no risk to overflow.
 */

/* function description */
uint32_t riscv_dsp_dprod_u8(uint8_t * FUNC_RESTRICT src1, uint8_t * FUNC_RESTRICT src2, uint32_t size)
{
    uint32_t sum = 0;

    /* y = x1[0] * x2[0] + x1[1] * x2[1] + x1[2] * x2[2] ..... + x1[size-1] * x2[size-1] */
    while (size != 0)
    {
        sum += (uint32_t) ((uint32_t)(*src1++) * (*src2++));
        size--;
    }
    return sum;
}

/**
 * @} end of dot_prod
 */
