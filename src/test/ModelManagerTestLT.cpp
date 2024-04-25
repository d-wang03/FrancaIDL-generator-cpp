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
#include <gtest/gtest.h>

TEST(ModelManagerTestLT, addModel_empty)
{
    BstIdl::FModelManager &mgr = BstIdl::FModelManager::getInstance();
    EXPECT_EQ(mgr.addModel(""), nullptr);
}

TEST(ModelManagerTestLT, findFObject)
{
    BstIdl::FModelManager &mgr = BstIdl::FModelManager::getInstance();
    BstIdl::FFactory &factory = BstIdl::FFactory::getInstance();

    mgr.clear();
    auto model = mgr.addModel("com.bst.idl.test.model1");
    // add TypeCollection1
    auto tc1 = factory.createFTypeCollection();
    tc1->setName("TypeCollection1");
    tc1->setContainer(model);
    // add tc to model1
    addListItem(model->getTypeCollections(), tc1);

    // add struct to tc
    auto struct1 = factory.createFStructType();
    struct1->setName("Struct1");
    struct1->setContainer(tc1);
    addListItem(tc1->getTypes(), struct1);

    // add interface to model
    auto interface1 = factory.createFInterface();
    interface1->setName("Interface1");
    interface1->setContainer(model);
    addListItem(model->getInterfaces(), interface1);

    // array to interface
    auto array1 = factory.createFArrayType();
    array1->setName("Array1");
    array1->setContainer(interface1);
    addListItem(interface1->getTypes(), array1);

    auto tcFqn = std::string("com.bst.idl.test.model1.TypeCollection1");
    auto interfaceFqn = std::string("com.bst.idl.test.model1.Interface1");
    auto publicFqn = std::string("com.bst.idl.test.model1.TypeCollection1.Struct1");
    auto elseFqn = std::string("com.bst.idl.test.model1.Interface1.Array1");
    EXPECT_EQ(mgr.findFObject(tcFqn), tc1);
    EXPECT_EQ(mgr.findFObject(interfaceFqn), interface1);
    EXPECT_EQ(mgr.findFObject(publicFqn), struct1);
    EXPECT_EQ(mgr.findFObject(elseFqn), nullptr);
}

TEST(ModelManagerTestLT, findFTypeCollection)
{
    BstIdl::FModelManager &mgr = BstIdl::FModelManager::getInstance();
    EXPECT_EQ(mgr.findFTypeCollection(""), nullptr);
}

TEST(ModelManagerTestLT, findFInterface)
{
    BstIdl::FModelManager &mgr = BstIdl::FModelManager::getInstance();
    EXPECT_EQ(mgr.findFInterface(""), nullptr);
}

TEST(ModelManagerTestLT, findPublicConstant)
{
    BstIdl::FModelManager &mgr = BstIdl::FModelManager::getInstance();
    EXPECT_EQ(mgr.findPublicConstant(""), nullptr);
}

TEST(ModelManagerTestLT, addPendingFidl)
{
    BstIdl::FModelManager &mgr = BstIdl::FModelManager::getInstance();
    EXPECT_EQ(mgr.addPendingFidl(""), false);
}

TEST(ModelManagerTestLT, addProcessedFidl)
{
    BstIdl::FModelManager &mgr = BstIdl::FModelManager::getInstance();
    EXPECT_EQ(mgr.addProcessedFidl(""), false);
}

TEST(ModelManagerTestLT, getDerivedFStructTypes)
{
    BstIdl::FModelManager &mgr = BstIdl::FModelManager::getInstance();
    BstIdl::FFactory &factory = BstIdl::FFactory::getInstance();
    mgr.clear();
    auto model1 = mgr.addModel("com.bst.idl.test.model1");
    // add TypeCollection1
    auto tc1 = factory.createFTypeCollection();
    tc1->setName("TypeCollection1");
    tc1->setContainer(model1);
    // add tc to model1
    addListItem(model1->getTypeCollections(), tc1);

    // add struct_base to tc
    auto struct_base = factory.createFStructType();
    struct_base->setName("Base");
    struct_base->setContainer(tc1);
    addListItem(tc1->getTypes(), struct_base);
    // add struct to tc
    auto struct1 = factory.createFStructType();
    struct1->setName("Struct1");
    struct1->setContainer(tc1);
    struct1->setBase(struct_base);
    addListItem(tc1->getTypes(), struct1);

    // add interface to model
    auto interface1 = factory.createFInterface();
    interface1->setName("Interface1");
    interface1->setContainer(model1);
    addListItem(model1->getInterfaces(), interface1);
    // add struct1 to interafce
    struct1 = factory.createFStructType();
    struct1->setName("Struct1");
    struct1->setContainer(interface1);
    struct1->setBase(struct_base);
    addListItem(interface1->getTypes(), struct1);
    // add struct2 to interafce
    auto struct2 = factory.createFStructType();
    struct2->setName("Struct2");
    struct2->setContainer(interface1);
    struct2->setBase(struct_base);
    addListItem(interface1->getTypes(), struct2);

    std::list<std::shared_ptr<BstIdl::FStructType>> struct_types;
    mgr.getDerivedFStructTypes(nullptr, nullptr, struct_types);
    EXPECT_TRUE(struct_types.empty());
    mgr.getDerivedFStructTypes(model1, struct1, struct_types);
    EXPECT_TRUE(struct_types.empty());
    mgr.getDerivedFStructTypes(model1, struct_base, struct_types);
    EXPECT_EQ(struct_types.size(), 3U);
}

