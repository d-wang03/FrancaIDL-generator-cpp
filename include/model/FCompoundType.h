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
#ifndef F_COMPOUND_TYPE_H
#define F_COMPOUND_TYPE_H

#include "FField.h"
#include "FType.h"
#include <list>
#include <memory>

namespace BstIdl
{

class FCompoundType : public FType
{
public:
    FCompoundType() = default;
    virtual ~FCompoundType() = default;
    FCompoundType(const FCompoundType &) = default;
    FCompoundType(FCompoundType &&) noexcept = default;
    FCompoundType &operator=(const FCompoundType &) = default;
    FCompoundType &operator=(FCompoundType &&) = default;

    std::list<std::shared_ptr<FField>> &getElements()
    {
        return m_elements;
    }

protected:
    std::list<std::shared_ptr<FField>> m_elements;
};

} // namespace BstIdl

#endif