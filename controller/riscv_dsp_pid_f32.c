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
#include "riscv_dsp_controller_math.h"

/**
 * @ingroup controller
 */

/**
 * @defgroup controlpid PID Control
 *
 * A proportional-integral-derivative controller (PID controller) is a
 * control loop feedback mechanism (controller) used in control systems.
 *
 *
 * <pre>
 *    dst[k] = dst[k - 1] + gain1 * src[k] + gain2 * src[k - 1]
 *             + gain3 * src[k - 2]
 *    gain1 = Kp + Ki + Kd
 *    gain2 = (-Kp) - (2 * Kd)
 *    gain3 = Kd
 * </pre>
 */

/**
 * @addtogroup controlpid
 * @{
 */

/**
 * @brief PID initializatopn control function of floating-point formats.
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

void riscv_dsp_init_pid_f32(riscv_dsp_pid_f32_t *instance, int32_t set)
{
    instance->gain1 = instance->Kp + instance->Ki + instance->Kd;
    instance->gain2 = -(instance->Kp) - ((float32_t) 2.0 * instance->Kd);
    instance->gain3 = instance->Kd;

    if (set)
    {
        instance->state[0] = 0;
        instance->state[1] = 0;
        instance->state[2] = 0;
    }
}

/**
 * @} end of controlpid
 */
