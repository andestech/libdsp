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
 * @ingroup complex
 */

/**
 * @defgroup clx_dot_prod Complex Dot Product
 *
 * This function computes the dot product of two vectors. The product is
 * defined as the two vectors multiply each element and then summed.
 *
 * The data in the input <code>src1</code>, <code>src2</code> vectors and
 * output <code>dst</code> are arranged in the array as:
 *            [real, imag, real, imag, real, imag, ...).
 * Each vector has a total of <code>2 * size</code> values.
 *
 * <pre>
 * for(n = 0; n < size; n++) {
 *   realsum += src1[(2 * n) + 0] * src2[(2 * n) + 0]];
 *   imagsum += src1[(2 * n) + 1] * src2[(2 * n) + 1];;
 * }
 * </pre>
 */

/**
 * @addtogroup clx_dot_prod
 * @{
 */

/**
 * @brief Compute the dot product type2 of the floating-potint complex vector.
 * @param[in]		*src1 the first input complex vector.
 * @param[in]		*src2 the second input complex vector.
 * @param[in]		size  size of the complex inputs.
 * @param[out]		*rout the real sum of the output.
 * @param[out]		*iout the imag sum of the output.
 * @return none.
 */

/* function description */
void riscv_dsp_cdprod_typ2_f32(const float32_t * FUNC_RESTRICT src1, const float32_t * FUNC_RESTRICT src2,
                         uint32_t size, float32_t * FUNC_RESTRICT rout, float32_t * FUNC_RESTRICT iout)
{
    float32_t realsum = 0.0f, imagsum = 0.0f;
    float32_t x0, x1, y0, y1;

    do
    {
        x0 = *src1++;
        x1 = *src1++;
        y0 = *src2++;
        y1 = *src2++;
        realsum = realsum + (x0 * y0) - (x1 * y1);
        imagsum = imagsum + (x0 * y1) + (x1 * y0);

        size--;
    }
    while (size != 0u);

    *rout = realsum;
    *iout = imagsum;
}

/**
 * @} end of clx_dot_prod
 */
