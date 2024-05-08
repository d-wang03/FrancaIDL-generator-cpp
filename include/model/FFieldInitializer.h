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
#ifndef F_FIELD_INITIALIZER_H
#define F_FIELD_INITIALIZER_H

#include "FField.h"
#include "FInitializerExpression.h"
#include <memory>

namespace BstIdl
{

class FFieldInitializer
{
public:
    FFieldInitializer() = default;
    virtual ~FFieldInitializer() = default;
    FFieldInitializer(const FFieldInitializer &) = default;
    FFieldInitializer(FFieldInitializer &&) noexcept = default;
    FFieldInitializer &operator=(const FFieldInitializer &) = default;
    FFieldInitializer &operator=(FFieldInitializer &&) = default;

    std::shared_ptr<FField> getElement() const
    {
        return m_element;
    }
    void setElement(const std::shared_ptr<FField> &value)
    {
        m_element = value;
    }
    std::string getElementName() const
    {
        if (m_element)
            return m_element->getName();
        return std::string();
    }
    std::shared_ptr<FInitializerExpression> getValue() const
    {
        return m_value;
    }
    void setValue(const std::shared_ptr<FInitializerExpression> &value)
    {
        m_value = value;
    }

protected:
    std::shared_ptr<FField> m_element;
    std::shared_ptr<FInitializerExpression> m_value;
};

} // namespace BstIdl

#endif