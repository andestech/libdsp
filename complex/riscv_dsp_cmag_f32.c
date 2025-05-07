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
#include <stdio.h>  //include this is because __ULong defined in math.h when using BSP320.
//BSP400 doesn't need this file.
#include <math.h>
#include "internal_utils_math.h"

/**
 * @ingroup complex
 */

/**
 * @defgroup clx_mag Complex Magnitude
 *
 * In mathematics, magnitude is the size of a mathematical object, a
 * property by which the object can be compared as larger or smaller than
 * other objects of the same kind. This function computes the the each
 * sample magnitude of a complex data vector.
 *
 * The data in the input <code>src</code> vector and output <code>dst</code>
 * are arranged in the array as:
 *            [real, imag, real, imag, real, imag, ...).
 * The input vector has a total of <code>2 * size</code> values.
 * The output vector has a total of <code>size</code> values.
 *
 * <pre>
 *      for(n = 0; n < size; n++) {
 *          dst[n] = sqrt (src[(2 * n) + 0] ^ 2 + src[(2 * n) + 1] ^ 2);
 *      }
 * </pre>
 */

/**
 * @addtogroup clx_mag
 * @{
 */

/**
 * @brief Compute the magnitude of the floating-potint complex vector.
 * @param[in]       *src points to the input complex vector.
 * @param[out]      *dst points to the output complex vector.
 * @param[in]       size size of the vectors.
 * @return none.
 */

/* function description */
void riscv_dsp_cmag_f32(const float32_t * FUNC_RESTRICT src, float32_t * FUNC_RESTRICT dst, uint32_t size)
{
    float32_t a, b;

    while (size != 0u)
    {
        /* y = sqrt(x[(2 * n) + 0] ^ 2 + x[(2 * n) + 1] ^ 2) */
        a = *src++;
        b = *src++;
        *dst++ = sqrtf(((a * a) + (b * b)));

        size--;
    }
}

/**
 * @} end of clx_mag
 */
