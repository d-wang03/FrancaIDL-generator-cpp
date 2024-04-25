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
#include "validator/ReferenceManager.h"
#include <gtest/gtest.h>

class ReferenceManagerTestLT : public ::testing::Test
{
protected:
    void SetUp() override
    {
        mgr.clear();
        // set model1
        auto model1 = mgr.addModel("com.bst.idl.testRefMgr.model1");
        // add TypeCollection1
        auto tc1 = factory.createFTypeCollection();
        tc1->setName("TypeCollection1");
        tc1->setContainer(model1);
        // add user−defined types and constant definitions
        ////array
        auto array1 = factory.createFArrayType();
        array1->setName("Array1");
        array1->setContainer(tc1);
        addListItem(tc1->getTypes(), std::move(array1));
        ////errorenum /enumeration
        auto errorEnum1 = factory.createFEnumerationType();
        errorEnum1->setName("ErrorEnum1");
        errorEnum1->setContainer(tc1);
        addListItem(tc1->getTypes(), std::move(errorEnum1));
        ////struct
        auto struct1 = factory.createFStructType();
        struct1->setName("Struct1");
        struct1->setContainer(tc1);
        addListItem(tc1->getTypes(), std::move(struct1));
        // constDef --struct1
        auto cst_def = factory.createFConstantDef();
        auto struct2 = factory.createFStructType();
        auto typeref = factory.createFTypeRef();
        struct2->setName("QerStruct1");
        typeref->setDerived(struct2);
        cst_def->setType(typeref);
        cst_def->setName("ConstStruct1");
        cst_def->setContainer(tc1);
        addListItem(tc1->getConstants(), std::move(cst_def));
        // constDef --struct2
        cst_def = factory.createFConstantDef();
        struct2 = factory.createFStructType();
        typeref = factory.createFTypeRef();
        struct2->setName("QerStruct2");
        typeref->setDerived(struct2);
        cst_def->setType(typeref);
        cst_def->setName("ConstStruct2");
        cst_def->setContainer(tc1);
        addListItem(tc1->getConstants(), std::move(cst_def));
        ////union
        auto union1 = factory.createFUnionType();
        union1->setName("Union1");
        union1->setContainer(tc1);
        addListItem(tc1->getTypes(), std::move(union1));
        // add tc to model1
        addListItem(model1->getTypeCollections(), std::move(tc1));

        // add interface1
        auto interface1 = factory.createFInterface();
        interface1->setName("Interface1");
        interface1->setContainer(model1);
        // method
        auto method = factory.createFMethod();
        method->setName("Method1");
        method->setContainer(interface1);
        addListItem(interface1->getMethods(), std::move(method));
        // errorenum 【's container is interface1】 /enumeration
        errorEnum1 = factory.createFEnumerationType();
        errorEnum1->setName("ErrorEnum1");
        errorEnum1->setContainer(interface1);
        addListItem(interface1->getTypes(), std::move(errorEnum1));
        // errorenum2【's container is interface1】 /enumeration
        errorEnum1 = factory.createFEnumerationType();
        errorEnum1->setName("ErrorEnum2");
        errorEnum1->setContainer(interface1);
        addListItem(interface1->getTypes(), std::move(errorEnum1));
        // constDef --struct1
        cst_def = factory.createFConstantDef();
        struct2 = factory.createFStructType();
        typeref = factory.createFTypeRef();
        struct2->setName("QerStruct1");
        typeref->setDerived(struct2);
        cst_def->setType(typeref);
        cst_def->setName("ConstStruct1");
        cst_def->setContainer(interface1);
        addListItem(interface1->getConstants(), std::move(cst_def));
        // add struct1
        struct1 = factory.createFStructType();
        struct1->setName("Struct1");
        struct1->setContainer(interface1);
        addListItem(interface1->getTypes(), std::move(struct1));
        // add struct1
        struct2 = factory.createFStructType();
        struct2->setName("Struct2");
        struct2->setContainer(interface1);
        addListItem(interface1->getTypes(), std::move(struct2));
        // array
        array1 = factory.createFArrayType();
        array1->setName("Array1");
        array1->setContainer(interface1);
        addListItem(interface1->getTypes(), std::move(array1));
        // union
        union1 = factory.createFUnionType();
        union1->setName("Union1");
        union1->setContainer(interface1);
        addListItem(interface1->getTypes(), std::move(union1));
        // union2
        auto union2 = factory.createFUnionType();
        union2->setName("Union2");
        union2->setContainer(interface1);
        addListItem(interface1->getTypes(), std::move(union2));
        // add interface1 to model1
        addListItem(model1->getInterfaces(), std::move(interface1));

        // add interface2
        auto interface2 = factory.createFInterface();
        interface2->setName("Interface2");
        interface2->setContainer(model1);
        ////errorenum in interface
        errorEnum1 = factory.createFEnumerationType();
        errorEnum1->setName("ErrorEnum1");
        errorEnum1->setContainer(interface2);
        addListItem(interface2->getTypes(), std::move(errorEnum1));
        // add interface2 to model1
        addListItem(model1->getInterfaces(), std::move(interface2));

        // model2
        auto model2 = mgr.addModel("com.bst.idl.testRefMgr.model2");
        // add typecollection
        auto tc = factory.createFTypeCollection();
        tc->setName("TypeCollection1");
        tc->setContainer(model2);
        // ErrorEnum1 /enumeration
        errorEnum1 = factory.createFEnumerationType();
        errorEnum1->setName("ErrorEnum1");
        errorEnum1->setContainer(tc);
        addListItem(tc->getTypes(), std::move(errorEnum1));
        ////struct
        struct1 = factory.createFStructType();
        struct1->setName("Struct1");
        struct1->setContainer(tc);
        addListItem(tc->getTypes(), std::move(struct1));
        ////union
        union1 = factory.createFUnionType();
        union1->setName("Union1");
        union1->setContainer(tc);
        addListItem(tc->getTypes(), std::move(union1));
        // add tc to model 2
        addListItem(model2->getTypeCollections(), std::move(tc));

        // inf1
        interface1 = factory.createFInterface();
        interface1->setName("Interface1");
        interface1->setContainer(model2);
        // ErrorEnum1 /enumeration
        errorEnum1 = factory.createFEnumerationType();
        errorEnum1->setName("ErrorEnum1");
        errorEnum1->setContainer(interface1);
        addListItem(interface1->getTypes(), std::move(errorEnum1));
        // add struct1
        struct1 = factory.createFStructType();
        struct1->setName("Struct1");
        struct1->setContainer(interface1);
        addListItem(interface1->getTypes(), std::move(struct1));
        // union
        union1 = factory.createFUnionType();
        union1->setName("Union1");
        union1->setContainer(interface1);
        addListItem(interface1->getTypes(), std::move(union1));
        // add interface1 to model2
        addListItem(model2->getInterfaces(), std::move(interface1));

        // add interface2
        interface2 = factory.createFInterface();
        interface2->setName("Interface2");
        interface2->setContainer(model2);
        addListItem(model2->getInterfaces(), std::move(interface2));
    }

