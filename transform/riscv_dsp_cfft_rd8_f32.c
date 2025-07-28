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
#include "internal_transform_math.h"
#define IFFT_DIRECT_CALC

/**
 * @ingroup transform
 */

/**
 * @addtogroup cfft_radix8
 * @{
 */

/**
 * @brief Function to implement the f32 Radix-2 Complex FFT
 *        Functions
 * @param[in]       *src     points to the input block data, src[2 * k] and
 *                           src[2 * k + 1] contain the real and imaginary
 *                           parts of the k-th data.
 * @param[in]       m        If not defined STATIC_VERSION, the data buffer
 *                           is of size 2 * n while n = 2 ** m.
 * @return valaue   0  success.
 *                  -1 invalid range.
 */


#define TRANSFORM_MATH_BIT_REV8                                             \
    uint16_t k = 1, r = n >> 3;                                             \
    uint16_t radix = 8, group = 8, parts = n >> 6;                          \
    uint16_t add_between_parts = n >> 6, add_between_groups = (n >> 3) + 1; \
    uint16_t add_in_part = n >> 3;                                          \
    uint16_t rg, rp;                                                        \
    for(int a = 1 ; a < group ; a++)                                        \
    {                                                                       \
        rg = r;                                                             \
        for(int b = 0 ; b < parts ; b++)                                    \
        {                                                                   \
            rp = rg;                                                        \
            for(int c = a ; c < radix ; c++)                                \
            {                                                               \
                TRANSFORM_MATH_SWAP(k, rp);                                 \
                k++;                                                        \
                rp += add_in_part;                                          \
            }                                                               \
            rg += add_between_parts;                                        \
            k += a;                                                         \
        }                                                                   \
        k++;                                                                \
        r += add_between_groups;                                            \
    }


static inline void riscv_dsp_bit_reversal8_f32(float32_t *src, uint32_t m)
{
    float32_t temp;
    uint32_t n = (1 << m);
    TRANSFORM_MATH_BIT_REV8;
}


uint16_t* get_reversal_table(uint32_t n)
{
    uint16_t* table;
    if(n == 16)
        table = bitRevIndexTable16;
    else if(n == 32)    
        table = bitRevIndexTable32;
    else if(n == 128)    
        table = bitRevIndexTable128;
    else if(n == 256)    
        table = bitRevIndexTable256;
    else
        table = bitRevIndexTable1024;
    return table;
}
/*
static inline void riscv_dsp_bit_reversal_table_f32(float32_t *src, uint32_t m)
{
    uint32_t n = (1 << m);
    uint16_t *table = get_reversal_table(n);
    float32_t temp;
    uint16_t *tp = table + 1, *te = table + table[0], r, k, r2, k2;
    while(tp != te)
    {
        r = tp[0];
        k = tp[1];
        TRANSFORM_MATH_SWAP(r, k);
        r2 = tp[2];
        k2 = tp[3];
        TRANSFORM_MATH_SWAP(r2, k2);
        tp += 4;
    }
}*/

static inline void riscv_dsp_bit_reversal_table_f32(float32_t *src, uint32_t m)
{
    uint32_t n = (1 << m);
    uint16_t *table = get_reversal_table(n);
    float32_t temp;
    uint16_t *tp = table + 1, *te = table + table[0], r, k;
    while(tp != te)
    {
        r = tp[0];
        k = tp[1];
        TRANSFORM_MATH_SWAP(r, k);
        tp += 2;
        r = tp[0];
        k = tp[1];
        TRANSFORM_MATH_SWAP(r, k);
        tp += 2;
    }
}
void radix8_butterfly_f32(float * pSrc, uint32_t m, uint32_t twidCoefModifier)
{
    /* Main loop */

#if FFT_LOGN > RES_LOGN
    float32_t p = TWO_PI / fftLen;
#endif /* FFT_LOGN > RES_LOGN */

    uint32_t fftLen = 1 << m;
    uint32_t ia1, ia2, ia3, ia4, ia5, ia6, ia7;
    uint32_t i1, i2, i3, i4, i5, i6, i7, i8;
    uint32_t id;
    uint32_t n1, n2, j;
    
    float32_t r1, r2, r3, r4, r5, r6, r7, r8;
    float32_t t1, t2;
    float32_t s1, s2, s3, s4, s5, s6, s7, s8;
    float32_t p1, p2, p3, p4;
    float32_t co2, co3, co4, co5, co6, co7, co8;
    float32_t si2, si3, si4, si5, si6, si7, si8;
    const float32_t C81 = 0.70710678118f;
    n2 = fftLen;
    while(1)
    {
        n1 = n2;
        n2 = n2 >> 3;
        i1 = 0;
        do
        {
            i2 = i1 + n2;
            i3 = i2 + n2;
            i4 = i3 + n2;
            i5 = i4 + n2;
            i6 = i5 + n2;
            i7 = i6 + n2;
            i8 = i7 + n2;
            r1 = pSrc[i1 << 1] + pSrc[i5 << 1];
            r5 = pSrc[i1 << 1] - pSrc[i5 << 1];
            r2 = pSrc[i2 << 1] + pSrc[i6 << 1];
            r6 = pSrc[i2 << 1] - pSrc[i6 << 1];
            r3 = pSrc[i3 << 1] + pSrc[i7 << 1];
            r7 = pSrc[i3 << 1] - pSrc[i7 << 1];
            r4 = pSrc[i4 << 1] + pSrc[i8 << 1];
            r8 = pSrc[i4 << 1] - pSrc[i8 << 1];
            t1 = r1 - r3;
            r1 = r1 + r3;
            r3 = r2 - r4;
            r2 = r2 + r4;
            pSrc[(i1 << 1)] = r1 + r2;
            pSrc[(i5 << 1)] = r1 - r2;
            r1 = pSrc[(i1 << 1) + 1] + pSrc[(i5 << 1) + 1];
            s5 = pSrc[(i1 << 1) + 1] - pSrc[(i5 << 1) + 1];
            r2 = pSrc[(i2 << 1) + 1] + pSrc[(i6 << 1) + 1];
            s6 = pSrc[(i2 << 1) + 1] - pSrc[(i6 << 1) + 1];
            s3 = pSrc[(i3 << 1) + 1] + pSrc[(i7 << 1) + 1];
            s7 = pSrc[(i3 << 1) + 1] - pSrc[(i7 << 1) + 1];
            r4 = pSrc[(i4 << 1) + 1] + pSrc[(i8 << 1) + 1];
            s8 = pSrc[(i4 << 1) + 1] - pSrc[(i8 << 1) + 1];
            t2 = r1 - s3;
            r1 = r1 + s3;
            s3 = r2 - r4;
            r2 = r2 + r4;
            pSrc[(i1 << 1) + 1] = r1 + r2;
            pSrc[(i5 << 1) + 1] = r1 - r2;
            r1 = (r6 - r8) * C81;
            r6 = (r6 + r8) * C81;
            r2 = (s6 - s8) * C81;
            s6 = (s6 + s8) * C81;
            pSrc[(i3 << 1)]     = t1 + s3;
            pSrc[(i7 << 1)]     = t1 - s3;
            pSrc[(i3 << 1) + 1] = t2 - r3;
            pSrc[(i7 << 1) + 1] = t2 + r3;
            t1 = r5 - r1;
            r5 = r5 + r1;
            r8 = r7 - r6;
            r7 = r7 + r6;
            t2 = s5 - r2;
            s5 = s5 + r2;
            s8 = s7 - s6;
            s7 = s7 + s6;
            pSrc[(i2 << 1)]     = r5 + s7;
            pSrc[(i8 << 1)]     = r5 - s7;
            pSrc[(i6 << 1)]     = t1 + s8;
            pSrc[(i4 << 1)]     = t1 - s8;
            pSrc[(i2 << 1) + 1] = s5 - r7;
            pSrc[(i8 << 1) + 1] = s5 + r7;
            pSrc[(i6 << 1) + 1] = t2 - r8;
            pSrc[(i4 << 1) + 1] = t2 + r8;
            i1 += n1;
        } while(i1 < fftLen);
        
        if(n2 < 8)
            break;
        
        ia1 = 0;
        j = 1;
        while(j < n2)
        {
            /*  index calculation for the coefficients */
            id  = ia1 + twidCoefModifier;
            ia1 = id;
            ia2 = ia1 + id;
            ia3 = ia2 + id;
            ia4 = ia3 + id;
            ia5 = ia4 + id;
            ia6 = ia5 + id;
            ia7 = ia6 + id;

            /*co2 = cos(ia1 * TWO_PI / (float)fftLen);
            si2 = sin(ia1 * TWO_PI / (float)fftLen);*/
           
#if FFT_LOGN > RES_LOGN
            GET_COS_SIN_VALUES(ia1, co2, si2, p, f32);
            GET_COS_SIN_VALUES(ia2, co3, si3, p, f32);
            GET_COS_SIN_VALUES(ia3, co4, si4, p, f32);
            GET_COS_SIN_VALUES(ia4, co5, si5, p, f32);
            GET_COS_SIN_VALUES(ia5, co6, si6, p, f32);
            GET_COS_SIN_VALUES(ia6, co7, si7, p, f32);
            GET_COS_SIN_VALUES(ia7, co8, si8, p, f32);
#else
#ifdef STATIC_VERSION
            GET_COS_SIN_VALUES(ia1, co2, si2, FFT_LOGN, f32);
            GET_COS_SIN_VALUES(ia2, co3, si3, FFT_LOGN, f32);
            GET_COS_SIN_VALUES(ia3, co4, si4, FFT_LOGN, f32);
            GET_COS_SIN_VALUES(ia4, co5, si5, FFT_LOGN, f32);
            GET_COS_SIN_VALUES(ia5, co6, si6, FFT_LOGN, f32);
            GET_COS_SIN_VALUES(ia6, co7, si7, FFT_LOGN, f32);
            GET_COS_SIN_VALUES(ia7, co8, si8, FFT_LOGN, f32);
#else
            GET_COS_SIN_VALUES(ia1, co2, si2, m, f32);
            GET_COS_SIN_VALUES(ia2, co3, si3, m, f32);
            GET_COS_SIN_VALUES(ia3, co4, si4, m, f32);
            GET_COS_SIN_VALUES(ia4, co5, si5, m, f32);
            GET_COS_SIN_VALUES(ia5, co6, si6, m, f32);
            GET_COS_SIN_VALUES(ia6, co7, si7, m, f32);
            GET_COS_SIN_VALUES(ia7, co8, si8, m, f32);
#endif
#endif

            i1 = j;
            /*  index calculation for the input */
            i2 = i1 + n2;
            i3 = i2 + n2;
            i4 = i3 + n2;
            i5 = i4 + n2;
            i6 = i5 + n2;
            i7 = i6 + n2;
            i8 = i7 + n2;
            r1 = pSrc[(i1 << 1)] + pSrc[(i5 << 1)];
            r5 = pSrc[(i1 << 1)] - pSrc[(i5 << 1)];
            r2 = pSrc[(i2 << 1)] + pSrc[(i6 << 1)];
            r6 = pSrc[(i2 << 1)] - pSrc[(i6 << 1)];
            r3 = pSrc[(i3 << 1)] + pSrc[(i7 << 1)];
            r7 = pSrc[(i3 << 1)] - pSrc[(i7 << 1)];
            r4 = pSrc[(i4 << 1)] + pSrc[(i8 << 1)];
            r8 = pSrc[(i4 << 1)] - pSrc[(i8 << 1)];
            t1 = r1 - r3;
            r1 = r1 + r3;
            r3 = r2 - r4;
            r2 = r2 + r4;
            pSrc[(i1 << 1)] = r1 + r2;
            r2 = r1 - r2;
            s1 = pSrc[(i1 << 1) + 1] + pSrc[(i5 << 1) + 1];
            s5 = pSrc[(i1 << 1) + 1] - pSrc[(i5 << 1) + 1];
            s2 = pSrc[(i2 << 1) + 1] + pSrc[(i6 << 1) + 1];
            s6 = pSrc[(i2 << 1) + 1] - pSrc[(i6 << 1) + 1];
            s3 = pSrc[(i3 << 1) + 1] + pSrc[(i7 << 1) + 1];
            s7 = pSrc[(i3 << 1) + 1] - pSrc[(i7 << 1) + 1];
            s4 = pSrc[(i4 << 1) + 1] + pSrc[(i8 << 1) + 1];
            s8 = pSrc[(i4 << 1) + 1] - pSrc[(i8 << 1) + 1];
            t2 = s1 - s3;
            s1 = s1 + s3;
            s3 = s2 - s4;
            s2 = s2 + s4;
            r1 = t1 + s3;
            t1 = t1 - s3;
            pSrc[(i1 << 1) + 1] = s1 + s2;
            s2 = s1 - s2;
            s1 = t2 - r3;
            t2 = t2 + r3;
            p1 = co5 * r2;
            p2 = si5 * s2;
            p3 = co5 * s2;
            p4 = si5 * r2;
            pSrc[(i5 << 1)]     = p1 + p2;
            pSrc[(i5 << 1) + 1] = p3 - p4;
            p1 = co3 * r1;
            p2 = si3 * s1;
            p3 = co3 * s1;
            p4 = si3 * r1;
            pSrc[(i3 << 1)]     = p1 + p2;
            pSrc[(i3 << 1) + 1] = p3 - p4;
            p1 = co7 * t1;
            p2 = si7 * t2;
            p3 = co7 * t2;
            p4 = si7 * t1;
            r1 = (r6 - r8) * C81;
            r6 = (r6 + r8) * C81;
            s1 = (s6 - s8) * C81;
            s6 = (s6 + s8) * C81;
            pSrc[(i7 << 1)]     = p1 + p2;
            pSrc[(i7 << 1) + 1] = p3 - p4;
            t1 = r5 - r1;
            r5 = r5 + r1;
            r8 = r7 - r6;
            r7 = r7 + r6;
            t2 = s5 - s1;
            s5 = s5 + s1;
            s8 = s7 - s6;
            s7 = s7 + s6;
            r1 = r5 + s7;
            r5 = r5 - s7;
            r6 = t1 + s8;
            t1 = t1 - s8;
            s1 = s5 - r7;
            s5 = s5 + r7;
            s6 = t2 - r8;
            t2 = t2 + r8;
            p1 = co2  * r1;
            p2 = si2  * s1;
            p3 = co2  * s1;
            p4 = si2  * r1;
            pSrc[(i2 << 1)]     = p1 + p2;
            pSrc[(i2 << 1) + 1] = p3 - p4;
            p1 = co8 * r5;
            p2 = si8 * s5;
            p3 = co8 * s5;
            p4 = si8 * r5;
            pSrc[(i8 << 1)]     = p1 + p2;
            pSrc[(i8 << 1) + 1] = p3 - p4;
            p1 = co6 * r6;
            p2 = si6 * s6;
            p3 = co6 * s6;
            p4 = si6 * r6;
            pSrc[(i6 << 1)]     = p1 + p2;
            pSrc[(i6 << 1) + 1] = p3 - p4;
            p1 = co4 * t1;
            p2 = si4 * t2;
            p3 = co4 * t2;
            p4 = si4 * t1;
            pSrc[(i4 << 1)]     = p1 + p2;
            pSrc[(i4 << 1) + 1] = p3 - p4;
            
            i1 += n1;

            while(i1 < fftLen)
            {
                /*  index calculation for the input */
                i2 = i1 + n2;
                i3 = i2 + n2;
                i4 = i3 + n2;
                i5 = i4 + n2;
                i6 = i5 + n2;
                i7 = i6 + n2;
                i8 = i7 + n2;
                r1 = pSrc[(i1 << 1)] + pSrc[(i5 << 1)];
                r5 = pSrc[(i1 << 1)] - pSrc[(i5 << 1)];
                r2 = pSrc[(i2 << 1)] + pSrc[(i6 << 1)];
                r6 = pSrc[(i2 << 1)] - pSrc[(i6 << 1)];
                r3 = pSrc[(i3 << 1)] + pSrc[(i7 << 1)];
                r7 = pSrc[(i3 << 1)] - pSrc[(i7 << 1)];
                r4 = pSrc[(i4 << 1)] + pSrc[(i8 << 1)];
                r8 = pSrc[(i4 << 1)] - pSrc[(i8 << 1)];
                t1 = r1 - r3;
                r1 = r1 + r3;
                r3 = r2 - r4;
                r2 = r2 + r4;
                pSrc[(i1 << 1)] = r1 + r2;
                r2 = r1 - r2;
                s1 = pSrc[(i1 << 1) + 1] + pSrc[(i5 << 1) + 1];
                s5 = pSrc[(i1 << 1) + 1] - pSrc[(i5 << 1) + 1];
                s2 = pSrc[(i2 << 1) + 1] + pSrc[(i6 << 1) + 1];
                s6 = pSrc[(i2 << 1) + 1] - pSrc[(i6 << 1) + 1];
                s3 = pSrc[(i3 << 1) + 1] + pSrc[(i7 << 1) + 1];
                s7 = pSrc[(i3 << 1) + 1] - pSrc[(i7 << 1) + 1];
                s4 = pSrc[(i4 << 1) + 1] + pSrc[(i8 << 1) + 1];
                s8 = pSrc[(i4 << 1) + 1] - pSrc[(i8 << 1) + 1];
                t2 = s1 - s3;
                s1 = s1 + s3;
                s3 = s2 - s4;
                s2 = s2 + s4;
                r1 = t1 + s3;
                t1 = t1 - s3;
                pSrc[(i1 << 1) + 1] = s1 + s2;
                s2 = s1 - s2;
                s1 = t2 - r3;
                t2 = t2 + r3;
                p1 = co5 * r2;
                p2 = si5 * s2;
                p3 = co5 * s2;
                p4 = si5 * r2;
                pSrc[(i5 << 1)]     = p1 + p2;
                pSrc[(i5 << 1) + 1] = p3 - p4;
                p1 = co3 * r1;
                p2 = si3 * s1;
                p3 = co3 * s1;
                p4 = si3 * r1;
                pSrc[(i3 << 1)]     = p1 + p2;
                pSrc[(i3 << 1) + 1] = p3 - p4;
                p1 = co7 * t1;
                p2 = si7 * t2;
                p3 = co7 * t2;
                p4 = si7 * t1;
                r1 = (r6 - r8) * C81;
                r6 = (r6 + r8) * C81;
                s1 = (s6 - s8) * C81;
                s6 = (s6 + s8) * C81;
                pSrc[(i7 << 1)]     = p1 + p2;
                pSrc[(i7 << 1) + 1] = p3 - p4;
                t1 = r5 - r1;
                r5 = r5 + r1;
                r8 = r7 - r6;
                r7 = r7 + r6;
                t2 = s5 - s1;
                s5 = s5 + s1;
                s8 = s7 - s6;
                s7 = s7 + s6;
                r1 = r5 + s7;
                r5 = r5 - s7;
                r6 = t1 + s8;
                t1 = t1 - s8;
                s1 = s5 - r7;
                s5 = s5 + r7;
                s6 = t2 - r8;
                t2 = t2 + r8;
                p1 = co2  * r1;
                p2 = si2  * s1;
                p3 = co2  * s1;
                p4 = si2  * r1;
                pSrc[(i2 << 1)]     = p1 + p2;
                pSrc[(i2 << 1) + 1] = p3 - p4;
                p1 = co8 * r5;
                p2 = si8 * s5;
                p3 = co8 * s5;
                p4 = si8 * r5;
                pSrc[(i8 << 1)]     = p1 + p2;
                pSrc[(i8 << 1) + 1] = p3 - p4;
                p1 = co6 * r6;
                p2 = si6 * s6;
                p3 = co6 * s6;
                p4 = si6 * r6;
                pSrc[(i6 << 1)]     = p1 + p2;
                pSrc[(i6 << 1) + 1] = p3 - p4;
                p1 = co4 * t1;
                p2 = si4 * t2;
                p3 = co4 * t2;
                p4 = si4 * t1;
                pSrc[(i4 << 1)]     = p1 + p2;
                pSrc[(i4 << 1) + 1] = p3 - p4;

                i1 += n1;
            }
            j++;
        }
        twidCoefModifier <<= 3;
    }    
}

