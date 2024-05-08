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
#include "model/FBasicTypeId.h"
#include "model/FEnumerationType.h"
#include "model/FEnumerator.h"
#include "model/FExpression.h"
#include "model/FFactory.h"
#include "model/FInitializerExpression.h"
#include "model/FModelManager.h"
#include "model/FOperation.h"
#include "model/FParser.h"
#include "parser/FidlParser.h"
#include "validator/ReferenceManager.h"
#include <gtest/gtest.h>

TEST(InitializerErrorTest, error_copy)
{
    auto err = BstIdl::FInitializerExpression::initializer_error("InitilizerErrorTest: Initilizer error string");
    auto errCopy(err);
    try
    {
        throw errCopy;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        EXPECT_STREQ(e.what(), "InitilizerErrorTest: Initilizer error string");
    }
}

TEST(BasicTypeIdTest, get_literal)
{
    auto type = BstIdl::FBasicTypeId::get("Float");
    EXPECT_STREQ(type->getLiteral().c_str(), "Float");
    type = BstIdl::FBasicTypeId::get("324234545634");
    EXPECT_STREQ(type->getLiteral().c_str(), "undefined");
}

TEST(BasicTypeIdTest, getByName)
{
    auto type = BstIdl::FBasicTypeId::getByName("Float");
    EXPECT_STREQ(type->getLiteral().c_str(), "Float");
    type = BstIdl::FBasicTypeId::getByName("wfweaf");
    EXPECT_STREQ(type->getLiteral().c_str(), "undefined");
}

TEST(BasicTypeIdTest, get)
{
    auto type = BstIdl::FBasicTypeId::get(20U);
    EXPECT_STREQ(type->getLiteral().c_str(), "undefined");
}

TEST(zAnnotationTest, D0EV)
{
    BstIdl::FAnnotation *arg = new BstIdl::FAnnotation("");
    delete (arg);
}

TEST(FAnnotationBlockTest, D0EV)
{
    BstIdl::FAnnotationBlock *arg = new BstIdl::FAnnotationBlock();
    delete (arg);
}

TEST(FAnnotationTypeTest, D0EV)
{
    BstIdl::FAnnotationType *arg = new BstIdl::FAnnotationType();
    delete (arg);
}

TEST(AugumentTest, D0EV)
{
    BstIdl::FArgument *arg = new BstIdl::FArgument();
    delete (arg);
}

TEST(FArrayTypeTest, D0EV)
{
    BstIdl::FArrayType *arg = new BstIdl::FArrayType();
    delete (arg);
}

TEST(FAttributeTest, D0EV)
{
    BstIdl::FAttribute *arg = new BstIdl::FAttribute();
    delete (arg);
}

TEST(FBasicTypeIdTest, D0EV)
{
    BstIdl::FBasicTypeId *arg = new BstIdl::FBasicTypeId();
    delete (arg);
}

TEST(FBinaryOperationTest, D0EV)
{
    BstIdl::FBinaryOperation *arg = new BstIdl::FBinaryOperation();
    delete (arg);
}

TEST(FBooleanConstantTest, D0EV)
{
    BstIdl::FBooleanConstant *arg = new BstIdl::FBooleanConstant();
    delete (arg);
}

TEST(FBracketInitializerTest, D0EV)
{
    BstIdl::FBracketInitializer *arg = new BstIdl::FBracketInitializer();
    delete (arg);
}

TEST(FBroadcastTest, D0EV)
{
    BstIdl::FBroadcast *arg = new BstIdl::FBroadcast();
    delete (arg);
}

TEST(FCompoundInitializerTest, D0EV)
{
    BstIdl::FCompoundInitializer *arg = new BstIdl::FCompoundInitializer();
    delete (arg);
}

TEST(FCompoundTypeTest, D0EV)
{
    BstIdl::FCompoundType *arg = new BstIdl::FCompoundType();
    delete (arg);
}

TEST(FConstantTest, D0EV)
{
    BstIdl::FConstant *arg = new BstIdl::FConstant();
    delete (arg);
}

TEST(FConstantDefTest, D0EV)
{
    BstIdl::FConstantDef *arg = new BstIdl::FConstantDef();
    delete (arg);
}

TEST(FDoubleConstantTest, D0EV)
{
    BstIdl::FDoubleConstant *arg = new BstIdl::FDoubleConstant();
    delete (arg);
}

TEST(FElementInitializerTest, D0EV)
{
    BstIdl::FElementInitializer *arg = new BstIdl::FElementInitializer();
    delete (arg);
}
TEST(FEnumerationTypeTest, D0EV)
{
    BstIdl::FEnumerationType *arg = new BstIdl::FEnumerationType();
    delete (arg);
}

TEST(FEnumeratorTest, D0EV)
{
    BstIdl::FEnumerator *arg = new BstIdl::FEnumerator();
    delete (arg);
}

