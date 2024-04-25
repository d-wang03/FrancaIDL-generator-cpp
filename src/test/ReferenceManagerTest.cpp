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
#include "validator/ReferenceManager.h"
#include "model/FFactory.h"
#include "model/FModelManager.h"
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
    // add TypeCollection1.ErrEnum1
    auto errEnum1 = FFactory::getInstance().createFEnumerationType();
    errEnum1->setName("ErrEnum1");
    addListItem(tc1->getTypes(), std::move(errEnum1));
    // add TypeCollection1.TypeDef1
    auto typedef1 = FFactory::getInstance().createFTypeDef();
    typedef1->setName("TypeDef1");
    addListItem(tc1->getTypes(), std::move(typedef1));
    // add TypeCollection1.Array1
    auto array1 = FFactory::getInstance().createFArrayType();
    array1->setName("Array1");
    addListItem(tc1->getTypes(), std::move(array1));
    // add TypeCollection1.Struct1
    auto struct1 = FFactory::getInstance().createFStructType();
    struct1->setName("Struct1");
    addListItem(tc1->getTypes(), std::move(struct1));
    // add TypeCollection1.Union1
    auto union1 = FFactory::getInstance().createFUnionType();
    union1->setName("Union1");
    addListItem(tc1->getTypes(), std::move(union1));
    // add TypeCollection1.Constant1
    auto constant1 = FFactory::getInstance().createFConstantDef();
    constant1->setName("Constant1");
    addListItem(tc1->getConstants(), std::move(constant1));
    // add TypeCollection1 to model1
    addListItem(model1->getTypeCollections(), std::move(tc1));

    // add Interface1
    auto interface1 = FFactory::getInstance().createFInterface();
    interface1->setName("Interface1");
    // add Interface1.Method1
    auto method1 = FFactory::getInstance().createFMethod();
    method1->setName("Method1");
    addListItem(interface1->getMethods(), std::move(method1));
    // add Interface1.ErrEnum1
    errEnum1 = FFactory::getInstance().createFEnumerationType();
    errEnum1->setName("ErrEnum1");
    addListItem(interface1->getTypes(), std::move(errEnum1));
    // add Interface1.TypeDef1
    typedef1 = FFactory::getInstance().createFTypeDef();
    typedef1->setName("TypeDef1");
    addListItem(interface1->getTypes(), std::move(typedef1));
    // add Interface1.Array1
    array1 = FFactory::getInstance().createFArrayType();
    array1->setName("Array1");
    addListItem(interface1->getTypes(), std::move(array1));
    // add Interface1.Struct1
    struct1 = FFactory::getInstance().createFStructType();
    struct1->setName("Struct1");
    addListItem(interface1->getTypes(), std::move(struct1));
    // add Interface1.Union1
    union1 = FFactory::getInstance().createFUnionType();
    union1->setName("Union1");
    addListItem(interface1->getTypes(), std::move(union1));
    // add Interface1.Constant1
    constant1 = FFactory::getInstance().createFConstantDef();
    constant1->setName("Constant1");
    addListItem(interface1->getConstants(), std::move(constant1));
    // add Interface1 to model1
    interface1->setContainer(model1);
    addListItem(model1->getInterfaces(), std::move(interface1));

    // add interface 2
    auto interface2 = FFactory::getInstance().createFInterface();
    interface2->setName("Interface2");
    // add Interface2.ErrEnum1
    errEnum1 = FFactory::getInstance().createFEnumerationType();
    errEnum1->setName("ErrEnum1");
    addListItem(interface2->getTypes(), std::move(errEnum1));
    // add Interface2.TypeDef1
    typedef1 = FFactory::getInstance().createFTypeDef();
    typedef1->setName("TypeDef1");
    addListItem(interface2->getTypes(), std::move(typedef1));
    // add Interface2.Array1
    array1 = FFactory::getInstance().createFArrayType();
    array1->setName("Array1");
    addListItem(interface2->getTypes(), std::move(array1));
    // add Interface2.Struct1
    struct1 = FFactory::getInstance().createFStructType();
    struct1->setName("Struct1");
    addListItem(interface2->getTypes(), std::move(struct1));
    // add Interface2.Union1
    union1 = FFactory::getInstance().createFUnionType();
    union1->setName("Union1");
    addListItem(interface2->getTypes(), std::move(union1));
    // add Interface2 to model1
    interface2->setContainer(model1);
    addListItem(model1->getInterfaces(), std::move(interface2));

    // set model 2
    auto model2 = mgr.addModel("com.bst.idl.test.model2");
    // add interface 1
    interface1 = FFactory::getInstance().createFInterface();
    interface1->setName("Interface1");
    // add Interface1.ErrEnum1
    errEnum1 = FFactory::getInstance().createFEnumerationType();
    errEnum1->setName("ErrEnum1");
    addListItem(interface1->getTypes(), std::move(errEnum1));
    // add Interface1.TypeDef1
    typedef1 = FFactory::getInstance().createFTypeDef();
    typedef1->setName("TypeDef1");
    addListItem(interface1->getTypes(), std::move(typedef1));
    // add Interface1.Array1
    array1 = FFactory::getInstance().createFArrayType();
    array1->setName("Array1");
    addListItem(interface1->getTypes(), std::move(array1));
    // add Interface1.Struct1
    struct1 = FFactory::getInstance().createFStructType();
    struct1->setName("Struct1");
    addListItem(interface1->getTypes(), std::move(struct1));
    // add Interface1.Union1
    union1 = FFactory::getInstance().createFUnionType();
    union1->setName("Union1");
    addListItem(interface1->getTypes(), std::move(union1));
    // add Interface1 to model 2
    interface1->setContainer(model2);
    addListItem(model2->getInterfaces(), std::move(interface1));

    // add interface 2
    interface2 = FFactory::getInstance().createFInterface();
    interface2->setName("Interface2");
    // add Interface2.ErrEnum1
    errEnum1 = FFactory::getInstance().createFEnumerationType();
    errEnum1->setName("ErrEnum1");
    addListItem(interface2->getTypes(), std::move(errEnum1));
    // add Interface2.TypeDef1
    typedef1 = FFactory::getInstance().createFTypeDef();
    typedef1->setName("TypeDef1");
    addListItem(interface2->getTypes(), std::move(typedef1));
    // add Interface2.Array1
    array1 = FFactory::getInstance().createFArrayType();
    array1->setName("Array1");
    addListItem(interface2->getTypes(), std::move(array1));
    // add Interface2.Struct1
    struct1 = FFactory::getInstance().createFStructType();
    struct1->setName("Struct1");
    addListItem(interface2->getTypes(), std::move(struct1));
    // add Interface2.Union1
    union1 = FFactory::getInstance().createFUnionType();
    union1->setName("Union1");
    addListItem(interface2->getTypes(), std::move(union1));
    // add Interface2 to model 2
    interface2->setContainer(model2);
    addListItem(model2->getInterfaces(), std::move(interface2));
}

