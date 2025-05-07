/******************************************************************************
 * Copyright (C) 2010-2018 Arm Limited or its affiliates. All rights reserved.*
 * Copyright (C) 2018-2021 Andes Technology Corporation. All rights reserved. *
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
#include "internal_quaternion.h"

/**
   @brief Conversion of quaternion to equivalent rotation matrix.
   @param[in]       src pointer of the input quaternions vector
   @param[out]      dst pointer of the output 3x3 rotations matrix
   @param[in]       size number of quaternions in a vector
   @return none.
   @par
   Format of rotation matrix
   The quaternion a + ib + jc + kd is converted into rotation matrix:
   <pre>
     a^2 + b^2 - c^2 - d^2                 2bc - 2ad                 2bd + 2ac
                 2bc + 2ad     a^2 - b^2 + c^2 - d^2                 2cd - 2ab
                 2bd - 2ac                 2cd + 2ab     a^2 - b^2 - c^2 + d^2
   </pre>
   Rotation matrix is saved in row order : R00 R01 R02 R10 R11 R12 R20 R21 R22
 */

void riscv_dsp_quaternion2rotation_f32(const float32_t * FUNC_RESTRICT src, float32_t * FUNC_RESTRICT dst, uint32_t size)
{
    uint32_t nb;
    for(nb = 0; nb < size; nb++)
    {
        float32_t q00 = DSP_SQUARE(src[0 + nb * 4]);
        float32_t q11 = DSP_SQUARE(src[1 + nb * 4]);
        float32_t q22 = DSP_SQUARE(src[2 + nb * 4]);
        float32_t q33 = DSP_SQUARE(src[3 + nb * 4]);
        float32_t q01 =  src[0 + nb * 4] * src[1 + nb * 4];
        float32_t q02 =  src[0 + nb * 4] * src[2 + nb * 4];
        float32_t q03 =  src[0 + nb * 4] * src[3 + nb * 4];
        float32_t q12 =  src[1 + nb * 4] * src[2 + nb * 4];
        float32_t q13 =  src[1 + nb * 4] * src[3 + nb * 4];
        float32_t q23 =  src[2 + nb * 4] * src[3 + nb * 4];

        float32_t xx = q00 + q11 - q22 - q33;
        float32_t yy = q00 - q11 + q22 - q33;
        float32_t zz = q00 - q11 - q22 + q33;
        float32_t xy = 2 * (q12 - q03);
        float32_t xz = 2 * (q13 + q02);
        float32_t yx = 2 * (q12 + q03);
        float32_t yz = 2 * (q23 - q01);
        float32_t zx = 2 * (q13 - q02);
        float32_t zy = 2 * (q23 + q01);

        dst[0 + nb * 9] = xx;
        dst[1 + nb * 9] = xy;
        dst[2 + nb * 9] = xz;
        dst[3 + nb * 9] = yx;
        dst[4 + nb * 9] = yy;
        dst[5 + nb * 9] = yz;
        dst[6 + nb * 9] = zx;
        dst[7 + nb * 9] = zy;
        dst[8 + nb * 9] = zz;
    }
}
