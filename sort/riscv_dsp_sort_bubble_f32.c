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
   * @private
   * @param[in]  instance  points to an instance of the sorting structure.
   * @param[in]  src       points to the block of input data.
   * @param[out] dst       points to the block of output data
   * @param[in]  size  number of samples to process.
   *
   * @par        Algorithm
   *               The bubble sort algorithm is a simple comparison algorithm that
   *               reads the elements of a vector from the beginning to the end,
   *               compares the adjacent ones and swaps them if they are in the
   *               wrong order. The procedure is repeated until there is nothing
   *               left to swap. Bubble sort is fast for input vectors that are
   *               nearly sorted.
   *
   * @par          It's an in-place algorithm. In order to obtain an out-of-place
   *               function, a memcpy of the source vector is performed
   */

void riscv_dsp_sort_bubble_f32(
    const riscv_dsp_sort_f32_t * instance,
    float32_t * src,
    float32_t * dst,
    uint32_t size)
{
    uint8_t order = instance->order;
    uint32_t i;
    uint8_t swap = 1;
    float32_t * pA;
    float32_t temp;

    if (src != dst) // out-of-place
    {
        memcpy(dst, src, size*sizeof(float32_t) );
        pA = dst;
    }
    else
        pA = src;


    while (swap == 1) // If nothing has been swapped after one loop stop
    {
        swap = 0;

        for (i = 0; i < size - 1; i++)
        {
            if (order == (pA[i] > pA[i+1]))
            {
                // Swap
                temp = pA[i];
                pA[i] = pA[i+1];
                pA[i+1] = temp;

                // Update flag
                swap = 1;
            }
        }

        size--;
    }
}

/**
  @} end of Sorting group
 */
