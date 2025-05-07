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
 * @defgroup clx_conj Complex Conjugate
 *
 * In mathematics, complex conjugates are a pair of complex numbers, both
 * having the same real part, but with imaginary parts of equal magnitude
 * and opposite signs. This function conjugate the each elements of a
 * complex data vector.
 *
 * The data in the input <code>src</code> vector and output <code>dst</code>
 * are arranged in the array as:
 *            [real, imag, real, imag, real, imag, ...).
 * Each vector has a total of <code>2 * size</code> values.
 *
 * <pre>
 *		for(n = 0; n < size; n++) {
 *			dst[(2 * n) + 0] = src[(2 * n) + 0];		///< real part
 *			dst[(2 * n) + 1] = -src[(2 * n) + 1];		///< imag part
 *		}
 * </pre>
 */

/**
 * @addtogroup clx_conj
 * @{
 */

/**
 * @brief Conjugate the floating-point complex vector.
 * @param[in]		*src the input complex vector.
 * @param[out]		*dst the output complex vector.
 * @param[in]		size size of the vectors.
 * @return none.
 */

/* function description */
void riscv_dsp_cconj_f32(const float32_t * FUNC_RESTRICT src, float32_t * FUNC_RESTRICT dst, uint32_t size)
{
    do
    {
        /* y[0] + jy[1] = x[0] + j (-1) x[1] */
        *dst++ = *src++;
        *dst++ = -*src++;

        size--;
    }
    while (size != 0u);
}

/**
 * @} end of clx_conj
 */
