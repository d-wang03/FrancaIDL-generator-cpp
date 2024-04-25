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
#ifndef F_INTEGER_CONSTANT_H
#define F_INTEGER_CONSTANT_H

#include "FBasicTypeId.h"
#include "FConstant.h"
#include "FTypeRef.h"
#include <cstdint>
#include <sstream>

namespace BstIdl
{

class FIntegerConstant : public FConstant
{
public:
    FIntegerConstant() = default;
    virtual ~FIntegerConstant() = default;
    FIntegerConstant(const FIntegerConstant &) = default;
    FIntegerConstant(FIntegerConstant &&) noexcept = default;
    FIntegerConstant &operator=(const FIntegerConstant &) = default;
    FIntegerConstant &operator=(FIntegerConstant &&) = default;

    int64_t getVal() const
    {
        return m_value;
    }
    void setVal(int64_t value)
    {
        m_value = value;
    }

    uint64_t getUnsignedVal() const
    {
        return m_unsignedValue;
    }
    void setUnsignedVal(uint64_t value)
    {
        m_unsignedValue = value;
    }

    void validate(std::shared_ptr<FTypeRef> &type, bool isArray) override
    {
        FConstant::validate(type, isArray);
        auto predefined = type->getPredefined();
        auto interval = type->getInterval();
        if (predefined && interval)
            throw initializer_error(
                "IntegerConstant : Invalid type reference.\tpredefined and interval are in conflict");

        uint32_t typeID = predefined ? predefined->getValue() : FBasicTypeId::INT64;
        // float can be implicitly converted to double.
        if (!predefined || !FBasicTypeId::isNumeric(typeID))
        {
            if (predefined)
                throw initializer_error("IntegerConstant: Initilizer not match type.\nactual type:" +
                                        type->getPredefined()->getName() + "\texpect:   Numeric type");
            else
            {
                throw initializer_error("IntegerConstant: Initilizer not match type.\n expect predefined Numeric type");
            }
        }

        if (m_value < 0 && FBasicTypeId::isUnsigned(typeID))
        {
            throw initializer_error("IntegerConstant: Initilizer not match type.\nactual type:" +
                                    type->getPredefined()->getName() + "\texpect:   signed type");
        }
    }
    void EvaluableValidate(std::shared_ptr<FTypeRef> &type, bool isArray, std::string &value, bool is_init_exp) override
    {
        FConstant::EvaluableValidate(type, isArray, value, is_init_exp);
        value = toString();
        uint32_t typeID;
        auto predefined = type->getPredefined();
        auto interval = type->getInterval();
        if (predefined)
        {
            typeID = predefined->getValue();
            if (is_init_exp)
            {
                boundaryCheck(typeID, value);
            }
        }
        if (interval)
        {
            if (is_init_exp)
            {
                boundaryCheck(interval->getUpperBound(), interval->getLowerBound(), value);
            }
        }
    }
    std::string toString() const override
    {
        if (m_value == 0)
            return std::to_string(m_unsignedValue);
        else
            return std::to_string(m_value);
    }

protected:
    int64_t m_value;
    uint64_t m_unsignedValue;
};

} // namespace BstIdl

#endif