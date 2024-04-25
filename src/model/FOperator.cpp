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
#include "model/FOperator.h"

namespace BstIdl
{
static const std::shared_ptr<FOperator> s_operatorTable[] = {
    std::make_shared<FOperator>(FOperator::UNDEFINED, "undefined", "undefined"),
    std::make_shared<FOperator>(FOperator::OR, "Or", "||"),
    std::make_shared<FOperator>(FOperator::EQUAL, "Equal", "=="),
    std::make_shared<FOperator>(FOperator::UNEQUAL, "Unequal", "!="),
    std::make_shared<FOperator>(FOperator::AND, "And", "&&"),
    std::make_shared<FOperator>(FOperator::SMALLER, "Smaller", "<"),
    std::make_shared<FOperator>(FOperator::SMALLER_OR_EQUAL, "SmallerOrEqual", "<="),
    std::make_shared<FOperator>(FOperator::GREATER_OR_EQUAL, "GreaterOrEqual", ">="),
    std::make_shared<FOperator>(FOperator::GREATER, "Greater", ">"),
    std::make_shared<FOperator>(FOperator::ADDITION, "addition", "+"),
    std::make_shared<FOperator>(FOperator::SUBTRACTION, "subtraction", "-"),
    std::make_shared<FOperator>(FOperator::MULTIPLICATION, "multiplication", "*"),
    std::make_shared<FOperator>(FOperator::DIVISION, "division", "/"),
    std::make_shared<FOperator>(FOperator::NEGATION, "negation", "!"),
};

std::shared_ptr<FOperator> FOperator::get(const std::string &literal)
{
    for (int i = 0; i < OPERATOR_MAX; ++i)
    {
        if (!s_operatorTable[i]->getLiteral().compare(literal))
            return s_operatorTable[i];
    }

    return s_operatorTable[UNDEFINED];
}

std::shared_ptr<FOperator> FOperator::getByName(const std::string &name)
{
    for (int i = 0; i < OPERATOR_MAX; ++i)
    {
        if (!s_operatorTable[i]->getName().compare(name))
            return s_operatorTable[i];
    }

    return s_operatorTable[UNDEFINED];
}

std::shared_ptr<FOperator> FOperator::get(uint32_t value)
{
    if (value < OPERATOR_MAX)
        return s_operatorTable[value];
    else
        return s_operatorTable[UNDEFINED];
}

} // namespace BstIdl