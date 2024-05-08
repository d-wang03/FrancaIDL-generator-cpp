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
#include "generator/HTMLGenerator.h"
#include "model/FFactory.h"
#include "parser/FidlParser.h"
#include "utilities/string_utility.h"
#include <gtest/gtest.h>
#include <list>

using namespace BstIdl;
TEST(DocGeneratorTest, multi_file)
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
    HTMLGenerator generator;
    generator.generate();
}

TEST(DocGeneratorTest, model)
{
    auto content = R"(package com.bst.idl.parser2.test
    <** @description: this is a interface 1. 
        @author: ting.du
        @see: MyInterface1
    **>
    interface MyInterface1 {}
    <** @description: this is a interface 2. 
        @author: ting.du
        @see: MyInterface2
    **>
    interface MyInterface2 {}

    )";

    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    if (ret == nullptr)
        return;
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser2.test");
    auto interfaces = ret->getInterfaces();
    EXPECT_EQ(interfaces.size(), 2U);
    auto interface = interfaces.front();
    EXPECT_TRUE(interface != nullptr);
    EXPECT_STREQ(interface->getName().c_str(), "MyInterface1");
    auto annotations = interface->getComment();
    EXPECT_EQ(annotations->getElements().size(), 3U);
    auto iter = annotations->getElements().cbegin();
    EXPECT_STREQ((*iter)->getType().getName().c_str(), "description");
    ++iter;
    EXPECT_STREQ((*iter)->getType().getName().c_str(), "author");
    ++iter;
    EXPECT_STREQ((*iter)->getType().getName().c_str(), "see");
    HTMLGenerator generator;
    generator.generate();
}

TEST(DocGeneratorTest, interface_version)
{
    auto content = R"(package com.bst.idl.parser3.test
    <** @description: this is a interface 1. 
        @author: ting.du
        @see: MyInterface1
    **>
    interface MyInterface1 {
        version { major 5 minor 0 }
    }

    )";

    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    if (ret == nullptr)
        return;
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser3.test");
    auto interfaces = ret->getInterfaces();
    EXPECT_EQ(interfaces.size(), 1U);
    auto interface = interfaces.front();
    EXPECT_TRUE(interface != nullptr);
    EXPECT_STREQ(interface->getName().c_str(), "MyInterface1");
    auto version = interface->getVersion();
    EXPECT_TRUE(version != nullptr);
    auto major = version->getMajor();
    EXPECT_EQ(major, 5U);
    auto minor = version->getMinor();
    EXPECT_EQ(minor, 0U);
    auto annotations = interface->getComment();
    EXPECT_EQ(annotations->getElements().size(), 3U);
    auto iter = annotations->getElements().cbegin();
    EXPECT_STREQ((*iter)->getType().getName().c_str(), "description");
    ++iter;
    EXPECT_STREQ((*iter)->getType().getName().c_str(), "author");
    ++iter;
    EXPECT_STREQ((*iter)->getType().getName().c_str(), "see");
    HTMLGenerator generator;
    generator.generate();
}

TEST(DocGeneratorTest, interface_attribute)
{
    auto content = R"(package com.bst.idl.parser4.test
    interface MyInterface1 {
        attribute UInt32 someAttribute1
        attribute UInt32[] someAttribute2
        typedef my_type is UInt32
        attribute my_type someAttribute3 readonly
    }

    )";

    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    if (ret == nullptr)
        return;
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser4.test");
    auto interfaces = ret->getInterfaces();
    EXPECT_EQ(interfaces.size(), 1U);
    auto interface = interfaces.front();
    EXPECT_TRUE(interface != nullptr);
    EXPECT_STREQ(interface->getName().c_str(), "MyInterface1");
    auto attributes = interface->getAttributes();
    EXPECT_EQ(attributes.size(), 3U);
    HTMLGenerator generator;
    generator.generate();
}

TEST(DocGeneratorTest, interface_types)
{
    auto content = R"(package com.bst.idl.parser5.test
    interface MyInterface1 {
        <** @description: test arrayType**>
        public array my_array of UInt16
        <** @description: test typedef**>
        typedef my_type is UInt32
        <** @description: test enumType**>
        enumeration my_enum 
        {
            zero = 0
            one
            two
        }
        <** @description: test extend enumType**>
        enumeration my_new_enum extends my_enum
        {
            three
            fore
        }
        <** @description: test structType**>
        struct my_struct
        {
            Float pai
            String name
        }
        <** @description: test unionType**>
        union my_union
        {
            Float pai
            String name
        }
        <** @description: test integerInterval**>
        typedef my_int is Integer(1,20)
        <** @description: test mapType**>
        map my_map {String to UInt16}
    }

    )";
    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    if (ret == nullptr)
        return;
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser5.test");
    auto interfaces = ret->getInterfaces();
    EXPECT_EQ(interfaces.size(), 1U);
    auto interface = interfaces.front();
    EXPECT_TRUE(interface != nullptr);
    EXPECT_STREQ(interface->getName().c_str(), "MyInterface1");
    auto types = interface->getTypes();
    HTMLGenerator generator;
    generator.generate();
}

TEST(DocGeneratorTest, interface_method)
{
    auto content = R"(package com.bst.idl.parser6.test
    interface MyInterface1 {
        method my_method
        {
            in 
            {
                UInt8 id
                String name
            }
            out
            {
                Float hight
                UInt8 age
            }
            error extends GenericErrors{
                DIVISION_BY_ZERO =1 
                OVERFLOW
                UNDERFLOW
            }
        }
        enumeration GenericErrors
        {
            INVALID_PARAMATERS
        }
    }

    )";

    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    if (ret == nullptr)
        return;
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser6.test");
    auto interfaces = ret->getInterfaces();
    EXPECT_EQ(interfaces.size(), 1U);
    auto interface = interfaces.front();
    EXPECT_TRUE(interface != nullptr);
    EXPECT_STREQ(interface->getName().c_str(), "MyInterface1");
    HTMLGenerator generator;
    generator.generate();
}

TEST(DocGeneratorTest, interface_broadcast)
{
    auto content = R"(package com.bst.idl.parser7.test
    interface MyInterface1 {
        broadcast my_broadcast
        {
            out
            {
                Float hight
                UInt8 age
            }
        }
    }

    )";

    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    if (ret == nullptr)
        return;
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser7.test");
    auto interfaces = ret->getInterfaces();
    EXPECT_EQ(interfaces.size(), 1U);
    auto interface = interfaces.front();
    EXPECT_TRUE(interface != nullptr);
    EXPECT_STREQ(interface->getName().c_str(), "MyInterface1");
    HTMLGenerator generator;
    generator.generate();
}