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
#include "FDModel/FDBoolean.h"
#include "FDModel/FDFactory.h"
#include "FDModel/FDModel.h"
#include "FDModel/FDModelManager.h"
#include "model/FFactory.h"
#include "model/FModel.h"
#include "model/FModelManager.h"
#include "validator/FDReferenceManager.h"
#include "validator/ReferenceManager.h"
#include <gtest/gtest.h>
#include <string>

class FDReferenceManagerTestLT : public ::testing::Test
{
protected:
    void SetUp() override
    {
        fdmgr.clear();
        fdRmgr.clear();

        auto model1 = fdmgr.addModel("com.bst.deployment.test.fdmodel1");
        // specification
        auto spec = fdfactory.createSpecification();
        spec->setName("Specification1");
        spec->setContainer(model1);
        addListItem(model1->getSpecifications(), spec);

        // declaration
        auto declaration1 = fdfactory.createFDDeclaration();
        declaration1->setName("Declaration1");
        addListItem(spec->getDeclarations(), declaration1);
        // interface[spec+use]
        auto fdi1 = fdfactory.createFDInterface();
        fdi1->setName("Interface1");
        fdi1->setContainer(model1);
        addListItem(model1->getDeployments(), fdi1);
        fdi1->setSpec(spec);

        // types
        auto type = std::make_shared<BstIdl::FDTypeDefinition>();
        // property
        auto prop = std::make_shared<BstIdl::FDProperty>();
        // propertyDecl
        auto propDecl = std::make_shared<BstIdl::FDPropertyDecl>();
        propDecl->setName("ifArray");
        // TYPEREF
        auto propType = BstIdl::FDPredefinedTypeId::get(3U);
        auto propTypeRef = std::make_shared<BstIdl::FDTypeRef>();
        propTypeRef->setPredefined(propType);
        propDecl->setType(propTypeRef);
        prop->setDecl(propDecl);
        // COMPLEXVALUE
        auto complexValue = std::make_shared<BstIdl::FDComplexValue>();
        auto boolean = std::make_shared<BstIdl::FDBoolean>();
        std::string value("false");
        boolean->setValue(value);
        complexValue->setSingle(boolean);
        prop->setValue(complexValue);

        type->addProperty(prop);
        fdi1->setTypes(type);
        addListItem(declaration1->getProperties(), propDecl);

        // attribute
        auto attribute = std::make_shared<BstIdl::FDAttribute>();
        // property
        prop = std::make_shared<BstIdl::FDProperty>();
        // propertyDecl
        propDecl = std::make_shared<BstIdl::FDPropertyDecl>();
        propDecl->setName("Integer1");
        // TYPEREF
        propType = BstIdl::FDPredefinedTypeId::get(2U);
        propTypeRef = std::make_shared<BstIdl::FDTypeRef>();
        propTypeRef->setPredefined(propType);
        propDecl->setType(propTypeRef);
        prop->setDecl(propDecl);
        // COMPLEXVALUE
        complexValue = std::make_shared<BstIdl::FDComplexValue>();
        auto integer = std::make_shared<BstIdl::FDInteger>();
        int intValue = 1012;
        integer->setValue(intValue);
        complexValue->setSingle(integer);
        prop->setValue(complexValue);

        attribute->addProperty(prop);
        fdi1->setAttributes(attribute);
        addListItem(declaration1->getProperties(), propDecl);

        // interface[spec+use]
        fdi1 = fdfactory.createFDInterface();
        fdi1->setName("Interface2");
        fdi1->setContainer(model1);
        addListItem(model1->getDeployments(), fdi1);
        fdi1->setSpec(spec);

        // interface[spec+use]
        fdi1 = fdfactory.createFDInterface();
        fdi1->setName("Interface3");
        fdi1->setContainer(model1);
        addListItem(model1->getDeployments(), fdi1);
        fdi1->setSpec(spec);

        // specification2
        auto spec2 = fdfactory.createSpecification();
        spec2->setName("Specification2");
        spec2->setContainer(model1);
        addListItem(model1->getSpecifications(), spec2);
        // add declaration1
        declaration1 = fdfactory.createFDDeclaration();
        declaration1->setName("Declaration1");
        addListItem(spec2->getDeclarations(), declaration1);
        // extension root[spec+use]
        auto types = fdfactory.createFDTypes();
        types->setName("TypeCollection1");
        types->setContainer(model1);
        addListItem(model1->getDeployments(), types);
        types->setSpec(spec2);
        // types
        type = std::make_shared<BstIdl::FDTypeDefinition>();
        // property
        prop = std::make_shared<BstIdl::FDProperty>();
        type->addProperty(prop);
        types->setTypes(type);

        // model2
        auto model2 = fdmgr.addModel("com.bst.deployment.test.fdmodel2");
        // specification
        auto spec1 = fdfactory.createSpecification();
        spec1->setName("Specification3");
        spec1->setContainer(model2);
        addListItem(model2->getSpecifications(), spec1);
        // declaration
        declaration1 = fdfactory.createFDDeclaration();
        declaration1->setName("Declaration1");
        addListItem(spec1->getDeclarations(), declaration1);
        // interface[spec+use]
        types = fdfactory.createFDTypes();
        types->setName("TypeCollection2");
        types->setContainer(model2);
        addListItem(model2->getDeployments(), types);
        types->setSpec(spec1);
        // types
        type = std::make_shared<BstIdl::FDTypeDefinition>();
        // property
        prop = std::make_shared<BstIdl::FDProperty>();
        // propertyDecl
        propDecl = std::make_shared<BstIdl::FDPropertyDecl>();
        propDecl->setName("Integer1");
        // TYPEREF
        propType = BstIdl::FDPredefinedTypeId::get(2U);
        propTypeRef = std::make_shared<BstIdl::FDTypeRef>();
        propTypeRef->setPredefined(propType);
        propDecl->setType(propTypeRef);
        prop->setDecl(propDecl);
        // COMPLEXVALUE
        complexValue = std::make_shared<BstIdl::FDComplexValue>();
        integer = std::make_shared<BstIdl::FDInteger>();
        intValue = 2024;
        integer->setValue(intValue);
        complexValue->setSingle(integer);
        prop->setValue(complexValue);
        type->addProperty(prop);
        types->setTypes(type);
        addListItem(declaration1->getProperties(), propDecl);

        // TypeCollection3
        types = fdfactory.createFDTypes();
        types->setName("TypeCollection3");
        types->setContainer(model2);
        addListItem(model2->getDeployments(), types);

        mgr.clear();
        refMgr.clear();
        // model
        auto model = mgr.addModel("com.bst.idl.test.model1");
        // tc
        auto tc = factory.createFTypeCollection();
        tc->setName("TypeCollection1");
        tc->setContainer(model);
        // add tc to model
        addListItem(model->getTypeCollections(), tc);
        // enumeration
        auto enumeration = factory.createFEnumerationType();
        enumeration->setName("Enum1");
        enumeration->setContainer(tc);
        // enumerator1
        auto enumerator1 = factory.createFEnumerator();
        enumerator1->setName("Enumerator1");
        enumerator1->setContainer(enumeration);
        auto value1 = factory.createFIntegerConstant();
        value1->setUnsignedVal(10U);
        enumerator1->setValue(value1);
        addListItem(enumeration->getEnumerators(), enumerator1);
        // enumerator2
        auto enumerator2 = factory.createFEnumerator();
        enumerator2->setName("Enumerator2");
        enumerator2->setContainer(enumeration);
        auto value2 = factory.createFIntegerConstant();
        value2->setUnsignedVal(12U);
        enumerator2->setValue(value2);
        addListItem(enumeration->getEnumerators(), enumerator2);
        // add enum to tc
        addListItem(tc->getTypes(), enumeration);
        // struct1
        auto struct1 = factory.createFStructType();
        struct1->setName("Struct1");
        struct1->setContainer(tc);
        auto field1 = factory.createFField();
        field1->setName("Field1");
        field1->setContainer(struct1);
        auto typeref = factory.createFTypeRef();
        typeref->setPredefined(BstIdl::FBasicTypeId::get(7U));
        field1->setType(typeref);
        // add struct1 to tc
        addListItem(struct1->getElements(), field1);
        addListItem(tc->getTypes(), struct1);
        // struct2 nested
        auto struct2 = factory.createFStructType();
        struct2->setName("Struct2");
        struct2->setContainer(tc);
        auto field = factory.createFField();
        field->setName("Field2");
        field->setContainer(struct2);
        typeref = factory.createFTypeRef();
        typeref->setDerived(struct1);
        field1->setType(typeref);
        // add nested struct2 to tc
        addListItem(struct2->getElements(), field);
        addListItem(tc->getTypes(), struct2);
        // union
        auto union1 = factory.createFUnionType();
        union1->setName("Union1");
        union1->setContainer(tc);
        field = factory.createFField();
        field->setName("Field1");
        field->setContainer(union1);
        typeref = factory.createFTypeRef();
        typeref->setPredefined(BstIdl::FBasicTypeId::get(7U));
        field->setType(typeref);
        addListItem(union1->getElements(), field);
        addListItem(tc->getTypes(), union1);
        // array
        auto array1 = factory.createFArrayType();
        array1->setName("Array1");
        array1->setContainer(tc);
        typeref = factory.createFTypeRef();
        typeref->setPredefined(BstIdl::FBasicTypeId::get(7U));
        array1->setElementType(typeref);
        // add array1 to tc
        addListItem(tc->getTypes(), array1);
        // map
        auto map1 = factory.createFMapType();
        map1->setName("Map1");
        map1->setContainer(tc);
        auto keyType = factory.createFTypeRef();
        keyType->setPredefined(BstIdl::FBasicTypeId::get(7U));
        auto valueType = factory.createFTypeRef();
        valueType->setPredefined(BstIdl::FBasicTypeId::get(10U));
        map1->setKeyType(keyType);
        map1->setValueType(valueType);
        // add map1 to tc
        addListItem(tc->getTypes(), map1);
        // typedef
        auto typedef1 = factory.createFTypeDef();
        typedef1->setName("RealUInt64");
        typedef1->setContainer(tc);
        typeref = factory.createFTypeRef();
        typeref->setPredefined(BstIdl::FBasicTypeId::get(8U));
        typedef1->setActualType(typeref);
        // add typedef1 to tc
        addListItem(tc->getTypes(), typedef1);

        // TypeCollection2
        tc = factory.createFTypeCollection();
        tc->setName("TypeCollection2");
        tc->setContainer(model);
        // add tc to model
        addListItem(model->getTypeCollections(), tc);

        // TypeCollection3
        tc = factory.createFTypeCollection();
        tc->setName("TypeCollection3");
        tc->setContainer(model);
        // add tc to model
        addListItem(model->getTypeCollections(), tc);

        // interface
        auto interface1 = factory.createFInterface();
        interface1->setName("Interface1");
        addListItem(model->getInterfaces(), interface1);

        // typedef
        typedef1 = factory.createFTypeDef();
        typedef1->setName("RealUInt64");
        typedef1->setContainer(interface1);
        typeref = factory.createFTypeRef();
        typeref->setPredefined(BstIdl::FBasicTypeId::get(8U));
        typedef1->setActualType(typeref);
        // add typedef1 to tc
        addListItem(interface1->getTypes(), typedef1);

        // attribute1
        auto attribute1 = factory.createFAttribute();
        attribute1->setName("Attribute1");
        attribute1->setContainer(interface1);
        typeref = factory.createFTypeRef();
        typeref->setPredefined(BstIdl::FBasicTypeId::get(9U));
        attribute1->setType(typeref);
        // add attribute to interface1
        addListItem(interface1->getAttributes(), attribute1);

        // attribute2
        attribute1 = factory.createFAttribute();
        attribute1->setName("Attribute2");
        attribute1->setContainer(interface1);
        typeref = factory.createFTypeRef();
        typeref->setPredefined(BstIdl::FBasicTypeId::get(8U));
        attribute1->setType(typeref);
        // add attribute to interface1
        addListItem(interface1->getAttributes(), attribute1);

        // attribute3
        attribute1 = factory.createFAttribute();
        attribute1->setName("Attribute3");
        attribute1->setContainer(interface1);
        typeref = factory.createFTypeRef();
        typeref->setPredefined(BstIdl::FBasicTypeId::get(8U));
        attribute1->setType(typeref);
        // add attribute to interface1
        addListItem(interface1->getAttributes(), attribute1);

        // array
        array1 = factory.createFArrayType();
        array1->setName("Array1");
        array1->setContainer(interface1);
        typeref = factory.createFTypeRef();
        typeref->setPredefined(BstIdl::FBasicTypeId::get(7U));
        array1->setElementType(typeref);
        // add array1 to interface1
        addListItem(interface1->getTypes(), array1);
        // struct

        // field
        // union
        // array

        // method
        auto method1 = factory.createFMethod();
        method1->setName("Method1");
        method1->setContainer(interface1);
        addListItem(interface1->getMethods(), method1);
        // in argument
        auto inArg = factory.createFArgument();
        inArg->setName("InArgument1");
        inArg->setContainer(method1);
        typeref = factory.createFTypeRef();
        typeref->setPredefined(BstIdl::FBasicTypeId::get(9U));
        inArg->setType(typeref);
        addListItem(method1->getInArgs(), inArg);
        // out argument
        auto outArg = factory.createFArgument();
        outArg->setName("OutArgument1");
        outArg->setContainer(method1);
        typeref = factory.createFTypeRef();
        typeref->setPredefined(BstIdl::FBasicTypeId::get(9U));
        outArg->setType(typeref);
        addListItem(method1->getOutArgs(), outArg);

        // broadcast
        auto broadcast1 = factory.createFBroadcast();
        broadcast1->setName("Broadcast1");
        broadcast1->setContainer(interface1);
        addListItem(interface1->getBroadcasts(), broadcast1);
        // argument
        outArg = factory.createFArgument();
        outArg->setName("OutArgument1");
        outArg->setContainer(broadcast1);
        typeref = factory.createFTypeRef();
        typeref->setPredefined(BstIdl::FBasicTypeId::get(9U));
        outArg->setType(typeref);
        addListItem(broadcast1->getOutArgs(), outArg);

        // Interface2
        auto interface2 = factory.createFInterface();
        interface2->setName("Interface2");
        addListItem(model->getInterfaces(), interface2);
        // enumeration
        enumeration = factory.createFEnumerationType();
        enumeration->setName("Enum1");
        enumeration->setContainer(interface2);
        // enumerator1
        enumerator1 = factory.createFEnumerator();
        enumerator1->setName("Enumerator1");
        enumerator1->setContainer(enumeration);
        value1 = factory.createFIntegerConstant();
        value1->setUnsignedVal(10U);
        enumerator1->setValue(value1);
        addListItem(enumeration->getEnumerators(), enumerator1);
        // enumerator2
        enumerator2 = factory.createFEnumerator();
        enumerator2->setName("Enumerator2");
        enumerator2->setContainer(enumeration);
        value2 = factory.createFIntegerConstant();
        value2->setUnsignedVal(12U);
        enumerator2->setValue(value2);
        addListItem(enumeration->getEnumerators(), enumerator2);
        // add enum to interface2
        addListItem(interface2->getTypes(), enumeration);
        // struct1
        struct1 = factory.createFStructType();
        struct1->setName("Struct1");
        struct1->setContainer(interface2);
        field1 = factory.createFField();
        field1->setName("Field1");
        field1->setContainer(struct1);
        typeref = factory.createFTypeRef();
        typeref->setPredefined(BstIdl::FBasicTypeId::get(7U));
        field1->setType(typeref);
        // add struct1 to interface2
        addListItem(struct1->getElements(), field1);
        addListItem(interface2->getTypes(), struct1);
        // struct2 nested
        struct2 = factory.createFStructType();
        struct2->setName("Struct2");
        struct2->setContainer(interface2);
        field = factory.createFField();
        field->setName("Field2");
        field->setContainer(struct2);
        typeref = factory.createFTypeRef();
        typeref->setDerived(struct1);
        field1->setType(typeref);
        // add nested struct2 to interface2
        addListItem(struct2->getElements(), field);
        addListItem(interface2->getTypes(), struct2);
        // union
        union1 = factory.createFUnionType();
        union1->setName("Union1");
        union1->setContainer(interface2);
        field = factory.createFField();
        field->setName("Field1");
        field->setContainer(union1);
        typeref = factory.createFTypeRef();
        typeref->setPredefined(BstIdl::FBasicTypeId::get(7U));
        field->setType(typeref);
        addListItem(union1->getElements(), field);
        addListItem(interface2->getTypes(), union1);
        // array
        array1 = factory.createFArrayType();
        array1->setName("Array1");
        array1->setContainer(interface2);
        typeref = factory.createFTypeRef();
        typeref->setPredefined(BstIdl::FBasicTypeId::get(7U));
        array1->setElementType(typeref);
        // add array1 to interface2
        addListItem(interface2->getTypes(), array1);
        // map
        map1 = factory.createFMapType();
        map1->setName("Map1");
        map1->setContainer(interface2);
        keyType = factory.createFTypeRef();
        keyType->setPredefined(BstIdl::FBasicTypeId::get(7U));
        valueType = factory.createFTypeRef();
        valueType->setPredefined(BstIdl::FBasicTypeId::get(10U));
        map1->setKeyType(keyType);
        map1->setValueType(valueType);
        // add map1 to interface2
        addListItem(interface2->getTypes(), map1);
        // typedef
        typedef1 = factory.createFTypeDef();
        typedef1->setName("RealUInt64");
        typedef1->setContainer(interface2);
        typeref = factory.createFTypeRef();
        typeref->setPredefined(BstIdl::FBasicTypeId::get(8U));
        typedef1->setActualType(typeref);
        // add typedef1 to interface2
        addListItem(interface2->getTypes(), typedef1);
        // method
        method1 = factory.createFMethod();
        method1->setName("Method1");
        method1->setContainer(interface2);
        addListItem(interface2->getMethods(), method1);
        // in argument
        inArg = factory.createFArgument();
        inArg->setName("InArgument1");
        inArg->setContainer(method1);
        typeref = factory.createFTypeRef();
        typeref->setPredefined(BstIdl::FBasicTypeId::get(9U));
        inArg->setType(typeref);
        addListItem(method1->getInArgs(), inArg);
        // out argument
        outArg = factory.createFArgument();
        outArg->setName("OutArgument1");
        outArg->setContainer(method1);
        typeref = factory.createFTypeRef();
        typeref->setPredefined(BstIdl::FBasicTypeId::get(9U));
        outArg->setType(typeref);
        addListItem(method1->getOutArgs(), outArg);
        // broadcast
        broadcast1 = factory.createFBroadcast();
        broadcast1->setName("Broadcast1");
        broadcast1->setContainer(interface2);
        addListItem(interface2->getBroadcasts(), broadcast1);
        // argument
        outArg = factory.createFArgument();
        outArg->setName("OutArgument1");
        outArg->setContainer(broadcast1);
        typeref = factory.createFTypeRef();
        typeref->setPredefined(BstIdl::FBasicTypeId::get(9U));
        outArg->setType(typeref);
        addListItem(broadcast1->getOutArgs(), outArg);

        // interface3
        auto interface3 = factory.createFInterface();
        interface3->setName("Interface3");
        addListItem(model->getInterfaces(), interface3);
        // enumeration
        enumeration = factory.createFEnumerationType();
        enumeration->setName("Enum1");
        enumeration->setContainer(interface3);
        // enumerator1
        enumerator1 = factory.createFEnumerator();
        enumerator1->setName("Enumerator1");
        enumerator1->setContainer(enumeration);
        value1 = factory.createFIntegerConstant();
        value1->setUnsignedVal(10U);
        enumerator1->setValue(value1);
        addListItem(enumeration->getEnumerators(), enumerator1);
        // enumerator2
        enumerator2 = factory.createFEnumerator();
        enumerator2->setName("Enumerator2");
        enumerator2->setContainer(enumeration);
        value2 = factory.createFIntegerConstant();
        value2->setUnsignedVal(12U);
        enumerator2->setValue(value2);
        addListItem(enumeration->getEnumerators(), enumerator2);
        // add enum to interface3
        addListItem(interface3->getTypes(), enumeration);
        // struct1
        struct1 = factory.createFStructType();
        struct1->setName("Struct1");
        struct1->setContainer(interface3);
        field1 = factory.createFField();
        field1->setName("Field1");
        field1->setContainer(struct1);
        typeref = factory.createFTypeRef();
        typeref->setPredefined(BstIdl::FBasicTypeId::get(7U));
        field1->setType(typeref);
        // add struct1 to interface3
        addListItem(struct1->getElements(), field1);
        addListItem(interface3->getTypes(), struct1);
        // struct2 nested
        struct2 = factory.createFStructType();
        struct2->setName("Struct2");
        struct2->setContainer(interface3);
        field = factory.createFField();
        field->setName("Field2");
        field->setContainer(struct2);
        typeref = factory.createFTypeRef();
        typeref->setDerived(struct1);
        field1->setType(typeref);
        // add nested struct2 to interface3
        addListItem(struct2->getElements(), field);
        addListItem(interface3->getTypes(), struct2);
        // union
        union1 = factory.createFUnionType();
        union1->setName("Union1");
        union1->setContainer(interface3);
        field = factory.createFField();
        field->setName("Field1");
        field->setContainer(union1);
        typeref = factory.createFTypeRef();
        typeref->setPredefined(BstIdl::FBasicTypeId::get(7U));
        field->setType(typeref);
        addListItem(union1->getElements(), field);
        addListItem(interface3->getTypes(), union1);
        // array
        array1 = factory.createFArrayType();
        array1->setName("Array1");
        array1->setContainer(interface3);
        typeref = factory.createFTypeRef();
        typeref->setPredefined(BstIdl::FBasicTypeId::get(7U));
        array1->setElementType(typeref);
        // add array1 to interface3
        addListItem(interface3->getTypes(), array1);
        // map
        map1 = factory.createFMapType();
        map1->setName("Map1");
        map1->setContainer(interface3);
        keyType = factory.createFTypeRef();
        keyType->setPredefined(BstIdl::FBasicTypeId::get(7U));
        valueType = factory.createFTypeRef();
        valueType->setPredefined(BstIdl::FBasicTypeId::get(10U));
        map1->setKeyType(keyType);
        map1->setValueType(valueType);
        // add map1 to interface3
        addListItem(interface3->getTypes(), map1);
        // typedef
        typedef1 = factory.createFTypeDef();
        typedef1->setName("RealUInt64");
        typedef1->setContainer(interface3);
        typeref = factory.createFTypeRef();
        typeref->setPredefined(BstIdl::FBasicTypeId::get(8U));
        typedef1->setActualType(typeref);
        // add typedef1 to interface3
        addListItem(interface3->getTypes(), typedef1);
        // method
        method1 = factory.createFMethod();
        method1->setName("Method1");
        method1->setContainer(interface3);
        addListItem(interface3->getMethods(), method1);
        // in argument
        inArg = factory.createFArgument();
        inArg->setName("InArgument1");
        inArg->setContainer(method1);
        typeref = factory.createFTypeRef();
        typeref->setPredefined(BstIdl::FBasicTypeId::get(9U));
        inArg->setType(typeref);
        addListItem(method1->getInArgs(), inArg);
        // out argument
        outArg = factory.createFArgument();
        outArg->setName("OutArgument1");
        outArg->setContainer(method1);
        typeref = factory.createFTypeRef();
        typeref->setPredefined(BstIdl::FBasicTypeId::get(9U));
        outArg->setType(typeref);
        addListItem(method1->getOutArgs(), outArg);
        // broadcast
        broadcast1 = factory.createFBroadcast();
        broadcast1->setName("Broadcast1");
        broadcast1->setContainer(interface3);
        addListItem(interface3->getBroadcasts(), broadcast1);
        // argument
        outArg = factory.createFArgument();
        outArg->setName("OutArgument1");
        outArg->setContainer(broadcast1);
        typeref = factory.createFTypeRef();
        typeref->setPredefined(BstIdl::FBasicTypeId::get(9U));
        outArg->setType(typeref);
        addListItem(broadcast1->getOutArgs(), outArg);
    }
    // FD
    BstIdl::FDFactory &fdfactory = BstIdl::FDFactory::getInstance();
    BstIdl::FDModelManager &fdmgr = BstIdl::FDModelManager::getInstance();
    BstIdl::FDReferenceManager &fdRmgr = BstIdl::FDReferenceManager::getInstance();
    // FIDL
    BstIdl::FModelManager &mgr = BstIdl::FModelManager::getInstance();
    BstIdl::ReferenceManager &refMgr = BstIdl::ReferenceManager::getInstance();
    BstIdl::FFactory &factory = BstIdl::FFactory::getInstance();
};

