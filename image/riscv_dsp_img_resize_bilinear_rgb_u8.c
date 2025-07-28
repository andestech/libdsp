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
#include "stdio.h"
#include <config.h>
#include "riscv_dsp_math_types.h"
#include "internal_nds_types.h"
#include <string.h>
#include <math.h>
#include <stdint.h> //uintptr_t

#define CHANGE

#define INTER_RESIZE_COEF_BITS  11
#define INTER_RESIZE_COEF_SCALE (1 << INTER_RESIZE_COEF_BITS)
#define MAX_ESIZE          16

#ifdef SHIFT_GAIN_FLOAT_TO_INT
#define INTER_FLOAT_TO_Q_SHIFT  18
#define INTER_FLOAT_1P0_TO_Q_VAL (1 << INTER_FLOAT_TO_Q_SHIFT)
#define INT_PART_Q              0xFFFC0000    // data length support to int32
#define SHIFT_TO_Q11          (INTER_FLOAT_TO_Q_SHIFT -INTER_RESIZE_COEF_BITS)
#endif


static inline uintptr_t riscv_dsp_align_size (uintptr_t sz, q31_t n)
{
    return (sz + n - 1) & -n;
}

#ifndef SHIFT_GAIN_FLOAT_TO_INT
static inline q31_t img_floor (float32_t a)
{
    return ( ( (a) >= 0) ? ( (q31_t) a) : ( (q31_t) a - 1));
}
#endif

static inline q31_t img_clip (q31_t x, q31_t a, q31_t b)
{
    return (x >= a ? (x < b ? x : b - 1) : a);
}

static void riscv_dsp_img_hresize_linear (const u8_t** src,
        int32_t** dst,
        q31_t count,
        const q31_t* xofs,
        const q15_t* alpha,
        q31_t dwidth,
        q31_t cn,
        q31_t xmax)
{
    q31_t k;
    q31_t dx;
    if (count == 2)
    {
        k = 0;
        const u8_t *S0 = src[k], *S1 = src[k + 1];
        int32_t *D0 = dst[k], *D1 = dst[k + 1];

        for (dx = 0; dx < xmax; dx+=3)
        {
            q31_t sx = xofs[dx];
            int32_t a0 = alpha[dx * 2], a1 = alpha[dx * 2 + 1];

            int32_t t0 = S0[sx] * a0 + S0[sx + 3] * a1;
            int32_t t1 = S1[sx] * a0 + S1[sx + 3] * a1;
            D0[dx] = t0;
            D1[dx] = t1;
            t0 = S0[sx + 1] * a0 + S0[sx + 4] * a1;
            t1 = S1[sx + 1] * a0 + S1[sx + 4] * a1;
            D0[dx+1] = t0;
            D1[dx+1] = t1;
            t0 = S0[sx + 2] * a0 + S0[sx + 5] * a1;
            t1 = S1[sx + 2] * a0 + S1[sx + 5] * a1;
            D0[dx+2] = t0;
            D1[dx+2] = t1;
        }
        for (dx = xmax; dx < dwidth; dx++)
        {
            q31_t sx = xofs[dx];
            D0[dx] = (int32_t) S0[sx] * INTER_RESIZE_COEF_SCALE;
            D1[dx] = (int32_t) S1[sx] * INTER_RESIZE_COEF_SCALE;
        }
    }
    if (count == 1)
    {
        k = 0;
        const u8_t *S0 = src[k];
        int32_t *D0 = dst[k];

        for (dx = 0; dx < xmax; dx+=cn)
        {
            q31_t sx = xofs[dx];
            int32_t a0 = alpha[dx * 2], a1 = alpha[dx * 2 + 1];
            D0[dx] = S0[sx] * a0 + S0[sx + 3] * a1;
            D0[dx + 1] = S0[sx + 1] * a0 + S0[sx + 4] * a1;
            D0[dx + 2] = S0[sx + 2] * a0 + S0[sx + 5] * a1;
        }
        for (dx = xmax; dx < dwidth; dx++)
        {
            D0[dx] = (int32_t) S0[xofs[dx]] * INTER_RESIZE_COEF_SCALE;
        }
    }
}
static inline u8_t riscv_dsp_cast_op (q31_t val)
{
    q31_t bits = INTER_RESIZE_COEF_BITS * 2;
    q31_t SHIFT = bits;
    q31_t DELTA = 1 << (bits - 1) ;
    q31_t temp = NDSV_MIN (255, NDSV_MAX (0, (val + DELTA) >> SHIFT)); //Rounding
    return (u8_t) (temp);
}

