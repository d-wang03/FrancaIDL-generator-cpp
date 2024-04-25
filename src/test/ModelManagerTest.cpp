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
#include <gtest/gtest.h>

using namespace BstIdl;

TEST(ModelManagerTest, addModel)
{
    FModelManager &mgr = FModelManager::getInstance();
    mgr.clear();
    auto model = mgr.addModel("com.bst.model.test");
    EXPECT_TRUE(model != nullptr);
    auto ret = mgr.findModel("com.bst.model.test");
    EXPECT_TRUE(ret != nullptr);
    EXPECT_STREQ(ret->getName().c_str(), "com.bst.model.test");
}

TEST(ModelManagerTest, addModel_duplicate)
{
    FModelManager &mgr = FModelManager::getInstance();
    mgr.clear();
    auto model = mgr.addModel("com.bst.model.test");
    EXPECT_TRUE(model != nullptr);
    auto ret = mgr.addModel("com.bst.model.test");
    EXPECT_TRUE(ret == model);
    EXPECT_EQ(mgr.size(), 1U);
}

TEST(ModelManagerTest, findInterface)
{
    FModelManager &mgr = FModelManager::getInstance();
    mgr.clear();
    auto model = mgr.addModel("com.bst.model.test");
    EXPECT_TRUE(model != nullptr);
    auto interface = FFactory::getInstance().createFInterface();
    interface->setName("MyInterface");
    model->getInterfaces().emplace_back(std::move(interface));
    interface = FFactory::getInstance().createFInterface();
    interface->setName("MyInterface2");
    model->getInterfaces().emplace_back(std::move(interface));
    auto found = mgr.findFInterface("com.bst.model.test.MyInterface2");
    EXPECT_TRUE(found != nullptr);
    found = mgr.findFInterface("com.bst.model.test.MyInterface");
    EXPECT_TRUE(found != nullptr);
}

TEST(ModelManagerTest, findInterface_longer_fqn)
{
    FModelManager &mgr = FModelManager::getInstance();
    mgr.clear();
    auto model = mgr.addModel("com.bst.model.test");
    EXPECT_TRUE(model != nullptr);
    auto interface = FFactory::getInstance().createFInterface();
    interface->setName("MyInterface");
    model->getInterfaces().emplace_back(std::move(interface));
    interface = FFactory::getInstance().createFInterface();
    interface->setName("MyInterface2");
    model->getInterfaces().emplace_back(std::move(interface));
    auto found = mgr.findFInterface("com.bst.model.test.MyInterface.aa");
    EXPECT_FALSE(found != nullptr);
}

TEST(ModelManagerTest, findInterface_shorter_fqn)
{
    FModelManager &mgr = FModelManager::getInstance();
    mgr.clear();
    auto model = mgr.addModel("com.bst.model.test");
    EXPECT_TRUE(model != nullptr);
    auto interface = FFactory::getInstance().createFInterface();
    interface->setName("MyInterface");
    model->getInterfaces().emplace_back(std::move(interface));
    interface = FFactory::getInstance().createFInterface();
    interface->setName("MyInterface2");
    model->getInterfaces().emplace_back(std::move(interface));
    auto found = mgr.findFInterface("com.bst.model");
    EXPECT_FALSE(found != nullptr);
}

TEST(ModelManagerTest, findInterface_invalid_fqn)
{
    FModelManager &mgr = FModelManager::getInstance();
    mgr.clear();
    auto model = mgr.addModel("com.bst.model.test");
    EXPECT_TRUE(model != nullptr);
    auto interface = FFactory::getInstance().createFInterface();
    interface->setName("MyInterface");
    model->getInterfaces().emplace_back(std::move(interface));
    interface = FFactory::getInstance().createFInterface();
    interface->setName("MyInterface2");
    model->getInterfaces().emplace_back(std::move(interface));
    auto found = mgr.findFInterface("com.bst.model...");
    EXPECT_FALSE(found != nullptr);
}

