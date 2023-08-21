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

#define LOG_Q31_ACCURACY 31

/* Bit to represent the normalization factor
   It is Ceiling[Log2[LOG_Q31_ACCURACY]] of the previous value.
   The Log2 algorithm is assuming that the value x is
   1 <= x < 2.

   But input value could be as small a 2^-LOG_Q31_ACCURACY
   which would give an integer part of -31.
*/
#define LOG_Q31_INTEGER_PART 5

/* 2.0 in Q30 */
#define LOQ_Q31_THRESHOLD (1u << LOG_Q31_ACCURACY)

/* HALF */
#define LOQ_Q31_Q32_HALF LOQ_Q31_THRESHOLD
#define LOQ_Q31_Q30_HALF (LOQ_Q31_Q32_HALF >> 2)


/* 1.0 / Log2[Exp[1]] in Q31 */
#define LOG_Q31_INVLOG2EXP 0x58b90bfbuL

/* Clay Turner algorithm */
static inline uint32_t _scalar_log_q31(uint32_t src)
{
   int32_t i;

   int32_t c = NDS_ISA_CLZ(src);
   int32_t normalization=0;

   /* 0.5 in q26 */
   uint32_t inc = LOQ_Q31_Q32_HALF >> (LOG_Q31_INTEGER_PART + 1);

   /* Will compute y = log2(x) for 1 <= x < 2.0 */
   uint32_t x;

   /* q26 */
   uint32_t y=0;

   /* q26 */
   int32_t tmp;


   /* Normalize and convert to q30 format */
   x = src;
   if ((c-1) < 0)
   {
     x = x >> (1-c);
   }
   else
   {
     x = x << (c-1);
   }
   normalization = c;

   /* Compute the Log2. Result is in q26
      because we know 0 <= y < 1.0 but
      do not want to use q32 to allow
      following computation with less instructions.
   */
   for(i = 0; i < LOG_Q31_ACCURACY ; i++)
   {
      x = ((int64_t)x*x)  >> (LOG_Q31_ACCURACY - 1);

      if (x >= LOQ_Q31_THRESHOLD)
      {
         y += inc ;
         x = x >> 1;
      }
      inc = inc >> 1;
   }

   /*
      Convert the Log2 to Log and apply normalization.
      We compute (y - normalisation) * (1 / Log2[e]).

   */

   /* q26 */
   tmp = (int32_t)y - (normalization << (LOG_Q31_ACCURACY - LOG_Q31_INTEGER_PART));

   /* q5.26 */
   y = ((int64_t)tmp * LOG_Q31_INVLOG2EXP) >> 31;



   return(y);

}


/* function description */
q31_t riscv_dsp_log_q31(q31_t src)
{
    q31_t output;
    output = _scalar_log_q31(src);
    return output;
}

/**
 * @} end of utilsset
 */
