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
#include "internal_utils_math.h"

#include "internal_isa.h"

/**
 * @ingroup basic
 */

/**
 * @addtogroup basicdiv
 * @{
 */

/**
 * @brief Division of positive 64-bits inputs divided by a positive 32-bits.
 * @param[in]       src1 the positive 64-bits input value.
 * @param[in]       src2 the positive 32-bits input value.
 * @return division of two inputs.
 */

/* function description */
q31_t riscv_dsp_div_u64_u32(uint64_t src1, uint32_t src2)
{
#ifdef NDS_CPU_64
    return  (src1 / src2);
#else
    return  dsp_div_u64_u32(src1, src2);
#endif
}

/**
 * @} end of basicdiv
 */