TEST(ModelManagerTest, findPublicFType)
{
    FModelManager &mgr = FModelManager::getInstance();
    mgr.clear();
    auto model = mgr.addModel("com.bst.model.test");
    EXPECT_TRUE(model != nullptr);
    auto tc = FFactory::getInstance().createFTypeCollection();
    tc->setName("MyTypes1");
    model->getTypeCollections().emplace_back(std::move(tc));
    tc = FFactory::getInstance().createFTypeCollection();
    tc->setName("MyTypes2");
    auto typed = FFactory::getInstance().createFTypeDef();
    typed->setName("TypeDef1");
    tc->getTypes().emplace_back(std::static_pointer_cast<FType>(typed));
    typed = FFactory::getInstance().createFTypeDef();
    typed->setName("TypeDef2");
    tc->getTypes().emplace_back(std::static_pointer_cast<FType>(typed));
    model->getTypeCollections().emplace_back(std::move(tc));

    auto found = mgr.findPublicFType("com.bst.model.test.MyTypes2.TypeDef1");
    EXPECT_TRUE(found != nullptr);
    found = mgr.findPublicFType("com.bst.model.test.MyTypes2.TypeDef2");
    EXPECT_TRUE(found != nullptr);
}

TEST(ModelManagerTest, findFType_longer_fqn)
{
    FModelManager &mgr = FModelManager::getInstance();
    mgr.clear();
    auto model = mgr.addModel("com.bst.model.test");
    EXPECT_TRUE(model != nullptr);
    auto tc = FFactory::getInstance().createFTypeCollection();
    tc->setName("MyTypes1");
    model->getTypeCollections().emplace_back(std::move(tc));
    tc = FFactory::getInstance().createFTypeCollection();
    tc->setName("MyTypes2");
    auto typed = FFactory::getInstance().createFTypeDef();
    typed->setName("TypeDef1");
    tc->getTypes().emplace_back(std::static_pointer_cast<FType>(typed));
    typed = FFactory::getInstance().createFTypeDef();
    typed->setName("TypeDef2");
    tc->getTypes().emplace_back(std::static_pointer_cast<FType>(typed));
    model->getTypeCollections().emplace_back(std::move(tc));

    auto found = mgr.findPublicFType("com.bst.model.test.MyTypes2.TypeDef1.aa");
    EXPECT_FALSE(found != nullptr);
}

TEST(ModelManagerTest, findFType_shorter_fqn)
{
    FModelManager &mgr = FModelManager::getInstance();
    mgr.clear();
    auto model = mgr.addModel("com.bst.model.test");
    EXPECT_TRUE(model != nullptr);
    auto tc = FFactory::getInstance().createFTypeCollection();
    tc->setName("MyTypes1");
    model->getTypeCollections().emplace_back(std::move(tc));
    tc = FFactory::getInstance().createFTypeCollection();
    tc->setName("MyTypes2");
    auto typed = FFactory::getInstance().createFTypeDef();
    typed->setName("TypeDef1");
    tc->getTypes().emplace_back(std::static_pointer_cast<FType>(typed));
    typed = FFactory::getInstance().createFTypeDef();
    typed->setName("TypeDef2");
    tc->getTypes().emplace_back(std::static_pointer_cast<FType>(typed));
    model->getTypeCollections().emplace_back(std::move(tc));

    auto found = mgr.findPublicFType("com.bst.model.test.MyTypes2");
    EXPECT_FALSE(found != nullptr);
}

TEST(ModelManagerTest, findFType_invalid_fqn)
{
    FModelManager &mgr = FModelManager::getInstance();
    mgr.clear();
    auto model = mgr.addModel("com.bst.model.test");
    EXPECT_TRUE(model != nullptr);
    auto tc = FFactory::getInstance().createFTypeCollection();
    tc->setName("MyTypes1");
    model->getTypeCollections().emplace_back(std::move(tc));
    tc = FFactory::getInstance().createFTypeCollection();
    tc->setName("MyTypes2");
    auto typed = FFactory::getInstance().createFTypeDef();
    typed->setName("TypeDef1");
    tc->getTypes().emplace_back(std::static_pointer_cast<FType>(typed));
    typed = FFactory::getInstance().createFTypeDef();
    typed->setName("TypeDef2");
    tc->getTypes().emplace_back(std::static_pointer_cast<FType>(typed));
    model->getTypeCollections().emplace_back(std::move(tc));

    auto found = mgr.findPublicFType("com.bst.model.test.MyTypes233..**");
    EXPECT_FALSE(found != nullptr);
}

