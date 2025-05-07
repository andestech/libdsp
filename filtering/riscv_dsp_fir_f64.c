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
#include "internal_filtering_math.h"

/* function description */
void riscv_dsp_fir_f64(const riscv_dsp_fir_f64_t * FUNC_RESTRICT instance,
                       float64_t * FUNC_RESTRICT src, float64_t * FUNC_RESTRICT dst, uint32_t size)
{
    float64_t *state = instance->state;
    float64_t *px, *pf, *scurr;
    uint32_t coeff_size = instance->coeff_size;
    uint32_t i, tapcnt;
    float64_t acc;

    scurr = &(instance->state[(coeff_size - 1u)]);

    /* block loop */
    while (size != 0u)
    {
        *scurr++ = *src++;
        acc = 0.0f;

        /* Initialize filter coefficient and state pointer */
        px = state;
        pf = instance->coeff;

        i = coeff_size;
        while (i != 0u)
        {
            acc += *px++ **pf++;
            i--;
        }

        *dst++ = acc;
        state = state + 1;
        size--;
    }

    scurr = instance->state;
    tapcnt = coeff_size - 1u;
    while (tapcnt != 0u)
    {
        *scurr++ = *state++;
        tapcnt--;
    }
}
