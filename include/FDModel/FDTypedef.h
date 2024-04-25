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
#ifndef F_D_TYPEDEF_H
#define F_D_TYPEDEF_H
#include "FDTypeDefinition.h"
#include "model/FTypeDef.h"
namespace BstIdl
{
class FDTypedef : public FDTypeDefinition
{
protected:
    std::shared_ptr<BstIdl::FTypeDef> m_target;

public:
    FDTypedef() : FDTypeDefinition()
    {
        setHost("typedefs");
    }
    virtual ~FDTypedef() = default;
    FDTypedef(const FDTypedef &) = default;
    FDTypedef(FDTypedef &&) noexcept = default;
    FDTypedef &operator=(const FDTypedef &) = default;
    FDTypedef &operator=(FDTypedef &&) = default;
    std::shared_ptr<BstIdl::FTypeDef> getTarget() const
    {
        return m_target;
    }
    void setTarget(const std::shared_ptr<BstIdl::FTypeDef> &target)
    {
        m_target = target;
        if (getName().empty() && target)
            setName(target->getName());
    }
};
} // namespace BstIdl
#endif