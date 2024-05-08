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
#include "model/FBinaryOperation.h"
#include "model/FTypeRef.h"

namespace BstIdl
{
void FBinaryOperation::validate(std::shared_ptr<FTypeRef> &type, bool isArray)
{
    FOperation::validate(type, isArray);
    uint32_t typeID = 0;
    auto predefined = type->getPredefined();
    auto interval = type->getInterval();
    if (predefined)
        typeID = predefined->getValue();
    if (interval)
        typeID = FBasicTypeId::INT64;
    if (predefined && interval)
        throw initializer_error("BinaryOperation : Invalid type reference.\tpredefined and interval are in conflict");

    switch (m_op->getValue())
    {
    case FOperator::OR:
    case FOperator::EQUAL:
    case FOperator::UNEQUAL:
    case FOperator::AND:
    case FOperator::SMALLER:
    case FOperator::SMALLER_OR_EQUAL:
    case FOperator::GREATER_OR_EQUAL:
    case FOperator::GREATER:
        if (!FBasicTypeId::isBoolean(typeID))
            if (predefined)
                throw initializer_error("BinaryOperation :Initilizer not match type .\nactual type:" +
                                        predefined->getName() + "\texpect:   Boolean");
        break;
    case FOperator::ADDITION:
    case FOperator::SUBTRACTION:
    case FOperator::MULTIPLICATION:
    case FOperator::DIVISION:
        if (!FBasicTypeId::isNumeric(typeID))
            throw initializer_error("BinaryOperation :Initilizer not match type.\nactual type:" +
                                    predefined->getName() + "\texpect:   Numeric type");
        break;
    default:
        throw initializer_error("BinaryOperation :Invalid operator.");
        break;
    }

    m_left->validate(type, false);
    m_right->validate(type, false);
}
void FBinaryOperation::EvaluableValidate(std::shared_ptr<FTypeRef> &type, bool isArray, std::string &value,
                                         bool is_init_exp)
{
    std::string l_value;
    std::string r_value;
    FExpression::EvaluableValidate(type, isArray, value, is_init_exp);
    uint32_t typeID = 0;
    auto predefined = type->getPredefined();
    auto interval = type->getInterval();
    if (predefined)
        typeID = predefined->getValue();
    if (interval)
        typeID = FBasicTypeId::INT64;
    m_left->EvaluableValidate(type, false, l_value, false);
    m_right->EvaluableValidate(type, false, r_value, false);

    switch (m_op->getValue())
    {
    case FOperator::ADDITION:
        getStringValue(typeID, l_value, r_value, value);
        if (is_init_exp)
        {
            if (predefined)
                boundaryCheck(typeID, value);
            if (interval)
                boundaryCheck(interval->getUpperBound(), interval->getLowerBound(), value);
        }
        break;
    case FOperator::SUBTRACTION:
        getStringValue(typeID, l_value, r_value, value);
        if (is_init_exp)
        {
            if (predefined)
                boundaryCheck(typeID, value);
            if (interval)
                boundaryCheck(interval->getUpperBound(), interval->getLowerBound(), value);
        }
        break;
    case FOperator::MULTIPLICATION:
        getStringValue(typeID, l_value, r_value, value);
        if (is_init_exp)
        {
            if (predefined)
                boundaryCheck(typeID, value);
            if (interval)
                boundaryCheck(interval->getUpperBound(), interval->getLowerBound(), value);
        }
        break;
    case FOperator::DIVISION:
        getStringValue(typeID, l_value, r_value, value);
        if (is_init_exp)
        {
            if (predefined)
                boundaryCheck(typeID, value);
            if (interval)
                boundaryCheck(interval->getUpperBound(), interval->getLowerBound(), value);
        }
        break;
    }
}

void FBinaryOperation::getStringValue(const uint32_t typeID, const std::string &l_value, const std::string &r_value,
                                      std::string &value)
{
    // wrong type
    if (!((typeID >= 1 && typeID <= 8) || (typeID >= 11 && typeID <= 12)))
    {
        value.clear();
        return;
    }

    long long s_lvalue;
    long long s_rvalue;
    unsigned long long u_lvalue;
    unsigned long long u_rvalue;
    long double ld_lvalue;
    long double ld_rvalue;
    int type = -1;
    if (typeID == FBasicTypeId::INT64)
    {
        type = 0;
        s_lvalue = std::stoll(l_value, nullptr);
        s_rvalue = std::stoll(r_value, nullptr);
    }
    else if (typeID == FBasicTypeId::UINT64)
    {
        type = 1;
        u_lvalue = std::stoul(l_value, nullptr);
        u_rvalue = std::stoul(r_value, nullptr);
    }
    else
    {
        type = 2;
        ld_lvalue = std::stold(l_value, nullptr);
        ld_rvalue = std::stold(r_value, nullptr);
    }

    switch (m_op->getValue())
    {
    case FOperator::ADDITION:
        if (!type)
            value = std::to_string(s_lvalue + s_rvalue);
        else if (type == 1)
            value = std::to_string(u_lvalue + u_rvalue);
        else
            value = std::to_string(ld_lvalue + ld_rvalue);
        break;
    case FOperator::SUBTRACTION:
        if (!type)
            value = std::to_string(s_lvalue - s_rvalue);
        else if (type == 1)
            value = std::to_string(u_lvalue - u_rvalue);
        else
            value = std::to_string(ld_lvalue - ld_rvalue);
        break;
    case FOperator::MULTIPLICATION:
        if (!type)
            value = std::to_string(s_lvalue * s_rvalue);
        else if (type == 1)
            value = std::to_string(u_lvalue * u_rvalue);
        else
            value = std::to_string(ld_lvalue * ld_rvalue);
        break;
    case FOperator::DIVISION:
        if (!type && std::abs(s_rvalue) >= 1e-8)
            value = std::to_string(s_lvalue / s_rvalue);
        else if (type == 1 && std::abs((double)u_rvalue) >= 1e-8)
            value = std::to_string(u_lvalue / u_rvalue);
        else if (type == 2 && std::abs(ld_rvalue) >= 1e-8)
            value = std::to_string(ld_lvalue / ld_rvalue);
        else
            throw initializer_error("BinaryOperation :divition by zero.");
        break;
    }
}

} // namespace BstIdl