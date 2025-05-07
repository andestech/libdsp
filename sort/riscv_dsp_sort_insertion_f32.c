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
  @addtogroup Sorting
  @{
 */

/**
   * @private
   * @param[in]  S          points to an instance of the sorting structure.
   * @param[in]  src       points to the block of input data.
   * @param[out] dst       points to the block of output data
   * @param[in]  size  number of samples to process.
   *
   * @par        Algorithm
   *               The insertion sort is a simple sorting algorithm that
   *               reads all the element of the input array and removes one element
   *               at a time, finds the location it belongs in the final sorted list,
   *               and inserts it there.
   *
   * @par          It's an in-place algorithm. In order to obtain an out-of-place
   *               function, a memcpy of the source vector is performed.
   */

void riscv_dsp_sort_insertion_f32(
    const riscv_dsp_sort_f32_t * instance,
    float32_t *src,
    float32_t* dst,
    uint32_t size)
{
    float32_t * pA;
    uint8_t order = instance->order;
    uint32_t i, j;
    float32_t temp;

    if (src != dst) // out-of-place
    {
        memcpy(dst, src, size*sizeof(float32_t) );
        pA = dst;
    }
    else
        pA = src;

    // Real all the element of the input array
    for (i = 0; i < size; i++)
    {
        // Move the i-th element to the right position
        for (j = i; j > 0 && order == (pA[j] < pA[j-1]); j--)
        {
            // Swap
            temp = pA[j];
            pA[j] = pA[j-1];
            pA[j-1] = temp;
        }
    }
}

/**
  @} end of Sorting group
 */
