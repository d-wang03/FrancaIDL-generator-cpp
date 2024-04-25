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
#ifndef F_D_PROPERTY_FLAG_H
#define F_D_PROPERTY_FLAG_H
#include "FDComplexValue.h"
#include <iostream>
namespace BstIdl
{
class FDPropertyFlag
{
protected:
    bool m_optional;
    std::shared_ptr<FDComplexValue> m_default;

public:
    FDPropertyFlag() : m_optional(false), m_default(nullptr)
    {
    }
    virtual ~FDPropertyFlag() = default;
    FDPropertyFlag(const FDPropertyFlag &) = default;
    FDPropertyFlag(FDPropertyFlag &&) noexcept = default;
    FDPropertyFlag &operator=(const FDPropertyFlag &) = default;
    FDPropertyFlag &operator=(FDPropertyFlag &&) = default;
    bool isOptional() const
    {
        return m_optional;
    }
    void setOptional(bool optional)
    {
        m_optional = optional;
    }
    bool isDefault() const
    {
        return m_default ? true : false;
    }
    std::shared_ptr<FDComplexValue> getDefault() const
    {
        return m_default;
    }
    void setDefault(const std::shared_ptr<FDComplexValue> &newDefault)
    {
        m_default = newDefault;
    }
    std::string getName() const
    {
        if (m_optional)
            return "optional";
        if (m_default)
            return "default=" + m_default->toString();
        return "Unknown";
    }
};
} // namespace BstIdl
#endif