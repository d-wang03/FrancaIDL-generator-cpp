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
#include <gtest/gtest.h>

using namespace BstIdl;

TEST(ParserTest, emptyString)
{
    auto ret = FidlParser::parseString("");
    EXPECT_TRUE(ret == nullptr);
}

TEST(ParserTest, packageDecl)
{
    auto ret = FidlParser::parseString("package com.bst.idl.parser.test");
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
}

TEST(ParserTest, packageDeclSingleID)
{
    auto ret = FidlParser::parseString("package com");
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com");
}

TEST(ParserTest, signalLineComment)
{
    auto ret = FidlParser::parseString("package com.bst.idl.parser.test\n//this is comment.\r\n");
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
}

TEST(ParserTest, importFQNPlus)
{
    auto ret = FidlParser::parseString(
        "package com.bst.idl.parser.test\nimport com.bst.common.* from \"../idlFile/common.fidl\"\n");
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    auto imports = ret->getImports();
    EXPECT_EQ(imports.size(), 1U);
    auto import = imports.front();
    EXPECT_TRUE(import != nullptr);
    EXPECT_STREQ(import->getImportedNamespace().c_str(), "com.bst.common.*");
    EXPECT_TRUE(contains(import->getImportURI().c_str(), "common.fidl"));
}

TEST(ParserTest, importFQN)
{
    auto ret = FidlParser::parseString(
        "package com.bst.idl.parser.test\nimport com.bst.common from \"../idlFile/common.fidl\"\n");
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    auto imports = ret->getImports();
    EXPECT_EQ(imports.size(), 1U);
    auto import = imports.front();
    EXPECT_TRUE(import != nullptr);
    EXPECT_STREQ(import->getImportedNamespace().c_str(), "com.bst.common");
    EXPECT_TRUE(contains(import->getImportURI().c_str(), "common.fidl"));
}

TEST(ParserTest, importID)
{
    auto ret = FidlParser::parseString("package com.bst.idl.parser.test\nimport com from \"../idlFile/common.fidl\"\n");
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    auto imports = ret->getImports();
    EXPECT_EQ(imports.size(), 1U);
    auto import = imports.front();
    EXPECT_TRUE(import != nullptr);
    EXPECT_STREQ(import->getImportedNamespace().c_str(), "com");
    EXPECT_TRUE(contains(import->getImportURI().c_str(), "common.fidl"));
}

TEST(ParserTest, importModel)
{
    auto ret = FidlParser::parseString("package com.bst.idl.parser.test\nimport model \"../idlFile/common.fidl\"\n");
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    auto imports = ret->getImports();
    EXPECT_EQ(imports.size(), 1U);
    auto import = imports.front();
    EXPECT_TRUE(import != nullptr);
    EXPECT_TRUE(import->getImportedNamespace().empty());
    EXPECT_TRUE(contains(import->getImportURI().c_str(), "common.fidl"));
}

TEST(ParserTest, importModelSignleQuot)
{
    auto ret = FidlParser::parseString("package com.bst.idl.parser.test\nimport model '../idlFile/common.fidl'\n");
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    auto imports = ret->getImports();
    EXPECT_EQ(imports.size(), 1U);
    auto import = imports.front();
    EXPECT_TRUE(import != nullptr);
    EXPECT_TRUE(import->getImportedNamespace().empty());
    EXPECT_TRUE(contains(import->getImportURI().c_str(), "common.fidl"));
}

TEST(ParserTest, typeCollection_empty)
{
    auto ret = FidlParser::parseString("package com.bst.idl.parser.test\ntypeCollection MyCollection {}\n");
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 1U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    EXPECT_STREQ(typeCollection->getName().c_str(), "MyCollection");
}

TEST(ParserTest, typeCollection_annnotation_singleline)
{
    auto ret = FidlParser::parseString("package com.bst.idl.parser.test\n<** @description: this is a type collection. "
                                       "**>\ntypeCollection MyCollection {}\n");
    EXPECT_TRUE(ret != nullptr);
    if (ret == nullptr)
        return;
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 1U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    EXPECT_STREQ(typeCollection->getName().c_str(), "MyCollection");
    auto annotations = typeCollection->getComment();
    EXPECT_EQ(annotations->getElements().size(), 1);
    auto annotation = annotations->getElements().front();
    EXPECT_STREQ(annotation->getType().getName().c_str(), "description");
}

