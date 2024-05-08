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
#include "model/FInitializerExpression.h"
#include "model/FModelManager.h"
#include "model/FOperation.h"
#include "model/FTypeDef.h"
#include "parser/FidlParser.h"
#include <climits>
#include <gtest/gtest.h>

class InitializerExpressionTest : public ::testing::Test
{

protected:
    void SetUp() override
    {
        mgr.clear();
        model = mgr.addModel("com.bst.idl.testExpression.model1");
        // add TypeCollection1
        auto tc1 = factory.createFTypeCollection();
        tc1->setName("TypeCollection1");
        tc1->setContainer(model);
        // add tc to model1
        addListItem(model->getTypeCollections(), std::move(tc1));

        auto interface1 = factory.createFInterface();
        interface1->setName("Interface1");
        interface1->setContainer(model);
        addListItem(model->getInterfaces(), std::move(interface1));
    }

    BstIdl::FModelManager &mgr = BstIdl::FModelManager::getInstance();
    BstIdl::FFactory &factory = BstIdl::FFactory::getInstance();
    std::shared_ptr<BstIdl::FModel> model;
};

// TODO: constant def ->name ->type ->m_rhs[即各种constant]

// basic type
TEST_F(InitializerExpressionTest, Expression_ToString)
{
    auto exp = BstIdl::FExpression();
    EXPECT_EQ(exp.toString(), std::string(""));
}

TEST_F(InitializerExpressionTest, Constant_ToString)
{
    auto cst = BstIdl::FConstant();
    EXPECT_EQ(cst.toString(), std::string(""));
}

TEST_F(InitializerExpressionTest, DoubleConstant_construct)
{
    auto double_cst = BstIdl::FDoubleConstant(10.12);
    double a = 10.12;
    EXPECT_EQ(double_cst.getVal(), a);
}

TEST_F(InitializerExpressionTest, DoubleConstant_copyConstruct)
{
    auto double_cst = BstIdl::FDoubleConstant(10.12);
    auto double_cst1 = BstIdl::FDoubleConstant(double_cst);
    EXPECT_EQ(double_cst1.toString(), "10.120000");
}

