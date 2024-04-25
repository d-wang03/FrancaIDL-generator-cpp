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
#ifndef F_D_INTEGER_H
#define F_D_INTEGER_H
#include "FDValue.h"
#include <iostream>
namespace BstIdl
{
class FDInteger : public FDValue
{
public:
    FDInteger() = default;
    virtual ~FDInteger() = default;
    FDInteger(const FDInteger &) = default;
    FDInteger(FDInteger &&) noexcept = default;
    FDInteger &operator=(const FDInteger &) = default;
    FDInteger &operator=(FDInteger &&) = default;

    int const getValue()
    {
        return m_value;
    }
    void setValue(const int &newValue)
    {
        m_value = newValue;
    }
    std::string const getFormattedValue()
    {
        return m_formattedValue;
    }

    void setFormattedValue(std::string &newFormattedValue)
    {
        m_formattedValue = newFormattedValue;
    }

    bool isInteger() const override
    {
        return true;
    }

    int getInteger() const override
    {
        return m_value;
    }

protected:
    int m_value;
    std::string m_formattedValue;
};
} // namespace BstIdl
#endif