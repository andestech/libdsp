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
#include "internal_sort_math.h"

/**
  @addtogroup Sorting
  @{
 */


/**
 * @brief Generic sorting function
 *
 * @param[in]  instance  points to an instance of the sorting structure.
 * @param[in]  src       points to the block of input data.
 * @param[out] dst       points to the block of output data.
 * @param[in]  size  number of samples to process.
 */

void riscv_dsp_sort_f32(
    const riscv_dsp_sort_f32_t * instance,
    float32_t * src,
    float32_t * dst,
    uint32_t size)
{
    switch (instance->alg)
    {
    case RISCV_DSP_SORT_BITONIC:
        riscv_dsp_sort_bitonic_f32(instance, src, dst, size);
        break;

    case RISCV_DSP_SORT_BUBBLE:
        riscv_dsp_sort_bubble_f32(instance, src, dst, size);
        break;

    case RISCV_DSP_SORT_HEAP:
        riscv_dsp_sort_heap_f32(instance, src, dst, size);
        break;

    case RISCV_DSP_SORT_INSERTION:
        riscv_dsp_sort_insertion_f32(instance, src, dst, size);
        break;

    case RISCV_DSP_SORT_QUICK:
        riscv_dsp_sort_quick_f32(instance, src, dst, size);
        break;

    case RISCV_DSP_SORT_SELECTION:
        riscv_dsp_sort_selection_f32(instance, src, dst, size);
        break;
    }
}

/**
  @} end of Sorting group
 */