TEST(ParserTest, typeCollection_annnotation_multiline)
{
    auto ret = FidlParser::parseString("package com.bst.idl.parser.test\n<** @description: this is a\n\t\\@\\*type "
                                       "collection. **>\ntypeCollection MyCollection {}\n");
    EXPECT_TRUE(ret != nullptr);
    if (ret == nullptr)
        return;
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 1U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    EXPECT_STREQ(typeCollection->getName().c_str(), "MyCollection");
    auto annotations = typeCollection->getComment();
    EXPECT_EQ(annotations->getElements().size(), 1);
    auto annotation = annotations->getElements().front();
    EXPECT_STREQ(annotation->getType().getName().c_str(), "description");
}

TEST(ParserTest, typeCollection_annnotation_invalid)
{
    auto ret = FidlParser::parseString("package com.bst.idl.parser.test\n<** @parameter: this is a\n\t\\@\\*type "
                                       "collection. **>\ntypeCollection MyCollection {}\n");
    EXPECT_TRUE(ret == nullptr);
}

TEST(ParserTest, typeCollection_annnotation_multiAnno)
{
    auto content = R"(package com.bst.idl.parser.test
    <** @description: this is a type collection. 
        @author: ding.wang
        @see: type collection
    **>
    typeCollection MyCollection {}
    )";

    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    if (ret == nullptr)
        return;
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 1U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    EXPECT_STREQ(typeCollection->getName().c_str(), "MyCollection");
    auto annotations = typeCollection->getComment();
    EXPECT_EQ(annotations->getElements().size(), 3U);
    auto iter = annotations->getElements().cbegin();
    EXPECT_STREQ((*iter)->getType().getName().c_str(), "description");
    ++iter;
    EXPECT_STREQ((*iter)->getType().getName().c_str(), "author");
    ++iter;
    EXPECT_STREQ((*iter)->getType().getName().c_str(), "see");
}

TEST(ParserTest, typeCollection_version)
{
        auto content = R"(package com.bst.idl.parser.test
        typeCollection MyCollection 
        {
            version { major 12 minor 109}
        }
        )";

    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    if (ret == nullptr)
        return;
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 1U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    EXPECT_STREQ(typeCollection->getName().c_str(), "MyCollection");
    auto version = typeCollection->getVersion();
    EXPECT_TRUE(version != nullptr);
    EXPECT_EQ(version->getMajor(), 12U);
    EXPECT_EQ(version->getMinor(), 109U);
}

TEST(ParserTest, intAssign)
{
    auto content = R"(package com.bst.idl.parser.test
    typeCollection MyCollection 
    {
       const UInt64 y =5
    }
    )";

    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 1U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    auto constdefs = typeCollection->getConstants();
    EXPECT_EQ(constdefs.size(), 1U);
    auto constdef = constdefs.front();
    auto exp = constdef->getRhs();
    ASSERT_EQ(std::static_pointer_cast<FIntegerConstant>(exp)->getUnsignedVal(), 5);
}

TEST(ParserTest, unaryIntAssign)
{
    auto content = R"(package com.bst.idl.parser.test
    typeCollection MyCollection 
    {
       const UInt64 y =-5
    }
    )";

    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 1U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    auto constdefs = typeCollection->getConstants();
    EXPECT_EQ(constdefs.size(), 1U);
    auto constdef = constdefs.front();
    auto exp = constdef->getRhs();
    auto uexp = std::static_pointer_cast<FUnaryOperation>(exp);
    ASSERT_EQ(std::static_pointer_cast<FIntegerConstant>(uexp->getOperand())->getUnsignedVal(), 5);
    EXPECT_STREQ((uexp->getOp()->getLiteral().c_str()), "-");
}

