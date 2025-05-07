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
#include "internal_sort_math.h"

/**
  @ingroup groupSupport
 */

/**
  @addtogroup Sorting
  @{
 */


/**
 * @param[in,out]  instance  points to an instance of the sorting structure.
 * @param[in]      order     Sorting order.
 * @param[in]      buf       Working buffer.
 */
void riscv_dsp_sort_merge_init_f32(riscv_dsp_sort_merge_f32_t * instance, riscv_dsp_sort_order order, float32_t * buf)
{
    instance->order = order;
    instance->buf   = buf;
}
/**
  @} end of Sorting group
 */