void riscv_dsp_img_vresize_linear (const int32_t** src, u8_t* dst, const q15_t* beta, q31_t width)
{
    q31_t b0 = beta[0], b1 = beta[1];
    const int32_t *S0 = src[0], *S1 = src[1];

    q31_t x = 0;
    for (x = 0; x < width; x += 3)
    {
        q31_t t0, t1, t2;
        t0 = S0[x] * b0 + S1[x] * b1;
        t1 = S0[x + 1] * b0 + S1[x + 1] * b1;
        t2 = S0[x + 2] * b0 + S1[x + 2] * b1;
        dst[x] = riscv_dsp_cast_op (t0);
        dst[x + 1] = riscv_dsp_cast_op (t1);
        dst[x + 2] = riscv_dsp_cast_op (t2);
    }

}

static void riscv_dsp_img_resize_generic_linear (u8_t* src,
        u8_t* dst,
        const q31_t* xofs,
        const q15_t* _alpha,
        const q31_t* yofs,
        const q15_t* _beta,
        q31_t xmin,
        q31_t xmax,
        q31_t ksize,
        q31_t srcw,
        q31_t srch,
        q31_t srcstep,
        q31_t dstw,
        q31_t dsth,
        q31_t channels)
{
    const q15_t* alpha = _alpha;
    const q15_t* beta = _beta;
    q31_t cn = channels;
    q31_t tmp = srcw*cn;
    srcw = tmp;
    //srcw *= cn;
    dstw *= cn;
#ifdef CHANGE
    int dirty = 1;
#endif
    q31_t bufstep = (q31_t) riscv_dsp_align_size (dstw, 64); //align 64-byte for cache friendly
    q31_t dststep = dstw;
    q31_t *buf = (q31_t*) NDSV_MALLOC (bufstep * ksize * sizeof (q31_t) + 64);
    const u8_t* srows[MAX_ESIZE];
    int32_t* rows[MAX_ESIZE];
    int32_t prev_sy[MAX_ESIZE];
    q31_t k, dy;
    tmp = xmin*cn;

    //xmin *= cn;
    xmin = tmp;
    xmax *= cn;
    for (k = 0; k < ksize; k++)
    {
        prev_sy[k] = -1;
        //rows[k] = (int32_t*) buf + bufstep * k;
        rows[k] = (int32_t*) (uintptr_t)riscv_dsp_align_size ((uintptr_t)buf, 64) + bufstep * k;
    }

    // image resize is a separable operation. In case of not too strong
    for (dy = 0; dy < dsth; dy++, beta += ksize)
    {
        q31_t sy0 = yofs[dy], k, k0 = ksize, k1 = 0, ksize2 = ksize / 2;
        for (k = 0; k < ksize; k++)
        {
            q31_t sy = img_clip (sy0 - ksize2 + 1 + k, 0, srch);//sy0=0~srch-1, k=0~1
            for (k1 = NDSV_MAX (k1, k); k1 < ksize; k1++)
            {
                if (sy == prev_sy[k1])  // if the sy-th row has been computed already, reuse it.
                {
                    if (k1 > k)
                    {
#ifdef CHANGE
                        int32_t* temp = rows[k];
                        rows[k] = rows[k1];
                        rows[k1] = temp;
#else
                        memcpy(rows[k], rows[k1], bufstep * sizeof(rows[0][0])); // k=0, k1=1
#endif
                    }
                    break;
                }
            }
            if (k1 == ksize)
            {
                k0 = NDSV_MIN (k0, k); // remember the first row that needs to be computed
            }
            srows[k] = (const u8_t*) (src + srcstep * sy);
            prev_sy[k] = sy;
        }

#ifdef CHANGE
        if (k0 < ksize)
        {
            riscv_dsp_img_hresize_linear(srows + k0, rows + k0, ksize - k0, xofs, alpha,
                                           dstw, cn, xmax);
            dirty = 1;
        }
        else if (prev_sy[0] == prev_sy[1] && dirty)
        {
            memcpy(rows[1], rows[0], bufstep * sizeof(rows[0][0]));
            dirty = 0;
        }
#else
        if (k0 < ksize)
        {
            riscv_dsp_img_hresize_linear(srows + k0, rows + k0, ksize - k0, xofs, alpha,
                                           dstw, cn, xmax);
        }
#endif
        riscv_dsp_img_vresize_linear ( (const int32_t**) rows, (u8_t*) (dst + dststep * dy), beta, dstw);
    }

    NDSV_FREE (buf);
}

