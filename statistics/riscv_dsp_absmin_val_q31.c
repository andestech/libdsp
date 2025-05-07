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
#include "internal_isa.h"

/* function description */
q31_t riscv_dsp_absmin_val_q31(const q31_t* FUNC_RESTRICT src, uint32_t size)
{
    //First, check if input size is zero?
    if (size == 0)
    {
        return (q31_t)0x80000000;
    }
    uint32_t i;
    i = 0U;

    q31_t min_val, temp_val;
    min_val = (*src > 0) ? *src : ((*src == INT32_MIN) ? INT32_MAX : -*src);
    src++;

    while (i < (size - 1))
    {
        temp_val = (*src > 0) ? *src : ((*src == INT32_MIN) ? INT32_MAX : -*src);
        src++;
        i++;
        if (min_val > temp_val)
        {
            min_val = temp_val;
        }
    }

    return min_val;

}
