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
#include "internal_utils_math.h"

extern q31_t riscv_dsp_sqrt_q31(q31_t src);
extern q63_t riscv_dsp_var_q31(const q31_t *src, uint32_t size);

/**
 * @ingroup statistics
 */

/**
 * @addtogroup std
 * @{
 */

/**
 * @brief Standard deviation of the q31 vector.
 * @param[in]       *src    points to the input vector.
 * @param[in]       size    size of the vectors.
 * @return Standard deviation value.
 *
 * <b>Function notes:</b>
 *
 * The 1.31 format input is multiplied yields a 2.62 format. In order to
 * avoid overflows, the input signal must be scaled down by
 * <code>log2(size)</code> bits, Finally, the 2.62 accumulator is right
 * shifted by 31 bits to yield a 1.31 format value.
 */

/* function description */
q31_t riscv_dsp_std_q31(const q31_t *src, uint32_t size)
{
    q31_t tmp = riscv_dsp_var_q31(src, size);
    q31_t dst = riscv_dsp_sqrt_q31(tmp);
    return dst;
}

/**
 * @} end of std.
 */
