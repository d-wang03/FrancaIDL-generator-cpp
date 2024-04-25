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
#ifndef PROVIDER_PROPERTY_ACCESSOR
#define PROVIDER_PROPERTY_ACCESSOR
#include "FDModel/FDeployedRootElement.h"
namespace BstCommonAPI
{
class ProviderPropertyAccessor
{
public:
    ProviderPropertyAccessor() = default;
    virtual ~ProviderPropertyAccessor() = default;
    ProviderPropertyAccessor(const ProviderPropertyAccessor &) = default;
    ProviderPropertyAccessor(ProviderPropertyAccessor &&) noexcept = default;
    ProviderPropertyAccessor &operator=(const ProviderPropertyAccessor &) = default;
    ProviderPropertyAccessor &operator=(ProviderPropertyAccessor &&) = default;
    ProviderPropertyAccessor(const std::shared_ptr<BstIdl::FDeployedRootElement> &target) : m_target(target)
    {
    }

    // host 'instances'
    std::string getDomain(const std::shared_ptr<BstIdl::FDExtensionElement> &obj)
    {
        return m_target->getString(std::static_pointer_cast<BstIdl::FDElement>(obj), "Domain");
    }
    std::string getInstanceId(const std::shared_ptr<BstIdl::FDExtensionElement> &obj)
    {
        return m_target->getString(std::static_pointer_cast<BstIdl::FDElement>(obj), "InstanceId");
    }
    std::list<std::string> getPreregisteredProperties(const std::shared_ptr<BstIdl::FDExtensionElement> &obj)
    {
        return m_target->getStringArray(std::static_pointer_cast<BstIdl::FDElement>(obj), "PreregisteredProperties");
    }
    std::string getVariant(const std::shared_ptr<BstIdl::FDExtensionElement> &obj)
    {
        return m_target->getString(std::static_pointer_cast<BstIdl::FDElement>(obj), "Variant");
    }
    std::string getYear(const std::shared_ptr<BstIdl::FDExtensionElement> &obj)
    {
        return m_target->getString(std::static_pointer_cast<BstIdl::FDElement>(obj), "SomeIpMulticastEventGroups");
    }

protected:
    std::shared_ptr<BstIdl::FDeployedRootElement> m_target;
};
} // namespace BstCommonAPI

#endif