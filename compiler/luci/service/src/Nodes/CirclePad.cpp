/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd. All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "luci/Service/CircleShapeInference.h"

#include "CircleCloneNode.h"
#include "CircleShapeInferenceHelper.h"
#include "HelperPads.h"

namespace luci
{

luci::CircleNode *CloneNodeLet<CN::OPQR>::visit(const luci::CirclePad *)
{
  return _graph->nodes()->create<luci::CirclePad>();
}

namespace sinf
{

loco::TensorShape Algorithm::visit(const luci::CirclePad *node)
{
  if (node->paddings()->arity() == 0)
  {

    auto paddings = loco::must_cast<luci::CircleConst *>(node->paddings());
    return use_paddings(node, paddings);
  }
  else
  {
    auto paddings = loco::must_cast<luci::CircleNode *>(node->paddings());
    LUCI_ASSERT(paddings->rank() == 2, "paddings should be [n, 2]");
    loco::TensorShape output_shape;
    auto input_shape = luci::shape_get(node->input()).template as<loco::TensorShape>();
    output_shape.rank(input_shape.rank());
    for (int32_t ni = 0; ni < input_shape.rank(); ++ni)
    {
      output_shape.dim(ni).unset();
    }
    return output_shape;
  }
}

} // namespace sinf
} // namespace luci
