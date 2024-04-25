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
#include "FDModel/FDModel.h"
#include "FDModel/FDModelManager.h"
#include "FDParser/FDParser.h"
#include "model/FModelManager.h"
#include "parser/FidlParser.h"
#include "validator/FDReferenceManager.h"
#include <gtest/gtest.h>
#include <string>

TEST(FDElementTest, allTest)
{
    std::string name = "../newidlFile/defineFDModel1.fdepl";
    BstIdl::FDParser parser;
    auto ret = parser.parse(name);
    EXPECT_EQ(ret.empty(), false);
    EXPECT_EQ(ret.size(), 2U);
    BstIdl::FDModelManager &depl_mgr = BstIdl::FDModelManager::getInstance();
    BstIdl::FDReferenceManager &depl_refMgr = BstIdl::FDReferenceManager::getInstance();
    BstIdl::FModelManager &idl_mgr = BstIdl::FModelManager::getInstance();
    BstIdl::FDFactory &depl_factpry = BstIdl::FDFactory::getInstance();
    auto first = ret.front();
    auto second = ret.back();
    EXPECT_STREQ(first->getName().c_str(), "defineFDModel1.fdepl");
    EXPECT_STREQ(second->getName().c_str(), "testMySpec.fdepl");
    auto imports = second->getImports();
    EXPECT_EQ(imports.size(), 1U);
    auto import = imports.front();
    auto specs = second->getSpecifications();
    EXPECT_EQ(specs.size(), 1U);
    auto spec = specs.front();
    import->setImportedSpec(spec->getName());
    EXPECT_EQ(import->getImportedSpec(), spec->getName());
    EXPECT_TRUE(second->getDeployments().empty());
    auto decls = spec->getAllDeclarations();
    EXPECT_EQ(decls.size(), 2U);
    auto decl = decls.front();
    auto properties = decl->getProperties();
    EXPECT_EQ(properties.size(), 3U);
    auto callSemantics = properties.front();
    EXPECT_EQ(spec->getSpecNames().size(), 1U);
    EXPECT_STREQ(callSemantics->getName().c_str(), "CallSemantics");
    EXPECT_STREQ(callSemantics->getType()->getComplex()->getName().c_str(), "FDEnumType");
    EXPECT_EQ(
        std::dynamic_pointer_cast<BstIdl::FDEnumType>(callSemantics->getType()->getComplex())->getEnumerators().size(),
        2U);

    auto depls = first->getDeployments();
    EXPECT_EQ(depls.size(), 1U);
    auto deplInterface = std::dynamic_pointer_cast<BstIdl::FDInterface>(depls.front());
    EXPECT_STREQ(deplInterface->getName().c_str(), "FDModel1Def");
    EXPECT_EQ(deplInterface->getTarget(), idl_mgr.findFInterface("fdmodel1.interface1"));
    const auto fdmodel = depl_mgr.getModelList().front();

    auto mapper = fdmodel->getMapper(deplInterface);
    EXPECT_EQ(mapper->getFDElement(deplInterface->getTarget()), deplInterface);
    // mapper
    // types:struct_infor1 in depl_interface
    EXPECT_EQ(deplInterface->getTypes().size(), 1U);
    EXPECT_STREQ(deplInterface->getTypes().front()->getName().c_str(), "infor1");
    auto struct1 = std::dynamic_pointer_cast<BstIdl::FDStruct>(deplInterface->getTypes().front());
    EXPECT_EQ(struct1->getFields().size(), 2U);
    // field1_st1 of struct_infor1
    auto str1 = struct1->getFields().front();
    EXPECT_STREQ(str1->getName().c_str(), "str1");
    EXPECT_EQ(str1->getProperties().front()->getDecl()->isDefault(), true);
    EXPECT_EQ(str1->getProperties().front()->getDecl()->getType()->getComplex()->getName(), std::string("FDEnumType"));
    EXPECT_EQ(str1->getOverwrites(), nullptr);
    // field2_struct3 of struct_infor1
    auto struct3 = struct1->getFields().back();
    EXPECT_STREQ(struct3->getName().c_str(), "struct3");
    EXPECT_STREQ(struct3->getTarget()->getName().c_str(), "struct3");
    EXPECT_EQ(mapper->getFDElement(struct3->getTarget()), struct3);
    EXPECT_EQ(std::dynamic_pointer_cast<BstIdl::FDStruct>(struct3), nullptr);

    // overwrite of field2_struct3
    auto struct3Overwrite = std::dynamic_pointer_cast<BstIdl::FDStructOverwrites>(struct3->getOverwrites());
    EXPECT_EQ(struct3Overwrite->getFields().size(), 1U);

    // field-str of struct3Overwrite q
    auto field = struct3Overwrite->getFields().front();
    EXPECT_STREQ(field->getName().c_str(), "str");

    EXPECT_EQ(field->getProperties().front()->getDecl()->getType()->getComplex()->getName(), std::string("FDEnumType"));
    EXPECT_EQ(field->getOverwrites(), nullptr);
    EXPECT_STREQ(field->getTarget()->getName().c_str(), "str");
    EXPECT_STREQ(field->getTarget()->getType()->getPredefined()->getName().c_str(), "String");
    EXPECT_NE(field->getTarget(), nullptr);

    // method--divide of interface1
    auto methods = deplInterface->getMethods();
    EXPECT_EQ(methods.size(), 1U);
    auto divide = methods.front();
    EXPECT_STREQ(divide->getName().c_str(), "fdmodel1.interface1.divide");
    EXPECT_EQ(divide->getTarget(), idl_mgr.findFMethod("fdmodel1.interface1.divide"));
    EXPECT_EQ(mapper->getFDElement(divide->getTarget()), divide);
    EXPECT_EQ(divide->getOptionalProps().size(), 1U);
    EXPECT_STREQ(divide->getOptionalProps().front()->getName().c_str(), "PerformanceImpact");
    EXPECT_EQ(divide->getOptionalProps().front()->getFlags().front()->isOptional(), true);
    EXPECT_EQ(divide->getOptionalProps().front()->getFlags().front()->getName(), std::string("optional"));
    EXPECT_EQ(divide->getHost()->getBuiltIn(), BstIdl::FDBuiltInPropertyHost::get(4));
    EXPECT_EQ(divide->getHost()->getName(), BstIdl::FDBuiltInPropertyHost::get(4)->getName());
    // getMandatoryProps
    EXPECT_EQ(divide->getMandatoryProps().size(), 2U);
    // prop1 CallSemantics
    EXPECT_STREQ(divide->getMandatoryProps().front()->getName().c_str(), "CallSemantics");
    EXPECT_STREQ(divide->getMandatoryProps().front()->getType()->getName().c_str(), "FDEnumType");
    auto callProp = divide->getProperties().front();
    EXPECT_TRUE(callProp != nullptr);
    EXPECT_STREQ(callProp->getName().c_str(), "CallSemantics");
    EXPECT_STREQ(callProp->getValue()->getSingle()->getName().c_str(), "synchronous");
    EXPECT_STREQ(std::dynamic_pointer_cast<BstIdl::FDEnumType>(callSemantics->getType()->getComplex())
                     ->getEnumerators()
                     .front()
                     ->getName()
                     .c_str(),
                 "synchronous");
    EXPECT_STREQ(callProp->getValue()->getSingle()->getEnumerator()->getName().c_str(), "synchronous");
    EXPECT_TRUE(callProp->getValue()->getSingle()->isEnumerator());
    EXPECT_EQ(callProp->getValue()->getSingle()->getEnumerator()->getFQN(),
              std::dynamic_pointer_cast<BstIdl::FDEnumType>(callSemantics->getType()->getComplex())
                  ->getEnumerators()
                  .front()
                  ->getFQN());
    // prop2 IsOptional
    EXPECT_STREQ(divide->getMandatoryProps().back()->getName().c_str(), "IsOptional");
    EXPECT_EQ(divide->getMandatoryProps().back()->getType()->getPredefined(),
              BstIdl::FDPredefinedTypeId::getByName("Boolean"));
    // argument
    auto arguments = divide->getInArguments()->getArguments();
    EXPECT_EQ(arguments.size(), 3U);
    EXPECT_EQ(divide->getOutArguments()->getArguments().size(), 3U);
    auto argStruct1 = arguments.front();
    EXPECT_STREQ(argStruct1->getName().c_str(), "struct1");
    EXPECT_EQ(argStruct1->getTarget(), idl_mgr.findFArgument("fdmodel1.interface1.divide.struct1"));
    auto argOverwrite = std::dynamic_pointer_cast<BstIdl::FDStructOverwrites>(argStruct1->getOverwrites());
    EXPECT_NE(argOverwrite->getFields().empty(), true);
    EXPECT_EQ(argOverwrite->getFields().size(), 1U);
    auto aaOverwrite =
        std::dynamic_pointer_cast<BstIdl::FDStructOverwrites>(argOverwrite->getFields().front()->getOverwrites());
    EXPECT_EQ(aaOverwrite->getFields().size(), 1U);
    EXPECT_EQ(aaOverwrite->getFields().front()->getName(), std::string("num1"));
    // out
    auto outArgs = divide->getOutArguments()->getArguments();
    // prop
    auto num1Prop = aaOverwrite->getFields().front()->getProperties().front();
    EXPECT_EQ(num1Prop->getDecl()->getName(), std::string("Encoding"));
    EXPECT_EQ(num1Prop->getValue()->toString(), std::string("utf16"));

    // broadcast
    EXPECT_EQ(deplInterface->getBroadcasts().empty(), false);
    EXPECT_EQ(deplInterface->getBroadcasts().size(), 1U);
    auto broadcast = deplInterface->getBroadcasts().front();
    auto host = broadcast->getHost();
    EXPECT_NE(host, nullptr);
    EXPECT_EQ(host->getBuiltIn(), BstIdl::FDBuiltInPropertyHost::getByName("broadcasts"));
    auto args = broadcast->getOutArguments()->getArguments();
    EXPECT_EQ(args.size(), 1U);
    auto location = args.front();
    EXPECT_EQ(mapper->getFDElement(location->getTarget()), location);
    EXPECT_STREQ(location->getTarget()->getName().c_str(), "location");
    EXPECT_TRUE(location->getOverwrites() == nullptr);
    EXPECT_TRUE(location->getDefaultProps().empty());
}