    BstIdl::FModelManager &mgr = BstIdl::FModelManager::getInstance();
    BstIdl::ReferenceManager &refMgr = BstIdl::ReferenceManager::getInstance();
    BstIdl::FFactory &factory = BstIdl::FFactory::getInstance();
};

// Manage

TEST_F(ReferenceManagerTestLT, resolveInterfaceManagesRef_inner)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto interface1 = model1->getInterfaces().front();
    refMgr.addInterfaceManagesRef(interface1, "com.bst.idl.testRefMgr.model1.Interface2");
    refMgr.resolveInterfaceManagesRef();
    auto &interfaces = interface1->getManagedInterfaces();
    EXPECT_EQ(interfaces.size(), 1);
    EXPECT_STREQ(interfaces.front()->getName().c_str(), "Interface2");
}

TEST_F(ReferenceManagerTestLT, resolveInterfaceManagesRef_cross)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto model2 = mgr.findModel("com.bst.idl.testRefMgr.model2");
    auto interface1 = model1->getInterfaces().front();
    EXPECT_EQ(model1->getInterfaces().size(), 2);
    refMgr.addInterfaceManagesRef(interface1, "com.bst.idl.testRefMgr.model2.Interface1");
    refMgr.resolveInterfaceManagesRef();
    auto &interfaces = interface1->getManagedInterfaces();
    EXPECT_EQ(interfaces.size(), 1);
    EXPECT_STREQ(interfaces.front()->getName().c_str(), "Interface1");
}

TEST_F(ReferenceManagerTestLT, resolveInterfaceManagesRef_unresolved)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto interface1 = model1->getInterfaces().front();
    refMgr.addInterfaceManagesRef(interface1, "com.bst.idl.testRefMgr.model1.Interface4");
    try
    {
        refMgr.resolveInterfaceManagesRef();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto &interfaces = interface1->getManagedInterfaces();
    EXPECT_TRUE(interfaces.empty());
}

TEST_F(ReferenceManagerTestLT, resolveInterfaceManagesRef_duplicate)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto interface1 = model1->getInterfaces().front();
    EXPECT_EQ(model1->getInterfaces().size(), 2);
    refMgr.addInterfaceManagesRef(interface1, "com.bst.idl.testRefMgr.model1.Interface2");
    refMgr.addInterfaceManagesRef(interface1, "com.bst.idl.testRefMgr.model2.Interface1");
    try
    {
        refMgr.resolveInterfaceManagesRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto &interfaces = interface1->getManagedInterfaces();
    EXPECT_TRUE(interfaces.size() == 2U);
}

TEST_F(ReferenceManagerTestLT, resolveInterfaceManagesRef_multi)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto interface1 = model1->getInterfaces().front();
    EXPECT_EQ(model1->getInterfaces().size(), 2);
    refMgr.addInterfaceManagesRef(interface1, "com.bst.idl.testRefMgr.model1.Interface2");
    refMgr.addInterfaceManagesRef(interface1, "com.bst.idl.testRefMgr.model2.Interface1");
    refMgr.addInterfaceManagesRef(interface1, "com.bst.idl.testRefMgr.model2.Interface2");
    try
    {
        refMgr.resolveInterfaceManagesRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto &interfaces = interface1->getManagedInterfaces();
    EXPECT_TRUE(interfaces.size() == 3U);
}

// Extends

// interface Extends
TEST_F(ReferenceManagerTestLT, resolveInterfaceExtendsRef)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto interface2 = model1->getInterfaces().back();
    auto interface1 = factory.createFInterface();
    refMgr.addInterfaceExtendsRef(interface1, "com.bst.idl.testRefMgr.model1.Interface2");
    refMgr.resolveInterfaceExtendsRef();
    EXPECT_EQ(interface1->getBase(), interface2);
}

TEST_F(ReferenceManagerTestLT, resolveInterfaceExtendsRef_unresolve)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto interface1 = factory.createFInterface();
    refMgr.addInterfaceExtendsRef(interface1, "com.bst.idl.testRefMgr.model1.Interface4");
    try
    {
        refMgr.resolveInterfaceExtendsRef();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    EXPECT_EQ(interface1->getBase(), nullptr);
}

TEST_F(ReferenceManagerTestLT, resolveInterfaceExtendsRef_duplicate)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto interface1 = factory.createFInterface();
    refMgr.addInterfaceExtendsRef(interface1, "com.bst.idl.testRefMgr.model1.Interface2");
    refMgr.addInterfaceExtendsRef(interface1, "com.bst.idl.testRefMgr.model2.Interface1");
    try
    {
        refMgr.resolveInterfaceExtendsRef();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    EXPECT_EQ(interface1->getBase(), nullptr);
}

