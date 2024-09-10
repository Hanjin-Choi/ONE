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

#include "logo/RemoveDeadNodeWithQueryPass.h"

#include "EmptyTestGraph.h"
#include "logo/DeadNodeQueryService.h"

#include <gtest/gtest.h>

TEST(RemoveDeadNodeWithQueryPassTest, name)
{
  logo::RemoveDeadNodeWithQueryPass pass;
  auto const name = pass.name();
  ASSERT_NE(nullptr, name);
}

TEST(RemoveDeadNodeWithQueryPassTest, run_NEG)
{
  loco::Graph g;
  logo::RemoveDeadNodeWithQueryPass pass;

  logo::create_empty_test_net(&g);

  ASSERT_FALSE(pass.run(&g));
}

TEST(RemoveDeadNodeWithQueryPassTest, run_pos)
{
  loco::Graph g;
  logo::RemoveDeadNodeWithQueryPass pass;

  // 1. 입력 노드 생성
  auto input_node = g.nodes()->create<loco::ConstGen>();
  {
    input_node->dtype(loco::DataType::FLOAT32);
    input_node->rank(1);
    input_node->dim(0) = 10;
    input_node->size<loco::DataType::FLOAT32>(1);
    input_node->at<loco::DataType::FLOAT32>(0) = 1.0f;
  }

  // 2. Push 노드 생성
  auto push_node = g.nodes()->create<loco::Push>();
  push_node->from(input_node);  // 입력 노드에서 Push 노드로 값 전달

  // 3. Dead Node 1 생성 (Push 노드에서 값 전달, 이후 연결 없음)
  auto dead_node1 = g.nodes()->create<loco::Push>();
  dead_node1->from(push_node);  // Push 노드에서 Dead Node 1로 값 전달

  // 4. Active Node 1 생성 (Push 노드에서 값 전달, 다음 노드로 연결)
  auto active_node1 = g.nodes()->create<loco::Push>();
  active_node1->from(push_node);  // Push 노드에서 Active Node 1로 값 전달

  // 5. Active Node 2 생성 (Push 노드에서 값 전달, 다음 출력 노드로 연결)
  auto active_node2 = g.nodes()->create<loco::Push>();
  active_node2->from(push_node);  // Push 노드에서 Active Node 2로 값 전달

  // 6. Dead Node 2 생성 (Push 노드에서 값 전달, 이후 연결 없음)
  auto dead_node2 = g.nodes()->create<loco::Push>();
  dead_node2->from(push_node);  // Push 노드에서 Dead Node 2로 값 전달

  auto active_node3 = g.nodes()->create<loco::Push>();
  active_node3->from(push_node);  // Push 노드의 값을 active_node3로 전달

  // 7. Active Node 1과 Active Node 2 연결
  active_node2->from(active_node1);  // Active Node 1에서 Active Node 2로 값 전달

  // 8. 출력 노드 생성 및 Active Node 2 연결
  auto output_node = g.outputs()->create();
  {
    output_node->name("output");
    output_node->dtype(loco::DataType::FLOAT32);
    loco::link(output_node, active_node3);  // Active Node 2에서 출력 노드로 값 전달
  }
  // 가상 노드 설정: dead_node2, active_node2는 가상 노드로 설정
  if (auto dialect = active_node1->dialect())
  {
    dialect->service(std::make_unique<luci::DeadNodeQueryServiceImpl>());
  }

  if (auto dialect = active_node3->dialect())
  {
    dialect->service(std::make_unique<luci::DeadNodeQueryServiceImpl>());
  }

  auto active_nodes = loco::active_nodes(loco::output_nodes(&g));

  // List dead(= non-active) nodes candidates
  std::set<loco::Node *> candidates;

  for (auto node : loco::all_nodes(&g))
  {
    if (active_nodes.find(node) == active_nodes.end())
    {
      candidates.insert(node);
    }
  }
  pass.run(&g);
  for (auto node : loco::all_nodes(&g))
  {
    ASSERT_EQ(candidates.find(node), candidates.end());
  }
}


