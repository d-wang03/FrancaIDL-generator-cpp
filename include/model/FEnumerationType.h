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
#ifndef F_ENUMERATION_TYPE_H
#define F_ENUMERATION_TYPE_H

#include "FEnumerator.h"
#include "FType.h"
#include "fmt/core.h"
#include <list>
#include <memory>

namespace BstIdl
{
class FStructType;
class FUnionType;
class FCompoundType;

class FEnumerationType : public FType
{
public:
    FEnumerationType() = default;
    virtual ~FEnumerationType() = default;
    FEnumerationType(const FEnumerationType &) = default;
    FEnumerationType(FEnumerationType &&) noexcept = default;
    FEnumerationType &operator=(const FEnumerationType &) = default;
    FEnumerationType &operator=(FEnumerationType &&) = default;

    std::list<std::shared_ptr<FEnumerator>> &getEnumerators()
    {
        return m_elements;
    }
    std::list<std::shared_ptr<FEnumerationType>> getBasesList()
    {
        std::list<std::shared_ptr<FEnumerationType>> bases;
        auto base = getBase();
        while (base)
        {
            bases.emplace_back(base);
            base = base->getBase();
        }
        return bases;
    }
    std::shared_ptr<FEnumerationType> getBase() const
    {
        return m_base;
    }
    void setBase(const std::shared_ptr<FEnumerationType> &value)
    {
        m_base = value;
    }
    std::string transform(std::string func) override
    {
        std::string ret = getName();
        auto trans = FObject::s_transMap[func];
        if (trans && trans->forFEnumerationType)
            ret = trans->forFEnumerationType(this);
        return ret;
    }
    std::shared_ptr<FEvaluableElement> findElement(std::list<std::string> &fqn) override;

protected:
    std::list<std::shared_ptr<FEnumerator>> m_elements;
    std::shared_ptr<FEnumerationType> m_base;
};

} // namespace BstIdl

#endif