void cfft_radix8by2_f32(float * p1, uint32_t m) 
{
    /* Main loop */

#if FFT_LOGN > RES_LOGN
#ifdef STATIC_VERSION
    float32_t p = TWO_PI / FFT_N;
#else
    unsigned int n = 1 << m;
    float32_t p = TWO_PI / n;
#endif /* STATIC_VERSION */
#endif /* FFT_LOGN > RES_LOGN */
    uint32_t fftLen = 1 << m;
    uint32_t L = fftLen;
    float32_t * pCol1, * pCol2, * pMid1, * pMid2;
    float32_t * p2 = p1 + L;
    uint16_t tw = 0;
    float32_t t1[4], t2[4], t3[4], t4[4], twR, twI;

    pCol1 = p1;
    pCol2 = p2;

    //    Define new length
    L >>= 1;
    //    Initialize mid pointers
    pMid1 = p1 + L;
    pMid2 = p2 + L;
    // do two dot Fourier transform
    float32_t *p2End = p2 + L;
    while(p2 != p2End)
    {
        t1[0] = p1[0];
        t1[1] = p1[1];
        t1[2] = p1[2];
        t1[3] = p1[3];

        t2[0] = p2[0];
        t2[1] = p2[1];
        t2[2] = p2[2];
        t2[3] = p2[3];

        t3[0] = pMid1[0];
        t3[1] = pMid1[1];
        t3[2] = pMid1[2];
        t3[3] = pMid1[3];

        t4[0] = pMid2[0];
        t4[1] = pMid2[1];
        t4[2] = pMid2[2];
        t4[3] = pMid2[3];

        p1[0] = t1[0] + t2[0];
        p1[1] = t1[1] + t2[1];
        p1[2] = t1[2] + t2[2];
        p1[3] = t1[3] + t2[3];    // col 1
        p1 += 4;

        t2[0] = t1[0] - t2[0];
        t2[1] = t1[1] - t2[1];
        t2[2] = t1[2] - t2[2];
        t2[3] = t1[3] - t2[3];    // for col 2

        pMid1[0] = t3[0] + t4[0];
        pMid1[1] = t3[1] + t4[1];
        pMid1[2] = t3[2] + t4[2];
        pMid1[3] = t3[3] + t4[3]; // col 1
        pMid1 += 4;

        t4[0] = t4[0] - t3[0];
        t4[1] = t4[1] - t3[1];
        t4[2] = t4[2] - t3[2];
        t4[3] = t4[3] - t3[3];    // for col 2
        
#if FFT_LOGN > RES_LOGN
        GET_COS_SIN_VALUES(tw, twR, twI, p, f32);
#else
#ifdef STATIC_VERSION
        GET_COS_SIN_VALUES(tw, twR, twI, FFT_LOGN, f32);
#else
        GET_COS_SIN_VALUES(tw, twR, twI, m, f32);
#endif
#endif
        tw++;

        // multiply by twiddle factors        
        // R  =  R  *  Tr - I * Ti
        *p2++ = t2[0] * twR + t2[1] * twI;
        // I  =  I  *  Tr + R * Ti
        *p2++ = t2[1] * twR - t2[0] * twI;
        
        // use vertical symmetry
        //  0.9988 - 0.0491i <==> -0.0491 - 0.9988i        
        *pMid2++ = t4[0] * twI - t4[1] * twR;
        *pMid2++ = t4[1] * twI + t4[0] * twR;

#if FFT_LOGN > RES_LOGN
        GET_COS_SIN_VALUES(tw, twR, twI, p, f32);
#else
#ifdef STATIC_VERSION
        GET_COS_SIN_VALUES(tw, twR, twI, FFT_LOGN, f32);
#else
        GET_COS_SIN_VALUES(tw, twR, twI, m, f32);
#endif
#endif
        tw++;
        *p2++ = t2[2] * twR + t2[3] * twI;
        *p2++ = t2[3] * twR - t2[2] * twI;        
        *pMid2++ = t4[2] * twI - t4[3] * twR;
        *pMid2++ = t4[3] * twI + t4[2] * twR;
    }

    // first col
    radix8_butterfly_f32( pCol1, m - 1, 1u); //2u
    // second col
    radix8_butterfly_f32( pCol2, m - 1, 1u); //2u
}

