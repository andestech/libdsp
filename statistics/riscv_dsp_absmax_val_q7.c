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
#include "internal_isa.h"

q7_t riscv_dsp_absmax_val_q7(const q7_t * FUNC_RESTRICT src, uint32_t size)
{
    //First, check if input size is zero?
    if (size <= 0)
    {
        return (q7_t)0x0;
    }
    uint32_t i;

    //pureC
    q7_t max_val, temp_val;
    max_val = (*src > 0) ? *src : ((*src == (q7_t)0x80) ? 0x7f : -*src);
    src++;

    i = 0U;
    while (i < (size - 1))
    {
        temp_val = (*src > 0) ? *src : ((*src == (q7_t)0x80) ? 0x7f : -*src);
        src++;
        i++;
        if (max_val < temp_val)
        {
            max_val = temp_val;
        }
    }
    return (q7_t)max_val;

}
