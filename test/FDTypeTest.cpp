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
#include "FDModel/FDType.h"
#include "FDModel/FDArgument.h"
#include "FDModel/FDArgumentList.h"
#include "FDModel/FDArray.h"
#include "FDModel/FDAttribute.h"
#include "FDModel/FDBoolean.h"
#include "FDModel/FDBroadcast.h"
#include "FDModel/FDBuiltInPropertyHost.h"
#include "FDModel/FDComplexValue.h"
#include "FDModel/FDDeclaration.h"
#include "FDModel/FDEnumType.h"
#include "FDModel/FDEnumValue.h"
#include "FDModel/FDEnumeration.h"
#include "FDModel/FDEnumerationOverwrites.h"
#include "FDModel/FDEnumerator.h"
#include "FDModel/FDExtensionElement.h"
#include "FDModel/FDExtensionRoot.h"
#include "FDModel/FDField.h"
#include "FDModel/FDGeneric.h"
#include "FDModel/FDImport.h"
#include "FDModel/FDInteger.h"
#include "FDModel/FDInterface.h"
#include "FDModel/FDInterfaceRef.h"
#include "FDModel/FDMap.h"
#include "FDModel/FDMapKey.h"
#include "FDModel/FDMapValue.h"
#include "FDModel/FDMapper.h"
#include "FDModel/FDMethod.h"
#include "FDModel/FDModel.h"
#include "FDModel/FDObject.h"
#include "FDModel/FDPlainTypeOverwrites.h"
#include "FDModel/FDPredefinedTypeId.h"
#include "FDModel/FDProperty.h"
#include "FDModel/FDPropertyDecl.h"
#include "FDModel/FDPropertyFlag.h"
#include "FDModel/FDPropertyHost.h"
#include "FDModel/FDRootElement.h"
#include "FDModel/FDSpecification.h"
#include "FDModel/FDString.h"
#include "FDModel/FDStruct.h"
#include "FDModel/FDStructOverwrites.h"
#include "FDModel/FDTypeDefinition.h"
#include "FDModel/FDTypeRef.h"
#include "FDModel/FDTypedef.h"
#include "FDModel/FDTypes.h"
#include "FDModel/FDUnion.h"
#include "FDModel/FDUnionOverwrites.h"
#include "FDModel/FDValue.h"
#include "FDModel/FDValueArray.h"
#include <gtest/gtest.h>

TEST(FDArgumentTest, D0EV)
{
    auto *arg = new BstIdl::FDArgument();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDArgumentListTest, D0EV)
{
    auto *arg = new BstIdl::FDArgumentList();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDArrayTest, D0EV)
{
    auto *arg = new BstIdl::FDArray();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDAttributeTest, D0EV)
{
    auto *arg = new BstIdl::FDAttribute();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDBooleanTest, D0EV)
{
    auto *arg = new BstIdl::FDBoolean();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDBroadcastTest, D0EV)
{
    auto *arg = new BstIdl::FDBroadcast();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDBuiltInPropertyHostTest, D0EV)
{
    auto *arg = new BstIdl::FDBuiltInPropertyHost();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDComplexValueTest, D0EV)
{
    auto *arg = new BstIdl::FDComplexValue();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDDeclarationTest, D0EV)
{
    auto *arg = new BstIdl::FDDeclaration();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDEnumTypeTest, D0EV)
{
    auto *arg = new BstIdl::FDEnumType();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDEnumValueTest, D0EV)
{
    auto *arg = new BstIdl::FDEnumValue();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDEnumerationTest, D0EV)
{
    auto *arg = new BstIdl::FDEnumeration();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDEnumerationOverwritesTest, D0EV)
{
    auto *arg = new BstIdl::FDEnumerationOverwrites();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDEnumeratorTest, D0EV)
{
    auto *arg = new BstIdl::FDEnumerator();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDExtensionElementTest, D0EV)
{
    auto *arg = new BstIdl::FDExtensionElement();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDExtensionRootTest, D0EV)
{
    auto *arg = new BstIdl::FDExtensionRoot();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDFieldTest, D0EV)
{
    auto *arg = new BstIdl::FDField();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDGenericTest, D0EV)
{
    auto *arg = new BstIdl::FDGeneric();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDImportTest, D0EV)
{
    auto *arg = new BstIdl::FDImport();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDIntegerTest, D0EV)
{
    auto *arg = new BstIdl::FDInteger();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDInterfaceTest, D0EV)
{
    auto *arg = new BstIdl::FDInterface();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDInterfaceRefTest, D0EV)
{
    auto *arg = new BstIdl::FDInterfaceRef();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDMapTest, D0EV)
{
    auto *arg = new BstIdl::FDMap();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDMapKeyTest, D0EV)
{
    auto *arg = new BstIdl::FDMapKey();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDMapValueTest, D0EV)
{
    auto *arg = new BstIdl::FDMapValue();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDMapperTest, D0EV)
{
    auto *arg = new BstIdl::FDMapper();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDMethodTest, D0EV)
{
    auto *arg = new BstIdl::FDMethod();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDModelTest, D0EV)
{
    auto *arg = new BstIdl::FDModel();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDObjectTest, D0EV)
{
    auto *arg = new BstIdl::FDObject();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDPlainTypeOverwritesTest, D0EV)
{
    auto *arg = new BstIdl::FDPlainTypeOverwrites();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDPredefinedTypeIdTest, D0EV)
{
    auto *arg = new BstIdl::FDPredefinedTypeId();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDPropertyTest, D0EV)
{
    auto *arg = new BstIdl::FDProperty();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDPropertyDeclTest, D0EV)
{
    auto *arg = new BstIdl::FDPropertyDecl();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDPropertyFlagTest, D0EV)
{
    auto *arg = new BstIdl::FDPropertyFlag();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}
