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

/**
 * @ingroup complex
 */

/**
 * @defgroup clx_mag_sqr Complex Magnitude Squared
 *
 * This function computes the magnitude squared for the each elements of a
 * complex vector.
 *
 * The data in the input <code>src</code> vector and output <code>dst</code>
 * are arranged in the array as:
 *            [real, imag, real, imag, real, imag, ...).
 * The input vector has a total of <code>2 * size</code> values.
 * The output vector has a total of <code>size</code> values.
 *
 * <pre>
 *      for(n = 0; n < size; n++) {
 *          dst[n] = src[(2 * n) + 0] ^ 2 + src[(2 * n) + 1] ^ 2;
 *      }
 * </pre>
 */

/**
 * @addtogroup clx_mag_sqr
 * @{
 */

/**
 * @brief Compute the magnitude squared of the floating-potint complex
 *        vector.
 * @param[in]       *src points to the input complex vector.
 * @param[out]      *dst points to the output complex vector.
 * @param[in]       size size of the vectors.
 * @return none.
 */

/* function description */
void riscv_dsp_cmag_sqr_f32(const float32_t * FUNC_RESTRICT src, float32_t * FUNC_RESTRICT dst,
                      uint32_t size)
{
    if (size > 0)
    {
        do
        {
            float32_t a, b;

            /* y = x[(2 * n) + 0] ^ 2 + x[(2 * n) + 1] ^ 2 */
            a = *src++;
            b = *src++;
            *dst++ = (a * a) + (b * b);

            size--;
        }
        while (size != 0);
    }
}

/**
 * @} end of clx_mag_sqr
 */
