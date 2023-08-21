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
#include <math.h>
#include "internal_utils_math.h"

/**
 * @ingroup utils
 */

/**
 * @defgroup utilsatan Arctan
 *
 * This function returns atan(x) through table look-up. The resolution of
 * the look-up table is pre-defined using global symbol RES_N. Application
 * should choose the best resolution based on - Once the RES_N is set, the
 * atan(x) for x leading to:
 *      1> higher resolution is not supported.
 *      2> same or lower resolution is simply retrieved from table.
 *
 * <pre>
 *      dst = atan(src);
 * </pre>
 *
 * The input argument is tangent of the radian to be returned. The returned
 * value falls in the range of [-PI / 2, PI / 2).
 */

/**
 * @addtogroup utilsatan
 * @{
 */

/**
 * @brief Arctan value of floating-point input.
 * @param[in]   src     the input value.
 * @return arctan value of the input.
 */

/* function description */
float32_t riscv_dsp_atan_f32(float32_t src)
{
#ifndef ENA_HIGHER_PERFORMANCE
    //Cycles:653
    //Maximum error: 1 x 10^(-6)
    //Average error: 1 x 10^(-8)
    return atanf(src);
#else
    //Cycles:464
    //Table Size: For resolution 1024, the table size is 1028.
    //Maximum error: 6 x 10^(-3)
    //Average error: 3 x 10^(-3)
    int32_t hi = NDS32_TAN_TABLE_SIZE - 1, lo = 0;
    int32_t index = (NDS32_TAN_TABLE_SIZE) >> 1;
    int32_t neg;
    union
    {
        float32_t f;
        uint32_t i;
    } y, z;

    /* binary search */
    y.f = src;
    neg = y.i & 0x80000000;
    y.i ^= neg;
    do
    {
        z.f = riscv_dsp_tan_table_f32[index];
        if (y.i < z.i)
        {
            hi = index;
            index = (hi + lo) >> 1;
        }
        else if (y.i != z.i)
        {
            lo = index;
            index = (hi + lo + 1) >> 1;
        }
        else
        {
            break;
        }
    }
    while (index > lo);

    if (index == 0)
        return src;
    else
    {
        z.f = index * (TWO_PI / RES_N);
        z.i |= neg;
        return z.f;
    }
#endif
}

/**
 * @} end of utilsatan
 */