TEST(ParserTest, binaryIntAssign)
{
    auto content = R"(package com.bst.idl.parser.test
    typeCollection MyCollection 
    {
       const UInt64 y =5+-4*8
    }
    )";

    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 1U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    auto constdefs = typeCollection->getConstants();
    EXPECT_EQ(constdefs.size(), 1U);
    auto constdef = constdefs.front();
    auto exp = constdef->getRhs();
    auto bop = std::static_pointer_cast<FBinaryOperation>(exp);
    auto lexp = (bop->getLeft());
    ASSERT_EQ(std::static_pointer_cast<FIntegerConstant>(lexp)->getUnsignedVal(), 5);
    auto rexp = (bop->getRight());
    EXPECT_TRUE(rexp != nullptr);
    auto rbop = std::static_pointer_cast<FBinaryOperation>(rexp);
    auto rbopl = (rbop->getLeft());
    EXPECT_TRUE(rbopl != nullptr);
    auto uexp = std::static_pointer_cast<FUnaryOperation>(rbopl);
    ASSERT_EQ(std::static_pointer_cast<FIntegerConstant>(uexp->getOperand())->getUnsignedVal(), 4);
    EXPECT_STREQ((uexp->getOp()->getLiteral().c_str()), "-");
    auto rbopr = (rbop->getRight());
    EXPECT_TRUE(rbopr != nullptr);
    ASSERT_EQ(std::static_pointer_cast<FIntegerConstant>(rbopr)->getUnsignedVal(), 8);
}

TEST(ParserTest, stringConst)
{
    auto content = R"(package com.bst.idl.parser.test
    typeCollection MyCollection 
    {
        const String tom="I have an apple."
    }
    )";
    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 1U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    auto constdefs = typeCollection->getConstants();
    EXPECT_EQ(constdefs.size(), 1U);
    auto constdef = constdefs.front();
    auto exp = constdef->getRhs();
    EXPECT_STREQ(std::static_pointer_cast<FStringConstant>(exp)->getVal().c_str(), "I have an apple.");
}

TEST(ParserTest, typedefConst)
{
    auto content = R"(package com.bst.idl.parser.test
    typeCollection MyCollection 
    {
        typedef MyInt16 is Int16
        const MyInt16 tom = 10
    }
    )";
    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 1U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    auto constdefs = typeCollection->getConstants();
    EXPECT_EQ(constdefs.size(), 1U);
    auto constdef = constdefs.front();
    EXPECT_STREQ(constdef->getType()->getDerived()->getName().c_str(), "MyInt16");
}

TEST(ParserTest, arrayConst)
{
    auto content = R"(package com.bst.idl.parser.test
    typeCollection MyCollection 
    {
        array MyInt16Array of Int16
        const MyInt16Array tom = []
    }
    )";
    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 1U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    auto constdefs = typeCollection->getConstants();
    EXPECT_EQ(constdefs.size(), 1U);
    auto constdef = constdefs.front();
    EXPECT_STREQ(constdef->getType()->getDerived()->getName().c_str(), "MyInt16Array");
}

TEST(ParserTest, typedef)
{
    auto content = R"(package com.bst.idl.parser.test
    typeCollection MyCollection 
    {
        typedef MyInt16 is Int16
    }
    )";

    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    if (ret == nullptr)
        return;
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 1U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    EXPECT_STREQ(typeCollection->getName().c_str(), "MyCollection");
    auto types = typeCollection->getTypes();
    EXPECT_EQ(types.size(), 1U);
    auto type = std::dynamic_pointer_cast<FTypeDef>(types.front());
    EXPECT_TRUE(type != nullptr);
    EXPECT_STREQ(type->getName().c_str(), "MyInt16");
    EXPECT_EQ(type->getActualType()->getPredefined()->getValue(), FBasicTypeId::INT16);
}

TEST(ParserTest, typedef_public)
{
    auto content = R"(package com.bst.idl.parser.test
    typeCollection MyCollection 
    {
        public typedef MyInt16 is Int16
    }
    )";

    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    if (ret == nullptr)
        return;
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 1U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    EXPECT_STREQ(typeCollection->getName().c_str(), "MyCollection");
    auto types = typeCollection->getTypes();
    EXPECT_EQ(types.size(), 1U);
    auto type = std::dynamic_pointer_cast<FTypeDef>(types.front());
    EXPECT_TRUE(type != nullptr);
    EXPECT_STREQ(type->getName().c_str(), "MyInt16");
    EXPECT_TRUE(type->isPublic());
    EXPECT_EQ(type->getActualType()->getPredefined()->getValue(), FBasicTypeId::INT16);
}