TEST(ReferenceManagerTest, resolveInterfaceExtendsRef_insideModel)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    refmgr.addInterfaceExtendsRef(interface1, "com.bst.idl.test.model1.Interface2");

    refmgr.resolveInterfaceExtendsRef();
    auto base = interface1->getBase();
    EXPECT_TRUE(base != nullptr);
    EXPECT_STREQ(base->getName().c_str(), "Interface2");
}

TEST(ReferenceManagerTest, resolveInterfaceExtendsRef_crossModel)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    refmgr.addInterfaceExtendsRef(interface1, "com.bst.idl.test.model2.Interface2");

    refmgr.resolveInterfaceExtendsRef();
    auto base = interface1->getBase();
    EXPECT_TRUE(base != nullptr);
    EXPECT_STREQ(base->getName().c_str(), "Interface2");
}

TEST(ReferenceManagerTest, resolveInterfaceExtendsRef_unresolved)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    refmgr.addInterfaceExtendsRef(interface1, "com.bst.idl.test.model2.Interface3");
    try
    {
        refmgr.resolveInterfaceExtendsRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }

    auto base = interface1->getBase();
    EXPECT_TRUE(base == nullptr);
}

TEST(ReferenceManagerTest, resolveInterfaceExtendsRef_duplicate)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    refmgr.addInterfaceExtendsRef(interface1, "com.bst.idl.test.model1.Interface2");
    refmgr.addInterfaceExtendsRef(interface1, "com.bst.idl.test.model2.Interface2");
    try
    {
        refmgr.resolveInterfaceExtendsRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto base = interface1->getBase();
    EXPECT_TRUE(base == nullptr);
}