void cfft_radix8by4_f32(float * p1, uint32_t m) 
{    
    /* Main loop */

#if FFT_LOGN > RES_LOGN
#ifdef STATIC_VERSION
    float32_t p = TWO_PI / FFT_N;
#else
    unsigned int n = 1 << m;
    float32_t p = TWO_PI / n;
#endif /* STATIC_VERSION */
#endif /* FFT_LOGN > RES_LOGN */
    uint32_t fftLen = 1 << m;
    uint32_t    L  = fftLen >> 1;
    float32_t * pCol1, *pCol2, *pCol3, *pCol4, *pEnd1, *pEnd2, *pEnd3, *pEnd4;
    uint32_t tw2 = 1, tw3 = 2, tw4 = 3;
    float32_t * p2 = p1 + L;
    float32_t * p3 = p2 + L;
    float32_t * p4 = p3 + L;
    float32_t t2[4], t3[4], t4[4], twR, twI;
    float32_t p1ap3_0, p1sp3_0, p1ap3_1, p1sp3_1;
    float32_t m0, m1, m2, m3;
    uint32_t twMod2 = 1, twMod3 = 2, twMod4 = 3;

    pCol1 = p1;         // points to real values by default
    pCol2 = p2;
    pCol3 = p3;
    pCol4 = p4;
    pEnd1 = p2 - 1;     // points to imaginary values by default
    pEnd2 = p3 - 1;
    pEnd3 = p4 - 1;
    pEnd4 = pEnd3 + L;
    L >>= 1;
    // do four dot Fourier transform
    // TOP
    p1ap3_0 = p1[0] + p3[0];
    p1sp3_0 = p1[0] - p3[0];
    p1ap3_1 = p1[1] + p3[1];
    p1sp3_1 = p1[1] - p3[1];

    // col 2
    t2[0] = p1sp3_0 + p2[1] - p4[1];
    t2[1] = p1sp3_1 - p2[0] + p4[0];
    // col 3
    t3[0] = p1ap3_0 - p2[0] - p4[0];
    t3[1] = p1ap3_1 - p2[1] - p4[1];
    // col 4
    t4[0] = p1sp3_0 - p2[1] + p4[1];
    t4[1] = p1sp3_1 + p2[0] - p4[0];
    // col 1
    *p1++ = p1ap3_0 + p2[0] + p4[0];
    *p1++ = p1ap3_1 + p2[1] + p4[1];

    // Twiddle factors are ones
    *p2++ = t2[0];
    *p2++ = t2[1];
    *p3++ = t3[0];
    *p3++ = t3[1];
    *p4++ = t4[0];
    *p4++ = t4[1];

    for (uint32_t l = (L - 2) >> 1; l > 0; l-- )
    {
        // TOP
        p1ap3_0 = p1[0] + p3[0];
        p1sp3_0 = p1[0] - p3[0];
        p1ap3_1 = p1[1] + p3[1];
        p1sp3_1 = p1[1] - p3[1];
        // col 2
        t2[0] = p1sp3_0 + p2[1] - p4[1];
        t2[1] = p1sp3_1 - p2[0] + p4[0];
        // col 3
        t3[0] = p1ap3_0 - p2[0] - p4[0];
        t3[1] = p1ap3_1 - p2[1] - p4[1];
        // col 4
        t4[0] = p1sp3_0 - p2[1] + p4[1];
        t4[1] = p1sp3_1 + p2[0] - p4[0];
        // col 1 - top
        *p1++ = p1ap3_0 + p2[0] + p4[0];
        *p1++ = p1ap3_1 + p2[1] + p4[1];

        // BOTTOM
        p1ap3_1 = pEnd1[-1] + pEnd3[-1];
        p1sp3_1 = pEnd1[-1] - pEnd3[-1];
        p1ap3_0 = pEnd1[0] + pEnd3[0];
        p1sp3_0 = pEnd1[0] - pEnd3[0];
        // col 2
        t2[2] = pEnd2[0]  - pEnd4[0] + p1sp3_1;
        t2[3] = pEnd1[0] - pEnd3[0] - pEnd2[-1] + pEnd4[-1];
        // col 3
        t3[2] = p1ap3_1 - pEnd2[-1] - pEnd4[-1];
        t3[3] = p1ap3_0 - pEnd2[0]  - pEnd4[0];
        // col 4
        t4[2] = pEnd2[0]  - pEnd4[0]  - p1sp3_1;
        t4[3] = pEnd4[-1] - pEnd2[-1] - p1sp3_0;
        // col 1 - Bottom
        *pEnd1-- = p1ap3_0 + pEnd2[0] + pEnd4[0];
        *pEnd1-- = p1ap3_1 + pEnd2[-1] + pEnd4[-1];

        // COL 2
        // read twiddle factors
        
#if FFT_LOGN > RES_LOGN
        GET_COS_SIN_VALUES(tw2, twR, twI, p, f32);
#else
#ifdef STATIC_VERSION
        GET_COS_SIN_VALUES(tw2, twR, twI, FFT_LOGN, f32);
#else
        GET_COS_SIN_VALUES(tw2, twR, twI, m, f32);
#endif /* STATIC_VERSION */
#endif /* FFT_LOGN > RES_LOGN */
        tw2 += twMod2;

        // multiply by twiddle factors
        //  let    Z1 = a + i(b),   Z2 = c + i(d)
        //   =>  Z1 * Z2  =  (a*c - b*d) + i(b*c + a*d)
        
        // Top
        m0 = t2[0] * twR;
        m1 = t2[1] * twI;
        m2 = t2[1] * twR;
        m3 = t2[0] * twI;
        
        *p2++ = m0 + m1;
        *p2++ = m2 - m3;
        // use vertical symmetry col 2
        // 0.9997 - 0.0245i  <==>  0.0245 - 0.9997i
        // Bottom
        m0 = t2[3] * twI;
        m1 = t2[2] * twR;
        m2 = t2[2] * twI;
        m3 = t2[3] * twR;
        
        *pEnd2-- = m0 - m1;
        *pEnd2-- = m2 + m3;

#if FFT_LOGN > RES_LOGN
        GET_COS_SIN_VALUES(tw3, twR, twI, p, f32);
#else
#ifdef STATIC_VERSION
        GET_COS_SIN_VALUES(tw3, twR, twI, FFT_LOGN, f32);
#else
        GET_COS_SIN_VALUES(tw3, twR, twI, m, f32);
#endif /* STATIC_VERSION */
#endif /* FFT_LOGN > RES_LOGN */
        tw3 += twMod3;

        // Top
        m0 = t3[0] * twR;
        m1 = t3[1] * twI;
        m2 = t3[1] * twR;
        m3 = t3[0] * twI;
        
        *p3++ = m0 + m1;
        *p3++ = m2 - m3;
        // use vertical symmetry col 3
        // 0.9988 - 0.0491i  <==>  -0.9988 - 0.0491i
        // Bottom
        m0 = -t3[3] * twR;
        m1 = t3[2] * twI;
        m2 = t3[2] * twR;
        m3 = t3[3] * twI;
        
        *pEnd3-- = m0 - m1;
        *pEnd3-- = m3 - m2;
                
#if FFT_LOGN > RES_LOGN
        GET_COS_SIN_VALUES(tw4, twR, twI, p, f32);
#else
#ifdef STATIC_VERSION
        GET_COS_SIN_VALUES(tw4, twR, twI, FFT_LOGN, f32);
#else
        GET_COS_SIN_VALUES(tw4, twR, twI, m, f32);
#endif /* STATIC_VERSION */
#endif /* FFT_LOGN > RES_LOGN */
        tw4 += twMod4;

        // Top
        m0 = t4[0] * twR;
        m1 = t4[1] * twI;
        m2 = t4[1] * twR;
        m3 = t4[0] * twI;
        
        *p4++ = m0 + m1;
        *p4++ = m2 - m3;
        // use vertical symmetry col 4
        // 0.9973 - 0.0736i  <==>  -0.0736 + 0.9973i
        // Bottom
        m0 = t4[3] * twI;
        m1 = t4[2] * twR;
        m2 = t4[2] * twI;
        m3 = t4[3] * twR;
        
        *pEnd4-- = m0 - m1;
        *pEnd4-- = m2 + m3;
    }

    //MIDDLE
    // Twiddle factors are 
    //  1.0000  0.7071-0.7071i  -1.0000i  -0.7071-0.7071i
    p1ap3_0 = p1[0] + p3[0];
    p1sp3_0 = p1[0] - p3[0];
    p1ap3_1 = p1[1] + p3[1];
    p1sp3_1 = p1[1] - p3[1];

    // col 2
    t2[0] = p1sp3_0 + p2[1] - p4[1];
    t2[1] = p1sp3_1 - p2[0] + p4[0];
    // col 3
    t3[0] = p1ap3_0 - p2[0] - p4[0];
    t3[1] = p1ap3_1 - p2[1] - p4[1];
    // col 4
    t4[0] = p1sp3_0 - p2[1] + p4[1];
    t4[1] = p1sp3_1 + p2[0] - p4[0];
    // col 1 - Top
    *p1++ = p1ap3_0 + p2[0] + p4[0];
    *p1++ = p1ap3_1 + p2[1] + p4[1];

#if FFT_LOGN > RES_LOGN
    GET_COS_SIN_VALUES(tw2, twR, twI, p, f32);
#else
#ifdef STATIC_VERSION
    GET_COS_SIN_VALUES(tw2, twR, twI, FFT_LOGN, f32);
#else
    GET_COS_SIN_VALUES(tw2, twR, twI, m, f32);
#endif /* STATIC_VERSION */
#endif /* FFT_LOGN > RES_LOGN */

    m0 = t2[0] * twR;
    m1 = t2[1] * twI;
    m2 = t2[1] * twR;
    m3 = t2[0] * twI;

    *p2++ = m0 + m1;
    *p2++ = m2 - m3;
    
#if FFT_LOGN > RES_LOGN
    GET_COS_SIN_VALUES(tw3, twR, twI, p, f32);
#else
#ifdef STATIC_VERSION
    GET_COS_SIN_VALUES(tw3, twR, twI, FFT_LOGN, f32);
#else
    GET_COS_SIN_VALUES(tw3, twR, twI, m, f32);
#endif /* STATIC_VERSION */
#endif /* FFT_LOGN > RES_LOGN */

    m0 = t3[0] * twR;
    m1 = t3[1] * twI;
    m2 = t3[1] * twR;
    m3 = t3[0] * twI;

    *p3++ = m0 + m1;
    *p3++ = m2 - m3;

#if FFT_LOGN > RES_LOGN
    GET_COS_SIN_VALUES(tw4, twR, twI, p, f32);
#else
#ifdef STATIC_VERSION
    GET_COS_SIN_VALUES(tw4, twR, twI, FFT_LOGN, f32);
#else
    GET_COS_SIN_VALUES(tw4, twR, twI, m, f32);
#endif /* STATIC_VERSION */
#endif /* FFT_LOGN > RES_LOGN */

    m0 = t4[0] * twR;
    m1 = t4[1] * twI;
    m2 = t4[1] * twR;
    m3 = t4[0] * twI;

    *p4++ = m0 + m1;
    *p4++ = m2 - m3;

    // first col
    radix8_butterfly_f32( pCol1, m - 2, 1u); //4u
    // second col
    radix8_butterfly_f32( pCol2, m - 2, 1u);
    // third col
    radix8_butterfly_f32( pCol3, m - 2, 1u);
    // fourth col
    radix8_butterfly_f32( pCol4, m - 2, 1u);
}


