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
#ifndef F_TYPED_ELEMENT_H
#define F_TYPED_ELEMENT_H

#include "FEvaluableElement.h"
#include "FTypeRef.h"

namespace BstIdl
{
class FExpression;
class FTypedElement : public FEvaluableElement
{
public:
    FTypedElement() = default;
    virtual ~FTypedElement() = default;
    FTypedElement(const FTypedElement &) = default;
    FTypedElement(FTypedElement &&) noexcept = default;
    FTypedElement &operator=(const FTypedElement &) = default;
    FTypedElement &operator=(FTypedElement &&) = default;

    std::shared_ptr<FTypeRef> getType() const
    {
        return m_typeRef;
    }
    void setType(const std::shared_ptr<FTypeRef> &value)
    {
        m_typeRef = value;
    }
    bool isArray() const
    {
        return m_isArray;
    }
    void setArray(bool value)
    {
        m_isArray = value;
    }
    bool isFixedSize() const
    {
        return m_isFixedSize;
    }
    std::shared_ptr<FExpression> getFixedSize() const
    {
        return m_fixedSize;
    }
    void setFixedSize(const std::shared_ptr<FExpression> &size)
    {
        if (size)
        {
            m_isFixedSize = true;
            m_fixedSize = size;
        }
        else
        {
            m_isFixedSize = false;
            m_fixedSize = nullptr;
        }
    }
    std::string toString() const
    {
        return m_isArray ? m_typeRef->toString() + "[]" : m_typeRef->toString();
    }

protected:
    std::shared_ptr<FTypeRef> m_typeRef;
    bool m_isArray;
    bool m_isFixedSize;
    std::shared_ptr<FExpression> m_fixedSize;
};

} // namespace BstIdl

#endif