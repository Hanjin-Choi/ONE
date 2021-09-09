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

#ifndef LUCI_INTERPRETER_LOADER_NODES_BUILDERS_H
#define LUCI_INTERPRETER_LOADER_NODES_BUILDERS_H

#include "loader/KernelBuilderHelper.h"

#include "luci/IR/CircleNodes.h"

namespace luci_interpreter
{

#define REGISTER_KERNEL(name)                                                            \
  std::unique_ptr<Kernel> build_kernel_Circle##name(const luci::CircleNode *circle_node, \
                                                    KernelBuilderHelper &helper);

#include "KernelsToBuild.lst"

#undef REGISTER_KERNEL

} // namespace luci_interpreter

#endif // LUCI_INTERPRETER_LOADER_NODES_BUILDERS_H
