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
#ifndef F_STRUCT_TYPE_H
#define F_STRUCT_TYPE_H

#include "FCompoundType.h"
#include "FTypeRef.h"
#include "fmt/core.h"
#include <memory>

namespace BstIdl
{
class FEnumerationType;
class FStructType : public FCompoundType
{
public:
    FStructType() = default;
    virtual ~FStructType() = default;
    FStructType(const FStructType &) = default;
    FStructType(FStructType &&) noexcept = default;
    FStructType &operator=(const FStructType &) = default;
    FStructType &operator=(FStructType &&) = default;
    std::list<std::shared_ptr<FStructType>> getBasesList()
    {
        std::list<std::shared_ptr<FStructType>> bases;
        auto base = getBase();
        while (base)
        {
            bases.emplace_back(base);
            base = base->getBase();
        }
        return bases;
    }
    std::shared_ptr<FStructType> getBase() const
    {
        return m_base;
    }
    void setBase(const std::shared_ptr<FStructType> &value)
    {
        m_base = value;
    }
    bool isPolymorphic() const
    {
        return m_isPolymorphic;
    }
    void setPolymorphic(bool value)
    {
        m_isPolymorphic = value;
    }
    std::string transform(std::string func) override
    {
        std::string ret = getName();
        auto trans = FObject::s_transMap[func];
        if (trans && trans->forFStructType)
            ret = trans->forFStructType(this);
        return ret;
    }
    std::shared_ptr<FEvaluableElement> findElement(std::list<std::string> &fqn) override;

protected:
    std::shared_ptr<FStructType> m_base;
    bool m_isPolymorphic;
};

} // namespace BstIdl

#endif