TEST(ReferenceManagerTest, resolveInterfaceExtendsRef_multipleRecord)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    refmgr.addInterfaceExtendsRef(interface1, "com.bst.idl.test.model1.Interface3");
    refmgr.addInterfaceExtendsRef(interface1, "com.bst.idl.test.model2.Interface2");
    try
    {
        refmgr.resolveInterfaceExtendsRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto base = interface1->getBase();
    EXPECT_TRUE(base != nullptr);
    EXPECT_STREQ(base->getName().c_str(), "Interface2");
}

TEST(ReferenceManagerTest, resolveInterfaceManagesRef_insideModel)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    refmgr.addInterfaceManagesRef(interface1, "com.bst.idl.test.model1.Interface2");

    auto interface2 = model1->getInterfaces().back();
    refmgr.resolveInterfaceManagesRef();
    auto manages = interface1->getManagedInterfaces();
    EXPECT_EQ(manages.size(), 1U);
    EXPECT_TRUE(manages.front() != nullptr);
    EXPECT_STREQ(manages.front()->getName().c_str(), "Interface2");
}

TEST(ReferenceManagerTest, resolveInterfaceManagesRef_crossModel)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    refmgr.addInterfaceManagesRef(interface1, "com.bst.idl.test.model2.Interface2");

    auto model2 = FModelManager::getInstance().findModel("com.bst.idl.test.model2");
    auto interface2 = model2->getInterfaces().back();
    refmgr.resolveInterfaceManagesRef();
    auto manages = interface1->getManagedInterfaces();
    EXPECT_EQ(manages.size(), 1U);
    EXPECT_TRUE(manages.front() != nullptr);
    EXPECT_STREQ(manages.front()->getName().c_str(), "Interface2");
}

TEST(ReferenceManagerTest, resolveInterfaceManagesRef_unresolved)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    refmgr.addInterfaceManagesRef(interface1, "com.bst.idl.test.model3.Interface2");
    refmgr.addInterfaceManagesRef(interface1, "com.bst.idl.test.model4.Interface2");

    try
    {
        refmgr.resolveInterfaceManagesRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto manages = interface1->getManagedInterfaces();
    EXPECT_TRUE(manages.empty());
}

TEST(ReferenceManagerTest, resolveInterfaceManagesRef_duplicate)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    refmgr.addInterfaceManagesRef(interface1, "com.bst.idl.test.model1.Interface2");
    refmgr.addInterfaceManagesRef(interface1, "com.bst.idl.test.model2.Interface2");
    try
    {
        refmgr.resolveInterfaceManagesRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto manages = interface1->getManagedInterfaces();
    EXPECT_EQ(manages.size(), 2U);
}

TEST(ReferenceManagerTest, resolveInterfaceManagesRef_multiRecord)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    refmgr.addInterfaceManagesRef(interface1, "com.bst.idl.test.model0.Interface2");
    refmgr.addInterfaceManagesRef(interface1, "com.bst.idl.test.model2.Interface2");
    refmgr.addInterfaceManagesRef(interface1, "com.bst.idl.test.model3.Interface2");
    try
    {
        refmgr.resolveInterfaceManagesRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto manages = interface1->getManagedInterfaces();
    EXPECT_EQ(manages.size(), 1U);
    EXPECT_TRUE(manages.front() != nullptr);
    EXPECT_STREQ(manages.front()->getName().c_str(), "Interface2");
}

