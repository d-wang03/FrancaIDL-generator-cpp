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
#include "msgbox-tools/MsgBoxGenerator.h"
#include "FDModel/FDEnumerator.h"
#include "FDModel/FDModelManager.h"
#include "msgbox-tools/BareMetalProxyGenerator.h"
#include "msgbox-tools/BareMetalStubGenerator.h"

namespace BstIdl
{

bool MsgBoxGenerator::generate()
{
    if (m_destDir.empty())
        return false;

    auto &fdmodels = BstIdl::FDModelManager::getInstance();
    if (fdmodels.size() == 0)
    {
        std::cerr << "No valid models!" << std::endl;
        return false;
    }

    for (const auto &model : fdmodels.getModelList())
    {
        for (const auto &element : model->getDeployments())
        {
            auto spec = element->getSpec();
            if (!spec || spec->getName() != "com.bst.ipc.deployment")
                continue;
            if (auto ptr = std::dynamic_pointer_cast<FDExtensionRoot>(element))
            {
                if (ptr->getTag() != "provider")
                    continue;
                if (!validateProvider(ptr))
                {
                    std::cerr << "Error provider deployment." << std::endl;
                    return false;
                }
                bool ret = false;
                if (m_providerType == "Server")
                {
                    if (m_rte == "BareMetal")
                    {
                        for (const auto &ins : ptr->getElements())
                        {
                            std::cout << "Generating baremetal server code ..." << std::endl;
                            if (!ins || ins->getTag() != "instance")
                                continue;
                            auto gen = std::make_shared<BareMetalStubGenerator>(m_destDir, m_srcEndID, ins);
                            ret = gen->generate();
                        }
                    }
                    // else
                    // call posix generator
                }
                else if (m_providerType == "Client")
                {
                    if (m_rte == "BareMetal")
                    {
                        for (const auto &ins : ptr->getElements())
                        {
                            std::cout << "Generating baremetal client code ..." << std::endl;
                            if (!ins || ins->getTag() != "instance")
                                continue;
                            auto gen = std::make_shared<BareMetalProxyGenerator>(m_destDir, m_srcEndID, ins);
                            ret = gen->generate();
                        }
                    }
                    // else
                    // call posix generator
                }
                if (ret)
                    std::cout << "Generation for " << ptr->getName() << " success." << std::endl;
                else
                {
                    std::cerr << "Generation for " << ptr->getName() << " fail." << std::endl;
                }
            }
        }
    }

    return true;
}

bool MsgBoxGenerator::validateProvider(const std::shared_ptr<FDExtensionRoot> &provider)
{
    if (!provider)
        return false;

    m_providerType.clear();
    m_rte.clear();
    m_srcEndID.clear();

    // get and validate ProviderType
    auto value = provider->getSingleValue("ProviderType");
    if (!value)
    {
        std::cerr << "No ProviderType defined." << std::endl;
        return false;
    }
    auto type = value->getEnumerator();
    if (!type)
    {
        std::cerr << "Invalid ProviderType." << std::endl;
        return false;
    }
    m_providerType = type->getName();
    if (m_providerType != "Server" && m_providerType != "Client")
    {
        std::cerr << "Invalid ProviderType." << std::endl;
        return false;
    }

    // get and validate RTE
    value = provider->getSingleValue("RTE");
    if (!value)
    {
        std::cerr << "No RTE defined." << std::endl;
        return false;
    }
    type = value->getEnumerator();
    if (!type)
    {
        std::cerr << "Invalid RTE." << std::endl;
        return false;
    }
    m_rte = type->getName();
    if (m_rte != "BareMetal" && m_rte != "Posix")
    {
        std::cerr << "Invalid RTE." << std::endl;
        return false;
    }

    // get and validate SrcEndID
    value = provider->getSingleValue("SrcEndID");
    if (!value)
    {
        std::cerr << "No SrcEndID defined." << std::endl;
        return false;
    }
    type = value->getEnumerator();
    if (!type)
    {
        std::cerr << "Invalid SrcEndID." << std::endl;
        return false;
    }
    m_srcEndID = type->getName();
    if (m_srcEndID != "CPU_0" && m_srcEndID != "CPU_1" && m_srcEndID != "CPU_2" && m_srcEndID != "CPU_3" &&
        m_srcEndID != "CPU_4" && m_srcEndID != "CPU_5" && m_srcEndID != "CPU_6" && m_srcEndID != "CPU_7" &&
        m_srcEndID != "CPUMP2_0" && m_srcEndID != "CPUMP2_1" && m_srcEndID != "ISPCV_0" && m_srcEndID != "ISPCV_1" &&
        m_srcEndID != "ISPCV_2" && m_srcEndID != "ISPCV_3" && m_srcEndID != "ISPCV_4" && m_srcEndID != "NET_0" &&
        m_srcEndID != "DMA_0" && m_srcEndID != "DMA_1" && m_srcEndID != "SWITCH_0" && m_srcEndID != "SWITCH_1" &&
        m_srcEndID != "SWITCH_2" && m_srcEndID != "SWITCH_3" && m_srcEndID != "SWITCH_4" && m_srcEndID != "SWITCH_5" &&
        m_srcEndID != "SECURE_0" && m_srcEndID != "SECURE_1" && m_srcEndID != "SAFETY_0" && m_srcEndID != "SAFETY_1" &&
        m_srcEndID != "REALTIME_0" && m_srcEndID != "REALTIME_1" && m_srcEndID != "REALTIME_2" &&
        m_srcEndID != "REALTIME_3" && m_srcEndID != "REALTIME_4" && m_srcEndID != "REALTIME_5" &&
        m_srcEndID != "MEDIA_0")
    {
        std::cerr << "Invalid SrcEndID." << std::endl;
        return false;
    }

    return true;
}

} // namespace BstIdl