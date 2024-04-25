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
#include "model/FUnaryOperation.h"
#include "model/FTypeRef.h"

namespace BstIdl
{
void FUnaryOperation::validate(std::shared_ptr<FTypeRef> &type, bool isArray)
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
        throw initializer_error(
            "UnaryOperation:Invalid type reference.\tpredefinedType and intervalType are in conflict");

    switch (m_op->getValue())
    {
    case FOperator::SUBTRACTION:
        if (!FBasicTypeId::isNumeric(typeID))
        {
            if (predefined)
                throw initializer_error("UnaryOperation:Initilizer not match type.\nactual type: " +
                                        type->getPredefined()->getName() + " expect  :  Numeric type");
            else
            {
                throw initializer_error("UnaryOperation:Initilizer not match type.\nactual type: " +
                                        type->getDerived()->getName() + " expect  :  Numeric type");
            }
        }
        break;
    case FOperator::NEGATION:
        if (!FBasicTypeId::isBoolean(typeID))
        {
            if (predefined)
                throw initializer_error("UnaryOperation:Initilizer not match type.\nactual type: " +
                                        type->getPredefined()->getName() + " expect  :  Boolean type");
            else
            {
                throw initializer_error("UnaryOperation:Initilizer not match type.\nactual type: " +
                                        type->getDerived()->getName() + " expect  :  Boolean type");
            }
        }
        break;
    default:
        throw initializer_error("UnaryOperation:Invalid operator.");
        break;
    }

    m_operand->validate(type, false);
}
void FUnaryOperation::EvaluableValidate(std::shared_ptr<FTypeRef> &type, bool isArray, std::string &value,
                                        bool is_init_exp)
{
    FOperation::EvaluableValidate(type, isArray, value, is_init_exp);
    uint32_t typeID = 0;
    auto predefined = type->getPredefined();
    auto interval = type->getInterval();

    if (interval)
        typeID = FBasicTypeId::INT64;
    else
        typeID = predefined->getValue();
    m_operand->EvaluableValidate(type, isArray, value, false);
    if (m_op->getValue() == FOperator::SUBTRACTION)
    {
        getStringValue(typeID, value);
        if (is_init_exp)
        {
            if (predefined)
            {
                boundaryCheck(typeID, value);
            }
            if (interval)
            {
                boundaryCheck(interval->getUpperBound(), interval->getLowerBound(), value);
            }
        }
    }
}

void FUnaryOperation::getStringValue(const uint32_t typeID, std::string &value)
{
    // wrong type
    if (!((typeID >= 1 && typeID <= 8) || (typeID >= 11 && typeID <= 12)))
    {
        value.clear();
        return;
    }

    if (typeID == FBasicTypeId::INT64)
    {
        auto tmp = std::stoll(value, nullptr);
        value = std::to_string(-(tmp));
        return;
    }
    else if (typeID == FBasicTypeId::UINT64)
    {
        auto tmp = std::stoull(value, nullptr);
        value = std::to_string(-(tmp));
        return;
    }
    else
    {
        auto tmp = std::stold(value, nullptr);
        value = std::to_string(-(tmp));
        return;
    }
}
} // namespace BstIdl