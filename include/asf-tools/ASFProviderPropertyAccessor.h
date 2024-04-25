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
#ifndef ASF_PROVIDER_PROPERTY_ACCESSOR
#define ASF_PROVIDER_PROPERTY_ACCESSOR
#include "FDModel/FDeployedRootElement.h"
namespace BstASF
{
class GeneralServiceGeneratorExtensions;
class ASFProviderPropertyAccessor
{
    friend class GeneralServiceGeneratorExtensions;

public:
    ASFProviderPropertyAccessor() = default;
    virtual ~ASFProviderPropertyAccessor() = default;
    ASFProviderPropertyAccessor(const ASFProviderPropertyAccessor &) = default;
    ASFProviderPropertyAccessor(ASFProviderPropertyAccessor &&) noexcept = default;
    ASFProviderPropertyAccessor &operator=(const ASFProviderPropertyAccessor &) = default;
    ASFProviderPropertyAccessor &operator=(ASFProviderPropertyAccessor &&) = default;
    ASFProviderPropertyAccessor(const std::shared_ptr<BstIdl::FDeployedRootElement> &target) : m_target(target)
    {
    }

    // host 'providers'
    //  auto strart config
    std::string getConfigType(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getEnum(std::static_pointer_cast<BstIdl::FDElement>(obj), "config_type");
    }
    std::string getProcessName(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getString(std::static_pointer_cast<BstIdl::FDElement>(obj), "ProcessName");
    }
    std::string getDescription(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getString(std::static_pointer_cast<BstIdl::FDElement>(obj), "Description");
    }
    std::list<std::string> getAfter(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getStringArray(std::static_pointer_cast<BstIdl::FDElement>(obj), "After");
    }
    std::list<std::string> getBefore(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getStringArray(std::static_pointer_cast<BstIdl::FDElement>(obj), "Before");
    }
    std::list<std::string> getWants(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getStringArray(std::static_pointer_cast<BstIdl::FDElement>(obj), "Wants");
    }
    std::list<std::string> getRequire(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getStringArray(std::static_pointer_cast<BstIdl::FDElement>(obj), "Require");
    }
    // type
    std::string getServiceType(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getEnum(std::static_pointer_cast<BstIdl::FDElement>(obj), "ServiceType");
    }
    std::string getServiceBusName(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getString(std::static_pointer_cast<BstIdl::FDElement>(obj), "ServiceBusName");
    }
    std::list<std::string> getEnvironmentVar(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getStringArray(std::static_pointer_cast<BstIdl::FDElement>(obj), "EnvironmentVars");
    }
    std::string getSLMExecCommand(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getString(std::static_pointer_cast<BstIdl::FDElement>(obj), "SLMExecCommand");
    }
    std::list<std::string> getServiceExecCommand(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getStringArray(std::static_pointer_cast<BstIdl::FDElement>(obj), "ServiceExecCommand");
    }
    std::list<std::string> getExecArgs(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getStringArray(std::static_pointer_cast<BstIdl::FDElement>(obj), "ExecArgs");
    }
    std::list<std::string> getServiceExecStartPre(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getStringArray(std::static_pointer_cast<BstIdl::FDElement>(obj), "ServiceExecStartPre");
    }
    std::list<std::string> getServiceExecStopPost(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getStringArray(std::static_pointer_cast<BstIdl::FDElement>(obj), "ServiceExecStopPost");
    }
    std::string getServiceWantedBy(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getString(std::static_pointer_cast<BstIdl::FDElement>(obj), "WantedBy");
    }
    std::string getXMLWaitfor(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getString(std::static_pointer_cast<BstIdl::FDElement>(obj), "XMLWaitfor");
    }
    std::list<std::string> getReuseSLMSystemInclusionName(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getStringArray(std::static_pointer_cast<BstIdl::FDElement>(obj),
                                        "ReuseSLMSystemInclusionName");
    }
    std::list<std::string> getReuseSLMSystemFileName(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getStringArray(std::static_pointer_cast<BstIdl::FDElement>(obj), "ReuseSLMSystemFileName");
    }
    //  module json
    bool getModuleAutoStartup(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        bool isOK;
        return m_target->getBoolean(std::static_pointer_cast<BstIdl::FDElement>(obj), "module_auto_startup", isOK);
    }
    bool getModuleAutoRestart(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        bool isOK;
        return m_target->getBoolean(std::static_pointer_cast<BstIdl::FDElement>(obj), "module_auto_restart", isOK);
    }
    std::string getModuleMaxStartAction(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getEnum(std::static_pointer_cast<BstIdl::FDElement>(obj), "module_max_start_action");
    }
    std::string getModuleMaxStartDtc(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getString(std::static_pointer_cast<BstIdl::FDElement>(obj), "module_max_start_dtc");
    }
    std::string getModuleVersion(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getString(std::static_pointer_cast<BstIdl::FDElement>(obj), "module_version");
    }
    std::string getModuleSpecialPara(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getString(std::static_pointer_cast<BstIdl::FDElement>(obj), "module_special_para");
    }
    std::string getModuleLabel(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        return m_target->getEnum(std::static_pointer_cast<BstIdl::FDElement>(obj), "module_label");
    }

    // host 'instances'
    std::string getInterfaceType(const std::shared_ptr<BstIdl::FDExtensionElement> &obj)
    {
        return m_target->getEnum(std::static_pointer_cast<BstIdl::FDElement>(obj), "InterfaceType");
    }
    std::string getInstanceType(const std::shared_ptr<BstIdl::FDExtensionElement> &obj)
    {
        return m_target->getEnum(std::static_pointer_cast<BstIdl::FDElement>(obj), "InstanceType");
    }
    std::string getUserLogicClassName(const std::shared_ptr<BstIdl::FDExtensionElement> &obj)
    {
        return m_target->getString(std::static_pointer_cast<BstIdl::FDElement>(obj), "UserLogicClassName");
    }
    std::string getUserLogicHeaderPath(const std::shared_ptr<BstIdl::FDExtensionElement> &obj)
    {
        return m_target->getString(std::static_pointer_cast<BstIdl::FDElement>(obj), "UserLogicHeaderPath");
    }

protected:
    std::shared_ptr<BstIdl::FDeployedRootElement> m_target;
};
} // namespace BstASF

#endif