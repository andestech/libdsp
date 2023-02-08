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
void riscv_dsp_lfir_q15(const riscv_dsp_lfir_q15_t * FUNC_RESTRICT instance, q15_t * FUNC_RESTRICT src, q15_t * FUNC_RESTRICT dst, uint32_t size)
{
    /* process the whole block */
    uint32_t blkCnt;
    q15_t *px;
    q15_t *pk;
    q31_t fcurr1, fnext1, gcurr1, gnext1;
    q31_t fcurr2, fnext2, gnext2;
    q31_t k, tmp;
    uint32_t stage;

    //unrolling 2 times.
    blkCnt = size >> 1u;
    while (blkCnt > 0u)
    {
        /* k1, k2, k3, ..., kM */
        pk = instance->coeff;
        px = instance->state;

        fcurr1 = *src++;
        fcurr2 = *src++;
        gcurr1 = *px;
        k = *pk++;
        tmp = ((fcurr1 * (k)) >> 15u);
        fnext1 = fcurr1 + ((gcurr1 * (k)) >> 15u);
        fnext1 = NDS_ISA_SATS(fnext1, 16u);
        gnext1 = tmp + gcurr1;
        gnext1 = NDS_ISA_SATS(gnext1, 16u);

        fnext2 = fcurr2 + tmp;
        fnext2 = NDS_ISA_SATS(fnext2, 16u);
        gnext2 = fcurr1 + ((fcurr2 * (k)) >> 15u);
        gnext2 = NDS_ISA_SATS(gnext2, 16u);
        * px++ = (q15_t)fcurr2;

        fcurr1 = fnext1;
        fcurr2 = fnext2;

        stage = instance->stage - 1;

        /* stage loop */
        while (stage != 0)
        {
            k = *pk++;
            gcurr1 = *px;
            *px++ = gnext2;
            fnext1 = fcurr1 + ((gcurr1 * (k)) >> 15u);
            fnext1 = NDS_ISA_SATS(fnext1, 16u);
            fnext2 = fcurr2 + ((gnext1 * (k)) >> 15u);
            fnext2 = NDS_ISA_SATS(fnext2, 16u);
            gnext2 = gnext1 + ((fcurr2 * (k)) >> 15u);
            gnext2 = NDS_ISA_SATS(gnext2, 16u);
            gnext1 = gcurr1 + ((fcurr1 * (k)) >> 15u);
            gnext1 = NDS_ISA_SATS(gnext1, 16u);
            fcurr1 = fnext1;
            fcurr2 = fnext2;

            stage--;
        }
        *dst++ = NDS_ISA_SATS(fcurr1, 16u);
        *dst++ = NDS_ISA_SATS(fcurr2, 16u);
        blkCnt--;
    }

    blkCnt = size % 0x2u;
    if (blkCnt > 0u)
    {
        /* k1, k2, k3, ..., kM */
        pk = instance->coeff;
        px = instance->state;

        /* get f0(n) and g0(n-1) */
        fcurr1 = *src++;
        gcurr1 = *px;
        k = *pk++;

        fnext1 = fcurr1 + ((gcurr1 * (k)) >> 15u);
        fnext1 = NDS_ISA_SATS(fnext1, 16u);
        gnext1 = gcurr1 + ((fcurr1 * (k)) >> 15u);
        gnext1 = NDS_ISA_SATS(gnext1, 16u);

        *px++ = fcurr1;
        fcurr1 = fnext1;

        stage = instance->stage - 1;

        /* stage loop */
        while (stage != 0)
        {
            k = *pk++;
            gcurr1 = *px;
            *px++ = gnext1;
            fnext1 = fcurr1 + ((gcurr1 * (k)) >> 15u);
            fnext1 = NDS_ISA_SATS(fnext1, 16u);
            gnext1 = gcurr1 + ((fcurr1 * (k)) >> 15u);
            gnext1 = NDS_ISA_SATS(gnext1, 16u);

            fcurr1 = fnext1;

            stage--;
        }

        *dst++ = fcurr1;
    }
}

/**
 * @} end of lfir
 */
