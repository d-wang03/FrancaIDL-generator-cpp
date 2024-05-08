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
#ifndef ASF_PROPERTY_ACCESSOR_H
#define ASF_PROPERTY_ACCESSOR_H
#include "ASFProviderPropertyAccessor.h"
#include "someipTools/SomeipPropertyAccessor.h"
namespace BstASF
{
class ASFPropertyAccessor
{
protected:
    enum class DeploymentType
    {
        NONE,
        INTERFACE,
        TYPE_COLLECTION,
        PROVIDER,
        OVERWRITE
    };
    DeploymentType type_;
    std::shared_ptr<ASFProviderPropertyAccessor> provider_;
    std::shared_ptr<BstCommonAPI::SomeipPropertyAccessor> someipproviderAccessor;

private:
    template <typename T1, typename T2, typename T3> auto provider_method(T1 &&func_ptr, T2 &&obj, T3 default_value)
    {
        if (type_ == DeploymentType::PROVIDER)
            return provider_ ? (provider_.get()->*func_ptr)(obj) : default_value;
        return default_value;
    }

public:
    ASFPropertyAccessor() = default;
    virtual ~ASFPropertyAccessor() = default;
    ASFPropertyAccessor(const ASFPropertyAccessor &) = default;
    ASFPropertyAccessor(ASFPropertyAccessor &&) noexcept = default;
    ASFPropertyAccessor &operator=(const ASFPropertyAccessor &) = default;
    ASFPropertyAccessor &operator=(ASFPropertyAccessor &&) = default;

    ASFPropertyAccessor(const std::shared_ptr<BstIdl::FDeployedProvider> &_target) : ASFPropertyAccessor()
    {
        type_ = (_target == nullptr ? DeploymentType::NONE : DeploymentType::PROVIDER);
        provider_ = std::make_shared<ASFProviderPropertyAccessor>(_target);
        auto fdExt = _target->getProvider();
        if (fdExt)
        {
            fdExt->updatePropsFromSpec();
            for (auto e : fdExt->getElements())
                if (e)
                    e->updatePropsForAll();
        }
        else
        {
            std::cerr << "fdExt == nullptr without updatePropsForAll" << std::endl;
        }
    }
    // get capi accessor?

    // host 'providers'
    //  auto strart config
    std::string getConfigType(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
            return provider_->getConfigType(obj);

        return std::string();
    }
    std::string getProcessName(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
            return provider_->getProcessName(obj);

        return std::string();
    }
    std::string getDescription(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
            return provider_->getDescription(obj);

        return std::string();
    }
    std::list<std::string> getAfter(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
            return provider_->getAfter(obj);

        return std::list<std::string>();
    }
    std::list<std::string> getBefore(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
            return provider_->getBefore(obj);

        return std::list<std::string>();
    }
    std::list<std::string> getWants(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
            return provider_->getWants(obj);

        return std::list<std::string>();
    }
    std::list<std::string> getRequire(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
            return provider_->getRequire(obj);

        return std::list<std::string>();
    }
    // type?
    std::string getServiceType(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
            return provider_->getServiceType(obj);

        return std::string();
    }
    std::string getServiceBusName(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
            return provider_->getServiceBusName(obj);

        return std::string();
    }
    std::list<std::string> getEnvironmentVar(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
            return provider_->getEnvironmentVar(obj);

        return std::list<std::string>();
    }
    std::string getSLMExecCommand(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
            return provider_->getSLMExecCommand(obj);

        return std::string();
    }
    std::list<std::string> getServiceExecCommand(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
            return provider_->getServiceExecCommand(obj);

        return std::list<std::string>();
    }
    std::list<std::string> getExecArgs(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
            return provider_->getExecArgs(obj);

        return std::list<std::string>();
    }
    std::list<std::string> getServiceExecStartPre(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
            return provider_->getServiceExecStartPre(obj);

        return std::list<std::string>();
    }
    std::list<std::string> getServiceExecStopPost(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
            return provider_->getServiceExecStopPost(obj);

        return std::list<std::string>();
    }
    std::string getServiceWantedBy(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
        {
            return provider_->getServiceWantedBy(obj);
        }

        return std::string();
    }
    std::string getXMLWaitfor(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
            return provider_->getXMLWaitfor(obj);

        return std::string();
    }
    std::list<std::string> getReuseSLMSystemInclusionName(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
            return provider_->getReuseSLMSystemInclusionName(obj);

        return std::list<std::string>();
    }
    std::list<std::string> getReuseSLMSystemFileName(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
            return provider_->getReuseSLMSystemFileName(obj);

        return std::list<std::string>();
    }
    //  module json
    bool getModuleAutoStartup(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
            return provider_->getModuleAutoStartup(obj);

        return false;
    }
    bool getModuleAutoRestart(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
            return provider_->getModuleAutoRestart(obj);

        return false;
    }
    std::string getModuleMaxStartAction(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
            return provider_->getModuleMaxStartAction(obj);

        return std::string();
    }
    std::string getModuleMaxStartDtc(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
        {
            return provider_->getModuleMaxStartDtc(obj);
        }

        return std::string();
    }
    std::string getModuleVersion(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
            return provider_->getModuleVersion(obj);

        return std::string();
    }
    std::string getModuleSpecialPara(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
            return provider_->getModuleSpecialPara(obj);

        return std::string();
    }
    std::string getModuleLabel(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
            return provider_->getModuleLabel(obj);

        return std::string();
    }

    /// parameters about someip config
    // vsomeip
    bool getSomeIpConfigGenEnable(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj, bool &isOK)
    {
        return provider_ ? provider_->getSomeIpConfigGenEnable(obj, isOK) : isOK = false;
    }

