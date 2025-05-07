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

static void _dsp_topdown_merge(float32_t * pA, uint32_t begin, uint32_t middle, uint32_t end, float32_t * pB, uint8_t order)
{
    /* Left  array is pA[begin:middle-1]
     * Right Array is pA[middle:end-1]
     * They are merged in pB
     */

    uint32_t i = begin;
    uint32_t j = middle;
    uint32_t k;

    // Read all the elements in the sublist
    for (k = begin; k < end; k++)
    {
        // Merge
        if (i < middle && (j >= end || order == (pA[i] <= pA[j])) )
        {
            pB[k] = pA[i];
            i++;
        }
        else
        {
            pB[k] = pA[j];
            j++;
        }
    }
}

//_dsp_merge_sort_core_f32(instance->buf, 0, size, pA, instance->order);
static void _dsp_merge_sort_core_f32(float32_t * pB, uint32_t begin, uint32_t end, float32_t * pA, uint8_t order)
{
    if ((int32_t)end - (int32_t)begin >= 2 )          // If run size != 1 divide
    {
        int32_t middle = (end + begin) / 2;           // Take the middle point

        _dsp_merge_sort_core_f32(pA, begin,  middle, pB, order);  // Sort the left part
        _dsp_merge_sort_core_f32(pA, middle,    end, pB, order);  // Sort the right part

        _dsp_topdown_merge(pB, begin, middle, end, pA, order);
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
   * @param[in]  instance  points to an instance of the sorting structure.
   * @param[in]  src       points to the block of input data.
   * @param[out] dst       points to the block of output data
   * @param[in]  size  number of samples to process.
   *
   * @par        Algorithm
   *               The merge sort algorithm is a comparison algorithm that
   *               divide the input array in sublists and merge them to produce
   *               longer sorted sublists until there is only one list remaining.
   *
   * @par          A work array is always needed. It must be allocated by the user
   *               linked to the instance at initialization time.
   *
   * @par          It's an in-place algorithm. In order to obtain an out-of-place
   *               function, a memcpy of the source vector is performed
   */


void riscv_dsp_sort_merge_f32(
    const riscv_dsp_sort_merge_f32_t * instance,
    float32_t *src,
    float32_t *dst,
    uint32_t size)
{
    float32_t * pA;

    /* Out-of-place */
    if (src != dst)
    {
        memcpy(dst, src, size*sizeof(float32_t));
        pA = dst;
    }
    else
        pA = src;

    /* A working buffer is needed */
    memcpy(instance->buf, src, size*sizeof(float32_t));

    _dsp_merge_sort_core_f32(instance->buf, 0, size, pA, instance->order);
}
/**
  @} end of Sorting group
 */
