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
#include "model/FAnnotationType.h"

namespace BstIdl
{
static const FAnnotationType s_annotationTable[] = {
    {FAnnotationType::UNDEFINED, "", ""},
    {FAnnotationType::DESCRIPTION, "description", "@description"},
    {FAnnotationType::AUTHOR, "author", "@author"},
    {FAnnotationType::EXPERIMENTAL, "experimental", "@experimental"},
    {FAnnotationType::DEPRECATED, "deprecated", "@deprecated"},
    {FAnnotationType::SEE, "see", "@see"},
    {FAnnotationType::PARAM_VALUE, "param", "@param"},
    {FAnnotationType::HIGH_VOLUME_VALUE, "high_volume", "@high-volume"},
    {FAnnotationType::HIGH_FREQUENCY_VALUE, "high_frequency", "@high-frequency"},
    {FAnnotationType::SOURCE_URI_VALUE, "source_uri", "@source-uri"},
    {FAnnotationType::SOURCE_ALIAS_VALUE, "source_alias", "@source-alias"},
    {FAnnotationType::DETAILS_VALUE, "details", "@details"},
    {FAnnotationType::RETURN_VALUE, "return", "@return"},
};

FAnnotationType FAnnotationType::get(const std::string &literal)
{
    for (int i = 0; i < PREDEFINED_MAX; ++i)
    {
        if (!s_annotationTable[i].getLiteral().compare(literal))
            return s_annotationTable[i];
    }

    return s_annotationTable[UNDEFINED];
}

FAnnotationType FAnnotationType::getByName(const std::string &name)
{
    for (int i = 0; i < PREDEFINED_MAX; ++i)
    {
        if (!s_annotationTable[i].getName().compare(name))
            return s_annotationTable[i];
    }

    return s_annotationTable[UNDEFINED];
}

FAnnotationType FAnnotationType::get(uint32_t value)
{
    if (value < PREDEFINED_MAX)
        return s_annotationTable[value];
    else
        return s_annotationTable[UNDEFINED];
}

} // namespace BstIdl