TEST(ReferenceManagerTest, resolveMethodErrorRef_insideInterface)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    auto method1 = interface1->getMethods().front();
    refmgr.addMethodErrorRef(method1, "com.bst.idl.test.model1.Interface1.ErrEnum1", interface1);

    refmgr.resolveMethodErrorRef();
    auto errEnum = method1->getErrorEnum();
    EXPECT_TRUE(errEnum != nullptr);
    EXPECT_STREQ(errEnum->getName().c_str(), "ErrEnum1");
    EXPECT_TRUE(errEnum == interface1->getTypes().front());
}

TEST(ReferenceManagerTest, resolveMethodErrorRef_crossInterface)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    auto method1 = interface1->getMethods().front();
    refmgr.addMethodErrorRef(method1, "com.bst.idl.test.model1.Interface2.ErrEnum1", interface1);
    try
    {
        refmgr.resolveMethodErrorRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto errEnum = method1->getErrorEnum();
    EXPECT_TRUE(errEnum == nullptr);
}

TEST(ReferenceManagerTest, resolveMethodErrorRef_crossModel)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    auto method1 = interface1->getMethods().front();
    refmgr.addMethodErrorRef(method1, "com.bst.idl.test.model2.Interface2.ErrEnum1", interface1);
    try
    {
        refmgr.resolveMethodErrorRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto errEnum = method1->getErrorEnum();
    EXPECT_TRUE(errEnum == nullptr);
}

TEST(ReferenceManagerTest, resolveMethodErrorRef_unresolved)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    auto method1 = interface1->getMethods().front();
    refmgr.addMethodErrorRef(method1, "com.bst.idl.test.model0.Interface2.ErrEnum1", interface1);
    refmgr.addMethodErrorRef(method1, "com.bst.idl.test.model1.Interface0.ErrEnum1", interface1);
    refmgr.addMethodErrorRef(method1, "com.bst.idl.test.model1.Interface3.ErrEnum1", interface1);
    try
    {
        refmgr.resolveMethodErrorRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto errEnum = method1->getErrorEnum();
    EXPECT_TRUE(errEnum == nullptr);
}

TEST(ReferenceManagerTest, resolveMethodErrorRef_duplicate)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    auto method1 = interface1->getMethods().front();
    refmgr.addMethodErrorRef(method1, "com.bst.idl.test.model1.TypeCollection1.ErrEnum1", interface1);
    refmgr.addMethodErrorRef(method1, "com.bst.idl.test.model1.Interface1.ErrEnum1", interface1);
    refmgr.addMethodErrorRef(method1, "com.bst.idl.test.model2.Interface1.ErrEnum1", interface1);
    try
    {
        refmgr.resolveMethodErrorRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto errEnum = method1->getErrorEnum();
    EXPECT_TRUE(errEnum == nullptr);
}

TEST(ReferenceManagerTest, resolveMethodErrorRef_multiRecord)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    auto method1 = interface1->getMethods().front();
    refmgr.addMethodErrorRef(method1, "com.bst.idl.test.model0.Interface1.ErrEnum1", interface1);
    refmgr.addMethodErrorRef(method1, "com.bst.idl.test.model1.Interface1.ErrEnum1", interface1);
    refmgr.addMethodErrorRef(method1, "com.bst.idl.test.model3.Interface3.ErrEnum1", interface1);
    try
    {
        refmgr.resolveMethodErrorRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto errEnum = method1->getErrorEnum();
    EXPECT_TRUE(errEnum != nullptr);
    EXPECT_STREQ(errEnum->getName().c_str(), "ErrEnum1");
    EXPECT_TRUE(errEnum == interface1->getTypes().front());
}

