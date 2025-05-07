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
#include <math.h>
#include "internal_quaternion.h"

/**
 * @brief Conversion of a rotation matrix to an equivalent quaternion.
 * @param[in]       src points to an array 3x3 rotation matrix (in row order)
 * @param[out]      dst points to an array quaternions
 * @param[in]       size number of quaternions in the array
 * @return none.
 *
 * q and -q are representing the same rotation. This ambiguity must be taken into
 * account when using the output of this function.
 */

void riscv_dsp_rotation2quaternion_f32(const float32_t * FUNC_RESTRICT src, float32_t * FUNC_RESTRICT dst, uint32_t size)
{
    uint32_t nb;
    for(nb = 0; nb < size; nb++)
    {
        const float32_t *r = &src[nb * 9];
        float32_t *q = &dst[nb * 4];

        float32_t trace = DSP_QUATER_RI(0, 0) + DSP_QUATER_RI(1, 1) + DSP_QUATER_RI(2, 2);

        float32_t doubler;
        float32_t s;

        if (trace > 0.0f)
        {
            doubler = sqrtf(trace + 1.0f) * 2.0f; // invs=4*qw
            s = 1.0f / doubler;
            q[0] = 0.25f * doubler;
            q[1] = (DSP_QUATER_RI(2, 1) - DSP_QUATER_RI(1, 2)) * s;
            q[2] = (DSP_QUATER_RI(0, 2) - DSP_QUATER_RI(2, 0)) * s;
            q[3] = (DSP_QUATER_RI(1, 0) - DSP_QUATER_RI(0, 1)) * s;
        }
        else if ((DSP_QUATER_RI(0, 0) > DSP_QUATER_RI(1, 1)) && (DSP_QUATER_RI(0, 0) > DSP_QUATER_RI(2, 2)) )
        {
            doubler = sqrtf(1.0f + DSP_QUATER_RI(0, 0) - DSP_QUATER_RI(1, 1) - DSP_QUATER_RI(2, 2)) * 2.0f; // invs=4*qx
            s = 1.0f / doubler;
            q[0] = (DSP_QUATER_RI(2, 1) - DSP_QUATER_RI(1, 2)) * s;
            q[1] = 0.25f * doubler;
            q[2] = (DSP_QUATER_RI(0, 1) + DSP_QUATER_RI(1, 0)) * s;
            q[3] = (DSP_QUATER_RI(0, 2) + DSP_QUATER_RI(2, 0)) * s;
        }
        else if (DSP_QUATER_RI(1, 1) > DSP_QUATER_RI(2, 2))
        {
            doubler = sqrtf(1.0f + DSP_QUATER_RI(1, 1) - DSP_QUATER_RI(0, 0) - DSP_QUATER_RI(2, 2)) * 2.0f; // invs=4*qy
            s = 1.0f / doubler;
            q[0] = (DSP_QUATER_RI(0, 2) - DSP_QUATER_RI(2, 0)) * s;
            q[1] = (DSP_QUATER_RI(0, 1) + DSP_QUATER_RI(1, 0)) * s;
            q[2] = 0.25f * doubler;
            q[3] = (DSP_QUATER_RI(1, 2) + DSP_QUATER_RI(2, 1)) * s;
        }
        else
        {
            doubler = sqrtf(1.0f + DSP_QUATER_RI(2, 2) - DSP_QUATER_RI(0, 0) - DSP_QUATER_RI(1, 1)) * 2.0f; // invs=4*qz
            s = 1.0f / doubler;
            q[0] = (DSP_QUATER_RI(1, 0) - DSP_QUATER_RI(0, 1)) * s;
            q[1] = (DSP_QUATER_RI(0, 2) + DSP_QUATER_RI(2, 0)) * s;
            q[2] = (DSP_QUATER_RI(1, 2) + DSP_QUATER_RI(2, 1)) * s;
            q[3] = 0.25f * doubler;
        }

    }
}
