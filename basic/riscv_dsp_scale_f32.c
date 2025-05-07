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
 * @ingroup basic
 */

/**
 * @defgroup scale Scale
 *
 * This scale function is defined as to multiply a vector by a specify
 * scalar value. The function process each element of the vector.
 *
 * For floating-point format, the algorithm is:
 *
 * <pre>
 *		dst[n] = src[n] * scale,  0 <= n < size.
 * </pre>
 *
 * In the q7, q15, and q31 functions, <code>scale</code> is a fractional
 * multiplication <code>scaleFract</code> and an arithmetic shift
 * <code>shift</code>. The shift allows the gain of the scaling operation
 * to exceed 1.0.
 *
 * The algorithm in Q7, Q15, and Q31 is:
 *
 * <pre>
 *		dst[n] = (src[n] * scalefract) << shift,  0 <= n < size.
 * </pre>
 *
 * The overall scale factor is applied to:
 *
 * <pre>
 *		scale = scalefract * (2 ^ shift).
 * </pre>
 */

/**
 * @addtogroup scale
 * @{
 */

/**
 * @brief To multiply a floating-point vectors by a floating-point scale.
 * @param[in]       *src points to the input vector.
 * @param[in]       scale is the value to be multiplied.
 * @param[out]      *dst points to the output vector.
 * @param[in]       size size of the vectors.
 * @return none.
 */

/* function description */
void riscv_dsp_scale_f32(float32_t * FUNC_RESTRICT src, float32_t scale, float32_t * FUNC_RESTRICT  dst,
                     uint32_t size)
{
    while (size != 0u)
    {
        /* y = x * scale */
        *dst++ = (*src++) * scale;

        size--;
    }
}

/**
 * @} end of scale
 */
