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
 * @ingroup complex
 */

/**
 * @addtogroup clx_conj
 * @{
 */

/**
 * @brief Conjugate the q15 complex vector.
 * @param[in]       *src the input complex vector.
 * @param[out]      *dst the output complex vector.
 * @param[in]       size size of the vectors.
 * @return none.
 *
 * <b>Function notes :</b>
 *
 * The Q15 value INT16_MIN (0x8000) will be saturated to the maximum
 * allowable positive value INT16_MAX.
 */

/* function description */
void riscv_dsp_cconj_q15(const q15_t * FUNC_RESTRICT src, q15_t * FUNC_RESTRICT dst, uint32_t size)
{
    q15_t a;

    while (size > 0)
    {
        *dst++ = *src++;
        a = *src++;
        *dst++ = (q15_t) NDS_ISA_KSUBH(0, a);
        size--;
    }
}

/**
 * @} end of clx_conj
 */