TEST(FDElementTest, plainOverwritesTest)
{
    BstIdl::FDModelManager &depl_mgr = BstIdl::FDModelManager::getInstance();
    BstIdl::FDReferenceManager &depl_refMgr = BstIdl::FDReferenceManager::getInstance();
    BstIdl::FModelManager &idl_mgr = BstIdl::FModelManager::getInstance();
    BstIdl::FDFactory &depl_factpry = BstIdl::FDFactory::getInstance();
    std::string name = "../newidlFile/defineFDModel2.fdepl";
    BstIdl::FDParser parser;
    auto ret = parser.parse(name);

    EXPECT_EQ(ret.empty(), false);
    EXPECT_EQ(ret.size(), 5U);
    auto first = ret.front();
    ret.pop_front();
    auto second = ret.front();
    EXPECT_STREQ(first->getName().c_str(), "defineFDModel2.fdepl");
    EXPECT_STREQ(second->getName().c_str(), "MySpec3.fdepl");
    auto imports = second->getImports();
    EXPECT_EQ(imports.size(), 1U);
    auto import = imports.front();
    auto specs = second->getSpecifications();
    EXPECT_EQ(specs.size(), 1U);
    auto spec = specs.front();
    EXPECT_STREQ(spec->getName().c_str(), "MySpec3");
    EXPECT_EQ(spec->getSpecNames().size(), 2U);
    EXPECT_STREQ(spec->getSpecNames().front().c_str(), "MySpec2");
    EXPECT_STREQ(spec->getSpecNames().back().c_str(), "MySpec3");
    import->setImportedSpec(spec->getName());
    EXPECT_EQ(import->getImportedSpec(), spec->getName());
    EXPECT_TRUE(second->getDeployments().empty());
    auto decls = spec->getAllDeclarations();
    EXPECT_EQ(decls.size(), 19U);
    auto decl = decls.front();
    auto properties = decl->getProperties();
    EXPECT_EQ(properties.size(), 4U);

    auto depls = first->getDeployments();
    EXPECT_EQ(depls.size(), 2U);
    auto deplInterface = std::dynamic_pointer_cast<BstIdl::FDInterface>(depls.back());
    EXPECT_STREQ(deplInterface->getName().c_str(), "fdmodel2.interface2");
    EXPECT_EQ(deplInterface->getTarget(), idl_mgr.findFInterface("fdmodel2.interface2"));
    const auto fdmodel = depl_mgr.getModelList().front();

    auto mapper = fdmodel->getMapper(deplInterface);
    EXPECT_EQ(mapper->getFDElement(deplInterface->getTarget()), deplInterface);
    auto interfacesDepl = first->getInterfaceDeployments("MySpec3");
    auto types = deplInterface->getTypes();
    EXPECT_EQ(types.size(), 13U);
    auto map = std::dynamic_pointer_cast<BstIdl::FDMap>(types.back());
    EXPECT_STREQ(map->getKey()->getHost()->getName().c_str(), "map_keys");
    EXPECT_EQ(map->getKey()->getHost()->isBuiltIn(), true);

    // auto
    types.pop_back();
    auto otherStruct = std::dynamic_pointer_cast<BstIdl::FDStruct>(types.back());
    EXPECT_EQ(otherStruct->getName(), std::string("OtherStruct"));
    EXPECT_EQ(otherStruct->getFields().size(), 7U);
    // field :fieldBool overwrite
    EXPECT_TRUE(otherStruct->getProperties().empty());
    auto stringField = otherStruct->getFields().front();
    // getValue
    EXPECT_NE(stringField->getOverwrites()->getValue("StringProp"), nullptr);
    EXPECT_NE(stringField->getOverwrites()->getValue("StringProp"), nullptr);
    auto strProp = stringField->getOverwrites()->getValue("StringProp");
    auto strValue = strProp->getSingle();
    auto strValuea = strProp->getArray();
    EXPECT_NE(strValue, nullptr);
    EXPECT_EQ(strValue->isEnumerator(), true);
    EXPECT_EQ(strValue->getEnumerator()->getName(), std::string("u"));

    otherStruct->getFields().pop_front();
    otherStruct->getFields().pop_front();
    auto boolField = otherStruct->getFields().front();
    EXPECT_EQ(boolField->getProperties().size(), 1U);
    EXPECT_EQ(boolField->getTarget()->getName(), std::string("fieldBool"));
    EXPECT_EQ(boolField->getOptionalProps().size(), 0U);
    EXPECT_EQ(boolField->getMandatoryProps().size(), 1U);
    EXPECT_EQ(boolField->getMandatoryProps().front()->getName(), std::string("SFieldProp"));
    EXPECT_EQ(boolField->getDefaultProps().size(), 0U);
    // overwrite booleanProp
    EXPECT_NE(boolField->getOverwrites()->getValue("BooleanProp"), nullptr);
    auto BooleanProp = boolField->getOverwrites()->getValue("BooleanProp");
    EXPECT_EQ(BooleanProp->getName(), std::string("FDComplexValue"));
    auto BooleanValue = BooleanProp->getSingle();
    EXPECT_EQ(BooleanValue->isBoolean(), true);
    EXPECT_EQ(BooleanValue->getBoolean(), true);

    // array
    types.pop_back();
    types.pop_back();
    types.pop_back();
    types.pop_back();
    auto otherArrayS = std::dynamic_pointer_cast<BstIdl::FDArray>(types.back());
    EXPECT_EQ(otherArrayS->getProperties().size(), 1U);
    auto sOverwrite = std::dynamic_pointer_cast<BstIdl::FDStructOverwrites>(otherArrayS->getOverwrites());
    EXPECT_NE(sOverwrite, nullptr);
    EXPECT_EQ(sOverwrite->getFields().empty(), true);
    EXPECT_NE(sOverwrite->getProperties().empty(), true);
    auto sProp = sOverwrite->getValue("StructProp");
    EXPECT_TRUE(sProp->getSingle()->isInteger());
    auto sValue = sProp->getSingle()->getInteger();
    EXPECT_EQ(sValue, 535);

    // attribute-attrS
    deplInterface->getAttributes().pop_back();
    auto attrS = deplInterface->getAttributes().back();
    EXPECT_STREQ(attrS->getName().c_str(), "attrS");
    // predefined *interfaceRef
    auto InterfaceRef = attrS->getProperties().back();
    EXPECT_STREQ(InterfaceRef->getName().c_str(), "InterfaceRef");
    EXPECT_EQ(InterfaceRef->getDecl()->getType()->getPredefined(), BstIdl::FDPredefinedTypeId::getByName("Interface"));
    auto ref = InterfaceRef->getValue()->getSingle();
    EXPECT_TRUE(ref->isInterfaceRef());
    auto infRef = std::dynamic_pointer_cast<BstIdl::FDInterfaceRef>(ref);
    EXPECT_TRUE(infRef != nullptr);
    EXPECT_EQ(infRef->getInterfaceRef(), idl_mgr.findFInterface("fdmodel2.interface2"));

    auto attrS_overWrites = std::dynamic_pointer_cast<BstIdl::FDStructOverwrites>(attrS->getOverwrites());
    EXPECT_EQ(attrS_overWrites->getFields().size(), 2U);
    EXPECT_EQ(attrS_overWrites->getFields().back()->getName(), std::string("field2"));

    auto field2 = attrS_overWrites->getFields().back();
    EXPECT_EQ(field2->getProperties().size(), 3U);
    // overwritten element have not target, so mapper do not update its declProps
    EXPECT_EQ(field2->getOptionalProps().size(), 0U);
    EXPECT_EQ(field2->getMandatoryProps().size(), 0U);
    EXPECT_EQ(field2->getDefaultProps().size(), 0U);

    // predefined string
    auto str = field2->getProperties().back();
    EXPECT_TRUE(str->getValue()->getSingle()->isString());
    EXPECT_STREQ(str->getValue()->getSingle()->getString().c_str(), "A string.");
    EXPECT_STREQ(str->getDecl()->getName().c_str(), "Str");
    EXPECT_STREQ(str->getDecl()->getType()->getArray().c_str(), "");
    EXPECT_STREQ(str->getDecl()->getType()->getName().c_str(), "String");
    EXPECT_STREQ(str->getValue()->toString().c_str(), "A string.");
    // StringIntArrayProp
    auto StringIntArrayProp = field2->getProperties().front();
    EXPECT_STREQ(StringIntArrayProp->getDecl()->getName().c_str(), "StringIntArrayProp");
    EXPECT_STREQ(StringIntArrayProp->getDecl()->getType()->getArray().c_str(), "[]");
    EXPECT_STREQ(StringIntArrayProp->getValue()->toString().c_str(), "2,4,8,16");
    // predefined & array  field↓property
    EXPECT_TRUE(StringIntArrayProp->getValue()->isArray());
    auto StringIntArrayDecl = StringIntArrayProp->getValue()->getArray();
    EXPECT_STREQ(StringIntArrayDecl->toString().c_str(), "2,4,8,16");
    EXPECT_EQ(StringIntArrayDecl->getValues().size(), 4U);

    // argument
    auto md = deplInterface->getMethods().back();
    auto arg = md->getInArguments()->getArguments().front();
    EXPECT_EQ(arg->getProperties().size(), 2U);
    EXPECT_EQ(arg->getMandatoryProps().size(), 1U);
    EXPECT_EQ(arg->getMandatoryProps().front()->getName(), std::string("ArgumentProp"));
    md->getInArguments()->getArguments().pop_front();
    arg = md->getInArguments()->getArguments().front();
    auto overwrite = arg->getOverwrites();
    EXPECT_EQ(overwrite->getProperties().size(), 1U);
    md->getInArguments()->getArguments().pop_front();
    arg = md->getInArguments()->getArguments().front();
    auto Enumeration_overWrites = std::dynamic_pointer_cast<BstIdl::FDEnumerationOverwrites>(arg->getOverwrites());
    EXPECT_EQ(Enumeration_overWrites->getEnumerators().front()->getProperties().size(), 1U);

    // broadcast
    auto broadcast = deplInterface->getBroadcasts().front();
    EXPECT_STREQ(broadcast->getName().c_str(), "fdmodel2.interface2.broadcast1");
    EXPECT_EQ(broadcast->getProperties().size(), 1U);
    auto prop = broadcast->getProperties().front();
    EXPECT_STREQ(prop->getDecl()->getName().c_str(), "BroascastProp");
    EXPECT_EQ(prop->getValue()->getSingle()->getInteger(), 115);
    auto out_args = broadcast->getOutArguments()->getArguments();
    EXPECT_EQ(out_args.size(), 2U);

    // TODO: map
}

