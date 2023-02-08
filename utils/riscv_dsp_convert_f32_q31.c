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
#include "internal_utils_math.h"

/**
 * @ingroup utils
 */

/**
 * @defgroup convertf32 Convert f32 vector
 *
 * Convert a floating-point vector.
 */

/**
 * @addtogroup convertf32
 * @{
 */

/**
 * @brief Convert a floating-point vector to Q31.
 * @param[in]       *src the input vector point.
 * @param[out]      *dst the output vector point.
 * @param[in]       size size of vectors.
 * @return none.
 *
 * <pre>
 *    while (size > 0)
 *    {
 *        dst = (q31_t) (src * INT32_MAX);
 *        size--;
 *    }
 * </pre>
 */

void riscv_dsp_convert_f32_q31(float32_t * FUNC_RESTRICT src, q31_t * FUNC_RESTRICT dst, uint32_t size)
{
    float32_t in;
    q31_t rst;
    while (size != 0u)
    {
        in = *src++;
#ifndef ENA_NDS_TOOLCHAIN
        if (in > (float32_t)1.0f)
            rst = (q31_t)INT32_MAX;
        else if (in < (float32_t) - 1.0f)
            rst = (q31_t)INT32_MIN;
        else
        {
#endif
            in *= (float32_t)(2147483648.0);
            if (in >= (float32_t)0)
                in += (float32_t)0.5;
            else
                in -= (float32_t)0.5;
            rst = (q31_t)in;
#ifndef ENA_NDS_TOOLCHAIN
        }
#endif
        *dst++ = rst;
        size--;
    }
}

/**
 * @} end of convertf32
 */
