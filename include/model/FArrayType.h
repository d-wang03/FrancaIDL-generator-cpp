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
#ifndef F_ARRAY_TYPE_H
#define F_ARRAY_TYPE_H

#include "FType.h"
#include "FTypeRef.h"

namespace BstIdl
{
class FStructType;
class FUnionType;
class FEnumerationType;
class FEvaluableElement;
class FExpression;
class FArrayType : public FType
{
public:
    FArrayType() = default;
    virtual ~FArrayType() = default;
    FArrayType(const FArrayType &) = default;
    FArrayType(FArrayType &&) noexcept = default;
    FArrayType &operator=(const FArrayType &) = default;
    FArrayType &operator=(FArrayType &&) = default;

    std::shared_ptr<FTypeRef> getElementType() const
    {
        return m_elementType;
    }
    void setElementType(const std::shared_ptr<FTypeRef> &value)
    {
        m_elementType = value;
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

    std::string transform(std::string func) override
    {
        std::string ret = getName();
        auto trans = FObject::s_transMap[func];
        if (trans && trans->forFArrayType)
            ret = trans->forFArrayType(this);
        return ret;
    }
    std::shared_ptr<FEvaluableElement> findElement(std::list<std::string> &fqn) override
    {
        if (fqn.empty() || !m_elementType)
            return nullptr;

        auto type = m_elementType->getDerived();
        return type ? type->findElement(fqn) : nullptr;
    }

protected:
    std::shared_ptr<FTypeRef> m_elementType;
    bool m_isFixedSize;
    std::shared_ptr<FExpression> m_fixedSize;
};

} // namespace BstIdl

#endif