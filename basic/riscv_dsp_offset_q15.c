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
 * @ingroup basic
 */

/**
 * @addtogroup offset
 * @{
 */

/**
 * @brief The offset of q15 vectors.
 * @param[in]       *src points to the input vector.
 * @param[in]       offset is the value to be added.
 * @param[out]      *dst points to the output vector.
 * @param[in]       size size of the vectors.
 * @return none.
 *
 * Output results are saturated in Q15 range [0x8000 0x7FFF].
 */
/* function description */
void riscv_dsp_offset_q15(q15_t * FUNC_RESTRICT src, q15_t offset, q15_t * FUNC_RESTRICT dst, uint32_t size)
{

    while (size != 0u)
    {
        *dst++ = (q15_t) NDS_ISA_KADDH(*src++, offset);
        size--;
    }

}

/**
 * @} end of offset
 */
