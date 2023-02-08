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
 * @addtogroup upsplfir
 * @{
 */

/**
 * @brief Function for the q15 Upsampling FIR filter.
 * @param[in]       *instance points to an instance of the Upsampling FIR
 *                            structure.
 * @param[in]       *src      points to the input block data.
 * @param[out]      *dst      points to the output block data.
 * @param[in]       size      number of the size.
 * @return none.
 *
 * Function notes:
 * Both coefficients and state variables are represented in 1.31 format
 * and multiplications yield a 2.62 result. The 2.62 results are accumulated
 * in a 64-bit accumulator. After all multiply-accumulates are performed, the
 * 2.62 accumulator is truncated to 1.32 format and then saturated to 1.31
 * format. In order to avoid overflows completely the input signal must be
 * scaled down by <code>1 / (plen / L)</code> since
 * <code>plen / L</code> additions occur per output sample.
 */

/* function description */
void riscv_dsp_upsplfir_q31(const riscv_dsp_upsplfir_q31_t * FUNC_RESTRICT instance, q31_t * FUNC_RESTRICT src, q31_t * FUNC_RESTRICT dst, uint32_t size)
{
    q31_t *state = instance->state;
    q31_t *scurr, *px, *pb;
    q63_t sum;
    uint32_t plen = instance->plen;
    uint32_t tapcnt, blkcnt;
    int32_t i;

    scurr = instance->state + ((q31_t) plen - 1u);

    blkcnt = size;
    while (blkcnt != 0u)
    {
        *scurr++ = *src++;

        i = (int32_t)instance->L - 1;
        while (i != 0)
        {
            sum = 0;
            px = state;
            pb = instance->coeff + i;

            tapcnt = plen;
            while (tapcnt != 0u)
            {
                sum += ((q63_t) * px++ **pb);
                pb += instance->L;
                tapcnt--;
            }

            *dst++ = (q31_t) (sum >> 31);
            i--;
        }
        sum = 0;
        px = state;
        pb = instance->coeff;

        tapcnt = plen;
        while (tapcnt != 0u)
        {
            sum += ((q63_t) * px++ **pb);
            pb += instance->L;
            tapcnt--;
        }

        *dst++ = (q31_t) (sum >> 31);
        state = state + 1;
        blkcnt--;
    }

    scurr = instance->state;

    i = (plen - 1u);
    while (i != 0u)
    {
        *scurr++ = *state++;
        i--;
    }
}

/**
 * @} end of upsplfir
 */
