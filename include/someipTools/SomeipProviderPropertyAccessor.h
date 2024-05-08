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
#ifndef SOMEIP_PROVIDER_PROPERTY_ACCESSOR
#define SOMEIP_PROVIDER_PROPERTY_ACCESSOR
#include "FDModel/FDeployedRootElement.h"
#include "SomeipIDataPropertyAccessor.h"
#include "capicxx-core-tools/ProviderPropertyAccessor.h"
namespace BstCommonAPI
{
class SomeipProviderPropertyAccessor : public ProviderPropertyAccessor
{
public:
    SomeipProviderPropertyAccessor() = default;
    virtual ~SomeipProviderPropertyAccessor() = default;
    SomeipProviderPropertyAccessor(const SomeipProviderPropertyAccessor &) = default;
    SomeipProviderPropertyAccessor(SomeipProviderPropertyAccessor &&) noexcept = default;
    SomeipProviderPropertyAccessor &operator=(const SomeipProviderPropertyAccessor &) = default;
    SomeipProviderPropertyAccessor &operator=(SomeipProviderPropertyAccessor &&) = default;
    SomeipProviderPropertyAccessor(const std::shared_ptr<BstIdl::FDeployedRootElement> &target)
        : ProviderPropertyAccessor(target)
    {
    }

    // host 'instances'
    int getSomeIpInstanceID(const std::shared_ptr<BstIdl::FDExtensionElement> &obj)
    {
        return m_target->getInteger(std::static_pointer_cast<BstIdl::FDElement>(obj), "SomeIpInstanceID");
    }
    std::string getSomeIpUnicastAddress(const std::shared_ptr<BstIdl::FDExtensionElement> &obj)
    {
        return m_target->getString(std::static_pointer_cast<BstIdl::FDElement>(obj), "SomeIpUnicastAddress");
    }
    int getSomeIpReliableUnicastPort(const std::shared_ptr<BstIdl::FDExtensionElement> &obj)
    {
        return m_target->getInteger(std::static_pointer_cast<BstIdl::FDElement>(obj), "SomeIpReliableUnicastPort");
    }
    int getSomeIpUnreliableUnicastPort(const std::shared_ptr<BstIdl::FDExtensionElement> &obj)
    {
        return m_target->getInteger(std::static_pointer_cast<BstIdl::FDElement>(obj), "SomeIpUnreliableUnicastPort");
    }
    std::list<int> getSomeIpMulticastEventGroups(const std::shared_ptr<BstIdl::FDExtensionElement> &obj)
    {
        return m_target->getIntegerArray(std::static_pointer_cast<BstIdl::FDElement>(obj),
                                         "SomeIpMulticastEventGroups");
    }
    std::list<std::string> getSomeIpMulticastAddresses(const std::shared_ptr<BstIdl::FDExtensionElement> &obj)
    {
        return m_target->getStringArray(std::static_pointer_cast<BstIdl::FDElement>(obj), "SomeIpMulticastAddresses");
    }
    std::list<int> getSomeIpMulticastPorts(const std::shared_ptr<BstIdl::FDExtensionElement> &obj)
    {
        return m_target->getIntegerArray(std::static_pointer_cast<BstIdl::FDElement>(obj), "SomeIpMulticastPorts");
    }
    std::list<int> getSomeIpMulticastThreshold(const std::shared_ptr<BstIdl::FDExtensionElement> &obj)
    {
        return m_target->getIntegerArray(std::static_pointer_cast<BstIdl::FDElement>(obj), "SomeIpMulticastThreshold");
    }
    SomeIpDefaultErrorCoding getSomeIpDefaultErrorCoding(const std::shared_ptr<BstIdl::FDExtensionElement> &obj)
    {
        std::string e = m_target->getEnum(std::static_pointer_cast<BstIdl::FDElement>(obj), "SomeIpDefaultErrorCoding");
        if (e.empty())
            return SomeIpDefaultErrorCoding::Invalid;
        if (e == "Header")
            return SomeIpDefaultErrorCoding::Header;
        return SomeIpDefaultErrorCoding::Invalid;
    }
};
} // namespace BstCommonAPI

#endif