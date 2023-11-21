/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd. All Rights Reserved
 * Copyright 2019 The TensorFlow Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LUCI_INTERPRETER_PAL_MINIMUM_COMMON_H
#define LUCI_INTERPRETER_PAL_MINIMUM_COMMON_H

#include "Params.h"
#include "PALUtils.h"
#include "ProcessBroadcastShapes.h"
#include "Broadcast.h"

namespace luci_interpreter_pal
{
inline void Minimum(const int flat_size, const float *input1_data, const float *input2_data,
                    float *output_data)
{
  for (int i = 0; i < flat_size; ++i)
  {
    output_data[i] = std::min(input1_data[i], input2_data[i]);
  }
}

template <typename T>
inline void
BroadcastMinimum4DSlow(const luci_interpreter::RuntimeShape &input1_shape, const T *input1_data,
                       const luci_interpreter::RuntimeShape &input2_shape, const T *input2_data,
                       const luci_interpreter::RuntimeShape &output_shape, T *output_data)
{
  auto func = [](const T &a, const T &b) -> const T & { return std::min(a, b); };
  BroadcastTISO4DSlow<float>(input1_shape, input1_data, input2_shape, input2_data, output_shape,
                             output_data, func);
}
} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_MINIMUM_COMMON_H
