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
#include "model/FInitializer.h"
#include "model/FModelManager.h"
#include "model/FOperation.h"
#include "model/FTypeDef.h"
#include <climits>
#include <gtest/gtest.h>

class InitializerTestLT : public ::testing::Test
{
protected:
    void SetUp()
    {
        mgr.clear();
        auto model1 = mgr.addModel("com.bst.idl.testInitializer.model1");
        // add tc
        auto tc1 = factory.createFTypeCollection();
        tc1->setContainer(model1);
        tc1->setName("TypeCollection1");

        // add struct1 to tc1
        auto struct1 = factory.createFStructType();
        struct1->setContainer(tc1);
        struct1->setName("Struct1");
        // add element1 to struct
        auto field1 = factory.createFField();
        field1->setArray(false);
        field1->setName("UInt64");
        auto typeref = factory.createFTypeRef();
        auto type = BstIdl::FBasicTypeId::get(8U);
        typeref->setPredefined(type);
        field1->setType(typeref);
        addListItem(struct1->getElements(), field1);
        // add element2 to struct
        auto field2 = factory.createFField();
        field2->setArray(false);
        field2->setName("Boolean");
        typeref = factory.createFTypeRef();
        type = BstIdl::FBasicTypeId::get(9U);
        typeref->setPredefined(type);
        field2->setType(typeref);
        addListItem(struct1->getElements(), field2);
        addListItem(tc1->getTypes(), struct1);

        // add union1 to tc1
        auto cst1 = factory.createFConstantDef();
        typeref = factory.createFTypeRef();
        auto union1 = factory.createFUnionType();
        typeref->setDerived(union1);
        cst1->setArray(false);
        cst1->setType(typeref);
        cst1->setName("Union1");

        // add element1 to union
        field1 = factory.createFField();
        field1->setArray(false);
        field1->setName("UInt64");
        typeref = factory.createFTypeRef();
        type = BstIdl::FBasicTypeId::get(8U);
        typeref->setPredefined(type);
        field1->setType(typeref);
        addListItem(union1->getElements(), field1);
        // add element2 to union
        field2 = factory.createFField();
        field2->setArray(false);
        field2->setName("Boolean");
        typeref = factory.createFTypeRef();
        type = BstIdl::FBasicTypeId::get(9U);
        typeref->setPredefined(type);
        field2->setType(typeref);
        addListItem(union1->getElements(), field2);
        addListItem(tc1->getConstants(), cst1);

        // add initializer to union1
        auto unionInit = factory.createFCompoundInitializer();
        auto fieldInit = factory.createFFieldInitializer();
        fieldInit->setElement(field1);
        auto init1 = factory.createFIntegerConstant();
        init1->setUnsignedVal(1012U);
        fieldInit->setValue(std::static_pointer_cast<BstIdl::FInitializerExpression>(init1));
        addListItem(unionInit->getElements(), fieldInit);
        // element2 initializer
        fieldInit = factory.createFFieldInitializer();
        fieldInit->setElement(field2);
        auto init2 = factory.createFBooleanConstant();
        init2->setVal(true);
        fieldInit->setValue(std::static_pointer_cast<BstIdl::FInitializerExpression>(init2));
        addListItem(unionInit->getElements(), fieldInit);
        cst1->setRhs(unionInit);

        // add constant struct1 to tc1
        cst1 = factory.createFConstantDef();
        typeref = factory.createFTypeRef();
        struct1 = factory.createFStructType();
        struct1->setPolymorphic(false);
        typeref->setDerived(struct1);
        cst1->setArray(false);
        cst1->setType(typeref);
        cst1->setName("Struct1");
        // add element to struct1
        field1 = factory.createFField();
        field1->setArray(false);
        field1->setName("UInt64");
        typeref = factory.createFTypeRef();
        type = BstIdl::FBasicTypeId::get(8U);
        typeref->setPredefined(type);
        field1->setType(typeref);
        addListItem(struct1->getElements(), field1);
        // add element2 to struct
        field2 = factory.createFField();
        field2->setArray(false);
        field2->setName("Boolean");
        typeref = factory.createFTypeRef();
        type = BstIdl::FBasicTypeId::get(9U);
        typeref->setPredefined(type);
        field2->setType(typeref);
        addListItem(struct1->getElements(), field2);

        // add initializer to constant1
        auto structInit = factory.createFCompoundInitializer();
        fieldInit = factory.createFFieldInitializer();
        fieldInit->setElement(field1);
        init1 = factory.createFIntegerConstant();
        init1->setUnsignedVal(1012U);
        fieldInit->setValue(std::static_pointer_cast<BstIdl::FInitializerExpression>(init1));
        addListItem(structInit->getElements(), fieldInit);
        // element2 initializer
        fieldInit = factory.createFFieldInitializer();
        fieldInit->setElement(field2);
        init2 = factory.createFBooleanConstant();
        init2->setVal(true);
        fieldInit->setValue(std::static_pointer_cast<BstIdl::FInitializerExpression>(init2));
        addListItem(structInit->getElements(), fieldInit);
        cst1->setRhs(structInit);
        // add cst1 to TC1
        addListItem(tc1->getConstants(), cst1);

        // add nested constant struct2 to tc1  【qre】
        auto cst = factory.createFConstantDef();
        typeref = factory.createFTypeRef();
        auto struct2 = factory.createFStructType();
        typeref->setDerived(struct2);
        cst->setType(typeref);
        cst->setName("Struct2");
        qre1 = factory.createFQualifiedElementRef();
        qre1->setElement(std::static_pointer_cast<BstIdl::FEvaluableElement>(cst));
        // add
        field1 = factory.createFField();
        typeref = factory.createFTypeRef();
        auto struct2_1 = factory.createFStructType();
        typeref->setDerived(struct2_1);
        field1->setType(typeref);
        addListItem(struct2->getElements(), field1);
        field1->setName("struct2_1");
        auto qre2 = factory.createFQualifiedElementRef();
        qre2->setField(field1);
        qre1->setQualifier(qre2);
        // add
        field1 = factory.createFField();
        typeref = factory.createFTypeRef();
        auto struct2_1_1 = factory.createFStructType();
        typeref->setDerived(struct2_1_1);
        field1->setType(typeref);
        addListItem(struct2_1->getElements(), field1);
        field1->setName("struct2_1_1");
        auto qre3 = factory.createFQualifiedElementRef();
        qre3->setField(field1);
        qre2->setQualifier(qre3);

        field2 = factory.createFField();
        field2->setArray(false);
        field2->setName("Boolean");
        typeref = factory.createFTypeRef();
        type = BstIdl::FBasicTypeId::get(9U);
        typeref->setPredefined(type);
        field2->setType(typeref);
        addListItem(struct2_1_1->getElements(), field2);
        auto qre4 = factory.createFQualifiedElementRef();
        qre3->setQualifier(qre4);
        qre4->setField(field2);
        // add cst to TC1
        addListItem(tc1->getConstants(), cst);

        // initializer_Compound
        auto struct2_1_1Init = factory.createFCompoundInitializer();
        fieldInit = factory.createFFieldInitializer();
        fieldInit->setElement(field2);
        init2 = factory.createFBooleanConstant();
        init2->setVal(false);
        fieldInit->setValue(std::static_pointer_cast<BstIdl::FInitializerExpression>(init2));
        addListItem(struct2_1_1Init->getElements(), fieldInit);

        auto struct2_1Init = factory.createFCompoundInitializer();
        addListItem(model1->getTypeCollections(), std::move(tc1));
    }
    BstIdl::FModelManager &mgr = BstIdl::FModelManager::getInstance();
    BstIdl::FFactory &factory = BstIdl::FFactory::getInstance();
    std::shared_ptr<BstIdl::FModel> model;
    std::shared_ptr<BstIdl::FQualifiedElementRef> qre1;
};

