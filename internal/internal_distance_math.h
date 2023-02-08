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
#ifndef __INTERNAL_DISTANCE_MATH_H__
#define __INTERNAL_DISTANCE_MATH_H__

#include "riscv_dsp_distance_math.h"

static inline void _dsp_boolean_dist_TT_FF_TF_FT(const uint32_t *src1,
                                                const uint32_t *src2,
                                                uint32_t numofbool,
                                                uint32_t *cnt_tt,
                                                uint32_t *cnt_ff,
                                                uint32_t *cnt_tf,
                                                uint32_t *cnt_ft)
{
    uint32_t _ctt=0;
    uint32_t _cff=0;
    uint32_t _ctf=0;
    uint32_t _cft=0;
    uint32_t a,b,ba,bb;
    int shift;
    uint32_t cnt = numofbool >> 5;
    uint32_t res = numofbool & 31;

    while(cnt != 0)
    {
       a = *src1++;
       b = *src2++;
       shift = 0;
       while(shift < 32)
       {
          ba = a & 1;
          bb = b & 1;
          a = a >> 1;
          b = b >> 1;
          _ctt += (ba && bb);
          _cff += ((1 ^ ba) && (1 ^ bb));
          _ctf += (ba && (1 ^ bb));
          _cft += ((1 ^ ba) && bb);
          shift ++;
       }
       cnt--;
    }

    a = *src1++;
    b = *src2++;

    a = a >> (32 - res);
    b = b >> (32 - res);

    while(res > 0)
    {
          ba = a & 1;
          bb = b & 1;
          a = a >> 1;
          b = b >> 1;
          _ctt += (ba && bb);
          _cff += ((1 ^ ba) && (1 ^ bb));
          _ctf += (ba && (1 ^ bb));
          _cft += ((1 ^ ba) && bb);
          res --;
    }
    *cnt_tt = _ctt;
    *cnt_ff = _cff;
    *cnt_tf = _ctf;
    *cnt_ft = _cft;
}

static inline void _dsp_boolean_dist_TT_TF_FT(const uint32_t *src1,
                                             const uint32_t *src2,
                                             uint32_t numofbool,
                                             uint32_t *cnt_tt,
                                             uint32_t *cnt_tf,
                                             uint32_t *cnt_ft)
{
    uint32_t _ctt=0;
    uint32_t _ctf=0;
    uint32_t _cft=0;
    uint32_t a,b,ba,bb;
    int shift;
    uint32_t cnt = numofbool >> 5;
    uint32_t res = numofbool & 31;

    while(cnt != 0)
    {
       a = *src1++;
       b = *src2++;
       shift = 0;
       while(shift < 32)
       {
          ba = a & 1;
          bb = b & 1;
          a = a >> 1;
          b = b >> 1;
          _ctt += (ba && bb);
          _ctf += (ba && (1 ^ bb));
          _cft += ((1 ^ ba) && bb);
          shift ++;
       }
       cnt--;
    }

    a = *src1++;
    b = *src2++;

    a = a >> (32 - res);
    b = b >> (32 - res);

    while(res > 0)
    {
          ba = a & 1;
          bb = b & 1;
          a = a >> 1;
          b = b >> 1;
          _ctt += (ba && bb);
          _ctf += (ba && (1 ^ bb));
          _cft += ((1 ^ ba) && bb);
          res--;
    }
    *cnt_tt = _ctt;
    *cnt_tf = _ctf;
    *cnt_ft = _cft;
}

static inline void _dsp_boolean_dist_TF_FT(const uint32_t *src1,
                                          const uint32_t *src2,
                                          uint32_t numofbool,
                                          uint32_t *cnt_tf,
                                          uint32_t *cnt_ft)
{
    uint32_t _ctf=0;
    uint32_t _cft=0;
    uint32_t a,b,ba,bb;
    int shift;
    uint32_t cnt = numofbool >> 5;
    uint32_t res = numofbool & 31;

    while(cnt != 0)
    {
       a = *src1++;
       b = *src2++;
       shift = 0;
       while(shift < 32)
       {
          ba = a & 1;
          bb = b & 1;
          a = a >> 1;
          b = b >> 1;
          _ctf += (ba && (1 ^ bb));
          _cft += ((1 ^ ba) && bb);
          shift ++;
       }
       cnt--;
    }

    a = *src1++;
    b = *src2++;

    a = a >> (32 - res);
    b = b >> (32 - res);

    while(res > 0)
    {
          ba = a & 1;
          bb = b & 1;
          a = a >> 1;
          b = b >> 1;
          _ctf += (ba && (1 ^ bb));
          _cft += ((1 ^ ba) && bb);
          res--;
    }
    *cnt_tf = _ctf;
    *cnt_ft = _cft;
}

static inline void _dsp_boolean_dist_TT(const uint32_t *src1,
                                       const uint32_t *src2,
                                       uint32_t numofbool,
                                       uint32_t *cnt_tt)
{
    uint32_t _ctt=0;
    uint32_t a,b,ba,bb;
    int shift;
    uint32_t cnt = numofbool >> 5;
    uint32_t res = numofbool & 31;

    while(cnt != 0)
    {
       a = *src1++;
       b = *src2++;
       shift = 0;
       while(shift < 32)
       {
          ba = a & 1;
          bb = b & 1;
          a = a >> 1;
          b = b >> 1;
          _ctt += (ba && bb);
          shift ++;
       }
       cnt--;
    }

    a = *src1++;
    b = *src2++;

    a = a >> (32 - res);
    b = b >> (32 - res);

    while(res > 0)
    {
          ba = a & 1;
          bb = b & 1;
          a = a >> 1;
          b = b >> 1;
          _ctt += (ba && bb);
          res--;
    }
    *cnt_tt = _ctt;
}
#endif // __INTERNAL_DISTANCE_MATH_H__
