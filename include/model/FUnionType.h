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
#ifndef F_UNION_TYPE_H
#define F_UNION_TYPE_H

#include "FCompoundType.h"
#include "FTypeRef.h"
#include "fmt/core.h"
#include <memory>

namespace BstIdl
{
class FEnumerationType;

class FUnionType : public FCompoundType
{
public:
    FUnionType() = default;
    virtual ~FUnionType() = default;
    FUnionType(const FUnionType &) = default;
    FUnionType(FUnionType &&) noexcept = default;
    FUnionType &operator=(const FUnionType &) = default;
    FUnionType &operator=(FUnionType &&) = default;
    std::list<std::shared_ptr<FUnionType>> getBasesList()
    {
        std::list<std::shared_ptr<FUnionType>> bases;
        auto base = getBase();
        while (base)
        {
            bases.emplace_back(base);
            base = base->getBase();
        }
        return bases;
    }
    std::shared_ptr<FUnionType> getBase() const
    {
        return m_base;
    }
    void setBase(const std::shared_ptr<FUnionType> &value)
    {
        m_base = value;
    }
    std::string transform(std::string func) override
    {
        std::string ret = getName();
        auto trans = FObject::s_transMap[func];
        if (trans && trans->forFUnionType)
            ret = trans->forFUnionType(this);
        return ret;
    }
    std::shared_ptr<FEvaluableElement> findElement(std::list<std::string> &fqn) override;

protected:
    std::shared_ptr<FUnionType> m_base;
};

} // namespace BstIdl

#endif