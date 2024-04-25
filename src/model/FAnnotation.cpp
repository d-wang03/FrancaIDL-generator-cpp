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
#include "model/FAnnotation.h"
#include "model/FAnnotationType.h"
#include "utilities/string_utility.h"

namespace BstIdl
{
class FAnnotationAux
{
private:
    static const char SEP = ':';
    static std::string buildRawText(const FAnnotationType &type, const std::string &comment)
    {
        auto literal = type.getLiteral();
        literal = literal.empty() ? "@undefined" : literal;
        return literal + " : " + comment;
    }

public:
    static FAnnotationType getType(const std::string &raw)
    {
        if (!raw.empty())
        {
            std::string::size_type sep = raw.find_first_of(SEP);
            if (sep != std::string::npos)
            {
                auto literal = raw.substr(0, sep);
                return FAnnotationType::get(trim(literal));
            }
        }
        return FAnnotationType();
    }

    static std::string getComment(const std::string &raw)
    {
        if (!raw.empty())
        {
            auto sep = raw.find_first_of(SEP);
            if (sep != std::string::npos)
            {
                auto comment = raw.substr(sep);
                comment.erase(0, 1);
                return trim(comment);
            }
        }
        return std::string();
    }

    static void setType(FAnnotation &host, const FAnnotationType &type)
    {
        std::string comment = getComment(host.getRawText());
        host.setRawText(buildRawText(type, comment));
    }

    static void setComment(FAnnotation &host, const std::string &comment)
    {
        auto type = getType(host.getRawText());
        host.setRawText(buildRawText(type, comment));
    }
};

FAnnotationType FAnnotation::getType() const
{
    return FAnnotationAux::getType(m_rawText);
}

std::string FAnnotation::getComment() const
{
    return FAnnotationAux::getComment(m_rawText);
}

void FAnnotation::setType(const FAnnotationType &type)
{
    FAnnotationAux::setType(*this, type);
}

void FAnnotation::setComment(const std::string &comment)
{
    FAnnotationAux::setComment(*this, comment);
}
} // namespace BstIdl