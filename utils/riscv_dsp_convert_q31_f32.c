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
#include "internal_math_types.h"

/**
 * @ingroup utils
 */

/**
 * @defgroup convertq31 Convert q31 vector
 *
 * Convert a Q31 vector.
 */

/**
 * @addtogroup convertq31
 * @{
 */

/**
 * @brief Convert a Q31 vector to floating.
 * @param[in]       *src the input vector point.
 * @param[out]      *dst the output vector point.
 * @param[in]       size size of vector.
 * @return none.
 *
 * <pre>
 *    while (size > 0)
 *    {
 *        dst = (float32_t) (src * UNIT_Q31);
 *        size--;
 *    }
 * </pre>
 */

void riscv_dsp_convert_q31_f32(q31_t * FUNC_RESTRICT src, float32_t * FUNC_RESTRICT dst, uint32_t size)
{
    while (size != 0u)
    {
        *dst++ = convert_q31_to_float(*src++);
        size--;
    }
}

/**
 * @} end of convertq31
 */
