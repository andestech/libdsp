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
 * @defgroup basicdiv Division
 *
 * In mathematics, division (÷) is an arithmetic operation. Specifically,
 * if b times c equals a: a = b * c, where b is not zero, then a divided by
 * b equals c, a / b = c
 *
 * <pre>
 *		dst[n] = src1[n] / src2[n],  0 <= n < size.
 * </pre>
 */

/**
 * @addtogroup basicdiv
 * @{
 */

/**
 * @brief Division of floating-point vectors.
 * @param[in]       *src1 points to the first input vector.
 * @param[in]       *src2 points to the second input vector.
 * @param[out]      *dst  points to the output vector.
 * @param[in]       size  size of the vectors.
 * @return none.
 */

/* function description */
void riscv_dsp_div_f32(float32_t * FUNC_RESTRICT src1, float32_t * FUNC_RESTRICT src2, float32_t * FUNC_RESTRICT dst,
                   uint32_t size)
{
    while (size != 0u)
    {
        /* y = x1 / x2 */
        *dst++ = (*src1++) / (*src2++);

        size--;
    }
}

/**
 * @} end of basicdiv
 */
