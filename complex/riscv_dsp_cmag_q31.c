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

/**
 * @ingroup complex
 */

/**
 * @addtogroup clx_mag
 * @{
 */

/**
 * @brief Compute the magnitude of the q31 complex vector.
 * @param[in]       *src points to the input complex vector.
 * @param[out]      *dst points to the output complex vector.
 * @param[in]       size size of the vectors.
 * @return none.
 *
 * <b>Function notes:</b>
 *
 * The multiplication outputs are in 1.31 x 1.31 = 2.62 format and
 * finally output is shift into 2.30 format.
 */

/* function description */
void riscv_dsp_cmag_q31(const q31_t * FUNC_RESTRICT src, q31_t * FUNC_RESTRICT dst, uint32_t size)
{
    q31_t a, b;

    do
    {
        /* y = sqrt(x[(2 * n) + 0] ^ 2 + x[(2 * n) + 1] ^ 2) */
        a = *src++;
        b = *src++;

        /* store the output in 2.30 format */
        *dst++ = riscv_dsp_sqrt_q31((((q31_t) (((q63_t) a * a) >> 33))
                               + ((q31_t) (((q63_t) b * b) >> 33))));

        size--;
    }
    while (size != 0u);
}

/**
 * @} end of clx_mag
 */