TEST_F(FDReferenceManagerTestLT, resolveFDPropertyDeclRef_innerSpec)
{
    fdRmgr.clear();
    auto model = fdmgr.findModel("com.bst.deployment.test.fdmodel1");
    EXPECT_NE(model, nullptr);
    std::string spec("Specification1");
    std::string name("Interface1");
    auto fdi = model->getInterfaceDeployment(spec, name);
    EXPECT_NE(fdi, nullptr);
    // types
    auto type = std::make_shared<BstIdl::FDTypeDefinition>();
    fdi->setTypes(type);
    // property
    auto prop = std::make_shared<BstIdl::FDProperty>();
    type->addProperty(prop);

    fdRmgr.addFDPropertyDeclRef(prop, "Specification1.ifArray");
    auto errList = fdRmgr.resolveFDPropertyDeclRef();
    EXPECT_TRUE(errList.empty());
    EXPECT_NE(prop->getDecl(), nullptr);
    EXPECT_STREQ(prop->getDecl()->getName().c_str(), "ifArray");
}

TEST_F(FDReferenceManagerTestLT, resolveFDPropertyDeclRef_crossSpec)
{
    fdRmgr.clear();
    auto model = fdmgr.findModel("com.bst.deployment.test.fdmodel1");
    EXPECT_NE(model, nullptr);
    std::string specName("Specification2");
    std::string name("TypeCollection1");
    auto types1 = model->getTypeCollectionDeployment(specName, name);
    EXPECT_NE(types1, nullptr);
    auto prop = types1->getTypes().front()->getProperties().front();
    EXPECT_NE(prop, nullptr);

    fdRmgr.addFDPropertyDeclRef(prop, "Specification1.ifArray");
    auto errList = fdRmgr.resolveFDPropertyDeclRef();
    EXPECT_TRUE(errList.empty());
    EXPECT_NE(prop->getDecl(), nullptr);
    EXPECT_STREQ(prop->getDecl()->getName().c_str(), "ifArray");
}

