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
#include "internal_filtering_math.h"

/**
 * @ingroup filtering
 */

/**
 * @addtogroup lfir
 * @{
 */

/**
 * @brief Function for the q31 lattice FIR filter.
 * @param[in]       *instance points to an instance of the lattice
 *                            FIR structure.
 * @param[in]       *src      points to the input block data.
 * @param[out]      *dst      points to the output block data.
 * @param[in]       size      number of the blocksize.
 * @return none.
 *
 * Function notes:
 * In order to avoid overflows the input signal must be scaled down by
 * 2*log2(stage) bits.
 */

/* function description */
void riscv_dsp_lfir_q31(const riscv_dsp_lfir_q31_t * FUNC_RESTRICT instance, q31_t * FUNC_RESTRICT src, q31_t * FUNC_RESTRICT dst, uint32_t size)
{
    /* process the whole block */
    while (size != 0)
    {
        q31_t *px;
        q31_t *pk;
        q31_t fcurr, fnext, gcurr, gnext;
        uint32_t stage;

        /* k1, k2, k3, ..., kM */
        pk = instance->coeff;
        px = instance->state;

        /* get f0(n) and g0(n-1) */
        fcurr = *src++;
        gcurr = *px;

        /* f1(n) = f0(n) + K1 * g0(n-1) */
        /* g1(n) = f0(n) * K1 + g0(n-1) */
        fnext = (q31_t) fcurr + (((q63_t) gcurr * (*pk)) >> 31);
        gnext = (q31_t) (((q63_t) fcurr * (*pk++)) >> 31) + gcurr;
        *px++ = fcurr;
        fcurr = fnext;

        stage = instance->stage - 1;

        /* stage loop */
        while (stage != 0)
        {
            gcurr = *px;
            *px++ = gnext;
            fnext = (q31_t) fcurr + (((q63_t) gcurr * (*pk)) >> 31);
            gnext = (q31_t) (((q63_t) fcurr * (*pk++)) >> 31) + gcurr;
            fcurr = fnext;
            stage--;
        }

        *dst++ = fcurr;
        size--;
    }
}

/**
 * @} end of lfir
 */