/*
TEST(FDPropertyHostTest, D0EV)
{
    auto * arg = new BstIdl::FDPropertyHost();
    EXPECT_TRUE(arg != nullptr);
    delete(arg);
}
*/

TEST(FDRootElementTest, D0EV)
{
    auto *arg = new BstIdl::FDRootElement();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDSpecificationTest, D0EV)
{
    auto *arg = new BstIdl::FDSpecification();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDStringTest, D0EV)
{
    auto *arg = new BstIdl::FDString();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDStructTest, D0EV)
{
    auto *arg = new BstIdl::FDStruct();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDStructOverwritesTest, D0EV)
{
    auto *arg = new BstIdl::FDStructOverwrites();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDTypeTest, D0EV)
{
    auto *arg = new BstIdl::FDType();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDTypeTest, getName)
{
    auto *arg = new BstIdl::FDType();
    EXPECT_TRUE(arg != nullptr);
    EXPECT_STREQ(arg->getName().c_str(), "FDType");
    delete (arg);
}

TEST(FDTypedefTest, D0EV)
{
    auto *arg = new BstIdl::FDTypedef();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDTypeDefinitionTest, D0EV)
{
    auto *arg = new BstIdl::FDTypeDefinition();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDTypeRefTest, D0EV)
{
    auto *arg = new BstIdl::FDTypeRef();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDTypesTest, D0EV)
{
    auto *arg = new BstIdl::FDTypes();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDUnionTest, D0EV)
{
    auto *arg = new BstIdl::FDUnion();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDUnionOverwritesTest, D0EV)
{
    auto *arg = new BstIdl::FDUnionOverwrites();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDValueTest, D0EV)
{
    auto *arg = new BstIdl::FDValue();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}

TEST(FDValueTest, virtualFunction)
{
    auto *arg = new BstIdl::FDValue();
    EXPECT_TRUE(arg != nullptr);
    EXPECT_EQ(arg->isBoolean(), false);
    EXPECT_EQ(arg->isEnumerator(), false);
    EXPECT_EQ(arg->isInteger(), false);
    EXPECT_EQ(arg->isInterfaceRef(), false);
    EXPECT_EQ(arg->isString(), false);

    EXPECT_EQ(arg->getBoolean(), false);
    EXPECT_EQ(arg->getInteger(), 0);
    EXPECT_EQ(arg->getInterfaceRef(), nullptr);
    EXPECT_EQ(arg->getString(), std::string());
    EXPECT_EQ(arg->getEnumerator(), nullptr);

    delete (arg);
}

TEST(FDValueArrayTest, D0EV)
{
    auto *arg = new BstIdl::FDValueArray();
    EXPECT_TRUE(arg != nullptr);
    delete (arg);
}