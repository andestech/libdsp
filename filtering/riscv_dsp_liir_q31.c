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
 * saturated to 1.32 format and then truncated to 1.31 format.
 */

/* function description */
void riscv_dsp_liir_q31(const riscv_dsp_liir_q31_t * FUNC_RESTRICT instance,
                        q31_t * FUNC_RESTRICT src, q31_t * FUNC_RESTRICT dst, uint32_t size)
{
    q31_t fcurr, fnext = 0, gcurr = 0, gnext;
    q63_t acc;
    q31_t *src1, *src2, *ref, *lad;
    q31_t *state, *scurr;
    uint32_t tapcnt, blkcnt;
    uint32_t nstage = instance->nstage;

    state = &instance->state[0];
    blkcnt = size;
    /* process the whole block */
    while (blkcnt != 0u)
    {
        fcurr = *src++;
        src1 = state;
        src2 = state;
        acc = 0;
        ref = &instance->rcoeff[0];
        lad = &instance->lcoeff[0];

        tapcnt = nstage;
        /* stage loop */
        while (tapcnt != 0u)
        {
            gcurr = *src1++;
        fnext = _dsp_truncate_q63_to_q31(((q63_t) fcurr -
                                          ((q31_t) (((q63_t) gcurr * (*ref)) >> 31))));
        gnext = _dsp_truncate_q63_to_q31(((q63_t) gcurr +
                                          ((q31_t) (((q63_t) fnext * (*ref++)) >> 31))));
        acc += ((q63_t) gnext * (*lad++));
            *src2++ = gnext;
            fcurr = fnext;
            tapcnt--;
        }
        acc += (q63_t) fnext * (*lad++);
        *src2++ = fnext;
        *dst++ = (q31_t) (acc >> 31u);
        state = state + 1u;
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
