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
 * @ingroup matrix
 */

/**
 * @addtogroup matrixscale
 * @{
 */

/**
 * @brief Multiply a scale value of q31 matrix.
 * @param[in]       *src       points to the input matrix.
 * @param[in]       scale_fract fractional multiplication.
 * @param[in]       shift       arithmetic shift.
 * @param[out]      *dst        points to the output matrix.
 * @param[in]       row         number of the matrix rows.
 * @param[in]       col         number of the matrix columns.
 * @return none.
 *
 * <b>Function notes:</b>
 *
 * The 1.31 format input are multiplied to yield a 2.62 intermediate result
 * and this is shifted with saturation to 1.31 format.
 */

/* function description */
void riscv_dsp_mat_scale_q31(const q31_t * FUNC_RESTRICT src, q31_t scale_fract, int32_t shift,
                       q31_t * FUNC_RESTRICT dst, uint32_t row, uint32_t col)
{
    uint32_t size;
    int32_t totalshift = shift + 1;

    //check if the input row or col is zero
    if ((row <= 0) || (col <= 0))
    {
        return;
    }
    size = row * col;
    q31_t temp1, temp2;
    do
    {
        /* C(m,n) = A(m,n) * scale and then saturated in Q31 range. */
        temp1 = ((q63_t) * src++ * scale_fract) >> 32;

        temp2 = temp1 << totalshift;
        if (temp1 != (temp2 >> totalshift))
        {
            temp2 = INT32_MAX ^ (temp1 >> 31);
        }
        *dst++ = temp2;

        size--;
    }
    while (size != 0u);
}

/**
 * @} end of matrixscale
 */