TEST_F(ReferenceManagerTestLT, resolveInterfaceExtendsRef_multi)
{
    refMgr.clear();
    auto interface1 = factory.createFInterface();
    refMgr.addInterfaceExtendsRef(interface1, "com.bst.idl.testRefMgr.model1.Interface2");
    refMgr.addInterfaceExtendsRef(interface1, "com.bst.idl.testRefMgr.model2.Interface1");
    refMgr.addInterfaceExtendsRef(interface1, "com.bst.idl.testRefMgr.model2.Interface2");

    try
    {
        refMgr.resolveInterfaceExtendsRef();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto base = interface1->getBase();
    EXPECT_NE(base, nullptr);
    EXPECT_STREQ(base->getName().c_str(), "Interface2");
}

// struct Extends
TEST_F(ReferenceManagerTestLT, resolveStructTypeExtendsRef_innertc)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto interface1 = model1->getInterfaces().front();
    auto struct1 = mgr.findFStructTypeInInterface(interface1, "com.bst.idl.testRefMgr.model1.Interface1.Struct1");
    refMgr.addStructTypeExtendsRef(struct1, "com.bst.idl.testRefMgr.model1.TypeCollection1.Struct1", nullptr);
    try
    {
        refMgr.resolveStructTypeExtendsRef();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    auto base = struct1->getBase();
    EXPECT_TRUE(base != nullptr);
    EXPECT_STREQ(base->getName().c_str(), "Struct1");
}

TEST_F(ReferenceManagerTestLT, resolveStructTypeExtendsRef_innerinf)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto interface1 = model1->getInterfaces().front();
    auto struct1 = mgr.findFStructTypeInInterface(interface1, "com.bst.idl.testRefMgr.model1.Interface1.Struct1");
    refMgr.addStructTypeExtendsRef(struct1, "com.bst.idl.testRefMgr.model1.Interface1.Struct2", interface1);
    refMgr.resolveStructTypeExtendsRef();
    auto base = struct1->getBase();
    EXPECT_TRUE(base != nullptr);
    EXPECT_STREQ(base->getName().c_str(), "Struct2");
}

TEST_F(ReferenceManagerTestLT, resolveStructTypeExtendsRef_cross_tc)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto interface1 = model1->getInterfaces().front();
    auto struct1 = mgr.findFStructTypeInInterface(interface1, "com.bst.idl.testRefMgr.model1.Interface1.Struct1");
    refMgr.addStructTypeExtendsRef(struct1, "com.bst.idl.testRefMgr.model2.TypeCollection1.Struct1", nullptr);
    refMgr.resolveStructTypeExtendsRef();
    auto base = struct1->getBase();
    EXPECT_TRUE(base != nullptr);
    EXPECT_STREQ(base->getName().c_str(), "Struct1");
}

TEST_F(ReferenceManagerTestLT, resolveStructTypeExtendsRef_cross_inf)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto interface1 = model1->getInterfaces().front();
    auto struct1 = mgr.findFStructTypeInInterface(interface1, "com.bst.idl.testRefMgr.model1.Interface1.Struct1");
    auto interface2 = mgr.findFInterface("com.bst.idl.testRefMgr.model2.Interface1");
    refMgr.addStructTypeExtendsRef(struct1, "com.bst.idl.testRefMgr.model2.Interface1.Struct1", interface2);
    refMgr.resolveStructTypeExtendsRef();
    auto base = struct1->getBase();
    EXPECT_TRUE(base != nullptr);
    EXPECT_STREQ(base->getName().c_str(), "Struct1");
}

TEST_F(ReferenceManagerTestLT, resolveStructTypeExtendsRef_wrongType_Unresolve)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto interface1 = model1->getInterfaces().front();
    auto struct1 = mgr.findFStructTypeInInterface(interface1, "com.bst.idl.testRefMgr.model1.Interface1.Struct1");
    refMgr.addStructTypeExtendsRef(struct1, "com.bst.idl.testRefMgr.model1.Interface1.Union1", interface1);
    try
    {
        refMgr.resolveStructTypeExtendsRef();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto base = struct1->getBase();
    EXPECT_TRUE(base == nullptr);
}

// cannot find the struct
TEST_F(ReferenceManagerTestLT, resolveStructTypeExtendsRef_Unresolve1)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto interface1 = model1->getInterfaces().front();
    auto struct1 = mgr.findFStructTypeInInterface(interface1, "com.bst.idl.testRefMgr.model1.Interface1.Struct1");
    refMgr.addStructTypeExtendsRef(struct1, "com.bst.idl.testRefMgr.model1.TypeCollection1.Struct2", nullptr);
    try
    {
        refMgr.resolveStructTypeExtendsRef();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto base = struct1->getBase();
    EXPECT_TRUE(base == nullptr);
}

TEST_F(ReferenceManagerTestLT, resolveStructTypeExtendsRef_Unresolve2)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto interface1 = model1->getInterfaces().front();
    auto struct1 = mgr.findFStructTypeInInterface(interface1, "com.bst.idl.testRefMgr.model1.Interface1.Struct1");
    refMgr.addStructTypeExtendsRef(struct1, "com.bst.idl.testRefMgr.model1.Interface1.Struct4", interface1);
    try
    {
        refMgr.resolveStructTypeExtendsRef();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto base = struct1->getBase();
    EXPECT_TRUE(base == nullptr);
}

TEST_F(ReferenceManagerTestLT, resolveStructTypeExtendsRef_Unresolve3)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto interface1 = model1->getInterfaces().front();
    auto interface2 = model1->getInterfaces().back();
    auto struct1 = mgr.findFStructTypeInInterface(interface1, "com.bst.idl.testRefMgr.model1.Interface1.Struct1");
    refMgr.addStructTypeExtendsRef(struct1, "com.bst.idl.testRefMgr.model1.Interface1.Struct2", interface2);
    try
    {
        refMgr.resolveStructTypeExtendsRef();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto base = struct1->getBase();
    EXPECT_TRUE(base == nullptr);
}