/* function description */
int32_t riscv_dsp_cfft_rd8_f32(float32_t *pSrc, uint32_t m)
{
#ifndef ENA_EMPTY_FUNC

#ifdef CHECK_RANGE
#ifdef STATIC_VERSION
    /* Quick return for FFT_LOGN < 4, FFT_LOGN > 14, and FFT_LOGN is odd */
    if (FFT_LOGN < 4 || FFT_LOGN > 14 || ((FFT_LOGN & 0x1) != 0))
    {
        /* only support FFT_N = 16, 64, 256, 1024, 4096, and 16384
         ** that is, FFT_LOGN = 4, 6, 8, 10, 12, and 14. */
#else
    /* Quick return for m < 4, m > 14, and m is odd */
    if (m < 4 || m > 14 || ((m & 0x1) != 0))
    {
        /* only support n = 16, 64, 256, 1024, 4096, and 16384 that is,
         ** m = 4, 6, 8, 10, 12, and 14. */
#endif /* STATIC_VERSION */
        return -1;
    }
#endif /* CHECK_RANGE */

    if(m % 3 == 0)
    {
        radix8_butterfly_f32(pSrc, m, 1);
        riscv_dsp_bit_reversal8_f32(pSrc, m);
    }
    else if(m % 3 == 1)
    {
        cfft_radix8by2_f32(pSrc, m);
        riscv_dsp_bit_reversal_table_f32(pSrc, m);
    }
    else if (m % 3 == 2)
    {
        cfft_radix8by4_f32(pSrc, m);
        riscv_dsp_bit_reversal_table_f32(pSrc, m);
    }    


    return 0;
#else
    riscv_dsp_empty_ret_func2(pSrc, m);
#endif
}
/* cifft */
#ifdef IFFT_DIRECT_CALC
void radix8_ibutterfly_f32(float * pSrc, uint32_t m, uint32_t twidCoefModifier, uint32_t scale_m)
{
    /* Main loop */

#if FFT_LOGN > RES_LOGN
    float32_t p = TWO_PI / fftLen;
#endif /* FFT_LOGN > RES_LOGN */

    uint32_t fftLen = 1 << m;
    uint32_t ia1, ia2, ia3, ia4, ia5, ia6, ia7;
    uint32_t i1, i2, i3, i4, i5, i6, i7, i8;
    uint32_t id;
    uint32_t n1, n2, j;
    uint32_t scale_fac = 1 << scale_m;    
    float invL_ref = 1.0f/(float32_t)scale_fac;
    float32_t invL;
    
    float32_t r1, r2, r3, r4, r5, r6, r7, r8;
    float32_t t1, t2;
    float32_t s1, s2, s3, s4, s5, s6, s7, s8;
    float32_t p1, p2, p3, p4;
    float32_t co2, co3, co4, co5, co6, co7, co8;
    float32_t si2, si3, si4, si5, si6, si7, si8;
#ifdef  RD8_IFFT_MODY
    float32_t tmp_r_1, tmp_r_2, tmp_i_1, tmp_i_2;
    float32_t tmp_rmr, tmp_rmi, tmp_imr, tmp_imi;
#endif
    const float32_t C81 = 0.70710678118f;
    n2 = fftLen;
    while(1)
    {
        n1 = n2;
        n2 = n2 >> 3;
        i1 = 0;
        if( n2 < 8)
        {
            invL = invL_ref;
        }
        else
        {
            invL = 1.0f;
        }
        do
        {
            i2 = i1 + n2;
            i3 = i2 + n2;
            i4 = i3 + n2;
            i5 = i4 + n2;
            i6 = i5 + n2;
            i7 = i6 + n2;
            i8 = i7 + n2;
            r1 = (pSrc[i1 << 1] + pSrc[i5 << 1]);    // 1r + 5r
            r5 = (pSrc[i1 << 1] - pSrc[i5 << 1]);    // 1r - 5r 
            r2 = (pSrc[i2 << 1] + pSrc[i6 << 1]);    // 2r + 6r
            r6 = (pSrc[i2 << 1] - pSrc[i6 << 1]);    // 2r - 6r
            r3 = (pSrc[i3 << 1] + pSrc[i7 << 1]);    // 3r + 7r
            r7 = (pSrc[i3 << 1] - pSrc[i7 << 1]);    // 3r - 7r
            r4 = (pSrc[i4 << 1] + pSrc[i8 << 1]);    // 4r + 8r
            r8 = (pSrc[i4 << 1] - pSrc[i8 << 1]);    // 4r - 8r
            t1 = r1 - r3;                          // 1r + 5r - 3r - 7r
            r1 = r1 + r3;                          // 1r + 5r + 3r + 7r
            r3 = r2 - r4;                          // 2r + 6r - 4r - 8r
            r2 = r2 + r4;                          // 2r + 6r + 4r + 8r
            pSrc[(i1 << 1)] = (r1 + r2)*invL;             // 1r + 2r + 3r + 4r + 5r + 6r + 7r + 8r
            pSrc[(i5 << 1)] = (r1 - r2)*invL;             // 1r + 5r + 3r + 7r - 2r - 6r - 4r - 8r
            r1 = (pSrc[(i1 << 1) + 1] + pSrc[(i5 << 1) + 1]);  // 1i + 5i
            s5 = (pSrc[(i1 << 1) + 1] - pSrc[(i5 << 1) + 1]);  // 1i - 5i
            r2 = (pSrc[(i2 << 1) + 1] + pSrc[(i6 << 1) + 1]);  // 2i + 6i
            s6 = (pSrc[(i2 << 1) + 1] - pSrc[(i6 << 1) + 1]);  // 2i - 6i
            s3 = (pSrc[(i3 << 1) + 1] + pSrc[(i7 << 1) + 1]);  // 3i + 7i
            s7 = (pSrc[(i3 << 1) + 1] - pSrc[(i7 << 1) + 1]);  // 3i - 7i
            r4 = (pSrc[(i4 << 1) + 1] + pSrc[(i8 << 1) + 1]);  // 4i + 8i
            s8 = (pSrc[(i4 << 1) + 1] - pSrc[(i8 << 1) + 1]);  // 4i - 8i
            t2 = r1 - s3;                          // (1i + 5i) - (3i + 7i) 
            r1 = r1 + s3;                          // (1i + 5i) + (3i + 7i)
            s3 = r2 - r4;                          // (2i + 6i) - (4i + 8i)
            r2 = r2 + r4;                          // (2i + 6i) + (4i + 8i)
            pSrc[(i1 << 1) + 1] = (r1 + r2)*invL;         // (1r + 5i) + (3i + 7i) + 2i + 6i + 4i + 8i 
            pSrc[(i5 << 1) + 1] = (r1 - r2)*invL;         // (1i + 5i) + (3i + 7i) - (2i + 6i) - (4i + 8i)
            r1 = (r6 - r8) * C81;                  // (2rt - 6rt) - (4rt - 8rt)
            r6 = (r6 + r8) * C81;                  // (2rt - 6rt) + (4rt - 8rt)
            r2 = (s6 - s8) * C81;                  // (2it - 6it) - (4it - 8it)
            s6 = (s6 + s8) * C81;                  // (2it - 6it) + (4it + 8it)

            pSrc[(i3 << 1)]     = (t1 - s3)*invL;         // 1r + 5r - 3r - 7r - ((2i + 6i) - (4i + 8i))
            pSrc[(i7 << 1)]     = (t1 + s3)*invL;         // 1r + 5r - 3r - 7r + ((2i + 6i) - (4i + 8i))
            pSrc[(i3 << 1) + 1] = (t2 + r3)*invL;         // (1i + 5i) - (3i + 7i)  + (2r + 6r - 4r - 8r) 
            pSrc[(i7 << 1) + 1] = (t2 - r3)*invL;         // (1i + 5i) - (3i + 7i)  - (2r + 6r - 4r - 8r)
            t1 = r5 - r1;                          // (1r - 5r) - ((2rt - 6rt) - (4rt - 8rt))
            r5 = r5 + r1;                          // (1r - 5r) + ((2rt - 6rt) - (4rt - 8rt))
            r8 = r7 - r6;                          // (3r - 7r) - ((2rt - 6rt) + (4rt - 8rt))
            r7 = r7 + r6;                          // (3r - 7r) + ((2rt - 6rt) + (4rt - 8rt))
            t2 = s5 - r2;                          // (1i - 5i) - ((2it - 6it) - (4it - 8it))
            s5 = s5 + r2;                          // (1i - 5i) + ((2it - 6it) - (4it - 8it))
            s8 = s7 - s6;                          // (3i - 7i) - ((2it - 6it) + (4it + 8it))
            s7 = s7 + s6;                          // (3i - 7i) + ((2it - 6it) + (4it + 8it))
            pSrc[(i2 << 1)]     = (r5 - s7)*invL;         // (1r - 5r) + ((2rt - 6rt) - (4rt - 8rt)) - ((3i - 7i) + ((2it - 6it) + (4it + 8it)))
            pSrc[(i8 << 1)]     = (r5 + s7)*invL;         // (1r - 5r) + ((2rt - 6rt) - (4rt - 8rt)) + ((3i - 7i) + ((2it - 6it) + (4it + 8it)))
            pSrc[(i6 << 1)]     = (t1 - s8)*invL;         // (1r - 5r) - ((2rt - 6rt) - (4rt - 8rt)) - ((3i - 7i) - ((2it - 6it) + (4it + 8it)))
            pSrc[(i4 << 1)]     = (t1 + s8)*invL;         // (1r - 5r) - ((2rt - 6rt) - (4rt - 8rt)) + ((3i - 7i) - ((2it - 6it) + (4it + 8it)))
            pSrc[(i2 << 1) + 1] = (s5 + r7)*invL;         // (1i - 5i) + ((2it - 6it) - (4it - 8it)) + ((3r - 7r) + ((2rt - 6rt) + (4rt - 8rt)))
            pSrc[(i8 << 1) + 1] = (s5 - r7)*invL;         // (1i - 5i) + ((2it - 6it) - (4it - 8it)) - ((3r - 7r) + ((2rt - 6rt) + (4rt - 8rt)))
            pSrc[(i6 << 1) + 1] = (t2 + r8)*invL;         // (1i - 5i) - ((2it - 6it) - (4it - 8it)) + ((3r - 7r) - ((2rt - 6rt) + (4rt - 8rt)))
            pSrc[(i4 << 1) + 1] = (t2 - r8)*invL;         // (1i - 5i) - ((2it - 6it) - (4it - 8it)) - ((3r - 7r) - ((2rt - 6rt) + (4rt - 8rt)))
            i1 += n1;
        } while(i1 < fftLen);
        
        if(n2 < 8)
        {
            break;
        }
        ia1 = 0;
        j = 1;
      
        while(j < n2)
        {
            /*  index calculation for the coefficients */
            id  = ia1 + twidCoefModifier;
            ia1 = id;
            ia2 = ia1 + id;
            ia3 = ia2 + id;
            ia4 = ia3 + id;
            ia5 = ia4 + id;
            ia6 = ia5 + id;
            ia7 = ia6 + id;

            /*co2 = cos(ia1 * TWO_PI / (float)fftLen);
            si2 = sin(ia1 * TWO_PI / (float)fftLen);*/
           
#if FFT_LOGN > RES_LOGN
            GET_COS_SIN_VALUES(ia1, co2, si2, p, f32);
            GET_COS_SIN_VALUES(ia2, co3, si3, p, f32);
            GET_COS_SIN_VALUES(ia3, co4, si4, p, f32);
            GET_COS_SIN_VALUES(ia4, co5, si5, p, f32);
            GET_COS_SIN_VALUES(ia5, co6, si6, p, f32);
            GET_COS_SIN_VALUES(ia6, co7, si7, p, f32);
            GET_COS_SIN_VALUES(ia7, co8, si8, p, f32);
#else
#ifdef STATIC_VERSION
            GET_COS_SIN_VALUES(ia1, co2, si2, FFT_LOGN, f32);
            GET_COS_SIN_VALUES(ia2, co3, si3, FFT_LOGN, f32);
            GET_COS_SIN_VALUES(ia3, co4, si4, FFT_LOGN, f32);
            GET_COS_SIN_VALUES(ia4, co5, si5, FFT_LOGN, f32);
            GET_COS_SIN_VALUES(ia5, co6, si6, FFT_LOGN, f32);
            GET_COS_SIN_VALUES(ia6, co7, si7, FFT_LOGN, f32);
            GET_COS_SIN_VALUES(ia7, co8, si8, FFT_LOGN, f32);
#else
            GET_COS_SIN_VALUES(ia1, co2, si2, m, f32);
            GET_COS_SIN_VALUES(ia2, co3, si3, m, f32);
            GET_COS_SIN_VALUES(ia3, co4, si4, m, f32);
            GET_COS_SIN_VALUES(ia4, co5, si5, m, f32);
            GET_COS_SIN_VALUES(ia5, co6, si6, m, f32);
            GET_COS_SIN_VALUES(ia6, co7, si7, m, f32);
            GET_COS_SIN_VALUES(ia7, co8, si8, m, f32);
#endif
#endif
            i1 = j;
            /*  index calculation for the input */
#ifndef RD8_IFFT_MODY
            i2 = i1 + n2;
            i3 = i2 + n2;
            i4 = i3 + n2;
            i5 = i4 + n2;
            i6 = i5 + n2;
            i7 = i6 + n2;
            i8 = i7 + n2;
            r1 = pSrc[(i1 << 1)] + pSrc[(i5 << 1)];   // 1r + 5r 
            r5 = pSrc[(i1 << 1)] - pSrc[(i5 << 1)];   // 1r - 5r
            r2 = pSrc[(i2 << 1)] + pSrc[(i6 << 1)];   // 2r + 6r
            r6 = pSrc[(i2 << 1)] - pSrc[(i6 << 1)];   // 2r - 6r
            r3 = pSrc[(i3 << 1)] + pSrc[(i7 << 1)];   // 3r + 7r
            r7 = pSrc[(i3 << 1)] - pSrc[(i7 << 1)];   // 3r - 7r
            r4 = pSrc[(i4 << 1)] + pSrc[(i8 << 1)];   // 4r + 8r
            r8 = pSrc[(i4 << 1)] - pSrc[(i8 << 1)];   // 4r - 8r
            t1 = r1 - r3;                             // 1r + 5r - 3r - 7r
            r1 = r1 + r3;                             // 1r + 5r + 3r + 7r
            r3 = r2 - r4;                             // 2r + 6r - 4r - 8r
            r2 = r2 + r4;                             // 2r + 6r + 4r + 8r
            pSrc[(i1 << 1)] = (r1 + r2);                // 1r + 2r + 3r + 4r + 5r + 6r + 7r + 8r
            r2 = r1 - r2;                             // 1r + 5r + 3r + 7r - 2r - 6r - 4r - 8r
            s1 = pSrc[(i1 << 1) + 1] + pSrc[(i5 << 1) + 1]; // 1i + 5i
            s5 = pSrc[(i1 << 1) + 1] - pSrc[(i5 << 1) + 1]; // 1i - 5i
            s2 = pSrc[(i2 << 1) + 1] + pSrc[(i6 << 1) + 1]; // 2i + 6i
            s6 = pSrc[(i2 << 1) + 1] - pSrc[(i6 << 1) + 1]; // 2i - 6i
            s3 = pSrc[(i3 << 1) + 1] + pSrc[(i7 << 1) + 1]; // 3i + 7i
            s7 = pSrc[(i3 << 1) + 1] - pSrc[(i7 << 1) + 1]; // 3i - 7i
            s4 = pSrc[(i4 << 1) + 1] + pSrc[(i8 << 1) + 1]; // 4i + 8i
            s8 = pSrc[(i4 << 1) + 1] - pSrc[(i8 << 1) + 1]; // 4i - 8i
            t2 = s1 - s3;                                   // (1i + 5i) - (3i + 7i) 
            s1 = s1 + s3;                                   // (1i + 5i) + (3i + 7i)
            s3 = s2 - s4;                                   // (2i + 6i) - (4i + 8i) 
            s2 = s2 + s4;                                   // (2i + 6i) + (4i + 8i)
            r1 = t1 - s3;                                   // (1r + 5r - 3r - 7r) - ((2i + 6i) - (4i + 8i))
            t1 = t1 + s3;                                   // (1r + 5r - 3r - 7r) + ((2i + 6i) - (4i + 8i))
            pSrc[(i1 << 1) + 1] = (s1 + s2);                  // ((1i + 5i) + (3i + 7i)) + ((2i + 6i) + (4i + 8i))
            s2 = s1 - s2;                                   // ((1i + 5i) + (3i + 7i)) - ((2i + 6i) + (4i + 8i))
            s1 = t2 + r3;                                   // ((1i + 5i) - (3i + 7i)) + ( 2r + 6r - 4r - 8r)
            t2 = t2 - r3;                                   // ((1i + 5i) - (3i + 7i)) - ( 2r + 6r - 4r - 8r)
            // r2 = 1r + 5r + 3r + 7r - 2r - 6r - 4r - 8r = pSrc[(i5 << 1)]
            // s2 = ((1i + 5i) + (3i + 7i)) - ((2i + 6i) + (4i + 8i)) = pSrc[(i5 << 1) + 1]
            p1 = co5 * r2;                                  // tw5r*(1r + 5r + 3r + 7r - 2r - 6r - 4r - 8r)
            p2 = si5 * s2;                                  // tw5i*( ((1i + 5i) + (3i + 7i)) - ((2i + 6i) + (4i + 8i)))
            p3 = co5 * s2;                                  // tw5r*(((1i + 5i) + (3i + 7i)) - ((2i + 6i) + (4i + 8i)))
            p4 = si5 * r2;                                  // tw5i*(1r + 5r + 3r + 7r - 2r - 6r - 4r - 8r)
            pSrc[(i5 << 1)]     = (p1 - p2);                  
            pSrc[(i5 << 1) + 1] = (p3 + p4);
            // r1 = (1r + 5r - 3r - 7r) - ((2i + 6i) - (4i + 8i)) = pSrc[(i3 << 1)]
            // s1 = ((1i + 5i) - (3i + 7i)) + ( 2r + 6r - 4r - 8r) = pSrc[(i3 << 1) + 1]
            p1 = co3 * r1;                                  // tw3r*((1r + 5r - 3r - 7r) - ((2i + 6i) - (4i + 8i)))
            p2 = si3 * s1;                                  // tw3i*(((1i + 5i) - (3i + 7i)) + ( 2r + 6r - 4r - 8r))
            p3 = co3 * s1;                                  // tw3r*(((1i + 5i) - (3i + 7i)) + ( 2r + 6r - 4r - 8r))
            p4 = si3 * r1;                                  // tw3i*((1r + 5r - 3r - 7r) - ((2i + 6i) - (4i + 8i)))
            pSrc[(i3 << 1)]     = (p1 - p2);
            pSrc[(i3 << 1) + 1] = (p3 + p4);
            // t1 = (1r + 5r - 3r - 7r) + ((2i + 6i) - (4i + 8i)) = pSrc[(i7 << 1)]
            // t2 = (1i + 5i) - (3i + 7i)  - (2r + 6r - 4r - 8r) = pSrc[(i7 << 1) + 1]
            p1 = co7 * t1;                                  // tw7r*((1r + 5r - 3r - 7r) + ((2i + 6i) - (4i + 8i)))
            p2 = si7 * t2;                                  // tw7i*(((1i + 5i) - (3i + 7i)) - ( 2r + 6r - 4r - 8r))
            p3 = co7 * t2;                                  // tw7r*(((1i + 5i) - (3i + 7i)) - ( 2r + 6r - 4r - 8r))
            p4 = si7 * t1;                                  // tw7i*((1r + 5r - 3r - 7r) + ((2i + 6i) - (4i + 8i)))
            r1 = (r6 - r8) * C81;                           // (2rt - 6rt) - (4rt - 8rt)
            r6 = (r6 + r8) * C81;                           // (2rt - 6rt) + (4rt - 8rt)
            s1 = (s6 - s8) * C81;                           // (2it - 6it) - (4it - 8it)
            s6 = (s6 + s8) * C81;                           // (2it - 6it) + (4it + 8it)
            pSrc[(i7 << 1)]     = (p1 - p2);
            pSrc[(i7 << 1) + 1] = (p3 + p4);
            t1 = r5 - r1;                                   // (1r - 5r) - ((2rt - 6rt) - (4rt - 8rt))
            r5 = r5 + r1;                                   // (1r - 5r) + ((2rt - 6rt) - (4rt - 8rt))
            r8 = r7 - r6;                                   // (3r - 7r) - ((2rt - 6rt) + (4rt - 8rt))
            r7 = r7 + r6;                                   // (3r - 7r) + ((2rt - 6rt) + (4rt - 8rt))
            t2 = s5 - s1;                                   // (1i - 5i) - ((2it - 6it) - (4it - 8it))
            s5 = s5 + s1;                                   // (1i - 5i) + ((2it - 6it) - (4it - 8it))
            s8 = s7 - s6;                                   // (3i - 7i) - ((2it - 6it) + (4it + 8it))
            s7 = s7 + s6;                                   // (3i - 7i) + ((2it - 6it) + (4it + 8it))

            // pSrc[(i2 << 1)] = (1r - 5r) + ((2rt - 6rt) - (4rt - 8rt)) - ((3i - 7i) + ((2it - 6it) + (4it + 8it)))
            // pSrc[(i8 << 1)] = (1r - 5r) + ((2rt - 6rt) - (4rt - 8rt)) + ((3i - 7i) + ((2it - 6it) + (4it + 8it)))
            r1 = r5 - s7;
            r5 = r5 + s7;
            // pSrc[(i6 << 1)] = (1r - 5r) - ((2rt - 6rt) - (4rt - 8rt)) - ((3i - 7i) - ((2it - 6it) + (4it + 8it))) 
            // pSrc[(i4 << 1)] = (1r - 5r) - ((2rt - 6rt) - (4rt - 8rt)) + ((3i - 7i) - ((2it - 6it) + (4it + 8it)))
            r6 = t1 - s8;
            t1 = t1 + s8;
            // pSrc[(i2 << 1) + 1] = (1i - 5i) + ((2it - 6it) - (4it - 8it)) + ((3r - 7r) + ((2rt - 6rt) + (4rt - 8rt)))
            // pSrc[(i8 << 1) + 1] = (1i - 5i) + ((2it - 6it) - (4it - 8it)) - ((3r - 7r) + ((2rt - 6rt) + (4rt - 8rt)))
            s1 = s5 + r7;
            s5 = s5 - r7;
            // pSrc[(i6 << 1) + 1] = (1i - 5i) - ((2it - 6it) - (4it - 8it)) + ((3r - 7r) - ((2rt - 6rt) + (4rt - 8rt)))
            // pSrc[(i4 << 1) + 1] = (1i - 5i) - ((2it - 6it) - (4it - 8it)) - ((3r - 7r) - ((2rt - 6rt) + (4rt - 8rt)))
            s6 = t2 + r8;
            t2 = t2 - r8;
            p1 = co2  * r1;                                 // tw2r*pSrc[(i2 << 1)] 
            p2 = si2  * s1;                                 // tw2i*pSrc[(i2 << 1) + 1]
            p3 = co2  * s1;                                 // tw2i*pSrc[(i2 << 1)] 
            p4 = si2  * r1;                                 // tw2r*pSrc[(i2 << 1) + 1]
            pSrc[(i2 << 1)]     = (p1 - p2);
            pSrc[(i2 << 1) + 1] = (p3 + p4);
            p1 = co8 * r5;                                  // tw8r*pSrc[(i8 << 1)]
            p2 = si8 * s5;                                  // tw8i*pSrc[(i8 << 1) + 1]
            p3 = co8 * s5;                                  // tw8i*pSrc[(i8 << 1)]
            p4 = si8 * r5;                                  // tw8r*pSrc[(i8 << 1) + 1]
            pSrc[(i8 << 1)]     = (p1 - p2);
            pSrc[(i8 << 1) + 1] = (p3 + p4);
            p1 = co6 * r6;                                  // tw6r*pSrc[(i6 << 1)]
            p2 = si6 * s6;                                  // tw6i*pSrc[(i6 << 1) + 1]
            p3 = co6 * s6;                                  // tw6i*pSrc[(i6 << 1)]
            p4 = si6 * r6;                                  // tw6r*pSrc[(i6 << 1) + 1]
            pSrc[(i6 << 1)]     = (p1 - p2);
            pSrc[(i6 << 1) + 1] = (p3 + p4);
            p1 = co4 * t1;                                  // tw4r*pSrc[(i4 << 1)]
            p2 = si4 * t2;                                  // tw4i*pSrc[(i4 << 1) + 1]
            p3 = co4 * t2;                                  // tw4i*pSrc[(i4 << 1)]
            p4 = si4 * t1;                                  // tw4r*pSrc[(i4 << 1) + 1]
            pSrc[(i4 << 1)]     = (p1 - p2);
            pSrc[(i4 << 1) + 1] = (p3 + p4);
#else
            i2 = i1 + n2;
            i3 = i2 + n2;
            i4 = i3 + n2;
            i5 = i4 + n2;
            i6 = i5 + n2;
            i7 = i6 + n2;
            i8 = i7 + n2;
            r1 = pSrc[i1 << 1] + pSrc[i5 << 1];    // 1r + 5r
            r5 = pSrc[i1 << 1] - pSrc[i5 << 1];    // 1r - 5r
            r2 = pSrc[i2 << 1] + pSrc[i6 << 1];    // 2r + 6r
            r6 = pSrc[i2 << 1] - pSrc[i6 << 1];    // 2r - 6r
            r3 = pSrc[i3 << 1] + pSrc[i7 << 1];    // 3r + 7r
            r7 = pSrc[i3 << 1] - pSrc[i7 << 1];    // 3r - 7r
            r4 = pSrc[i4 << 1] + pSrc[i8 << 1];    // 4r + 8r
            r8 = pSrc[i4 << 1] - pSrc[i8 << 1];    // 4r - 8r
            t1 = r1 - r3;                          // 1r + 5r - 3r - 7r
            r1 = r1 + r3;                          // 1r + 5r + 3r + 7r
            r3 = r2 - r4;                          // 2r + 6r - 4r - 8r
            r2 = r2 + r4;                          // 2r + 6r + 4r + 8r

            pSrc[(i1 << 1)] = (r1 + r2);           //  1r + 2r + 3r + 4r + 5r + 6r + 7r + 8r
            tmp_r_1 = (r1 - r2);                   //  1r + 5r + 3r + 7r - 2r - 6r - 4r - 8r = pSrc[(i5 << 1)]
            r1 = pSrc[(i1 << 1) + 1] + pSrc[(i5 << 1) + 1];  // 1i + 5i
            s5 = pSrc[(i1 << 1) + 1] - pSrc[(i5 << 1) + 1];  // 1i - 5i
            r2 = pSrc[(i2 << 1) + 1] + pSrc[(i6 << 1) + 1];  // 2i + 6i
            s6 = pSrc[(i2 << 1) + 1] - pSrc[(i6 << 1) + 1];  // 2i - 6i
            s3 = pSrc[(i3 << 1) + 1] + pSrc[(i7 << 1) + 1];  // 3i + 7i
            s7 = pSrc[(i3 << 1) + 1] - pSrc[(i7 << 1) + 1];  // 3i - 7i
            r4 = pSrc[(i4 << 1) + 1] + pSrc[(i8 << 1) + 1];  // 4i + 8i
            s8 = pSrc[(i4 << 1) + 1] - pSrc[(i8 << 1) + 1];  // 4i - 8i            
            t2 = r1 - s3;                          // (1i + 5i) - (3i + 7i)
            r1 = r1 + s3;                          // (1i + 5i) + (3i + 7i)
            s3 = r2 - r4;                          // (2i + 6i) - (4i + 8i)
            r2 = r2 + r4;                          // (2i + 6i) + (4i + 8i)
            pSrc[(i1 << 1) + 1] = (r1 + r2);       // (1r + 5i) + (3i + 7i) + 2i + 6i + 4i + 8i
            tmp_i_1 = (r1 - r2);                   // (1i + 5i) + (3i + 7i) - (2i + 6i) - (4i + 8i) = pSrc[(i5 << 1) + 1]
            tmp_rmr = tmp_r_1* co5;
            tmp_rmi = tmp_r_1* si5;
            tmp_imr = tmp_i_1* co5;
            tmp_imi = tmp_i_1* si5;
            pSrc[(i5 << 1)] = (tmp_rmr - tmp_imi);
            pSrc[(i5 << 1) + 1] = (tmp_rmi + tmp_imr);

            r1 = (r6 - r8) * C81;                  // (2rt - 6rt) - (4rt - 8rt)
            r6 = (r6 + r8) * C81;                  // (2rt - 6rt) + (4rt - 8rt)
            r2 = (s6 - s8) * C81;                  // (2it - 6it) - (4it - 8it)
            s6 = (s6 + s8) * C81;                  // (2it - 6it) + (4it + 8it)

            tmp_r_1 = (t1 - s3);                   // 1r + 5r - 3r - 7r - ((2i + 6i) - (4i + 8i)) = pSrc[(i3 << 1)]
            tmp_i_1 = (t2 + r3);                   // (1i + 5i) - (3i + 7i)  + (2r + 6r - 4r - 8r) = pSrc[(i3 << 1) + 1]
            tmp_r_2 = (t1 + s3);                   // 1r + 5r - 3r - 7r + ((2i + 6i) - (4i + 8i)) = pSrc[(i7 << 1)]
            tmp_i_2 = (t2 - r3);                   // (1i + 5i) - (3i + 7i)  - (2r + 6r - 4r - 8r) = pSrc[(i7 << 1) + 1]
   
            tmp_rmr = tmp_r_1* co3;
            tmp_rmi = tmp_r_1* si3;
            tmp_imr = tmp_i_1* co3;
            tmp_imi = tmp_i_1* si3;
            pSrc[(i3 << 1)] = (tmp_rmr - tmp_imi);
            pSrc[(i3 << 1) + 1] = (tmp_rmi + tmp_imr);

            tmp_rmr = tmp_r_2* co7;
            tmp_rmi = tmp_r_2* si7;
            tmp_imr = tmp_i_2* co7;
            tmp_imi = tmp_i_2* si7;            
            pSrc[(i7 << 1)] = (tmp_rmr - tmp_imi);
            pSrc[(i7 << 1) + 1] = (tmp_rmi + tmp_imr);

            t1 = r5 - r1;                          // (1r - 5r) - ((2rt - 6rt) - (4rt - 8rt))
            r5 = r5 + r1;                          // (1r - 5r) + ((2rt - 6rt) - (4rt - 8rt))
            r8 = r7 - r6;                          // (3r - 7r) - ((2rt - 6rt) + (4rt - 8rt))
            r7 = r7 + r6;                          // (3r - 7r) + ((2rt - 6rt) + (4rt - 8rt))
            t2 = s5 - r2;                          // (1i - 5i) - ((2it - 6it) - (4it - 8it))
            s5 = s5 + r2;                          // (1i - 5i) + ((2it - 6it) - (4it - 8it))
            s8 = s7 - s6;                          // (3i - 7i) - ((2it - 6it) + (4it + 8it))
            s7 = s7 + s6;                          // (3i - 7i) + ((2it - 6it) + (4it + 8it))

            tmp_r_1 = (r5 - s7);                   // (1r - 5r) + ((2rt - 6rt) - (4rt - 8rt)) - ((3i - 7i) + ((2it - 6it) + (4it + 8it)))
            tmp_i_1 = (s5 + r7);                   // (1i - 5i) + ((2it - 6it) - (4it - 8it)) + ((3r - 7r) + ((2rt - 6rt) + (4rt - 8rt)))
            tmp_r_2 = (r5 + s7);                   // (1r - 5r) + ((2rt - 6rt) - (4rt - 8rt)) + ((3i - 7i) + ((2it - 6it) + (4it + 8it)))
            tmp_i_2 = (s5 - r7);                   // (1i - 5i) + ((2it - 6it) - (4it - 8it)) - ((3r - 7r) + ((2rt - 6rt) + (4rt - 8rt)))

            tmp_rmr = tmp_r_1* co2;
            tmp_rmi = tmp_r_1* si2;
            tmp_imr = tmp_i_1* co2;
            tmp_imi = tmp_i_1* si2;           
            pSrc[(i2 << 1)] = (tmp_rmr - tmp_imi);
            pSrc[(i2 << 1) + 1] = (tmp_rmi + tmp_imr);

            tmp_rmr = tmp_r_2* co8;
            tmp_rmi = tmp_r_2* si8;
            tmp_imr = tmp_i_2* co8;
            tmp_imi = tmp_i_2* si8;
            pSrc[(i8 << 1)] = (tmp_rmr - tmp_imi);
            pSrc[(i8 << 1) + 1] = (tmp_rmi + tmp_imr);

            tmp_r_1 = (t1 - s8);                   // (1r - 5r) - ((2rt - 6rt) - (4rt - 8rt)) - ((3i - 7i) - ((2it - 6it) + (4it + 8it)))
            tmp_i_1 = (t2 + r8);                   // (1i - 5i) - ((2it - 6it) - (4it - 8it)) + ((3r - 7r) - ((2rt - 6rt) + (4rt - 8rt)))
            tmp_r_2 = (t1 + s8);                   // (1r - 5r) - ((2rt - 6rt) - (4rt - 8rt)) + ((3i - 7i) - ((2it - 6it) + (4it + 8it)))
            tmp_i_2 = (t2 - r8);                   // (1i - 5i) - ((2it - 6it) - (4it - 8it)) - ((3r - 7r) - ((2rt - 6rt) + (4rt - 8rt)))
            
            tmp_rmr = tmp_r_1* co6;
            tmp_rmi = tmp_r_1* si6;
            tmp_imr = tmp_i_1* co6;
            tmp_imi = tmp_i_1* si6;
            pSrc[(i6 << 1)] = (tmp_rmr - tmp_imi);
            pSrc[(i6 << 1) + 1] = (tmp_rmi + tmp_imr);

            tmp_rmr = tmp_r_2* co4;
            tmp_rmi = tmp_r_2* si4;
            tmp_imr = tmp_i_2* co4;
            tmp_imi = tmp_i_2* si4;
            pSrc[(i4 << 1)] = (tmp_rmr - tmp_imi);
            pSrc[(i4 << 1) + 1] = (tmp_rmi + tmp_imr);
#endif            

            i1 += n1;
            
            while(i1 < fftLen)
            {
                /*  index calculation for the input */
                i2 = i1 + n2;
                i3 = i2 + n2;
                i4 = i3 + n2;
                i5 = i4 + n2;
                i6 = i5 + n2;
                i7 = i6 + n2;
                i8 = i7 + n2;
                r1 = pSrc[(i1 << 1)] + pSrc[(i5 << 1)];
                r5 = pSrc[(i1 << 1)] - pSrc[(i5 << 1)];
                r2 = pSrc[(i2 << 1)] + pSrc[(i6 << 1)];
                r6 = pSrc[(i2 << 1)] - pSrc[(i6 << 1)];
                r3 = pSrc[(i3 << 1)] + pSrc[(i7 << 1)];
                r7 = pSrc[(i3 << 1)] - pSrc[(i7 << 1)];
                r4 = pSrc[(i4 << 1)] + pSrc[(i8 << 1)];
                r8 = pSrc[(i4 << 1)] - pSrc[(i8 << 1)];
                t1 = r1 - r3;
                r1 = r1 + r3;
                r3 = r2 - r4;
                r2 = r2 + r4;
                pSrc[(i1 << 1)] = (r1 + r2);
                r2 = r1 - r2;
                s1 = pSrc[(i1 << 1) + 1] + pSrc[(i5 << 1) + 1];
                s5 = pSrc[(i1 << 1) + 1] - pSrc[(i5 << 1) + 1];
                s2 = pSrc[(i2 << 1) + 1] + pSrc[(i6 << 1) + 1];
                s6 = pSrc[(i2 << 1) + 1] - pSrc[(i6 << 1) + 1];
                s3 = pSrc[(i3 << 1) + 1] + pSrc[(i7 << 1) + 1];
                s7 = pSrc[(i3 << 1) + 1] - pSrc[(i7 << 1) + 1];
                s4 = pSrc[(i4 << 1) + 1] + pSrc[(i8 << 1) + 1];
                s8 = pSrc[(i4 << 1) + 1] - pSrc[(i8 << 1) + 1];
                t2 = s1 - s3;
                s1 = s1 + s3;
                s3 = s2 - s4;
                s2 = s2 + s4;
                r1 = t1 - s3;
                t1 = t1 + s3;
                pSrc[(i1 << 1) + 1] = (s1 + s2);
                s2 = s1 - s2;
                s1 = t2 + r3;
                t2 = t2 - r3;
                p1 = co5 * r2;
                p2 = si5 * s2;
                p3 = co5 * s2;
                p4 = si5 * r2;
                pSrc[(i5 << 1)]     = (p1 - p2);
                pSrc[(i5 << 1) + 1] = (p3 + p4);
                p1 = co3 * r1;
                p2 = si3 * s1;
                p3 = co3 * s1;
                p4 = si3 * r1;
                pSrc[(i3 << 1)]     = (p1 - p2);
                pSrc[(i3 << 1) + 1] = (p3 + p4);
                p1 = co7 * t1;
                p2 = si7 * t2;
                p3 = co7 * t2;
                p4 = si7 * t1;
                r1 = (r6 - r8) * C81;
                r6 = (r6 + r8) * C81;
                s1 = (s6 - s8) * C81;
                s6 = (s6 + s8) * C81;
                pSrc[(i7 << 1)]     = (p1 - p2);
                pSrc[(i7 << 1) + 1] = (p3 + p4);
                t1 = r5 - r1;
                r5 = r5 + r1;
                r8 = r7 - r6;
                r7 = r7 + r6;
                t2 = s5 - s1;
                s5 = s5 + s1;
                s8 = s7 - s6;
                s7 = s7 + s6;
                r1 = r5 - s7;
                r5 = r5 + s7;
                r6 = t1 - s8;
                t1 = t1 + s8;
                s1 = s5 + r7;
                s5 = s5 - r7;
                s6 = t2 + r8;
                t2 = t2 - r8;
                p1 = co2  * r1;
                p2 = si2  * s1;
                p3 = co2  * s1;
                p4 = si2  * r1;
                pSrc[(i2 << 1)]     = (p1 - p2);
                pSrc[(i2 << 1) + 1] = (p3 + p4);
                p1 = co8 * r5;
                p2 = si8 * s5;
                p3 = co8 * s5;
                p4 = si8 * r5;
                pSrc[(i8 << 1)]     = (p1 - p2);
                pSrc[(i8 << 1) + 1] = (p3 + p4);
                p1 = co6 * r6;
                p2 = si6 * s6;
                p3 = co6 * s6;
                p4 = si6 * r6;
                pSrc[(i6 << 1)]     = (p1 - p2);
                pSrc[(i6 << 1) + 1] = (p3 + p4);
                p1 = co4 * t1;
                p2 = si4 * t2;
                p3 = co4 * t2;
                p4 = si4 * t1;
                pSrc[(i4 << 1)]     = (p1 - p2);
                pSrc[(i4 << 1) + 1] = (p3 + p4);

                i1 += n1;
            }
            j++;
        }
        twidCoefModifier <<= 3;
    }    
}
#endif // IFFT_DIRECT_CALC

#ifdef IFFT_DIRECT_CALC
void cifft_radix8by2_f32(float * p1, uint32_t m) 
{
    /* Main loop */
#if FFT_LOGN > RES_LOGN
#ifdef STATIC_VERSION
    float32_t p = TWO_PI / FFT_N;
#else
    unsigned int n = 1 << m;
    float32_t p = TWO_PI / n;
#endif /* STATIC_VERSION */
#endif /* FFT_LOGN > RES_LOGN */
    uint32_t fftLen = 1 << m;
    uint32_t L = fftLen;
    //float32_t * pCol1, * pCol2, * pMid1, * pMid2;
    float32_t * pCol1, * pCol2;
    float32_t * p2 = p1 + L;
    uint32_t tw = 0;
    float32_t t1[4], t2[4], twR, twI;

    pCol1 = p1;
    pCol2 = p2;
#if 0

    float32_t *p2End = p2 + L;
    while(p2 != p2End)
    {
        t1[0] = p1[0];
        t1[1] = p1[1];
        t1[2] = p1[2];
        t1[3] = p1[3];

        t2[0] = p2[0];
        t2[1] = p2[1];
        t2[2] = p2[2];
        t2[3] = p2[3];

        p1[0] = (t1[0] + t2[0]);
        p1[1] = (t1[1] + t2[1]);
        p1[2] = (t1[2] + t2[2]);
        p1[3] = (t1[3] + t2[3]);    // col 1
        p1 += 4;

        t2[0] = t1[0] - t2[0];
        t2[1] = t1[1] - t2[1];
        t2[2] = t1[2] - t2[2];
        t2[3] = t1[3] - t2[3];    // for col 2

#if FFT_LOGN > RES_LOGN
        GET_COS_SIN_VALUES(tw, twR, twI, p, f32);
#else
#ifdef STATIC_VERSION
        GET_COS_SIN_VALUES(tw, twR, twI, FFT_LOGN, f32);
#else
        GET_COS_SIN_VALUES(tw, twR, twI, m, f32);
#endif
#endif
        tw++;
        // multiply by twiddle factors
        // R  =  R  *  Tr - I * Ti
        *p2++ = (t2[0] * twR - t2[1] * twI);
        // I  =  I  *  Tr + R * Ti
        *p2++ = (t2[1] * twR + t2[0] * twI);

#if FFT_LOGN > RES_LOGN
        GET_COS_SIN_VALUES(tw, twR, twI, p, f32);
#else
#ifdef STATIC_VERSION
        GET_COS_SIN_VALUES(tw, twR, twI, FFT_LOGN, f32);
#else
        GET_COS_SIN_VALUES(tw, twR, twI, m, f32);
#endif
#endif
        tw++;

        *p2++ = (t2[2] * twR - t2[3] * twI);
        *p2++ = (t2[3] * twR + t2[2] * twI);
    }
#else
// this way gets better perofrmance
    float32_t * pMid1, * pMid2;
    float32_t t3[4], t4[4];
    //    Define new length
    L >>= 1;
    //    Initialize mid pointers
    pMid1 = p1 + L;
    pMid2 = p2 + L;
    // do two dot Fourier transform
    float32_t *p2End = p2 + L;
    while(p2 != p2End)
    {
        t1[0] = p1[0];
        t1[1] = p1[1];
        t1[2] = p1[2];
        t1[3] = p1[3];

        t2[0] = p2[0];
        t2[1] = p2[1];
        t2[2] = p2[2];
        t2[3] = p2[3];

        t3[0] = pMid1[0];
        t3[1] = pMid1[1];
        t3[2] = pMid1[2];
        t3[3] = pMid1[3];

        t4[0] = pMid2[0];
        t4[1] = pMid2[1];
        t4[2] = pMid2[2];
        t4[3] = pMid2[3];

        p1[0] = (t1[0] + t2[0]);
        p1[1] = (t1[1] + t2[1]);
        p1[2] = (t1[2] + t2[2]);
        p1[3] = (t1[3] + t2[3]);    // col 1
        p1 += 4;

        t2[0] = t1[0] - t2[0];
        t2[1] = t1[1] - t2[1];
        t2[2] = t1[2] - t2[2];
        t2[3] = t1[3] - t2[3];    // for col 2

        pMid1[0] = (t3[0] + t4[0]);
        pMid1[1] = (t3[1] + t4[1]);
        pMid1[2] = (t3[2] + t4[2]);
        pMid1[3] = (t3[3] + t4[3]); // col 1
        pMid1 += 4;

        t4[0] = t4[0] - t3[0];
        t4[1] = t4[1] - t3[1];
        t4[2] = t4[2] - t3[2];
        t4[3] = t4[3] - t3[3];    // for col 2
        
#if FFT_LOGN > RES_LOGN
        GET_COS_SIN_VALUES(tw, twR, twI, p, f32);
#else
#ifdef STATIC_VERSION
        GET_COS_SIN_VALUES(tw, twR, twI, FFT_LOGN, f32);
#else
        GET_COS_SIN_VALUES(tw, twR, twI, m, f32);
#endif
#endif
        tw++;
        // multiply by twiddle factors
        // R  =  R  *  Tr - I * Ti
        *p2++ = (t2[0] * twR - t2[1] * twI);
        // I  =  I  *  Tr + R * Ti
        *p2++ = (t2[1] * twR + t2[0] * twI);

        // use vertical symmetry
        //  cos(t) + isin(t) -> cos(pi/2 + t) + isin(pi/2 + t )
        *pMid2++ = (t4[0] * twI + t4[1] * twR);
        *pMid2++ = (t4[1] * twI - t4[0] * twR);

#if FFT_LOGN > RES_LOGN
        GET_COS_SIN_VALUES(tw, twR, twI, p, f32);
#else
#ifdef STATIC_VERSION
        GET_COS_SIN_VALUES(tw, twR, twI, FFT_LOGN, f32);
#else
        GET_COS_SIN_VALUES(tw, twR, twI, m, f32);
#endif
#endif
        tw++;
        *p2++ = (t2[2] * twR - t2[3] * twI);
        *p2++ = (t2[3] * twR + t2[2] * twI);

        // cos(t) + isin(t) -> cos(pi/2 + t) + isin(pi/2 + t )
        *pMid2++ = (t4[2] * twI + t4[3] * twR);
        *pMid2++ = (t4[3] * twI - t4[2] * twR);
    }
#endif
    

    // first col
    radix8_ibutterfly_f32( pCol1, (m - 1), 1u, m); //2u
    // second col
    radix8_ibutterfly_f32( pCol2, (m - 1), 1u, m); //2u    
}
#endif // IFFT_DIRECT_CALC


#ifdef IFFT_DIRECT_CALC
void cifft_radix8by4_f32(float * p1, uint32_t m)
{
    /* Main loop */
#if FFT_LOGN > RES_LOGN
#ifdef STATIC_VERSION
    float32_t p = TWO_PI / FFT_N;
#else
    unsigned int n = 1 << m;
    float32_t p = TWO_PI / n;
#endif /* STATIC_VERSION */
#endif /* FFT_LOGN > RES_LOGN */
    uint32_t fftLen = 1 << m;
    uint32_t    L  = fftLen >> 1;
    float32_t * pCol1, *pCol2, *pCol3, *pCol4;
    uint32_t tw2 = 1, tw3 = 2, tw4 = 3;
    float32_t * p2 = p1 + L;
    float32_t * p3 = p2 + L;
    float32_t * p4 = p3 + L;
    float32_t t2[4], t3[4], t4[4], twR, twI;
    float32_t p1ap3_0, p1sp3_0, p1ap3_1, p1sp3_1;
    float32_t m0, m1, m2, m3;
    uint32_t twMod2 = 1, twMod3 = 2, twMod4 = 3;

    pCol1 = p1;         // points to real values by default
    pCol2 = p2;
    pCol3 = p3;
    pCol4 = p4;

    L >>= 1;
    // do four dot Fourier transform
    // TOP
    p1ap3_0 = p1[0] + p3[0];          // 1r + 3r
    p1sp3_0 = p1[0] - p3[0];          // 1r - 3r
    p1ap3_1 = p1[1] + p3[1];          // 1i + 3i
    p1sp3_1 = p1[1] - p3[1];          // 1i - 3i

    // col 2
    t2[0] = p1sp3_0 - p2[1] + p4[1];  // 1r - 3r - 2i + 4i
    t2[1] = p1sp3_1 + p2[0] - p4[0];  // 1i - 3i + 2r - 4r
    // col 3
    t3[0] = p1ap3_0 - p2[0] - p4[0];  // 1r + 3r - 2r - 4r
    t3[1] = p1ap3_1 - p2[1] - p4[1];  // 1i + 3i - 2i - 4i
    // col 4
    t4[0] = p1sp3_0 + p2[1] - p4[1];  // 1r - 3r + 2i - 4i
    t4[1] = p1sp3_1 - p2[0] + p4[0];  // 1i - 3i - 2r + 4r
    // col 1
    *p1++ = (p1ap3_0 + p2[0] + p4[0]);
    *p1++ = (p1ap3_1 + p2[1] + p4[1]);

    // Twiddle factors are ones
    *p2++ = t2[0];
    *p2++ = t2[1];
    *p3++ = t3[0];
    *p3++ = t3[1];
    *p4++ = t4[0];
    *p4++ = t4[1];

    for (uint32_t l = (L - 2) ; l > 0; l-- )
    {
        p1ap3_0 = p1[0] + p3[0];
        p1sp3_0 = p1[0] - p3[0];
        p1ap3_1 = p1[1] + p3[1];
        p1sp3_1 = p1[1] - p3[1];
    
        // col 2
        t2[0] = p1sp3_0 - p2[1] + p4[1];
        t2[1] = p1sp3_1 + p2[0] - p4[0];
        // col 3
        t3[0] = p1ap3_0 - p2[0] - p4[0];
        t3[1] = p1ap3_1 - p2[1] - p4[1];
        // col 4
        t4[0] = p1sp3_0 + p2[1] - p4[1];
        t4[1] = p1sp3_1 - p2[0] + p4[0];
        // col 1 - Top
        *p1++ = (p1ap3_0 + p2[0] + p4[0]);
        *p1++ = (p1ap3_1 + p2[1] + p4[1]);
    #if FFT_LOGN > RES_LOGN
        GET_COS_SIN_VALUES(tw2, twR, twI, p, f32);
    #else
    #ifdef STATIC_VERSION
        GET_COS_SIN_VALUES(tw2, twR, twI, FFT_LOGN, f32);
    #else
        GET_COS_SIN_VALUES(tw2, twR, twI, m, f32);
    #endif /* STATIC_VERSION */
    #endif /* FFT_LOGN > RES_LOGN */
        tw2 += twMod2;
    
        m0 = t2[0] * twR;
        m1 = t2[1] * twI;
        m2 = t2[1] * twR;
        m3 = t2[0] * twI;
    
        *p2++ = (m0 - m1);
        *p2++ = (m2 + m3);
    
#if FFT_LOGN > RES_LOGN
        GET_COS_SIN_VALUES(tw3, twR, twI, p, f32);
#else
#ifdef STATIC_VERSION
        GET_COS_SIN_VALUES(tw3, twR, twI, FFT_LOGN, f32);
#else
        GET_COS_SIN_VALUES(tw3, twR, twI, m, f32);
#endif /* STATIC_VERSION */
#endif /* FFT_LOGN > RES_LOGN */
        tw3 += twMod3;

        m0 = t3[0] * twR;
        m1 = t3[1] * twI;
        m2 = t3[1] * twR;
        m3 = t3[0] * twI;
    
        *p3++ = (m0 - m1);
        *p3++ = (m2 + m3);
    
#if FFT_LOGN > RES_LOGN
        GET_COS_SIN_VALUES(tw4, twR, twI, p, f32);
#else
#ifdef STATIC_VERSION
        GET_COS_SIN_VALUES(tw4, twR, twI, FFT_LOGN, f32);
#else
        GET_COS_SIN_VALUES(tw4, twR, twI, m, f32);
#endif /* STATIC_VERSION */
#endif /* FFT_LOGN > RES_LOGN */
        tw4 += twMod4;

        m0 = t4[0] * twR;
        m1 = t4[1] * twI;
        m2 = t4[1] * twR;
        m3 = t4[0] * twI;
    
        *p4++ = (m0 - m1);
        *p4++ = (m2 + m3);
    }           
    //MIDDLE
    // Twiddle factors are 
    //  1.0000  0.7071-0.7071i  -1.0000i  -0.7071-0.7071i
    p1ap3_0 = p1[0] + p3[0];
    p1sp3_0 = p1[0] - p3[0];
    p1ap3_1 = p1[1] + p3[1];
    p1sp3_1 = p1[1] - p3[1];

    // col 2
    t2[0] = p1sp3_0 - p2[1] + p4[1];
    t2[1] = p1sp3_1 + p2[0] - p4[0];
    // col 3
    t3[0] = p1ap3_0 - p2[0] - p4[0];
    t3[1] = p1ap3_1 - p2[1] - p4[1];
    // col 4
    t4[0] = p1sp3_0 + p2[1] - p4[1];
    t4[1] = p1sp3_1 - p2[0] + p4[0];
    // col 1 - Top
    *p1++ = (p1ap3_0 + p2[0] + p4[0]);
    *p1++ = (p1ap3_1 + p2[1] + p4[1]);

#if FFT_LOGN > RES_LOGN
    GET_COS_SIN_VALUES(tw2, twR, twI, p, f32);
#else
#ifdef STATIC_VERSION
    GET_COS_SIN_VALUES(tw2, twR, twI, FFT_LOGN, f32);
#else
    GET_COS_SIN_VALUES(tw2, twR, twI, m, f32);
#endif /* STATIC_VERSION */
#endif /* FFT_LOGN > RES_LOGN */

    twI = -twI;

    m0 = t2[0] * twR;
    m1 = t2[1] * twI;
    m2 = t2[1] * twR;
    m3 = t2[0] * twI;

    *p2++ = (m0 + m1);
    *p2++ = (m2 - m3);
    
#if FFT_LOGN > RES_LOGN
    GET_COS_SIN_VALUES(tw3, twR, twI, p, f32);
#else
#ifdef STATIC_VERSION
    GET_COS_SIN_VALUES(tw3, twR, twI, FFT_LOGN, f32);
#else
    GET_COS_SIN_VALUES(tw3, twR, twI, m, f32);
#endif /* STATIC_VERSION */
#endif /* FFT_LOGN > RES_LOGN */

    twI = -twI;
    m0 = t3[0] * twR;
    m1 = t3[1] * twI;
    m2 = t3[1] * twR;
    m3 = t3[0] * twI;

    *p3++ = (m0 + m1);
    *p3++ = (m2 - m3);

#if FFT_LOGN > RES_LOGN
    GET_COS_SIN_VALUES(tw4, twR, twI, p, f32);
#else
#ifdef STATIC_VERSION
    GET_COS_SIN_VALUES(tw4, twR, twI, FFT_LOGN, f32);
#else
    GET_COS_SIN_VALUES(tw4, twR, twI, m, f32);
#endif /* STATIC_VERSION */
#endif /* FFT_LOGN > RES_LOGN */

    twI = -twI;
    m0 = t4[0] * twR;
    m1 = t4[1] * twI;
    m2 = t4[1] * twR;
    m3 = t4[0] * twI;

    *p4++ = (m0 + m1);
    *p4++ = (m2 - m3);

    // first col
    radix8_ibutterfly_f32( pCol1, m - 2, 1u, m); //4u
    // second col
    radix8_ibutterfly_f32( pCol2, m - 2, 1u, m);
    // third col
    radix8_ibutterfly_f32( pCol3, m - 2, 1u, m);
    // fourth col
    radix8_ibutterfly_f32( pCol4, m - 2, 1u, m);
}
#endif // IFFT_DIRECT_CALC
/* function description */

#ifdef IFFT_DIRECT_CALC
int32_t riscv_dsp_cifft_rd8_f32(float32_t *pSrc, uint32_t m)
{
#ifndef ENA_EMPTY_FUNC

#ifdef CHECK_RANGE
#ifdef STATIC_VERSION
    /* Quick return for FFT_LOGN < 4, FFT_LOGN > 14, and FFT_LOGN is odd */
    if (FFT_LOGN < 4 || FFT_LOGN > 14 || ((FFT_LOGN & 0x1) != 0))
    {
        /* only support FFT_N = 16, 64, 256, 1024, 4096, and 16384
         ** that is, FFT_LOGN = 4, 6, 8, 10, 12, and 14. */
#else
    /* Quick return for m < 4, m > 14, and m is odd */
    if (m < 4 || m > 14 || ((m & 0x1) != 0))
    {
        /* only support n = 16, 64, 256, 1024, 4096, and 16384 that is,
         ** m = 4, 6, 8, 10, 12, and 14. */
#endif /* STATIC_VERSION */
        return -1;
    }
#endif /* CHECK_RANGE */

    if(m % 3 == 0)
    {
        radix8_ibutterfly_f32(pSrc, m, 1, m);
        riscv_dsp_bit_reversal8_f32(pSrc, m);
    }
    else if(m % 3 == 1)
    {
        cifft_radix8by2_f32(pSrc, m);
        riscv_dsp_bit_reversal_table_f32(pSrc, m);
    }
    else if (m % 3 == 2)
    {
        cifft_radix8by4_f32(pSrc, m);
        riscv_dsp_bit_reversal_table_f32(pSrc, m);
    }    


    return 0;
#else
    riscv_dsp_empty_ret_func2(pSrc, m);
#endif
}

#else //IFFT_DIRECT_CALC
int32_t riscv_dsp_cifft_rd8_f32(float32_t *pSrc, uint32_t m)
{
#ifndef ENA_EMPTY_FUNC
    // done by using the property: 
    // a* x b* = (axb)*, x*: complex conjugate of x

    uint32_t fftLen = 1 << m;
    /*  Conjugate input data  */
    float *pp = pSrc + 1;
    for(uint32_t l=0; l < fftLen; l++) 
    {
        *pp = -*pp;
        pp += 2;
    }

    riscv_dsp_cfft_rd8_f32(pSrc, m);


    float32_t invL = 1.0f/(float32_t)fftLen;
    /*  Conjugate and scale output data */
    pp = pSrc;
    for(uint32_t l=0; l<fftLen; l++) 
    {
        *pp++ *=  invL ;
        *pp  = -(*pp) * invL;
        pp++;
    }
    return 0;
#else
    riscv_dsp_empty_ret_func2(src, m);
#endif
}
#endif //IFFT_DIRECT_CALC

/**
 * @} end of cfft_radix8
 */