TEST(ModelManagerTest, findPublicFEnumerationType)
{
    FModelManager &mgr = FModelManager::getInstance();
    mgr.clear();
    auto model = mgr.addModel("com.bst.model.test");
    EXPECT_TRUE(model != nullptr);
    auto tc = FFactory::getInstance().createFTypeCollection();
    tc->setName("MyTypes1");
    model->getTypeCollections().emplace_back(std::move(tc));
    tc = FFactory::getInstance().createFTypeCollection();
    tc->setName("MyTypes2");
    auto typed = FFactory::getInstance().createFEnumerationType();
    typed->setName("Enum1");
    tc->getTypes().emplace_back(std::static_pointer_cast<FType>(typed));
    typed = FFactory::getInstance().createFEnumerationType();
    typed->setName("Enum2");
    tc->getTypes().emplace_back(std::static_pointer_cast<FType>(typed));
    model->getTypeCollections().emplace_back(std::move(tc));

    auto found = mgr.findPublicFEnumerationType("com.bst.model.test.MyTypes2.Enum1");
    EXPECT_TRUE(found != nullptr);
    found = mgr.findPublicFEnumerationType("com.bst.model.test.MyTypes2.Enum2");
    EXPECT_TRUE(found != nullptr);
}

TEST(ModelManagerTest, findPublicFStructType)
{
    FModelManager &mgr = FModelManager::getInstance();
    mgr.clear();
    auto model = mgr.addModel("com.bst.model.test");
    EXPECT_TRUE(model != nullptr);
    auto tc = FFactory::getInstance().createFTypeCollection();
    tc->setName("MyTypes1");
    model->getTypeCollections().emplace_back(std::move(tc));
    tc = FFactory::getInstance().createFTypeCollection();
    tc->setName("MyTypes2");
    auto typed = FFactory::getInstance().createFStructType();
    typed->setName("Struct1");
    tc->getTypes().emplace_back(std::static_pointer_cast<FType>(typed));
    typed = FFactory::getInstance().createFStructType();
    typed->setName("Struct2");
    tc->getTypes().emplace_back(std::static_pointer_cast<FType>(typed));
    model->getTypeCollections().emplace_back(std::move(tc));

    auto found = mgr.findPublicFStructType("com.bst.model.test.MyTypes2.Struct1");
    EXPECT_TRUE(found != nullptr);
    found = mgr.findPublicFStructType("com.bst.model.test.MyTypes2.Struct2");
    EXPECT_TRUE(found != nullptr);
}

TEST(ModelManagerTest, findPublicFUnionType)
{
    FModelManager &mgr = FModelManager::getInstance();
    mgr.clear();
    auto model = mgr.addModel("com.bst.model.test");
    EXPECT_TRUE(model != nullptr);
    auto tc = FFactory::getInstance().createFTypeCollection();
    tc->setName("MyTypes1");
    model->getTypeCollections().emplace_back(std::move(tc));
    tc = FFactory::getInstance().createFTypeCollection();
    tc->setName("MyTypes2");
    auto typed = FFactory::getInstance().createFUnionType();
    typed->setName("Union1");
    tc->getTypes().emplace_back(std::static_pointer_cast<FType>(typed));
    typed = FFactory::getInstance().createFUnionType();
    typed->setName("Union2");
    tc->getTypes().emplace_back(std::static_pointer_cast<FType>(typed));
    model->getTypeCollections().emplace_back(std::move(tc));

    auto found = mgr.findPublicFUnionType("com.bst.model.test.MyTypes2.Union1");
    EXPECT_TRUE(found != nullptr);
    found = mgr.findPublicFUnionType("com.bst.model.test.MyTypes2.Union2");
    EXPECT_TRUE(found != nullptr);
}

