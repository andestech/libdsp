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
 * @ingroup statistics
 */

/**
 * @defgroup var Variance
 *
 * The variance is a measure of how far a set of numbers is spread out.
 * A variance of zero indicates that all the values are identical.
 * A non-zero variance is always positive: a small variance indicates that
 * the data points tend to be very close to the mean. This function
 * calculates the variance of the elements in the input vectors.
 *
 * <pre>
 *      dst = (sos - sum ^ 2 / size) / (size - 1),
 *      where sos(sum of the square) = src[0] * src[0] + src[1] * src[1]
 *              + src[2] * src[2] + ... + src[size-1] * src[size-1],
 *            sum = src[0] + src[1] + src[2] + ... + src[size-1].
 * </pre>
 */

/**
 * @addtogroup var
 * @{
 */

/**
 * @brief Variance of the floating-potint vector.
 * @param[in]       *src    points to the input vector.
 * @param[in]       size    size of the vectors.
 * @return Variance value.
 */

/* function description */
float32_t riscv_dsp_var_f32(const float32_t *src, uint32_t size)
{
    float32_t dst = 0.0f;

    if (size > 1)
    {
        float32_t sum = 0.0f, sos = 0.0f;
        float32_t tmp1, sqrsum;
        uint32_t i = size;

        //Revising while to do while, the cycles for V3_Os are decreased from 181233 to 181231.
        //(Keil's cycles are 170171)
        //The cycles for V3m_Os are decreased from 246007 to 244982.
        //(Keil's cycles are 237170)
        do
        {
            tmp1 = *src++;
            sos += tmp1 * tmp1;
            sum += tmp1;
        }
        while (--i);

        sqrsum = ((sum * sum) / (float32_t) size);
        //Revised from size - 1.0f to size -1, the inst/cycle for V3m_O3 are decreased from 179927/244914 to 179913/244891.
        dst = ((sos - sqrsum) / (float32_t) (size - 1));
    }
    return dst;
}

/**
 * @} end of var.
 */
