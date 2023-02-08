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
#include <riscv_dsp_filtering_math.h>

/**
 * @ingroup filtering
 */

/**
 * @defgroup conv Convolution
 *
 * Convolution is a mathematical operation on two finite length vectors,
 * producing a third finite length output vector that is typically viewed as
 * a modified version of one of the original functions. Convolution is
 * similar to cross-correlation. The algorithm is as follow:
 *
 * Let <code>src1[n]</code> and <code>src2[n]</code> be the input vectors of
 * length <code>len1</code> and <code>len2</code>. Then the convolution
 *
 * <pre>
 *               dst[n] = src1[n] * src2[n];
 * </pre>
 *
 * is defined as
 *
 * <pre>
 *                  len1
 *         dst[n] = sum (src1[k] x src2[n - k])
 *                  k=0
 * </pre>
 * where <code>dst[n]</code> is of length <code>len1 + len2 - 1</code> and
 * the interval is defined as <code>n = 0, 1, ..., len1  + len2 - 2</code>.
 */

/**
 * @addtogroup conv
 * @{
 */

/**
 * @brief Convolution of the floating-point vectors.
 * @param[in]       *src1 points to the first input vector.
 * @param[in]       len1  length of the first input vector.
 * @param[in]       *src2 points to the second input vector.
 * @param[in]       len2  length of the second input vector.
 * @param[out]      *dst  points to the output vector where the length is
 *                        len1 + len2 - 1.
 * @return none.
 */

/* function description */
void riscv_dsp_conv_f32(float32_t * FUNC_RESTRICT src1, uint32_t len1,
                  float32_t * FUNC_RESTRICT src2, uint32_t len2, float32_t * FUNC_RESTRICT dst)
{
    uint32_t output_size = len1 + len2 - 1;
    riscv_dsp_conv_partial_f32((float32_t *) src1, len1, (float32_t *) src2, len2, (float32_t *) dst, 0, output_size);
}

/**
 * @} end of conv
 */