TEST_F(ReferenceManagerTestLT, resolveStructTypeExtendsRef_duplicate)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto interface1 = model1->getInterfaces().front();
    auto struct1 = mgr.findFStructTypeInInterface(interface1, "com.bst.idl.testRefMgr.model1.Interface1.Struct1");
    refMgr.addStructTypeExtendsRef(struct1, "com.bst.idl.testRefMgr.model1.Interface1.Struct2", interface1);
    refMgr.addStructTypeExtendsRef(struct1, "com.bst.idl.testRefMgr.model1.TypeCollection1.Struct1", nullptr);
    try
    {
        refMgr.resolveStructTypeExtendsRef();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto base = struct1->getBase();
    EXPECT_TRUE(base == nullptr);
}

TEST_F(ReferenceManagerTestLT, resolveStructTypeExtendsRef_multi)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto interface1 = model1->getInterfaces().front();
    auto struct1 = mgr.findFStructTypeInInterface(interface1, "com.bst.idl.testRefMgr.model1.Interface1.Struct1");
    refMgr.addStructTypeExtendsRef(struct1, "com.bst.idl.testRefMgr.model1.TypeCollection1.Struct1", nullptr);
    refMgr.addStructTypeExtendsRef(struct1, "com.bst.idl.testRefMgr.model1.Interface1.Struct2", interface1);
    refMgr.addStructTypeExtendsRef(struct1, "com.bst.idl.testRefMgr.model2.TypeCollection1.Struct1", nullptr);
    try
    {
        refMgr.resolveStructTypeExtendsRef();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto base = struct1->getBase();
    EXPECT_TRUE(base != nullptr);
    EXPECT_STREQ(base->getName().c_str(), "Struct1");
}

// union Extends
TEST_F(ReferenceManagerTestLT, resolveUnionTypeExtendsRef_innertc)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto interface1 = model1->getInterfaces().front();
    auto union1 = mgr.findFUnionTypeInInterface(interface1, "com.bst.idl.testRefMgr.model1.Interface1.Union1");
    refMgr.addUnionTypeExtendsRef(union1, "com.bst.idl.testRefMgr.model1.TypeCollection1.Union1", nullptr);
    refMgr.resolveUnionTypeExtendsRef();
    auto base = union1->getBase();
    EXPECT_TRUE(base != nullptr);
    EXPECT_STREQ(base->getName().c_str(), "Union1");
}

TEST_F(ReferenceManagerTestLT, resolveUnionTypeExtendsRef_innerinf)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto interface1 = model1->getInterfaces().front();
    auto union1 = mgr.findFUnionTypeInInterface(interface1, "com.bst.idl.testRefMgr.model1.Interface1.Union1");
    refMgr.addUnionTypeExtendsRef(union1, "com.bst.idl.testRefMgr.model1.Interface1.Union2", interface1);
    refMgr.resolveUnionTypeExtendsRef();
    auto base = union1->getBase();
    EXPECT_TRUE(base != nullptr);
    EXPECT_STREQ(base->getName().c_str(), "Union2");
}

TEST_F(ReferenceManagerTestLT, resolveSUnionTypeExtendsRef_cross_tc)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto interface1 = model1->getInterfaces().front();
    auto union1 = mgr.findFUnionTypeInInterface(interface1, "com.bst.idl.testRefMgr.model1.Interface1.Union1");
    refMgr.addUnionTypeExtendsRef(union1, "com.bst.idl.testRefMgr.model2.TypeCollection1.Union1", nullptr);
    refMgr.resolveUnionTypeExtendsRef();
    auto base = union1->getBase();
    EXPECT_TRUE(base != nullptr);
    EXPECT_STREQ(base->getName().c_str(), "Union1");
}

TEST_F(ReferenceManagerTestLT, resolveUnionTypeExtendsRef_cross_inf)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto interface1 = model1->getInterfaces().front();
    auto union1 = mgr.findFUnionTypeInInterface(interface1, "com.bst.idl.testRefMgr.model1.Interface1.Union1");
    auto interface2 = mgr.findFInterface("com.bst.idl.testRefMgr.model2.Interface1");
    refMgr.addUnionTypeExtendsRef(union1, "com.bst.idl.testRefMgr.model2.Interface1.Union1", interface2);
    refMgr.resolveUnionTypeExtendsRef();
    auto base = union1->getBase();
    EXPECT_TRUE(base != nullptr);
    EXPECT_STREQ(base->getName().c_str(), "Union1");
}

TEST_F(ReferenceManagerTestLT, resolveUnionTypeExtendsRef_wrongType_Unresolve)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto interface1 = model1->getInterfaces().front();
    auto union1 = mgr.findFUnionTypeInInterface(interface1, "com.bst.idl.testRefMgr.model1.Interface1.Union1");
    refMgr.addUnionTypeExtendsRef(union1, "com.bst.idl.testRefMgr.model1.Interface1.Struct1", interface1);
    try
    {
        refMgr.resolveUnionTypeExtendsRef();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto base = union1->getBase();
    EXPECT_TRUE(base == nullptr);
}

// cannot find the Union
TEST_F(ReferenceManagerTestLT, resolveUnionTypeExtendsRef_Unresolve1)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto interface1 = model1->getInterfaces().front();
    auto union1 = mgr.findFUnionTypeInInterface(interface1, "com.bst.idl.testRefMgr.model1.Interface1.Union1");
    refMgr.addUnionTypeExtendsRef(union1, "com.bst.idl.testRefMgr.model1.TypeCollection1.Union2", nullptr);
    try
    {
        refMgr.resolveUnionTypeExtendsRef();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto base = union1->getBase();
    EXPECT_TRUE(base == nullptr);
}

TEST_F(ReferenceManagerTestLT, resolveUnionTypeExtendsRef_Unresolve2)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto interface1 = model1->getInterfaces().front();
    auto union1 = mgr.findFUnionTypeInInterface(interface1, "com.bst.idl.testRefMgr.model1.Interface1.Union1");
    refMgr.addUnionTypeExtendsRef(union1, "com.bst.idl.testRefMgr.model1.Interface1.Union4", interface1);
    try
    {
        refMgr.resolveUnionTypeExtendsRef();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto base = union1->getBase();
    EXPECT_TRUE(base == nullptr);
}

