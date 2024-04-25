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
#ifndef F_D_ENUMERATION_OVERWRITES_H
#define F_D_ENUMERATION_OVERWRITES_H
#include "FDEnumValue.h"
#include "FDTypeOverwrites.h"
namespace BstIdl
{
class FDEnumerationOverwrites : public FDTypeOverwrites
{
protected:
    std::list<std::shared_ptr<FDEnumValue>> enumerators;

public:
    FDEnumerationOverwrites(std::shared_ptr<FDEnumValue> enumerator)
    {
        enumerators.emplace_back(enumerator);
    }
    FDEnumerationOverwrites() = default;
    virtual ~FDEnumerationOverwrites() = default;
    FDEnumerationOverwrites(const FDEnumerationOverwrites &) = default;
    FDEnumerationOverwrites(FDEnumerationOverwrites &&) noexcept = default;
    FDEnumerationOverwrites &operator=(const FDEnumerationOverwrites &) = default;
    FDEnumerationOverwrites &operator=(FDEnumerationOverwrites &&) = default;
    std::list<std::shared_ptr<FDEnumValue>> getEnumerators()
    {
        return enumerators;
    }
    void setEnumerators(const std::shared_ptr<FDEnumValue> &enumerator)
    {
        enumerators.emplace_back(enumerator);
    }
};
} // namespace BstIdl
#endif