TEST(FDElementTest, ExtensionTest)
{
    BstIdl::FDModelManager &depl_mgr = BstIdl::FDModelManager::getInstance();
    BstIdl::FDReferenceManager &depl_refMgr = BstIdl::FDReferenceManager::getInstance();
    BstIdl::FModelManager &idl_mgr = BstIdl::FModelManager::getInstance();
    BstIdl::FDFactory &depl_factpry = BstIdl::FDFactory::getInstance();
    std::string name = "../newidlFile/ExtensionDef.fdepl";
    BstIdl::FDParser parser;
    auto ret = parser.parse(name);

    EXPECT_EQ(ret.empty(), false);
    EXPECT_EQ(ret.size(), 4U);
    std::string specName("Server");
    auto extensionDepl = ret.back()->getProviderDeployment(specName);
    EXPECT_EQ(extensionDepl, nullptr);
}

TEST(FDModelManagerTest, getFQN)
{
    BstIdl::FDModelManager &depl_mgr = BstIdl::FDModelManager::getInstance();
    std::string name = "../newidlFile/defineFDModel2.fdepl";
    BstIdl::FDParser parser;
    auto ret = parser.parse(name);

    EXPECT_EQ(ret.empty(), false);
    EXPECT_EQ(ret.size(), 5U);
    auto fdmodel = depl_mgr.getModelList().front();
    std::string specName("MySpec3");
    auto interfaces = fdmodel->getInterfaceDeployments(specName);
    EXPECT_EQ(interfaces.size(), 1U);
    auto interface2 = interfaces.front();
    // attribute-attrS
    interface2->getAttributes().pop_back();
    auto attrS = interface2->getAttributes().back();
    EXPECT_STREQ(attrS->getName().c_str(), "attrS");
    // predefined  interfaceRef
    auto InterfaceRef = attrS->getProperties().back();
    auto ref = InterfaceRef->getValue()->getSingle();
    EXPECT_TRUE(ref->isInterfaceRef());
    auto infRef = std::dynamic_pointer_cast<BstIdl::FDInterfaceRef>(ref);
    EXPECT_EQ(depl_mgr.getFQN(infRef), interface2->getTarget()->getFQN());
}

