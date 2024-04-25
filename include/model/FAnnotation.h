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
#ifndef F_ANNOTATION_H
#define F_ANNOTATION_H
#include "FAnnotationType.h"

namespace BstIdl
{

class FAnnotation
{
public:
    FAnnotation(const std::string &raw) : m_rawText(raw)
    {
    }
    FAnnotation() = default;
    virtual ~FAnnotation() = default;
    FAnnotation(const FAnnotation &) = default;
    FAnnotation(FAnnotation &&) noexcept = default;
    FAnnotation &operator=(const FAnnotation &) = default;
    FAnnotation &operator=(FAnnotation &&) = default;

    std::string getRawText() const
    {
        return m_rawText;
    }
    void setRawText(const std::string &rawText)
    {
        m_rawText = rawText;
    }
    FAnnotationType getType() const;
    std::string getComment() const;
    void setType(const FAnnotationType &type);
    void setComment(const std::string &comment);

protected:
    std::string m_rawText; // Complete annotation information
};

} // namespace BstIdl

#endif