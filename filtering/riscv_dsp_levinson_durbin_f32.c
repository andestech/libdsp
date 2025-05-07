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

/**
  @brief         Levinson Durbin
  @param[in]     src      autocovariance vector starting with lag 0 (length is coeff_size + 1)
  @param[out]    coeff      autoregressive coefficients
  @param[out]    err      prediction error (variance)
  @param[in]     coeff_size   number of autoregressive coefficients
  @return        none
 */

/* function description */
void riscv_dsp_levinson_durbin_f32(const float32_t * FUNC_RESTRICT src, float32_t * FUNC_RESTRICT coeff, float32_t * FUNC_RESTRICT err, uint32_t coeff_size)
{
    float32_t e;
    uint32_t p;

    coeff[0] = src[1] / src[0];

    e = src[0] - src[1] * coeff[0];
    for(p = 1; p < coeff_size; p++)
    {
        float32_t suma = 0.0f;
        float32_t sumb = 0.0f;
        float32_t k;
        uint32_t nb, j, i;

        for(i = 0; i < p; i++)
        {
            suma += coeff[i] * src[p - i];
            sumb += coeff[i] * src[i + 1];
        }

        k = (src[p + 1] - suma) / (src[0] - sumb);


        nb = p >> 1;
        j = 0;
        for(i = 0; i < nb ; i++)
        {
            float32_t x, y;

            x = coeff[j] - k * coeff[p - 1 - j];
            y = coeff[p - 1 - j] - k * coeff[j];

            coeff[j] = x;
            coeff[p - 1 - j] = y;

            j++;
        }

        nb = p & 1;
        if (nb)
        {
            coeff[j] = coeff[j] - k * coeff[p - 1 - j];
        }

        coeff[p] = k;
        e = e * (1.0f - k * k);
    }
    *err = e;
}
