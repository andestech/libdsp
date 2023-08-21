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
 * @brief Function for the q31 NLMS filter.
 * @param[in]       *instance points to an instance of the NLMS structure.
 * @param[in]       *src      points to the input block data.
 * @param[in]       *ref      points to the reference data.
 * @param[out]      *dst      points to the output data.
 * @param[out]      *err      points to the error data.
 * @param[in]       size      number of the blocksize.
 * @return none.
 */

/* function description */
void riscv_dsp_nlms_q31(riscv_dsp_nlms_q31_t * FUNC_RESTRICT instance,
                        q31_t * FUNC_RESTRICT src, q31_t * FUNC_RESTRICT ref, q31_t * FUNC_RESTRICT dst, q31_t * FUNC_RESTRICT err,
                        uint32_t size)
{
    q31_t *pState = instance->state;
    q31_t *px, *pb, *scurr;
    q31_t mu = instance->mu;
    uint32_t coeff_size = instance->coeff_size;
    uint32_t tapcnt;
    q31_t e = 0, w = 0, d = 0;
    q63_t acc, energy;
    q31_t in;
    q31_t x0;
    q31_t errormu, onebyenergy = 0;
    q31_t postshift, coef, acc_l, acc_h;
    uint32_t uShift = ((uint32_t) instance->postshift + 1u);
    uint32_t lShift = 32u - uShift;
    energy = instance->energy;

    scurr = &(instance->state[(coeff_size - 1u)]);
    x0 = instance->x0;

    /* block process */
    while (size != 0u)
    {
        *scurr++ = *src;
        px = pState;
        pb = instance->coeff;

        /* filter phase */
        in = *src++;
        energy =
            (q31_t) ((((q63_t) energy << 32) - (((q63_t) x0 * x0) << 1)) >> 32);

        energy = (q31_t) (((((q63_t) in * in) << 1) + (energy << 32)) >> 32);

        acc = 0;

        tapcnt = coeff_size;

        while (tapcnt != 0u)
        {
        acc += (q63_t) (*px++) * (*pb++);
            tapcnt--;
        }

        // lower part of acc
        acc_l = acc & 0xffffffff;

        // highr part of acc
        acc_h = (acc >> 32) & 0xffffffff;

        acc = (uint32_t) acc_l >> lShift | acc_h << uShift;
        *dst++ = (q31_t) acc;

        /* update phase */
        d = *(ref++);
        e = d - (q31_t) acc;
        *err++ = e;

        postshift = _dsp_recip_q31(energy + DELTA_Q31, &onebyenergy);

        errormu = (q31_t) (((q63_t) e * mu) >> 31);

        w = _dsp_truncate_q63_to_q31(((q63_t) errormu * onebyenergy) >> (31 - postshift));

        px = pState;
        pb = instance->coeff;

        tapcnt = coeff_size;
        while (tapcnt != 0u)
        {
        coef = (q31_t) (((q63_t) w * (*px++)) >> (32));
            *pb = _dsp_truncate_q63_to_q31((q63_t) * pb + (coef << 1u));
            pb++;
            tapcnt--;
        }

        x0 = *pState++;
        size--;
    }

    /* clean up for next frame */
    instance->energy = energy;
    instance->x0 = x0;

    scurr = instance->state;
    tapcnt = (coeff_size - 1u);
    while (tapcnt != 0u)
    {
        *scurr++ = *pState++;
        tapcnt--;
    }
}

/**
 * @} end of nlms
 */
