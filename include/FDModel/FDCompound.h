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
#ifndef F_D_COMPOUND_H
#define F_D_COMPOUND_H
#include "FDField.h"
#include "FDTypeDefinition.h"
#include <list>
namespace BstIdl
{
class FDCompound : virtual public FDTypeDefinition
{
protected:
    std::list<std::shared_ptr<FDField>> m_fields;

public:
    FDCompound() = default;
    virtual ~FDCompound() = default;
    FDCompound(const FDCompound &) = default;
    FDCompound(FDCompound &&) noexcept = default;
    FDCompound &operator=(const FDCompound &) = default;
    FDCompound &operator=(FDCompound &&) = default;
    std::list<std::shared_ptr<FDField>> &getFields()
    {
        return m_fields;
    }
    void setFields(const std::shared_ptr<FDField> &field)
    {
        m_fields.emplace_back(field);
    }
};
} // namespace BstIdl
#endif