static void riscv_dsp_img_resize_cal_offset_linear (q31_t* xofs,
        q15_t* ialpha,
        q31_t* yofs,
        q15_t* ibeta,
        q31_t *xmin,
        q31_t *xmax,
        q31_t ksize,
        q31_t ksize2,
        q31_t srcw,
        q31_t srch,
        q31_t dstw,
        q31_t dsth,
        q31_t channels)
{
#ifdef SHIFT_GAIN_FLOAT_TO_INT
    float32_t scale_x = (float32_t) srcw / dstw;
    float32_t scale_y = (float32_t) srch / dsth;
    float32_t tmp_x = (float32_t)scale_x * INTER_FLOAT_1P0_TO_Q_VAL;
    float32_t tmp_y = (float32_t)scale_y * INTER_FLOAT_1P0_TO_Q_VAL;
    float32_t gain_shift_x_f = (float32_t) scale_x*0.5f * INTER_FLOAT_1P0_TO_Q_VAL ;
    float32_t gain_shift_y_f = (float32_t) scale_y*0.5f * INTER_FLOAT_1P0_TO_Q_VAL ;

    q31_t half_q = INTER_FLOAT_1P0_TO_Q_VAL/2;
    q31_t cn = channels;

    q31_t scale_x_q = (q31_t) tmp_x;
    q31_t scale_y_q = (q31_t) tmp_y;

    q31_t cbu_q[MAX_ESIZE];
    q31_t q_x, q_y, k, sx, sy, dx, dy, sx_q0, sy_q0;
    q31_t gain_shift_x = (q31_t) gain_shift_x_f;
    q31_t gain_shift_y = (q31_t) gain_shift_y_f;
    for (dx = 0; dx < dstw; dx++)
    {
        q_x = ((dx*scale_x_q) + gain_shift_x - half_q);
        sx = ((q_x >=0) ? (q_x & INT_PART_Q) : (((q_x) & INT_PART_Q) - INTER_FLOAT_1P0_TO_Q_VAL));
        q_x -= sx;        // fraction part.
        sx_q0 = (sx >> INTER_FLOAT_TO_Q_SHIFT); // from qx -> q0
        if (sx_q0 < ksize2 - 1)
        {
            *xmin = dx + 1;
            if (sx_q0 < 0)
                q_x= 0, sx_q0 = 0;
        }

        if (sx_q0 + ksize2 >= srcw)
        {
            *xmax = NDSV_MIN (*xmax, dx);
            if (sx_q0 >= srcw - 1)
                q_x = 0, sx_q0 = srcw - 1;
        }
        sx = sx_q0;

        cbu_q[0] = (INTER_FLOAT_1P0_TO_Q_VAL - q_x) >> SHIFT_TO_Q11;   // 1.0 - fx
        cbu_q[1] = (q_x) >> SHIFT_TO_Q11;
        for (k = 0, sx *= cn; k < cn; k++)
        {
            xofs[dx * cn + k] = sx + k;
        }

        for (k = 0; k < ksize; k++)
        {
            ialpha[dx * cn * ksize + k] = (q15_t)(cbu_q[k]);
        }
        for (; k < cn * ksize; k++)
        {
            ialpha[dx * cn * ksize + k] = ialpha[dx * cn * ksize + k - ksize];
        }
    }

    for (dy = 0; dy < dsth; dy++)
    {
        q_y = (dy*scale_y_q) + gain_shift_y - half_q;
        sy = ((q_y >=0) ? (q_y & INT_PART_Q) : (((q_y) & INT_PART_Q) - INTER_FLOAT_1P0_TO_Q_VAL) );
        q_y -= sy ;
        sy_q0 = (sy >> INTER_FLOAT_TO_Q_SHIFT); // from qx -> q0
        yofs[dy] = sy_q0;
        cbu_q[0] = (INTER_FLOAT_1P0_TO_Q_VAL - q_y) >> SHIFT_TO_Q11;   // 1.0 - fx
        cbu_q[1] = (q_y) >> SHIFT_TO_Q11;
        for (k = 0; k < ksize; k++)
        {
            ibeta[dy * ksize + k] = (q15_t) (cbu_q[k]);
        }
    }

#else //SHIFT_GAIN_FLOAT_TO_INT
    float32_t inv_scale_x = (float32_t) dstw / srcw;
    float32_t inv_scale_y = (float32_t) dsth / srch;

    q31_t cn = channels;
    float32_t scale_x = 1.0f / inv_scale_x;
    float32_t scale_y = 1.0f / inv_scale_y;

    q31_t k, sx, sy, dx, dy;

    float32_t fx, fy;

    float32_t cbuf[MAX_ESIZE];

    for (dx = 0; dx < dstw; dx++)
    {
        fx = (float32_t) ( (dx + 0.5f) * scale_x - 0.5f);
        sx = img_floor (fx);
        fx -= sx;

        if (sx < ksize2 - 1)
        {
            *xmin = dx + 1;
            if (sx < 0)
                fx = 0, sx = 0;
        }

        if (sx + ksize2 >= srcw)
        {
            *xmax = NDSV_MIN (*xmax, dx);
            if (sx >= srcw - 1)
                fx = 0, sx = srcw - 1;
        }

        cbuf[0] = 1.f - fx;
        cbuf[1] = fx;
        for (k = 0, sx *= cn; k < cn; k++)
            xofs[dx * cn + k] = sx + k;

        for (k = 0; k < ksize; k++)
        {
            ialpha[dx * cn * ksize + k] = (q15_t)(cbuf[k] * INTER_RESIZE_COEF_SCALE);
        }
        for (; k < cn * ksize; k++)
        {
            ialpha[dx * cn * ksize + k] = ialpha[dx * cn * ksize + k - ksize];
        }
    }
    for (dy = 0; dy < dsth; dy++)
    {
        fy = (float32_t) ( (dy + 0.5f) * scale_y - 0.5f);
        sy = img_floor (fy);
        fy -= sy;
        yofs[dy] = sy;

        cbuf[0] = 1.0f - fy;
        cbuf[1] = fy;

        for (k = 0; k < ksize; k++)
        {
            ibeta[dy * ksize + k] = (q15_t) (cbuf[k] * INTER_RESIZE_COEF_SCALE);
        }
    }
#endif //SHIFT_GAIN_FLOAT_TO_INT
}