TEST_F(FDReferenceManagerTestLT, resolveFDPropertyDeclRef_crossModel)
{
    fdRmgr.clear();
    auto model = fdmgr.findModel("com.bst.deployment.test.fdmodel1");
    EXPECT_NE(model, nullptr);
    std::string specName("Specification2");
    std::string name("TypeCollection1");
    auto types1 = model->getTypeCollectionDeployment(specName, name);
    EXPECT_NE(types1, nullptr);
    auto prop = types1->getTypes().front()->getProperties().front();
    EXPECT_NE(prop, nullptr);

    fdRmgr.addFDPropertyDeclRef(prop, "Specification3.Integer1");
    auto errList = fdRmgr.resolveFDPropertyDeclRef();
    EXPECT_TRUE(errList.empty());
    EXPECT_NE(prop->getDecl(), nullptr);
    EXPECT_STREQ(prop->getDecl()->getName().c_str(), "Integer1");
}

// undefinedPropertyDecl
TEST_F(FDReferenceManagerTestLT, resolveFDPropertyDeclRef_undefined)
{
    fdRmgr.clear();
    auto model = fdmgr.findModel("com.bst.deployment.test.fdmodel1");
    EXPECT_NE(model, nullptr);
    std::string specName("Specification2");
    std::string name("TypeCollection1");
    auto types1 = model->getTypeCollectionDeployment(specName, name);
    EXPECT_NE(types1, nullptr);
    auto prop = types1->getTypes().front()->getProperties().front();
    EXPECT_NE(prop, nullptr);

    fdRmgr.addFDPropertyDeclRef(prop, "Specification3.Integer2");
    auto errList = fdRmgr.resolveFDPropertyDeclRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(prop->getDecl(), nullptr);
}

