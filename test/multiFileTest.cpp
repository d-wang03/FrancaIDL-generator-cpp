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
#include "parser/FidlParser.h"
#include "utilities/string_utility.h"
#include "gtest/gtest.h"
#include <list>

using namespace BstIdl;

TEST(MultiParserTest, multi_file)
{
    std::string name = "../idlFile/file1.fidl";
    FidlParser parser;
    auto ret = parser.parse(name);
    EXPECT_EQ(ret.size(), 2U);
    auto first = ret.front();
    EXPECT_TRUE(first != nullptr);
    EXPECT_STREQ(first->getName().c_str(), "com.bst.idl.parser.test");
    auto imports = first->getImports();
    EXPECT_EQ(imports.size(), 1U);
    auto import = imports.front();
    EXPECT_TRUE(import != nullptr);
    EXPECT_STREQ(import->getImportedNamespace().c_str(), "com.bst.common");
    EXPECT_TRUE(contains(import->getImportURI().c_str(), "../idlFile/common.fidl"));
    auto last = ret.back();
    EXPECT_TRUE(last != nullptr);
    EXPECT_STREQ(last->getName().c_str(), "com.bst.common");
}