TEST_F(InitializerExpressionTest, DoubleConstant)
{
    auto tc1 = model->getTypeCollections().front();
    auto cst = factory.createFConstantDef();
    cst->setName("Constant");
    cst->setContainer(tc1);
    auto type = BstIdl::FBasicTypeId::get(12U);
    auto typeref = factory.createFTypeRef();
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto double_cst = factory.createFDoubleConstant();
    double_cst->setVal(10.12);
    double a = 10.12;
    EXPECT_EQ(double_cst->getVal(), a);

    std::string value;
    bool valiEnable = true;
    try
    {
        double_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
        double_cst->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_TRUE(valiEnable);
    cst->setRhs(double_cst);
    EXPECT_EQ(cst->getRhs(), double_cst);
}

TEST_F(InitializerExpressionTest, DoubleConstant_notMatchType_predefined)
{
    auto tc1 = model->getTypeCollections().front();
    auto cst = factory.createFConstantDef();
    cst->setName("Constant");
    cst->setContainer(tc1);
    auto type = BstIdl::FBasicTypeId::get(11U);
    auto typeref = factory.createFTypeRef();
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto double_cst = factory.createFDoubleConstant();
    double_cst->setVal(10.12);
    double a = 10.12;
    EXPECT_EQ(double_cst->getVal(), a);
    std::string value;
    bool valiEnable = true;
    try
    {
        double_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
        double_cst->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
    cst->setRhs(double_cst);
    EXPECT_EQ(cst->getRhs(), double_cst);
}

TEST_F(InitializerExpressionTest, DoubleConstant_notMatchType_notPredefined)
{
    auto tc1 = model->getTypeCollections().front();
    auto cst = factory.createFConstantDef();
    cst->setName("Constant");
    cst->setContainer(tc1);
    auto type = factory.createFArrayType();
    auto typeref = factory.createFTypeRef();
    typeref->setDerived(type);
    EXPECT_EQ(typeref->getPredefined(), nullptr);
    cst->setType(typeref);
    cst->setArray(false);

    auto double_cst = factory.createFDoubleConstant();
    double_cst->setVal(10.12);
    double a = 10.12;
    EXPECT_EQ(double_cst->getVal(), a);
    std::string value;
    bool valiEnable = true;
    try
    {
        double_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
        double_cst->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
    cst->setRhs(double_cst);
    EXPECT_EQ(cst->getRhs(), double_cst);
}

TEST_F(InitializerExpressionTest, DoubleConstant_notMatchType_isArray)
{
    auto tc1 = model->getTypeCollections().front();
    auto cst = factory.createFConstantDef();
    cst->setName("Constant");
    cst->setContainer(tc1);
    auto type = factory.createFArrayType();
    auto typeref = factory.createFTypeRef();
    typeref->setDerived(type);
    EXPECT_EQ(typeref->getPredefined(), nullptr);
    cst->setType(typeref);
    cst->setArray(true);

    auto double_cst = factory.createFDoubleConstant();
    double_cst->setVal(10.12);
    double a = 10.12;
    EXPECT_EQ(double_cst->getVal(), a);
    std::string value;
    bool valiEnable = true;
    try
    {
        double_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
        double_cst->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
}

TEST_F(InitializerExpressionTest, DoubleConstant_nullTypeToValidate)
{
    auto tc1 = model->getTypeCollections().front();
    auto cst = factory.createFConstantDef();
    cst->setName("Constant");
    cst->setContainer(tc1);
    auto type = factory.createFArrayType();
    auto typeref = factory.createFTypeRef();
    typeref = nullptr;
    cst->setType(typeref);
    cst->setArray(true);

    auto double_cst = factory.createFDoubleConstant();
    double_cst->setVal(10.12);
    double a = 10.12;
    EXPECT_EQ(double_cst->getVal(), a);
    std::string value;
    bool valiEnable = true;
    try
    {
        double_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
        double_cst->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
}
/*
TEST_F(InitializerExpressionTest, DoubleConstant_expressionOverflowed)
{
    auto tc1 = model->getTypeCollections().front();
    auto cst = factory.createFConstantDef();
    cst->setName("Constant");
    cst->setContainer(tc1);
    auto type = factory.createFArrayType();
    auto typeref = factory.createFTypeRef();
    typeref->setDerived(type);
    EXPECT_EQ(typeref->getPredefined(), nullptr);
    cst->setType(typeref);
    cst->setArray(false);

    auto double_cst = factory.createFDoubleConstant();
    double_cst->setVal(DBL_MAX + 1);
    std::string value;
    bool valiEnable = true;
    try
    {
        double_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);

}
*/
TEST_F(InitializerExpressionTest, DoubleConstant_toString)
{
    auto double_cst = BstIdl::FDoubleConstant(10.12);
    EXPECT_EQ(double_cst.toString(), "10.120000");
}

// float

TEST_F(InitializerExpressionTest, FloatConstant_construct)
{
    auto float_cst = BstIdl::FFloatConstant(10.12f);
    float a = 10.12f;
    EXPECT_EQ(float_cst.getVal(), a);
}

TEST_F(InitializerExpressionTest, FloatConstant_copyConstruct)
{
    auto float_cst = BstIdl::FFloatConstant(10.12f);
    auto float_cst1 = BstIdl::FFloatConstant(float_cst);
    EXPECT_EQ(float_cst.getVal(), float_cst1.getVal());
}

TEST_F(InitializerExpressionTest, FloatConstant)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(11U);
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto float_cst = factory.createFFloatConstant();
    float_cst->setVal(10.12f);
    cst->setRhs(float_cst);
    bool valiEnable = true;
    std::string value;
    try
    {
        float_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
        float_cst->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_TRUE(valiEnable) << float_cst->getVal();
    EXPECT_EQ(float_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, FloatConstant_noMatchType_predefined)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(9U);
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto float_cst = factory.createFFloatConstant();
    float_cst->setVal(10.12f);
    cst->setRhs(float_cst);
    bool valiEnable = true;
    std::string value;
    try
    {
        float_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
        float_cst->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
    EXPECT_EQ(float_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, FloatConstant_noMatchType_notPredefined)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type = factory.createFArrayType();
    typeref->setDerived(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto float_cst = factory.createFFloatConstant();
    float_cst->setVal(10.12f);
    cst->setRhs(float_cst);
    bool valiEnable = true;
    std::string value;
    try
    {
        float_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
        float_cst->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
    EXPECT_EQ(float_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, FloatConstant_noMatchType_isArray)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(11U);
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(true);

    auto float_cst = factory.createFFloatConstant();
    float_cst->setVal(10.12f);
    cst->setRhs(float_cst);
    bool valiEnable = true;
    std::string value;
    try
    {
        float_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
        float_cst->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
    EXPECT_EQ(float_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, FloatConstant_nullTypeToValidate)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    cst->setType(typeref);
    typeref = nullptr;
    cst->setArray(false);

    auto float_cst = factory.createFFloatConstant();
    float_cst->setVal(10.12f);
    cst->setRhs(float_cst);
    bool valiEnable = true;
    std::string value;
    try
    {
        float_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
        float_cst->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
    EXPECT_EQ(float_cst, cst->getRhs());
}
/*
TEST_F(InitializerExpressionTest, FloatConstant_expressionOverflowed)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(11U);
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto float_cst = factory.createFFloatConstant();
    float_cst->setVal(FLT_MAX + 100);
    cst->setRhs(float_cst);
    bool valiEnable = true;
    std::string value;
    try
    {
        float_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
        //float_cst->validate(typeref, cst->isArray());
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
    EXPECT_EQ(float_cst, cst->getRhs());
}
*/

TEST_F(InitializerExpressionTest, FloatConstant_toString)
{
    auto float_cst = BstIdl::FFloatConstant(10.12f);
    EXPECT_EQ(float_cst.toString(), "10.120000");
}

// string
TEST_F(InitializerExpressionTest, StringConstant_construct)
{
    auto string_cst = BstIdl::FStringConstant();
    string_cst.setVal("lt");
    EXPECT_STREQ(string_cst.getVal().c_str(), "lt");
}

TEST_F(InitializerExpressionTest, StringConstant_copyConstruct)
{
    auto string_cst = BstIdl::FStringConstant();
    string_cst.setVal("lt");
    auto string_cst1 = BstIdl::FStringConstant(string_cst);
    EXPECT_EQ(string_cst.getVal(), string_cst1.getVal());
}

TEST_F(InitializerExpressionTest, StringConstant)
{
    auto cst = factory.createFConstantDef();
    auto type = BstIdl::FBasicTypeId::get(10U);
    auto typeref = factory.createFTypeRef();
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto string_cst = factory.createFStringConstant();
    string_cst->setVal("lt");
    cst->setRhs(string_cst);
    bool valiEnable = true;
    try
    {
        string_cst->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_TRUE(valiEnable);
    EXPECT_EQ(string_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, StringConstant_notMatchType_predefined)
{
    auto cst = factory.createFConstantDef();
    auto type = BstIdl::FBasicTypeId::get(13U);
    auto typeref = factory.createFTypeRef();
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto string_cst = factory.createFStringConstant();
    string_cst->setVal("lt");
    cst->setRhs(string_cst);
    bool valiEnable = true;
    try
    {
        string_cst->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
    EXPECT_EQ(string_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, StringConstant_notMatchType_notpredefined)
{
    auto cst = factory.createFConstantDef();
    auto type = factory.createFArrayType();
    auto typeref = factory.createFTypeRef();
    typeref->setDerived(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto string_cst = factory.createFStringConstant();
    string_cst->setVal("lt");
    cst->setRhs(string_cst);
    bool valiEnable = true;
    try
    {
        string_cst->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
    EXPECT_EQ(string_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, StringConstant_notMatchType_isArray)
{
    auto cst = factory.createFConstantDef();
    auto type = BstIdl::FBasicTypeId::get(10U);
    auto typeref = factory.createFTypeRef();
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(true);

    auto string_cst = factory.createFStringConstant();
    string_cst->setVal("lt");
    cst->setRhs(string_cst);
    bool valiEnable = true;
    try
    {
        string_cst->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
    EXPECT_EQ(string_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, StringConstant_nullTypeToValidate)
{
    auto cst = factory.createFConstantDef();
    auto type = BstIdl::FBasicTypeId::get(10U);
    auto typeref = factory.createFTypeRef();
    typeref = nullptr;
    cst->setType(typeref);
    cst->setArray(false);

    auto string_cst = factory.createFStringConstant();
    string_cst->setVal("lt");
    cst->setRhs(string_cst);
    bool valiEnable = true;
    try
    {
        string_cst->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
    EXPECT_EQ(string_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, StringConstant_toString)
{
    auto string_cst = factory.createFStringConstant();
    string_cst->setVal("lt");
    EXPECT_EQ(string_cst->toString(), "\"" + string_cst->getVal() + "\"");
}

// boolean
TEST_F(InitializerExpressionTest, BooleanConstant_defaultConstruct)
{
    auto boolean_cst = BstIdl::FBooleanConstant();
    boolean_cst.setVal(true);
    EXPECT_TRUE(boolean_cst.isVal());
    boolean_cst.setVal(false);
    EXPECT_FALSE(boolean_cst.isVal());
}

TEST_F(InitializerExpressionTest, BooleanConstant_construct)
{
    auto boolean_cst = BstIdl::FBooleanConstant(true);
    EXPECT_TRUE(boolean_cst.isVal());
}

TEST_F(InitializerExpressionTest, BooleanConstant_cpyConstruct)
{
    auto boolean_cst = BstIdl::FBooleanConstant(true);
    auto boolean_cst1 = BstIdl::FBooleanConstant(boolean_cst);
    EXPECT_EQ(boolean_cst.isVal(), boolean_cst1.isVal());
}

TEST_F(InitializerExpressionTest, BooleanConstant)
{
    auto cst = factory.createFConstantDef();
    auto type = BstIdl::FBasicTypeId::get(9U);
    auto typeref = factory.createFTypeRef();
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto boolean_cst = factory.createFBooleanConstant();
    boolean_cst->setVal(false);
    cst->setRhs(boolean_cst);
    bool valiEnable = true;
    try
    {
        boolean_cst->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_TRUE(valiEnable);
    EXPECT_EQ(boolean_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, BooleanConstant_notMatchType_predefined)
{
    auto cst = factory.createFConstantDef();
    auto type = BstIdl::FBasicTypeId::get(11U);
    auto typeref = factory.createFTypeRef();
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto boolean_cst = factory.createFBooleanConstant();
    boolean_cst->setVal(false);
    cst->setRhs(boolean_cst);
    bool valiEnable = true;
    try
    {
        boolean_cst->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
    EXPECT_EQ(boolean_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, BooleanConstant_notMatchType_notPredefined)
{
    auto cst = factory.createFConstantDef();
    auto type = factory.createFArrayType();
    auto typeref = factory.createFTypeRef();
    typeref->setDerived(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto boolean_cst = factory.createFBooleanConstant();
    boolean_cst->setVal(false);
    cst->setRhs(boolean_cst);
    bool valiEnable = true;
    try
    {
        boolean_cst->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
    EXPECT_EQ(boolean_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, BooleanConstant_notMatchType_isArray)
{
    auto cst = factory.createFConstantDef();
    auto type = BstIdl::FBasicTypeId::get(9U);
    auto typeref = factory.createFTypeRef();
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(true);

    auto boolean_cst = factory.createFBooleanConstant();
    boolean_cst->setVal(false);
    cst->setRhs(boolean_cst);
    bool valiEnable = true;
    try
    {
        boolean_cst->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
    EXPECT_EQ(boolean_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, BooleanConstant_nullTypeToValidate)
{
    auto cst = factory.createFConstantDef();
    auto type = BstIdl::FBasicTypeId::get(9U);
    auto typeref = factory.createFTypeRef();
    typeref = nullptr;
    cst->setType(typeref);
    cst->setArray(false);

    auto boolean_cst = factory.createFBooleanConstant();
    boolean_cst->setVal(false);
    cst->setRhs(boolean_cst);
    bool valiEnable = true;
    try
    {
        boolean_cst->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
    EXPECT_EQ(boolean_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, BooleanConstant_toString)
{
    auto boolean_cst = factory.createFBooleanConstant();
    boolean_cst->setVal(false);
    EXPECT_EQ(std::string("false"), boolean_cst->toString());
    boolean_cst->setVal(true);
    EXPECT_EQ(std::string("true"), boolean_cst->toString());
}

// integer
TEST_F(InitializerExpressionTest, IntegerConstant_construct)
{
    auto integer_cst = BstIdl::FIntegerConstant();
    integer_cst.setVal(10);
    EXPECT_EQ(integer_cst.getVal(), 10);
}

TEST_F(InitializerExpressionTest, IntegerConstant_copyConstruct)
{
    auto integer_cst = BstIdl::FIntegerConstant();
    integer_cst.setVal(10);
    auto integer_cst1 = BstIdl::FIntegerConstant(integer_cst);
    EXPECT_EQ(integer_cst.getVal(), integer_cst1.getVal());
}
/*
TEST_F(InitializerExpressionTest, IntegerConstant_notMatchType_predefined_boundary)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(11U);
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto integer_cst = factory.createFIntegerConstant();
    integer_cst->setUnsignedVal(10);
    cst->setRhs(integer_cst);
    bool valiEnable = true;
    long double value = 10L;
    try
    {
        integer_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
        integer_cst->validate(typeref, cst->isArray());
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
    EXPECT_EQ(integer_cst, cst->getRhs());
}
*/
TEST_F(InitializerExpressionTest, IntegerConstant_notMatchType_elseType)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(13U);
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto integer_cst = factory.createFIntegerConstant();
    integer_cst->setVal(10);
    cst->setRhs(integer_cst);
    bool valiEnable = true;
    std::string value;
    try
    {
        integer_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
        integer_cst->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
    EXPECT_EQ(integer_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, IntegerConstant_notMatchType_notPredefined)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type = factory.createFArrayType();
    typeref->setDerived(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto integer_cst = factory.createFIntegerConstant();
    integer_cst->setVal(10);
    cst->setRhs(integer_cst);
    bool valiEnable = true;
    std::string value;
    try
    {
        integer_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
        integer_cst->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
    EXPECT_EQ(integer_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, IntegerConstant_notMatchType_invalidTypeReference)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(1U);
    auto type1 = std::make_shared<BstIdl::FIntegerInterval>();
    typeref->setPredefined(type);
    typeref->setInterval(type1);
    cst->setType(typeref);
    cst->setArray(false);

    auto integer_cst = factory.createFIntegerConstant();
    integer_cst->setVal(10);
    cst->setRhs(integer_cst);
    bool valiEnable = true;
    std::string value;
    try
    {
        integer_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
        integer_cst->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
    EXPECT_EQ(integer_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, IntegerConstant_notMatchType_isArray)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(1U);
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(true);

    auto integer_cst = factory.createFIntegerConstant();
    integer_cst->setVal(10);
    cst->setRhs(integer_cst);
    bool valiEnable = true;
    std::string value;
    try
    {
        integer_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
        integer_cst->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
    EXPECT_EQ(integer_cst, cst->getRhs());
}
/*
TEST_F(InitializerExpressionTest, IntegerConstant_notMatchType_conflictUnsigned)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(2U);
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto integer_cst = factory.createFIntegerConstant();
    integer_cst->setUnsignedVal(-10);
    cst->setRhs(integer_cst);
    bool valiEnable = true;
    long double value = 10L;
    try
    {
        integer_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
        integer_cst->validate(typeref, cst->isArray());
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
    EXPECT_EQ(integer_cst, cst->getRhs());
}
*/
TEST_F(InitializerExpressionTest, IntegerConstant_nullTypeToValidate)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    typeref = nullptr;
    cst->setType(typeref);
    cst->setArray(false);

    auto integer_cst = factory.createFIntegerConstant();
    integer_cst->setVal(10);
    cst->setRhs(integer_cst);
    bool valiEnable = true;
    try
    {
        integer_cst->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
    EXPECT_EQ(integer_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, IntegerConstant_EvaluableInt8)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(1U);
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto integer_cst = factory.createFIntegerConstant();
    integer_cst->setUnsignedVal(127);
    cst->setRhs(integer_cst);
    bool valiEnable = true;
    std::string value;
    try
    {
        integer_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_TRUE(valiEnable);
    EXPECT_EQ(integer_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, IntegerConstant_throwEvaluableInt8)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(1U);
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto integer_cst = factory.createFIntegerConstant();
    integer_cst->setUnsignedVal(128);
    cst->setRhs(integer_cst);
    bool valiEnable = true;
    std::string value;
    try
    {
        integer_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
    EXPECT_EQ(integer_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, IntegerConstant_EvaluableUInt8)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(2U);
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto integer_cst = factory.createFIntegerConstant();
    integer_cst->setUnsignedVal(255);
    cst->setRhs(integer_cst);
    bool valiEnable = true;
    std::string value;
    try
    {
        integer_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_TRUE(valiEnable);
    EXPECT_EQ(integer_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, IntegerConstant_throwEvaluableUInt8)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(2U);
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto integer_cst = factory.createFIntegerConstant();
    integer_cst->setUnsignedVal(256);
    cst->setRhs(integer_cst);
    bool valiEnable = true;
    std::string value;
    try
    {
        integer_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
    EXPECT_EQ(integer_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, IntegerConstant_EvaluableInt16)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(3U);
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto integer_cst = factory.createFIntegerConstant();
    integer_cst->setUnsignedVal(SHRT_MAX);
    cst->setRhs(integer_cst);
    bool valiEnable = true;
    std::string value;
    try
    {
        integer_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_TRUE(valiEnable);
    EXPECT_EQ(integer_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, IntegerConstant_throwEvaluableInt16)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(3U);
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto integer_cst = factory.createFIntegerConstant();
    integer_cst->setUnsignedVal(SHRT_MAX + 1);
    cst->setRhs(integer_cst);
    bool valiEnable = true;
    std::string value;
    try
    {
        integer_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
    EXPECT_EQ(integer_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, IntegerConstant_EvaluableUInt16)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(4U);
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto integer_cst = factory.createFIntegerConstant();
    integer_cst->setUnsignedVal(USHRT_MAX);
    cst->setRhs(integer_cst);
    bool valiEnable = true;
    std::string value;
    try
    {
        integer_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_TRUE(valiEnable);
    EXPECT_EQ(integer_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, IntegerConstant_throwEvaluableUInt16)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(4U);
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto integer_cst = factory.createFIntegerConstant();
    integer_cst->setUnsignedVal(USHRT_MAX + 10);
    cst->setRhs(integer_cst);
    bool valiEnable = true;
    std::string value;
    try
    {
        integer_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
    EXPECT_EQ(integer_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, IntegerConstant_EvaluableInt32)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(5U);
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto integer_cst = factory.createFIntegerConstant();
    integer_cst->setUnsignedVal(INT_MAX);
    cst->setRhs(integer_cst);
    bool valiEnable = true;
    std::string value;
    try
    {
        integer_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_TRUE(valiEnable);
    EXPECT_EQ(integer_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, IntegerConstant_throwEvaluableInt32)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(5U);
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto integer_cst = factory.createFIntegerConstant();
    integer_cst->setUnsignedVal((uint64_t)INT_MAX + 1UL);
    cst->setRhs(integer_cst);
    bool valiEnable = true;
    std::string value;
    try
    {
        integer_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
    EXPECT_EQ(integer_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, IntegerConstant_EvaluableUInt32)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(6U);
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto integer_cst = factory.createFIntegerConstant();
    integer_cst->setUnsignedVal(UINT_MAX);
    cst->setRhs(integer_cst);
    bool valiEnable = true;
    std::string value;
    try
    {
        integer_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_TRUE(valiEnable);
    EXPECT_EQ(integer_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, IntegerConstant_throwEvaluableUInt32)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(6U);
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto integer_cst = factory.createFIntegerConstant();
    integer_cst->setUnsignedVal(LLONG_MAX);
    cst->setRhs(integer_cst);
    bool valiEnable = true;
    std::string value;
    try
    {
        integer_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
    EXPECT_EQ(integer_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, IntegerConstant_EvaluableInt64)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(7U);
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto integer_cst = factory.createFIntegerConstant();
    integer_cst->setUnsignedVal(LLONG_MAX);
    cst->setRhs(integer_cst);
    bool valiEnable = true;
    std::string value;
    try
    {
        integer_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_TRUE(valiEnable);
    EXPECT_EQ(integer_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, IntegerConstant_throwEvaluableInt64)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(7U);
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto integer_cst = factory.createFIntegerConstant();
    integer_cst->setUnsignedVal((uint64_t)LLONG_MAX + 1);
    cst->setRhs(integer_cst);
    bool valiEnable = true;
    std::string value;
    try
    {
        integer_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
    EXPECT_EQ(integer_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, IntegerConstant_EvaluableUInt64)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(BstIdl::FBasicTypeId::UINT64);
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto integer_cst = factory.createFIntegerConstant();
    integer_cst->setUnsignedVal(ULLONG_MAX);
    cst->setRhs(integer_cst);
    bool valiEnable = true;
    std::string value;
    try
    {
        integer_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_TRUE(valiEnable);
    EXPECT_EQ(integer_cst, cst->getRhs());
}
/*
TEST_F(InitializerExpressionTest, IntegerConstant_throwEvaluableUInt64)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(8U);
    typeref->setPredefined(type);
    cst->setType(typeref);
    cst->setArray(false);

    auto integer_cst = factory.createFIntegerConstant();
    integer_cst->setUnsignedVal(ULLONG_MAX + 1);
    cst->setRhs(integer_cst);
    bool valiEnable = true;
    std::string value;
    try
    {
        integer_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
    EXPECT_EQ(integer_cst, cst->getRhs());
}
*/
TEST_F(InitializerExpressionTest, IntegerConstant_envaluableInterval)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type1 = std::make_shared<BstIdl::FIntegerInterval>();
    type1->setLowerBound(255);
    type1->setUpperBound(1000);
    typeref->setInterval(type1);
    cst->setType(typeref);
    cst->setArray(false);

    auto integer_cst = factory.createFIntegerConstant();
    integer_cst->setUnsignedVal(210);
    cst->setRhs(integer_cst);
    bool valiEnable = true;
    std::string value;
    try
    {
        integer_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_FALSE(valiEnable);
    EXPECT_EQ(integer_cst, cst->getRhs());
}

TEST_F(InitializerExpressionTest, IntegerConstant_envaluableInterval1)
{
    auto cst = factory.createFConstantDef();
    auto typeref = factory.createFTypeRef();
    auto type1 = std::make_shared<BstIdl::FIntegerInterval>();
    type1->setLowerBound(255);
    type1->setUpperBound(1000);
    typeref->setInterval(type1);
    cst->setType(typeref);
    cst->setArray(false);

    auto integer_cst = factory.createFIntegerConstant();
    integer_cst->setUnsignedVal(266);
    cst->setRhs(integer_cst);
    bool valiEnable = true;
    std::string value;
    try
    {
        integer_cst->EvaluableValidate(typeref, cst->isArray(), value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        valiEnable = false;
    }
    EXPECT_TRUE(valiEnable);
    EXPECT_EQ(integer_cst, cst->getRhs());
}

// operation
TEST(OperationTest, Operator_get_value)
{
    auto &factory = BstIdl::FFactory::getInstance();
    auto op = BstIdl::FOperator::get(12U);
    EXPECT_EQ(op->getValue(), BstIdl::FOperator::DIVISION);
    EXPECT_STREQ(op->getName().c_str(), "division");
    EXPECT_STREQ(op->getLiteral().c_str(), "/");

    auto binaryOp = factory.createFBinaryOperation();
    binaryOp->setOp(op);
    auto left = factory.createFIntegerConstant();
    left->setUnsignedVal(1012);
    auto right = factory.createFIntegerConstant();
    right->setUnsignedVal(2024);
    binaryOp->setLeft(left);
    binaryOp->setRight(right);
    EXPECT_EQ(op, binaryOp->getOp());
    EXPECT_STREQ(binaryOp->toString().c_str(), "1012/2024");
}

TEST(OperationTest, Operator_get_value_undefined)
{
    auto op = BstIdl::FOperator::get(14U);
    EXPECT_EQ(op->getValue(), BstIdl::FOperator::UNDEFINED);
    EXPECT_STREQ(op->getName().c_str(), "undefined");
    EXPECT_STREQ(op->getLiteral().c_str(), "undefined");
}

TEST(OperationTest, Operator_get_Literal)
{
    auto &factory = BstIdl::FFactory::getInstance();
    auto op = BstIdl::FOperator::get("/");
    EXPECT_EQ(op->getValue(), BstIdl::FOperator::DIVISION);
    EXPECT_STREQ(op->getName().c_str(), "division");
    EXPECT_STREQ(op->getLiteral().c_str(), "/");

    auto binaryOp = factory.createFBinaryOperation();
    binaryOp->setOp(op);
    auto left = factory.createFIntegerConstant();
    left->setUnsignedVal(1012);
    auto right = factory.createFIntegerConstant();
    right->setUnsignedVal(2024);
    binaryOp->setLeft(left);
    binaryOp->setRight(right);
    EXPECT_EQ(op, binaryOp->getOp());
    EXPECT_STREQ(binaryOp->toString().c_str(), "1012/2024");
}

TEST(OperationTest, Operator_get_Literal_undefined)
{
    auto op = BstIdl::FOperator::get("lt");
    EXPECT_EQ(op->getValue(), BstIdl::FOperator::UNDEFINED);
    EXPECT_STREQ(op->getName().c_str(), "undefined");
    EXPECT_STREQ(op->getLiteral().c_str(), "undefined");
}

TEST(OperationTest, Operator_getByName)
{
    auto &factory = BstIdl::FFactory::getInstance();
    auto op = BstIdl::FOperator::getByName("division");
    EXPECT_EQ(op->getValue(), BstIdl::FOperator::DIVISION);
    EXPECT_STREQ(op->getName().c_str(), "division");
    EXPECT_STREQ(op->getLiteral().c_str(), "/");

    auto binaryOp = factory.createFBinaryOperation();
    binaryOp->setOp(op);
    auto left = factory.createFIntegerConstant();
    left->setUnsignedVal(1012);
    auto right = factory.createFIntegerConstant();
    right->setUnsignedVal(2024);
    binaryOp->setLeft(left);
    binaryOp->setRight(right);
    EXPECT_EQ(op, binaryOp->getOp());
    EXPECT_STREQ(binaryOp->toString().c_str(), "1012/2024");
}

TEST(OperationTest, Operator_getByName_undefined)
{
    auto op = BstIdl::FOperator::getByName("lt");
    EXPECT_EQ(op->getValue(), BstIdl::FOperator::UNDEFINED);
    EXPECT_STREQ(op->getName().c_str(), "undefined");
    EXPECT_STREQ(op->getLiteral().c_str(), "undefined");
}

TEST(OperationTest, Operaion_toString)
{
    auto &factory = BstIdl::FFactory::getInstance();
    auto op = BstIdl::FOperator::getByName("division");
    auto binaryOp = factory.createFBinaryOperation();
    binaryOp->setOp(op);
    auto left = factory.createFIntegerConstant();
    left->setUnsignedVal(1012);
    auto right = factory.createFIntegerConstant();
    right->setUnsignedVal(2024);
    binaryOp->setLeft(left);
    binaryOp->setRight(right);
    EXPECT_EQ(op, binaryOp->getOp());
    EXPECT_STREQ(binaryOp->toString().c_str(), "1012/2024");
}

// binaryOperation
TEST(BinaryOperationTest, set_get)
{
    auto &factory = BstIdl::FFactory::getInstance();
    auto cst = factory.createFConstantDef();
    cst->setArray(false);
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(8U);
    typeref->setPredefined(type);
    cst->setType(typeref);

    auto binaryOp = factory.createFBinaryOperation();
    auto op = BstIdl::FOperator::get(9U);
    binaryOp->setOp(op);
    auto left = factory.createFIntegerConstant();
    left->setUnsignedVal(1012);
    auto right = factory.createFIntegerConstant();
    right->setUnsignedVal(2024);
    binaryOp->setLeft(left);
    binaryOp->setRight(right);

    cst->setRhs(binaryOp);
    long double value = 10L;
    bool validate = true;
    try
    {
        binaryOp->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        validate = false;
    }
    EXPECT_TRUE(validate);
    EXPECT_EQ(binaryOp->getLeft(), left);
    EXPECT_EQ(binaryOp->getRight(), right);
}

TEST(BinaryOperationTest, nullTypetoValidate)
{
    auto &factory = BstIdl::FFactory::getInstance();
    auto cst = factory.createFConstantDef();
    cst->setArray(false);
    auto typeref = factory.createFTypeRef();
    typeref = nullptr;
    cst->setType(typeref);

    auto binaryOp = factory.createFBinaryOperation();
    auto op = BstIdl::FOperator::get(9U);
    binaryOp->setOp(op);
    auto left = factory.createFIntegerConstant();
    left->setUnsignedVal(1012);
    auto right = factory.createFIntegerConstant();
    right->setUnsignedVal(2024);
    binaryOp->setLeft(left);
    binaryOp->setRight(right);

    cst->setRhs(binaryOp);
    long double value = 10L;
    bool validate = true;
    try
    {
        binaryOp->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        validate = false;
    }
    EXPECT_FALSE(validate);
}

TEST(BinaryOperationTest, notMatchType_isArray)
{
    auto &factory = BstIdl::FFactory::getInstance();
    auto cst = factory.createFConstantDef();
    cst->setArray(true);
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(8U);
    typeref->setPredefined(type);
    cst->setType(typeref);

    auto binaryOp = factory.createFBinaryOperation();
    auto op = BstIdl::FOperator::get(9U);
    binaryOp->setOp(op);
    auto left = factory.createFIntegerConstant();
    left->setUnsignedVal(1012);
    auto right = factory.createFIntegerConstant();
    right->setUnsignedVal(2024);
    binaryOp->setLeft(left);
    binaryOp->setRight(right);

    cst->setRhs(binaryOp);
    long double value = 10L;
    bool validate = true;
    try
    {
        binaryOp->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        validate = false;
    }
    EXPECT_FALSE(validate);
}

TEST(BinaryOperationTest, nullType_derived)
{
    auto &factory = BstIdl::FFactory::getInstance();
    auto cst = factory.createFConstantDef();
    cst->setArray(false);
    auto typeref = factory.createFTypeRef();
    auto type = factory.createFArrayType();
    typeref->setDerived(type);
    cst->setType(typeref);

    auto binaryOp = factory.createFBinaryOperation();
    auto op = BstIdl::FOperator::get(9U);
    binaryOp->setOp(op);
    auto left = factory.createFIntegerConstant();
    left->setUnsignedVal(1012);
    auto right = factory.createFIntegerConstant();
    right->setUnsignedVal(2024);
    binaryOp->setLeft(left);
    binaryOp->setRight(right);

    cst->setRhs(binaryOp);
    long double value = 10L;
    bool validate = true;
    try
    {
        binaryOp->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        validate = false;
    }
    EXPECT_FALSE(validate);
}

TEST(BinaryOperationTest, invalidTypeReference)
{
    auto &factory = BstIdl::FFactory::getInstance();
    auto cst = factory.createFConstantDef();
    cst->setArray(false);
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(8U);
    typeref->setPredefined(type);
    auto type1 = factory.createFIntegerInterval();
    typeref->setInterval(type1);
    cst->setType(typeref);

    auto binaryOp = factory.createFBinaryOperation();
    auto op = BstIdl::FOperator::get(9U);
    binaryOp->setOp(op);
    auto left = factory.createFIntegerConstant();
    left->setUnsignedVal(1012);
    auto right = factory.createFIntegerConstant();
    right->setUnsignedVal(2024);
    binaryOp->setLeft(left);
    binaryOp->setRight(right);

    cst->setRhs(binaryOp);
    long double value = 10L;
    bool validate = true;
    try
    {
        binaryOp->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        validate = false;
    }
    EXPECT_FALSE(validate);
}

TEST(BinaryOperationTest, invalidOperator)
{
    auto &factory = BstIdl::FFactory::getInstance();
    auto cst = factory.createFConstantDef();
    cst->setArray(false);
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(8U);
    typeref->setPredefined(type);
    cst->setType(typeref);

    auto binaryOp = factory.createFBinaryOperation();
    auto op = BstIdl::FOperator::get(20U);
    binaryOp->setOp(op);
    auto left = factory.createFIntegerConstant();
    left->setUnsignedVal(1012);
    auto right = factory.createFIntegerConstant();
    right->setUnsignedVal(2024);
    binaryOp->setLeft(left);
    binaryOp->setRight(right);

    cst->setRhs(binaryOp);
    long double value = 10L;
    bool validate = true;
    try
    {
        binaryOp->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        validate = false;
    }
    EXPECT_FALSE(validate);
}

TEST(BinaryOperationTest, notMatchType_notboolean)
{
    auto &factory = BstIdl::FFactory::getInstance();
    auto cst = factory.createFConstantDef();
    cst->setArray(false);
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(8U);
    typeref->setPredefined(type);
    cst->setType(typeref);

    auto binaryOp = factory.createFBinaryOperation();
    auto op = BstIdl::FOperator::get(4U);
    binaryOp->setOp(op);
    auto left = factory.createFIntegerConstant();
    left->setUnsignedVal(1012);
    auto right = factory.createFIntegerConstant();
    right->setUnsignedVal(2024);
    binaryOp->setLeft(left);
    binaryOp->setRight(right);

    cst->setRhs(binaryOp);
    long double value = 10L;
    bool validate = true;
    try
    {
        binaryOp->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        validate = false;
    }
    EXPECT_FALSE(validate);
}

TEST(BinaryOperationTest, notMatchType_notNumeric)
{
    auto &factory = BstIdl::FFactory::getInstance();
    auto cst = factory.createFConstantDef();
    cst->setArray(false);
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(0U);
    typeref->setPredefined(type);
    cst->setType(typeref);

    auto binaryOp = factory.createFBinaryOperation();
    auto op = BstIdl::FOperator::get(9U);
    binaryOp->setOp(op);
    auto left = factory.createFIntegerConstant();
    left->setUnsignedVal(1012);
    auto right = factory.createFIntegerConstant();
    right->setUnsignedVal(2024);
    binaryOp->setLeft(left);
    binaryOp->setRight(right);

    cst->setRhs(binaryOp);
    long double value = 10L;
    bool validate = true;
    try
    {
        binaryOp->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        validate = false;
    }
    EXPECT_FALSE(validate);
}

TEST(BinaryOperationTest, intervalBoolean)
{
    auto &factory = BstIdl::FFactory::getInstance();
    auto cst = factory.createFConstantDef();
    cst->setArray(false);
    auto typeref = factory.createFTypeRef();
    auto type1 = factory.createFIntegerInterval();
    typeref->setInterval(type1);
    cst->setType(typeref);

    auto binaryOp = factory.createFBinaryOperation();
    auto op = BstIdl::FOperator::get(1U);
    binaryOp->setOp(op);
    auto left = factory.createFIntegerConstant();
    left->setUnsignedVal(1012);
    auto right = factory.createFIntegerConstant();
    right->setUnsignedVal(2024);
    binaryOp->setLeft(left);
    binaryOp->setRight(right);

    cst->setRhs(binaryOp);
    long double value = 10L;
    bool validate = true;
    try
    {
        binaryOp->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        validate = false;
    }
    EXPECT_FALSE(validate);
}

TEST(BinaryOperationTest, evalubaleValidate_interavalAdd)
{
    auto &factory = BstIdl::FFactory::getInstance();
    auto cst = factory.createFConstantDef();
    cst->setArray(false);
    auto typeref = factory.createFTypeRef();
    auto type1 = factory.createFIntegerInterval();
    type1->setLowerBound(1000);
    type1->setUpperBound(4000);
    typeref->setInterval(type1);
    cst->setType(typeref);

    auto binaryOp = factory.createFBinaryOperation();
    auto op = BstIdl::FOperator::get(9U);
    binaryOp->setOp(op);
    auto left = factory.createFIntegerConstant();
    left->setUnsignedVal(1012);
    auto right = factory.createFIntegerConstant();
    right->setUnsignedVal(2024);
    binaryOp->setLeft(left);
    binaryOp->setRight(right);

    cst->setRhs(binaryOp);
    std::string value;
    bool validate = true;
    try
    {
        binaryOp->EvaluableValidate(typeref, cst->isArray(), value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        validate = false;
    }
    EXPECT_TRUE(validate);
}

TEST(BinaryOperationTest, evalubaleValidate_interavalSub)
{
    auto &factory = BstIdl::FFactory::getInstance();
    auto cst = factory.createFConstantDef();
    cst->setArray(false);
    auto typeref = factory.createFTypeRef();
    auto type1 = factory.createFIntegerInterval();
    type1->setLowerBound(1000);
    type1->setUpperBound(4000);
    typeref->setInterval(type1);
    cst->setType(typeref);

    auto binaryOp = factory.createFBinaryOperation();
    auto op = BstIdl::FOperator::get(10U);
    binaryOp->setOp(op);
    auto left = factory.createFIntegerConstant();
    left->setUnsignedVal(1012);
    auto right = factory.createFIntegerConstant();
    right->setUnsignedVal(12);
    binaryOp->setLeft(left);
    binaryOp->setRight(right);

    cst->setRhs(binaryOp);
    std::string value;
    bool validate = true;
    try
    {
        binaryOp->EvaluableValidate(typeref, cst->isArray(), value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        validate = false;
    }
    EXPECT_TRUE(validate);
}

TEST(BinaryOperationTest, evalubaleValidate_interavalMulti)
{
    auto &factory = BstIdl::FFactory::getInstance();
    auto cst = factory.createFConstantDef();
    cst->setArray(false);
    auto typeref = factory.createFTypeRef();
    auto type1 = factory.createFIntegerInterval();
    type1->setLowerBound(1000);
    type1->setUpperBound(4000);
    typeref->setInterval(type1);
    cst->setType(typeref);

    auto binaryOp = factory.createFBinaryOperation();
    auto op = BstIdl::FOperator::get(11U);
    binaryOp->setOp(op);
    auto left = factory.createFIntegerConstant();
    left->setUnsignedVal(1012);
    auto right = factory.createFIntegerConstant();
    right->setUnsignedVal(3);
    binaryOp->setLeft(left);
    binaryOp->setRight(right);

    cst->setRhs(binaryOp);
    std::string value;
    bool validate = true;
    try
    {
        binaryOp->EvaluableValidate(typeref, cst->isArray(), value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        validate = false;
    }
    EXPECT_TRUE(validate);
}

TEST(BinaryOperationTest, evalubaleValidate_interavalDivision)
{
    auto &factory = BstIdl::FFactory::getInstance();
    auto cst = factory.createFConstantDef();
    cst->setArray(false);
    auto typeref = factory.createFTypeRef();
    auto type1 = factory.createFIntegerInterval();
    type1->setLowerBound(0);
    type1->setUpperBound(4000);
    typeref->setInterval(type1);
    cst->setType(typeref);

    auto binaryOp = factory.createFBinaryOperation();
    auto op = BstIdl::FOperator::get(12U);
    binaryOp->setOp(op);
    auto left = factory.createFIntegerConstant();
    left->setUnsignedVal(2024);
    auto right = factory.createFIntegerConstant();
    right->setUnsignedVal(2024);
    binaryOp->setLeft(left);
    binaryOp->setRight(right);

    cst->setRhs(binaryOp);
    std::string value;
    bool validate = true;
    try
    {
        binaryOp->EvaluableValidate(typeref, cst->isArray(), value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        validate = false;
    }
    EXPECT_TRUE(validate);
}

TEST(BinaryOperationTest, evalubaleValidate_Division)
{
    auto &factory = BstIdl::FFactory::getInstance();
    auto cst = factory.createFConstantDef();
    cst->setArray(false);
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(8U);
    typeref->setPredefined(type);
    cst->setType(typeref);

    auto binaryOp = factory.createFBinaryOperation();
    auto op = BstIdl::FOperator::get(12U);
    binaryOp->setOp(op);
    auto left = factory.createFIntegerConstant();
    left->setUnsignedVal(2024);
    auto right = factory.createFIntegerConstant();
    right->setUnsignedVal(2024);
    binaryOp->setLeft(left);
    binaryOp->setRight(right);

    cst->setRhs(binaryOp);
    std::string value;
    bool validate = true;
    try
    {
        binaryOp->EvaluableValidate(typeref, cst->isArray(), value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        validate = false;
    }
    EXPECT_TRUE(validate);
}

TEST(BinaryOperationTest, evalubaleValidate_Add)
{
    auto &factory = BstIdl::FFactory::getInstance();
    auto cst = factory.createFConstantDef();
    cst->setArray(false);
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(8U);
    typeref->setPredefined(type);
    cst->setType(typeref);

    auto binaryOp = factory.createFBinaryOperation();
    auto op = BstIdl::FOperator::get(9U);
    binaryOp->setOp(op);
    auto left = factory.createFIntegerConstant();
    left->setUnsignedVal(2024);
    auto right = factory.createFIntegerConstant();
    right->setUnsignedVal(2024);
    binaryOp->setLeft(left);
    binaryOp->setRight(right);

    cst->setRhs(binaryOp);
    std::string value;
    bool validate = true;
    try
    {
        binaryOp->EvaluableValidate(typeref, cst->isArray(), value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        validate = false;
    }
    EXPECT_TRUE(validate);
}

TEST(BinaryOperationTest, evalubaleValidate_DivisionByZero)
{
    auto &factory = BstIdl::FFactory::getInstance();
    auto cst = factory.createFConstantDef();
    cst->setArray(false);
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(8U);
    typeref->setPredefined(type);
    cst->setType(typeref);

    auto binaryOp = factory.createFBinaryOperation();
    auto op = BstIdl::FOperator::get(12U);
    binaryOp->setOp(op);
    auto left = factory.createFIntegerConstant();
    left->setUnsignedVal(2024);
    auto right = factory.createFIntegerConstant();
    right->setUnsignedVal(0);
    binaryOp->setLeft(left);
    binaryOp->setRight(right);

    cst->setRhs(binaryOp);
    std::string value;
    bool validate = true;
    try
    {
        binaryOp->EvaluableValidate(typeref, cst->isArray(), value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        validate = false;
    }
    EXPECT_FALSE(validate);
}

TEST(UnaryOperationTest, toString)
{
    auto &factory = BstIdl::FFactory::getInstance();
    auto cst = factory.createFConstantDef();
    cst->setArray(false);
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(8U);
    typeref->setPredefined(type);
    cst->setType(typeref);

    auto unaryOp = factory.createFUnaryOperation();
    auto op = BstIdl::FOperator::get(10U);
    auto operand = factory.createFIntegerConstant();
    operand->setUnsignedVal(100U);
    unaryOp->setOperand(operand);
    unaryOp->setOp(op);
    cst->setRhs(unaryOp);

    EXPECT_EQ(operand, unaryOp->getOperand());
    EXPECT_STREQ(unaryOp->toString().c_str(), "-100");
}

TEST(UnaryOperationTest, validate_Predefined)
{
    auto &factory = BstIdl::FFactory::getInstance();
    auto cst = factory.createFConstantDef();
    cst->setArray(false);
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(7U);
    typeref->setPredefined(type);
    cst->setType(typeref);

    auto unaryOp = factory.createFUnaryOperation();
    auto op = BstIdl::FOperator::get(10U);
    auto operand = factory.createFIntegerConstant();
    operand->setUnsignedVal(100U);
    unaryOp->setOperand(operand);
    unaryOp->setOp(op);

    cst->setRhs(unaryOp);
    std::string value;
    bool validate = true;
    try
    {
        unaryOp->validate(typeref, cst->isArray());
        unaryOp->EvaluableValidate(typeref, cst->isArray(), value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        validate = false;
    }
    EXPECT_TRUE(validate);
}

TEST(UnaryOperationTest, Evalidate_Interval)
{
    auto &factory = BstIdl::FFactory::getInstance();
    auto cst = factory.createFConstantDef();
    cst->setArray(false);
    auto typeref = factory.createFTypeRef();
    auto type1 = factory.createFIntegerInterval();
    type1->setLowerBound(-200);
    type1->setUpperBound(4000);
    typeref->setInterval(type1);
    cst->setType(typeref);

    auto unaryOp = factory.createFUnaryOperation();
    auto op = BstIdl::FOperator::get(10U);
    auto operand = factory.createFIntegerConstant();
    operand->setUnsignedVal(100U);
    unaryOp->setOperand(operand);
    unaryOp->setOp(op);

    cst->setRhs(unaryOp);
    std::string value;
    bool validate = true;
    try
    {
        unaryOp->EvaluableValidate(typeref, cst->isArray(), value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        validate = false;
    }
    EXPECT_TRUE(validate);
}

TEST(UnaryOperationTest, invalidTypeReference)
{
    auto &factory = BstIdl::FFactory::getInstance();
    auto cst = factory.createFConstantDef();
    cst->setArray(false);
    auto typeref = factory.createFTypeRef();
    auto type1 = factory.createFIntegerInterval();
    type1->setLowerBound(-200);
    type1->setUpperBound(4000);
    auto type = BstIdl::FBasicTypeId::get(13U);
    typeref->setPredefined(type);
    typeref->setInterval(type1);
    cst->setType(typeref);

    auto unaryOp = factory.createFUnaryOperation();
    auto op = BstIdl::FOperator::get(10U);
    auto operand = factory.createFIntegerConstant();
    operand->setUnsignedVal(100U);
    unaryOp->setOperand(operand);
    unaryOp->setOp(op);

    cst->setRhs(unaryOp);
    long double value;
    bool validate = true;
    try
    {
        unaryOp->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        validate = false;
    }
    // integerconst need predefined
    EXPECT_FALSE(validate);
}

TEST(UnaryOperationTest, validate_notNumeric_predefined)
{
    auto &factory = BstIdl::FFactory::getInstance();
    auto cst = factory.createFConstantDef();
    cst->setArray(false);
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(13U);
    typeref->setPredefined(type);
    cst->setType(typeref);

    auto unaryOp = factory.createFUnaryOperation();
    auto op = BstIdl::FOperator::get(10U);
    auto operand = factory.createFIntegerConstant();
    operand->setUnsignedVal(100U);
    unaryOp->setOperand(operand);
    unaryOp->setOp(op);

    cst->setRhs(unaryOp);
    long double value;
    bool validate = true;
    try
    {
        unaryOp->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        validate = false;
    }
    EXPECT_FALSE(validate);
}

TEST(UnaryOperationTest, validate_notNumeric_interval)
{
    auto &factory = BstIdl::FFactory::getInstance();
    auto cst = factory.createFConstantDef();
    cst->setArray(false);
    auto typeref = factory.createFTypeRef();
    auto type1 = factory.createFIntegerInterval();
    type1->setLowerBound(-200);
    type1->setUpperBound(4000);
    typeref->setInterval(type1);
    cst->setType(typeref);

    auto unaryOp = factory.createFUnaryOperation();
    auto op = BstIdl::FOperator::get(10U);
    auto operand = factory.createFIntegerConstant();
    operand->setUnsignedVal(100U);
    unaryOp->setOperand(operand);
    unaryOp->setOp(op);

    cst->setRhs(unaryOp);
    long double value;
    bool validate = true;
    try
    {
        unaryOp->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        validate = false;
    }
    // integerconst need predefined
    EXPECT_FALSE(validate);
}

TEST(UnaryOperationTest, validateNegation)
{
    auto &factory = BstIdl::FFactory::getInstance();
    auto cst = factory.createFConstantDef();
    cst->setArray(false);
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(9U);
    typeref->setPredefined(type);
    cst->setType(typeref);

    auto unaryOp = factory.createFUnaryOperation();
    auto op = BstIdl::FOperator::get(13U);
    auto operand = factory.createFBooleanConstant();
    operand->setVal(false);
    unaryOp->setOperand(operand);
    unaryOp->setOp(op);

    cst->setRhs(unaryOp);
    long double value;
    bool validate = true;
    try
    {
        unaryOp->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        validate = false;
    }
    EXPECT_TRUE(validate);
}

TEST(UnaryOperationTest, validate_notBoolean_predefined)
{
    auto &factory = BstIdl::FFactory::getInstance();
    auto cst = factory.createFConstantDef();
    cst->setArray(false);
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(8U);
    typeref->setPredefined(type);
    cst->setType(typeref);

    auto unaryOp = factory.createFUnaryOperation();
    auto op = BstIdl::FOperator::get(13U);
    auto operand = factory.createFIntegerConstant();
    operand->setUnsignedVal(100U);
    unaryOp->setOperand(operand);
    unaryOp->setOp(op);

    cst->setRhs(unaryOp);
    long double value;
    bool validate = true;
    try
    {
        unaryOp->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        validate = false;
    }
    EXPECT_FALSE(validate);
}

TEST(UnaryOperationTest, validate_invalidOperator)
{
    auto &factory = BstIdl::FFactory::getInstance();
    auto cst = factory.createFConstantDef();
    cst->setArray(false);
    auto typeref = factory.createFTypeRef();
    auto type = BstIdl::FBasicTypeId::get(8U);
    typeref->setPredefined(type);
    cst->setType(typeref);

    auto unaryOp = factory.createFUnaryOperation();
    auto op = BstIdl::FOperator::get(1U);
    auto operand = factory.createFIntegerConstant();
    operand->setUnsignedVal(100U);
    unaryOp->setOperand(operand);
    unaryOp->setOp(op);

    cst->setRhs(unaryOp);
    long double value;
    bool validate = true;
    try
    {
        unaryOp->validate(typeref, cst->isArray());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        validate = false;
    }
    EXPECT_FALSE(validate);
}

TEST(EvaluableElementTest, valueToString)
{
    auto content = R"(package com.bst.idl.parser.test1
    interface Test1 {
        attribute UInt32 testint noRead
        attribute Double temp readonly noSubscriptions
        typedef ButtonId is Int32
        method walk fireAndForget {
        in {
                Boolean b
            }
            out 
            {
                String str
            }
        }
        method stand:a 
        {
            in 
            {
                Boolean b
            }
            out 
            {
                String str
            }
         }

        method stand:b 
        {
            in 
            {
                Boolean b
            }
        }
        broadcast broad:a selective{
            out 
            {
                ButtonId id
                Boolean isLongPress
            }          
        }
        broadcast broad:b {
            out 
            {
                ButtonId id
            }          
        }
    }
    )";

    auto ret = BstIdl::FidlParser::parseString(content);
    EXPECT_TRUE(ret != nullptr);
    auto &mgr = BstIdl::FModelManager::getInstance();
    EXPECT_FALSE(mgr.empty());
    auto model = mgr.getModelList().front();
    auto interface = model->getInterfaces().front();
    auto attributes = interface->getAttributes();
    auto methods = interface->getMethods();
    auto broadcasts = interface->getBroadcasts();
    // broadcast
    EXPECT_EQ(broadcasts.front()->isSelective(), true);
    EXPECT_EQ(broadcasts.back()->isSelective(), false);
    EXPECT_STREQ(broadcasts.front()->getSelector().c_str(), "a");
    EXPECT_STREQ(broadcasts.back()->getSelector().c_str(), "b");
    // attributes
    EXPECT_EQ(attributes.front()->isNoRead(), true);
    EXPECT_EQ(attributes.back()->isReadonly(), true);
    EXPECT_EQ(attributes.back()->isNoSubscriptions(), true);
    // methods
    EXPECT_EQ(methods.front()->isFireAndForget(), true);
    methods.pop_front();
    EXPECT_STREQ(methods.front()->getSelector().c_str(), "a");
    EXPECT_STREQ(methods.back()->getSelector().c_str(), "b");
}