//
TEST_F(FDReferenceManagerTestLT, resolveFDPropertyDeclRef_dulplicate)
{
    fdRmgr.clear();
    auto model = fdmgr.findModel("com.bst.deployment.test.fdmodel1");
    EXPECT_NE(model, nullptr);
    std::string spec("Specification1");
    std::string name("Interface1");
    auto fdi = model->getInterfaceDeployment(spec, name);
    EXPECT_NE(fdi, nullptr);
    auto prop = fdi->getTypes().front()->getProperties().front();
    EXPECT_NE(prop, nullptr);

    fdRmgr.addFDPropertyDeclRef(prop, "Specification3.Integer1");
    auto errList = fdRmgr.resolveFDPropertyDeclRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(prop->getDecl(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveFDPropertyDeclRef_multi)
{
    fdRmgr.clear();
    auto model = fdmgr.findModel("com.bst.deployment.test.fdmodel1");
    EXPECT_NE(model, nullptr);
    std::string specName("Specification2");
    std::string name("TypeCollection1");
    auto fdtc = model->getTypeCollectionDeployment(specName, name);
    EXPECT_NE(fdtc, nullptr);
    auto prop = fdtc->getTypes().front()->getProperties().front();
    EXPECT_NE(prop, nullptr);

    fdRmgr.addFDPropertyDeclRef(prop, "Specification3.Integer1");
    fdRmgr.addFDPropertyDeclRef(prop, "Specification1.ifArray");
    fdRmgr.addFDPropertyDeclRef(prop, "Specification1.Integer1");
    auto errList = fdRmgr.resolveFDPropertyDeclRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_STREQ(prop->getDecl()->getName().c_str(), "Integer1");
}

// resolveExtensionRootSpecificationRef
TEST_F(FDReferenceManagerTestLT, resolveExtensionRootSpecificationRef)
{
    fdRmgr.clear();
    auto extRoot = fdfactory.createFDExtensionRoot();
    extRoot->setTag("provider");

    fdRmgr.addExtensionRootSpecificationRef(extRoot, "Specification1");
    auto errList = fdRmgr.resolveExtensionRootSpecificationRef();
    EXPECT_TRUE(errList.empty());
    EXPECT_STREQ(extRoot->getSpec()->getName().c_str(), "Specification1");
}

TEST_F(FDReferenceManagerTestLT, resolveExtensionRootSpecificationRef_undefinedExt)
{
    fdRmgr.clear();
    auto extRoot = fdfactory.createFDExtensionRoot();
    extRoot = nullptr;

    fdRmgr.addExtensionRootSpecificationRef(extRoot, "Specification1");
    auto errList = fdRmgr.resolveExtensionRootSpecificationRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(extRoot, nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveExtensionRootSpecificationRef_undefinedSpec)
{
    fdRmgr.clear();
    auto extRoot = fdfactory.createFDExtensionRoot();
    extRoot->setTag("provider");

    fdRmgr.addExtensionRootSpecificationRef(extRoot, "Specification5");
    auto errList = fdRmgr.resolveExtensionRootSpecificationRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(extRoot->getSpec(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveExtensionRootSpecificationRef_duplicate)
{
    fdRmgr.clear();
    auto extRoot = fdfactory.createFDExtensionRoot();
    extRoot->setTag("provider");
    auto spec = fdmgr.findFDSpecification("Specification1");
    extRoot->setSpec(spec);

    fdRmgr.addExtensionRootSpecificationRef(extRoot, "Specification2");
    auto errList = fdRmgr.resolveExtensionRootSpecificationRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(extRoot->getSpec(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveExtensionRootSpecificationRef_multi)
{
    fdRmgr.clear();
    auto extRoot = fdfactory.createFDExtensionRoot();
    extRoot->setTag("provider");
    auto spec = fdmgr.findFDSpecification("Specification3");
    extRoot->setSpec(spec);

    fdRmgr.addExtensionRootSpecificationRef(extRoot, "Specification2");
    fdRmgr.addExtensionRootSpecificationRef(extRoot, "Specification1");
    auto errList = fdRmgr.resolveExtensionRootSpecificationRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_STREQ(extRoot->getSpec()->getName().c_str(), "Specification1");
}

// resolveExtensionRootTargetRef
TEST_F(FDReferenceManagerTestLT, resolveExtensionRootTargetRef)
{
    fdRmgr.clear();
    auto extRoot = fdfactory.createFDExtensionRoot();
    extRoot->setTag("provider");

    fdRmgr.addExtensionRootTargetRef(extRoot, "com.bst.idl.test.model1.Interface1");
    auto errList = fdRmgr.resolveExtensionRootTargetRef();
    EXPECT_TRUE(errList.empty());
    EXPECT_EQ(extRoot->getTarget(), mgr.findFObject("com.bst.idl.test.model1.Interface1"));
}

TEST_F(FDReferenceManagerTestLT, resolveExtensionRootTargetRef_undefinedExt)
{
    fdRmgr.clear();
    auto extRoot = fdfactory.createFDExtensionRoot();
    extRoot = nullptr;

    fdRmgr.addExtensionRootTargetRef(extRoot, "com.bst.idl.test.model1.Interface1");
    auto errList = fdRmgr.resolveExtensionRootTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(extRoot, nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveExtensionRootTargetRef_undefinedTarget)
{
    fdRmgr.clear();
    auto extRoot = fdfactory.createFDExtensionRoot();
    extRoot->setTag("provider");

    fdRmgr.addExtensionRootTargetRef(extRoot, "com.bst.idl.test.model1.Interface5");
    auto errList = fdRmgr.resolveExtensionRootTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(extRoot->getTarget(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveExtensionRootTargetRef_duplicate)
{
    fdRmgr.clear();
    auto extRoot = fdfactory.createFDExtensionRoot();
    extRoot->setTag("provider");
    auto target = mgr.findFObject("com.bst.idl.test.model1.Interface2");
    extRoot->setTarget(target);

    fdRmgr.addExtensionRootTargetRef(extRoot, "com.bst.idl.test.model1.Interface1");
    auto errList = fdRmgr.resolveExtensionRootTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(extRoot->getTarget(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveExtensionRootTargetRef_multi)
{
    fdRmgr.clear();
    auto extRoot = fdfactory.createFDExtensionRoot();
    extRoot->setTag("provider");
    auto target = mgr.findFObject("com.bst.idl.test.model1.Interface3");
    extRoot->setTarget(target);

    fdRmgr.addExtensionRootTargetRef(extRoot, "com.bst.idl.test.model1.Interface2");
    fdRmgr.addExtensionRootTargetRef(extRoot, "com.bst.idl.test.model1.Interface1");
    auto errList = fdRmgr.resolveExtensionRootTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_STREQ(extRoot->getTarget()->getName().c_str(), "Interface1");
}

// resolveExtensionRootUseRef
TEST_F(FDReferenceManagerTestLT, resolveExtensionRootUseRef)
{
    fdRmgr.clear();
    auto extRoot = fdfactory.createFDExtensionRoot();
    extRoot->setTag("provider");

    fdRmgr.addExtensionRootUseRef(extRoot, "TypeCollection2");
    auto errList = fdRmgr.resolveExtensionRootUseRef();
    EXPECT_TRUE(errList.empty());
    EXPECT_EQ(extRoot->getUse().front(), fdmgr.findFDRootElement("TypeCollection2"));
}

TEST_F(FDReferenceManagerTestLT, resolveExtensionRootUseRef_undefinedExt)
{
    fdRmgr.clear();
    auto extRoot = fdfactory.createFDExtensionRoot();
    extRoot = nullptr;

    fdRmgr.addExtensionRootUseRef(extRoot, "TypeCollection1");
    auto errList = fdRmgr.resolveExtensionRootUseRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(extRoot, nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveExtensionRootUseRef_undefinedTarget)
{
    fdRmgr.clear();
    auto extRoot = fdfactory.createFDExtensionRoot();
    extRoot->setTag("provider");

    fdRmgr.addExtensionRootUseRef(extRoot, "TypeCollection10");
    auto errList = fdRmgr.resolveExtensionRootUseRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(extRoot->getUse().empty(), true);
}

TEST_F(FDReferenceManagerTestLT, resolveExtensionRootUseRef_multi)
{
    fdRmgr.clear();
    auto extRoot = fdfactory.createFDExtensionRoot();
    extRoot->setTag("provider");

    fdRmgr.addExtensionRootUseRef(extRoot, "TypeCollection1");
    fdRmgr.addExtensionRootUseRef(extRoot, "TypeCollection2");
    fdRmgr.addExtensionRootUseRef(extRoot, "Interface1");
    auto errList = fdRmgr.resolveExtensionRootUseRef();
    EXPECT_TRUE(errList.empty());
    EXPECT_EQ(extRoot->getUse().size(), 3U);
}

// resolveExtensionElementTargetRef
TEST_F(FDReferenceManagerTestLT, resolveExtensionElementTargetRef)
{
    fdRmgr.clear();
    auto element = fdfactory.createFDExtensionElement();

    fdRmgr.addExtensionElementTargetRef(element, "com.bst.idl.test.model1.Interface1");
    auto errList = fdRmgr.resolveExtensionElementTargetRef();
    EXPECT_TRUE(errList.empty());
    EXPECT_EQ(element->getTarget(), mgr.findFObject("com.bst.idl.test.model1.Interface1"));
}

TEST_F(FDReferenceManagerTestLT, resolveExtensionElementTargetRef_undefinedExt)
{
    fdRmgr.clear();
    auto element = fdfactory.createFDExtensionElement();
    element = nullptr;

    fdRmgr.addExtensionElementTargetRef(element, "com.bst.idl.test.model1.Interface1");
    auto errList = fdRmgr.resolveExtensionElementTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(element, nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveExtensionElementTargetRef_undefinedTarget)
{
    fdRmgr.clear();
    auto element = fdfactory.createFDExtensionElement();

    fdRmgr.addExtensionElementTargetRef(element, "com.bst.idl.test.model1.Interface5");
    auto errList = fdRmgr.resolveExtensionElementTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(element->getTarget(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveExtensionElementTargetRef_duplicate)
{
    fdRmgr.clear();
    auto element = fdfactory.createFDExtensionElement();
    auto target = mgr.findFObject("com.bst.idl.test.model1.Interface2");
    element->setTarget(target);

    fdRmgr.addExtensionElementTargetRef(element, "com.bst.idl.test.model1.Interface1");
    auto errList = fdRmgr.resolveExtensionElementTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(element->getTarget(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveExtensionElementTargetRef_multi)
{
    fdRmgr.clear();
    auto element = fdfactory.createFDExtensionElement();
    auto target = mgr.findFObject("com.bst.idl.test.model1.Interface3");
    element->setTarget(target);

    fdRmgr.addExtensionElementTargetRef(element, "com.bst.idl.test.model1.Interface2");
    fdRmgr.addExtensionElementTargetRef(element, "com.bst.idl.test.model1.Interface1");
    auto errList = fdRmgr.resolveExtensionElementTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_STREQ(element->getTarget()->getName().c_str(), "Interface1");
}

// resolveSpecificationExtendsRef
TEST_F(FDReferenceManagerTestLT, resolveSpecificationExtendsRef_innerModel)
{
    fdRmgr.clear();
    std::string name("Specification1");
    auto spec = fdmgr.findFDSpecification(name);

    fdRmgr.addSpecificationExtendsRef(spec, "Specification2");
    auto errList = fdRmgr.resolveSpecificationExtendsRef();
    EXPECT_TRUE(errList.empty());
    EXPECT_EQ(spec->getBase(), fdmgr.findFDSpecification("Specification2"));
}

TEST_F(FDReferenceManagerTestLT, resolveSpecificationExtendsRef_crossModel)
{
    fdRmgr.clear();
    std::string name("Specification1");
    auto spec = fdmgr.findFDSpecification(name);

    fdRmgr.addSpecificationExtendsRef(spec, "Specification3");
    auto errList = fdRmgr.resolveSpecificationExtendsRef();
    EXPECT_TRUE(errList.empty());
    EXPECT_EQ(spec->getBase(), fdmgr.findFDSpecification("Specification3"));
}

TEST_F(FDReferenceManagerTestLT, resolveSpecificationExtendsRef_undefined)
{
    fdRmgr.clear();
    std::string name("Specification1");
    auto spec = fdmgr.findFDSpecification(name);

    fdRmgr.addSpecificationExtendsRef(spec, "Specification10");
    auto errList = fdRmgr.resolveSpecificationExtendsRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(spec->getBase(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveSpecificationExtendsRef_dulpli)
{
    fdRmgr.clear();
    std::string name("Specification1");
    auto spec = fdmgr.findFDSpecification(name);
    auto base = fdmgr.findFDSpecification("Specification2");
    spec->setBase(base);

    fdRmgr.addSpecificationExtendsRef(spec, "Specification3");
    auto errList = fdRmgr.resolveSpecificationExtendsRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(spec->getBase(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveSpecificationExtendsRef_multi)
{
    fdRmgr.clear();
    std::string name("Specification1");
    auto spec = fdmgr.findFDSpecification(name);

    fdRmgr.addSpecificationExtendsRef(spec, "Specification3");
    fdRmgr.addSpecificationExtendsRef(spec, "Specification2");
    auto errList = fdRmgr.resolveSpecificationExtendsRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(spec->getBase(), nullptr);
}

// resolveTypesSpecificationRef
TEST_F(FDReferenceManagerTestLT, resolveTypesSpecificationRef)
{
    fdRmgr.clear();
    auto model = fdmgr.findModel("com.bst.deployment.test.fdmodel1");
    auto types = fdfactory.createFDTypes();
    types->setName("TypeCollection3");
    types->setContainer(model);
    addListItem(model->getDeployments(), types);

    fdRmgr.addTypesSpecificationRef(types, "Specification1");
    auto errList = fdRmgr.resolveTypesSpecificationRef();
    EXPECT_TRUE(errList.empty());
    EXPECT_EQ(types->getSpec(), fdmgr.findFDSpecification("Specification1"));
}

TEST_F(FDReferenceManagerTestLT, resolveTypesSpecificationRef_undefined)
{
    fdRmgr.clear();
    auto types = fdfactory.createFDTypes();

    fdRmgr.addTypesSpecificationRef(types, "Specification6");
    auto errList = fdRmgr.resolveTypesSpecificationRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(types->getSpec(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveTypesSpecificationRef_duplicate)
{
    fdRmgr.clear();
    auto types = fdfactory.createFDTypes();
    types->setSpec(fdmgr.findFDSpecification("Specification1"));

    fdRmgr.addTypesSpecificationRef(types, "Specification2");
    auto errList = fdRmgr.resolveTypesSpecificationRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(types->getSpec(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveTypesSpecificationRef_multi)
{
    fdRmgr.clear();
    auto types = fdfactory.createFDTypes();

    fdRmgr.addTypesSpecificationRef(types, "Specification1");
    fdRmgr.addTypesSpecificationRef(types, "Specification2");
    fdRmgr.addTypesSpecificationRef(types, "Specification3");
    auto errList = fdRmgr.resolveTypesSpecificationRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(types->getSpec(), fdmgr.findFDSpecification("Specification3"));
}

// resolveTypesTypeCollectionRef
TEST_F(FDReferenceManagerTestLT, resolveTypesTypeCollectionRef)
{
    fdRmgr.clear();
    auto types = fdfactory.createFDTypes();

    fdRmgr.addTypesTypeCollectionRef(types, "com.bst.idl.test.model1.TypeCollection1");
    auto errList = fdRmgr.resolveTypesTypeCollectionRef();
    EXPECT_TRUE(errList.empty());
    EXPECT_EQ(types->getTarget(), mgr.findFTypeCollection("com.bst.idl.test.model1.TypeCollection1"));
}

TEST_F(FDReferenceManagerTestLT, resolveTypesTypeCollectionRef_undefined)
{
    fdRmgr.clear();
    auto types = fdfactory.createFDTypes();

    fdRmgr.addTypesTypeCollectionRef(types, "com.bst.idl.test.model1.TypeCollection9");
    auto errList = fdRmgr.resolveTypesTypeCollectionRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(types->getTarget(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveTypesTypeCollectionRef_duplicate)
{
    fdRmgr.clear();
    auto types = fdfactory.createFDTypes();
    types->setTarget(mgr.findFTypeCollection("com.bst.idl.test.model1.TypeCollection2"));

    fdRmgr.addTypesTypeCollectionRef(types, "com.bst.idl.test.model1.TypeCollection1");
    auto errList = fdRmgr.resolveTypesTypeCollectionRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(types->getTarget(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveTypesTypeCollectionRef_multi)
{
    fdRmgr.clear();
    auto types = fdfactory.createFDTypes();

    fdRmgr.addTypesTypeCollectionRef(types, "com.bst.idl.test.model1.TypeCollection1");
    fdRmgr.addTypesTypeCollectionRef(types, "com.bst.idl.test.model1.TypeCollection2");
    fdRmgr.addTypesTypeCollectionRef(types, "com.bst.idl.test.model1.TypeCollection3");
    auto errList = fdRmgr.resolveTypesTypeCollectionRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(types->getTarget(), mgr.findFTypeCollection("com.bst.idl.test.model1.TypeCollection3"));
}

// resolveTypesUseRef
TEST_F(FDReferenceManagerTestLT, resolveTypesUseRef)
{
    fdRmgr.clear();
    auto types = fdfactory.createFDTypes();

    fdRmgr.addTypesUseRef(types, "TypeCollection1");
    auto errList = fdRmgr.resolveTypesUseRef();
    EXPECT_TRUE(errList.empty());
    EXPECT_EQ(types->getUse().front(), fdmgr.findFDRootElement("TypeCollection1"));
}

TEST_F(FDReferenceManagerTestLT, resolveTypesUseRef_undefined)
{
    fdRmgr.clear();
    auto types = fdfactory.createFDTypes();

    fdRmgr.addTypesUseRef(types, "TypeCollection9");
    auto errList = fdRmgr.resolveTypesUseRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(types->getUse().front(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveTypesUseRef_duplicate)
{
    fdRmgr.clear();
    auto types = fdfactory.createFDTypes();
    types->addUse(fdmgr.findFDRootElement("TypeCollection1"));

    fdRmgr.addTypesUseRef(types, "TypeCollection1");
    auto errList = fdRmgr.resolveTypesUseRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(types->getUse().front(), fdmgr.findFDRootElement("TypeCollection1"));
}

TEST_F(FDReferenceManagerTestLT, resolveTypesUseRef_multi)
{
    fdRmgr.clear();
    auto types = fdfactory.createFDTypes();

    fdRmgr.addTypesUseRef(types, "TypeCollection1");
    fdRmgr.addTypesUseRef(types, "TypeCollection3");
    fdRmgr.addTypesUseRef(types, "TypeCollection2");
    auto errList = fdRmgr.resolveTypesUseRef();
    EXPECT_TRUE(errList.empty());
    EXPECT_EQ(types->getUse().size(), 3U);
}

// resolveInterfaceSpecificationRef
TEST_F(FDReferenceManagerTestLT, resolveInterfaceSpecificationRef)
{
    fdRmgr.clear();
    auto interface = fdfactory.createFDInterface();

    fdRmgr.addInterfaceSpecificationRef(interface, "Specification1");
    auto errList = fdRmgr.resolveInterfaceSpecificationRef();
    EXPECT_TRUE(errList.empty());
    EXPECT_EQ(interface->getSpec(), fdmgr.findFDSpecification("Specification1"));
}

TEST_F(FDReferenceManagerTestLT, resolveInterfaceSpecificationRef_undefined)
{
    fdRmgr.clear();
    auto interface = fdfactory.createFDInterface();

    fdRmgr.addInterfaceSpecificationRef(interface, "Specification9");
    auto errList = fdRmgr.resolveInterfaceSpecificationRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(interface->getSpec(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveInterfaceSpecificationRef_duplicate)
{
    fdRmgr.clear();
    auto interface = fdfactory.createFDInterface();
    interface->setSpec(fdmgr.findFDSpecification("Specification1"));

    fdRmgr.addInterfaceSpecificationRef(interface, "Specification2");
    auto errList = fdRmgr.resolveInterfaceSpecificationRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(interface->getSpec(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveInterfaceSpecificationRef_multi)
{
    fdRmgr.clear();
    auto interface = fdfactory.createFDInterface();

    fdRmgr.addInterfaceSpecificationRef(interface, "Specification2");
    fdRmgr.addInterfaceSpecificationRef(interface, "Specification1");
    fdRmgr.addInterfaceSpecificationRef(interface, "Specification3");
    auto errList = fdRmgr.resolveInterfaceSpecificationRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(interface->getSpec(), fdmgr.findFDSpecification("Specification3"));
}

// resolveInterfaceTargetRef
TEST_F(FDReferenceManagerTestLT, resolveInterfaceTargetRef)
{
    fdRmgr.clear();
    auto interface = fdfactory.createFDInterface();

    fdRmgr.addInterfaceInterfaceRef(interface, "com.bst.idl.test.model1.Interface1");
    auto errList = fdRmgr.resolveInterfaceTargetRef();
    EXPECT_TRUE(errList.empty());
    EXPECT_EQ(interface->getTarget(), mgr.findFInterface("com.bst.idl.test.model1.Interface1"));
}

TEST_F(FDReferenceManagerTestLT, resolveInterfaceTargetRef_undefined)
{
    fdRmgr.clear();
    auto interface = fdfactory.createFDInterface();

    fdRmgr.addInterfaceInterfaceRef(interface, "com.bst.idl.test.model1.Interface9");
    auto errList = fdRmgr.resolveInterfaceTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(interface->getTarget(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveInterfaceTargetRef_duplicate)
{
    fdRmgr.clear();
    auto interface = fdfactory.createFDInterface();
    interface->setTarget(mgr.findFInterface("com.bst.idl.test.model1.Interface1"));

    fdRmgr.addInterfaceInterfaceRef(interface, "com.bst.idl.test.model1.Interface2");
    auto errList = fdRmgr.resolveInterfaceTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(interface->getTarget(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveInterfaceTargetRef_multi)
{
    fdRmgr.clear();
    auto interface = fdfactory.createFDInterface();

    fdRmgr.addInterfaceInterfaceRef(interface, "com.bst.idl.test.model1.Interface2");
    fdRmgr.addInterfaceInterfaceRef(interface, "com.bst.idl.test.model1.Interface3");
    fdRmgr.addInterfaceInterfaceRef(interface, "com.bst.idl.test.model1.Interface1");
    auto errList = fdRmgr.resolveInterfaceTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(interface->getTarget(), mgr.findFInterface("com.bst.idl.test.model1.Interface1"));
}

// resolveInterfaceUseRef
TEST_F(FDReferenceManagerTestLT, resolveInterfaceUseRef)
{
    fdRmgr.clear();
    auto interface = fdfactory.createFDInterface();

    fdRmgr.addInterfaceUseRef(interface, "Interface1");
    auto errList = fdRmgr.resolveInterfaceUseRef();
    EXPECT_TRUE(errList.empty());
    EXPECT_EQ(interface->getUse().front(), fdmgr.findFDRootElement("Interface1"));
}

TEST_F(FDReferenceManagerTestLT, resolveInterfaceUseRef_undefined)
{
    fdRmgr.clear();
    auto interface = fdfactory.createFDInterface();

    fdRmgr.addInterfaceUseRef(interface, "Interface9");
    auto errList = fdRmgr.resolveInterfaceUseRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(interface->getUse().front(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveInterfaceUseRef_duplicate)
{
    fdRmgr.clear();
    auto interface = fdfactory.createFDInterface();
    interface->addUse(fdmgr.findFDRootElement("Interface1"));

    fdRmgr.addInterfaceUseRef(interface, "Interface1");
    auto errList = fdRmgr.resolveInterfaceUseRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(interface->getUse().front(), fdmgr.findFDRootElement("Interface1"));
}

TEST_F(FDReferenceManagerTestLT, resolveInterfaceUseRef_multi)
{
    fdRmgr.clear();
    auto interface = fdfactory.createFDInterface();

    fdRmgr.addInterfaceUseRef(interface, "Interface1");
    fdRmgr.addInterfaceUseRef(interface, "Interface2");
    fdRmgr.addInterfaceUseRef(interface, "Interface3");
    auto errList = fdRmgr.resolveInterfaceUseRef();
    EXPECT_TRUE(errList.empty());
    EXPECT_EQ(interface->getUse().size(), 3U);
}

// 14个target
// resolveAttributeTargetRef
TEST_F(FDReferenceManagerTestLT, resolveAttributeTargetRef)
{
    fdRmgr.clear();
    auto attribute = std::make_shared<BstIdl::FDAttribute>();

    fdRmgr.addAttributeAttributeRef(attribute, "com.bst.idl.test.model1.Interface1.Attribute1");
    auto errList = fdRmgr.resolveAttributeTargetRef();
    EXPECT_TRUE(errList.empty());
    EXPECT_EQ(attribute->getTarget(), mgr.findFAttribute("com.bst.idl.test.model1.Interface1.Attribute1"));
}

TEST_F(FDReferenceManagerTestLT, resolveAttributeTargetRef_undefined)
{
    fdRmgr.clear();
    auto attribute = std::make_shared<BstIdl::FDAttribute>();

    fdRmgr.addAttributeAttributeRef(attribute, "com.bst.idl.test.model1.Interface1.Attribute9");
    auto errList = fdRmgr.resolveAttributeTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(attribute->getTarget(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveAttributeTargetRef_duplicate)
{
    fdRmgr.clear();
    auto attribute = std::make_shared<BstIdl::FDAttribute>();
    attribute->setTarget(mgr.findFAttribute("com.bst.idl.test.model1.Interface1.Attribute1"));

    fdRmgr.addAttributeAttributeRef(attribute, "com.bst.idl.test.model1.Interface1.Attribute2");
    auto errList = fdRmgr.resolveAttributeTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(attribute->getTarget(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveAttributeTargetRef_multi)
{
    fdRmgr.clear();
    auto attribute = std::make_shared<BstIdl::FDAttribute>();

    fdRmgr.addAttributeAttributeRef(attribute, "com.bst.idl.test.model1.Interface1.Attribute2");
    fdRmgr.addAttributeAttributeRef(attribute, "com.bst.idl.test.model1.Interface1.Attribute3");
    fdRmgr.addAttributeAttributeRef(attribute, "com.bst.idl.test.model1.Interface1.Attribute1");
    auto errList = fdRmgr.resolveAttributeTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(attribute->getTarget(), mgr.findFAttribute("com.bst.idl.test.model1.Interface1.Attribute1"));
}
/*
//resolveMethodTargetRef
TEST_F(FDReferenceManagerTestLT, resolveMethodTargetRef)
{
    fdRmgr.clear();
    auto method = std::make_shared<BstIdl::FDMethod>();

    fdRmgr.addMethodTargetRef(method, "com.bst.idl.test.model1.Interface1.Method1");
    auto errList = fdRmgr.resolveMethodTargetRef();
    EXPECT_TRUE(errList.empty());
    EXPECT_EQ(method->getTarget(), mgr.findFMethod("com.bst.idl.test.model1.Interface1.Method1"));
}

TEST_F(FDReferenceManagerTestLT, resolveMethodTargetRef_undefined)
{
    fdRmgr.clear();
    auto method = std::make_shared<BstIdl::FDMethod>();

    fdRmgr.addMethodTargetRef(method, "com.bst.idl.test.model1.Interface1.Method4");
    auto errList = fdRmgr.resolveMethodTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(method->getTarget(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveMethodTargetRef_duplicate)
{
    fdRmgr.clear();
    auto method = std::make_shared<BstIdl::FDMethod>();
    method->setTarget(mgr.findFMethod("com.bst.idl.test.model1.Interface1.Method1"));

    fdRmgr.addMethodTargetRef(method, "com.bst.idl.test.model1.Interface2.Method1");
    auto errList = fdRmgr.resolveMethodTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(method->getTarget(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveMethodTargetRef_multi)
{
    fdRmgr.clear();
    auto method = std::make_shared<BstIdl::FDMethod>();
    method->setTarget(mgr.findFMethod("com.bst.idl.test.model1.Interface1.Method1"));

    fdRmgr.addMethodTargetRef(method, "com.bst.idl.test.model1.Interface2.Method1");
    fdRmgr.addMethodTargetRef(method, "com.bst.idl.test.model1.Interface3.Method1");
    auto errList = fdRmgr.resolveMethodTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(method->getTarget(), mgr.findFMethod("com.bst.idl.test.model1.Interface3.Method1"));
}

//resolveArgumentTargetRef
TEST_F(FDReferenceManagerTestLT, resolveArgumentTargetRef)
{
    fdRmgr.clear();
    auto argument = std::make_shared<BstIdl::FDArgument>();

    fdRmgr.addArgumentTargetRef(argument, "com.bst.idl.test.model1.Interface1.Method1.InArgument1");
    auto errList = fdRmgr.resolveArgumentTargetRef();
    EXPECT_TRUE(errList.empty());
    EXPECT_EQ(argument->getTarget(), mgr.findFArgument("com.bst.idl.test.model1.Interface1.Method1.InArgument1"));
}

TEST_F(FDReferenceManagerTestLT, resolveArgumentTargetRef_undefined)
{
    fdRmgr.clear();
    auto argument = std::make_shared<BstIdl::FDArgument>();

    fdRmgr.addArgumentTargetRef(argument, "com.bst.idl.test.model1.Interface1.Method1.InArgument9");
    auto errList = fdRmgr.resolveArgumentTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(argument->getTarget(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveArgumentTargetRef_duplicate)
{
    fdRmgr.clear();
    auto argument = std::make_shared<BstIdl::FDArgument>();
    argument->setTarget(mgr.findFArgument("com.bst.idl.test.model1.Interface1.Method1.InArgument1"));

    fdRmgr.addArgumentTargetRef(argument, "com.bst.idl.test.model1.Interface2.Broadcast1.OutArgument1");
    auto errList = fdRmgr.resolveArgumentTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(argument->getTarget(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveArgumentTargetRef_multi)
{
    fdRmgr.clear();
    auto argument = std::make_shared<BstIdl::FDArgument>();
    argument->setTarget(mgr.findFArgument("com.bst.idl.test.model1.Interface1.Method1.InArgument1"));

    fdRmgr.addArgumentTargetRef(argument, "com.bst.idl.test.model1.Interface2.Method1.InArgument1");
    fdRmgr.addArgumentTargetRef(argument, "com.bst.idl.test.model1.Interface2.Broadcast1.InArgument1");
    auto errList = fdRmgr.resolveArgumentTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(argument->getTarget(), mgr.findFArgument("com.bst.idl.test.model1.Interface2.Broadcast1.InArgument1"));
}
*/
// resolveArrayTargetRef
TEST_F(FDReferenceManagerTestLT, resolveArrayTargetRef)
{
    fdRmgr.clear();
    auto array = std::make_shared<BstIdl::FDArray>();

    fdRmgr.addArrayTargetRef(array, "com.bst.idl.test.model1.Interface2.Array1");
    auto errList = fdRmgr.resolveArrayTargetRef();
    EXPECT_TRUE(errList.empty());
    EXPECT_EQ(array->getTarget(), std::dynamic_pointer_cast<BstIdl::FArrayType>(
                                      mgr.findPublicAndPrivateFType("com.bst.idl.test.model1.Interface2.Array1")));
}

TEST_F(FDReferenceManagerTestLT, resolveArrayTargetRef_undefined)
{
    fdRmgr.clear();
    auto array = std::make_shared<BstIdl::FDArray>();

    fdRmgr.addArrayTargetRef(array, "com.bst.idl.test.model1.Interface1.Array9");
    auto errList = fdRmgr.resolveArrayTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(array->getTarget(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveArrayTargetRef_duplicate)
{
    fdRmgr.clear();
    auto array = std::make_shared<BstIdl::FDArray>();
    array->setTarget(std::dynamic_pointer_cast<BstIdl::FArrayType>(
        mgr.findPublicAndPrivateFType("com.bst.idl.test.model1.Interface2.Array1")));

    fdRmgr.addArrayTargetRef(array, "com.bst.idl.test.model1.Interface3.Array1");
    auto errList = fdRmgr.resolveArrayTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(array->getTarget(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveArrayTargetRef_multi)
{
    fdRmgr.clear();
    auto array = std::make_shared<BstIdl::FDArray>();
    array->setTarget(std::dynamic_pointer_cast<BstIdl::FArrayType>(
        mgr.findPublicAndPrivateFType("com.bst.idl.test.model1.Interface2.Array1")));

    fdRmgr.addArrayTargetRef(array, "com.bst.idl.test.model1.Interface3.Array1");
    fdRmgr.addArrayTargetRef(array, "com.bst.idl.test.model1.TypeCollection1.Array1");
    auto errList = fdRmgr.resolveArrayTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(array->getTarget(), std::dynamic_pointer_cast<BstIdl::FArrayType>(
                                      mgr.findPublicAndPrivateFType("com.bst.idl.test.model1.TypeCollection1.Array1")));
}

// resolveFDStructTargetRef
TEST_F(FDReferenceManagerTestLT, resolveFDStructTargetRef)
{
    fdRmgr.clear();
    auto struct_ = std::make_shared<BstIdl::FDStruct>();

    fdRmgr.addFDStructTargetRef(struct_, "com.bst.idl.test.model1.Interface2.Struct1");
    auto errList = fdRmgr.resolveFDStructTargetRef();
    EXPECT_TRUE(errList.empty());
    EXPECT_EQ(struct_->getTarget(), std::dynamic_pointer_cast<BstIdl::FStructType>(
                                        mgr.findPublicAndPrivateFType("com.bst.idl.test.model1.Interface2.Struct1")));
}

TEST_F(FDReferenceManagerTestLT, resolveFDStructTargetRef_undefined)
{
    fdRmgr.clear();
    auto struct_ = std::make_shared<BstIdl::FDStruct>();

    fdRmgr.addFDStructTargetRef(struct_, "com.bst.idl.test.model1.Interface1.Struct9");
    auto errList = fdRmgr.resolveFDStructTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(struct_->getTarget(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveFDStructTargetRef_duplicate)
{
    fdRmgr.clear();
    auto struct_ = std::make_shared<BstIdl::FDStruct>();
    struct_->setTarget(std::dynamic_pointer_cast<BstIdl::FStructType>(
        mgr.findPublicAndPrivateFType("com.bst.idl.test.model1.Interface2.Struct1")));

    fdRmgr.addFDStructTargetRef(struct_, "com.bst.idl.test.model1.Interface3.Struct1");
    auto errList = fdRmgr.resolveFDStructTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(struct_->getTarget(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveFDStructTargetRef_multi)
{
    fdRmgr.clear();
    auto struct_ = std::make_shared<BstIdl::FDStruct>();
    struct_->setTarget(std::dynamic_pointer_cast<BstIdl::FStructType>(
        mgr.findPublicAndPrivateFType("com.bst.idl.test.model1.Interface2.Struct1")));

    fdRmgr.addFDStructTargetRef(struct_, "com.bst.idl.test.model1.Interface3.Struct1");
    fdRmgr.addFDStructTargetRef(struct_, "com.bst.idl.test.model1.TypeCollection1.Struct1");
    auto errList = fdRmgr.resolveFDStructTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(struct_->getTarget(), std::dynamic_pointer_cast<BstIdl::FStructType>(mgr.findPublicAndPrivateFType(
                                        "com.bst.idl.test.model1.TypeCollection1.Struct1")));
}

// resolveFDUnionTargetRef
TEST_F(FDReferenceManagerTestLT, resolveFDUnionTargetRef)
{
    fdRmgr.clear();
    auto union_ = std::make_shared<BstIdl::FDUnion>();

    fdRmgr.addFDUnionTargetRef(union_, "com.bst.idl.test.model1.Interface2.Union1");
    auto errList = fdRmgr.resolveFDUnionTargetRef();
    EXPECT_TRUE(errList.empty());
    EXPECT_EQ(union_->getTarget(), std::dynamic_pointer_cast<BstIdl::FUnionType>(
                                       mgr.findPublicAndPrivateFType("com.bst.idl.test.model1.Interface2.Union1")));
}

TEST_F(FDReferenceManagerTestLT, resolveFDUnionTargetRef_undefined)
{
    fdRmgr.clear();
    auto union_ = std::make_shared<BstIdl::FDUnion>();

    fdRmgr.addFDUnionTargetRef(union_, "com.bst.idl.test.model1.Interface1.Union9");
    auto errList = fdRmgr.resolveFDUnionTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(union_->getTarget(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveFDUnionTargetRef_duplicate)
{
    fdRmgr.clear();
    auto union_ = std::make_shared<BstIdl::FDUnion>();
    union_->setTarget(std::dynamic_pointer_cast<BstIdl::FUnionType>(
        mgr.findPublicAndPrivateFType("com.bst.idl.test.model1.Interface2.Union1")));

    fdRmgr.addFDUnionTargetRef(union_, "com.bst.idl.test.model1.Interface3.Union1");
    auto errList = fdRmgr.resolveFDUnionTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(union_->getTarget(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveFDUnionTargetRef_multi)
{
    fdRmgr.clear();
    auto union_ = std::make_shared<BstIdl::FDUnion>();
    union_->setTarget(std::dynamic_pointer_cast<BstIdl::FUnionType>(
        mgr.findPublicAndPrivateFType("com.bst.idl.test.model1.Interface2.Union1")));

    fdRmgr.addFDUnionTargetRef(union_, "com.bst.idl.test.model1.Interface3.Union1");
    fdRmgr.addFDUnionTargetRef(union_, "com.bst.idl.test.model1.TypeCollection1.Union1");
    auto errList = fdRmgr.resolveFDUnionTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(union_->getTarget(), std::dynamic_pointer_cast<BstIdl::FUnionType>(mgr.findPublicAndPrivateFType(
                                       "com.bst.idl.test.model1.TypeCollection1.Union1")));
}

// resolveFDTypedefTargetRef
TEST_F(FDReferenceManagerTestLT, resolveFDTypedefTargetRef)
{
    fdRmgr.clear();
    auto typeDef = std::make_shared<BstIdl::FDTypedef>();

    fdRmgr.addFDTypedefTargetRef(typeDef, "com.bst.idl.test.model1.Interface2.RealUInt64");
    auto errList = fdRmgr.resolveFDTypedefTargetRef();
    EXPECT_TRUE(errList.empty());
    EXPECT_EQ(typeDef->getTarget(), std::dynamic_pointer_cast<BstIdl::FTypeDef>(mgr.findPublicAndPrivateFType(
                                        "com.bst.idl.test.model1.Interface2.RealUInt64")));
}

TEST_F(FDReferenceManagerTestLT, resolveFDTypedefTargetRef_undefined)
{
    fdRmgr.clear();
    auto typeDef = std::make_shared<BstIdl::FDTypedef>();

    fdRmgr.addFDTypedefTargetRef(typeDef, "com.bst.idl.test.model1.Interface1.RealUInt649");
    auto errList = fdRmgr.resolveFDTypedefTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(typeDef->getTarget(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveFDTypedefTargetRef_duplicate)
{
    fdRmgr.clear();
    auto typeDef = std::make_shared<BstIdl::FDTypedef>();
    typeDef->setTarget(std::dynamic_pointer_cast<BstIdl::FTypeDef>(
        mgr.findPublicAndPrivateFType("com.bst.idl.test.model1.Interface2.RealUInt64")));

    fdRmgr.addFDTypedefTargetRef(typeDef, "com.bst.idl.test.model1.Interface3.RealUInt64");
    auto errList = fdRmgr.resolveFDTypedefTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(typeDef->getTarget(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveFDTypedefTargetRef_multi)
{
    fdRmgr.clear();
    auto typeDef = std::make_shared<BstIdl::FDTypedef>();
    typeDef->setTarget(std::dynamic_pointer_cast<BstIdl::FTypeDef>(
        mgr.findPublicAndPrivateFType("com.bst.idl.test.model1.Interface2.RealUInt64")));

    fdRmgr.addFDTypedefTargetRef(typeDef, "com.bst.idl.test.model1.Interface3.RealUInt64");
    fdRmgr.addFDTypedefTargetRef(typeDef, "com.bst.idl.test.model1.TypeCollection1.RealUInt64");
    auto errList = fdRmgr.resolveFDTypedefTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(typeDef->getTarget(), std::dynamic_pointer_cast<BstIdl::FTypeDef>(mgr.findPublicAndPrivateFType(
                                        "com.bst.idl.test.model1.TypeCollection1.RealUInt64")));
}

// resolveFDEnumerationTargetRef
TEST_F(FDReferenceManagerTestLT, resolveFDEnumerationTargetRef)
{
    fdRmgr.clear();
    auto enumeration = std::make_shared<BstIdl::FDEnumeration>();

    fdRmgr.addFDEnumerationTargetRef(enumeration, "com.bst.idl.test.model1.TypeCollection1.Enum1");
    auto errList = fdRmgr.resolveFDEnumerationTargetRef();
    EXPECT_TRUE(errList.empty());
    EXPECT_EQ(enumeration->getTarget(),
              std::dynamic_pointer_cast<BstIdl::FEnumerationType>(
                  mgr.findPublicAndPrivateFType("com.bst.idl.test.model1.TypeCollection1.Enum1")));
}

TEST_F(FDReferenceManagerTestLT, resolveFDEnumerationTargetRef_undefined)
{
    fdRmgr.clear();
    auto enumeration = std::make_shared<BstIdl::FDEnumeration>();

    fdRmgr.addFDEnumerationTargetRef(enumeration, "com.bst.idl.test.model1.Interface2.Enum9");
    auto errList = fdRmgr.resolveFDEnumerationTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(enumeration->getTarget(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveFDEnumerationTargetRef_duplicate)
{
    fdRmgr.clear();
    auto enumeration = std::make_shared<BstIdl::FDEnumeration>();
    enumeration->setTarget(std::dynamic_pointer_cast<BstIdl::FEnumerationType>(
        mgr.findPublicAndPrivateFType("com.bst.idl.test.model1.TypeCollection1.Enum1")));

    fdRmgr.addFDEnumerationTargetRef(enumeration, "com.bst.idl.test.model1.Interface2.Enum1");
    auto errList = fdRmgr.resolveFDEnumerationTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(enumeration->getTarget(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveFDEnumerationTargetRef_multi)
{
    fdRmgr.clear();
    auto enumeration = std::make_shared<BstIdl::FDEnumeration>();
    enumeration->setTarget(std::dynamic_pointer_cast<BstIdl::FEnumerationType>(
        mgr.findPublicAndPrivateFType("com.bst.idl.test.model1.Interface3.Enum1")));

    fdRmgr.addFDEnumerationTargetRef(enumeration, "com.bst.idl.test.model1.Interface2.Enum1");
    fdRmgr.addFDEnumerationTargetRef(enumeration, "com.bst.idl.test.model1.TypeCollection1.Enum1");
    auto errList = fdRmgr.resolveFDEnumerationTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(enumeration->getTarget(),
              std::dynamic_pointer_cast<BstIdl::FEnumerationType>(
                  mgr.findPublicAndPrivateFType("com.bst.idl.test.model1.TypeCollection1.Enum1")));
}
/*
//resolveFDEnumValueTargetRef
TEST_F(FDReferenceManagerTestLT, resolveFDEnumValueTargetRef)
{
    fdRmgr.clear();
    auto enumValue = std::make_shared<BstIdl::FDEnumValue>();

    fdRmgr.addFDEnumValueTargetRef(enumValue, "com.bst.idl.test.model1.TypeCollection1.Enum1.Enumerator1");
    auto errList = fdRmgr.resolveFDEnumValueTargetRef();
    EXPECT_TRUE(errList.empty());
    EXPECT_EQ(enumValue->getTarget(), mgr.findFEnumerator("com.bst.idl.test.model1.TypeCollection1.Enum1.Enumerator1"));
}

TEST_F(FDReferenceManagerTestLT, resolveFDEnumValueTargetRef_duplicate)
{
    fdRmgr.clear();
    auto enumValue = std::make_shared<BstIdl::FDEnumValue>();
    enumValue->setTarget(mgr.findFEnumerator("com.bst.idl.test.model1.TypeCollection1.Enum1.Enumerator1"));

    fdRmgr.addFDEnumValueTargetRef(enumValue, "com.bst.idl.test.model1.TypeCollection1.Enum1.Enumerator2");
    auto errList = fdRmgr.resolveFDEnumValueTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(enumValue->getTarget(), nullptr);

}

TEST_F(FDReferenceManagerTestLT, resolveFDEnumValueTargetRef_multi)
{
    fdRmgr.clear();
    auto enumValue = std::make_shared<BstIdl::FDEnumValue>();
    enumValue->setTarget(mgr.findFEnumerator("com.bst.idl.test.model1.TypeCollection1.Enum1.Enumerator2"));

    fdRmgr.addFDEnumValueTargetRef(enumValue, "com.bst.idl.test.model1.Interface2.Enum1.Enumerator1");
    fdRmgr.addFDEnumValueTargetRef(enumValue, "com.bst.idl.test.model1.Interface2.Enum1.Enumerator2");
    auto errList = fdRmgr.resolveFDEnumValueTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(enumValue->getTarget(), mgr.findFEnumerator("com.bst.idl.test.model1.Interface2.Enum1.Enumerator2"));
}
*/

// resolveFDMapTargetRef
TEST_F(FDReferenceManagerTestLT, resolveFDMapTargetRef)
{
    fdRmgr.clear();
    auto map_ = std::make_shared<BstIdl::FDMap>();

    fdRmgr.addFDMapTargetRef(map_, "com.bst.idl.test.model1.TypeCollection1.Map1");
    auto errList = fdRmgr.resolveFDMapTargetRef();
    EXPECT_TRUE(errList.empty());
    EXPECT_EQ(map_->getTarget(), std::dynamic_pointer_cast<BstIdl::FMapType>(
                                     mgr.findPublicAndPrivateFType("com.bst.idl.test.model1.TypeCollection1.Map1")));
}

TEST_F(FDReferenceManagerTestLT, resolveFDMapTargetRef_undefined)
{
    fdRmgr.clear();
    auto map_ = std::make_shared<BstIdl::FDMap>();

    fdRmgr.addFDMapTargetRef(map_, "com.bst.idl.test.model1.Interface2.Map9");
    auto errList = fdRmgr.resolveFDMapTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(map_->getTarget(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveFDMapTargetRef_duplicate)
{
    fdRmgr.clear();
    auto map_ = std::make_shared<BstIdl::FDMap>();
    map_->setTarget(std::dynamic_pointer_cast<BstIdl::FMapType>(
        mgr.findPublicAndPrivateFType("com.bst.idl.test.model1.TypeCollection1.Map1")));

    fdRmgr.addFDMapTargetRef(map_, "com.bst.idl.test.model1.Interface2.Map1");
    auto errList = fdRmgr.resolveFDMapTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(map_->getTarget(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveFDMapTargetRef_multi)
{
    fdRmgr.clear();
    auto map_ = std::make_shared<BstIdl::FDMap>();
    map_->setTarget(std::dynamic_pointer_cast<BstIdl::FMapType>(
        mgr.findPublicAndPrivateFType("com.bst.idl.test.model1.Interface3.Map1")));

    fdRmgr.addFDMapTargetRef(map_, "com.bst.idl.test.model1.Interface2.Map1");
    fdRmgr.addFDMapTargetRef(map_, "com.bst.idl.test.model1.TypeCollection1.Map1");
    auto errList = fdRmgr.resolveFDMapTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(map_->getTarget(), std::dynamic_pointer_cast<BstIdl::FMapType>(
                                     mgr.findPublicAndPrivateFType("com.bst.idl.test.model1.TypeCollection1.Map1")));
}

// resolveFDInterfaceValueRef
TEST_F(FDReferenceManagerTestLT, resolveFDInterfaceValueRef)
{
    fdRmgr.clear();
    auto interfaceValueRef = std::make_shared<BstIdl::FDInterfaceRef>();

    fdRmgr.addFDInterfaceValueRef(interfaceValueRef, "com.bst.idl.test.model1.Interface1");
    auto errList = fdRmgr.resolveFDInterfaceValueRef();
    EXPECT_TRUE(errList.empty());
    EXPECT_EQ(interfaceValueRef->getTarget(), mgr.findFInterface("com.bst.idl.test.model1.Interface1"));
}

TEST_F(FDReferenceManagerTestLT, resolveFDInterfaceValueRef_undefined)
{
    fdRmgr.clear();
    auto interfaceValueRef = std::make_shared<BstIdl::FDInterfaceRef>();

    fdRmgr.addFDInterfaceValueRef(interfaceValueRef, "com.bst.idl.test.model1.Interface9");
    auto errList = fdRmgr.resolveFDInterfaceValueRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(interfaceValueRef->getTarget(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveFDInterfaceValueRef_duplicate)
{
    fdRmgr.clear();
    auto interfaceValueRef = std::make_shared<BstIdl::FDInterfaceRef>();
    interfaceValueRef->setTarget(mgr.findFInterface("com.bst.idl.test.model1.Interface3"));

    fdRmgr.addFDInterfaceValueRef(interfaceValueRef, "com.bst.idl.test.model1.Interface2");
    auto errList = fdRmgr.resolveFDInterfaceValueRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(interfaceValueRef->getTarget(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveFDInterfaceValueRef_multi)
{
    fdRmgr.clear();
    auto interfaceValueRef = std::make_shared<BstIdl::FDInterfaceRef>();
    interfaceValueRef->setTarget(mgr.findFInterface("com.bst.idl.test.model1.Interface3"));

    fdRmgr.addFDInterfaceValueRef(interfaceValueRef, "com.bst.idl.test.model1.Interface2");
    fdRmgr.addFDInterfaceValueRef(interfaceValueRef, "com.bst.idl.test.model1.Interface1");
    auto errList = fdRmgr.resolveFDInterfaceValueRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(interfaceValueRef->getTarget(), mgr.findFInterface("com.bst.idl.test.model1.Interface1"));
}

/*
//resolveBroadcastTargetRef
TEST_F(FDReferenceManagerTestLT, resolveBroadcastTargetRef)
{
    fdRmgr.clear();
    auto broadcast = std::make_shared<BstIdl::FDBroadcast>();

    fdRmgr.addBroadcastTargetRef(broadcast, "com.bst.idl.test.model1.Interface1.Broadcast1");
    auto errList = fdRmgr.resolveBroadcastTargetRef();
    EXPECT_TRUE(errList.empty());
    EXPECT_EQ(broadcast->getTarget(), mgr.findFBroadcast("com.bst.idl.test.model1.Interface1.Broadcast1"));
}

TEST_F(FDReferenceManagerTestLT, resolveBroadcastTargetRef_undefined)
{
    fdRmgr.clear();
    auto broadcast = std::make_shared<BstIdl::FDBroadcast>();

    fdRmgr.addBroadcastTargetRef(broadcast, "com.bst.idl.test.model1.Interface1.Broadcast9");
    auto errList = fdRmgr.resolveBroadcastTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(broadcast->getTarget(), nullptr);
}

TEST_F(FDReferenceManagerTestLT, resolveBroadcastTargetRef_duplicate)
{
    fdRmgr.clear();
    auto broadcast = std::make_shared<BstIdl::FDBroadcast>();
    broadcast->setTarget(mgr.findFBroadcast("com.bst.idl.test.model1.Interface3.Broadcast1"));

    fdRmgr.addBroadcastTargetRef(broadcast, "com.bst.idl.test.model1.Interface2.Broadcast1");
    auto errList = fdRmgr.resolveBroadcastTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(broadcast->getTarget(), nullptr);

}

TEST_F(FDReferenceManagerTestLT, resolveBroadcastTargetRef_multi)
{
    fdRmgr.clear();
    auto broadcast = std::make_shared<BstIdl::FDBroadcast>();
    broadcast->setTarget(mgr.findFBroadcast("com.bst.idl.test.model1.Interface3.Broadcast1"));

    fdRmgr.addBroadcastTargetRef(broadcast, "com.bst.idl.test.model1.Interface2.Broadcast1");
    fdRmgr.addBroadcastTargetRef(broadcast, "com.bst.idl.test.model1.Interface1.Broadcast1");
    auto errList = fdRmgr.resolveBroadcastTargetRef();
    EXPECT_FALSE(errList.empty());
    EXPECT_EQ(broadcast->getTarget(), mgr.findFBroadcast("com.bst.idl.test.model1.Interface1.Broadcast1"));
}
*/