// compound  struct\union
TEST_F(InitializerTestLT, CompoundInitializer_validateStruct)
{
    auto model = mgr.findModel("com.bst.idl.testInitializer.model1");
    auto tc = model->getTypeCollections().front();
    auto constants = tc->getConstants();
    constants.pop_front();
    auto cst = constants.front();
    auto typeref = cst->getType();
    auto struct1 = std::dynamic_pointer_cast<BstIdl::FStructType>(typeref->getDerived());
    EXPECT_FALSE(struct1->isPolymorphic());
    auto compoundInit = std::dynamic_pointer_cast<BstIdl::FCompoundInitializer>(cst->getRhs());
    auto &elements = compoundInit->getElements();
    EXPECT_EQ(elements.size(), 2U);

    std::string value;
    bool ifthrow = false;
    try
    {
        compoundInit->EvaluableValidate(typeref, cst->isArray(), value, true);
        compoundInit->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        ifthrow = true;
    }
    EXPECT_FALSE(ifthrow);
}

TEST_F(InitializerTestLT, CompoundInitializer_validateUnion)
{
    auto model = mgr.findModel("com.bst.idl.testInitializer.model1");
    auto tc = model->getTypeCollections().front();
    auto cst = tc->getConstants().front();
    auto typeref = cst->getType();
    auto union1 = std::dynamic_pointer_cast<BstIdl::FStructType>(typeref->getDerived());
    auto compoundInit = std::dynamic_pointer_cast<BstIdl::FCompoundInitializer>(cst->getRhs());
    auto &elements = compoundInit->getElements();
    EXPECT_EQ(elements.size(), 2U);

    std::string value;
    bool ifthrow = false;
    try
    {
        compoundInit->EvaluableValidate(typeref, cst->isArray(), value, true);
        compoundInit->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        ifthrow = true;
    }
    EXPECT_FALSE(ifthrow);
}
// bracket  isarray/array/map

