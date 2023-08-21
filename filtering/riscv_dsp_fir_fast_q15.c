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
 * @addtogroup fir
 * @{
 */

/**
 * @brief Function for the q15 FIR filter.
 * @param[in]       *instance points to an instance of the FIR structure.
 * @param[in]       *src      points to the input block data.
 * @param[out]      *dst      points to the output block data.
 * @param[in]       size      number of the blocksize.
 * @return none.
 *
 * Function notes:
 * Both coefficients and state variables are represented in Q15 format and multiplications yield
 * a Q30 result. The results are accumulated in a 32-bit accumulator in Q2.30 format. Lastly, the
 * outputs are saturated to yield a result in Q1.15 format.
 */

/* function description */
void riscv_dsp_fir_fast_q15(const riscv_dsp_fir_q15_t * FUNC_RESTRICT instance,
                      q15_t * FUNC_RESTRICT src, q15_t * FUNC_RESTRICT dst, uint32_t size)
{
    q15_t *state = instance->state;
    q15_t *coeff = instance->coeff;
    q15_t *px, *pf, *scurr;
    uint32_t coeff_size = instance->coeff_size;
    uint32_t i, tapcnt;
    q31_t acc0, acc1, acc2, acc3;

    scurr = &(instance->state[(coeff_size - 1u)]);

    uint32_t sizecnt = size >> 2;
    while (sizecnt != 0u)
    {
        *scurr++ = *src++;
        *scurr++ = *src++;
        *scurr++ = *src++;
        *scurr++ = *src++;
        acc0 = 0;
        acc1 = 0;
        acc2 = 0;
        acc3 = 0;

        /* Initialize filter coefficient and state pointer */
        px = state;
        pf = coeff;

        i = coeff_size;
        while (i != 0u)
        {
            acc0 += (q31_t) * px **pf;
            acc1 += (q31_t) * (px + 1) **pf;
            acc2 += (q31_t) * (px + 2) **pf;
            acc3 += (q31_t) * (px + 3) **pf;
            px++;
            pf++;
            i--;
        }

        *dst++ = (q15_t) NDS_ISA_SATS((acc0 >> 15u), 16);
        *dst++ = (q15_t) NDS_ISA_SATS((acc1 >> 15u), 16);
        *dst++ = (q15_t) NDS_ISA_SATS((acc2 >> 15u), 16);
        *dst++ = (q15_t) NDS_ISA_SATS((acc3 >> 15u), 16);

        state = state + 4;
        sizecnt--;
    }

    sizecnt = size % 0x4u;
    while (sizecnt != 0)
    {
        *scurr++ = *src++;
        acc0 = 0;

        px = state;
        pf = coeff;

        i = coeff_size;
        while (i != 0u)
        {
            acc0 += (q31_t) * px++ **pf++;
            i--;
        }

        *dst++ = (q15_t) NDS_ISA_SATS((acc0 >> 15u), 16u);

        state = state + 1;
        sizecnt--;
    }
    scurr = instance->state;
    tapcnt = (coeff_size - 1u);

    while (tapcnt != 0u)
    {
        *scurr++ = *state++;
        tapcnt--;
    }

}

/**
 * @} end of fir
 */
