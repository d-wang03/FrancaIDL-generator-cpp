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
#ifndef MSG_TYPE_H
#define MSG_TYPE_H
#include <memory>
#include <string>

namespace BstASF
{
class MsgType
{
protected:
    uint32_t m_value;
    std::string m_name;
    std::string m_literal;

public:
    enum Type
    {
        Method,
        Broadcast,
        Attribute,
        Reply,
        AvailStatus,
        MAX_VALUE
    };

    MsgType(const uint32_t &value, const std::string &name, const std::string &literal)
        : m_value(value), m_name(name), m_literal(literal)
    {
    }

    MsgType() = default;
    virtual ~MsgType() = default;
    MsgType(const MsgType &) = default;
    MsgType(MsgType &&) noexcept = default;
    MsgType &operator=(const MsgType &) = default;
    MsgType &operator=(MsgType &&) = default;

    static std::shared_ptr<MsgType> get(const std::string &literal);
    static std::shared_ptr<MsgType> getByName(const std::string &name);
    static std::shared_ptr<MsgType> get(uint32_t value);

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

} // namespace BstASF
#endif