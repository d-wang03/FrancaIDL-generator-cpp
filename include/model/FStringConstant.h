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
#ifndef F_STRING_CONSTANT_H
#define F_STRING_CONSTANT_H

#include "FConstant.h"

#include <string>

namespace BstIdl
{

class FStringConstant : public FConstant
{
public:
    FStringConstant() = default;
    virtual ~FStringConstant() = default;
    FStringConstant(const FStringConstant &) = default;
    FStringConstant(FStringConstant &&) noexcept = default;
    FStringConstant &operator=(const FStringConstant &) = default;
    FStringConstant &operator=(FStringConstant &&) = default;

    std::string getVal() const
    {
        return m_value;
    }
    void setVal(std::string value)
    {
        m_value = value;
    }

    void validate(std::shared_ptr<FTypeRef> &type, bool isArray) override
    {
        FConstant::validate(type, isArray);
        auto predefined = type->getPredefined();
        if (!predefined || predefined->getValue() != FBasicTypeId::STRING)
        {
            if (predefined)
                throw initializer_error("StringConstant: Initilizer not match type.\nactual type:" +
                                        type->getPredefined()->getName() + "\texpect:   String");
            else
            {
                throw initializer_error("StringConstant: Initilizer not match type.\n expect predefined type:String");
            }
        }
    }

    std::string toString() const override
    {
        return "\"" + getVal() + "\"";
    }

protected:
    std::string m_value;
};

} // namespace BstIdl

#endif