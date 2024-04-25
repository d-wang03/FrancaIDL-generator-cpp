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
#include "model/FBracketInitializer.h"
#include "model/FArrayType.h"
#include "model/FMapType.h"
#include "model/FTypeRef.h"

namespace BstIdl
{
void FBracketInitializer::validate(std::shared_ptr<FTypeRef> &type, bool isArray)
{
    FInitializer::validate(type, isArray);
    if (isArray)
    {
        for (const auto &item : m_elements)
        {
            auto initializer = item->getFirst();
            if (initializer != nullptr)
                initializer->validate(type, false);
            else
                throw initializer_error("BracketInitializer:Element initilizer has null value.");
        }
    }
    else
    {
        auto derived = type->getDerived();
        if (derived == nullptr)
        {
            auto predefined = type->getPredefined();
            if (predefined)
            {
                throw initializer_error("BracketInitializer :Initilizer not match type .\nactual type:" +
                                        predefined->getName() + "\texpect:   Derived type");
            }
            else
            {
                throw initializer_error("BracketInitializer :Initilizer not match type .\nactual type: "
                                        "IntegerInterval\texpect:   Derived type");
            }
        }
        auto arrayType = std::dynamic_pointer_cast<FArrayType>(derived);
        auto mapType = std::dynamic_pointer_cast<FMapType>(derived);

        if (arrayType != nullptr)
        {
            for (const auto &item : m_elements)
            {
                auto initializer = item->getFirst();
                if (initializer != nullptr)
                {
                    auto tmp_type = arrayType->getElementType();
                    initializer->validate(tmp_type, false);
                }

                else
                    throw initializer_error("BracketInitializer arrayType:Element initilizer has null value.");
            }
        }
        else if (mapType != nullptr)
        {
            for (const auto &item : m_elements)
            {
                auto initializer = item->getFirst();
                if (initializer != nullptr)
                {
                    auto tmp_type = mapType->getKeyType();
                    initializer->validate(tmp_type, false);
                }

                else
                    throw initializer_error("BracketInitializer mapType first: Element initilizer has null value.");

                initializer = item->getSecond();
                if (initializer != nullptr)
                {
                    auto tmp_type = mapType->getValueType();
                    initializer->validate(tmp_type, false);
                }

                else
                    throw initializer_error("BracketInitializer mapType ssecond: Element initilizer has null value.");
            }
        }
        else
            throw initializer_error("BracketInitializer :Initilizer not match type.\nactual type: " +
                                    type->getDerived()->getName() + "expect  :  1:map 2:array");
    }
}
void FBracketInitializer::EvaluableValidate(std::shared_ptr<FTypeRef> &type, bool isArray, std::string &value,
                                            bool is_init_exp)
{
    FInitializer::EvaluableValidate(type, isArray, value, is_init_exp);
    if (isArray)
    {
        for (const auto &item : m_elements)
        {
            auto initializer = item->getFirst();
            if (initializer != nullptr)
                initializer->EvaluableValidate(type, false, value, is_init_exp);
            else
                throw initializer_error("BracketInitializer :Element initilizer has null value.");
        }
    }
    else
    {
        auto derived = type->getDerived();
        if (derived == nullptr)
            throw initializer_error("BracketInitializer :Initilizer not match type.");
        auto arrayType = std::dynamic_pointer_cast<FArrayType>(derived);
        auto mapType = std::dynamic_pointer_cast<FMapType>(derived);

        if (arrayType != nullptr)
        {
            for (const auto &item : m_elements)
            {
                auto initializer = item->getFirst();
                if (initializer != nullptr)
                {
                    auto tmp_type = arrayType->getElementType();
                    initializer->EvaluableValidate(tmp_type, false, value, is_init_exp);
                }
                else
                    throw initializer_error("BracketInitializer :Element initilizer has null value.");
            }
        }
        else if (mapType != nullptr)
        {
            for (const auto &item : m_elements)
            {
                auto initializer = item->getFirst();
                if (initializer != nullptr)
                {
                    auto tmp_type = mapType->getKeyType();
                    initializer->EvaluableValidate(tmp_type, false, value, is_init_exp);
                }

                else
                    throw initializer_error("BracketInitializer first:Element initilizer has null value.");

                initializer = item->getSecond();
                if (initializer != nullptr)
                {
                    auto tmp_type = mapType->getValueType();
                    initializer->EvaluableValidate(tmp_type, false, value, is_init_exp);
                }
                else
                    throw initializer_error("BracketInitializer second:Element initilizer has null value.");
            }
        }
        else
            throw initializer_error("BracketInitializer :Initilizer not match type.\nactual type: " +
                                    type->getDerived()->getName() + "expect  :  1:map 2:array");
    }
}
} // namespace BstIdl