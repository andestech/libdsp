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
 * @addtogroup lms
 * @{
 */

/**
 * @brief Function for the q31 LMS filter.
 * @param[in]       *instance points to an instance of the LMS structure.
 * @param[in]       *src      points to the input block data.
 * @param[in]       *ref      points to the reference data.
 * @param[out]      *dst      points to the output data.
 * @param[out]      *err      points to the error data.
 * @param[in]       size      number of the blocksize.
 * @return none.
 *
 * Function notes:
 * Both coefficients and state variables are represented in 1.31 format
 * and multiplications yield a 2.62 result. The 2.62 results are accumulated
 * in a 64-bit accumulator and is right shifted by 31 bits and saturated to
 * 1.31 formatthe to yield the final result. In order to avoid overflows
 * completely the input signal must be scaled down by log2(coeff_size) bits.
 */

/* function description */
void riscv_dsp_lms_q31(const riscv_dsp_lms_q31_t * FUNC_RESTRICT instance,
                   q31_t * FUNC_RESTRICT src, q31_t * FUNC_RESTRICT ref, q31_t * FUNC_RESTRICT dst, q31_t * FUNC_RESTRICT err, uint32_t size)
{
    q31_t *state = instance->state;
    q31_t *px, *pf, *scurr;
    q31_t mu = instance->mu;
    uint32_t coeff_size = instance->coeff_size;
    uint32_t tapcnt;
    q63_t acc;
    q31_t acc_l, acc_h;
    q31_t w, e = 0;
    int32_t ushift = ((uint32_t) instance->shift + 1u);
    int32_t lshift = 32u - ushift;

    scurr = &(instance->state[(coeff_size - 1u)]);

    /* block process */
    while (size != 0u)
    {
        *scurr++ = *src++;
        px = state;
        pf = instance->coeff;
        acc = 0;

        /* filter phase */
        tapcnt = coeff_size;
        while (tapcnt != 0u)
        {
            acc += ((q63_t) (*px++)) * (*pf++);
            tapcnt--;
        }

        acc_l = acc & 0xffffffff;
        acc_h = (acc >> 32) & 0xffffffff;
        acc = (uint32_t) acc_l >> lshift | acc_h << ushift;
        *dst++ = (q31_t) acc;

        /* update phase */
        e = (*ref++) - (q31_t) acc;
        *err++ = (q31_t) e;
        w = (q31_t) (((q63_t) e * mu) >> 31);
        px = state++;
        pf = instance->coeff;

        tapcnt = coeff_size;
        while (tapcnt != 0u)
        {
            LOOP_HINT_NO_HWLOOP();   //add this to improve performance since this loop flow is not regular.
            *pf++ += ((q31_t) (((q63_t) w * (*px++)) >> 32)) << 1u;
            tapcnt--;
        }
        /* end of update phase */
        size--;
    }
    /* end of block process */

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
