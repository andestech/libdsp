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
#include "riscv_dsp_interp.h"

/* function description */
void riscv_dsp_spline_interp_init_f32(riscv_dsp_spline_interp_f32_t * instance,
                                      riscv_dsp_spline_bound_cond cond,
                                      const float32_t * srcx,
                                      const float32_t * srcy,
                                      uint32_t nsize,
                                      float32_t * coeffs,
                                      float32_t * tmpbuf)
{
    /*** COEFFICIENTS COMPUTATION ***/
    /* Type (boundary conditions):
        - Natural spline          ( S1''(x1) = 0 ; Sn''(xn) = 0 )
        - Parabolic runout spline ( S1''(x1) = S2''(x2) ; Sn-1''(xn-1) = Sn''(xn) ) */

    /* (n-1)-long buffers for b, c, and d coefficients */
    float32_t * b = coeffs;
    float32_t * c = coeffs + (nsize - 1);
    float32_t * d = coeffs + (2 * (nsize - 1));

    float32_t * u = tmpbuf;               /* (n-1)-long scratch buffer for u elements */
    float32_t * z = tmpbuf + (nsize - 1); /* n-long scratch buffer for z elements */

    float32_t hi, hm1; /* h(i) and h(i-1) */
    float32_t Bi; /* B(i), i-th element of matrix B=LZ */
    float32_t li; /* l(i), i-th element of matrix L    */
    float32_t cp1; /* Temporary value for c(i+1) */

    int32_t i; /* Loop counter */

    instance->x = srcx;
    instance->y = srcy;
    instance->num_x = nsize;

    /* == Solve LZ=B to obtain z(i) and u(i) == */

    /* -- Row 1 -- */
    /* B(0) = 0, not computed */
    /* u(1,2) = a(1,2)/a(1,1) = a(1,2) */
    if(cond == RISCV_DSP_SPLINE_NATURAL)
        u[0] = 0;  /* a(1,2) = 0 */
    else if(cond == RISCV_DSP_SPLINE_PARABOLIC_RUNOUT)
        u[0] = -1; /* a(1,2) = -1 */

    z[0] = 0;  /* z(1) = B(1)/a(1,1) = 0 always */

    /* -- Rows 2 to N-1 (N=n+1) -- */
    hm1 = srcx[1] - srcx[0]; /* Initialize h(i-1) = h(1) = x(2)-x(1) */

    for (i = 1; i < (int32_t)nsize - 1; i++)
    {
        /* Compute B(i) */
        hi = srcx[i + 1] - srcx[i];
        Bi = 3 * (srcy[i + 1] - srcy[i]) / hi - 3 * (srcy[i] - srcy[i - 1]) / hm1;

        /* l(i) = a(i)-a(i,i-1)*u(i-1) = 2[h(i-1)+h(i)]-h(i-1)*u(i-1) */
        li = 2 * (hi + hm1) - hm1 * u[i - 1];

        /* u(i) = a(i,i+1)/l(i) = h(i)/l(i) */
        u[i] = hi / li;

        /* z(i) = [B(i)-h(i-1)*z(i-1)]/l(i) */
        z[i] = (Bi - hm1 * z[i - 1]) / li;

        /* Update h(i-1) for next iteration */
        hm1 = hi;
    }

    /* -- Row N -- */
    /* l(N) = a(N,N)-a(N,N-1)u(N-1) */
    /* z(N) = [-a(N,N-1)z(N-1)]/l(N) */
    if(cond == RISCV_DSP_SPLINE_NATURAL)
    {
        /* li = 1;     a(N,N) = 1; a(N,N-1) = 0 */
        z[nsize - 1] = 0;   /* a(N,N-1) = 0 */
    }
    else if(cond == RISCV_DSP_SPLINE_PARABOLIC_RUNOUT)
    {
        li = 1 + u[nsize - 2];             /* a(N,N) = 1; a(N,N-1) = -1 */
        z[nsize - 1] = z[nsize - 2] / li;  /* a(N,N-1) = -1 */
    }

    /* == Solve UX = Z to obtain c(i) and    */
    /*    compute b(i) and d(i) from c(i) == */

    cp1 = z[nsize - 1]; /* Initialize c(i+1) = c(N) = z(N) */

    for (i = nsize - 2; i >= 0; i--)
    {
        /* c(i) = z(i)-u(i+1)c(i+1) */
        c[i] = z[i] - u[i] * cp1;

        hi = srcx[i + 1] - srcx[i];
        /* b(i) = [y(i+1)-y(i)]/h(i)-h(i)*[c(i+1)+2*c(i)]/3 */
        b[i] = (srcy[i + 1] - srcy[i]) / hi - hi * (cp1 + 2 * c[i]) / 3;

        /* d(i) = [c(i+1)-c(i)]/[3*h(i)] */
        d[i] = (cp1 - c[i]) / (3 * hi);

        /* Update c(i+1) for next iteration */
        cp1 = c[i];
    }

    /* == Finally, store the coefficients in the instance == */
    instance->coeffs = coeffs;
}
