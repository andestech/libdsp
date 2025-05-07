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
#include "riscv_dsp_basic_math.h"

#define ONE_Q31 0x7FFFFFFFL
#define TWO_Q30 0x7FFFFFFFL

#define HALF_Q31 0x00008000L
#define ONE_Q15 0x7FFF
#define HALF_Q15 0x3FFF
#define LOWPART_MASK 0x07FFF

static inline q31_t dsp_mul32x16(q31_t a, q15_t b)
{
    q31_t r = ((q63_t)a * b) >> 15;
    return(r);
}

static inline q31_t dsp_mul32x32(q31_t a, q31_t b)
{
    q31_t r = ((q63_t)a * b) >> 31;
    return(r);
}

static inline q31_t dsp_divide(q31_t n, q31_t d)
{
    int16_t shift;
    q15_t inverse;
    q31_t r;
    riscv_dsp_divide_q15(HALF_Q15, d >> 16, &inverse, &shift);
    r = dsp_mul32x16(d, inverse);
    r = TWO_Q30 - (r << shift);
    r = dsp_mul32x16(r, inverse);
    r = dsp_mul32x32(r, n) ;
    r = r << (shift + 2);
    return(r);
}

#include "stdio.h"

/* function description */
void riscv_dsp_levinson_durbin_q31(const q31_t * FUNC_RESTRICT src, q31_t * FUNC_RESTRICT coeff, q31_t * FUNC_RESTRICT err, uint32_t coeff_size)
{
    q31_t e;
    uint32_t p;

    //coeff[0] = src[1] / src[0];
    coeff[0] = dsp_divide(src[1], src[0]);

    //e = src[0] - src[1] * coeff[0];
    e = src[0] - dsp_mul32x32(src[1], coeff[0]);
    for(p = 1; p < coeff_size; p++)
    {
        q63_t suma = 0;
        q63_t sumb = 0;
        q31_t k;
        uint32_t nb, j, i;

        for(i = 0; i < p; i++)
        {
            suma += ((q63_t)coeff[i] * src[p - i]);
            sumb += ((q63_t)coeff[i] * src[i + 1]);
        }

        suma = suma >> 31;
        sumb = sumb >> 31;

        //k = (src[p+1]-suma)/(src[0] - sumb);
        k = dsp_divide(src[p + 1] - (q31_t)suma, src[0] - (q31_t)sumb);

        nb = p >> 1;
        j = 0;
        for(i = 0; i < nb ; i++)
        {
            q31_t x, y;

            //x = coeff[j] - k * coeff[p-1-j];
            x = coeff[j] - dsp_mul32x32(k, coeff[p - 1 - j]);

            //y = coeff[p-1-j] - k * coeff[j];
            y = coeff[p - 1 - j] - dsp_mul32x32(k, coeff[j]);

            coeff[j] = x;
            coeff[p - 1 - j] = y;

            j++;
        }

        nb = p & 1;
        if (nb)
        {
            //coeff[j] = coeff[j]- k * coeff[p-1-j];
            coeff[j] = coeff[j] - dsp_mul32x32(k, coeff[p - 1 - j]);
        }

        coeff[p] = k;

        // e = e * (1 - k*k);
        e = dsp_mul32x32(e, ONE_Q31 - dsp_mul32x32(k, k));
    }
    *err = e;
}
