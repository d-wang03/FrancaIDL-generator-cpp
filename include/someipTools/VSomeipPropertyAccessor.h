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
#ifndef V_SOMEIP_PROPERTY_ACCESSOR
#define SOMEIP_PROPERTY_ACCESSOR
#include "FDModel/FDeployedRootElement.h"
#include "SomeipIDataPropertyAccessor.h"
#include "capicxx-core-tools/ProviderPropertyAccessor.h"
namespace BstCommonAPI
{
class VSomeipPropertyAccessor : public ProviderPropertyAccessor
{
public:
    VSomeipPropertyAccessor() = default;
    virtual ~VSomeipPropertyAccessor() = default;
    VSomeipPropertyAccessor(const VSomeipPropertyAccessor &) = default;
    VSomeipPropertyAccessor(VSomeipPropertyAccessor &&) noexcept = default;
    VSomeipPropertyAccessor &operator=(const VSomeipPropertyAccessor &) = default;
    VSomeipPropertyAccessor &operator=(VSomeipPropertyAccessor &&) = default;
    VSomeipPropertyAccessor(const std::shared_ptr<BstIdl::FDeployedRootElement> &target)
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

    // vsomeip property
    bool getVSomeIpConfigGenEnable(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj, bool &isOK)
    {
        return m_target->getBoolean(std::static_pointer_cast<BstIdl::FDElement>(obj), "VSomeIpConfigGenEnable", isOK);
    }
    std::string getVSomeIpClientHostUnicast(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getString(std::static_pointer_cast<BstIdl::FDElement>(obj), "VSomeIpClientHostUnicast");
    }
    std::string getVSomeIpServerHostUnicast(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getString(std::static_pointer_cast<BstIdl::FDElement>(obj), "VSomeIpServerHostUnicast");
    }
    // logging
    std::string getVSomeIpLoggingLevel(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getEnum(std::static_pointer_cast<BstIdl::FDElement>(obj), "VSomeIpLoggingLevel");
    }
    bool getVSomeIpLoggingViaConsole(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj, bool &isOK)
    {
        return m_target->getBoolean(std::static_pointer_cast<BstIdl::FDElement>(obj), "VSomeIpLoggingViaConsole", isOK);
    }
    bool getVSomeIpCreateLogFile(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj, bool &isOK)
    {
        return m_target->getBoolean(std::static_pointer_cast<BstIdl::FDElement>(obj), "VSomeIpCreateLogFile", isOK);
    }
    std::string getVSomeIpLogFileAbsPath(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getString(std::static_pointer_cast<BstIdl::FDElement>(obj), "VSomeIpLogFileAbsPath");
    }
    bool getVSomeIpLoggingDLT(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj, bool &isOK)
    {
        return m_target->getBoolean(std::static_pointer_cast<BstIdl::FDElement>(obj), "VSomeIpLoggingDLT", isOK);
    }
    bool getVSomeIpVersionCyclicLogEable(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj, bool &isOK)
    {
        return m_target->getBoolean(std::static_pointer_cast<BstIdl::FDElement>(obj), "VSomeIpVersionCyclicLogEable",
                                    isOK);
    }
    int getVSomeIpVersionLogInterval(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getInteger(std::static_pointer_cast<BstIdl::FDElement>(obj), "VSomeIpVersionLogInterval");
    }
    int getVSomeIpMemoryLogInterval(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getInteger(std::static_pointer_cast<BstIdl::FDElement>(obj), "VSomeIpMemoryLogInterval");
    }
    int getVSomeIpStatusLogInterval(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getInteger(std::static_pointer_cast<BstIdl::FDElement>(obj), "VSomeIpStatusLogInterval");
    }
    // applications
    std::list<std::string> getVSomeIpApplicationNames(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getStringArray(std::static_pointer_cast<BstIdl::FDElement>(obj), "VSomeIpApplicationNames");
    }

    std::list<std::string> getVSomeIpApplicationIDs(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getStringArray(std::static_pointer_cast<BstIdl::FDElement>(obj), "VSomeIpApplicationIDs");
    }

    // service-discovery
    bool getVSomeIpServiceDiscoveryEnable(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj, bool &isOK)
    {
        return m_target->getBoolean(std::static_pointer_cast<BstIdl::FDElement>(obj), "ServiceDiscoveryEnable", isOK);
    }
    std::string getVSomeIpServiceDiscoveryMulticastAddress(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getString(std::static_pointer_cast<BstIdl::FDElement>(obj),
                                   "ServiceDiscoveryMulticastAddress");
    }
    int getVSomeIpServiceDiscoveryPort(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getInteger(std::static_pointer_cast<BstIdl::FDElement>(obj), "ServiceDiscoveryPort");
    }
    std::string getVSomeIpServiceDiscoveryProtocol(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getEnum(std::static_pointer_cast<BstIdl::FDElement>(obj), "ServiceDiscoveryProtocol");
    }
    int getVSomeIpServiceDiscoveryInitialDelayMin(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getInteger(std::static_pointer_cast<BstIdl::FDElement>(obj),
                                    "ServiceDiscoveryInitialDelayMin");
    }
    int getVSomeIpServiceDiscoveryInitialDelayMax(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getInteger(std::static_pointer_cast<BstIdl::FDElement>(obj),
                                    "ServiceDiscoveryInitialDelayMax");
    }
    int getVSomeIpServiceDiscoveryRepetitionsBaseDelay(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getInteger(std::static_pointer_cast<BstIdl::FDElement>(obj),
                                    "ServiceDiscoveryRepetitionsBaseDelay");
    }
    int getVSomeIpServiceDiscoveryRepetitionsMax(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getInteger(std::static_pointer_cast<BstIdl::FDElement>(obj), "ServiceDiscoveryRepetitionsMax");
    }
    int getVSomeIpServiceDiscoveryttl(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getInteger(std::static_pointer_cast<BstIdl::FDElement>(obj), "ServiceDiscoveryttl");
    }
    int getVSomeIpServiceDiscoveryCyclicOfferDelay(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getInteger(std::static_pointer_cast<BstIdl::FDElement>(obj),
                                    "ServiceDiscoveryCyclicOfferDelay");
    }
    int getVSomeIpServiceDiscoveryRequestResponseDelay(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getInteger(std::static_pointer_cast<BstIdl::FDElement>(obj),
                                    "ServiceDiscoveryRequestResponseDelay");
    }
};
} // namespace BstCommonAPI

#endif