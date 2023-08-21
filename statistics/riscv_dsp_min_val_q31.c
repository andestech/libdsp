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

/* function description */
q31_t riscv_dsp_min_val_q31(const q31_t * FUNC_RESTRICT src, uint32_t size)
{
    q31_t min_val, temp_val;
    uint32_t i;

    //First, check if input size is zero?
    if (size <= 0)
    {
        return (q31_t)0x7fffffff;
    }

    i = 0U;
    min_val = *src++;

    while (i < (size - 1))
    {
        temp_val = *src++;
        i++;
        min_val = (min_val < temp_val) ? min_val : temp_val;
    }
    return min_val;
}

/**
 * @} end of min.
 */