    std::string getSomeIpClientHostUnicast(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        std::string ret;
        return provider_method(&ASFProviderPropertyAccessor::getSomeIpClientHostUnicast, obj, ret);
    }
    std::string getSomeIpServerHostUnicast(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        std::string ret;
        return provider_method(&ASFProviderPropertyAccessor::getSomeIpServerHostUnicast, obj, ret);
    }
    // logging
    std::string getSomeIpLoggingLevel(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        std::string ret;
        return provider_method(&ASFProviderPropertyAccessor::getSomeIpLoggingLevel, obj, ret);
    }
    bool getSomeIpLoggingViaConsole(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj, bool &isOK)
    {
        return provider_ ? provider_->getSomeIpLoggingViaConsole(obj, isOK) : isOK = false;
    }
    bool getSomeIpCreateLogFile(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj, bool &isOK)
    {
        return provider_ ? provider_->getSomeIpCreateLogFile(obj, isOK) : isOK = false;
    }
    std::string getSomeIpLogFileAbsPath(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        std::string ret;
        return provider_method(&ASFProviderPropertyAccessor::getSomeIpLogFileAbsPath, obj, ret);
    }
    bool getSomeIpLoggingDLT(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj, bool &isOK)
    {
        return provider_ ? provider_->getSomeIpLoggingDLT(obj, isOK) : isOK = false;
    }
    bool getSomeIpVersionCyclicLogEable(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj, bool &isOK)
    {
        return provider_ ? provider_->getSomeIpVersionCyclicLogEable(obj, isOK) : isOK = false;
    }
    int getSomeIpVersionLogInterval(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        int ret = -1;
        return provider_method(&ASFProviderPropertyAccessor::getSomeIpVersionLogInterval, obj, ret);
    }
    int getSomeIpMemoryLogInterval(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        int ret = -1;
        return provider_method(&ASFProviderPropertyAccessor::getSomeIpMemoryLogInterval, obj, ret);
    }
    int getSomeIpStatusLogInterval(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        int ret = -1;
        return provider_method(&ASFProviderPropertyAccessor::getSomeIpStatusLogInterval, obj, ret);
    }
    // applications
    std::list<std::string> getSomeIpApplicationNames(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        std::list<std::string> ret;
        return provider_method(&ASFProviderPropertyAccessor::getSomeIpApplicationNames, obj, ret);
    }
    std::list<std::string> getSomeIpApplicationIDs(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        std::list<std::string> ret;
        return provider_method(&ASFProviderPropertyAccessor::getSomeIpApplicationIDs, obj, ret);
    }
    // service-discovery
    bool getSomeIpServiceDiscoveryEnable(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj, bool &isOK)
    {
        return provider_ ? provider_->getSomeIpServiceDiscoveryEnable(obj, isOK) : isOK = false;
    }
    std::string getSomeIpServiceDiscoveryMulticastAddress(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        std::string ret;
        return provider_method(&ASFProviderPropertyAccessor::getSomeIpServiceDiscoveryMulticastAddress, obj, ret);
    }
    int getSomeIpServiceDiscoveryPort(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        int ret = -1;
        return provider_method(&ASFProviderPropertyAccessor::getSomeIpServiceDiscoveryPort, obj, ret);
    }

    std::string getSomeIpServiceDiscoveryProtocol(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        std::string ret;
        return provider_method(&ASFProviderPropertyAccessor::getSomeIpServiceDiscoveryProtocol, obj, ret);
    }
    int getSomeIpServiceDiscoveryInitialDelayMin(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        int ret = -1;
        return provider_method(&ASFProviderPropertyAccessor::getSomeIpServiceDiscoveryInitialDelayMin, obj, ret);
    }
    int getSomeIpServiceDiscoveryInitialDelayMax(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        int ret = -1;
        return provider_method(&ASFProviderPropertyAccessor::getSomeIpServiceDiscoveryInitialDelayMax, obj, ret);
    }
    int getSomeIpServiceDiscoveryRepetitionsBaseDelay(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        int ret = -1;
        return provider_method(&ASFProviderPropertyAccessor::getSomeIpServiceDiscoveryRepetitionsBaseDelay, obj, ret);
    }
    int getSomeIpServiceDiscoveryRepetitionsMax(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        int ret = -1;
        return provider_method(&ASFProviderPropertyAccessor::getSomeIpServiceDiscoveryRepetitionsMax, obj, ret);
    }
    int getSomeIpServiceDiscoveryttl(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        int ret = -1;
        return provider_method(&ASFProviderPropertyAccessor::getSomeIpServiceDiscoveryttl, obj, ret);
    }
    int getSomeIpServiceDiscoveryCyclicOfferDelay(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        int ret = -1;
        return provider_method(&ASFProviderPropertyAccessor::getSomeIpServiceDiscoveryCyclicOfferDelay, obj, ret);
    }
    int getSomeIpServiceDiscoveryRequestResponseDelay(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        int ret = -1;
        return provider_method(&ASFProviderPropertyAccessor::getSomeIpServiceDiscoveryRequestResponseDelay, obj, ret);
    }

    // host 'instances'
    std::string getInterfaceType(const std::shared_ptr<BstIdl::FDExtensionElement> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
            return provider_->getInterfaceType(obj);

        return std::string();
    }
    std::string getInstanceType(const std::shared_ptr<BstIdl::FDExtensionElement> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
            return provider_->getInstanceType(obj);

        return std::string();
    }
    std::string getUserLogicClassName(const std::shared_ptr<BstIdl::FDExtensionElement> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
            return provider_->getUserLogicClassName(obj);

        return std::string();
    }
    std::string getUserLogicHeaderPath(const std::shared_ptr<BstIdl::FDExtensionElement> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
            return provider_->getUserLogicHeaderPath(obj);

        return std::string();
    }
};
} // namespace BstASF

#endif
