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
 * @defgroup clx_mul Complex-by-Complex multiplication
 *
 * This function multiplies a complex vector by another complex vector and
 * generates a complex result.
 *
 * The data in the input <code>src</code> vector and output <code>dst</code>
 * are arranged in the array as:
 *            [real, imag, real, imag, real, imag, ...).
 * Each complex vector has a total of <code>2 * size</code> real values.
 *
 * <pre>
 * for(n = 0; n < size; n++) {
 *   dst[(2 * n) + 0] = src1[(2 * n) + 0] * src2[(2 * n) + 0] - src1[(2 * n) + 1] * src2[(2 * n) + 1];
 *   dst[(2 * n) + 1] = src1[(2 * n) + 0] * src2[(2 * n) + 1] + src1[(2 * n) + 1] * src2[(2 * n) + 0];
 *      }
 * </pre>
 */

/**
 * @addtogroup clx_mul
 * @{
 */

/**
 * @brief Multiply two floating-point complex vector.
 * @param[in]		*src1 the first input complex vector.
 * @param[in]		*src2 the second input complex vector.
 * @param[out]		*dst  output complex vector.
 * @param[in]		size size of the vectors.
 * @return none.
 */

/* function description */
void riscv_dsp_cmul_f32(const float32_t * FUNC_RESTRICT src1, const float32_t * FUNC_RESTRICT src2,
                  float32_t * FUNC_RESTRICT dst, uint32_t size)
{
    float32_t a, b, c, d;

    do
    {
        /* y[2 * i] = x1[2 * i] * x2[2 * i]
         ** - x1[2 * i + 1] * x2[2 * i + 1]. */
        /* y[2 * i + 1] = x1[2 * i] * x2[2 * i + 1]
         ** + x1[2 * i + 1] * x2[2 * i]. */
        a = *src1++;
        b = *src1++;
        c = *src2++;
        d = *src2++;
        *dst++ = (a * c) - (b * d);
        *dst++ = (a * d) + (b * c);

        size--;

    }
    while (size != 0u);
}

/**
 * @} end of clx_mul
 */
