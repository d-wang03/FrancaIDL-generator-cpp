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
#ifndef F_FLOAT_CONSTANT_H
#define F_FLOAT_CONSTANT_H

#include "FConstant.h"

namespace BstIdl
{

class FFloatConstant : public FConstant
{
public:
    FFloatConstant(float value) : FConstant(), m_value(value)
    {
    }
    FFloatConstant() = default;
    virtual ~FFloatConstant() = default;
    FFloatConstant(const FFloatConstant &) = default;
    FFloatConstant(FFloatConstant &&) noexcept = default;
    FFloatConstant &operator=(const FFloatConstant &) = default;
    FFloatConstant &operator=(FFloatConstant &&) = default;

    float getVal() const
    {
        return m_value;
    }
    void setVal(float value)
    {
        m_value = value;
    }

    void validate(std::shared_ptr<FTypeRef> &type, bool isArray) override
    {
        FConstant::validate(type, isArray);
        auto predefined = type->getPredefined();
        // float can be implicitly converted to double.
        if (!predefined || !FBasicTypeId::isReal(predefined->getValue()))
        {
            if (predefined)
                throw initializer_error("FloatConstant: Initilizer not match type.\nactual type:" +
                                        type->getPredefined()->getName() + "\texpect:   1:float    2:double");
            else
            {
                throw initializer_error(
                    "FloatConstant: Initilizer not match type.\n expect predefined type:   1:float    2:double");
            }
        }
    }

    void EvaluableValidate(std::shared_ptr<FTypeRef> &type, bool isArray, std::string &value, bool is_init_exp) override
    {
        value = toString();
        if (is_init_exp)
        {
            boundaryCheck(FBasicTypeId::FLOAT, value);
        }
    }
    std::string toString() const override
    {
        return std::to_string(getVal());
    }

protected:
    float m_value;
};

} // namespace BstIdl

#endif