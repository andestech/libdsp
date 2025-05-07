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

#include "internal_isa.h"

/**
 * @ingroup complex
 */

/**
 * @addtogroup clx_dot_prod
 * @{
 */

/**
 * @brief Compute the dot product type2 of the q15 complex vector.
 * @param[in]       *src1 the first input complex vector.
 * @param[in]       *src2 the second input complex vector.
 * @param[in]       size  size of the vectors.
 * @param[out]      *rout the real sum of the output.
 * @param[out]      *iout the imag sum of the output.
 * @return none.
 *
 * <b>Function notes:</b>
 *
 * The multiplication outputs are in 1.15 x 1.15 = 2.30 format and
 * finally output is shift into q24 format.
 */

/* function description */
void riscv_dsp_cdprod_typ2_q15(const q15_t * FUNC_RESTRICT src1, const q15_t * FUNC_RESTRICT src2,
                         uint32_t size, q31_t * FUNC_RESTRICT rout, q31_t * FUNC_RESTRICT iout)
{

    q63_t realsum = 0, imagsum = 0;

    q15_t x0, x1, y0, y1;
    while (size != 0u)
    {
        x0 = *src1++;
        x1 = *src1++;
        y0 = *src2++;
        y1 = *src2++;
    realsum += (q63_t)x0 * y0;
    imagsum += (q63_t)x0 * y1;
    realsum -= (q63_t)x1 * y1;
    imagsum += (q63_t)x1 * y0;
        size--;
    }

    /* Store the real and imaginary results in q24 format  */
    *rout = (q31_t) ((realsum) >> 6);
    *iout = (q31_t) ((imagsum) >> 6);
}

/**
 * @} end of clx_dot_prod
 */
