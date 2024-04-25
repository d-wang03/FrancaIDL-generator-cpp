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
#ifndef F_D_ENUM_TYPE_H
#define F_D_ENUM_TYPE_H
#include "FDEnumerator.h"
#include "FDType.h"
#include <list>
#include <memory>
namespace BstIdl
{
class FDEnumType : public FDType
{
public:
    FDEnumType(const std::shared_ptr<FDEnumerator> &enumerator)
    {
        enumerators.emplace_back();
    }
    FDEnumType() = default;
    virtual ~FDEnumType() = default;
    FDEnumType(const FDEnumType &) = default;
    FDEnumType(FDEnumType &&) noexcept = default;
    FDEnumType &operator=(const FDEnumType &) = default;
    FDEnumType &operator=(FDEnumType &&) = default;

    std::list<std::shared_ptr<FDEnumerator>> const getEnumerators()
    {
        return enumerators;
    }
    void setEnumerators(const std::shared_ptr<FDEnumerator> &enumerator)
    {
        enumerators.emplace_back(enumerator);
    }
    std::string getName() const override
    {
        return "FDEnumType";
    }

protected:
    std::list<std::shared_ptr<FDEnumerator>> enumerators;
};
} // namespace BstIdl
#endif