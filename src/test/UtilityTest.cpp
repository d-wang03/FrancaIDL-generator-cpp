/*
 * Copyright (c) 2024 Black Sesame Technologies
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "model/FFactory.h"
#include "model/FModel.h"
#include "model/FModelManager.h"
#include "utilities/list_utility.h"
#include "utilities/path_utility.h"
#include "utilities/string_utility.h"
#include "validator/ReferenceManager.h"
#include <gtest/gtest.h>
#include <string>

// string_utility.h
TEST(StringTest, simplify_null)
{
    EXPECT_TRUE(simplify(std::string("")).empty());
}

TEST(StringTest, simplify_null1)
{
    EXPECT_TRUE(simplify(std::string("   ")).empty());
}

TEST(StringTest, simplify_begin_and_rbegin)
{
    EXPECT_STREQ(simplify(std::string(" l t ")).c_str(), "l t");
}

TEST(StringTest, simplify_duplicated)
{
    EXPECT_STREQ(simplify(std::string("    l  t  ")).c_str(), "l t");
}

TEST(StringTest, replace_all_in_for)
{
    std::string inout("Thisltisltalttestltstring.");
    std::string what("lt");
    std::string with(" ");
    inout = replace_all(inout, what, with);
    EXPECT_STREQ(inout.c_str(), "This is a test string.");
}

TEST(StringTest, join)
{
    std::list<std::string> list = {"This", "is", "a", "test", "string", "."};
    std::string separator(" ");
    EXPECT_STREQ(join(list, separator).c_str(), "This is a test string .");
}

TEST(StringTest, contains_else)
{
    std::string str("This is a test string.");
    std::string what("lt");
    EXPECT_FALSE(contains(str, what));
}

// PATH
TEST(CurrentAppPathTest, getInstance)
{
    auto &path = CurrentAppPath::getInstance();
}

// LIST_UTILITY_H
class ListTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        mgr.clear();
        model = mgr.addModel("com.bst.idl.testList.model1");
    }

    BstIdl::FModelManager &mgr = BstIdl::FModelManager::getInstance();
    BstIdl::FFactory &factory = BstIdl::FFactory::getInstance();
    std::shared_ptr<BstIdl::FModel> model;
};

TEST_F(ListTest, addListItemByName)
{
    auto interface1 = factory.createFInterface();
    interface1->setName("Interface1");
    interface1->setContainer(model);
    addListItemByName(model->getInterfaces(), std::move(interface1));
    EXPECT_EQ(model->getInterfaces().size(), 1);
}
