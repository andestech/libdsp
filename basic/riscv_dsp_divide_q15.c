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

int riscv_dsp_divide_q15(q15_t numerator, q15_t denominator, q15_t *quotient, int16_t *shift)
{
    int16_t sign=0;
    q31_t temp;
    int16_t shiftForNormalizing;

    *shift = 0;

    sign = (numerator >> 15) ^ (denominator >> 15);

    if (denominator == 0)
    {
       if (sign)
       {
          *quotient = 0x8000;
       }
       else
       {
          *quotient = 0x7FFF;
       }
       return -1;
    }

    q15_t dvd, dvs;
    dvd = NDS_ISA_ABS(numerator);
    dvs = NDS_ISA_ABS(denominator);

    temp = ((q31_t)dvd << 15) / ((q31_t)dvs);

    shiftForNormalizing= 17 - NDS_ISA_CLZ(temp);

    if (shiftForNormalizing > 0)
    {
       *shift = shiftForNormalizing;
       temp = temp >> shiftForNormalizing;
    }
    if (sign)
    {
      temp = -temp;
    }

    *quotient = temp;

    return 0;
}