TEST_F(ReferenceManagerTestLT, resolveUnionTypeExtendsRef_Unresolve3)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto interface1 = model1->getInterfaces().front();
    auto interface2 = model1->getInterfaces().back();
    auto union1 = mgr.findFUnionTypeInInterface(interface1, "com.bst.idl.testRefMgr.model1.Interface1.Union1");
    refMgr.addUnionTypeExtendsRef(union1, "com.bst.idl.testRefMgr.model1.Interface1.Union2", interface2);
    try
    {
        refMgr.resolveUnionTypeExtendsRef();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto base = union1->getBase();
    EXPECT_TRUE(base == nullptr);
}

TEST_F(ReferenceManagerTestLT, resolveUnionTypeExtendsRef_duplicate)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto interface1 = model1->getInterfaces().front();
    auto union1 = mgr.findFUnionTypeInInterface(interface1, "com.bst.idl.testRefMgr.model1.Interface1.Union1");
    refMgr.addUnionTypeExtendsRef(union1, "com.bst.idl.testRefMgr.model1.Interface1.Union2", interface1);
    refMgr.addUnionTypeExtendsRef(union1, "com.bst.idl.testRefMgr.model1.TypeCollection1.Union1", nullptr);
    try
    {
        refMgr.resolveUnionTypeExtendsRef();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto base = union1->getBase();
    EXPECT_TRUE(base == nullptr);
}

TEST_F(ReferenceManagerTestLT, resolveUnionTypeExtendsRef_multi)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto interface1 = model1->getInterfaces().front();
    auto union1 = mgr.findFUnionTypeInInterface(interface1, "com.bst.idl.testRefMgr.model1.Interface1.Union1");
    refMgr.addUnionTypeExtendsRef(union1, "com.bst.idl.testRefMgr.model1.TypeCollection1.Union1", nullptr);
    refMgr.addUnionTypeExtendsRef(union1, "com.bst.idl.testRefMgr.model1.Interface1.Union2", interface1);
    refMgr.addUnionTypeExtendsRef(union1, "com.bst.idl.testRefMgr.model2.TypeCollection1.Union1", nullptr);
    try
    {
        refMgr.resolveUnionTypeExtendsRef();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto base = union1->getBase();
    EXPECT_TRUE(base != nullptr);
    EXPECT_STREQ(base->getName().c_str(), "Union1");
}

// enumeration Extends
TEST_F(ReferenceManagerTestLT, resolveEnumerationTypeExtendsRef_innertc)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto interface1 = model1->getInterfaces().front();
    auto enum1 = mgr.findFEnumerationTypeInInterface(interface1, "com.bst.idl.testRefMgr.model1.Interface1.ErrorEnum1");
    refMgr.addEnumerationTypeExtendsRef(enum1, "com.bst.idl.testRefMgr.model1.TypeCollection1.ErrorEnum1", nullptr);
    refMgr.resolveEnumerationTypeExtendsRef();
    auto base = enum1->getBase();
    EXPECT_TRUE(base != nullptr);
    EXPECT_STREQ(base->getName().c_str(), "ErrorEnum1");
}

TEST_F(ReferenceManagerTestLT, resolveEnumerationTypeExtendsRef_innerinf)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto interface1 = model1->getInterfaces().front();
    auto enum1 = mgr.findFEnumerationTypeInInterface(interface1, "com.bst.idl.testRefMgr.model1.Interface1.ErrorEnum1");
    refMgr.addEnumerationTypeExtendsRef(enum1, "com.bst.idl.testRefMgr.model1.Interface1.ErrorEnum2", interface1);
    refMgr.resolveEnumerationTypeExtendsRef();
    auto base = enum1->getBase();
    EXPECT_TRUE(base != nullptr);
    EXPECT_STREQ(base->getName().c_str(), "ErrorEnum2");
}

TEST_F(ReferenceManagerTestLT, resolveEnumerationTypeExtendsRef_cross_tc)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto interface1 = model1->getInterfaces().front();
    auto enum1 = mgr.findFEnumerationTypeInInterface(interface1, "com.bst.idl.testRefMgr.model1.Interface1.ErrorEnum1");
    refMgr.addEnumerationTypeExtendsRef(enum1, "com.bst.idl.testRefMgr.model2.TypeCollection1.ErrorEnum1", nullptr);
    refMgr.resolveEnumerationTypeExtendsRef();
    auto base = enum1->getBase();
    EXPECT_TRUE(base != nullptr);
    EXPECT_STREQ(base->getName().c_str(), "ErrorEnum1");
}

TEST_F(ReferenceManagerTestLT, resolveEnumerationTypeExtendsRef_cross_inf)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto interface1 = model1->getInterfaces().front();
    auto enum1 = mgr.findFEnumerationTypeInInterface(interface1, "com.bst.idl.testRefMgr.model1.Interface1.ErrorEnum1");
    auto interface2 = mgr.findFInterface("com.bst.idl.testRefMgr.model2.Interface1");
    refMgr.addEnumerationTypeExtendsRef(enum1, "com.bst.idl.testRefMgr.model2.Interface1.ErrorEnum1", interface2);
    refMgr.resolveEnumerationTypeExtendsRef();
    auto base = enum1->getBase();
    EXPECT_TRUE(base != nullptr);
    EXPECT_STREQ(base->getName().c_str(), "ErrorEnum1");
}

TEST_F(ReferenceManagerTestLT, resolveEnumerationTypeExtendsRef_wrongType_Unresolve)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto interface1 = model1->getInterfaces().front();
    auto enum1 = mgr.findFEnumerationTypeInInterface(interface1, "com.bst.idl.testRefMgr.model1.Interface1.ErrorEnum1");
    refMgr.addEnumerationTypeExtendsRef(enum1, "com.bst.idl.testRefMgr.model1.Interface1.Struct1", interface1);
    try
    {
        refMgr.resolveEnumerationTypeExtendsRef();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto base = enum1->getBase();
    EXPECT_TRUE(base == nullptr);
}