TEST(ModelManagerTestLT, allFind)
{
    auto &factory = BstIdl::FFactory::getInstance();
    auto content = R"(package com.Bst.idl.test
    interface Interface1 {
    attribute UInt64 integer
    attribute Double double

    const UInt32 cst1 = 25
    const Boolean cst2 = false
    
    method test1{
        in{
            Boolean check1
        }
    }
    method test2{
        in{
            Boolean check2
        }
        out{
            Boolean out1
        }
    }
    broadcast test3{
        out{
            Boolean check3
        }
    }
    broadcast test4{
        out{
            Boolean check4
        }
    }
    broadcast test5{
        out{
            Boolean check5
            Boolean check6
        }
    }
    
    enumeration Enum1 {
        STACKFLOW
        OVERFLOW
        LALALA
    }
})";

    auto ret = BstIdl::FidlParser::parseString(content);
    BstIdl::FModelManager &mgr = BstIdl::FModelManager::getInstance();
    EXPECT_TRUE(ret != nullptr);
    EXPECT_EQ(mgr.findFAttribute(""), nullptr);
    EXPECT_EQ(mgr.findFAttribute("com.Bst.idl.test.Interface1.integer")->getType()->getPredefined(),
              BstIdl::FBasicTypeId::get(8U));
    EXPECT_EQ(mgr.findFAttribute("com.Bst.idl.test.Interface1.integer1"), nullptr);
    EXPECT_EQ(mgr.findFMethod(""), nullptr);
    EXPECT_EQ(mgr.findFMethod("com.Bst.idl.test.Interface1.test1")->getInArgs().size(), 1U);
    EXPECT_EQ(mgr.findFMethod("com.Bst.idl.test.Interface1.test9"), nullptr);
    EXPECT_EQ(mgr.findFBroadcast(""), nullptr);
    EXPECT_EQ(mgr.findFBroadcast("com.Bst.idl.test.Interface1.test5")->getOutArgs().size(), 2U);
    EXPECT_EQ(mgr.findFBroadcast("com.Bst.idl.test.Interface1.test4")->getOutArgs().size(), 1U);
    EXPECT_EQ(mgr.findFBroadcast("com.Bst.idl.test.Interface1.test9"), nullptr);
    EXPECT_EQ(mgr.findFConstantDef(""), nullptr);
    EXPECT_EQ(mgr.findFConstantDef("com.Bst.idl.test.Interface1.cst1")->getName(), std::string("cst1"));
    EXPECT_EQ(mgr.findFConstantDef("com.Bst.idl.test.Interface1.cst3"), nullptr);
    EXPECT_EQ(mgr.findFArgument(""), nullptr);
    EXPECT_EQ(mgr.findFArgument("com.Bst.idl.test.Interface1.test2.check2")->getType()->getPredefined(),
              BstIdl::FBasicTypeId::get(9U));
    EXPECT_EQ(mgr.findFArgument("com.Bst.idl.test.Interface1.test2.check1"), nullptr);
    EXPECT_EQ(mgr.findFArgument("com.Bst.idl.test.Interface1.test3.check3")->getType()->getPredefined(),
              BstIdl::FBasicTypeId::get(9U));
    EXPECT_EQ(mgr.findFArgument("com.Bst.idl.test.Interface1.test2.out1")->getType()->getPredefined(),
              BstIdl::FBasicTypeId::get(9U));
    EXPECT_EQ(mgr.findFArgument("com.Bst.idl.test.Interface1.test3.check1"), nullptr);
}
