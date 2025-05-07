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
 * @ingroup basic
 */

/**
 * @addtogroup basicdiv
 * @{
 */

/**
 * This function provides the fix-point division. The dividend (src1)
 * should be less
 * then divisor (src2).
 * @brief Division of q31 inputs.
 * @param[in]       src1 the smaller input value.
 * @param[in]       src2 the larger input value.
 * @return division of two inputs.
 */

#define NORM_SH     (29)
#define CST_1       (1L << NORM_SH)
#define CST_2       (2L * CST_1)
#define D2QN(F, SH) ((int)((float)F * (float)(1L << SH)))
#define GHMUL(X, Y) ((int)(((q63_t)X * Y) >> 32))

/* function description */
q31_t riscv_dsp_div_q31(q31_t src1, q31_t src2)
{
    /* y = src1 / src2 */
    /* Lease squares + Newton-Raphson division */
    int sh, dvd, dvs;
    const q31_t cst_2 = CST_2;
    int sign = (src1 ^ src2) >> 31;
    q31_t xi;
    q31_t temp;

    /* Normalize dvs to Q(NORM_SH) */
    dvd = NDS_ISA_ABS(src1);
    dvs = NDS_ISA_ABS(src2);
    sh = (int32_t)NDS_ISA_CLZ(dvs) - (int32_t)(32 - NORM_SH);
    dvs = (sh >= 0) ? (dvs << sh) : (dvs >> (-sh));

    /* Using lease squares equation to get the initial value. */
    /* The coef. of X^2 equation are
     ** 2.6192200905308 * X^2 - 5.83543568753 * X + 4.2349928801493 */
    xi = GHMUL(dvs, D2QN(2.6192200905308, 29));
    xi = xi - D2QN(5.83543568753, 26);
    xi = GHMUL(dvs, xi);
    xi = xi + D2QN(4.2349928801493, 23);    /* q23 */
    temp = D2QN(2.0, 20) - GHMUL(xi, dvs);  /* q20 */
    temp <<= 9;                             /* q29 */
    xi = GHMUL(xi, temp);                   /* q20 */
    xi <<= 9;

    temp = GHMUL(xi, dvs);
    temp <<= 3;
    xi = GHMUL(xi, (cst_2 - temp));
    xi <<= 3;
    /* After the second iteration:                     */
    /* Error: max is 153, min is -32                   */
    /* The inst count is 48, The cycle count is 58     */

    /* Restore the format back to Q31 */
    sh = NORM_SH - (31 - NORM_SH) - sh;
    xi = ((q63_t)xi * dvd) >> sh;    /* sh is never less than zero. */

    if (sign != 0)
        xi *= sign;

    return xi;
}

/**
 * @} end of basicdiv
 */
