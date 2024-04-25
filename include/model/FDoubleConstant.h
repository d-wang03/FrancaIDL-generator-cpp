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
#ifndef F_DOUBLE_CONSTANT_H
#define F_DOUBLE_CONSTANT_H

#include "FConstant.h"
#include <cfloat>

namespace BstIdl
{

class FDoubleConstant : public FConstant
{
public:
    FDoubleConstant(double value) : FConstant(), m_value(value)
    {
    }
    FDoubleConstant() = default;
    virtual ~FDoubleConstant() = default;
    FDoubleConstant(const FDoubleConstant &) = default;
    FDoubleConstant(FDoubleConstant &&) noexcept = default;
    FDoubleConstant &operator=(const FDoubleConstant &) = default;
    FDoubleConstant &operator=(FDoubleConstant &&) = default;

    double getVal() const
    {
        return m_value;
    }
    void setVal(double value)
    {
        m_value = value;
    }

    void validate(std::shared_ptr<FTypeRef> &type, bool isArray) override
    {
        FConstant::validate(type, isArray);
        auto predefined = type->getPredefined();
        if (!predefined || predefined->getValue() != FBasicTypeId::DOUBLE)
        {
            if (predefined)
                throw initializer_error("DoubleConstant: Initilizer not match type.\nactual type:" +
                                        type->getPredefined()->getName() + "\texpect:   double");
            else
            {
                throw initializer_error(
                    "DoubleConstant: Initilizer not match type.\n expect predefined type:   double");
            }
        }
    }
    void EvaluableValidate(std::shared_ptr<FTypeRef> &type, bool isArray, std::string &value, bool is_init_exp) override
    {
        value = toString();
        if (is_init_exp)
        {
            boundaryCheck(FBasicTypeId::DOUBLE, value);
        }
    }
    std::string toString() const override
    {
        return std::to_string(getVal());
    }

protected:
    double m_value;
};

} // namespace BstIdl

#endif