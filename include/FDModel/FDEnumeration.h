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
#ifndef F_D_ENUMERATION_H
#define F_D_ENUMERATION_H
#include "FDEnumValue.h"
#include "FDTypeDefinition.h"
#include "model/FEnumerationType.h"
namespace BstIdl
{
class FDEnumeration : public FDTypeDefinition
{
public:
    FDEnumeration() : FDElement(), FDTypeDefinition()
    {
        setHost("enumerations");
    }
    virtual ~FDEnumeration() = default;
    FDEnumeration(const FDEnumeration &) = default;
    FDEnumeration(FDEnumeration &&) noexcept = default;
    FDEnumeration &operator=(const FDEnumeration &) = default;
    FDEnumeration &operator=(FDEnumeration &&) = default;

    std::shared_ptr<BstIdl::FEnumerationType> getTarget() const
    {
        return m_target;
    }

    void setTarget(const std::shared_ptr<BstIdl::FEnumerationType> target)
    {
        m_target = target;
        if (getName().empty() && target)
            setName(target->getName());
    }

    std::list<std::shared_ptr<FDEnumValue>> &getEnumerators()
    {
        return m_enumrators;
    }
    void setEnumerators(const std::shared_ptr<FDEnumValue> enumrator)
    {
        m_enumrators.emplace_back(enumrator);
    }

protected:
    std::shared_ptr<BstIdl::FEnumerationType> m_target;
    std::list<std::shared_ptr<FDEnumValue>> m_enumrators;
};

} // namespace BstIdl

#endif