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

/**
 * @ingroup utils
 */

/**
 * @defgroup utilsset Set the vector
 *
 * Set a specify value to the vector.
 *
 * <pre>
 *    while (size > 0)
 *    {
 *        dst = val;
 *        size--;
 *    }
 * </pre>
 */

/**
 * @addtogroup utilsset
 * @{
 */

/**
 * @brief Set the floating-point vector.
 * @param[in]       val specify floating-point value.
 * @param[out]      *dst the output vector point.
 * @param[in]       size size of the vector.
 * @return none.
 */

/* function description */
void riscv_dsp_set_f32(float32_t val, float32_t *dst, uint32_t size)
{
    while (size != 0u)
    {
        /* y = |x| */
        *dst++ = val;
        size--;
    }
}

/**
 * @} end of utilsset
 */
