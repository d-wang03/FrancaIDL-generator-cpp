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
#include "FDModel/FDFactory.h"
#include "FDParser/FDParser.h"
#include "utilities/string_utility.h"
#include <gtest/gtest.h>

using namespace BstIdl;
TEST(FDParserTest, emptyString)
{
    auto ret = FDParser::parseString("");
    EXPECT_TRUE(ret == nullptr);
}

TEST(FDParserTest, packageDecl)
{
    auto ret = FDParser::parseString("package com.bst.idl.parser.test");
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
}

TEST(FDParserTest, importDecl)
{
    auto content = R"(package com.bst.idl.parser.test
    import "../idlFile/hello.fidl"
    import "../idlFile/world.fdepl"
    )";
    BstIdl::FModelManager &idl_mgr = BstIdl::FModelManager::getInstance();
    idl_mgr.clear();
    auto ret = FDParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    FDModelManager &depl_mgr = FDModelManager::getInstance();
    EXPECT_TRUE(contains(idl_mgr.getPendingFidls().front().c_str(), "hello.fidl"));
    EXPECT_TRUE(contains(depl_mgr.getPendingModels().front().c_str(), "world.fdepl"));
}

TEST(FDParserTest, specification)
{
    auto content = R"(package com.bst.idl.parser.test
    import "../idlFile/hello.fidl"
    specification org.deployspecs.SampleDeploySpec {
        for methods {
            CallSemantics: {synchronous, asynchronous} (default: asynchronous);
            Priority: {low, medium, high} (default: low);
            IsOptional: Boolean;
            Range: Integer[] (default: {1,2,3});
        }
        }
    )";
    BstIdl::FModelManager &idl_mgr = BstIdl::FModelManager::getInstance();
    idl_mgr.clear();
    auto ret = FDParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");

    FDModelManager &depl_mgr = FDModelManager::getInstance();
    EXPECT_TRUE(contains(idl_mgr.getPendingFidls().front().c_str(), "hello.fidl"));
    auto specifications = ret->getSpecifications();
    EXPECT_EQ(specifications.size(), 1U);
    auto specification = specifications.front();
    EXPECT_STREQ(specification->getName().c_str(), "org.deployspecs.SampleDeploySpec");
    auto declarations = specification->getDeclarations();
    EXPECT_EQ(declarations.size(), 1U);
    auto declaration = declarations.front();
    EXPECT_STREQ(declaration->getHost()->getBuiltIn()->getName().c_str(), "methods");
    auto properties = declaration->getProperties();
    EXPECT_EQ(properties.size(), 4U);
    auto callSemantics = properties.front();
    EXPECT_STREQ(callSemantics->getName().c_str(), "CallSemantics");
    auto type = callSemantics->getType()->getComplex();
    EXPECT_TRUE(type != nullptr);
    auto enumrators = std::dynamic_pointer_cast<FDEnumType>(type)->getEnumerators();
    EXPECT_EQ(enumrators.size(), 2U);
    EXPECT_STREQ(enumrators.front()->getName().c_str(), "synchronous");
    enumrators.pop_front();
    EXPECT_STREQ(enumrators.front()->getName().c_str(), "asynchronous");
}

TEST(FDParserTest, deployment)
{
    std::string name = "../idlFile/example2.fdepl";
    FDParser parser;
    auto ret = parser.parse(name);
    EXPECT_EQ(ret.size(), 2U);
    auto first = ret.front();
    EXPECT_TRUE(first != nullptr);
    EXPECT_STREQ(first->getName().c_str(), "example2.fdepl");
    auto imports = first->getImports();
    EXPECT_EQ(imports.size(), 2U);
    auto import1 = imports.front();
    EXPECT_TRUE(import1 != nullptr);
    EXPECT_TRUE(contains(import1->getImportURI().c_str(), "/idlFile/example1.fdepl"));
    auto import2 = imports.back();
    EXPECT_TRUE(contains(import2->getImportURI().c_str(), "/idlFile/example.fidl"));
    auto deployment = first->getDeployments().front();
    auto interface_dep = std::dynamic_pointer_cast<FDInterface>(deployment);
    EXPECT_TRUE(interface_dep != nullptr);
    auto spec = interface_dep->getSpec();
    EXPECT_TRUE(spec != nullptr);
    EXPECT_STREQ(spec->getName().c_str(), "examples.SimpleSpec");
    auto declaration = spec->getDeclarations().front();
    EXPECT_TRUE(declaration != nullptr);
    auto host = declaration->getHost();
    EXPECT_TRUE(host != nullptr);
    auto host_name = host->getBuiltIn()->getName().c_str();
    EXPECT_STREQ(host_name, "strings");
    auto properties = declaration->getProperties();
    EXPECT_EQ(properties.size(), 1U);
    auto Encoding = properties.front();
    EXPECT_STREQ(Encoding->getName().c_str(), "Encoding");
    auto type = Encoding->getType()->getComplex();
    EXPECT_TRUE(type != nullptr);
    auto enumrators = std::dynamic_pointer_cast<FDEnumType>(type)->getEnumerators();
    EXPECT_EQ(enumrators.size(), 3U);
    EXPECT_STREQ(enumrators.front()->getName().c_str(), "utf8");
    enumrators.pop_front();
    EXPECT_STREQ(enumrators.front()->getName().c_str(), "utf16");
    enumrators.pop_front();
    EXPECT_STREQ(enumrators.front()->getName().c_str(), "unicode");
    auto flag = Encoding->getFlags().front();
    EXPECT_TRUE(flag != nullptr);
    auto defaultValue = flag->getDefault()->getSingle();
    EXPECT_TRUE(defaultValue != nullptr);
    auto enumFlag = std::dynamic_pointer_cast<FDGeneric>(defaultValue);
    EXPECT_TRUE(enumFlag != nullptr);
    auto value = enumFlag->getValue();
    EXPECT_TRUE(std::dynamic_pointer_cast<FDEnumerator>(value) != nullptr);
    EXPECT_STREQ(std::dynamic_pointer_cast<FDEnumerator>(value)->getName().c_str(), "utf8");
    auto target = interface_dep->getTarget();
    EXPECT_TRUE(target != nullptr);
    EXPECT_STREQ(target->getName().c_str(), "MediaPlayer");
    auto types = target->getTypes();
    EXPECT_EQ(types.size(), 2U);
    auto first_type = types.front();
    EXPECT_TRUE(first_type != nullptr);
    auto first_type_name = first_type->getName();
    EXPECT_STREQ(first_type_name.c_str(), "Person");
    auto last = types.back();
    EXPECT_TRUE(last != nullptr);
    auto last_type_name = last->getName();
    EXPECT_STREQ(last_type_name.c_str(), "TrackInfo");
    auto interface_dep_types = interface_dep->getTypes();
    EXPECT_EQ(interface_dep_types.size(), 2U);
    auto types_one = interface_dep_types.front();
    auto struct_one = std::dynamic_pointer_cast<FDStruct>(types_one);
    EXPECT_TRUE(struct_one != nullptr);
    auto target_one = struct_one->getTarget();
    EXPECT_TRUE(target_one != nullptr);
    auto target_one_name = target_one->getName();
    EXPECT_STREQ(target_one_name.c_str(), "Person");
}