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
 * @addtogroup utilsdup
 * @{
 */

/**
 * @brief Duplicate the Q15 vector
 * @param[in]       *src the input vector point.
 * @param[out]      *dst the output vector point.
 * @param[in]       size size of vectors.
 * @return none.
 */

/* function description */
FUNC_ATTR_FORCE_MEMCPY_ZOL
void riscv_dsp_dup_q15(q15_t * FUNC_RESTRICT src, q15_t * FUNC_RESTRICT dst, uint32_t size)
{
    while (size != 0u)
    {
        /* y = |x| */
        *dst++ = *src++;
        size--;
    }
}

/**
 * @} end of utilsdup
 */
