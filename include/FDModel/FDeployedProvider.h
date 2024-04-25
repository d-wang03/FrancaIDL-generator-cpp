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
#ifndef F_DEPLOYED_PROVIDER_H
#define F_DEPLOYED_PROVIDER_H
#include "FDeployedRootElement.h"
namespace BstIdl
{
class FDeployedProvider : public FDeployedRootElement
{
public:
    FDeployedProvider() = default;
    virtual ~FDeployedProvider() = default;
    FDeployedProvider(const FDeployedProvider &) = default;
    FDeployedProvider(FDeployedProvider &&) noexcept = default;
    FDeployedProvider &operator=(const FDeployedProvider &) = default;
    FDeployedProvider &operator=(FDeployedProvider &&) = default;
    FDeployedProvider(const std::shared_ptr<FDExtensionRoot> &provider) : FDeployedRootElement(provider)
    {
    }
    std::shared_ptr<FDExtensionRoot> getProvider() const
    {
        return m_rootElement;
    }
};
} // namespace BstIdl
#endif