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
#ifndef F_D_STRING_H
#define F_D_STRING_H
#include "FDValue.h"
#include <iostream>
namespace BstIdl
{
class FDString : public FDValue
{
public:
    FDString() = default;
    virtual ~FDString() = default;
    FDString(const FDString &) = default;
    FDString(FDString &&) noexcept = default;
    FDString &operator=(const FDString &) = default;
    FDString &operator=(FDString &&) = default;

    std::string const getValue()
    {
        return m_value;
    }
    void setValue(const std::string &newValue)
    {
        m_value = newValue;
    }

    bool isString() const override
    {
        return true;
    }

    std::string getString() const override
    {
        return m_value;
    }

protected:
    std::string m_value;
};
} // namespace BstIdl
#endif