TEST(ParserTest, typedef_public_anno)
{
    auto content = R"(package com.bst.idl.parser.test
    typeCollection MyCollection 
    {
        <**@description: this is MyInt16. **>
        public typedef MyInt16 is Int16
    }
    )";

    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    if (ret == nullptr)
        return;
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 1U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    EXPECT_STREQ(typeCollection->getName().c_str(), "MyCollection");
    auto types = typeCollection->getTypes();
    EXPECT_EQ(types.size(), 1U);
    auto type = std::dynamic_pointer_cast<FTypeDef>(types.front());
    EXPECT_TRUE(type != nullptr);
    EXPECT_STREQ(type->getName().c_str(), "MyInt16");
    EXPECT_TRUE(type->isPublic());
    EXPECT_EQ(type->getActualType()->getPredefined()->getValue(), FBasicTypeId::INT16);
    auto comments = type->getComment();
    EXPECT_TRUE(comments != nullptr);
    auto annos = comments->getElements();
    EXPECT_EQ(annos.size(), 1U);
    EXPECT_STREQ(annos.front()->getType().getName().c_str(), "description");
}

TEST(ParserTest, typedef_anno)
{
    auto content = R"(package com.bst.idl.parser.test
    typeCollection MyCollection 
    {
        <**@description: this is MyInt16. **>
        typedef MyInt16 is Int16
    }
    )";

    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    if (ret == nullptr)
        return;
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 1U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    EXPECT_STREQ(typeCollection->getName().c_str(), "MyCollection");
    auto types = typeCollection->getTypes();
    EXPECT_EQ(types.size(), 1U);
    auto type = std::dynamic_pointer_cast<FTypeDef>(types.front());
    EXPECT_TRUE(type != nullptr);
    EXPECT_STREQ(type->getName().c_str(), "MyInt16");
    EXPECT_EQ(type->getActualType()->getPredefined()->getValue(), FBasicTypeId::INT16);
    auto comments = type->getComment();
    EXPECT_TRUE(comments != nullptr);
    auto annos = comments->getElements();
    EXPECT_EQ(annos.size(), 1U);
    EXPECT_STREQ(annos.front()->getType().getName().c_str(), "description");
}

TEST(ParserTest, array)
{
    auto content = R"(package com.bst.idl.parser.test
    typeCollection MyCollection 
    {
        array MyInt16Array of Int16
    }
    )";

    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    if (ret == nullptr)
        return;
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 1U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    EXPECT_STREQ(typeCollection->getName().c_str(), "MyCollection");
    auto types = typeCollection->getTypes();
    EXPECT_EQ(types.size(), 1U);
    auto type = std::dynamic_pointer_cast<FArrayType>(types.front());
    EXPECT_TRUE(type != nullptr);
    EXPECT_STREQ(type->getName().c_str(), "MyInt16Array");
    EXPECT_EQ(type->getElementType()->getPredefined()->getValue(), FBasicTypeId::INT16);
}

TEST(ParserTest, array_public)
{
    auto content = R"(package com.bst.idl.parser.test
    typeCollection MyCollection 
    {
        public array MyInt16Array of Int16
    }
    )";

    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    if (ret == nullptr)
        return;
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 1U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    EXPECT_STREQ(typeCollection->getName().c_str(), "MyCollection");
    auto types = typeCollection->getTypes();
    EXPECT_EQ(types.size(), 1U);
    auto type = std::dynamic_pointer_cast<FArrayType>(types.front());
    EXPECT_TRUE(type != nullptr);
    EXPECT_STREQ(type->getName().c_str(), "MyInt16Array");
    EXPECT_TRUE(type->isPublic());
    EXPECT_EQ(type->getElementType()->getPredefined()->getValue(), FBasicTypeId::INT16);
}