// cannot find the enum
TEST_F(ReferenceManagerTestLT, resolveEnumerationTypeExtendsRef_Unresolve1)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto interface1 = model1->getInterfaces().front();
    auto enum1 = mgr.findFEnumerationTypeInInterface(interface1, "com.bst.idl.testRefMgr.model1.Interface1.ErrorEnum1");
    refMgr.addEnumerationTypeExtendsRef(enum1, "com.bst.idl.testRefMgr.model1.TypeCollection1.ErrorEnum4", nullptr);
    try
    {
        refMgr.resolveEnumerationTypeExtendsRef();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto base = enum1->getBase();
    EXPECT_TRUE(base == nullptr);
}

TEST_F(ReferenceManagerTestLT, resolveEnumerationTypeExtendsRef_Unresolve2)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto interface1 = model1->getInterfaces().front();
    auto enum1 = mgr.findFEnumerationTypeInInterface(interface1, "com.bst.idl.testRefMgr.model1.Interface1.ErrorEnum1");
    refMgr.addEnumerationTypeExtendsRef(enum1, "com.bst.idl.testRefMgr.model1.Interface1.ErrorEnum4", interface1);
    try
    {
        refMgr.resolveEnumerationTypeExtendsRef();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto base = enum1->getBase();
    EXPECT_TRUE(base == nullptr);
}

TEST_F(ReferenceManagerTestLT, resolveEnumerationTypeExtendsRef_Unresolve3)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto interface1 = model1->getInterfaces().front();
    auto interface2 = model1->getInterfaces().back();
    auto enum1 = mgr.findFEnumerationTypeInInterface(interface1, "com.bst.idl.testRefMgr.model1.Interface1.ErrorEnum1");
    refMgr.addEnumerationTypeExtendsRef(enum1, "com.bst.idl.testRefMgr.model1.Interface1.ErrorEnum2", interface2);
    try
    {
        refMgr.resolveEnumerationTypeExtendsRef();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto base = enum1->getBase();
    EXPECT_TRUE(base == nullptr);
}

TEST_F(ReferenceManagerTestLT, resolveEnumerationTypeExtendsRef_duplicate)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto interface1 = model1->getInterfaces().front();
    auto enum1 = mgr.findFEnumerationTypeInInterface(interface1, "com.bst.idl.testRefMgr.model1.Interface1.ErrorEnum1");
    refMgr.addEnumerationTypeExtendsRef(enum1, "com.bst.idl.testRefMgr.model1.Interface1.ErrorEnum2", interface1);
    refMgr.addEnumerationTypeExtendsRef(enum1, "com.bst.idl.testRefMgr.model1.TypeCollection1.ErrorEnum1", nullptr);
    try
    {
        refMgr.resolveEnumerationTypeExtendsRef();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto base = enum1->getBase();
    EXPECT_TRUE(base == nullptr);
}

TEST_F(ReferenceManagerTestLT, resolveEnumerationTypeExtendsRef_multi)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto interface1 = model1->getInterfaces().front();
    auto enum1 = mgr.findFEnumerationTypeInInterface(interface1, "com.bst.idl.testRefMgr.model1.Interface1.ErrorEnum1");
    refMgr.addEnumerationTypeExtendsRef(enum1, "com.bst.idl.testRefMgr.model1.TypeCollection1.ErrorEnum1", nullptr);
    refMgr.addEnumerationTypeExtendsRef(enum1, "com.bst.idl.testRefMgr.model1.Interface1.ErrorEnum2", interface1);
    refMgr.addEnumerationTypeExtendsRef(enum1, "com.bst.idl.testRefMgr.model2.TypeCollection1.ErrorEnum1", nullptr);
    try
    {
        refMgr.resolveEnumerationTypeExtendsRef();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto base = enum1->getBase();
    EXPECT_TRUE(base != nullptr);
    EXPECT_STREQ(base->getName().c_str(), "ErrorEnum1");
}

// Ref--Expression

TEST_F(ReferenceManagerTestLT, resolveMethodErrorRef_innerTC)
{
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto interface1 = model1->getInterfaces().front();
    auto method1 = interface1->getMethods().front();
    EXPECT_TRUE(method1->getErrorEnum() == nullptr);
    refMgr.addMethodErrorRef(method1, "com.bst.idl.testRefMgr.model1.TypeCollection1.ErrorEnum1", nullptr);
    refMgr.resolveMethodErrorRef();
    EXPECT_TRUE(method1->getErrorEnum() != nullptr);
}

TEST_F(ReferenceManagerTestLT, resolveMethodErrorRef_innerIF)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto interface1 = model1->getInterfaces().front();
    auto method1 = interface1->getMethods().front();
    EXPECT_TRUE(method1->getErrorEnum() == nullptr);
    refMgr.addMethodErrorRef(method1, "com.bst.idl.testRefMgr.model1.Interface1.ErrorEnum1", interface1);
    refMgr.resolveMethodErrorRef();
    EXPECT_TRUE(method1->getErrorEnum() != nullptr);
}

TEST_F(ReferenceManagerTestLT, resolveMethodErrorRef_inner_annotherIF)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto interface1 = model1->getInterfaces().front();
    auto interface2 = model1->getInterfaces().back();
    auto method1 = interface1->getMethods().front();
    EXPECT_TRUE(method1->getErrorEnum() == nullptr);
    refMgr.addMethodErrorRef(method1, "com.bst.idl.testRefMgr.model1.Interface2.ErrorEnum1", interface2);
    refMgr.resolveMethodErrorRef();
    EXPECT_TRUE(method1->getErrorEnum() != nullptr);
}

