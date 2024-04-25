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
#ifndef F_D_MAPPER_H
#define F_D_MAPPER_H
#include "FDExtensionRoot.h"
#include "FDInterface.h"
#include "FDTypes.h"
#include <map>
#include <memory>
namespace BstIdl
{
class FDMapper
{
private:
    std::map<std::shared_ptr<BstIdl::FObject>, std::shared_ptr<FDElement>> m_mapping;
    void init(const std::shared_ptr<FDInterface> &fdi);
    void initReferenced(std::list<std::shared_ptr<FDRootElement>> &used);
    void initTypeCollection(const std::shared_ptr<FDTypes> &fdTypes);
    void initTypes(const std::list<std::shared_ptr<FDTypeDefinition>> &fdTypes);
    void initTypes(const std::list<std::shared_ptr<FType>> &ftypes, const std::shared_ptr<FDTypes> &fdTypes);
    void initArguments(const std::shared_ptr<FDArgumentList> &args);
    void initArguments(const std::list<std::shared_ptr<FArgument>> &args,
                       const std::shared_ptr<FDArgumentList> &fdArgs);
    void initExtension(const std::shared_ptr<FDExtensionRoot> &fdExt);

public:
    FDMapper() = default;
    virtual ~FDMapper() = default;
    FDMapper(const FDMapper &) = default;
    FDMapper(FDMapper &&) noexcept = default;
    FDMapper &operator=(const FDMapper &) = default;
    FDMapper &operator=(FDMapper &&) = default;
    FDMapper(const std::shared_ptr<FDInterface> &fdi)
    {
        init(fdi);
    }
    FDMapper(const std::shared_ptr<FDTypes> &fdt)
    {
        initTypeCollection(fdt);
    }
    FDMapper(const std::shared_ptr<FDRootElement> &fdt)
    {
        if (auto ptr = std::dynamic_pointer_cast<FDInterface>(fdt))
            init(ptr);
        else if (auto ptr = std::dynamic_pointer_cast<FDTypes>(fdt))
            initTypeCollection(ptr);
        else if (auto ptr = std::dynamic_pointer_cast<FDExtensionRoot>(fdt))
            initExtension(ptr);
    }

    void clear()
    {
        m_mapping.clear();
    }

    std::shared_ptr<FDElement> getFDElement(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        if (m_mapping.count(obj))
        {
            return m_mapping[obj];
        }
        return nullptr;
    }
};

} // namespace BstIdl
#endif