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

/**
 * @ingroup controller
 */

/**
 * @defgroup controlpark Park Transform
 *
 * In a mathematical Park Transform, the two phases <code>alpha</code> and
 * <code>beta</code> frame representation calculated with the Clarke
 * transform is then fed to a vector rotation block where it is rotated over
 * an angle θ to followthe frame <code> a and b</code> attached to the rotor
 * flux. The rotation over an angle θ is done according to the formulas:
 *
 * <pre>
 *      a = alpha * cos + beta * sin,
 *      b = (-alpha) * sin + beta * cos.
 *          where sin and cos are the cosine and sine value of angle θ.
 * </pre>
 */

/**
 * @addtogroup controlpark
 * @{
 */

/**
 * @} end of controlpark
 */
