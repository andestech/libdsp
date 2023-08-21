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
 * Both coefficients and state variables are represented in 1.15 format
 * and multiplications yield a 2.30 result. The 2.30 results are accumulated
 * in a 64-bit accumulator in 34.30 format and the results is truncated
 * to 34.15 format by discarding low 15 bits. Lastly, the outputs is
 * saturated to yield a result in 1.15 format.
 */

/* function description */
void riscv_dsp_fir_q15(const riscv_dsp_fir_q15_t * FUNC_RESTRICT instance,
                 q15_t * FUNC_RESTRICT src, q15_t * FUNC_RESTRICT dst, uint32_t size)
{
    q15_t *state = instance->state;
    q15_t *px, *pf, *scurr;
    uint32_t coeff_size = instance->coeff_size;
    uint32_t i, tapcnt;
    q63_t acc;

    scurr = &(instance->state[(coeff_size - 1u)]);


    while (size != 0u)
    {
        *scurr++ = *src++;
        acc = 0;

        /* Initialize filter coefficient and state pointer */
        px = state;
        pf = instance->coeff;

        i = coeff_size;
        while (i != 0u)
        {
            acc += (q31_t) * px++ **pf++;
            i--;
        }

        *dst++ = (q15_t) NDS_ISA_SATS((acc >> 15u), 16);

        state = state + 1;
        size--;
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
