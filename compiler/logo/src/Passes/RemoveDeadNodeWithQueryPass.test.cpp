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
#include "luci/IR/DeadNodeQueryService.h"
#include "loco/IR/Dialect.h"

#include <logo/DeadNodeQueryService.h>

#include "loco/IR/CircleNodes.h"

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

TEST(RemoveDeadNodeWithQueryPassTest, run_pos_first_try)
{
//                                       [input]
//                                          |
//                                       [push]
//                  /                 |                  \              \             \ 
//           [dead1]        [virtual_active1]     [virtual_active2]     [dead2]    [active3]
//             it =0               it=1                  it=2             it=3       it=4
//                                                                                    |
//
    loco::Graph g;
    logo::RemoveDeadNodeWithQueryPass pass;

    auto input_node = g.nodes()->create<loco::ConstGen>();
    {
        input_node->dtype(loco::DataType::FLOAT32);
        input_node->rank(1);
        input_node->dim(0) = 10;
        input_node->size<loco::DataType::FLOAT32>(1);
        input_node->at<loco::DataType::FLOAT32>(0) = 1.0f;
    }

    auto push_node = g.nodes()->create<loco::Push>();
    push_node->from(input_node);

    auto dead_node1 = g.nodes()->create<loco::Push>();
    dead_node1->from(push_node);

    auto active_node1 = g.nodes()->create<loco::Push>();
    active_node1->from(push_node);

    auto active_node2 = g.nodes()->create<loco::Push>();
    active_node2->from(push_node);

    auto dead_node2 = g.nodes()->create<loco::Push>();
    dead_node2->from(push_node);

    auto active_node3 = g.nodes()->create<loco::Push>();
    active_node3->from(push_node);

    auto output_node = g.outputs()->create();
    {
        output_node->name("output");
        output_node->dtype(loco::DataType::FLOAT32);
        loco::link(output_node, active_node3);
    }

    if (auto dialect = const_cast<loco::Dialect *>(active_node1->dialect()))
    {
        dialect->service(std::make_unique<luci::DeadNodeQueryServiceImpl>());
    }

    if (auto dialect = const_cast<loco::Dialect *>(active_node2->dialect()))
    {
        dialect->service(std::make_unique<luci::DeadNodeQueryServiceImpl>());
    }

    auto active_nodes = loco::active_nodes(loco::output_nodes(&g));

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

TEST(RemoveDeadNodeWithQueryPassTest, run_pos_with_CircleNode_second_try)
{
//                                 [input]
//                               /            \
//                      [split]              [dead]
//           /             |         \
//    [virtual1]  [active]   [virtual2]
//                          |
//                     [output]
    loco::Graph g;
    logo::RemoveDeadNodeWithQueryPass pass;

    auto input_node = g.nodes()->create<luci::CircleConst>();
    {
        input_node->dtype(loco::DataType::FLOAT32);
        input_node->rank(1);
        input_node->dim(0) = 10;
        input_node->size<loco::DataType::FLOAT32>(1);
        input_node->at<loco::DataType::FLOAT32>(0) = 1.0f;
    }
    luci::CircleSplit split;
    g.nodes()->at(0) = split;

    auto split_node = g.nodes()->create<luci::CircleSplit>();

    auto split_out_node_1 = g.nodes()->create<luci::CircleSplitOut>();
    auto split_out_node_2 = g.nodes()->create<luci::CircleSplitOut>();
    auto split_out_node_3 = g.nodes()->create<luci::CircleSplitOut>();
    loco::Push *dummy;
    split_node->num_split(3);
    split_out_node_1->input(split_node);
    split_out_node_1->index(0);
    split_out_node_2->input(split_node);
    split_out_node_2->index(1);
    split_out_node_3->input(split_node);
    split_out_node_3->index(2);
    auto output_node = g.outputs()->create();
    {
        output_node->name("output");
        output_node->dtype(loco::DataType::FLOAT32);
        loco::link(output_node, dummy);
    }
    auto active_node2 = g.nodes()->create<loco::Push>();
    active_node2->dialect()->service<logo::DeadNodeQueryService>(
        std::make_unique<luci::DeadNodeQueryServiceImpl>());
    auto active_nodes = loco::active_nodes(loco::output_nodes(&g));

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

class CustomDeadNodeQueryServiceImpl : public logo::DeadNodeQueryService
{
public:
    bool isDeadNode(loco::Node *node) override
    {
        auto g = node->graph();
        auto input_nodes_vec = loco::input_nodes(g);
        auto output_nodes_vec = loco::output_nodes(g);

        auto input_nodes = std::set<loco::Node *>(input_nodes_vec.begin(), input_nodes_vec.end());
        auto output_nodes = std::set<loco::Node *>(output_nodes_vec.begin(), output_nodes_vec.end());
        auto active_nodes = loco::active_nodes(output_nodes_vec);

        if (active_nodes.find(node) != active_nodes.end())
            return false;
        if (input_nodes.find(node) != input_nodes.end())
            return false;

        return true;
    }
};

class CustomDialect : public loco::Dialect
{
public:
    template <typename ConcreteService>
    void addService(std::unique_ptr<ConcreteService> &&service)
    {
        this->service(std::move(service));
    }
};

TEST(RemoveDeadNodeWithQueryPassTest, run_pos_with_custom_dialect_Third_try)
{
    loco::Graph g;
    logo::RemoveDeadNodeWithQueryPass pass;

//                                       [input]
//                                          |
//                                       [push]
//                  /                 |                  \              \             \ 
//           [dead1]        [virtual_active1]     [virtual_active2]     [dead2]    [active3]
//             it =0               it=1                  it=2             it=3       it=4
//                                                                                    |
//                                                                                 [output]
    auto input_node = g.nodes()->create<loco::ConstGen>();
    {
        input_node->dtype(loco::DataType::FLOAT32);
        input_node->rank(1);
        input_node->dim(0) = 10;
        input_node->size<loco::DataType::FLOAT32>(1);
        input_node->at<loco::DataType::FLOAT32>(0) = 1.0f;
    }

    auto custom_dialect = std::make_unique<CustomDialect>();
    custom_dialect->addService(std::make_unique<CustomDeadNodeQueryServiceImpl>());

    auto push_node = g.nodes()->create<loco::Push>();
    push_node->from(input_node);

    auto dead_node1 = g.nodes()->create<loco::Push>();
    dead_node1->from(push_node);

    auto active_node1 = g.nodes()->create<loco::Push>();
    active_node1->from(push_node);

    auto active_node2 = g.nodes()->create<loco::Push>();
    active_node2->from(push_node);

    auto dead_node2 = g.nodes()->create<loco::Push>();
    dead_node2->from(push_node);

    auto active_node3 = g.nodes()->create<loco::Push>();
    active_node3->from(push_node);

    auto output_node = g.outputs()->create();
    {
        output_node->name("output");
        output_node->dtype(loco::DataType::FLOAT32);
        loco::link(output_node, active_node3);
    }

    auto active_nodes = loco::active_nodes(loco::output_nodes(&g));

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
        if (candidates.find(node) != candidates.end())
        {
            ASSERT_EQ(candidates.find(node), candidates.end());
        }
    }
}