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
#include "../include/asf-tools/GeneralServiceGenerator.h"
#include "FDParser/FDParser.h"
#include "model/FFactory.h"
#include "utilities/string_utility.h"
#include <gtest/gtest.h>
#include <list>

using namespace BstIdl;

TEST(GeneralServiceGenerator, generateDef)
{
    std::string name = "../idlFile/generatorDef.fdepl";
    FDParser parser;
    auto ret = parser.parse(name);
    EXPECT_EQ(ret.size(), 4U);
    ret.pop_back();
    auto last = ret.back();
    EXPECT_TRUE(last != nullptr);
    EXPECT_STREQ(last->getName().c_str(), "asf_gen.fdepl");

    std::string src_gen_dir("./generatorDef/");
    bool generate_stub = true;

    auto generator = std::make_shared<BstASF::GeneralServiceGenerator>(src_gen_dir.append("src-gen"), generate_stub);
    auto res = generator->generate();
    EXPECT_EQ(res, true);
}

TEST(GeneralServiceGenerator, generateDef2)
{
    std::string name = "../idlFile/generatorDef2.fdepl";
    FDParser parser;
    auto ret = parser.parse(name);
    EXPECT_EQ(ret.size(), 2U);

    auto last = ret.back();
    EXPECT_TRUE(last != nullptr);
    EXPECT_STREQ(last->getName().c_str(), "asf_gen.fdepl");

    std::string src_gen_dir("./generatorDef2/");
    bool generate_stub = true;
    auto generator = BstASF::GeneralServiceGenerator(src_gen_dir.append("src-gen"), generate_stub);
    auto res = generator.generate();
    EXPECT_EQ(res, true);
}