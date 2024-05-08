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
#include "model/FCompoundInitializer.h"
#include "model/FStructType.h"
#include "model/FUnionType.h"
#include <algorithm>

namespace BstIdl
{
std::shared_ptr<FFieldInitializer> FCompoundInitializer::findElement(const std::string &_field_name) const
{
    auto iter = std::find_if(m_elements.cbegin(), m_elements.cend(),
                             [&_field_name](auto ele) { return ele->getElementName() == _field_name; });
    if (iter == m_elements.cend())
        return nullptr;
    return (*iter);
}

void FCompoundInitializer::validate(std::shared_ptr<FTypeRef> &type, bool isArray)
{
    FInitializer::validate(type, isArray);
    if (isArray)
        throw initializer_error("CompoundInitializer:Initilizer not match type.\nactual type: " +
                                type->getDerived()->getName() + "[] expect  :  derived type");

    auto derived = type->getDerived();
    std::list<std::shared_ptr<FField>> elements;
    auto structType = std::dynamic_pointer_cast<FStructType>(derived);
    if (structType != nullptr)
    {
        elements = structType->getElements();
    }
    auto unionType = std::dynamic_pointer_cast<FUnionType>(derived);
    if (unionType != nullptr)
    {
        elements = unionType->getElements();
    }
    if (unionType == nullptr && structType == nullptr)
        throw initializer_error(
            "CompoundInitializer : Invalid type reference.\tunionType and structType are in conflict");

    for (const auto &item : m_elements)
    {
        auto name = item->getElement()->getName();
        auto iter = std::find_if(elements.cbegin(), elements.cend(), [name, &item](const auto &element) {
            if (element->getName() == name)
            {
                auto initilizer = item->getValue();
                if (initilizer != nullptr)
                {
                    auto tmp_type = element->getType();
                    initilizer->validate(tmp_type, element->isArray());
                }

                else
                    throw initializer_error("CompoundInitializer : Field initilizer has null value.");
                return true;
            }
            else
            {
                return false;
            }
        });
        if (iter == elements.cend())
            throw initializer_error("CompoundInitializer : Initilizer not match type.");
    }
}

void FCompoundInitializer::EvaluableValidate(std::shared_ptr<FTypeRef> &type, bool isArray, std::string &value,
                                             bool is_init_exp)
{
    FInitializer::EvaluableValidate(type, isArray, value, is_init_exp);
    if (isArray)
        throw initializer_error("CompoundInitializer:Initilizer not match type.\nactual type: " +
                                type->getDerived()->getName() + "[] expect  :  derived type");
    auto derived = type->getDerived();
    std::list<std::shared_ptr<FField>> elements;
    auto structType = std::dynamic_pointer_cast<FStructType>(derived);
    if (structType != nullptr)
    {
        elements = structType->getElements();
    }
    auto unionType = std::dynamic_pointer_cast<FUnionType>(derived);
    if (unionType != nullptr)
    {
        elements = unionType->getElements();
    }

    for (const auto &item : m_elements) // struct/union 's elements
    {
        auto name = item->getElementName();
        auto iter =
            std::find_if(elements.cbegin(), elements.cend(), [name, &item, &value, &is_init_exp](const auto &element) {
                if (element->getName() == name)
                {
                    auto initilizer = item->getValue();
                    if (initilizer != nullptr)
                    {
                        auto tmp_type = element->getType();
                        initilizer->EvaluableValidate(tmp_type, false, value, is_init_exp);
                    }

                    else
                        throw initializer_error("CompoundInitializer : Field initilizer has null value.");
                    return true;
                }
                else
                {
                    return false;
                }
            });
        if (iter == elements.cend())
            throw initializer_error("CompoundInitializer : Initilizer not match type.");
    }
}
} // namespace BstIdl