TEST(FEvaluableElementTest, D0EV)
{
    BstIdl::FEvaluableElement *arg = new BstIdl::FEvaluableElement();
    delete (arg);
}

TEST(FExpressionTest, D0EV)
{
    BstIdl::FExpression *arg = new BstIdl::FExpression();
    delete (arg);
}

TEST(FFieldTest, D0EV)
{
    BstIdl::FField *arg = new BstIdl::FField();
    delete (arg);
}

TEST(FFieldInitializerTest, D0EV)
{
    BstIdl::FFieldInitializer *arg = new BstIdl::FFieldInitializer();
    delete (arg);
}

TEST(FFloatConstantTest, D0EV)
{
    BstIdl::FFloatConstant *arg = new BstIdl::FFloatConstant();
    delete (arg);
}

TEST(FInitializerTest, D0EV)
{
    BstIdl::FInitializer *arg = new BstIdl::FInitializer();
    delete (arg);
}

TEST(FInitializerExpressionTest, D0EV)
{
    BstIdl::FInitializerExpression *arg = new BstIdl::FInitializerExpression();
    delete (arg);
}

TEST(FIntegerConstantTest, D0EV)
{
    BstIdl::FIntegerConstant *arg = new BstIdl::FIntegerConstant();
    delete (arg);
}

TEST(FIntegerIntervalTest, D0EV)
{
    BstIdl::FIntegerInterval *arg = new BstIdl::FIntegerInterval();
    delete (arg);
}

TEST(FInterfaceTest, D0EV)
{
    BstIdl::FInterface *arg = new BstIdl::FInterface();
    delete (arg);
}

TEST(FMapTypeTest, D0EV)
{
    BstIdl::FMapType *arg = new BstIdl::FMapType();
    delete (arg);
}

TEST(FMethodTest, D0EV)
{
    BstIdl::FMethod *arg = new BstIdl::FMethod();
    delete (arg);
}

TEST(FModelTest, D0EV)
{
    BstIdl::FModel *arg = new BstIdl::FModel();
    delete (arg);
}

TEST(FModelElementTest, D0EV)
{
    BstIdl::FModelElement *arg = new BstIdl::FModelElement();
    delete (arg);
}

TEST(FObjectTest, D0EV)
{
    BstIdl::FObject *arg = new BstIdl::FObject();
    delete (arg);
}

TEST(FObjectTest, findElement)
{
    BstIdl::FObject *arg = new BstIdl::FObject();
    std::string func;
    EXPECT_EQ(arg->transform(func), arg->getName());
    delete (arg);
}

TEST(FOperationTest, D0EV)
{
    BstIdl::FOperation *arg = new BstIdl::FOperation();
    arg->toString();
    delete (arg);
}

TEST(FOperatorTest, D0EV)
{
    BstIdl::FOperator *arg = new BstIdl::FOperator();
    delete (arg);
}

TEST(FParserTest, D0EV)
{
    BstIdl::FParser *arg = new BstIdl::FParser();
    delete (arg);
}

TEST(FQualifiedElementRefTest, D0EV)
{
    BstIdl::FQualifiedElementRef *arg = new BstIdl::FQualifiedElementRef();
    delete (arg);
}

TEST(FStringConstantTest, D0EV)
{
    BstIdl::FStringConstant *arg = new BstIdl::FStringConstant();
    delete (arg);
}

TEST(FStructTypeTest, D0EV)
{
    BstIdl::FStructType *arg = new BstIdl::FStructType();
    delete (arg);
}

TEST(FTypeTest, D0EV)
{
    BstIdl::FType *arg = new BstIdl::FType();
    delete (arg);
}

TEST(FTypeTest, virtualfindElement)
{
    BstIdl::FType *arg = new BstIdl::FType();
    std::list<std::string> fqnList;
    EXPECT_EQ(arg->findElement(fqnList), nullptr);
    delete (arg);
}

TEST(FTypeCollectionTest, D0EV)
{
    BstIdl::FTypeCollection *arg = new BstIdl::FTypeCollection();
    delete (arg);
}

TEST(FTypeDefTest, D0EV)
{
    BstIdl::FTypeDef *arg = new BstIdl::FTypeDef();
    delete (arg);
}

TEST(FTypeRefTest, D0EV)
{
    BstIdl::FTypeRef *arg = new BstIdl::FTypeRef();
    delete (arg);
}

TEST(FTypedElementTest, D0EV)
{
    BstIdl::FTypedElement *arg = new BstIdl::FTypedElement();
    delete (arg);
}

TEST(FUnaryOperationTest, D0EV)
{
    BstIdl::FUnaryOperation *arg = new BstIdl::FUnaryOperation();
    delete (arg);
}

TEST(FUnionTypeTest, D0EV)
{
    BstIdl::FUnionType *arg = new BstIdl::FUnionType();
    delete (arg);
}

TEST(FVersionTest, D0EV)
{
    BstIdl::FVersion *arg = new BstIdl::FVersion();
    delete (arg);
}