TEST(ModelManagerTest, FType_mismatch)
{
    FModelManager &mgr = FModelManager::getInstance();
    mgr.clear();
    auto model = mgr.addModel("com.bst.model.test");
    EXPECT_TRUE(model != nullptr);
    auto tc = FFactory::getInstance().createFTypeCollection();
    tc->setName("MyTypes1");
    model->getTypeCollections().emplace_back(std::move(tc));
    tc = FFactory::getInstance().createFTypeCollection();
    tc->setName("MyTypes2");
    auto typed = FFactory::getInstance().createFTypeDef();
    typed->setName("Union1");
    tc->getTypes().emplace_back(std::static_pointer_cast<FType>(typed));
    typed = FFactory::getInstance().createFTypeDef();
    typed->setName("Union2");
    tc->getTypes().emplace_back(std::static_pointer_cast<FType>(typed));
    model->getTypeCollections().emplace_back(std::move(tc));

    auto found = mgr.findPublicFUnionType("com.bst.model.test.MyTypes2.Union1");
    EXPECT_TRUE(found == nullptr);
    found = mgr.findPublicFUnionType("com.bst.model.test.MyTypes2.Union2");
    EXPECT_TRUE(found == nullptr);

    auto found2 = mgr.findPublicFType("com.bst.model.test.MyTypes2.Union1");
    EXPECT_TRUE(found2 != nullptr);
    found2 = mgr.findPublicFType("com.bst.model.test.MyTypes2.Union2");
    EXPECT_TRUE(found2 != nullptr);
}

TEST(ModelManagerTest, findPublicConstant)
{
    FModelManager &mgr = FModelManager::getInstance();
    mgr.clear();
    auto model = mgr.addModel("com.bst.model.test");
    EXPECT_TRUE(model != nullptr);
    auto tc = FFactory::getInstance().createFTypeCollection();
    tc->setName("MyTypes1");
    model->getTypeCollections().emplace_back(std::move(tc));
    tc = FFactory::getInstance().createFTypeCollection();
    tc->setName("MyTypes2");
    auto item = FFactory::getInstance().createFConstantDef();
    item->setName("Constant1");
    tc->getConstants().emplace_back(std::move(item));
    item = FFactory::getInstance().createFConstantDef();
    item->setName("Constant2");
    tc->getConstants().emplace_back(std::move(item));
    model->getTypeCollections().emplace_back(std::move(tc));

    auto found = mgr.findPublicConstant("com.bst.model.test.MyTypes2.Constant1");
    EXPECT_TRUE(found != nullptr);
    found = mgr.findPublicConstant("com.bst.model.test.MyTypes2.Constant2");
    EXPECT_TRUE(found != nullptr);
}

TEST(ModelManagerTest, findFEvaluableElement_longer_fqn)
{
    FModelManager &mgr = FModelManager::getInstance();
    mgr.clear();
    auto model = mgr.addModel("com.bst.model.test");
    EXPECT_TRUE(model != nullptr);
    auto tc = FFactory::getInstance().createFTypeCollection();
    tc->setName("MyTypes1");
    model->getTypeCollections().emplace_back(std::move(tc));
    tc = FFactory::getInstance().createFTypeCollection();
    tc->setName("MyTypes2");
    auto item = FFactory::getInstance().createFConstantDef();
    item->setName("Constant1");
    tc->getConstants().emplace_back(std::move(item));
    item = FFactory::getInstance().createFConstantDef();
    item->setName("Constant2");
    tc->getConstants().emplace_back(std::move(item));
    model->getTypeCollections().emplace_back(std::move(tc));

    auto found = mgr.findPublicConstant("com.bst.model.test.MyTypes2.Constant1.xxx");
    EXPECT_TRUE(found == nullptr);
    found = mgr.findPublicConstant("com.bst.model.test.MyTypes2.Constant2.xxx.xxx");
    EXPECT_TRUE(found == nullptr);
}

