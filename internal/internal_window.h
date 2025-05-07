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
#ifndef __INTERNAL_WINDOW_H__
#define __INTERNAL_WINDOW_H__

#include <math.h>
#include "internal_math_types.h"

FUNC_ATTR_FFAST_MATH
static inline void dsp_weighted_sum_of_cos_1(MM_TYPE *out,
                                          uint32_t size,
                                          MM_TYPE c0,
                                          MM_TYPE c1)
{
#ifdef ENA_VEC_ISA
#else
    MM_TYPE k = (MM_TYPE)2.0 / ((MM_TYPE) size);
    MM_TYPE w;
    for(uint32_t i = 0; i < size; i++)
    {
         w = (MM_TYPE)TYPE_PI * i * k;
         w = c0 + c1 * COS_TYPE(w);
         out[i] = w;
    }
#endif
}

FUNC_ATTR_FFAST_MATH
static inline void dsp_weighted_sum_of_cos_2(MM_TYPE *out,
                                          uint32_t size,
                                          MM_TYPE c0,
                                          MM_TYPE c1,
                                          MM_TYPE c2)
{
#ifdef ENA_VEC_ISA
#else
    MM_TYPE k = (MM_TYPE)2.0 / ((MM_TYPE) size);
    MM_TYPE w;
    for(uint32_t i = 0; i < size; i++)
    {
         w = TYPE_PI * i * k;
         w = c0 + c1 * COS_TYPE(w) + c2 * COS_TYPE((MM_TYPE)2.0 * w);
         out[i] = w;
    }
#endif
}

static inline void dsp_weighted_sum_of_cos_3(MM_TYPE *out,
                                          uint32_t size,
                                          MM_TYPE c0,
                                          MM_TYPE c1,
                                          MM_TYPE c2,
                                          MM_TYPE c3)
{
#ifdef ENA_VEC_ISA
#else
    MM_TYPE k = (MM_TYPE)2.0 / ((MM_TYPE) size);
    MM_TYPE w;
    for(uint32_t i = 0; i < size; i++)
    {
         w = TYPE_PI * i * k;
         w = c0 + c1 * COS_TYPE(w) + c2 * COS_TYPE((MM_TYPE)2.0 * w) + c3 * COS_TYPE((MM_TYPE)3.0 * w);
         out[i] = w;
    }
#endif
}

static inline void dsp_weighted_sum_of_cos_4(MM_TYPE *out,
                                          uint32_t size,
                                          MM_TYPE c0,
                                          MM_TYPE c1,
                                          MM_TYPE c2,
                                          MM_TYPE c3,
                                          MM_TYPE c4)
{
#ifdef ENA_VEC_ISA
#else
    MM_TYPE k = (MM_TYPE)2.0 / ((MM_TYPE) size);
    MM_TYPE w;
    for(uint32_t i = 0; i < size; i++)
    {
         w = TYPE_PI * i * k;
         w = c0 + c1 * COS_TYPE(w) + c2 * COS_TYPE((MM_TYPE)2.0 * w) + c3 * COS_TYPE((MM_TYPE)3.0 * w) + c4 * COS_TYPE((MM_TYPE)4.0 * w);
         out[i] = w;
    }
#endif
}

FUNC_ATTR_FFAST_MATH
static inline void dsp_weighted_sum_of_cos_5(MM_TYPE *out,
                                          uint32_t size,
                                          MM_TYPE c0,
                                          MM_TYPE c1,
                                          MM_TYPE c2,
                                          MM_TYPE c3,
                                          MM_TYPE c4,
                                          MM_TYPE c5)
{
#ifdef ENA_VEC_ISA
#else
    MM_TYPE k = (MM_TYPE)2.0 / ((MM_TYPE) size);
    MM_TYPE w;
    for(uint32_t i = 0; i < size; i++)
    {
         w = TYPE_PI * i * k;
         w = c0 + c1 * COS_TYPE(w) + c2 * COS_TYPE((MM_TYPE)2.0 * w) + c3 * COS_TYPE((MM_TYPE)3.0 * w) + c4 * COS_TYPE((MM_TYPE)4.0 * w)
             + c5 * COS_TYPE((MM_TYPE)5.0 * w);
         out[i] = w;
    }
#endif
}

static inline void dsp_weighted_sum_of_cos_6(MM_TYPE *out,
                                          uint32_t size,
                                          MM_TYPE c0,
                                          MM_TYPE c1,
                                          MM_TYPE c2,
                                          MM_TYPE c3,
                                          MM_TYPE c4,
                                          MM_TYPE c5,
                                          MM_TYPE c6)
{
#ifdef ENA_VEC_ISA
#else
    MM_TYPE k = (MM_TYPE)2.0 / ((MM_TYPE) size);
    MM_TYPE w;
    for(uint32_t i = 0; i < size; i++)
    {
         w = TYPE_PI * i * k;
         w = c0 + c1 * COS_TYPE(w) + c2 * COS_TYPE((MM_TYPE)2.0 * w) + c3 * COS_TYPE((MM_TYPE)3.0 * w) + c4 * COS_TYPE((MM_TYPE)4.0 * w)
             + c5 * COS_TYPE((MM_TYPE)5.0 * w) + c6 * COS_TYPE((MM_TYPE)6.0 * w);
         out[i] = w;
    }
#endif
}

