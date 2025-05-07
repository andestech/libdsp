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

/**
 * @ingroup utils
 */

/**
 * @addtogroup utilsset
 * @{
 */

/**
 * @brief Set the Q7 vector.
 * @param[in]       val specify Q7 value.
 * @param[out]      *dst the output vector point.
 * @param[in]       size size of the vector.
 * @return none.
 */

/* function description */
void riscv_dsp_set_q7(q7_t val, q7_t *dst, uint32_t size)
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
