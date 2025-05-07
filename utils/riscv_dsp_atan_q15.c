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
//#include <stdlib.h>
#include "internal_utils_math.h"

/**
 * @ingroup utils
 */

/**
 * @addtogroup utilsatan
 * @{
 */

/**
 * @brief Arctan value of floating-point input.
 * @param[in]   src     the input value.
 * @return arctan value of the input.
 *
 * Function notes:
 * The input argument is tangent of the radian to be returned. It is scaled
 * as Q(RES_LOGN).15-(RES_LOGN). For example, it is Q8.7 if RES_N = 256. The
 * returned value falls in the range of [-PI / 2, PI / 2) scaled as
 * depending on global symbol FOUR_PI_RANGE:
 *  1> defined: [-0.25, 0.25)
 *  2> otherwise: [-0.5, 0.5)
 */

/* function description */
q15_t riscv_dsp_atan_q15(q15_t src)
{
    long hi = NDS32_TAN_TABLE_SIZE - 1, lo = 0;
    long index = (NDS32_TAN_TABLE_SIZE) >> 1;
    long neg = (src < 0);

    /* quick return of atan(-inf) = -PI / 2 */
    if (src == INT16_MIN)
    {
        return -HALF_PI_Q15;
    }

    src = dsp_abs(src);

    do
    {
        q15_t val = riscv_dsp_tan_table_q15[index];
        if (src < val)
        {
            hi = index;
        }
        else if (src > val)
        {
            lo = index;
        }
        else
        {
            break;
        }
        index = (hi + lo) >> 1;
    }
    while (index > lo);

    if (neg)
    {
        return -index * STEP_UNIT_Q15;
    }
    else
    {
        return index * STEP_UNIT_Q15;
    }
}

/**
 * @} end of utilsatan
 */
