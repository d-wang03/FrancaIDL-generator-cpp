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
#ifndef F_D_PREDEFINED_TYPE_ID_H
#define F_D_PREDEFINED_TYPE_ID_H
#include <memory>
#include <string>
namespace BstIdl
{
class FDPredefinedTypeId
{
protected:
    uint32_t m_value;
    std::string m_name;
    std::string m_literal;

public:
    enum PredefinedTypeId
    {
        UNDEFINED = 0,
        INTEGER,
        STRING,
        BOOLEAN,
        INTERFACE
    };
    FDPredefinedTypeId(uint32_t value, const std::string &name, const std::string &literal)
        : m_value(value), m_name(name), m_literal(literal)
    {
    }

    FDPredefinedTypeId() = default;
    virtual ~FDPredefinedTypeId() = default;
    FDPredefinedTypeId(const FDPredefinedTypeId &) = default;
    FDPredefinedTypeId(FDPredefinedTypeId &&) noexcept = default;
    FDPredefinedTypeId &operator=(const FDPredefinedTypeId &) = default;
    FDPredefinedTypeId &operator=(FDPredefinedTypeId &&) = default;

    static std::shared_ptr<FDPredefinedTypeId> get(const std::string &literal);
    static std::shared_ptr<FDPredefinedTypeId> getByName(const std::string &name);
    static std::shared_ptr<FDPredefinedTypeId> get(uint32_t value);

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
};

} // namespace BstIdl
#endif