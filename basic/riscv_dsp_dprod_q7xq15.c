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
 * @addtogroup dot_prod
 * @{
 */

/**
 * @brief Dot production of q7 * q15 vectors.
 * @param[in]       *src1 points to the q7_t format input vector.
 * @param[in]       *src2 points to the q15 format input vector.
 * @param[in]       size  size of the vectors.
 * @return dot product of two input vectors.
 *
 * The output of multiplications is in 1.22 format and then added to an
 * accumulator in 10.22 format. The return result is in 10.22 format.
 * When the size of the vectors less than (2^9)-1, there is no risk to overflow.
 */

/* y = x1[0] * x2[0] + x1[1] * x2[1] + x1[2] * x2[2]
 ** ..... + x1[size-1] * x2[size-1] */

/* function description */
q31_t riscv_dsp_dprod_q7xq15(q7_t * FUNC_RESTRICT src1, q15_t * FUNC_RESTRICT src2, uint32_t size)
{
    q31_t sum = 0;
    uint32_t size2 = size >> 1;
    uint32_t rest_count = size & 1;

    while (size2 != 0)
    {
        sum += (q31_t) ((*src1++) * (*src2++));
        sum += (q31_t) ((*src1++) * (*src2++));
        size2 --;
    }

    if (rest_count != 0)
    {
        sum += (q31_t) ((*src1++) * (*src2++));
    }
    return (sum);
}

/**
 * @} end of dot_prod
 */
