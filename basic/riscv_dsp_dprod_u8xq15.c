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
 * @addtogroup dot_prod
 * @{
 */

/**
 * @brief Dot production of u8 * q15 vectors.
 * @param[in]       *src1 points to the uint8_t format input vector.
 * @param[in]       *src2 points to the q15 format input vector.
 * @param[in]       size  size of the vectors.
 * @return dot product of two input vectors.
 *
 * The output of multiplications is in 1.23 format and then added to an
 * accumulator in 9.23 format. The return result is in 9.23 format.
 * When the size of the vectors less than (2^8)-1, there is no risk to overflow.
 */

/* function description */
q31_t riscv_dsp_dprod_u8xq15(uint8_t * FUNC_RESTRICT src1, q15_t * FUNC_RESTRICT src2, uint32_t size)
{
    q31_t sum = 0;

    while (size >= 4)
    {
#ifdef ENA_HIGHER_PERFORMANCE_DPROD_U8XQ15
        sum += (q31_t) ((*src1++) * (*src2++));
        sum += (q31_t) ((*src1++) * (*src2++));
        sum += (q31_t) ((*src1++) * (*src2++));
        sum += (q31_t) ((*src1++) * (*src2++));
#else
    sum = NDS_ISA_KADDW(sum, (*src1++) * (*src2++));
    sum = NDS_ISA_KADDW(sum, (*src1++) * (*src2++));
    sum = NDS_ISA_KADDW(sum, (*src1++) * (*src2++));
    sum = NDS_ISA_KADDW(sum, (*src1++) * (*src2++));
#endif
        size -= 4;
    }

    while (size != 0)
    {
#ifdef ENA_HIGHER_PERFORMANCE_DPROD_U8XQ15
        sum += (q31_t) ((*src1++) * (*src2++));
#else
    sum = NDS_ISA_KADDW(sum, (*src1++) * (*src2++));
#endif
        size--;
    }

    return sum;
}

/**
 * @} end of dot_prod
 */