TEST_F(ReferenceManagerTestLT, resolveMethodErrorRef_crossTC)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto interface1 = model1->getInterfaces().front();
    auto method1 = interface1->getMethods().front();
    EXPECT_TRUE(method1->getErrorEnum() == nullptr);
    refMgr.addMethodErrorRef(method1, "com.bst.idl.testRefMgr.model2.TypeCollection1.ErrorEnum1", nullptr);
    refMgr.resolveMethodErrorRef();
    EXPECT_TRUE(method1->getErrorEnum() != nullptr);
}

TEST_F(ReferenceManagerTestLT, resolveMethodErrorRef_crossIF)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto interface1 = model1->getInterfaces().front();
    auto method1 = interface1->getMethods().front();
    auto model2 = mgr.findModel("com.bst.idl.testRefMgr.model2");
    auto interface2_1 = model2->getInterfaces().front();
    EXPECT_TRUE(method1->getErrorEnum() == nullptr);
    refMgr.addMethodErrorRef(method1, "com.bst.idl.testRefMgr.model2.Interface1.ErrorEnum1", interface2_1);
    refMgr.resolveMethodErrorRef();
    EXPECT_TRUE(method1->getErrorEnum() != nullptr);
}

// cannot find the type in typecollection
TEST_F(ReferenceManagerTestLT, resolveMethodErrorRef_unresolved_TC)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto interface1 = model1->getInterfaces().front();
    auto method1 = interface1->getMethods().front();
    refMgr.addMethodErrorRef(method1, "com.bst.idl.testRefMgr.model1.Typecollection1.ErrorEnum3", nullptr);
    try
    {
        refMgr.resolveMethodErrorRef();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    EXPECT_TRUE(method1->getErrorEnum() == nullptr);
}

// cannot find the type in interfaces
TEST_F(ReferenceManagerTestLT, resolveMethodErrorRef_unresolved_IF)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto interface1 = model1->getInterfaces().front();
    auto method1 = interface1->getMethods().front();
    refMgr.addMethodErrorRef(method1, "com.bst.idl.testRefMgr.model1.Interface1.ErrorEnum4", interface1);
    try
    {
        refMgr.resolveMethodErrorRef();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    EXPECT_TRUE(method1->getErrorEnum() == nullptr);
}

TEST_F(ReferenceManagerTestLT, resolveMethodErrorRef_dulplicated)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto interface1 = model1->getInterfaces().front();
    auto interface2 = model1->getInterfaces().back();
    auto method1 = interface1->getMethods().front();
    refMgr.addMethodErrorRef(method1, "com.bst.idl.testRefMgr.model1.Interface1.ErrorEnum2", interface1);
    refMgr.addMethodErrorRef(method1, "com.bst.idl.testRefMgr.model1.Interface2.ErrorEnum1", interface2);

    try
    {
        refMgr.resolveMethodErrorRef();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    EXPECT_TRUE(method1->getErrorEnum() == nullptr);
}

TEST_F(ReferenceManagerTestLT, resolveMethodErrorRef__multiOddRecords)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto interface1 = model1->getInterfaces().front();
    auto method1 = interface1->getMethods().front();
    refMgr.addMethodErrorRef(method1, "com.bst.idl.testRefMgr.model1.Interface1.ErrorEnum1", interface1);
    refMgr.addMethodErrorRef(method1, "com.bst.idl.testRefMgr.model1.TypeCollection1.ErrorEnum1", nullptr);
    refMgr.addMethodErrorRef(method1, "com.bst.idl.testRefMgr.model1.Interface1.ErrorEnum2", interface1);
    try
    {
        refMgr.resolveMethodErrorRef();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto errEnum = method1->getErrorEnum();
    EXPECT_TRUE(errEnum != nullptr);
    EXPECT_STREQ(errEnum->getName().c_str(), "ErrorEnum2");
}

// resolveTypeRefDerivedRef
TEST_F(ReferenceManagerTestLT, resolveTypeRefDerivedRef_tc)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto typeref1 = factory.createFTypeRef();
    refMgr.addTypeRefDerivedRef(typeref1, "com.bst.idl.testRefMgr.model1.TypeCollection1.Struct1", nullptr);
    refMgr.resolveTypeRefDerviedRef();
    auto derived = typeref1->getDerived();
    EXPECT_TRUE(derived != nullptr);
    EXPECT_STREQ(derived->getName().c_str(), "Struct1");
}

TEST_F(ReferenceManagerTestLT, resolveTypeRefDerivedRef_inf)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto interface1 = model1->getInterfaces().front();
    auto typeref1 = factory.createFTypeRef();
    refMgr.addTypeRefDerivedRef(typeref1, "com.bst.idl.testRefMgr.model1.Interface1.Struct1", interface1);
    refMgr.resolveTypeRefDerviedRef();
    auto derived = typeref1->getDerived();
    EXPECT_TRUE(derived != nullptr);
    EXPECT_STREQ(derived->getName().c_str(), "Struct1");
}

// fqn&interfsce not point to the same interface
TEST_F(ReferenceManagerTestLT, resolveTypeRefDerivedRef_unresolved_1)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto interface2 = model1->getInterfaces().back();
    auto typeref1 = factory.createFTypeRef();
    refMgr.addTypeRefDerivedRef(typeref1, "com.bst.idl.testRefMgr.model1.Interface1.Struct1", interface2);
    try
    {
        refMgr.resolveTypeRefDerviedRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto derived = typeref1->getDerived();
    EXPECT_TRUE(derived == nullptr);
}

// cannot find the type in typecollection
TEST_F(ReferenceManagerTestLT, resolveTypeRefDerivedRef_unresolved_2)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto interface2 = model1->getInterfaces().back();
    auto typeref1 = factory.createFTypeRef();
    refMgr.addTypeRefDerivedRef(typeref1, "com.bst.idl.testRefMgr.model1.TypeCollection1.Struct4", interface2);
    try
    {
        refMgr.resolveTypeRefDerviedRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto derived = typeref1->getDerived();
    EXPECT_TRUE(derived == nullptr);
}

