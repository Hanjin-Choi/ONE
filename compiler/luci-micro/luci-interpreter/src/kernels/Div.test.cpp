/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd. All Rights Reserved
 * Copyright 2017 The TensorFlow Authors. All Rights Reserved.
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

#include "kernels/Div.h"
#include "kernels/TestUtils.h"
#include "luci_interpreter/TestMemoryManager.h"

namespace luci_interpreter
{
namespace kernels
{
namespace
{

using namespace testing;

class DivTest : public ::testing::Test
{
protected:
  void SetUp() override { _memory_manager = std::make_unique<TestMemoryManager>(); }

  std::unique_ptr<IMemoryManager> _memory_manager;
};

float GetTolerance(float min, float max)
{
  const float kQuantizedStep = (max - min) / 255.0f;
  const float kQuantizedTolerance = 2.0f * kQuantizedStep + kQuantizedStep * kQuantizedStep;
  return kQuantizedTolerance;
}

TEST_F(DivTest, Float)
{
  Shape base_shape = {2, 3, 1, 1};

  std::vector<int32_t> output_shape = {2, 3, 1, 1};

  std::vector<float> input1_data{0.3f, 2.3f, 0.9f, 0.5f, 0.8f, 1.1f};
  std::vector<float> input2_data{0.2f, 1.6f, 0.5f, 0.4f, 1.6f, 0.4f};
  std::vector<float> test_outputs{1.5f, 1.4375f, 1.8f, 1.25f, 0.5f, 2.75f};

  Tensor input1_tensor =
    makeInputTensor<DataType::FLOAT32>(base_shape, input1_data, _memory_manager.get());
  Tensor input2_tensor =
    makeInputTensor<DataType::FLOAT32>(base_shape, input2_data, _memory_manager.get());

  Tensor output_tensor = makeOutputTensor(DataType::FLOAT32);

  DivParams params{};
  params.activation = Activation::RELU;

  Div kernel(&input1_tensor, &input2_tensor, &output_tensor, params);
  kernel.configure();
  _memory_manager->allocate_memory(output_tensor);
  kernel.execute();

  EXPECT_THAT(extractTensorData<float>(output_tensor), FloatArrayNear(test_outputs, 0.0001f));
  EXPECT_THAT(extractTensorShape(output_tensor), ::testing::ElementsAreArray(output_shape));
}

TEST_F(DivTest, FloatBroadcast)
{
  Shape input1_shape = {1, 3};
  Shape input2_shape = {3, 1};

  std::vector<float> input1_data{-0.3f, 2.3f, 0.9f};
  std::vector<float> input2_data{0.2f, 1.6f, 0.5f};
  std::vector<float> test_outputs{0.f, 11.5f, 4.5f, 0.f, 1.4375f, 0.5625f, 0.f, 4.6f, 1.8f};

  Tensor input1_tensor =
    makeInputTensor<DataType::FLOAT32>(input1_shape, input1_data, _memory_manager.get());
  Tensor input2_tensor =
    makeInputTensor<DataType::FLOAT32>(input2_shape, input2_data, _memory_manager.get());

  Tensor output_tensor = makeOutputTensor(DataType::FLOAT32);

  DivParams params{};
  params.activation = Activation::RELU;

  Div kernel(&input1_tensor, &input2_tensor, &output_tensor, params);
  kernel.configure();
  _memory_manager->allocate_memory(output_tensor);
  kernel.execute();

  EXPECT_THAT(extractTensorData<float>(output_tensor), FloatArrayNear(test_outputs, 0.0001f));
}

TEST_F(DivTest, Uint8)
{
  Shape base_shape = {1, 2, 2, 1};

  std::vector<int32_t> output_shape = {1, 2, 2, 1};

  std::vector<float> input1_data = {-0.8f, -0.2f, 0.3f, 0.7f};
  std::vector<float> input2_data = {-0.8f, 0.4f, 0.8f, 1.0f};
  std::vector<float> test_outputs{1.0f, 0.f, 0.375f, 0.7f};

  const float kQuantizedTolerance = GetTolerance(-1.0, 1.0);

  std::pair<float, int32_t> quant_param = quantizationParams<uint8_t>(-1.f, 1.f);

  Tensor input1_tensor = makeInputTensor<DataType::U8>(
    base_shape, quant_param.first, quant_param.second, input1_data, _memory_manager.get());
  Tensor input2_tensor = makeInputTensor<DataType::U8>(
    base_shape, quant_param.first, quant_param.second, input2_data, _memory_manager.get());

  Tensor output_tensor =
    makeOutputTensor(getElementType<uint8_t>(), quant_param.first, quant_param.second);

  DivParams params{};
  params.activation = Activation::RELU;

  Div kernel(&input1_tensor, &input2_tensor, &output_tensor, params);
  kernel.configure();
  _memory_manager->allocate_memory(output_tensor);
  kernel.execute();

  EXPECT_THAT(dequantizeTensorData(output_tensor),
              FloatArrayNear(test_outputs, kQuantizedTolerance));
  EXPECT_THAT(extractTensorShape(output_tensor), ::testing::ElementsAreArray(output_shape));
}

template <loco::DataType DType> void checkInteger(luci_interpreter::IMemoryManager *memory_manager)
{
  using dtype = typename loco::DataTypeImpl<DType>::Type;
  Shape base_shape = {2, 3, 1, 2};
  std::vector<Shape> test_shapes{{1, 1, 3, 2}, {1, 3, 1, 2}, {2, 1, 3, 1}, {2, 3, 1, 1}};

  std::vector<std::vector<dtype>> test_outputs = {{5,  6,  2, 0,  10, 3, //
                                                   10, 0,  4, 5,  20, 0, //
                                                   0,  0,  0, 2,  0,  0, //
                                                   2,  0,  1, 10, 5,  0, //
                                                   2,  3,  1, 0,  5,  1, //
                                                   18, 20, 7, 0,  37, 10},
                                                  {5, 6, 4, 5, 0, 0, 2, 0, 1, 0, 37, 10},
                                                  {5, 7, 4, 6, 2, 3, 10, 0,  8,  0,  4, 0,
                                                   0, 0, 0, 0, 0, 0, 0,  10, 5,  0,  1, 0,
                                                   0, 0, 5, 9, 1, 1, 0,  0,  37, 50, 7, 10},
                                                  {5, 7, 8, 0, 0, 0, 0, 10, 5, 9, 7, 10}};
  std::vector<dtype> input1_data{20, 30, 40, -17, -4, -7, 11, -31, 10, 19, 75, 100};
  std::vector<dtype> input2_data{4, 5, 10, -3, 2, 10};
  for (size_t i = 0; i < test_shapes.size(); ++i)
  {
    Tensor input1_tensor = makeInputTensor<DType>(base_shape, input1_data, memory_manager);
    Tensor input2_tensor = makeInputTensor<DType>(test_shapes[i], input2_data, memory_manager);
    Tensor output_tensor = makeOutputTensor(DType);

    DivParams params{};
    params.activation = Activation::RELU;

    Div kernel(&input1_tensor, &input2_tensor, &output_tensor, params);
    kernel.configure();
    memory_manager->allocate_memory(output_tensor);
    kernel.execute();

    EXPECT_THAT(extractTensorData<dtype>(output_tensor), test_outputs[i])
      << "With shape number " << i;
  }
}

TEST_F(DivTest, SInt64)
{
  checkInteger<loco::DataType::S64>(_memory_manager.get());
  SUCCEED();
}

TEST_F(DivTest, SInt32)
{
  checkInteger<loco::DataType::S32>(_memory_manager.get());
  SUCCEED();
}

TEST_F(DivTest, Input_Output_Type_NEG)
{
  Tensor input1_tensor = makeInputTensor<DataType::FLOAT32>({1}, {1.f}, _memory_manager.get());
  Tensor input2_tensor = makeInputTensor<DataType::S32>({1}, {2}, _memory_manager.get());
  Tensor output_tensor = makeOutputTensor(DataType::FLOAT32);

  DivParams params{};
  params.activation = Activation::RELU;

  Div kernel(&input1_tensor, &input2_tensor, &output_tensor, params);
  EXPECT_ANY_THROW(kernel.configure());
}

TEST_F(DivTest, Invalid_Input_Type_NEG)
{
  Tensor input1_tensor = makeInputTensor<DataType::U64>({1}, {1}, _memory_manager.get());
  Tensor input2_tensor = makeInputTensor<DataType::U64>({1}, {2}, _memory_manager.get());
  Tensor output_tensor = makeOutputTensor(DataType::U64);

  DivParams params{};
  params.activation = Activation::RELU;

  Div kernel(&input1_tensor, &input2_tensor, &output_tensor, params);
  kernel.configure();
  _memory_manager->allocate_memory(output_tensor);
  EXPECT_ANY_THROW(kernel.execute());
}

TEST_F(DivTest, Invalid_Output_Type_NEG)
{
  Tensor input1_tensor = makeInputTensor<DataType::S32>({1}, {1}, _memory_manager.get());
  Tensor input2_tensor = makeInputTensor<DataType::S32>({1}, {2}, _memory_manager.get());
  Tensor output_tensor = makeOutputTensor(DataType::S64);

  DivParams params{};
  params.activation = Activation::RELU;

  Div kernel(&input1_tensor, &input2_tensor, &output_tensor, params);
  EXPECT_ANY_THROW(kernel.configure());
}

} // namespace
} // namespace kernels
} // namespace luci_interpreter