TEST(ModelManagerTest, findFEvaluableElement_shorter_fqn)
{
    FModelManager &mgr = FModelManager::getInstance();
    mgr.clear();
    auto model = mgr.addModel("com.bst.model.test");
    EXPECT_TRUE(model != nullptr);
    auto tc = FFactory::getInstance().createFTypeCollection();
    tc->setName("MyTypes1");
    model->getTypeCollections().emplace_back(std::move(tc));
    tc = FFactory::getInstance().createFTypeCollection();
    tc->setName("MyTypes2");
    auto item = FFactory::getInstance().createFConstantDef();
    item->setName("Constant1");
    tc->getConstants().emplace_back(std::move(item));
    item = FFactory::getInstance().createFConstantDef();
    item->setName("Constant2");
    tc->getConstants().emplace_back(std::move(item));
    model->getTypeCollections().emplace_back(std::move(tc));

    auto found = mgr.findPublicConstant("com.bst.model.test.MyTypes2");
    EXPECT_TRUE(found == nullptr);
    found = mgr.findPublicConstant("com.bst.model.test");
    EXPECT_TRUE(found == nullptr);
}

TEST(ModelManagerTest, findFEvaluableElement_invalid_fqn)
{
    FModelManager &mgr = FModelManager::getInstance();
    mgr.clear();
    auto model = mgr.addModel("com.bst.model.test");
    EXPECT_TRUE(model != nullptr);
    auto tc = FFactory::getInstance().createFTypeCollection();
    tc->setName("MyTypes1");
    model->getTypeCollections().emplace_back(std::move(tc));
    tc = FFactory::getInstance().createFTypeCollection();
    tc->setName("MyTypes2");
    auto item = FFactory::getInstance().createFConstantDef();
    item->setName("Constant1");
    tc->getConstants().emplace_back(std::move(item));
    item = FFactory::getInstance().createFConstantDef();
    item->setName("Constant2");
    tc->getConstants().emplace_back(std::move(item));
    model->getTypeCollections().emplace_back(std::move(tc));

    auto found = mgr.findPublicConstant("com.bst.model.test.MyTypes2.Constant1...");
    EXPECT_TRUE(found == nullptr);
    found = mgr.findPublicConstant("com.bst.model.test.MyTypes2.Constant2*.");
    EXPECT_TRUE(found == nullptr);
}

TEST(ModelManagerTest, findFTypeInInterface)
{
    auto interface = FFactory::getInstance().createFInterface();
    interface->setName("TestInterface");
    auto item = FFactory::getInstance().createFTypeDef();
    item->setName("TypeDef1");
    interface->getTypes().emplace_back(std::move(item));
    item = FFactory::getInstance().createFTypeDef();
    item->setName("TypeDef2");
    interface->getTypes().emplace_back(std::move(item));

    FModelManager &mgr = FModelManager::getInstance();
    mgr.clear();
    auto found = mgr.findFTypeInInterface(interface, "TestInterface.TypeDef1");
    EXPECT_TRUE(found != nullptr);
    found = mgr.findFTypeInInterface(interface, "TestInterface.TypeDef2");
    EXPECT_TRUE(found != nullptr);
}

TEST(ModelManagerTest, findFEnumerationTypeInInterface)
{
    auto interface = FFactory::getInstance().createFInterface();
    interface->setName("TestInterface");
    auto item = FFactory::getInstance().createFEnumerationType();
    item->setName("Enum1");
    interface->getTypes().emplace_back(std::move(item));
    item = FFactory::getInstance().createFEnumerationType();
    item->setName("Enum2");
    interface->getTypes().emplace_back(std::move(item));

    FModelManager &mgr = FModelManager::getInstance();
    mgr.clear();
    auto found = mgr.findFEnumerationTypeInInterface(interface, "TestInterface.Enum1");
    EXPECT_TRUE(found != nullptr);
    found = mgr.findFEnumerationTypeInInterface(interface, "TestInterface.Enum2");
    EXPECT_TRUE(found != nullptr);
}