TEST(ReferenceManagerTest, resolveTypeRefDerviedRef)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    auto typeref1 = FFactory::getInstance().createFTypeRef();
    refmgr.addTypeRefDerivedRef(typeref1, "com.bst.idl.test.model1.Interface1.Struct1", interface1);
    refmgr.resolveTypeRefDerviedRef();
    auto struct1 = typeref1->getDerived();
    EXPECT_TRUE(struct1 != nullptr);
    EXPECT_STREQ(struct1->getName().c_str(), "Struct1");
}

TEST(ReferenceManagerTest, resolveTypeRefDerviedRef_unresolved)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    auto typeref1 = FFactory::getInstance().createFTypeRef();
    refmgr.addTypeRefDerivedRef(typeref1, "com.bst.idl.test.model1.Interface2.Struct1", interface1);
    try
    {
        refmgr.resolveTypeRefDerviedRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto struct1 = typeref1->getDerived();
    EXPECT_TRUE(struct1 == nullptr);
}

TEST(ReferenceManagerTest, resolveTypeRefDerviedRef_duplicate)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    auto typeref1 = FFactory::getInstance().createFTypeRef();
    refmgr.addTypeRefDerivedRef(typeref1, "com.bst.idl.test.model1.Interface1.Struct1", interface1);
    refmgr.addTypeRefDerivedRef(typeref1, "com.bst.idl.test.model1.TypeCollection1.Struct1", interface1);
    try
    {
        refmgr.resolveTypeRefDerviedRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto struct1 = typeref1->getDerived();
    EXPECT_TRUE(struct1 == nullptr);
}

TEST(ReferenceManagerTest, resolveTypeRefDerviedRef_multiRecord)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    auto typeref1 = FFactory::getInstance().createFTypeRef();
    refmgr.addTypeRefDerivedRef(typeref1, "com.bst.idl.test.model2.Interface1.Struct1", interface1);
    refmgr.addTypeRefDerivedRef(typeref1, "com.bst.idl.test.model1.TypeCollection1.Struct1", interface1);
    refmgr.addTypeRefDerivedRef(typeref1, "com.bst.idl.test.model1.Interface2.Struct2", interface1);
    try
    {
        refmgr.resolveTypeRefDerviedRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto struct1 = typeref1->getDerived();
    EXPECT_TRUE(struct1 != nullptr);
    EXPECT_STREQ(struct1->getName().c_str(), "Struct1");
}

TEST(ReferenceManagerTest, resolveStructTypeExtendsRef)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    auto struct2 = FFactory::getInstance().createFStructType();
    refmgr.addStructTypeExtendsRef(struct2, "com.bst.idl.test.model1.Interface1.Struct1", interface1);
    refmgr.resolveStructTypeExtendsRef();
    auto struct1 = struct2->getBase();
    EXPECT_TRUE(struct1 != nullptr);
    EXPECT_STREQ(struct1->getName().c_str(), "Struct1");
}

TEST(ReferenceManagerTest, resolveStructTypeExtendsRef_unresolved)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    auto struct2 = FFactory::getInstance().createFStructType();
    refmgr.addStructTypeExtendsRef(struct2, "com.bst.idl.test.model1.Interface2.Struct1", interface1);
    try
    {
        refmgr.resolveStructTypeExtendsRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto struct1 = struct2->getBase();
    EXPECT_TRUE(struct1 == nullptr);
}

TEST(ReferenceManagerTest, resolveStructTypeExtendsRef_duplicate)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    auto struct2 = FFactory::getInstance().createFStructType();
    refmgr.addStructTypeExtendsRef(struct2, "com.bst.idl.test.model1.Interface1.Struct1", interface1);
    refmgr.addStructTypeExtendsRef(struct2, "com.bst.idl.test.model1.TypeCollection1.Struct1", interface1);
    try
    {
        refmgr.resolveStructTypeExtendsRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto struct1 = struct2->getBase();
    EXPECT_TRUE(struct1 == nullptr);
}

