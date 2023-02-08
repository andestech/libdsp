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
 * @brief Function for the q15 NLMS filter.
 * @param[in]       *instance points to an instance of the NLMS structure.
 * @param[in]       *src      points to the input block data.
 * @param[in]       *ref      points to the reference data.
 * @param[out]      *dst      points to the output data.
 * @param[out]      *err      points to the error data.
 * @param[in]       size      number of the blocksize.
 * @return none.
 */

/* function description */
void riscv_dsp_nlms_q15(riscv_dsp_nlms_q15_t * FUNC_RESTRICT instance,
                        q15_t * FUNC_RESTRICT src, q15_t * FUNC_RESTRICT ref, q15_t * FUNC_RESTRICT dst, q15_t * FUNC_RESTRICT err,
                        uint32_t size)
{
    q15_t *pState = instance->state;
    q15_t *px, *pb, *scurr;
    q15_t mu = instance->mu;
    uint32_t coeff_size = instance->coeff_size;
    uint32_t tapcnt;
    q15_t e = 0, w = 0, d = 0;
    q63_t acc;
    q31_t energy;
    q15_t in;
    q15_t x0;
    q15_t errormu, onebyenergy = 0;
    q15_t energy_q15 = 0;
    q15_t postshift;
    q31_t acc_l, acc_h;
    uint32_t lShift = (15 - (int32_t) instance->postshift);
    uint32_t uShift = 32 - lShift;
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
        energy = NDS_ISA_KSUBW(energy, (((q31_t) x0 * (x0)) >> 15));
        energy = NDS_ISA_KADDW(energy, (((q31_t) in * (in)) >> 15));
        acc = 0;

        tapcnt = coeff_size;

        while (tapcnt != 0u)
        {
        acc += (q31_t) (*px++) * (*pb++);
            tapcnt--;
        }


        // lower part of acc
        acc_l = acc & 0xffffffff;

        // higher part of acc
        acc_h = (acc >> 32) & 0xffffffff;

        acc = (uint32_t) acc_l >> lShift | acc_h << uShift;

        *dst++ = NDS_ISA_SATS(acc, 16u);

        /* update phase */
        d = *(ref++);
        e = d - (q15_t) acc;
        *err++ = e;
        energy_q15 = NDS_ISA_SATS(energy, 16u);
        energy_q15 = NDS_ISA_KADDH(energy_q15, DELTA_Q15);
        postshift = _dsp_recip_q15(energy_q15, &onebyenergy);

        errormu = (q15_t) (((q31_t) e * mu) >> 15);
        acc = (((q31_t) errormu * onebyenergy) >> (15 - postshift));

        w = NDS_ISA_SATS((q31_t) acc, 16u);

        px = pState;
        pb = instance->coeff;

        tapcnt = coeff_size;
        while (tapcnt != 0u)
        {
        *pb = NDS_ISA_KADDH( *pb, NDS_ISA_KHMBB( w, *px++));
            pb++;
            tapcnt--;
        }

        x0 = *pState++;
        size--;
    }

    /* clean up for next frame */
    instance->energy = (q15_t) energy;
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
