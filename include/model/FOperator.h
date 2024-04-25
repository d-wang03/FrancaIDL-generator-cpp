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
#ifndef F_OPERATOR_H
#define F_OPERATOR_H

#include <cstdint>
#include <memory>
#include <string>

namespace BstIdl
{
class FOperator
{
public:
    enum Operator
    {
        UNDEFINED = 0,
        OR,
        EQUAL,
        UNEQUAL,
        AND,
        SMALLER,
        SMALLER_OR_EQUAL,
        GREATER_OR_EQUAL,
        GREATER,
        ADDITION,
        SUBTRACTION,
        MULTIPLICATION,
        DIVISION,
        NEGATION,
        OPERATOR_MAX
    };

    FOperator(uint32_t value, const std::string &name, const std::string &literal)
        : m_value(value), m_name(name), m_literal(literal)
    {
    }
    FOperator() = default;
    virtual ~FOperator() = default;
    FOperator(const FOperator &) = default;
    FOperator(FOperator &&) noexcept = default;
    FOperator &operator=(const FOperator &) = default;
    FOperator &operator=(FOperator &&) = default;

    static std::shared_ptr<FOperator> get(const std::string &literal);
    static std::shared_ptr<FOperator> getByName(const std::string &name);
    static std::shared_ptr<FOperator> get(uint32_t value);

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

protected:
    uint32_t m_value;
    std::string m_name;
    std::string m_literal;
};
} // namespace BstIdl

#endif