TEST(ModelManagerTest, findFStructTypeInInterface)
{
    auto interface = FFactory::getInstance().createFInterface();
    interface->setName("TestInterface");
    auto item = FFactory::getInstance().createFStructType();
    item->setName("Struct1");
    interface->getTypes().emplace_back(std::move(item));
    item = FFactory::getInstance().createFStructType();
    item->setName("Struct2");
    interface->getTypes().emplace_back(std::move(item));

    FModelManager &mgr = FModelManager::getInstance();
    mgr.clear();
    auto found = mgr.findFStructTypeInInterface(interface, "TestInterface.Struct1");
    EXPECT_TRUE(found != nullptr);
    found = mgr.findFStructTypeInInterface(interface, "TestInterface.Struct2");
    EXPECT_TRUE(found != nullptr);
}

TEST(ModelManagerTest, findFUnionTypeInInterface)
{
    auto interface = FFactory::getInstance().createFInterface();
    interface->setName("TestInterface");
    auto item = FFactory::getInstance().createFUnionType();
    item->setName("Union1");
    interface->getTypes().emplace_back(std::move(item));
    item = FFactory::getInstance().createFUnionType();
    item->setName("Union2");
    interface->getTypes().emplace_back(std::move(item));

    FModelManager &mgr = FModelManager::getInstance();
    mgr.clear();
    auto found = mgr.findFUnionTypeInInterface(interface, "TestInterface.Union1");
    EXPECT_TRUE(found != nullptr);
    found = mgr.findFUnionTypeInInterface(interface, "TestInterface.Union2");
    EXPECT_TRUE(found != nullptr);
}

TEST(ModelManagerTest, findFConstantDefInInterface)
{
    auto interface = FFactory::getInstance().createFInterface();
    interface->setName("TestInterface");
    auto item = FFactory::getInstance().createFConstantDef();
    item->setName("Constant1");
    interface->getConstants().emplace_back(std::move(item));
    item = FFactory::getInstance().createFConstantDef();
    item->setName("Constant2");
    interface->getConstants().emplace_back(std::move(item));

    FModelManager &mgr = FModelManager::getInstance();
    mgr.clear();
    auto found = mgr.findFConstantDefInInterface(interface, "TestInterface.Constant1");
    EXPECT_TRUE(found != nullptr);
    found = mgr.findFConstantDefInInterface(interface, "TestInterface.Constant2");
    EXPECT_TRUE(found != nullptr);
}

