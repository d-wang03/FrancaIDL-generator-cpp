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
#ifndef F_D_TYPES_H
#define F_D_TYPES_H
#include "FDRootElement.h"
#include "FDTypeDefinition.h"
#include "model/FTypeCollection.h"
namespace BstIdl
{
class FDTypes : public FDRootElement
{
protected:
    std::shared_ptr<BstIdl::FTypeCollection> m_target;
    std::list<std::shared_ptr<FDTypeDefinition>> m_types;

public:
    FDTypes() : FDRootElement()
    {
        setHost(std::make_shared<FDPropertyHost>("type_collections"));
    }
    virtual ~FDTypes() = default;
    FDTypes(const FDTypes &) = default;
    FDTypes(FDTypes &&) noexcept = default;
    FDTypes &operator=(const FDTypes &) = default;
    FDTypes &operator=(FDTypes &&) = default;
    std::shared_ptr<BstIdl::FTypeCollection> const getTarget()
    {
        return m_target;
    }
    void setTarget(const std::shared_ptr<BstIdl::FTypeCollection> &target)
    {
        m_target = target;
        if (getName().empty() && target)
            setName(target->getName());
    }
    std::list<std::shared_ptr<FDTypeDefinition>> &getTypes()
    {
        return m_types;
    }
    void setTypes(std::shared_ptr<FDTypeDefinition> &type)
    {
        m_types.emplace_back(type);
    }
};
} // namespace BstIdl
#endif