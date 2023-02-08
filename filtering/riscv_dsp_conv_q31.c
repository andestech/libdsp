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
 * @addtogroup conv
 * @{
 */

/**
 * @brief Convolution of the q31 vectors.
 * @param[in]       *src1 points to the first input vector.
 * @param[in]       len1  length of the first input vector.
 * @param[in]       *src2 points to the second input vector.
 * @param[in]       len2  length of the second input vector.
 * @param[out]      *dst  points to the output vector where the length is
 *                        len1 + len2 - 1.
 * @return none.
 *
 * Function notes:
 * Both inputs are in 1.31 format and the 64-bit accumulator has a 2.62
 * format and maintains full precision of the intermediate multiplication
 * results but provides only a single guard bit. The input signals should be
 * scaled down to avoid intermediate overflows. Scale down the inputs by
 * log2(min(len1, len2)), The 2.62 accumulator is right shifted by 31
 * bits and saturated to 1.31 forma t to yield the final result.
 */

/* function description */
void riscv_dsp_conv_q31(q31_t * FUNC_RESTRICT src1, uint32_t len1,
                    q31_t * FUNC_RESTRICT src2, uint32_t len2, q31_t * FUNC_RESTRICT dst)
{
    uint32_t output_size = len1 + len2 - 1;
    riscv_dsp_conv_partial_q31((q31_t *) src1, len1, (q31_t *) src2, len2, (q31_t *) dst,  0, output_size);
}

/**
 * @} end of conv
 */
