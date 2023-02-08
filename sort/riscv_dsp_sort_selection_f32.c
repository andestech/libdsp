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
   * @private
   * @param[in]  S          points to an instance of the sorting structure.
   * @param[in]  src       points to the block of input data.
   * @param[out] dst       points to the block of output data
   * @param[in]  size  number of samples to process.
   *
   * @par        Algorithm
   *               The Selection sort algorithm is a comparison algorithm that
   *               divides the input array into a sorted and an unsorted sublist
   *               (initially the sorted sublist is empty and the unsorted sublist
   *               is the input array), looks for the smallest (or biggest)
   *               element in the unsorted sublist, swapping it with the leftmost
   *               one, and moving the sublists boundary one element to the right.
   *
   * @par          It's an in-place algorithm. In order to obtain an out-of-place
   *               function, a memcpy of the source vector is performed.
   */

void riscv_dsp_sort_selection_f32(
    const riscv_dsp_sort_f32_t * instance,
    float32_t * src,
    float32_t * dst,
    uint32_t size)
{
    uint32_t i, j, k;
    uint8_t order = instance->order;
    float32_t temp;

    float32_t * pA;

    if (src != dst) // out-of-place
    {
        memcpy(dst, src, size*sizeof(float32_t) );
        pA = dst;
    }
    else
        pA = src;

    /*  Move the boundary one element to the right */
    for (i = 0; i < size - 1; i++)
    {
        /* Initialize the minimum/maximum as the first element */
        k = i;

        /* Look in the unsorted list to find the minimum/maximum value */
        for (j = i + 1; j < size; j++)
        {
            if (order == (pA[j] < pA[k]) )
            {
                /* Update value */
                k = j;
            }
        }

        if (k != i)
        {
            /* Swap the minimum/maximum with the leftmost element */
            temp = pA[i];
            pA[i] = pA[k];
            pA[k] = temp;
        }
    }
}

/**
  @} end of Sorting group
 */
