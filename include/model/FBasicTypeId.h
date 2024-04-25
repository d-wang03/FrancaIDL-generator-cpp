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
#ifndef F_BASIC_TYPE_ID_H
#define F_BASIC_TYPE_ID_H

#include "FTransformer.h"
#include "FType.h"
#include <cstdint>
#include <memory>
#include <string>
namespace BstIdl
{
class FBasicTypeId : public FType
{
public:
    enum BasicTypeId
    {
        UNDEFINED = 0,
        INT8,
        UINT8,
        INT16,
        UINT16,
        INT32,
        UINT32,
        INT64,
        UINT64,
        BOOLEAN,
        STRING,
        FLOAT,
        DOUBLE,
        BYTE_BUFFER,
        BASIC_TYPEID_MAX
    };

    FBasicTypeId(uint32_t value, const std::string &name, const std::string &literal)
        : m_value(value), m_name(name), m_literal(literal)
    {
    }
    FBasicTypeId() = default;
    virtual ~FBasicTypeId() = default;
    FBasicTypeId(const FBasicTypeId &) = default;
    FBasicTypeId(FBasicTypeId &&) noexcept = default;
    FBasicTypeId &operator=(const FBasicTypeId &) = default;
    FBasicTypeId &operator=(FBasicTypeId &&) = default;

    static std::shared_ptr<FBasicTypeId> get(const std::string &literal);
    static std::shared_ptr<FBasicTypeId> getByName(const std::string &name);
    static std::shared_ptr<FBasicTypeId> get(uint32_t value);
    static bool isBoolean(uint32_t value)
    {
        return value == BOOLEAN;
    }
    static bool isInteger(uint32_t value)
    {
        return (value > UNDEFINED && value < BOOLEAN);
    }
    static bool isReal(uint32_t value)
    {
        return (value == DOUBLE || value == FLOAT);
    }
    static bool isNumeric(uint32_t value)
    {
        return isInteger(value) || isReal(value);
    }
    static bool isUnsigned(uint32_t value)
    {
        return value == UINT8 || value == UINT16 || value == UINT32 || value == UINT64;
    }
    static bool isSigned(uint32_t value)
    {
        return value == INT8 || value == INT16 || value == INT32 || value == INT64;
    }
    static bool isString(uint32_t value)
    {
        return value == STRING;
    }
    uint32_t getValue() const
    {
        return m_value;
    }
    std::string getName() const
    {
        return m_name;
    }
    std::string getLiteral() const
    {
        return m_literal;
    }
    std::string transform(std::string func) override
    {
        std::string ret = getName();
        auto trans = FObject::s_transMap[func];
        if (trans && trans->forFBasicTypeId)
            ret = trans->forFBasicTypeId(this);
        return ret;
    }

protected:
    uint32_t m_value;
    std::string m_name;
    std::string m_literal;
};
} // namespace BstIdl

#endif