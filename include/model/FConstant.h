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
#ifndef F_CONSTANT_H
#define F_CONSTANT_H

#include "FExpression.h"

namespace BstIdl
{

class FConstant : public FExpression
{
public:
    FConstant() = default;
    virtual ~FConstant() = default;
    FConstant(const FConstant &) = default;
    FConstant(FConstant &&) noexcept = default;
    FConstant &operator=(const FConstant &) = default;
    FConstant &operator=(FConstant &&) = default;
    std::string toString() const override
    {
        return "";
    }
};

} // namespace BstIdl

#endif