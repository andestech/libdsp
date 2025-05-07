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
 * @defgroup clx_mul_real Complex-by-Real multiplications.
 *
 * This function multiplies a complex vector by a real vector and
 * generates a complex result.
 *
 * The data in the input <code>src</code> vector and output <code>dst</code>
 * are arranged in the array as:
 *            [real, imag, real, imag, real, imag, ...).
 * The input vector <code>src</code> and output vector <code>dst</code> have
 * a total of <code>2 * size</code> real values.
 * The input <code>real</code> vector has a total of <code>size</code> values.
 *
 * <pre>
 *      for(n = 0; n < size; n++) {
 *      dst[(2 * n) + 0] += src[(2 * n) + 0] * real[n];
 *      dst[(2 * n) + 1] += src[(2 * n) + 1] * real[n];
 *      }
 * </pre>
 */

/**
 * @addtogroup clx_mul_real
 * @{
 */

/**
 * @brief Multiply the folating-point complex vector by a real vector.
 * @param[in]		*src  the input complex vector.
 * @param[in]		*real the input real vector.
 * @param[out]		*dst  output complex vector.
 * @param[in]		size size of the vectors.
 * @return none.
 */

/* function description */
void riscv_dsp_cmul_real_f32(const float32_t * FUNC_RESTRICT  src, const float32_t * FUNC_RESTRICT real,
                       float32_t * FUNC_RESTRICT dst, uint32_t size)
{
    float32_t a;

    do
    {
        /* y[2 * i] = x[2 * i] * real[i] */
        /* y[2 * i + 1] = x[2 * i + 1] * real[i] */
        a = *real++;
        *dst++ = (*src++) * a;
        *dst++ = (*src++) * a;

        size--;
    }
    while (size != 0u);
}

/**
 * @} end of clx_mul_real
 */
