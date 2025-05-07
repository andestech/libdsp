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

#include "internal_isa.h"
#include "riscv_dsp_controller_math.h"

/**
 * @ingroup controller
 */

/**
 * @addtogroup controlpid
 * @{
 */

/**
 * @brief PID initializatopn control function of Q31 formats.
 * @param[in, out]    *instance    points to an instance of the PID
 *                                 controliler.
 * @param[in]               set    for 1 will set the state, 0 will not.
 * @return none.
 *
 * This function will calculate the PID control structure gain
 * <code>gain1</code>, <code>gain2</code> and <code>gain3</code> by seting
 * the variable <code>Kp</code>, <code>Ki</code> and <code>Kd</code>. The
 * state variable will set to all zeros.
 */

void riscv_dsp_init_pid_q31(riscv_dsp_pid_q31_t *instance, int32_t set)
{
    q31_t tmp;
    q31_t kp, ki, kd;
    kp = instance->Kp;
    ki = instance->Ki;
    kd = instance->Kd;
    tmp = (q31_t) NDS_ISA_KADDW(kp, ki);
    instance->gain1 = (q31_t) NDS_ISA_KADDW(tmp, kd);
    tmp = (q31_t) NDS_ISA_KADDW(kd, kd);
    instance->gain2 = -(q31_t) NDS_ISA_KADDW(tmp, kp);
    instance->gain3 = kd;

    if (set)
    {
        //memset(instance->state, 0, 3u * sizeof(q31_t));
        instance->state[0] = 0;
        instance->state[1] = 0;
        instance->state[2] = 0;
    }
}

/**
 * @} end of controlpid
 */
