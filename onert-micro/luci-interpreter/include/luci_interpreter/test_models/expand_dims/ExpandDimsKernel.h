/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd. All Rights Reserved
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

#ifndef LUCI_INTERPRETER_TEST_MODELS_EXPAND_DIMS_KERNEL_H
#define LUCI_INTERPRETER_TEST_MODELS_EXPAND_DIMS_KERNEL_H

#include "luci_interpreter/test_models/TestDataBase.h"

namespace luci_interpreter
{
namespace test_kernel
{

namespace neg_test_expand_dims
{

/*
 * ExpandDims Kernel:
 *
 *     Input(3, 3)   Const([1]) - wrong type for const
 *          |       /
 *       ExpandDims
 *          |
 *     Output(3, 1, 3)
 */

const unsigned char test_kernel_model_circle[] = {
  0x18, 0x00, 0x00, 0x00, 0x43, 0x49, 0x52, 0x30, 0x00, 0x00, 0x0e, 0x00, 0x14, 0x00, 0x00, 0x00,
  0x0c, 0x00, 0x08, 0x00, 0x10, 0x00, 0x04, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
  0x44, 0x00, 0x00, 0x00, 0x70, 0x01, 0x00, 0x00, 0x8c, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
  0x30, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x06, 0x00, 0x08, 0x00, 0x04, 0x00, 0x06, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
  0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x8c, 0xff, 0xff, 0xff, 0x90, 0xff, 0xff, 0xff,
  0x94, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00,
  0x18, 0x00, 0x14, 0x00, 0x10, 0x00, 0x0c, 0x00, 0x08, 0x00, 0x04, 0x00, 0x0e, 0x00, 0x00, 0x00,
  0x14, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00,
  0x68, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x6d, 0x61, 0x69, 0x6e, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x14, 0x00, 0x00, 0x00,
  0x10, 0x00, 0x0c, 0x00, 0x07, 0x00, 0x08, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34,
  0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x04, 0x00, 0x04, 0x00,
  0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00,
  0x3c, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0xa8, 0xff, 0xff, 0xff, 0x0c, 0x00, 0x00, 0x00,
  0x02, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x6f, 0x66, 0x6d, 0x00,
  0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
  0x0c, 0x00, 0x14, 0x00, 0x10, 0x00, 0x0f, 0x00, 0x08, 0x00, 0x04, 0x00, 0x0c, 0x00, 0x00, 0x00,
  0x10, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x10, 0x00, 0x00, 0x00,
  0x04, 0x00, 0x00, 0x00, 0x69, 0x66, 0x6d, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x0c, 0x00, 0x10, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x08, 0x00, 0x04, 0x00, 0x0c, 0x00, 0x00, 0x00,
  0x0c, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
  0x69, 0x66, 0x6d, 0x31, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
  0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x0c, 0x00,
  0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x46, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x46, 0x11, 0x00, 0x00, 0x00, 0x4f, 0x4e, 0x45, 0x2d, 0x74, 0x66, 0x6c, 0x69,
  0x74, 0x65, 0x32, 0x63, 0x69, 0x72, 0x63, 0x6c, 0x65, 0x00, 0x00, 0x00};
} // namespace neg_test_expand_dims

namespace expand_dims_kernel
{
/*
 * ExpandDims Kernel:
 *
 *     Input(3, 3)   Const([1])
 *          |       /
 *       ExpandDims
 *          |
 *     Output(3, 1, 3)
 */
const unsigned char test_kernel_model_circle[] = {
  0x18, 0x00, 0x00, 0x00, 0x43, 0x49, 0x52, 0x30, 0x00, 0x00, 0x0e, 0x00, 0x14, 0x00, 0x00, 0x00,
  0x0c, 0x00, 0x08, 0x00, 0x10, 0x00, 0x04, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
  0x44, 0x00, 0x00, 0x00, 0x70, 0x01, 0x00, 0x00, 0x8c, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
  0x30, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x06, 0x00, 0x08, 0x00, 0x04, 0x00, 0x06, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
  0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x8c, 0xff, 0xff, 0xff, 0x90, 0xff, 0xff, 0xff,
  0x94, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00,
  0x18, 0x00, 0x14, 0x00, 0x10, 0x00, 0x0c, 0x00, 0x08, 0x00, 0x04, 0x00, 0x0e, 0x00, 0x00, 0x00,
  0x14, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00,
  0x68, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x6d, 0x61, 0x69, 0x6e, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x14, 0x00, 0x00, 0x00,
  0x10, 0x00, 0x0c, 0x00, 0x07, 0x00, 0x08, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34,
  0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x04, 0x00, 0x04, 0x00,
  0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00,
  0x3c, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0xa8, 0xff, 0xff, 0xff, 0x0c, 0x00, 0x00, 0x00,
  0x02, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x6f, 0x66, 0x6d, 0x00,
  0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
  0x0c, 0x00, 0x14, 0x00, 0x10, 0x00, 0x0f, 0x00, 0x08, 0x00, 0x04, 0x00, 0x0c, 0x00, 0x00, 0x00,
  0x10, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x10, 0x00, 0x00, 0x00,
  0x04, 0x00, 0x00, 0x00, 0x69, 0x66, 0x6d, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x0c, 0x00, 0x10, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x08, 0x00, 0x04, 0x00, 0x0c, 0x00, 0x00, 0x00,
  0x0c, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
  0x69, 0x66, 0x6d, 0x31, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
  0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x0c, 0x00,
  0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x46, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x46, 0x11, 0x00, 0x00, 0x00, 0x4f, 0x4e, 0x45, 0x2d, 0x74, 0x66, 0x6c, 0x69,
  0x74, 0x65, 0x32, 0x63, 0x69, 0x72, 0x63, 0x6c, 0x65, 0x00, 0x00, 0x00};

const std::vector<float> input_data = {10.438837, -1.168417,  -6.455261, -1.3638954, 31.58745,
                                       29.395872, -10.366383, 7.6131954, 9.837751};

const std::vector<float> reference_output_data = {10.438837,  -1.168417, -6.455261,
                                                  -1.3638954, 31.58745,  29.395872,
                                                  -10.366383, 7.6131954, 9.837751};

} // namespace expand_dims_kernel

template <typename T> class TestDataExpandDimsKernel : public TestDataBase<T>
{
public:
  TestDataExpandDimsKernel()
  {
    _input_data = expand_dims_kernel::input_data;
    _reference_output_data = expand_dims_kernel::reference_output_data;
    _test_kernel_model_circle = expand_dims_kernel::test_kernel_model_circle;
  }

  ~TestDataExpandDimsKernel() override = default;

  const unsigned char *get_model_ptr() override final { return _test_kernel_model_circle; }

  const std::vector<T> &get_input_data_by_index(int i) override final
  {
    switch (i)
    {
      case 0:
        return _input_data;
      default:
        assert(false && "Wrong input index");
    }
  }

  const std::vector<T> &get_output_data_by_index(int i) override final
  {
    assert(i == 0);
    return _reference_output_data;
  }

protected:
  std::vector<T> _input_data;
  std::vector<T> _reference_output_data;
  const unsigned char *_test_kernel_model_circle;
};

class NegTestDataInvalidInputTypeExpandDimsKernel : public NegTestDataBase
{
public:
  NegTestDataInvalidInputTypeExpandDimsKernel()
  {
    _test_kernel_model_circle = neg_test_expand_dims::test_kernel_model_circle;
  }

  ~NegTestDataInvalidInputTypeExpandDimsKernel() override = default;

  const unsigned char *get_model_ptr() override final { return _test_kernel_model_circle; }

protected:
  const unsigned char *_test_kernel_model_circle;
};

} // namespace test_kernel
} // namespace luci_interpreter

#endif // LUCI_INTERPRETER_TEST_MODELS_EXPAND_DIMS_KERNEL_H
