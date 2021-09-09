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

#include "Builders.h"

#include "kernels/Pack.h"

namespace luci_interpreter
{

std::unique_ptr<Kernel> build_kernel_CirclePack(const luci::CircleNode *circle_node,
                                                KernelBuilderHelper &helper)
{
  const auto *node = dynamic_cast<const luci::CirclePack *>(circle_node);
  if (node == nullptr)
    throw std::runtime_error("wrong builder for operation");
  assert(node->arity() == node->values_count());

  std::vector<const Tensor *> inputs(node->values_count());
  for (uint32_t i = 0; i < node->values_count(); ++i)
  {
    inputs[i] = helper.getInputTensor(node->values(i));
  }
  Tensor *output = helper.getOutputTensor(node);

  PackParams params{};
  params.axis = node->axis();
  params.values_count = node->values_count();

  return std::make_unique<kernels::Pack>(std::move(inputs), output, params);
}

} // namespace luci_interpreter
