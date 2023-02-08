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
 * @brief Function for the q31 lattice IIR filter.
 * @param[in]       *instance points to an instance of the lattice
 *                            IIR structure.
 * @param[in]       *src      points to the input block data.
 * @param[out]      *dst      points to the output block data.
 * @param[in]       size      number of the blocksize.
 * @return none.
 *
 * Function notes:
 * Both coefficients and state variables are represented in 1.31 format
 * and multiplications yield a 2.62 result. The 2.62 results are accumulated
 * in a 64-bit accumulator. In order to avoid overflows completely the input
 * signaland must be scaled down by <code>2*log2(nstages)</code> bits. After
 *  all multiply-accumulates are performed, the 2.62 accumulator is
 * saturated to 1.32 format and then truncated to 1.31 format. And the <code>size
 * </code> must be a multiple of 2.
 */

/* function description */
void riscv_dsp_liir_fast_q31(const riscv_dsp_liir_q31_t * FUNC_RESTRICT instance,
                             q31_t * FUNC_RESTRICT src, q31_t * FUNC_RESTRICT dst, uint32_t size)
{
    q31_t fcurr, gcurr = 0;
    q31_t fcurr1, gcurr1 = 0;
    q63_t acc1;
    q31_t *pst;
    q63_t acc;
    q31_t *ref, *lad;
    q31_t *state, *scurr;
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
    fcurr = _dsp_truncate_q63_to_q31(((q63_t) fcurr -
                                      ((q31_t) (((q63_t) gcurr * (val_ref)) >> 31))));
    gcurr = _dsp_truncate_q63_to_q31(((q63_t) gcurr +
                                      ((q31_t) (((q63_t) fcurr * (val_ref)) >> 31))));
    acc += ((q63_t) gcurr * (val_lad));
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
    fcurr = _dsp_truncate_q63_to_q31(((q63_t) fcurr -
                                      ((q31_t) (((q63_t) gcurr * (val_ref1)) >> 31))));
    gcurr = _dsp_truncate_q63_to_q31(((q63_t) gcurr +
                                      ((q31_t) (((q63_t) fcurr * (val_ref1)) >> 31))));
    acc += ((q63_t) gcurr * (val_lad1));
            lad++;
            ref++;
            gcurr1 = gcurr;
    fcurr1 = _dsp_truncate_q63_to_q31(((q63_t) fcurr1 -
                                       ((q31_t) (((q63_t) gcurr * (val_ref)) >> 31))));
    gcurr1 = _dsp_truncate_q63_to_q31(((q63_t) gcurr1 +
                                       ((q31_t) (((q63_t) fcurr1 * (val_ref)) >> 31))));
    acc1 += ((q63_t) gcurr1 * (val_lad));

            val_ref = val_ref1;
            val_lad = val_lad1;

            *pst = gcurr1;
            pst++;
            tapcnt--;
        }
        gcurr1 = fcurr;
    fcurr1 = _dsp_truncate_q63_to_q31(((q63_t) fcurr1 -
                                       ((q31_t) (((q63_t) fcurr * (val_ref)) >> 31))));
    gcurr1 = _dsp_truncate_q63_to_q31(((q63_t) gcurr1 +
                                       ((q31_t) (((q63_t) fcurr1 * (val_ref)) >> 31))));
    acc1 += ((q63_t) gcurr1 * (val_lad));
    acc += ((q63_t) fcurr * (*lad));
    acc1 += ((q63_t) fcurr1 * (*lad));
        *pst++ = gcurr1;
        *pst++ = fcurr1;
        *dst++ = (q31_t) (acc >> 31u);
        *dst++ = (q31_t) (acc1 >> 31u);

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