TEST(ReferenceManagerTest, resolveStructTypeExtendsRef_multiRecord)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    auto struct2 = FFactory::getInstance().createFStructType();
    refmgr.addStructTypeExtendsRef(struct2, "com.bst.idl.test.model1.Interface2.Struct1", interface1);
    refmgr.addStructTypeExtendsRef(struct2, "com.bst.idl.test.model2.Interface1.Struct1", interface1);
    refmgr.addStructTypeExtendsRef(struct2, "com.bst.idl.test.model1.Interface1.Struct1", interface1);
    try
    {
        refmgr.resolveStructTypeExtendsRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto struct1 = struct2->getBase();
    EXPECT_TRUE(struct1 != nullptr);
    EXPECT_STREQ(struct1->getName().c_str(), "Struct1");
}

TEST(ReferenceManagerTest, resolveUnionTypeExtendsRef)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    auto union2 = FFactory::getInstance().createFUnionType();
    refmgr.addUnionTypeExtendsRef(union2, "com.bst.idl.test.model1.Interface1.Union1", interface1);
    refmgr.resolveUnionTypeExtendsRef();
    auto union1 = union2->getBase();
    EXPECT_TRUE(union1 != nullptr);
    EXPECT_STREQ(union1->getName().c_str(), "Union1");
}

TEST(ReferenceManagerTest, resolveUnionTypeExtendsRef_unresolved)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    auto union2 = FFactory::getInstance().createFUnionType();
    refmgr.addUnionTypeExtendsRef(union2, "com.bst.idl.test.model1.Interface2.Union1", interface1);
    try
    {
        refmgr.resolveUnionTypeExtendsRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto union1 = union2->getBase();
    EXPECT_TRUE(union1 == nullptr);
}

TEST(ReferenceManagerTest, resolveUnionTypeExtendsRef_duplicate)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    auto union2 = FFactory::getInstance().createFUnionType();
    refmgr.addUnionTypeExtendsRef(union2, "com.bst.idl.test.model1.Interface1.Union1", interface1);
    refmgr.addUnionTypeExtendsRef(union2, "com.bst.idl.test.model1.TypeCollection1.Union1", interface1);
    try
    {
        refmgr.resolveUnionTypeExtendsRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto union1 = union2->getBase();
    EXPECT_TRUE(union1 == nullptr);
}

TEST(ReferenceManagerTest, resolveUnionTypeExtendsRef_multiRecord)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    auto union2 = FFactory::getInstance().createFUnionType();
    refmgr.addUnionTypeExtendsRef(union2, "com.bst.idl.test.model1.Interface2.Union1", interface1);
    refmgr.addUnionTypeExtendsRef(union2, "com.bst.idl.test.model1.TypeCollection1.Union1", interface1);
    refmgr.addUnionTypeExtendsRef(union2, "com.bst.idl.test.model2.Interface1.Union1", interface1);
    try
    {
        refmgr.resolveUnionTypeExtendsRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto union1 = union2->getBase();
    EXPECT_TRUE(union1 != nullptr);
    EXPECT_STREQ(union1->getName().c_str(), "Union1");
}

TEST(ReferenceManagerTest, resolveEnumerationTypeExtendsRef)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    auto enum2 = FFactory::getInstance().createFEnumerationType();
    refmgr.addEnumerationTypeExtendsRef(enum2, "com.bst.idl.test.model1.Interface1.ErrEnum1", interface1);
    refmgr.resolveEnumerationTypeExtendsRef();
    auto enum1 = enum2->getBase();
    EXPECT_TRUE(enum1 != nullptr);
    EXPECT_STREQ(enum1->getName().c_str(), "ErrEnum1");
}

TEST(ReferenceManagerTest, resolveEnumerationTypeExtendsRef_unresolved)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    auto enum2 = FFactory::getInstance().createFEnumerationType();
    refmgr.addEnumerationTypeExtendsRef(enum2, "com.bst.idl.test.model2.Interface1.ErrEnum1", interface1);
    try
    {
        refmgr.resolveEnumerationTypeExtendsRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto enum1 = enum2->getBase();
    EXPECT_TRUE(enum1 == nullptr);
}

