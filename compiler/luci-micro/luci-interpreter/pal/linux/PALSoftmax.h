/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd. All Rights Reserved
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

#ifndef LUCI_INTERPRETER_PAL_SOFTMAX_H
#define LUCI_INTERPRETER_PAL_SOFTMAX_H

#include <tensorflow/lite/kernels/internal/optimized/optimized_ops.h>

namespace luci_interpreter_pal
{
static inline void PopulateSoftmaxLookupTable(tflite::SoftmaxParams *data, float input_scale,
                                              float beta)
{
  tflite::optimized_ops::PopulateSoftmaxLookupTable(data, input_scale, beta);
}

static inline void InitializeParams(tflite::SoftmaxParams *params, float input_scale, float beta)
{
  // Do nothing for linux
  (void)params;
  (void)input_scale;
  (void)beta;
}

template <typename In, typename Out>
static inline void Softmax(const tflite::SoftmaxParams &params,
                           const tflite::RuntimeShape &input_shape, const In *input_data,
                           const tflite::RuntimeShape &output_shape, Out *output_data)
{
  tflite::optimized_ops::Softmax(params, input_shape, input_data, output_shape, output_data);
}
} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_SOFTMAX_H
