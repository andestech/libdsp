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

static void _dsp_bitonic_sort_core_f32(float32_t *src, uint32_t n, uint8_t order)
{

    uint32_t step;
    uint32_t k, j;
    float32_t *leftPtr, *rightPtr;
    float32_t temp;

    step = n >> 1;
    leftPtr = src;
    rightPtr = src + n - 1;

    for (k = 0; k < step; k++)
    {
        if (order == (*leftPtr > *rightPtr))
        {
            // Swap
            temp = *leftPtr;
            *leftPtr = *rightPtr;
            *rightPtr = temp;
        }

        leftPtr++;  // Move right
        rightPtr--; // Move left
    }

    // Merge
    for (step = (n >> 2); step > 0; step /= 2)
    {
        for (j = 0; j < n; j = j + step * 2)
        {
            leftPtr  = src + j;
            rightPtr = src + j + step;

            for (k = 0; k < step; k++)
            {
                //if (*leftPtr > *rightPtr)
                if (order == (*leftPtr > *rightPtr))
                {
                    // Swap
                    temp = *leftPtr;
                    *leftPtr = *rightPtr;
                    *rightPtr = temp;
                }

                leftPtr++;
                rightPtr++;
            }
        }
    }
}

/**
  @ingroup sort
 */

/**
  @defgroup Sorting Vector sorting algorithms

  Sort the elements of a vector
 */

/**
  @addtogroup Sort
  @{
 */

/**
  * @private
  * @param[in]  instance  points to an instance of the sorting structure.
  * @param[in]  src       points to the block of input data.
  * @param[out] dst       points to the block of output data
  * @param[in]  size  number of samples to process.
  */

void riscv_dsp_sort_bitonic_f32(
    const riscv_dsp_sort_f32_t * instance,
    float32_t * src,
    float32_t * dst,
    uint32_t size)
{
    uint16_t s, i;
    uint8_t order = instance->order;

    float32_t * pA;

    if (src != dst) // out-of-place
    {
        memcpy(dst, src, size*sizeof(float32_t) );
        pA = dst;
    }
    else
        pA = src;

    if ( (size & (size - 1)) == 0 ) // Powers of 2 only
    {
        for (s = 2; s <= size; s = s * 2)
        {
            for (i = 0; i < size; i = i + s)
                _dsp_bitonic_sort_core_f32(pA + i, s, order);
        }
    }
}

/**
  @} end of Sorting group
 */