TEST(ReferenceManagerTest, resolveEnumerationTypeExtendsRef_duplicate)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    auto enum2 = FFactory::getInstance().createFEnumerationType();
    refmgr.addEnumerationTypeExtendsRef(enum2, "com.bst.idl.test.model1.Interface1.ErrEnum1", interface1);
    refmgr.addEnumerationTypeExtendsRef(enum2, "com.bst.idl.test.model1.TypeCollection1.ErrEnum1", interface1);
    try
    {
        refmgr.resolveEnumerationTypeExtendsRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto enum1 = enum2->getBase();
    EXPECT_TRUE(enum1 == nullptr);
}

TEST(ReferenceManagerTest, resolveEnumerationTypeExtendsRef_multiRecord)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    auto enum2 = FFactory::getInstance().createFEnumerationType();
    refmgr.addEnumerationTypeExtendsRef(enum2, "com.bst.idl.test.model2.Interface1.ErrEnum1", interface1);
    refmgr.addEnumerationTypeExtendsRef(enum2, "com.bst.idl.test.model1.TypeCollection1.ErrEnum1", interface1);
    refmgr.addEnumerationTypeExtendsRef(enum2, "com.bst.idl.test.model1.Interface2.ErrEnum1", interface1);
    try
    {
        refmgr.resolveEnumerationTypeExtendsRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto enum1 = enum2->getBase();
    EXPECT_TRUE(enum1 != nullptr);
    EXPECT_STREQ(enum1->getName().c_str(), "ErrEnum1");
}

TEST(ReferenceManagerTest, resolveQerElementExtendsRef)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    auto qer = FFactory::getInstance().createFQualifiedElementRef();
    refmgr.addQerElementRef(qer, "com.bst.idl.test.model1.Interface1.Constant1", interface1);
    refmgr.resolveQerElementRef();
    auto element1 = qer->getElement();
    EXPECT_TRUE(element1 != nullptr);
    EXPECT_STREQ(element1->getName().c_str(), "Constant1");
}

TEST(ReferenceManagerTest, resolveQerElementExtendsRef_unresolved)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    auto qer = FFactory::getInstance().createFQualifiedElementRef();
    refmgr.addQerElementRef(qer, "com.bst.idl.test.model1.Interface2.Constant1", interface1);
    try
    {
        refmgr.resolveQerElementRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto element1 = qer->getElement();
    EXPECT_TRUE(element1 == nullptr);
}

TEST(ReferenceManagerTest, resolveQerElementExtendsRef_duplicate)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    auto qer = FFactory::getInstance().createFQualifiedElementRef();
    refmgr.addQerElementRef(qer, "com.bst.idl.test.model1.Interface1.Constant1", interface1);
    refmgr.addQerElementRef(qer, "com.bst.idl.test.model1.TypeCollection1.Constant1", interface1);
    try
    {
        refmgr.resolveQerElementRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto element1 = qer->getElement();
    EXPECT_TRUE(element1 == nullptr);
}

TEST(ReferenceManagerTest, resolveQerElementExtendsRef_multiRecord)
{
    prepare_env();
    auto &refmgr = ReferenceManager::getInstance();
    refmgr.clear();
    auto model1 = FModelManager::getInstance().findModel("com.bst.idl.test.model1");
    auto interface1 = model1->getInterfaces().front();
    auto qer = FFactory::getInstance().createFQualifiedElementRef();
    refmgr.addQerElementRef(qer, "com.bst.idl.test.model1.Interface2.Constant1", interface1);
    refmgr.addQerElementRef(qer, "com.bst.idl.test.model1.Interface1.Constant1", interface1);
    refmgr.addQerElementRef(qer, "com.bst.idl.test.model2.Interface2.Constant1", interface1);
    try
    {
        refmgr.resolveQerElementRef();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    auto element1 = qer->getElement();
    EXPECT_TRUE(element1 != nullptr);
    EXPECT_STREQ(element1->getName().c_str(), "Constant1");
}