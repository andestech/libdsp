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

/* function description */
float64_t riscv_dsp_var_f64(const float64_t *src, uint32_t size)
{
    float64_t dst = 0.0f;

    if (size > 1)
    {
        float64_t sum = 0.0, sos = 0.0;
        float64_t tmp1, sqrsum;
        uint32_t i = size;

        do
        {
            tmp1 = *src++;
            sos += tmp1 * tmp1;
            sum += tmp1;
        }
        while (--i);

        sqrsum = ((sum * sum) / (float64_t) size);
        dst = ((sos - sqrsum) / (float64_t) (size - 1));
    }
    return dst;
}
