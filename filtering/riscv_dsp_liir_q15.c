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
 * saturated to yield a result in 1.15 format.
 */
/* function description */
void riscv_dsp_liir_q15(const riscv_dsp_liir_q15_t * FUNC_RESTRICT instance,
                  q15_t * FUNC_RESTRICT src, q15_t * FUNC_RESTRICT dst, uint32_t size)
{
    q31_t fcurr, fnext = 0, gcurr = 0, gnext;
    q31_t gnext1;
    q31_t gcurr1 = 0;
    q15_t *pst1;

    q63_t acc;
    q15_t *pst, *ref, *lad;

    q15_t *state, *scurr;
    uint32_t tapcnt, blkcnt;
    uint32_t nstage = instance->nstage;


    state = &instance->state[0];
    blkcnt = size;;
    /* process the whole block */
    while (blkcnt != 0u)
    {
        fcurr = *src++;

        pst = state;
        acc = 0;
        ref = &instance->rcoeff[0];
        lad = &instance->lcoeff[0];

        pst1 = state + 1;
        tapcnt = nstage >> 1;
        while (tapcnt != 0u)
        {
            q31_t val_ref, val_lad;
            gcurr = *pst;
            gcurr1 = *pst1;
            val_ref = *ref;
            val_lad = *lad;
        fnext = fcurr - (((val_ref) * gcurr) >> 15);
        fnext = NDS_ISA_SATS(fnext, 16u);
        gnext = ((fnext * (val_ref)) >> 15) + gcurr;
        gnext = NDS_ISA_SATS(gnext, 16u);
            ref++;
            fcurr = fnext;
            val_ref = *ref;
        fnext = fcurr - (((val_ref) * gcurr1) >> 15);
        fnext = NDS_ISA_SATS(fnext, 16u);
        gnext1 = ((fnext * (val_ref)) >> 15) + gcurr1;
        gnext1 = NDS_ISA_SATS(gnext1, 16u);
            ref++;
        acc += (q31_t) (gnext * (val_lad));
            lad++;
            val_lad = *lad;
        acc += (q31_t) (gnext1 * (val_lad));
            lad++;
            *pst = gnext;
            *pst1 = gnext1;
            pst += 2;
            pst1 += 2;
            fcurr = fnext;
            tapcnt--;
        }
        tapcnt = nstage & 0x1u;
        if (tapcnt != 0u)
        {
            q31_t val_ref, val_lad;
            gcurr = *pst;
            val_ref = *ref;
            val_lad = *lad;
        fnext = fcurr - (((val_ref) * gcurr) >> 15);
        fnext = NDS_ISA_SATS(fnext, 16u);
        gnext = ((fnext * (val_ref)) >> 15) + gcurr;
        gnext = NDS_ISA_SATS(gnext, 16u);
        acc += (q31_t) (gnext * (val_lad));
            ref++;
            fcurr = fnext;
            lad++;
            *pst = gnext;
            pst++;
        }
        acc += (q31_t) (fnext * (*lad));
        lad++;
        *pst = fnext;
        *dst++ = NDS_ISA_SATS(acc >> 15, 16u);
        state ++;
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