TEST(ParserTest, array_public_anno)
{
    auto content = R"(package com.bst.idl.parser.test
    typeCollection MyCollection 
    {
        <**@description: this is MyInt16Array. **>
        public array MyInt16Array of Int16
    }
    )";

    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    if (ret == nullptr)
        return;
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 1U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    EXPECT_STREQ(typeCollection->getName().c_str(), "MyCollection");
    auto types = typeCollection->getTypes();
    EXPECT_EQ(types.size(), 1U);
    auto type = std::dynamic_pointer_cast<FArrayType>(types.front());
    EXPECT_TRUE(type != nullptr);
    EXPECT_STREQ(type->getName().c_str(), "MyInt16Array");
    EXPECT_TRUE(type->isPublic());
    EXPECT_EQ(type->getElementType()->getPredefined()->getValue(), FBasicTypeId::INT16);
    auto comments = type->getComment();
    EXPECT_TRUE(comments != nullptr);
    auto annos = comments->getElements();
    EXPECT_EQ(annos.size(), 1U);
    EXPECT_STREQ(annos.front()->getType().getName().c_str(), "description");
}

TEST(ParserTest, array_anno)
{
    auto content = R"(package com.bst.idl.parser.test
    typeCollection MyCollection 
    {
        <**@description: this is MyInt16Array. **>
        public array MyInt16Array of Int16
    }
    )";

    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    if (ret == nullptr)
        return;
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 1U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    EXPECT_STREQ(typeCollection->getName().c_str(), "MyCollection");
    auto types = typeCollection->getTypes();
    EXPECT_EQ(types.size(), 1U);
    auto type = std::dynamic_pointer_cast<FArrayType>(types.front());
    EXPECT_TRUE(type != nullptr);
    EXPECT_STREQ(type->getName().c_str(), "MyInt16Array");
    EXPECT_EQ(type->getElementType()->getPredefined()->getValue(), FBasicTypeId::INT16);
    auto comments = type->getComment();
    EXPECT_TRUE(comments != nullptr);
    auto annos = comments->getElements();
    EXPECT_EQ(annos.size(), 1U);
    EXPECT_STREQ(annos.front()->getType().getName().c_str(), "description");
}

TEST(ParserTest, struct_predefinedElements)
{
    auto content = R"(package com.bst.idl.parser.test
    typeCollection MyCollection 
    {
        struct MyStruct
        {
            UInt16 m_a
        }
    }
    )";
    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    if (ret == nullptr)
        return;
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 1U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    EXPECT_STREQ(typeCollection->getName().c_str(), "MyCollection");
    auto types = typeCollection->getTypes();
    EXPECT_EQ(types.size(), 1U);
    auto type = std::dynamic_pointer_cast<FStructType>(types.front());
    EXPECT_TRUE(type != nullptr);
    EXPECT_STREQ(type->getName().c_str(), "MyStruct");
    auto elements = type->getElements();
    EXPECT_EQ(elements.size(), 1U);
    auto ele1 = elements.front();
    EXPECT_STREQ(ele1->getName().c_str(), "m_a");
    EXPECT_STREQ(ele1->getType()->getPredefined()->getName().c_str(), "UInt16");
}

TEST(ParserTest, struct_derivedElements)
{
    auto content = R"(package com.bst.idl.parser.test
    typeCollection MyCollection 
    {
        struct MyStruct
        {
            MyStruct2 a
            MyArray b
            MyTypeDef c
            MyEnum d
            MyUnion e
        }

        struct MyStruct2
        {
            UInt16 value
        }

        array MyArray of UInt8

        typedef MyTypeDef is UInt16

        enumeration MyEnum
        {
            ITEM1
            ITEM2
            ITEM3
        }

        union MyUnion
        {
            UInt8 a
            UInt16 b
        }
    }
    )";
    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    if (ret == nullptr)
        return;
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 1U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    EXPECT_STREQ(typeCollection->getName().c_str(), "MyCollection");
    auto types = typeCollection->getTypes();
    EXPECT_EQ(types.size(), 6U);
    auto type = std::dynamic_pointer_cast<FStructType>(types.front());
    EXPECT_TRUE(type != nullptr);
    EXPECT_STREQ(type->getName().c_str(), "MyStruct");
    auto elements = type->getElements();
    EXPECT_EQ(elements.size(), 5U);
    auto iter = elements.cbegin();

    EXPECT_STREQ((*iter)->getName().c_str(), "a");
    EXPECT_STREQ((*iter)->getType()->getDerived()->getName().c_str(), "MyStruct2");
    ++iter;

    EXPECT_STREQ((*iter)->getName().c_str(), "b");
    EXPECT_STREQ((*iter)->getType()->getDerived()->getName().c_str(), "MyArray");
    ++iter;

    EXPECT_STREQ((*iter)->getName().c_str(), "c");
    EXPECT_STREQ((*iter)->getType()->getDerived()->getName().c_str(), "MyTypeDef");
    ++iter;

    EXPECT_STREQ((*iter)->getName().c_str(), "d");
    EXPECT_STREQ((*iter)->getType()->getDerived()->getName().c_str(), "MyEnum");
    ++iter;

    EXPECT_STREQ((*iter)->getName().c_str(), "e");
    EXPECT_STREQ((*iter)->getType()->getDerived()->getName().c_str(), "MyUnion");
    ++iter;
}

