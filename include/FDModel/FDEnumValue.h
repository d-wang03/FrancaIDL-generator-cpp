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
#ifndef F_D_ENUM_VALUE_H
#define F_D_ENUM_VALUE_H
#include "FDElement.h"
#include "model/FEnumerator.h"
namespace BstIdl
{
class FDEnumValue : public FDElement
{
public:
    FDEnumValue() : FDElement()
    {
        setHost("enumerators");
    }
    virtual ~FDEnumValue() = default;
    FDEnumValue(const FDEnumValue &) = default;
    FDEnumValue(FDEnumValue &&) noexcept = default;
    FDEnumValue &operator=(const FDEnumValue &) = default;
    FDEnumValue &operator=(FDEnumValue &&) = default;
    const std::shared_ptr<BstIdl::FEnumerator> getTarget()
    {
        return m_target;
    }

    void setTarget(const std::shared_ptr<BstIdl::FEnumerator> target)
    {
        m_target = target;
        if (getName().empty() && target)
            setName(target->getName());
    }

protected:
    std::shared_ptr<BstIdl::FEnumerator> m_target;
};
} // namespace BstIdl
#endif