static inline void dsp_weighted_sum_of_cos_7(MM_TYPE *out,
                                          uint32_t size,
                                          MM_TYPE c0,
                                          MM_TYPE c1,
                                          MM_TYPE c2,
                                          MM_TYPE c3,
                                          MM_TYPE c4,
                                          MM_TYPE c5,
                                          MM_TYPE c6,
                                          MM_TYPE c7)
{
#ifdef ENA_VEC_ISA
#else
    MM_TYPE k = (MM_TYPE)2.0 / ((MM_TYPE) size);
    MM_TYPE w;
    for(uint32_t i = 0; i < size; i++)
    {
         w = TYPE_PI * i * k;
         w = c0 + c1 * COS_TYPE(w) + c2 * COS_TYPE((MM_TYPE)2.0 * w) + c3 * COS_TYPE((MM_TYPE)3.0 * w) + c4 * COS_TYPE((MM_TYPE)4.0 * w)
             + c5 * COS_TYPE((MM_TYPE)5.0 * w) + c6 * COS_TYPE((MM_TYPE)6.0 * w) + c7 * COS_TYPE((MM_TYPE)7.0 * w);
         out[i] = w;
    }
#endif
}

static inline void dsp_weighted_sum_of_cos_8(MM_TYPE *out,
                                          uint32_t size,
                                          MM_TYPE c0,
                                          MM_TYPE c1,
                                          MM_TYPE c2,
                                          MM_TYPE c3,
                                          MM_TYPE c4,
                                          MM_TYPE c5,
                                          MM_TYPE c6,
                                          MM_TYPE c7,
                                          MM_TYPE c8)
{
#ifdef ENA_VEC_ISA
#else
    MM_TYPE k = (MM_TYPE)2.0 / ((MM_TYPE) size);
    MM_TYPE w;
    for(uint32_t i = 0; i < size; i++)
    {
         w = TYPE_PI * i * k;
         w = c0 + c1 * COS_TYPE(w) + c2 * COS_TYPE((MM_TYPE)2.0 * w) + c3 * COS_TYPE((MM_TYPE)3.0 * w) + c4 * COS_TYPE((MM_TYPE)4.0 * w)
             + c5 * COS_TYPE((MM_TYPE)5.0 * w) + c6 * COS_TYPE((MM_TYPE)6.0 * w) + c7 * COS_TYPE((MM_TYPE)7.0 * w)
             + c8 * COS_TYPE((MM_TYPE)8.0 * w);
         out[i] = w;
    }
#endif
}

static inline void dsp_weighted_sum_of_cos_9(MM_TYPE *out,
                                          uint32_t size,
                                          MM_TYPE c0,
                                          MM_TYPE c1,
                                          MM_TYPE c2,
                                          MM_TYPE c3,
                                          MM_TYPE c4,
                                          MM_TYPE c5,
                                          MM_TYPE c6,
                                          MM_TYPE c7,
                                          MM_TYPE c8,
                                          MM_TYPE c9)
{
#ifdef ENA_VEC_ISA
#else
    MM_TYPE k = (MM_TYPE)2.0 / ((MM_TYPE) size);
    MM_TYPE w;
    for(uint32_t i = 0; i < size; i++)
    {
         w = TYPE_PI * i * k;
         w = c0 + c1 * COS_TYPE(w) + c2 * COS_TYPE((MM_TYPE)2.0 * w) + c3 * COS_TYPE((MM_TYPE)3.0 * w) + c4 * COS_TYPE((MM_TYPE)4.0 * w)
             + c5 * COS_TYPE((MM_TYPE)5.0 * w) + c6 * COS_TYPE((MM_TYPE)6.0 * w) + c7 * COS_TYPE((MM_TYPE)7.0 * w)
             + c8 * COS_TYPE((MM_TYPE)8.0 * w) + c9 * COS_TYPE((MM_TYPE)9.0 * w);
         out[i] = w;
    }
#endif
}
static inline void dsp_weighted_sum_of_cos_10(MM_TYPE *out,
                                          uint32_t size,
                                          MM_TYPE c0,
                                          MM_TYPE c1,
                                          MM_TYPE c2,
                                          MM_TYPE c3,
                                          MM_TYPE c4,
                                          MM_TYPE c5,
                                          MM_TYPE c6,
                                          MM_TYPE c7,
                                          MM_TYPE c8,
                                          MM_TYPE c9,
                                          MM_TYPE c10)
{
#ifdef ENA_VEC_ISA
#else
    MM_TYPE k = (MM_TYPE)2.0 / ((MM_TYPE) size);
    MM_TYPE w;
    for(uint32_t i = 0; i < size; i++)
    {
         w = TYPE_PI * i * k;
         w = c0 + c1 * COS_TYPE(w) + c2 * COS_TYPE((MM_TYPE)2.0 * w) + c3 * COS_TYPE((MM_TYPE)3.0 * w) + c4 * COS_TYPE((MM_TYPE)4.0 * w)
             + c5 * COS_TYPE((MM_TYPE)5.0 * w) + c6 * COS_TYPE((MM_TYPE)6.0 * w) + c7 * COS_TYPE((MM_TYPE)7.0 * w)
             + c8 * COS_TYPE((MM_TYPE)8.0 * w) + c9 * COS_TYPE((MM_TYPE)9.0 * w) + c10 * COS_TYPE((MM_TYPE)10.0 * w);
         out[i] = w;
    }
#endif
}
#endif // __INTERNAL_WINDOW_H__