TEST(ParserTest, struct_anno)
{
    auto content = R"(package com.bst.idl.parser.test
    typeCollection MyCollection 
    {
        <**@description: this is MyStruct. **>
        struct MyStruct
        {
            <**@description: this is m_a. **>
            UInt16 m_a
        }
    }
    )";
    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    if (ret == nullptr)
        return;
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 1U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    EXPECT_STREQ(typeCollection->getName().c_str(), "MyCollection");
    auto types = typeCollection->getTypes();
    EXPECT_EQ(types.size(), 1U);
    auto type = std::dynamic_pointer_cast<FStructType>(types.front());
    EXPECT_TRUE(type != nullptr);
    EXPECT_STREQ(type->getName().c_str(), "MyStruct");

    auto comments = type->getComment();
    EXPECT_TRUE(comments != nullptr);
    auto annos = comments->getElements();
    EXPECT_EQ(annos.size(), 1U);
    EXPECT_STREQ(annos.front()->getType().getName().c_str(), "description");

    auto elements = type->getElements();
    EXPECT_EQ(elements.size(), 1U);
    auto ele1 = elements.front();
    EXPECT_STREQ(ele1->getName().c_str(), "m_a");
    EXPECT_STREQ(ele1->getType()->getPredefined()->getName().c_str(), "UInt16");

    comments = ele1->getComment();
    EXPECT_TRUE(comments != nullptr);
    annos = comments->getElements();
    EXPECT_EQ(annos.size(), 1U);
    EXPECT_STREQ(annos.front()->getType().getName().c_str(), "description");
}

TEST(ParserTest, struct_extends)
{
    auto content = R"(package com.bst.idl.parser.test
    typeCollection MyCollection 
    {
        struct MyStruct extends MyStruct2
        {
            UInt16 m_a
        }

        struct MyStruct2
        {
            UInt32 mb
        }
    }
    )";
    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    if (ret == nullptr)
        return;
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 1U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    EXPECT_STREQ(typeCollection->getName().c_str(), "MyCollection");
    auto types = typeCollection->getTypes();
    EXPECT_EQ(types.size(), 2U);
    auto type = std::dynamic_pointer_cast<FStructType>(types.front());
    EXPECT_TRUE(type != nullptr);
    EXPECT_STREQ(type->getName().c_str(), "MyStruct");
    auto base = type->getBase();
    EXPECT_TRUE(base != nullptr);
    EXPECT_STREQ(base->getName().c_str(), "MyStruct2");
}

TEST(ParserTest, struct_extends_FQN)
{
    auto content = R"(package com.bst.idl.parser.test
    typeCollection MyCollection 
    {
        struct MyStruct extends MyCollection2.MyStruct2
        {
            UInt16 m_a
        }
    }

    typeCollection MyCollection2 
    {
        struct MyStruct2
        {
            UInt32 mb
        }
    }
    )";
    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    if (ret == nullptr)
        return;
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 2U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    EXPECT_STREQ(typeCollection->getName().c_str(), "MyCollection");
    auto types = typeCollection->getTypes();
    EXPECT_EQ(types.size(), 1U);
    auto type = std::dynamic_pointer_cast<FStructType>(types.front());
    EXPECT_TRUE(type != nullptr);
    EXPECT_STREQ(type->getName().c_str(), "MyStruct");
    auto base = type->getBase();
    EXPECT_TRUE(base != nullptr);
    EXPECT_STREQ(base->getName().c_str(), "MyStruct2");
}

