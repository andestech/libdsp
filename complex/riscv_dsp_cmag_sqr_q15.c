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

#include "internal_isa.h"

/**
 * @ingroup complex
 */

/**
 * @addtogroup clx_mag_sqr
 * @{
 */

/**
 * @brief Compute the magnitude squared of the q15 complex vector.
 * @param[in]       *src points to the input complex vector.
 * @param[out]      *dst points to the output complex vector.
 * @param[in]       size size of the vectors.
 * @return none.
 *
 * <b>Function notes:</b>
 *
 * The function implements 1.15 by 1.15 multiplications and finally output
 * is converted into 3.13 format.
 */

/* function description */
void riscv_dsp_cmag_sqr_q15(const q15_t * FUNC_RESTRICT src, q15_t * FUNC_RESTRICT dst, uint32_t size)
{

    if (size > 0)
    {
        do
        {
            q15_t a, b;

            /* y = x[(2 * n) + 0] ^ 2 + x[(2 * n) + 1] ^ 2 */
            a = *src++;
            b = *src++;

            /* Store the result in 3.13 format */
            *dst++ = (q15_t) (((q63_t)(((q31_t) a * a)
                                       + ((q31_t) b * b))) >> 17);

            size--;
        }
        while (size != 0u);
    }

}

/**
 * @} end of clx_mag_sqr
 */
