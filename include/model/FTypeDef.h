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
#ifndef F_TYPE_DEF_H
#define F_TYPE_DEF_H

#include "FType.h"
#include "FTypeRef.h"
#include <memory>

namespace BstIdl
{

class FTypeDef : public FType
{
public:
    FTypeDef() = default;
    virtual ~FTypeDef() = default;
    FTypeDef(const FTypeDef &) = default;
    FTypeDef(FTypeDef &&) noexcept = default;
    FTypeDef &operator=(const FTypeDef &) = default;
    FTypeDef &operator=(FTypeDef &&) = default;

    std::shared_ptr<FTypeRef> getActualType() const
    {
        return m_actualType;
    }
    void setActualType(const std::shared_ptr<FTypeRef> &value)
    {
        m_actualType = value;
    }
    std::string transform(std::string func) override
    {
        std::string ret = getName();
        auto trans = FObject::s_transMap[func];
        if (trans && trans->forFTypeDef)
            ret = trans->forFTypeDef(this);
        return ret;
    }
    std::shared_ptr<FEvaluableElement> findElement(std::list<std::string> &fqn) override
    {
        if (fqn.empty() || !m_actualType)
            return nullptr;

        auto type = m_actualType->getDerived();
        return type ? type->findElement(fqn) : nullptr;
    }

protected:
    std::shared_ptr<FTypeRef> m_actualType;
};

} // namespace BstIdl

#endif