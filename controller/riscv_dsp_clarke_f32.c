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
 * @defgroup controlclarke Clarke Transform
 *
 * Clarke transform is a mathematical transformation employed to simplify
 * the analysis of three-phase system <code> a, b and c</code> to a
 * two-phase <code>alpha and beta</code> orthogonal system.
 *
 * Consider now a particular case with <code>alpha</code> superposed with
 * <code>a</code> and <code>a + b + c</code> is zero, in this condition
 * <code> a, b and c</code> can be transformed to <code>alpha</code> and
 * <code>beta</code> with following mathematical transformation:
 *
 * <pre>
 *      alpha = a,
 *      beta = (1 / sqrt(3))* a + (2 / sqrt(3)) * b,
 *      a + b + c = 0.
 * </pre>
 */

/**
 * @addtogroup controlclarke
 * @{
 */

/**
 * @} end of controlclarke
 */
