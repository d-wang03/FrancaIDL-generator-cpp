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
#include "model/FModelManager.h"
#include "parser/FidlParser.h"
#include "validator/ReferenceManager.h"
#include <gtest/gtest.h>

using namespace BstIdl;
static void prepare_env()
{
    auto &mgr = FModelManager::getInstance();
    mgr.clear();
    // set model 1
    auto model1 = mgr.addModel("com.bst.idl.test.model1");
    // add TypeCollection1
    auto tc1 = FFactory::getInstance().createFTypeCollection();
    tc1->setName("TypeCollection1");
    addListItem(model1->getTypeCollections(), tc1);
    // add TypeCollection1.Struct1
    auto struct1 = FFactory::getInstance().createFStructType();
    struct1->setName("Struct1");
    addListItem(tc1->getTypes(), struct1);
    // add TypeCollection1.Struct1
    auto struct2 = FFactory::getInstance().createFStructType();
    struct2->setName("Struct2");
    addListItem(tc1->getTypes(), struct2);
    // add TypeCollection1.Struct1.Struct2_1
    auto element1 = FFactory::getInstance().createFField();
    element1->setName("Struct2_1");
    auto typeRef = FFactory::getInstance().createFTypeRef();
    typeRef->setDerived(std::static_pointer_cast<FType>(struct2));
    element1->setType(typeRef);
    addListItem(struct1->getElements(), element1);
    // add TypeCollection1.Struct2.uint8_1
    auto element2 = FFactory::getInstance().createFField();
    element2->setName("uint8_1");
    typeRef = FFactory::getInstance().createFTypeRef();
    typeRef->setPredefined(FBasicTypeId::get("UInt8"));
    element2->setType(typeRef);
    addListItem(struct2->getElements(), element2);

    // add TypeCollection1.Constant1
    auto constant1 = FFactory::getInstance().createFConstantDef();
    constant1->setName("Constant1");
    typeRef = FFactory::getInstance().createFTypeRef();
    typeRef->setDerived(std::static_pointer_cast<FType>(struct1));
    constant1->setType(typeRef);
    addListItem(tc1->getConstants(), constant1);
    // add TypeCollection1.Constant2
    auto constant2 = FFactory::getInstance().createFConstantDef();
    constant2->setName("Constant1");
    typeRef = FFactory::getInstance().createFTypeRef();
    typeRef->setPredefined(FBasicTypeId::get("UInt8"));
    constant2->setType(typeRef);
    auto qer1 = FFactory::getInstance().createFQualifiedElementRef();
    qer1->setElement(std::static_pointer_cast<FEvaluableElement>(constant1));
    auto qer2 = FFactory::getInstance().createFQualifiedElementRef();
    qer2->setQualifier(qer1);
    auto field = FFactory::getInstance().createFField();
    field->setName("Struct2_1");
    qer2->setField(field);
    auto qer3 = FFactory::getInstance().createFQualifiedElementRef();
    qer3->setQualifier(qer2);
    field = FFactory::getInstance().createFField();
    field->setName("uint8_1");
    qer3->setField(field);
    constant2->setRhs(std::static_pointer_cast<FInitializerExpression>(qer3));
    addListItem(tc1->getConstants(), constant2);
}

TEST(InitilizerTest, qre_validate)
{
    prepare_env();
    auto model1 = FModelManager::getInstance().getModelList().front();
    auto tc1 = model1->getTypeCollections().front();
    auto constants = tc1->getConstants();
    auto constant1 = constants.front();
    constants.pop_front();
    auto constant2 = constants.front();
    constants.pop_front();
    bool invalid = false;
    try
    {
        auto typeRef = FFactory::getInstance().createFTypeRef();
        typeRef->setPredefined(FBasicTypeId::get("UInt8"));
        constant2->getRhs()->validate(typeRef, false);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        invalid = true;
    }
    EXPECT_FALSE(invalid);
}