void riscv_dsp_img_resize_bilinear_rgb_u8(u8_t* dst,
        u32_t dst_width,
        u32_t dst_height,
        u8_t* src,
        u32_t src_width,
        u32_t src_height,
        u32_t src_stride)
{
    q31_t dstw = dst_width;
    q31_t dsth = dst_height;
    q31_t srcw = src_width;
    q31_t srch = src_height;

    q31_t cn = 3;
    q31_t xmin = 0;
    q31_t xmax = dstw;
    q31_t width = dstw * cn;

    q31_t ksize = 0, ksize2;
    ksize = 2;
    ksize2 = ksize / 2;

    // (4 * dstw + dsth) * (4 + 4 * 2);
    u8_t *buf = (u8_t*) NDSV_MALLOC ( (width + dsth) * (sizeof (q31_t) + sizeof (float32_t) * ksize));

    q31_t* xofs = (q31_t*) buf;
    q31_t* yofs = xofs + width;
    q15_t* ialpha = (q15_t*) (yofs + dsth);
    q15_t* ibeta = ialpha + width * ksize;

    riscv_dsp_img_resize_cal_offset_linear(xofs, ialpha, yofs, ibeta, &xmin, &xmax, ksize, ksize2, srcw, srch, dstw, dsth, cn);
    riscv_dsp_img_resize_generic_linear(src, dst, xofs, ialpha, yofs, ibeta, xmin, xmax, ksize, srcw, srch, src_stride, dstw, dsth, cn);
    NDSV_FREE (buf);
}
