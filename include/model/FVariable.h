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
#ifndef F_VARIABLE_H
#define F_VARIABLE_H

#include "FTypedElement.h"

namespace BstIdl
{

class FVariable : public FTypedElement
{
public:
    FVariable() = default;
    virtual ~FVariable() = default;
    FVariable(const FVariable &) = default;
    FVariable(FVariable &&) noexcept = default;
    FVariable &operator=(const FVariable &) = default;
    FVariable &operator=(FVariable &&) = default;
    std::string transform(std::string func) override
    {
        std::string ret = getName();
        auto trans = FObject::s_transMap[func];
        if (trans && trans->forFVariable)
            ret = trans->forFVariable(this);
        return ret;
    }
};

} // namespace BstIdl

#endif