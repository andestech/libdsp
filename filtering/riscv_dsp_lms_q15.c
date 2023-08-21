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
#include "internal_isa.h"
/**
 * @ingroup filtering
 */

/**
 * @addtogroup lms
 * @{
 */

/**
 * @brief Function for the q15 LMS filter.
 * @param[in]       *instance points to an instance of the LMS structure.
 * @param[in]       *src      points to the input block data.
 * @param[in]       *ref      points to the reference data.
 * @param[out]      *dst      points to the output data.
 * @param[out]      *err      points to the error data.
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
void riscv_dsp_lms_q15(const riscv_dsp_lms_q15_t * FUNC_RESTRICT instance, q15_t * FUNC_RESTRICT src,
                 q15_t * FUNC_RESTRICT ref, q15_t * FUNC_RESTRICT dst, q15_t * FUNC_RESTRICT err, uint32_t size)
{
    q15_t *px, *pf, *scurr;
    q15_t mu = instance->mu;
    uint32_t coeff_size = instance->coeff_size;
    uint32_t tapcnt;
    q63_t acc;
    q31_t acc_l, acc_h;
    q15_t w, e = 0;
    int32_t lshift = (15 - (int32_t) instance->shift);
    int32_t ushift = (32 - lshift);
    scurr = &(instance->state[(coeff_size - 1u)]);
    q15_t *state = instance->state;

    /* block process */
    while (size != 0u)
    {
        acc = 0;
        /* filter phase */
        tapcnt = coeff_size;
        pf = instance->coeff;
        *scurr++ = *src++;
        px = state;
        while (tapcnt != 0u)
        {
            acc += (q63_t) ((q31_t) (*px++) * (*pf++));
            tapcnt--;
        }
        acc_l = acc & 0xffffffff;
        acc_h = (acc >> 32) & 0xffffffff;
        acc = (uint32_t) acc_l >> lshift | acc_h << ushift;
        acc = NDS_ISA_SATS(acc, 16u);
        *dst++ = (q15_t) acc;

        /* update phase */
        e = (*ref++) - (q15_t) acc;
        *err++ = (q15_t) e;

        w = (q15_t) (((q31_t) e * (mu)) >> 15);
        tapcnt = coeff_size;
        pf = instance->coeff;
        px = state++;
        while (tapcnt != 0u)
        {
            *pf++ += (q15_t) (((q31_t) w * (*px++)) >> 15);
            tapcnt--;
        }
        /* end of update phase */
        size--;
    }

    /* clean up for next frame */
    scurr = instance->state;
    tapcnt = (coeff_size - 1u);

    while (tapcnt != 0u)
    {

        *scurr++ = *state++;
        tapcnt--;
    }
}

/**
 * @} end of lms
 */
