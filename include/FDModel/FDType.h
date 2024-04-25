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
#ifndef F_D_TYPE_H
#define F_D_TYPE_H

#include <string>

namespace BstIdl
{
class FDType
{
public:
    FDType() = default;
    virtual ~FDType() = default;
    FDType(const FDType &) = default;
    FDType(FDType &&) noexcept = default;
    FDType &operator=(const FDType &) = default;
    FDType &operator=(FDType &&) = default;
    virtual std::string getName() const
    {
        return "FDType";
    }
};
} // namespace BstIdl
#endif