TEST_F(InitializerTestLT, CompoundInitializer_validate_isArray)
{
    // add union1 to tc1
    auto cst1 = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto union1 = factory.createFUnionType();
    typeref->setDerived(union1);
    cst1->setArray(true);
    cst1->setType(typeref);
    cst1->setName("Union1");

    // add element1 to union
    auto field1 = factory.createFField();
    field1->setArray(false);
    field1->setName("UInt64");
    auto typeref1 = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(8U);
    typeref1->setPredefined(type);
    field1->setType(typeref1);
    addListItem(union1->getElements(), field1);
    // add initializer to union1
    auto unionInit = factory.createFCompoundInitializer();
    auto fieldInit = factory.createFFieldInitializer();
    fieldInit->setElement(field1);
    auto init1 = factory.createFIntegerConstant();
    init1->setUnsignedVal(1012U);
    fieldInit->setValue(std::static_pointer_cast<BstIdl::FInitializerExpression>(init1));
    addListItem(unionInit->getElements(), fieldInit);
    cst1->setRhs(unionInit);

    std::string value;
    bool ifthrow = false;
    try
    {
        unionInit->EvaluableValidate(typeref, cst1->isArray(), value, true);
        unionInit->validate(typeref, cst1->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        ifthrow = true;
    }
    EXPECT_TRUE(ifthrow);
}

TEST_F(InitializerTestLT, CompoundInitializer_validate_bothNull)
{
    // false derived
    auto cst = factory.createFConstantDef();
    cst->setArray(false);
    auto typeref = factory.createFTypeRef();
    auto map1 = factory.createFMapType();
    typeref->setDerived(map1);
    map1->setName("Map");
    cst->setType(typeref);

    auto init = factory.createFCompoundInitializer();
    cst->setRhs(init);

    std::string value;
    bool ifthrow = false;
    try
    {
        init->EvaluableValidate(typeref, cst->isArray(), value, true);
        init->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        ifthrow = true;
    }
    EXPECT_TRUE(ifthrow);
}

TEST_F(InitializerTestLT, CompoundInitializer_validate_initilizerNull)
{
    // add union1 to tc1
    auto cst1 = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto union1 = factory.createFUnionType();
    typeref->setDerived(union1);
    cst1->setArray(false);
    cst1->setType(typeref);
    cst1->setName("Union1");

    // add element1 to union
    auto field1 = factory.createFField();
    field1->setArray(false);
    field1->setName("UInt64");
    auto typeref1 = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(8U);
    typeref1->setPredefined(type);
    field1->setType(typeref1);
    addListItem(union1->getElements(), field1);
    // add initializer to union1
    auto unionInit = factory.createFCompoundInitializer();
    auto fieldInit = factory.createFFieldInitializer();
    fieldInit->setElement(field1);
    auto init1 = factory.createFIntegerConstant();
    init1->setUnsignedVal(1012U);
    fieldInit->setValue(nullptr);
    addListItem(unionInit->getElements(), fieldInit);
    cst1->setRhs(unionInit);

    long double value;
    bool ifthrow = false;
    try
    {
        unionInit->validate(typeref, cst1->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        ifthrow = true;
    }
    EXPECT_TRUE(ifthrow);
}

TEST_F(InitializerTestLT, CompoundInitializer_validate_initilizerNotMatch)
{
    // add union1 to tc1
    auto cst1 = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto union1 = factory.createFUnionType();
    typeref->setDerived(union1);
    cst1->setArray(false);
    cst1->setType(typeref);
    cst1->setName("Union1");

    // add element1 to union
    auto field1 = factory.createFField();
    field1->setArray(false);
    field1->setName("UInt64");
    auto typeref1 = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(8U);
    typeref1->setPredefined(type);
    field1->setType(typeref1);
    addListItem(union1->getElements(), field1);
    // add initializer to union1
    auto unionInit = factory.createFCompoundInitializer();
    auto fieldInit = factory.createFFieldInitializer();
    auto field2 = factory.createFField();
    field2->setArray(false);
    field2->setName("Boolean");
    fieldInit->setElement(field2);
    auto init1 = factory.createFIntegerConstant();
    init1->setUnsignedVal(1012U);
    fieldInit->setValue(std::static_pointer_cast<BstIdl::FInitializerExpression>(init1));
    addListItem(unionInit->getElements(), fieldInit);
    cst1->setRhs(unionInit);

    long double value;
    bool ifthrow = false;
    try
    {
        unionInit->validate(typeref, cst1->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        ifthrow = true;
    }
    EXPECT_TRUE(ifthrow);
}

TEST_F(InitializerTestLT, Initilizer_error)
{
    auto cst = factory.createFConstantDef();
    cst->setArray(false);
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(9U);
    typeref->setPredefined(type);
    cst->setName("Boolean");
    cst->setType(typeref);

    auto init = factory.createFCompoundInitializer();
    cst->setRhs(init);
    long double value;
    bool ifthrow = false;
    try
    {
        init->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        ifthrow = true;
    }
    EXPECT_TRUE(ifthrow);
}

TEST_F(InitializerTestLT, Qre_elementNull)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(9U);
    typeref->setPredefined(type);
    cst->setName("Boolean");
    cst->setType(typeref);
    cst->setArray(false);
    auto qre = factory.createFQualifiedElementRef();
    qre->setElement(nullptr);

    long double value;
    bool ifthrow = false;
    try
    {
        qre->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        ifthrow = true;
    }
    EXPECT_TRUE(ifthrow);
}

TEST_F(InitializerTestLT, Qre_notConstant)
{
    auto field = factory.createFField();
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(9U);
    typeref->setPredefined(type);
    field->setArray(false);
    field->setName("Boolean");
    field->setType(typeref);
    auto qre = factory.createFQualifiedElementRef();
    qre->setElement(std::static_pointer_cast<BstIdl::FEvaluableElement>(field));

    long double value;
    bool ifthrow = false;
    try
    {
        qre->validate(typeref, field->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        ifthrow = true;
    }
    EXPECT_TRUE(ifthrow);
}

TEST_F(InitializerTestLT, Qre_nested_notCompound)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(9U);
    typeref->setPredefined(type);
    cst->setName("Boolean");
    cst->setType(typeref);
    cst->setArray(false);
    auto qre = factory.createFQualifiedElementRef();
    qre->setElement(std::static_pointer_cast<BstIdl::FEvaluableElement>(cst));
    auto qre2 = factory.createFQualifiedElementRef();
    qre2->setQualifier(qre);
    long double value;
    bool ifthrow = false;
    try
    {
        qre2->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        ifthrow = true;
    }
    EXPECT_TRUE(ifthrow);
}

TEST_F(InitializerTestLT, Qre_fieldNull)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type = factory.createFStructType();
    typeref->setDerived(type);
    cst->setName("Struct");
    cst->setType(typeref);
    cst->setArray(false);
    auto qre = factory.createFQualifiedElementRef();
    qre->setElement(std::static_pointer_cast<BstIdl::FEvaluableElement>(cst));
    auto qre2 = factory.createFQualifiedElementRef();
    qre2->setQualifier(qre);
    EXPECT_EQ(qre2->getQualifier(), qre);
    auto typeref1 = factory.createFTypeRef();
    auto type1 = BstIdl::FBasicTypeId::get(9U);
    typeref1->setPredefined(type1);
    long double value;
    bool ifthrow = false;
    try
    {
        qre2->validate(typeref1, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        ifthrow = true;
    }
    EXPECT_TRUE(ifthrow);
}

TEST_F(InitializerTestLT, Qre_invalidElementRef)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto struct1 = factory.createFStructType();
    typeref->setDerived(struct1);
    cst->setName("Struct");
    cst->setType(typeref);
    cst->setArray(false);
    // add element to struct1
    auto field1 = factory.createFField();
    field1->setArray(false);
    field1->setName("UInt64");
    auto typeref1 = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(8U);
    typeref1->setPredefined(type);
    field1->setType(typeref1);
    addListItem(struct1->getElements(), field1);

    auto field2 = factory.createFField();
    field2->setArray(false);
    field2->setName("Int64");
    auto qre = factory.createFQualifiedElementRef();
    qre->setElement(std::static_pointer_cast<BstIdl::FEvaluableElement>(cst));
    auto qre2 = factory.createFQualifiedElementRef();
    qre2->setQualifier(qre);
    qre2->setField(field2);
    long double value;
    bool ifthrow = false;
    try
    {
        qre2->validate(typeref1, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        ifthrow = true;
    }
    EXPECT_TRUE(ifthrow);
}

TEST_F(InitializerTestLT, Qre_notMatchType)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto struct1 = factory.createFStructType();
    typeref->setDerived(struct1);
    cst->setName("Struct");
    cst->setType(typeref);
    cst->setArray(false);
    // add element to struct1
    auto field1 = factory.createFField();
    field1->setArray(false);
    field1->setName("UInt64");
    auto typeref1 = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(8U);
    typeref1->setPredefined(type);
    field1->setType(typeref1);
    addListItem(struct1->getElements(), field1);

    auto qre = factory.createFQualifiedElementRef();
    qre->setElement(std::static_pointer_cast<BstIdl::FEvaluableElement>(cst));
    auto qre2 = factory.createFQualifiedElementRef();
    qre2->setQualifier(qre);
    qre2->setField(field1);
    auto typeref2 = factory.createFTypeRef();
    auto type1 = BstIdl::FBasicTypeId::get(13U);
    typeref2->setPredefined(type1);
    long double value;
    bool ifthrow = false;
    try
    {
        qre2->validate(typeref2, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        ifthrow = true;
    }
    EXPECT_TRUE(ifthrow);
}

TEST_F(InitializerTestLT, Qre_toString)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type = factory.createFStructType();
    typeref->setDerived(type);
    cst->setName("Struct");
    cst->setType(typeref);
    cst->setArray(false);
    // add element to struct1
    auto field1 = factory.createFField();
    field1->setArray(false);
    field1->setName("UInt64");
    auto typeref1 = factory.createFTypeRef();
    auto type1 = BstIdl::FBasicTypeId::get(8U);
    typeref1->setPredefined(type1);
    field1->setType(typeref1);
    addListItem(type->getElements(), field1);

    auto qre = factory.createFQualifiedElementRef();
    qre->setElement(std::static_pointer_cast<BstIdl::FEvaluableElement>(cst));
    auto qre2 = factory.createFQualifiedElementRef();
    qre2->setQualifier(qre);
    EXPECT_EQ(qre2->getQualifier(), qre);
    qre2->setField(field1);

    EXPECT_STREQ(qre2->toString().c_str(), "Struct->UInt64");
}

TEST(zBracketInitializerTest, validate_isArray)
{
    BstIdl::FFactory &factory = BstIdl::FFactory::getInstance();
    auto constant = factory.createFConstantDef();
    constant->setArray(true);
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(8U);
    typeref->setPredefined(type);
    constant->setType(typeref);

    auto bracketInit = factory.createFBracketInitializer();
    auto elementInit = factory.createFElementInitializer();
    auto init = factory.createFIntegerConstant();
    init->setUnsignedVal(1U);
    elementInit->setFirst(init);
    addListItem(bracketInit->getElements(), elementInit);
    elementInit = factory.createFElementInitializer();
    init = factory.createFIntegerConstant();
    init->setUnsignedVal(LONG_MAX);
    elementInit->setFirst(init);
    addListItem(bracketInit->getElements(), elementInit);
    elementInit = factory.createFElementInitializer();
    init = factory.createFIntegerConstant();
    init->setUnsignedVal(1U);
    elementInit->setFirst(init);
    addListItem(bracketInit->getElements(), elementInit);

    std::string value;
    bool ifthrow = false;
    try
    {
        bracketInit->EvaluableValidate(typeref, constant->isArray(), value, true);
        bracketInit->validate(typeref, constant->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        ifthrow = true;
    }
    EXPECT_FALSE(ifthrow);
}

TEST(BracketInitializerTest, validate_isArray_initializerNull)
{
    BstIdl::FFactory &factory = BstIdl::FFactory::getInstance();
    auto constant = factory.createFConstantDef();
    constant->setArray(true);
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(8U);
    typeref->setPredefined(type);
    constant->setType(typeref);

    auto bracketInit = factory.createFBracketInitializer();
    auto elementInit = factory.createFElementInitializer();
    auto init = factory.createFIntegerConstant();
    init->setUnsignedVal(1U);
    elementInit->setFirst(init);
    addListItem(bracketInit->getElements(), elementInit);
    elementInit = factory.createFElementInitializer();
    init = factory.createFIntegerConstant();
    init->setUnsignedVal(0U);
    elementInit->setFirst(init);
    addListItem(bracketInit->getElements(), elementInit);
    elementInit = factory.createFElementInitializer();
    elementInit->setFirst(nullptr);
    addListItem(bracketInit->getElements(), elementInit);

    std::string value;
    bool ifthrow = false;
    try
    {
        bracketInit->EvaluableValidate(typeref, constant->isArray(), value, true);
        bracketInit->validate(typeref, constant->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        ifthrow = true;
    }
    EXPECT_TRUE(ifthrow);
}

TEST(BracketInitializerTest, EvaluableValidate_isArray_initializerNull)
{
    BstIdl::FFactory &factory = BstIdl::FFactory::getInstance();
    auto constant = factory.createFConstantDef();
    constant->setArray(true);
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(8U);
    typeref->setPredefined(type);
    constant->setType(typeref);

    auto bracketInit = factory.createFBracketInitializer();
    auto elementInit = factory.createFElementInitializer();
    auto init = factory.createFIntegerConstant();
    init->setUnsignedVal(1U);
    elementInit->setFirst(init);
    addListItem(bracketInit->getElements(), elementInit);
    elementInit = factory.createFElementInitializer();
    init = factory.createFIntegerConstant();
    init->setUnsignedVal(2U);
    elementInit->setFirst(init);
    addListItem(bracketInit->getElements(), elementInit);
    elementInit = factory.createFElementInitializer();
    addListItem(bracketInit->getElements(), elementInit);

    std::string value;
    bool ifthrow = false;
    try
    {
        bracketInit->EvaluableValidate(typeref, constant->isArray(), value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        ifthrow = true;
    }
    EXPECT_TRUE(ifthrow);
}

TEST(BracketInitializerTest, validate_arrayDerived)
{
    BstIdl::FFactory &factory = BstIdl::FFactory::getInstance();

    auto typeref = factory.createFTypeRef();
    auto array = factory.createFArrayType();
    typeref->setDerived(array);
    auto typeref1 = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(8U);
    typeref1->setPredefined(type);
    array->setElementType(typeref1);

    // add bracketInitializer
    auto bracketInit = factory.createFBracketInitializer();
    auto elementInit = factory.createFElementInitializer();
    auto init = factory.createFIntegerConstant();
    init->setUnsignedVal(1U);
    elementInit->setFirst(init);
    addListItem(bracketInit->getElements(), elementInit);
    elementInit = factory.createFElementInitializer();
    init = factory.createFIntegerConstant();
    init->setUnsignedVal(LONG_MAX);
    elementInit->setFirst(init);
    addListItem(bracketInit->getElements(), elementInit);
    elementInit = factory.createFElementInitializer();
    init = factory.createFIntegerConstant();
    init->setUnsignedVal(1U);
    elementInit->setFirst(init);
    addListItem(bracketInit->getElements(), elementInit);

    std::string value;
    bool ifthrow = false;
    try
    {
        bracketInit->EvaluableValidate(typeref, false, value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        ifthrow = true;
    }
    EXPECT_FALSE(ifthrow);
}

TEST(BracketInitializerTest, validate_array_InvalidInit)
{
    BstIdl::FFactory &factory = BstIdl::FFactory::getInstance();

    auto typeref = factory.createFTypeRef();
    auto array = factory.createFArrayType();
    typeref->setDerived(array);
    auto typeref1 = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(8U);
    typeref1->setPredefined(type);
    array->setElementType(typeref1);

    // add bracketInitializer
    auto bracketInit = factory.createFBracketInitializer();
    auto elementInit = factory.createFElementInitializer();
    auto init = factory.createFIntegerConstant();
    init->setUnsignedVal(1U);
    elementInit->setFirst(init);
    addListItem(bracketInit->getElements(), elementInit);
    elementInit = factory.createFElementInitializer();
    init = factory.createFIntegerConstant();
    init->setUnsignedVal(0U);
    elementInit->setFirst(init);
    addListItem(bracketInit->getElements(), elementInit);
    elementInit = factory.createFElementInitializer();
    elementInit->setFirst(nullptr);
    addListItem(bracketInit->getElements(), elementInit);

    long double value;
    bool ifthrow = false;
    try
    {
        bracketInit->validate(typeref, false);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        ifthrow = true;
    }
    EXPECT_TRUE(ifthrow);
}

TEST(BracketInitializerTest, validate_mapDerived)
{
    BstIdl::FFactory &factory = BstIdl::FFactory::getInstance();

    auto typeref = factory.createFTypeRef();
    auto map = factory.createFMapType();
    typeref->setDerived(map);
    auto typeref1 = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(8U);
    typeref1->setPredefined(type);
    map->setKeyType(typeref1);
    auto typeref2 = factory.createFTypeRef();
    auto type1 = BstIdl::FBasicTypeId::get(9U);
    typeref2->setPredefined(type1);
    map->setValueType(typeref2);

    // add bracketInitializer
    auto bracketInit = factory.createFBracketInitializer();
    auto elementInit = factory.createFElementInitializer();
    auto intInit = factory.createFIntegerConstant();
    intInit->setUnsignedVal(10U);
    elementInit->setFirst(intInit);
    auto boolInit = factory.createFBooleanConstant();
    boolInit->setVal(true);
    elementInit->setSecond(boolInit);
    addListItem(bracketInit->getElements(), elementInit);
    elementInit = factory.createFElementInitializer();
    intInit = factory.createFIntegerConstant();
    intInit->setUnsignedVal(12U);
    elementInit->setFirst(intInit);
    boolInit = factory.createFBooleanConstant();
    boolInit->setVal(true);
    elementInit->setSecond(boolInit);
    addListItem(bracketInit->getElements(), elementInit);
    elementInit = factory.createFElementInitializer();
    intInit = factory.createFIntegerConstant();
    intInit->setUnsignedVal(20U);
    elementInit->setFirst(intInit);
    boolInit = factory.createFBooleanConstant();
    boolInit->setVal(true);
    elementInit->setSecond(boolInit);
    addListItem(bracketInit->getElements(), elementInit);

    std::string value;
    bool ifthrow = false;
    try
    {
        bracketInit->EvaluableValidate(typeref, false, value, true);
        bracketInit->validate(typeref, false);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        ifthrow = true;
    }
    EXPECT_FALSE(ifthrow);
}

TEST(BracketInitializerTest, validate_map_initializerNullKey)
{
    BstIdl::FFactory &factory = BstIdl::FFactory::getInstance();

    auto typeref = factory.createFTypeRef();
    auto map = factory.createFMapType();
    typeref->setDerived(map);
    auto typeref1 = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(8U);
    typeref1->setPredefined(type);
    map->setKeyType(typeref1);
    auto typeref2 = factory.createFTypeRef();
    auto type1 = BstIdl::FBasicTypeId::get(9U);
    typeref2->setPredefined(type1);
    map->setValueType(typeref2);

    // add bracketInitializer
    auto bracketInit = factory.createFBracketInitializer();
    auto elementInit = factory.createFElementInitializer();
    auto intInit = factory.createFIntegerConstant();
    intInit->setUnsignedVal(10U);
    elementInit->setFirst(nullptr);
    auto boolInit = factory.createFBooleanConstant();
    boolInit->setVal(true);
    elementInit->setSecond(boolInit);
    addListItem(bracketInit->getElements(), elementInit);
    elementInit = factory.createFElementInitializer();
    intInit = factory.createFIntegerConstant();
    intInit->setUnsignedVal(12U);
    elementInit->setFirst(intInit);
    boolInit = factory.createFBooleanConstant();
    boolInit->setVal(true);
    elementInit->setSecond(boolInit);
    addListItem(bracketInit->getElements(), elementInit);
    elementInit = factory.createFElementInitializer();
    intInit = factory.createFIntegerConstant();
    intInit->setUnsignedVal(20U);
    elementInit->setFirst(intInit);
    boolInit = factory.createFBooleanConstant();
    boolInit->setVal(true);
    elementInit->setSecond(boolInit);
    addListItem(bracketInit->getElements(), elementInit);

    std::string value;
    bool ifthrow = false;
    try
    {
        bracketInit->EvaluableValidate(typeref, false, value, true);
        bracketInit->validate(typeref, false);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        ifthrow = true;
    }
    EXPECT_TRUE(ifthrow);
}

TEST(BracketInitializerTest, validate_map_initializerNullValue)
{
    BstIdl::FFactory &factory = BstIdl::FFactory::getInstance();

    auto typeref = factory.createFTypeRef();
    auto map = factory.createFMapType();
    typeref->setDerived(map);
    auto typeref1 = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(8U);
    typeref1->setPredefined(type);
    map->setKeyType(typeref1);
    auto typeref2 = factory.createFTypeRef();
    auto type1 = BstIdl::FBasicTypeId::get(9U);
    typeref2->setPredefined(type1);
    map->setValueType(typeref2);

    // add bracketInitializer
    auto bracketInit = factory.createFBracketInitializer();
    auto elementInit = factory.createFElementInitializer();
    auto intInit = factory.createFIntegerConstant();
    intInit->setUnsignedVal(10U);
    elementInit->setFirst(intInit);
    auto boolInit = factory.createFBooleanConstant();
    boolInit->setVal(true);
    elementInit->setSecond(nullptr);
    addListItem(bracketInit->getElements(), elementInit);
    elementInit = factory.createFElementInitializer();
    intInit = factory.createFIntegerConstant();
    intInit->setUnsignedVal(12U);
    elementInit->setFirst(intInit);
    boolInit = factory.createFBooleanConstant();
    boolInit->setVal(true);
    elementInit->setSecond(boolInit);
    addListItem(bracketInit->getElements(), elementInit);
    elementInit = factory.createFElementInitializer();
    intInit = factory.createFIntegerConstant();
    intInit->setUnsignedVal(20U);
    elementInit->setFirst(intInit);
    boolInit = factory.createFBooleanConstant();
    boolInit->setVal(true);
    elementInit->setSecond(boolInit);
    addListItem(bracketInit->getElements(), elementInit);

    std::string value;
    bool ifthrow = false;
    try
    {
        bracketInit->EvaluableValidate(typeref, false, value, true);
        bracketInit->validate(typeref, false);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        ifthrow = true;
    }
    EXPECT_TRUE(ifthrow);
}

TEST(BracketInitializerTest, validate_otherDerived)
{
    BstIdl::FFactory &factory = BstIdl::FFactory::getInstance();

    auto typeref = factory.createFTypeRef();
    auto enum1 = factory.createFEnumerationType();
    typeref->setDerived(enum1);

    // add bracketInitializer
    auto bracketInit = factory.createFBracketInitializer();
    auto elementInit = factory.createFElementInitializer();
    addListItem(bracketInit->getElements(), elementInit);

    std::string value;
    bool ifthrow = false;
    try
    {
        bracketInit->EvaluableValidate(typeref, false, value, true);
        bracketInit->validate(typeref, false);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        ifthrow = true;
    }
    EXPECT_TRUE(ifthrow);
}

TEST(BracketInitializerTest, evaluableValidate_otherDerived)
{
    BstIdl::FFactory &factory = BstIdl::FFactory::getInstance();

    auto typeref = factory.createFTypeRef();
    auto enum1 = factory.createFEnumerationType();
    typeref->setDerived(enum1);

    // add bracketInitializer
    auto bracketInit = factory.createFBracketInitializer();
    auto elementInit = factory.createFElementInitializer();
    addListItem(bracketInit->getElements(), elementInit);

    std::string value;
    bool ifthrow = false;
    try
    {
        bracketInit->EvaluableValidate(typeref, false, value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        ifthrow = true;
    }
    EXPECT_TRUE(ifthrow);
}

TEST(BracketInitializerTest, evaluableValidate_map_initializerNullValue)
{
    BstIdl::FFactory &factory = BstIdl::FFactory::getInstance();

    auto typeref = factory.createFTypeRef();
    auto map = factory.createFMapType();
    typeref->setDerived(map);
    auto typeref1 = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(8U);
    typeref1->setPredefined(type);
    map->setKeyType(typeref1);
    auto typeref2 = factory.createFTypeRef();
    auto type1 = BstIdl::FBasicTypeId::get(9U);
    typeref2->setPredefined(type1);
    map->setValueType(typeref2);

    // add bracketInitializer
    auto bracketInit = factory.createFBracketInitializer();
    auto elementInit = factory.createFElementInitializer();
    auto intInit = factory.createFIntegerConstant();
    intInit->setUnsignedVal(10U);
    elementInit->setFirst(intInit);
    auto boolInit = factory.createFBooleanConstant();
    boolInit->setVal(true);
    elementInit->setSecond(nullptr);
    addListItem(bracketInit->getElements(), elementInit);
    elementInit = factory.createFElementInitializer();
    intInit = factory.createFIntegerConstant();
    intInit->setUnsignedVal(12U);
    elementInit->setFirst(intInit);
    boolInit = factory.createFBooleanConstant();
    boolInit->setVal(true);
    elementInit->setSecond(boolInit);
    addListItem(bracketInit->getElements(), elementInit);
    elementInit = factory.createFElementInitializer();
    intInit = factory.createFIntegerConstant();
    intInit->setUnsignedVal(20U);
    elementInit->setFirst(intInit);
    boolInit = factory.createFBooleanConstant();
    boolInit->setVal(true);
    elementInit->setSecond(boolInit);
    addListItem(bracketInit->getElements(), elementInit);

    std::string value;
    bool ifthrow = false;
    try
    {
        bracketInit->EvaluableValidate(typeref, false, value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        ifthrow = true;
    }
    EXPECT_TRUE(ifthrow);
}

TEST(BracketInitializerTest, evaluableValidate_map_initializerNullKey)
{
    BstIdl::FFactory &factory = BstIdl::FFactory::getInstance();

    auto typeref = factory.createFTypeRef();
    auto map = factory.createFMapType();
    typeref->setDerived(map);
    auto typeref1 = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(8U);
    typeref1->setPredefined(type);
    map->setKeyType(typeref1);
    auto typeref2 = factory.createFTypeRef();
    auto type1 = BstIdl::FBasicTypeId::get(9U);
    typeref2->setPredefined(type1);
    map->setValueType(typeref2);

    // add bracketInitializer
    auto bracketInit = factory.createFBracketInitializer();
    auto elementInit = factory.createFElementInitializer();
    auto intInit = factory.createFIntegerConstant();
    intInit->setUnsignedVal(10U);
    elementInit->setFirst(nullptr);
    auto boolInit = factory.createFBooleanConstant();
    boolInit->setVal(true);
    elementInit->setSecond(boolInit);
    addListItem(bracketInit->getElements(), elementInit);
    elementInit = factory.createFElementInitializer();
    intInit = factory.createFIntegerConstant();
    intInit->setUnsignedVal(12U);
    elementInit->setFirst(intInit);
    boolInit = factory.createFBooleanConstant();
    boolInit->setVal(true);
    elementInit->setSecond(boolInit);
    addListItem(bracketInit->getElements(), elementInit);
    elementInit = factory.createFElementInitializer();
    intInit = factory.createFIntegerConstant();
    intInit->setUnsignedVal(20U);
    elementInit->setFirst(intInit);
    boolInit = factory.createFBooleanConstant();
    boolInit->setVal(true);
    elementInit->setSecond(boolInit);
    addListItem(bracketInit->getElements(), elementInit);

    std::string value;
    bool ifthrow = false;
    try
    {
        bracketInit->EvaluableValidate(typeref, false, value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        ifthrow = true;
    }
    EXPECT_TRUE(ifthrow);
}

TEST(BracketInitializerTest, evaluableValidate_array_InvalidInit)
{
    BstIdl::FFactory &factory = BstIdl::FFactory::getInstance();

    auto typeref = factory.createFTypeRef();
    auto array = factory.createFArrayType();
    typeref->setDerived(array);
    auto typeref1 = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(8U);
    typeref1->setPredefined(type);
    array->setElementType(typeref1);

    // add bracketInitializer
    auto bracketInit = factory.createFBracketInitializer();
    auto elementInit = factory.createFElementInitializer();
    auto init = factory.createFIntegerConstant();
    init->setUnsignedVal(1U);
    elementInit->setFirst(init);
    addListItem(bracketInit->getElements(), elementInit);
    elementInit = factory.createFElementInitializer();
    init = factory.createFIntegerConstant();
    init->setUnsignedVal(0U);
    elementInit->setFirst(init);
    addListItem(bracketInit->getElements(), elementInit);
    elementInit = factory.createFElementInitializer();
    elementInit->setFirst(nullptr);
    addListItem(bracketInit->getElements(), elementInit);

    std::string value;
    bool ifthrow = false;
    try
    {
        bracketInit->EvaluableValidate(typeref, false, value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        ifthrow = true;
    }
    EXPECT_TRUE(ifthrow);
}