TEST(ParserTest, union)
{
    auto content = R"(package com.bst.idl.parser.test
    typeCollection MyCollection 
    {
        union MyUnion
        {
            UInt16 m_a
        }
    }
    )";
    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    if (ret == nullptr)
        return;
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 1U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    EXPECT_STREQ(typeCollection->getName().c_str(), "MyCollection");
    auto types = typeCollection->getTypes();
    EXPECT_EQ(types.size(), 1U);
    auto type = std::dynamic_pointer_cast<FUnionType>(types.front());
    EXPECT_TRUE(type != nullptr);
    EXPECT_STREQ(type->getName().c_str(), "MyUnion");
}

TEST(ParserTest, union_anno)
{
    auto content = R"(package com.bst.idl.parser.test
    typeCollection MyCollection 
    {
        <**@description: this is MyUnion. **>
        union MyUnion
        {
            <**@description: this is m_a. **>
            UInt16 m_a
        }
    }
    )";
    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    if (ret == nullptr)
        return;
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 1U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    EXPECT_STREQ(typeCollection->getName().c_str(), "MyCollection");
    auto types = typeCollection->getTypes();
    EXPECT_EQ(types.size(), 1U);
    auto type = std::dynamic_pointer_cast<FUnionType>(types.front());
    EXPECT_TRUE(type != nullptr);
    EXPECT_STREQ(type->getName().c_str(), "MyUnion");

    auto comments = type->getComment();
    EXPECT_TRUE(comments != nullptr);
    auto annos = comments->getElements();
    EXPECT_EQ(annos.size(), 1U);
    EXPECT_STREQ(annos.front()->getType().getName().c_str(), "description");

    auto elements = type->getElements();
    EXPECT_EQ(elements.size(), 1U);
    auto element = elements.front();
    EXPECT_STREQ(element->getName().c_str(), "m_a");

    comments = element->getComment();
    EXPECT_TRUE(comments != nullptr);
    annos = comments->getElements();
    EXPECT_EQ(annos.size(), 1U);
    EXPECT_STREQ(annos.front()->getType().getName().c_str(), "description");
}

TEST(ParserTest, union_extends)
{
    auto content = R"(package com.bst.idl.parser.test
    typeCollection MyCollection 
    {
        union MyUnion extends MyUnion2
        {
            UInt16 m_a
        }

        union MyUnion2
        {
            UInt32 mb
        }
    }
    )";
    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    if (ret == nullptr)
        return;
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 1U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    EXPECT_STREQ(typeCollection->getName().c_str(), "MyCollection");
    auto types = typeCollection->getTypes();
    EXPECT_EQ(types.size(), 2U);
    auto type = std::dynamic_pointer_cast<FUnionType>(types.front());
    EXPECT_TRUE(type != nullptr);
    EXPECT_STREQ(type->getName().c_str(), "MyUnion");
    auto base = type->getBase();
    EXPECT_TRUE(base != nullptr);
    EXPECT_STREQ(base->getName().c_str(), "MyUnion2");
}

TEST(ParserTest, union_extends_FQN)
{
    auto content = R"(package com.bst.idl.parser.test
    typeCollection MyCollection 
    {
        union MyUnion extends MyCollection2.MyUnion2
        {
            UInt16 m_a
        }
    }

    typeCollection MyCollection2 
    {
        union MyUnion2
        {
            UInt32 mb
        }
    }
    )";
    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    if (ret == nullptr)
        return;
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 2U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    EXPECT_STREQ(typeCollection->getName().c_str(), "MyCollection");
    auto types = typeCollection->getTypes();
    EXPECT_EQ(types.size(), 1U);
    auto type = std::dynamic_pointer_cast<FUnionType>(types.front());
    EXPECT_TRUE(type != nullptr);
    EXPECT_STREQ(type->getName().c_str(), "MyUnion");
    auto base = type->getBase();
    EXPECT_TRUE(base != nullptr);
    EXPECT_STREQ(base->getName().c_str(), "MyUnion2");
}

