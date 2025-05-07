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

static void _dsp_heapify(float32_t * src, uint32_t n, uint32_t i, uint8_t order)
{
    /* Put all the elements of src in heap order */
    uint32_t k = i; // Initialize largest/smallest as root
    uint32_t l = 2 * i + 1; // left = 2*i + 1
    uint32_t r = 2 * i + 2; // right = 2*i + 2
    float32_t temp;

    if (l < n && order == (src[l] > src[k]) )
        k = l;

    if (r < n && order == (src[r] > src[k]) )
        k = r;

    if (k != i)
    {
        temp = src[i];
        src[i] = src[k];
        src[k] = temp;

        _dsp_heapify(src, n, k, order);
    }
}

/**
  @ingroup groupSupport
 */

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
   *               The heap sort algorithm is a comparison algorithm that
   *               divides the input array into a sorted and an unsorted region,
   *               and shrinks the unsorted region by extracting the largest
   *               element and moving it to the sorted region. A heap data
   *               structure is used to find the maximum.
   *
   * @par          It's an in-place algorithm. In order to obtain an out-of-place
   *               function, a memcpy of the source vector is performed.
   */
void riscv_dsp_sort_heap_f32(
    const riscv_dsp_sort_f32_t * instance,
    float32_t * src,
    float32_t * dst,
    uint32_t size)
{
    float32_t * pA;
    int32_t i;
    float32_t temp;

    if (src != dst) // out-of-place
    {
        memcpy(dst, src, size*sizeof(float32_t) );
        pA = dst;
    }
    else
        pA = src;

    // Build the heap array so that the largest value is the root
    for (i = size / 2 - 1; i >= 0; i--)
        _dsp_heapify(pA, size, i, instance->order);

    for (i = size - 1; i >= 0; i--)
    {
        // Swap
        temp = pA[i];
        pA[i] = pA[0];
        pA[0] = temp;

        // Restore heap order
        _dsp_heapify(pA, i, 0, instance->order);
    }
}
/**
  @} end of Sorting group
 */