TEST(FDModelManagerTest, size)
{
    BstIdl::FDModelManager &depl_mgr = BstIdl::FDModelManager::getInstance();
    std::string name = "../newidlFile/defineFDModel2.fdepl";
    BstIdl::FDParser parser;
    auto ret = parser.parse(name);

    EXPECT_EQ(ret.empty(), false);
    EXPECT_EQ(ret.size(), 5U);
    EXPECT_EQ(depl_mgr.size(), 5U);
}

TEST(FDModelManagerTest, empty)
{
    BstIdl::FDModelManager &depl_mgr = BstIdl::FDModelManager::getInstance();
    std::string name = "../newidlFile/defineFDModel2.fdepl";
    BstIdl::FDParser parser;
    auto ret = parser.parse(name);

    EXPECT_EQ(ret.empty(), false);
    EXPECT_EQ(ret.size(), 5U);
    EXPECT_EQ(depl_mgr.empty(), false);
}

TEST(FDModelManagerTest, makeModelName_canfind)
{
    BstIdl::FDModelManager &depl_mgr = BstIdl::FDModelManager::getInstance();
    std::string name = "../newidlFile/defineFDModel2.fdepl";
    BstIdl::FDParser parser;
    auto ret = parser.parse(name);

    EXPECT_EQ(ret.empty(), false);
    EXPECT_EQ(ret.size(), 5U);
    EXPECT_STREQ(depl_mgr.makeModelName("testMySpec.fdepl").c_str(), "testMySpec.fdepl0");
    EXPECT_STREQ(depl_mgr.makeModelName("defineFDModel2.fdepl").c_str(), "defineFDModel2.fdepl1");
}

