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
#ifndef F_DEPLOYED_ROOT_ELEMENT_H
#define F_DEPLOYED_ROOT_ELEMENT_H
#include "FDExtensionRoot.h"
#include "GenericPropertyAccessor.h"
namespace BstIdl
{
class FDeployedRootElement : public GenericPropertyAccessor
{
public:
    FDeployedRootElement() = default;
    virtual ~FDeployedRootElement() = default;
    FDeployedRootElement(const FDeployedRootElement &) = default;
    FDeployedRootElement(FDeployedRootElement &&) noexcept = default;
    FDeployedRootElement &operator=(const FDeployedRootElement &) = default;
    FDeployedRootElement &operator=(FDeployedRootElement &&) = default;
    FDeployedRootElement(const std::shared_ptr<FDExtensionRoot> &rootElement)
        : GenericPropertyAccessor(rootElement->getSpec()), m_rootElement(rootElement)
    {
    }
    std::shared_ptr<FDExtensionRoot> getRootElement() const
    {
        return m_rootElement;
    }

protected:
    std::shared_ptr<FDExtensionRoot> m_rootElement;
};
} // namespace BstIdl
#endif