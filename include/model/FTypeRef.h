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
#ifndef F_TYPE_REF_H
#define F_TYPE_REF_H

#include "FBasicTypeId.h"
#include "FIntegerInterval.h"

#include <memory>

namespace BstIdl
{
class FTypeRef : public FObject
{
public:
    FTypeRef() = default;
    virtual ~FTypeRef() = default;
    FTypeRef(const FTypeRef &) = default;
    FTypeRef(FTypeRef &&) noexcept = default;
    FTypeRef &operator=(const FTypeRef &) = default;
    FTypeRef &operator=(FTypeRef &&) = default;

    std::shared_ptr<FBasicTypeId> getPredefined() const
    {
        return m_predefined;
    }
    void setPredefined(const std::shared_ptr<FBasicTypeId> &value)
    {
        m_predefined = value;
        setName(m_predefined->getName());
    }

    std::shared_ptr<FType> getDerived() const
    {
        return m_derived;
    }
    void setDerived(const std::shared_ptr<FType> &value)
    {
        m_derived = value;
        if (value)
            setName(value->getName());
    }

    std::shared_ptr<FIntegerInterval> getInterval() const
    {
        return m_interval;
    }
    void setInterval(const std::shared_ptr<FIntegerInterval> &value)
    {
        m_interval = value;
        setName(value->getName());
    }

    std::string toString() const
    {
        return getName();
    }
    bool isPolymorphic();
    std::string transform(std::string func) override
    {
        std::string ret;
        auto trans = FObject::s_transMap[func];
        if (trans && trans->forFTypeRef)
            ret = trans->forFTypeRef(this);
        else
        {
            auto basic_type = getPredefined();
            auto derived_type = getDerived();
            auto interval_type = getInterval();
            if (basic_type)
            {
                return basic_type->transform(func);
            }
            else if (derived_type)
            {
                return derived_type->transform(func);
            }
            else if (interval_type)
            {
                return interval_type->transform("interval_type_" + func);
            }
        }
        return ret;
    }
    // for model validating
    std::string toStringRecursively() const
    {
        if (getPredefined())
            return getName();
        else if (getInterval())
            return m_interval->toString();
        else if (getDerived())
        {
            return m_derived->getFQN();
        }
        else
            return std::string();
    }

protected:
    std::shared_ptr<FBasicTypeId> m_predefined;
    std::shared_ptr<FType> m_derived;
    std::shared_ptr<FIntegerInterval> m_interval;
};

} // namespace BstIdl

#endif