TEST(ParserTest, enum)
{
    auto content = R"(package com.bst.idl.parser.test
    typeCollection MyCollection 
    {
        enumeration MyEnum
        {
            VALUE1
        }
    }
    )";
    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    if (ret == nullptr)
        return;
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 1U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    EXPECT_STREQ(typeCollection->getName().c_str(), "MyCollection");
    auto types = typeCollection->getTypes();
    EXPECT_EQ(types.size(), 1U);
    auto type = std::dynamic_pointer_cast<FEnumerationType>(types.front());
    EXPECT_TRUE(type != nullptr);
    EXPECT_STREQ(type->getName().c_str(), "MyEnum");
}

TEST(ParserTest, enum_anno)
{
    auto content = R"(package com.bst.idl.parser.test
    typeCollection MyCollection 
    {
        <**@description: this is MyEnum. **>
        enumeration MyEnum
        {
            <**@description: this is VALUE1. **>
            VALUE1
        }
    }
    )";
    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    if (ret == nullptr)
        return;
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 1U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    EXPECT_STREQ(typeCollection->getName().c_str(), "MyCollection");
    auto types = typeCollection->getTypes();
    EXPECT_EQ(types.size(), 1U);
    auto type = std::dynamic_pointer_cast<FEnumerationType>(types.front());
    EXPECT_TRUE(type != nullptr);
    EXPECT_STREQ(type->getName().c_str(), "MyEnum");

    auto comments = type->getComment();
    EXPECT_TRUE(comments != nullptr);
    auto annos = comments->getElements();
    EXPECT_EQ(annos.size(), 1U);
    EXPECT_STREQ(annos.front()->getType().getName().c_str(), "description");

    auto elements = type->getEnumerators();
    EXPECT_EQ(elements.size(), 1U);
    auto element = elements.front();
    EXPECT_STREQ(element->getName().c_str(), "VALUE1");

    comments = element->getComment();
    EXPECT_TRUE(comments != nullptr);
    annos = comments->getElements();
    EXPECT_EQ(annos.size(), 1U);
    EXPECT_STREQ(annos.front()->getType().getName().c_str(), "description");
}

TEST(ParserTest, enum_extends)
{
    auto content = R"(package com.bst.idl.parser.test
    typeCollection MyCollection 
    {
        enumeration MyEnum extends MyEnum2
        {
            VALUE1
        }

        enumeration MyEnum2
        {
            VALUE2
        }
    }
    )";
    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    if (ret == nullptr)
        return;
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 1U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    EXPECT_STREQ(typeCollection->getName().c_str(), "MyCollection");
    auto types = typeCollection->getTypes();
    EXPECT_EQ(types.size(), 2U);
    auto type = std::dynamic_pointer_cast<FEnumerationType>(types.front());
    EXPECT_TRUE(type != nullptr);
    EXPECT_STREQ(type->getName().c_str(), "MyEnum");
    auto base = type->getBase();
    EXPECT_TRUE(base != nullptr);
    EXPECT_STREQ(base->getName().c_str(), "MyEnum2");
}

TEST(ParserTest, enum_extends_FQN)
{
    auto content = R"(package com.bst.idl.parser.test
    typeCollection MyCollection 
    {
        enumeration MyEnum extends MyCollection2.MyEnum2
        {
            VALUE1
        }
    }

    typeCollection MyCollection2 
    {
        enumeration MyEnum2
        {
            VALUE2
        }
    }
    )";
    auto ret = FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    if (ret == nullptr)
        return;
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.idl.parser.test");
    auto typeCollections = ret->getTypeCollections();
    EXPECT_EQ(typeCollections.size(), 2U);
    auto typeCollection = typeCollections.front();
    EXPECT_TRUE(typeCollection != nullptr);
    EXPECT_STREQ(typeCollection->getName().c_str(), "MyCollection");
    auto types = typeCollection->getTypes();
    EXPECT_EQ(types.size(), 1U);
    auto type = std::dynamic_pointer_cast<FEnumerationType>(types.front());
    EXPECT_TRUE(type != nullptr);
    EXPECT_STREQ(type->getName().c_str(), "MyEnum");
    auto base = type->getBase();
    EXPECT_TRUE(base != nullptr);
    EXPECT_STREQ(base->getName().c_str(), "MyEnum2");
}
