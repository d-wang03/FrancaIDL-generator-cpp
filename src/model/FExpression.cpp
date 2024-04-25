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
#include "model/FExpression.h"
#include "model/FBasicTypeId.h"
#include "model/FTypeRef.h"
#include "utilities/string_utility.h"
#include <climits>
#include <cmath>
#include <cstring>
#include <float.h>
#include <iostream>

namespace BstIdl
{
void FExpression::validate(std::shared_ptr<FTypeRef> &type, bool isArray)
{
    FInitializerExpression::validate(type, isArray);
    if (isArray || (type->getPredefined() == nullptr && type->getInterval() == nullptr))
        throw initializer_error("Initilizer not match type.\tIntervalType and PredefinedType are in conflict");
}
void FExpression::EvaluableValidate(std::shared_ptr<FTypeRef> &type, bool isArray, std::string &value, bool is_init_exp)
{
    FInitializerExpression::EvaluableValidate(type, isArray, value, is_init_exp);
}
void FExpression::boundaryCheck(uint32_t typeID, const std::string &value)
{
    switch (typeID)
    {
    case FBasicTypeId::INT8: {
        std::cout << "value = " << value << '\n';
        try
        {
            auto n = std::stoi(value, nullptr);
            if (n < INT8_MIN || n > INT8_MAX)
                throw std::out_of_range("");
        }
        catch (const std::out_of_range &oor)
        {
            throw initializer_error("The value of the expression overflowed.");
        }
        catch (const std::invalid_argument &ia)
        {
            throw initializer_error("The value of the expression is invalid.");
        }

        break;
    }
    case FBasicTypeId::BasicTypeId::UINT8: {
        std::cout << "value = " << value << '\n';
        try
        {
            auto n = std::stoul(value, nullptr);
            if (n < 0 || n > UINT8_MAX)
                throw std::out_of_range("");
        }
        catch (const std::out_of_range &oor)
        {
            throw initializer_error("The value of the expression overflowed.");
        }
        catch (const std::invalid_argument &ia)
        {
            throw initializer_error("The value of the expression is invalid.");
        }
        break;
    }
    case FBasicTypeId::INT16: {
        std::cout << "value = " << value << '\n';
        try
        {
            auto n = std::stoi(value, nullptr);
            if (n < INT16_MIN || n > INT16_MAX)
                throw std::out_of_range("");
        }
        catch (const std::out_of_range &oor)
        {
            throw initializer_error("The value of the expression overflowed.");
        }
        catch (const std::invalid_argument &ia)
        {
            throw initializer_error("The value of the expression is invalid.");
        }
        break;
    }
    case FBasicTypeId::UINT16: {
        std::cout << "value = " << value << '\n';
        try
        {
            auto n = std::stoul(value, nullptr);
            if (n < 0 || n > UINT16_MAX)
                throw std::out_of_range("");
        }
        catch (const std::out_of_range &oor)
        {
            throw initializer_error("The value of the expression overflowed.");
        }
        catch (const std::invalid_argument &ia)
        {
            throw initializer_error("The value of the expression is invalid.");
        }
        break;
    }
    case FBasicTypeId::INT32: {
        std::cout << "value = " << value << '\n';
        try
        {
            auto n = std::stoi(value, nullptr);
        }
        catch (const std::out_of_range &oor)
        {
            throw initializer_error("The value of the expression overflowed.");
        }
        catch (const std::invalid_argument &ia)
        {
            throw initializer_error("The value of the expression is invalid.");
        }
        break;
    }
    case FBasicTypeId::UINT32: {
        std::cout << "value = " << value << '\n';
        try
        {
            auto n = std::stoul(value, nullptr);
            uint32_t num = (uint32_t)n;
            if (num != n)
                throw std::out_of_range("");
        }
        catch (const std::out_of_range &oor)
        {
            throw initializer_error("The value of the expression overflowed.");
        }
        catch (const std::invalid_argument &ia)
        {
            throw initializer_error("The value of the expression is invalid.");
        }
        break;
    }
    case FBasicTypeId::INT64: {
        std::cout << "value = " << value << '\n';
        try
        {
            auto n = std::stoll(value, nullptr);
        }
        catch (const std::out_of_range &oor)
        {
            throw initializer_error("The value of the expression overflowed.");
        }
        catch (const std::invalid_argument &ia)
        {
            throw initializer_error("The value of the expression is invalid.");
        }
        break;
    }
    case FBasicTypeId::UINT64: {
        std::cout << "value = " << value << '\n';
        try
        {
            auto n = std::stoull(value, nullptr);
        }
        catch (const std::out_of_range &oor)
        {
            throw initializer_error("The value of the expression overflowed.");
        }
        catch (const std::invalid_argument &ia)
        {
            throw initializer_error("The value of the expression is invalid.");
        }
        break;
    }
    case FBasicTypeId::FLOAT: {
        std::cout << "value = " << value << '\n';
        try
        {
            auto n = std::stof(value, nullptr);
        }
        catch (const std::out_of_range &oor)
        {
            throw initializer_error("The value of the expression overflowed.");
        }
        catch (const std::invalid_argument &ia)
        {
            throw initializer_error("The value of the expression is invalid.");
        }
        break;
    }
    case FBasicTypeId::DOUBLE: {
        std::cout << "value = " << value << '\n';
        try
        {
            auto n = std::stod(value, nullptr);
        }
        catch (const std::out_of_range &oor)
        {
            throw initializer_error("The value of the expression overflowed.");
        }
        catch (const std::invalid_argument &ia)
        {
            throw initializer_error("The value of the expression is invalid.");
        }
        break;
    }
    }
}

void FExpression::boundaryCheck(int64_t upper_bound, int64_t lower_bound, const std::string &value)
{
    std::cout << "value = " << value << '\n';
    try
    {
        auto n = std::stoll(value, nullptr);
        if (n < lower_bound || n > upper_bound)
            throw std::out_of_range("");
    }
    catch (const std::out_of_range &oor)
    {
        throw initializer_error("The value of the expression overflowed.");
    }
    catch (const std::invalid_argument &ia)
    {
        throw initializer_error("The value of the expression is invalid.");
    }
}
} // namespace BstIdl