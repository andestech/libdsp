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

/**
 * @ingroup basic
 */

/**
 * @defgroup basicneg Negate
 *
 * Negation is defined as convert the sign(+ or -) symbol. This function
 * negate the each elements of a vector.
 *
 * <pre>
 *		dst[n] = -src[n], if src[n] >= 0
 *             =  src[n], if src[n] < 0.     0 <= n < size.
 * </pre>
 */

/**
 * @addtogroup basicneg
 * @{
 */

/**
 * @brief floating-potint neg
 * @param[in]       *src points to the input vector.
 * @param[out]      *dst points to the output vector.
 * @param[in]       size size of the vectors.
 * @return none.
 */

/* function description */
void riscv_dsp_neg_f32(float32_t * FUNC_RESTRICT src, float32_t * FUNC_RESTRICT dst, uint32_t size)
{
    while (size != 0u)
    {
        /* y = -x */
        *dst++ = -*src++;

        size--;
    }
}

/**
 * @} end of basicneg
 */
