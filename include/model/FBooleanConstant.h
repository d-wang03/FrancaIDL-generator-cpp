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
#ifndef F_BOOLEAN_CONSTANT_H
#define F_BOOLEAN_CONSTANT_H

#include "FConstant.h"

namespace BstIdl
{

class FBooleanConstant : public FConstant
{
public:
    FBooleanConstant(bool value) : FConstant(), m_val(value)
    {
    }
    FBooleanConstant() = default;
    virtual ~FBooleanConstant() = default;
    FBooleanConstant(const FBooleanConstant &) = default;
    FBooleanConstant(FBooleanConstant &&) noexcept = default;
    FBooleanConstant &operator=(const FBooleanConstant &) = default;
    FBooleanConstant &operator=(FBooleanConstant &&) = default;

    bool isVal() const
    {
        return m_val;
    }
    void setVal(bool value)
    {
        m_val = value;
    }

    void validate(std::shared_ptr<FTypeRef> &type, bool isArray) override
    {
        FConstant::validate(type, isArray);
        auto predefined = type->getPredefined();
        if (!predefined || predefined->getValue() != FBasicTypeId::BOOLEAN)
        {
            if (predefined)
                throw initializer_error("BooleanConstant: Initilizer not match type.\nactual type:" +
                                        type->getPredefined()->getName() + "\texpect:   Boolean");
            else
            {
                throw initializer_error(
                    "BooleanConstant: Initilizer not match type.\n expect predefined type:   Boolean");
            }
        }
    }
    std::string toString() const override
    {
        if (isVal())
        {
            return "true";
        }
        else
        {
            return "false";
        }
    }

protected:
    bool m_val;
};

} // namespace BstIdl

#endif