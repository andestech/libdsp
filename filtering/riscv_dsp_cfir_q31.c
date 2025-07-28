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
void riscv_dsp_cfir_q31(const riscv_dsp_cfir_q31_t * FUNC_RESTRICT instance, q31_t * FUNC_RESTRICT src, q31_t * FUNC_RESTRICT dst, uint32_t size)
{

    q31_t *state = instance->state;
    q31_t *coeff = instance->coeff;
    q31_t *pst, *scurr;
    q31_t *pcoe;
    q63_t acc0_re, acc0_im;
    long coeff_size = instance->coeff_size;
    uint32_t statecnt, szcnt, coecnt;

    scurr = &(instance->state[(coeff_size - 1u) * 2]);

    szcnt = size;
    q31_t coe_re, coe_im;
    q31_t st_re, st_im;
    while (szcnt > 0u)
    {
        *scurr++ = *src++;
        *scurr++ = *src++;
        acc0_re = 0;
        acc0_im = 0;

        pst = state;
        pcoe = coeff;

        coecnt = coeff_size;
        while (coecnt > 0u)
        {
            coe_re = *pcoe++;
            coe_im = *pcoe++;
            st_re = *pst++;
            st_im = *pst++;

            acc0_re += (q63_t) coe_re * st_re;
            acc0_re -= (q63_t) coe_im * st_im;
            acc0_im += (q63_t) coe_re * st_im;
            acc0_im += (q63_t) coe_im * st_re;
            coecnt--;
        }
        *dst++ = (q31_t) (acc0_re >> 31u);
        *dst++ = (q31_t) (acc0_im >> 31u);
        state = state + 2;
        szcnt--;
    }

    scurr = instance->state;
    statecnt = (coeff_size - 1u);

    while (statecnt != 0u)
    {
        *scurr++ = *state++;
        *scurr++ = *state++;
        statecnt--;
    }
}
