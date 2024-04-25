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
#ifndef F_FIELD_H
#define F_FIELD_H

#include "FTypedElement.h"

namespace BstIdl
{

class FField : public FTypedElement
{
public:
    FField() = default;
    virtual ~FField() = default;
    FField(const FField &) = default;
    FField(FField &&) noexcept = default;
    FField &operator=(const FField &) = default;
    FField &operator=(FField &&) = default;
    std::string transform(std::string func) override
    {
        std::string ret = getName();
        auto trans = FObject::s_transMap[func];
        if (trans && trans->forFField)
            ret = trans->forFField(this);
        return ret;
    }
};

} // namespace BstIdl

#endif