// cannot find the type in interface
TEST_F(ReferenceManagerTestLT, resolveTypeRefDerivedRef_unresolved_3)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto interface1 = model1->getInterfaces().front();
    auto typeref1 = factory.createFTypeRef();
    refMgr.addTypeRefDerivedRef(typeref1, "com.bst.idl.testRefMgr.model1.Interface1.Struct4", interface1);
    try
    {
        refMgr.resolveTypeRefDerviedRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto derived = typeref1->getDerived();
    EXPECT_TRUE(derived == nullptr);
}

TEST_F(ReferenceManagerTestLT, resolveTypeRefDerivedRef_dulplicate)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto interface1 = model1->getInterfaces().front();
    auto typeref1 = factory.createFTypeRef();
    refMgr.addTypeRefDerivedRef(typeref1, "com.bst.idl.testRefMgr.model1.Interface1.Struct1", interface1);
    refMgr.addTypeRefDerivedRef(typeref1, "com.bst.idl.testRefMgr.model1.Interface1.Array1", interface1);
    try
    {
        refMgr.resolveTypeRefDerviedRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto derived = typeref1->getDerived();
    EXPECT_TRUE(derived == nullptr);
}

TEST_F(ReferenceManagerTestLT, resolveTypeRefDerivedRef_multiOddRecords)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto interface1 = model1->getInterfaces().front();
    auto typeref1 = factory.createFTypeRef();
    refMgr.addTypeRefDerivedRef(typeref1, "com.bst.idl.testRefMgr.model1.Interface1.Struct1", interface1);
    refMgr.addTypeRefDerivedRef(typeref1, "com.bst.idl.testRefMgr.model1.Interface1.Array1", interface1);
    refMgr.addTypeRefDerivedRef(typeref1, "com.bst.idl.testRefMgr.model1.TypeCollection1.ErrorEnum1", interface1);
    try
    {
        refMgr.resolveTypeRefDerviedRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto errorEnum1 = typeref1->getDerived();
    EXPECT_TRUE(errorEnum1 != nullptr);
    EXPECT_STREQ(errorEnum1->getName().c_str(), "ErrorEnum1");
    auto tc = model1->getTypeCollections().front();
}

// resolveQerElementRef
TEST_F(ReferenceManagerTestLT, resolveQerElementRef_tc)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto tc = model1->getTypeCollections().front();
    auto cstruct = tc->getConstants().front();
    auto qer = factory.createFQualifiedElementRef();
    refMgr.addQerElementRef(qer, "com.bst.idl.testRefMgr.model1.TypeCollection1.ConstStruct1", nullptr);
    refMgr.resolveQerElementRef();
    EXPECT_EQ(qer->getElement(), cstruct);
}

TEST_F(ReferenceManagerTestLT, resolveQerElementRef_inf)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto interface1 = model1->getInterfaces().front();
    auto cstruct = interface1->getConstants().front();
    auto qer = factory.createFQualifiedElementRef();
    refMgr.addQerElementRef(qer, "com.bst.idl.testRefMgr.model1.Interface1.ConstStruct1", interface1);
    refMgr.resolveQerElementRef();
    EXPECT_EQ(qer->getElement(), cstruct);
}

TEST_F(ReferenceManagerTestLT, resolveQerElementRef_unresolved1)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto interface2 = model1->getInterfaces().back();
    auto qer = factory.createFQualifiedElementRef();
    refMgr.addQerElementRef(qer, "com.bst.idl.testRefMgr.model1.Interface1.ConstStruct1", interface2);
    try
    {
        refMgr.resolveQerElementRef();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    EXPECT_EQ(qer->getElement(), nullptr);
}

TEST_F(ReferenceManagerTestLT, resolveQerElementRef_unresolved2)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto interface2 = model1->getInterfaces().back();
    auto qer = factory.createFQualifiedElementRef();
    refMgr.addQerElementRef(qer, "com.bst.idl.testRefMgr.model1.TypeCollection1.ConstStruct3", interface2);
    try
    {
        refMgr.resolveQerElementRef();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    EXPECT_EQ(qer->getElement(), nullptr);
}

TEST_F(ReferenceManagerTestLT, resolveQerElementRef_unresolved3)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto interface1 = model1->getInterfaces().front();
    auto qer = factory.createFQualifiedElementRef();
    refMgr.addQerElementRef(qer, "com.bst.idl.testRefMgr.model1.Interface1.ConstStruct6", interface1);
    try
    {
        refMgr.resolveQerElementRef();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    EXPECT_EQ(qer->getElement(), nullptr);
}

TEST_F(ReferenceManagerTestLT, resolveQerElementRef_dulplicate)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto interface1 = model1->getInterfaces().front();
    auto qer = factory.createFQualifiedElementRef();
    refMgr.addQerElementRef(qer, "com.bst.idl.testRefMgr.model1.Interface1.ConstStruct1", interface1);
    refMgr.addQerElementRef(qer, "com.bst.idl.testRefMgr.model1.TypeCollection1.ConstStruct1", nullptr);
    try
    {
        refMgr.resolveQerElementRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    EXPECT_EQ(qer->getElement(), nullptr);
}

TEST_F(ReferenceManagerTestLT, resolveQerElementRef_multiOddRecords)
{
    refMgr.clear();
    auto model1 = mgr.findModel("com.bst.idl.testRefMgr.model1");
    auto interface1 = model1->getInterfaces().front();
    auto qer = factory.createFQualifiedElementRef();
    refMgr.addQerElementRef(qer, "com.bst.idl.testRefMgr.model1.Interface1.ConstStruct1", interface1);
    refMgr.addQerElementRef(qer, "com.bst.idl.testRefMgr.model1.TypeCollection1.ConstStruct2", interface1);
    refMgr.addQerElementRef(qer, "com.bst.idl.testRefMgr.model1.TypeCollection1.ConstStruct1", interface1);
    try
    {
        refMgr.resolveQerElementRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto element = qer->getElement();
    EXPECT_TRUE(element != nullptr);
    EXPECT_STREQ(element->getName().c_str(), "ConstStruct1");
}