TEST(InitilizerTest, parser_const)
{
    auto content = R"(package com.bst.idl.parser.test
    typeCollection MyCollection 
    {
        struct Struct1
        {
            Struct2 struct2_1
            UInt16  uint16_1
        }
        struct Struct2
        {
            Struct3 struct3_1
        }
        struct Struct3
        {
            UInt8   uint8_1
        }

        const Struct1 struct1_1 = {struct2_1:{struct3_1:{uint8_1:5}},uint16_1:12}
        const UInt16 uint16_1 = struct1_1->struct2_1->struct3_1->uint8_1 + struct1_1->uint16_1
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
    EXPECT_EQ(types.size(), 3U);
    auto constants = typeCollection->getConstants();
    EXPECT_EQ(constants.size(), 2U);
    auto const1 = constants.front();
    constants.pop_front();
    auto const2 = constants.front();
    constants.pop_front();
    EXPECT_TRUE(const1 != nullptr);
    EXPECT_TRUE(const2 != nullptr);

    bool invalid = false;
    try
    {
        auto type1 = const1->getType();
        auto type2 = const2->getType();
        const1->getRhs()->validate(type1, false);
        const2->getRhs()->validate(type2, false);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        invalid = true;
    }
    EXPECT_FALSE(invalid);
}

TEST(InitilizerTest, const_int_overflow)
{
    auto content = R"(package com.bst.idl.parser.test
    typeCollection MyCollection 
    {
        const UInt8 x0=-1
        const Int8 x=129
        const Int16 y=215
        const Int8 z1=y-133
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
    auto constants = typeCollection->getConstants();
    EXPECT_EQ(constants.size(), 4U);
    auto const1 = constants.front();
    constants.pop_front();
    auto const2 = constants.front();
    constants.pop_front();
    auto const3 = constants.front();
    constants.pop_front();
    auto const4 = constants.front();
    constants.pop_front();
    EXPECT_TRUE(const1 != nullptr);
    EXPECT_TRUE(const2 != nullptr);
    EXPECT_TRUE(const3 != nullptr);
    EXPECT_TRUE(const4 != nullptr);

    bool invalid = false;
    try
    {
        auto type1 = const1->getType();
        auto type2 = const2->getType();
        auto type3 = const3->getType();
        auto type4 = const4->getType();
        const1->getRhs()->validate(type1, false);
        const2->getRhs()->validate(type2, false);
        const3->getRhs()->validate(type3, false);
        const3->getRhs()->validate(type4, false);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        invalid = true;
    }

    bool invalid1 = false;
    bool invalid2 = false;
    bool invalid3 = false;
    bool invalid4 = false;
    try
    {
        std::string value;
        auto type1 = const1->getType();
        const1->getRhs()->EvaluableValidate(type1, false, value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        invalid1 = true;
    }
    try
    {
        std::string value;
        auto type2 = const2->getType();
        const2->getRhs()->EvaluableValidate(type2, false, value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        invalid2 = true;
    }
    try
    {
        std::string value;
        auto type3 = const3->getType();
        const3->getRhs()->EvaluableValidate(type3, false, value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        invalid3 = true;
    }
    try
    {
        std::string value;
        auto type4 = const4->getType();
        const4->getRhs()->EvaluableValidate(type4, false, value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        invalid4 = true;
    }
    EXPECT_TRUE(invalid1);
    EXPECT_TRUE(invalid2);
    EXPECT_FALSE(invalid3);
    EXPECT_FALSE(invalid4);
}

TEST(InitilizerTest, typedef_int_overflow)
{
    auto content = R"(package com.bst.idl.parser.test
    typeCollection MyCollection 
    {
        typedef TypeOne is UInt8
        typedef TypeTwo is Int16
        const TypeOne x=129
        const TypeTwo y=215
        const TypeOne z1=x-133
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
    auto constants = typeCollection->getConstants();
    EXPECT_EQ(constants.size(), 3U);
    auto const1 = constants.front();
    constants.pop_front();
    auto const2 = constants.front();
    constants.pop_front();
    auto const3 = constants.front();
    constants.pop_front();
    EXPECT_TRUE(const1 != nullptr);
    EXPECT_TRUE(const2 != nullptr);
    EXPECT_TRUE(const3 != nullptr);
    bool invalid1 = false;
    bool invalid2 = false;
    bool invalid3 = false;
    try
    {
        auto type = const1->getType();
        const1->getRhs()->validate(type, false);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        invalid1 = true;
    }
    try
    {
        auto type = const2->getType();
        const2->getRhs()->validate(type, false);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        invalid2 = true;
    }
    try
    {
        auto type = const3->getType();
        const3->getRhs()->validate(type, false);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        invalid3 = true;
    }
    EXPECT_FALSE(invalid1);
    EXPECT_FALSE(invalid2);
    EXPECT_FALSE(invalid3);

    invalid1 = false;
    invalid2 = false;
    invalid3 = false;
    try
    {
        std::string value;
        auto type = const1->getType();
        const1->getRhs()->EvaluableValidate(type, false, value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        invalid1 = true;
    }
    try
    {
        std::string value;
        auto type = const2->getType();
        const2->getRhs()->EvaluableValidate(type, false, value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        invalid2 = true;
    }
    try
    {
        std::string value;
        auto type = const3->getType();
        const3->getRhs()->EvaluableValidate(type, false, value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        invalid3 = true;
    }
    EXPECT_FALSE(invalid1);
    EXPECT_FALSE(invalid2);
    EXPECT_TRUE(invalid3);
}

TEST(InitilizerTest, anonymous_array_overflow)
{
    auto content = R"(package com.bst.idl.parser.test
    typeCollection MyCollection 
    {
        typedef TypeOne is Int8
        const TypeOne x=12*-8
        const Float y=215.3f
        const Int16 [] array1=[x,y,15]
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
    auto constants = typeCollection->getConstants();
    EXPECT_EQ(constants.size(), 3U);
    auto const1 = constants.front();
    constants.pop_front();
    auto const2 = constants.front();
    constants.pop_front();
    auto const3 = constants.front();
    constants.pop_front();
    EXPECT_TRUE(const1 != nullptr);
    EXPECT_TRUE(const2 != nullptr);
    EXPECT_TRUE(const3 != nullptr);
    bool invalid1 = false;
    bool invalid2 = false;
    bool invalid3 = false;
    try
    {
        auto type = const1->getType();
        const1->getRhs()->validate(type, false);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        invalid1 = true;
    }
    try
    {
        auto type = const2->getType();
        const2->getRhs()->validate(type, false);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        invalid2 = true;
    }
    try
    {
        auto type = const3->getType();
        const3->getRhs()->validate(type, const3->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        invalid3 = true;
    }
    EXPECT_FALSE(invalid1);
    EXPECT_FALSE(invalid2);
    EXPECT_TRUE(invalid3);

    invalid1 = false;
    invalid2 = false;
    invalid3 = false;
    try
    {
        std::string value;
        auto type = const1->getType();
        const1->getRhs()->EvaluableValidate(type, const1->isArray(), value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        invalid1 = true;
    }
    try
    {
        std::string value;
        auto type = const2->getType();
        const2->getRhs()->EvaluableValidate(type, const2->isArray(), value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        invalid2 = true;
    }
    try
    {
        std::string value;
        auto type = const3->getType();
        const3->getRhs()->EvaluableValidate(type, const3->isArray(), value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        invalid3 = true;
    }
    EXPECT_FALSE(invalid1);
    EXPECT_FALSE(invalid2);
    EXPECT_FALSE(invalid3);
}
