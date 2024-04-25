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
#ifndef F_ANNOTATION_TYPE_H
#define F_ANNOTATION_TYPE_H

#include <cstdint>
#include <string>
namespace BstIdl
{
class FAnnotationType
{
public:
    enum predefinedAnnotations
    {
        UNDEFINED = 0,
        DESCRIPTION,
        AUTHOR,
        EXPERIMENTAL,
        DEPRECATED,
        SEE,
        PARAM_VALUE,
        HIGH_VOLUME_VALUE,
        HIGH_FREQUENCY_VALUE,
        SOURCE_URI_VALUE,
        SOURCE_ALIAS_VALUE,
        DETAILS_VALUE,
        RETURN_VALUE,
        PREDEFINED_MAX
    };

    FAnnotationType(uint32_t value, const std::string &name, const std::string &literal)
        : m_value(value), m_name(name), m_literal(literal)
    {
    }
    FAnnotationType() = default;
    virtual ~FAnnotationType() = default;
    FAnnotationType(const FAnnotationType &) = default;
    FAnnotationType(FAnnotationType &&) noexcept = default;
    FAnnotationType &operator=(const FAnnotationType &) = default;
    FAnnotationType &operator=(FAnnotationType &&) = default;

    static FAnnotationType get(const std::string &literal);
    static FAnnotationType getByName(const std::string &name);
    static FAnnotationType get(uint32_t value);

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