TEST(ModelManagerTest, getFQN)
{
    auto &mgr = FModelManager::getInstance();
    mgr.clear();
    // add model1
    auto model1 = mgr.addModel("com.bst.idl.test.model1");
    // add model1.interface1
    auto interface1 = FFactory::getInstance().createFInterface();
    interface1->setName("interface1");
    interface1->setContainer(model1);
    addListItem(model1->getInterfaces(), interface1);
    // add typedef1
    auto typedef1 = FFactory::getInstance().createFTypeDef();
    typedef1->setName("typedef1");
    typedef1->setContainer(interface1);
    addListItem(interface1->getTypes(), typedef1);
    // add array1
    auto array1 = FFactory::getInstance().createFArrayType();
    array1->setName("array1");
    array1->setContainer(interface1);
    addListItem(interface1->getTypes(), array1);
    // add struct1
    auto struct1 = FFactory::getInstance().createFStructType();
    struct1->setName("struct1");
    struct1->setContainer(interface1);
    addListItem(interface1->getTypes(), struct1);
    // add enum1
    auto enum1 = FFactory::getInstance().createFEnumerationType();
    enum1->setName("enum1");
    enum1->setContainer(interface1);
    addListItem(interface1->getTypes(), enum1);
    // add union1
    auto union1 = FFactory::getInstance().createFUnionType();
    union1->setName("union1");
    union1->setContainer(interface1);
    addListItem(interface1->getTypes(), union1);
    // add constant1
    auto constant1 = FFactory::getInstance().createFConstantDef();
    constant1->setName("constant1");
    constant1->setContainer(interface1);
    addListItem(interface1->getConstants(), constant1);
    // add method1
    auto method1 = FFactory::getInstance().createFMethod();
    method1->setName("method1");
    method1->setContainer(interface1);
    addListItem(interface1->getMethods(), method1);
    // add attribute1
    auto attribute1 = FFactory::getInstance().createFAttribute();
    attribute1->setName("attribute1");
    attribute1->setContainer(interface1);
    addListItem(interface1->getAttributes(), attribute1);
    // add broadcast1
    auto broadcast1 = FFactory::getInstance().createFBroadcast();
    broadcast1->setName("broadcast1");
    broadcast1->setContainer(interface1);
    addListItem(interface1->getBroadcasts(), broadcast1);

    // add typecollection1
    auto tc1 = FFactory::getInstance().createFTypeCollection();
    tc1->setName("typecollection1");
    tc1->setContainer(model1);
    addListItem(model1->getTypeCollections(), tc1);
    // add typedef2
    auto typedef2 = FFactory::getInstance().createFTypeDef();
    typedef2->setName("typedef2");
    typedef2->setContainer(tc1);
    addListItem(tc1->getTypes(), typedef2);
    // add array2
    auto array2 = FFactory::getInstance().createFArrayType();
    array2->setName("array2");
    array2->setContainer(tc1);
    addListItem(tc1->getTypes(), array2);
    // add struct2
    auto struct2 = FFactory::getInstance().createFStructType();
    struct2->setName("struct2");
    struct2->setContainer(tc1);
    addListItem(tc1->getTypes(), struct2);
    // add enum2
    auto enum2 = FFactory::getInstance().createFEnumerationType();
    enum2->setName("enum2");
    enum2->setContainer(tc1);
    addListItem(tc1->getTypes(), enum2);
    // add union2
    auto union2 = FFactory::getInstance().createFUnionType();
    union2->setName("union2");
    union2->setContainer(tc1);
    addListItem(tc1->getTypes(), union2);
    // add constant2
    auto constant2 = FFactory::getInstance().createFConstantDef();
    constant2->setName("constant2");
    constant2->setContainer(tc1);
    addListItem(tc1->getConstants(), constant2);

    // test getFQN
    EXPECT_STREQ(interface1->getFQN().c_str(), "com.bst.idl.test.model1.interface1");
    EXPECT_STREQ(typedef1->getFQN().c_str(), "com.bst.idl.test.model1.interface1.typedef1");
    EXPECT_STREQ(array1->getFQN().c_str(), "com.bst.idl.test.model1.interface1.array1");
    EXPECT_STREQ(struct1->getFQN().c_str(), "com.bst.idl.test.model1.interface1.struct1");
    EXPECT_STREQ(enum1->getFQN().c_str(), "com.bst.idl.test.model1.interface1.enum1");
    EXPECT_STREQ(union1->getFQN().c_str(), "com.bst.idl.test.model1.interface1.union1");
    EXPECT_STREQ(constant1->getFQN().c_str(), "com.bst.idl.test.model1.interface1.constant1");
    EXPECT_STREQ(method1->getFQN().c_str(), "com.bst.idl.test.model1.interface1.method1");
    EXPECT_STREQ(attribute1->getFQN().c_str(), "com.bst.idl.test.model1.interface1.attribute1");
    EXPECT_STREQ(broadcast1->getFQN().c_str(), "com.bst.idl.test.model1.interface1.broadcast1");
    EXPECT_STREQ(tc1->getFQN().c_str(), "com.bst.idl.test.model1.typecollection1");
    EXPECT_STREQ(typedef2->getFQN().c_str(), "com.bst.idl.test.model1.typecollection1.typedef2");
    EXPECT_STREQ(array2->getFQN().c_str(), "com.bst.idl.test.model1.typecollection1.array2");
    EXPECT_STREQ(struct2->getFQN().c_str(), "com.bst.idl.test.model1.typecollection1.struct2");
    EXPECT_STREQ(enum2->getFQN().c_str(), "com.bst.idl.test.model1.typecollection1.enum2");
    EXPECT_STREQ(union2->getFQN().c_str(), "com.bst.idl.test.model1.typecollection1.union2");
    EXPECT_STREQ(constant2->getFQN().c_str(), "com.bst.idl.test.model1.typecollection1.constant2");
}