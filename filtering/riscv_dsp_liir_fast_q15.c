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
#include "internal_math_types.h"
#include "internal_filtering_math.h"

/**
 * @ingroup filtering
 */

/**
 * @addtogroup liir
 * @{
 */

/**
 * @brief Function for the q15 lattice IIR filter.
 * @param[in]       *instance points to an instance of the lattice
 *                            IIR structure.
 * @param[in]       *src      points to the input block data.
 * @param[out]      *dst      points to the output block data.
 * @param[in]       size      number of the blocksize.
 * @return none.
 *
 * Function notes:
 * Both coefficients and state variables are represented in 1.15 format
 * and multiplications yield a 2.30 result. The 2.30 results are accumulated
 * in a 64-bit accumulator in 34.30 format and the results is truncated
 * to 34.15 format by discarding low 15 bits. Lastly, the outputs is
 * saturated to yield a result in 1.15 format. And the <code>size </code> must
 * be a multiple of 2.
 */

/* function description */
void riscv_dsp_liir_fast_q15(const riscv_dsp_liir_q15_t * FUNC_RESTRICT instance,
                       q15_t * FUNC_RESTRICT src, q15_t * FUNC_RESTRICT dst, uint32_t size)
{
    q31_t fcurr, gcurr = 0;
    q31_t fcurr1, gcurr1 = 0;
    q63_t acc;
    q63_t acc1;
    q15_t *pst, *ref, *lad;
    q15_t *state, *scurr;
    uint32_t tapcnt, blkcnt;
    uint32_t nstage = instance->nstage;

    state = &instance->state[0];
    blkcnt = size >> 1;
    /* process the whole block */
    while (blkcnt != 0u)
    {
        q31_t val_ref, val_lad;
        q31_t val_ref1, val_lad1;

        fcurr = *src++;
        fcurr1 = *src++;

        pst = state;
        acc = acc1 = 0;

        ref = &instance->rcoeff[0];
        lad = &instance->lcoeff[0];

        gcurr = *pst;
        val_ref = *ref;
        val_lad = *lad;
        fcurr = fcurr - (((val_ref) * gcurr) >> 15);
        fcurr = NDS_ISA_SATS(fcurr, 16u);
        gcurr = ((fcurr * (val_ref)) >> 15) + gcurr;
        gcurr = NDS_ISA_SATS(gcurr, 16u);
        acc += (q31_t) (gcurr * (val_lad));
        lad++;
        ref++;
        *pst = gcurr;
        pst++;

        tapcnt = nstage - 1;
        /* stage loop */
        while (tapcnt != 0u)
        {
            gcurr = *pst;
            val_ref1 = *ref;
            val_lad1 = *lad;
            fcurr = fcurr - (((val_ref1) * gcurr) >> 15);
            fcurr = NDS_ISA_SATS(fcurr, 16u);
            gcurr = ((fcurr * (val_ref1)) >> 15) + gcurr;
            gcurr = NDS_ISA_SATS(gcurr, 16u);
            acc += (q31_t) (gcurr * (val_lad1));
            lad++;
            ref++;
            gcurr1 = gcurr;
            fcurr1 = fcurr1 - (((val_ref) * gcurr) >> 15);
            fcurr1 = NDS_ISA_SATS(fcurr1, 16u);
            gcurr1 = ((fcurr1 * (val_ref)) >> 15) + gcurr1;
            gcurr1 = NDS_ISA_SATS(gcurr1, 16u);
            acc1 += (q31_t) (gcurr1 * (val_lad));

            val_ref = val_ref1;
            val_lad = val_lad1;

            *pst = gcurr1;
            pst++;
            tapcnt--;
        }
        gcurr1 = fcurr;
        fcurr1 = fcurr1 - (((val_ref) * fcurr) >> 15);
        fcurr1 = NDS_ISA_SATS(fcurr1, 16u);
        gcurr1 = ((fcurr1 * (val_ref)) >> 15) + gcurr1;
        gcurr1 = NDS_ISA_SATS(gcurr1, 16u);
        acc1 += (q31_t) (gcurr1 * (val_lad));
        acc += (q31_t) (fcurr * (*lad));
        acc1 += (q31_t) (fcurr1 * (*lad));
        *pst++ = gcurr1;
        *pst++ = fcurr1;
        *dst++ = NDS_ISA_SATS(acc >> 15, 16u);
        *dst++ = NDS_ISA_SATS(acc1 >> 15, 16u);

        state += 2;
        blkcnt--;
    }

    scurr = &instance->state[0];
    state = &instance->state[size];

    tapcnt = nstage;
    while (tapcnt != 0u)
    {
        *scurr++ = *state++;
        tapcnt--;
    }
}

/**
 * @} end of liir
 */