TEST(FDModelManagerTest, addModel_emptyFQN)
{
    BstIdl::FDModelManager &depl_mgr = BstIdl::FDModelManager::getInstance();
    std::string name = "../newidlFile/defineFDModel2.fdepl";
    BstIdl::FDParser parser;
    auto ret = parser.parse(name);

    EXPECT_EQ(ret.empty(), false);
    EXPECT_EQ(ret.size(), 5U);
    EXPECT_EQ(depl_mgr.addModel(""), nullptr);
}

TEST(FDModelManagerTest, addPendingModel_emptyFile)
{
    BstIdl::FDModelManager &depl_mgr = BstIdl::FDModelManager::getInstance();
    std::string name = "../newidlFile/defineFDModel2.fdepl";
    BstIdl::FDParser parser;
    auto ret = parser.parse(name);

    EXPECT_EQ(ret.empty(), false);
    EXPECT_EQ(ret.size(), 5U);

    bool ifAdd = true;
    try
    {
        ifAdd = depl_mgr.addPendingModel("");
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    EXPECT_EQ(ifAdd, false);
}

TEST(FDModelManagerTest, addProcessedModel_emptyFile)
{
    BstIdl::FDModelManager &depl_mgr = BstIdl::FDModelManager::getInstance();
    std::string name = "../newidlFile/defineFDModel2.fdepl";
    BstIdl::FDParser parser;
    auto ret = parser.parse(name);

    EXPECT_EQ(ret.empty(), false);
    EXPECT_EQ(ret.size(), 5U);

    bool ifAdd = true;
    try
    {
        ifAdd = depl_mgr.addProcessedModel("");
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    EXPECT_EQ(ifAdd, false);
}

TEST(FDModelManagerTest, findProcessedModel_emptyFile)
{
    BstIdl::FDModelManager &depl_mgr = BstIdl::FDModelManager::getInstance();
    std::string name = "../newidlFile/defineFDModel2.fdepl";
    BstIdl::FDParser parser;
    auto ret = parser.parse(name);

    EXPECT_EQ(ret.empty(), false);
    EXPECT_EQ(ret.size(), 5U);

    bool ifFind = true;
    try
    {
        ifFind = depl_mgr.findProcessedModel("");
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    EXPECT_EQ(ifFind, false);
}

TEST(FDModelManagerTest, findFDSpecification_emptyFQN)
{
    BstIdl::FDModelManager &depl_mgr = BstIdl::FDModelManager::getInstance();
    std::string name = "../newidlFile/defineFDModel2.fdepl";
    BstIdl::FDParser parser;
    auto ret = parser.parse(name);

    EXPECT_EQ(ret.empty(), false);
    EXPECT_EQ(ret.size(), 5U);
    EXPECT_EQ(depl_mgr.findFDSpecification(""), nullptr);
}

TEST(FDModelManagerTest, findFDPropertyDecl_emptyFQN)
{
    BstIdl::FDModelManager &depl_mgr = BstIdl::FDModelManager::getInstance();
    std::string name = "../newidlFile/defineFDModel2.fdepl";
    BstIdl::FDParser parser;
    auto ret = parser.parse(name);

    EXPECT_EQ(ret.empty(), false);
    EXPECT_EQ(ret.size(), 5U);
    EXPECT_EQ(depl_mgr.findFDPropertyDecl(""), nullptr);
}

TEST(FDModelManagerTest, findFDRootElement_emptyFQN)
{
    BstIdl::FDModelManager &depl_mgr = BstIdl::FDModelManager::getInstance();
    std::string name = "../newidlFile/defineFDModel2.fdepl";
    BstIdl::FDParser parser;
    auto ret = parser.parse(name);

    EXPECT_EQ(ret.empty(), false);
    EXPECT_EQ(ret.size(), 5U);
    EXPECT_EQ(depl_mgr.findFDRootElement(""), nullptr);
}

TEST(FDFactoryTest, createDeployment)
{
    BstIdl::FDFactory &factory = BstIdl::FDFactory::getInstance();
    auto depl = factory.createDeployment();
    EXPECT_NE(depl, nullptr);
}

TEST(FDFactoryTest, createFDRootElement)
{
    BstIdl::FDFactory &factory = BstIdl::FDFactory::getInstance();
    auto rootElement = factory.createFDRootElement();
    EXPECT_NE(rootElement, nullptr);
}

TEST(FDBuiltInPropertyHostTest, getByName_undefined)
{
    EXPECT_EQ(BstIdl::FDBuiltInPropertyHost::getByName("1111LT"),
              BstIdl::FDBuiltInPropertyHost::getByName("undefined"));
}

TEST(FDBuiltInPropertyHostTest, get_undefined)
{
    EXPECT_EQ(BstIdl::FDBuiltInPropertyHost::get(100), BstIdl::FDBuiltInPropertyHost::getByName("undefined"));
}

TEST(FDPredefinedTypeIdTest, get_undefined)
{
    EXPECT_EQ(BstIdl::FDPredefinedTypeId::get(100), BstIdl::FDPredefinedTypeId::getByName("undefined"));
}

TEST(FDPredefinedTypeIdTest, getByName_undefined)
{
    EXPECT_EQ(BstIdl::FDPredefinedTypeId::getByName("1111LT"), BstIdl::FDPredefinedTypeId::getByName("undefined"));
}

TEST(FDPredefinedTypeIdTest, getLiteral_undefined)
{
    EXPECT_EQ(BstIdl::FDPredefinedTypeId::get("1111LT"), BstIdl::FDPredefinedTypeId::get("undefined"));
}
