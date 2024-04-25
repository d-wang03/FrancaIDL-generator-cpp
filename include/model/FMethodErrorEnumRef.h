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
#ifndef F_METHOD_ERROR_ENUM_REF_H
#define F_METHOD_ERROR_ENUM_REF_H

#include "FEnumerator.h"
#include "FExpression.h"
#include <memory>

namespace BstIdl
{

class FMethodErrorEnumRef : public FExpression
{
public:
    FMethodErrorEnumRef() = default;
    virtual ~FMethodErrorEnumRef() = default;
    FMethodErrorEnumRef(const FMethodErrorEnumRef &) = default;
    FMethodErrorEnumRef(FMethodErrorEnumRef &&) noexcept = default;
    FMethodErrorEnumRef &operator=(const FMethodErrorEnumRef &) = default;
    FMethodErrorEnumRef &operator=(FMethodErrorEnumRef &&) = default;

    std::shared_ptr<FEnumerator> getEnumerator() const
    {
        return m_enumerator;
    }
    void setEnumerator(const std::shared_ptr<FEnumerator> &value)
    {
        m_enumerator = value;
    }
    std::string toString() const override
    {
        return m_enumerator->toString();
    }

protected:
    std::shared_ptr<FEnumerator> m_enumerator;
};

} // namespace BstIdl

#endif