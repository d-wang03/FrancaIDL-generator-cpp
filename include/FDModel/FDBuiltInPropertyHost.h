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
#ifndef F_D_BUILT_IN_PROPETY_HOST_ID_H
#define F_D_BUILT_IN_PROPETY_HOST_ID_H
#include <memory>
#include <string>
namespace BstIdl
{
class FDBuiltInPropertyHost
{
protected:
    uint32_t m_value;
    std::string m_name;
    std::string m_literal;

public:
    enum BuiltInPropertyHost
    {
        UNDEFINED = 0,
        TYPE_COLLECTIONS,
        INTERFACES,
        ATTRIBUTES,
        METHODS,
        BROADCASTS,
        ARGUMENTS,
        STRUCT_FIELDS,
        UNION_FIELDS,
        ARRAYS,
        STRUCTS,
        UNIONS,
        ENUMERATIONS,
        ENUMERATORS,
        TYPEDEFS,
        STRINGS,
        NUMBERS,
        INTEGERS,
        FLOATS,
        BOOLEANS,
        BYTE_BUFFERS,
        FIELDS,
        MAPS,
        MAP_KEYS,
        MAP_VALUES,
        MAX_VALUE
    };
    FDBuiltInPropertyHost(uint32_t value, const std::string &name, const std::string &literal)
        : m_value(value), m_name(name), m_literal(literal)
    {
    }

    FDBuiltInPropertyHost() = default;
    virtual ~FDBuiltInPropertyHost() = default;
    FDBuiltInPropertyHost(const FDBuiltInPropertyHost &) = default;
    FDBuiltInPropertyHost(FDBuiltInPropertyHost &&) noexcept = default;
    FDBuiltInPropertyHost &operator=(const FDBuiltInPropertyHost &) = default;
    FDBuiltInPropertyHost &operator=(FDBuiltInPropertyHost &&) = default;

    static std::shared_ptr<FDBuiltInPropertyHost> get(const std::string &literal);
    static std::shared_ptr<FDBuiltInPropertyHost> getByName(const std::string &name);
    static std::shared_ptr<FDBuiltInPropertyHost> get(uint32_t value);

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