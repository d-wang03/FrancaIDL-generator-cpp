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
#include "someipTools/SomeipGenerator.h"
#include "FDParser/FDParser.h"
#include "asf-tools/GeneralServiceGeneratorExtensions.h"
#include "capicxx-core-tools/FrancaGeneratorExtensions.h"
#include "parser/FidlParser.h"
#include "someipTools/FInterfaceSomeIPDeploymentGenerator.h"
#include "someipTools/FInterfaceSomeIPProxyGenerator.h"
#include "someipTools/FInterfaceSomeIPStubAdapterGenerator.h"
#include "someipTools/FTypeCollectionSomeIPDeploymentGenerator.h"
#include "someipTools/SomeipPropertyAccessor.h"
#include "utilities/list_utility.h"
#include "utilities/string_utility.h"

namespace BstCommonAPI
{
bool SomeipGenerator::generate()
{
    if (m_destDir.empty())
        return false;

    auto &fmodels = BstIdl::FModelManager::getInstance();
    auto &fdmodels = BstIdl::FDModelManager::getInstance();
    if (fmodels.size() == 0 && fdmodels.size() == 0)
    {
        std::cerr << "No valid models!" << std::endl;
        return false;
    }

    std::list<std::shared_ptr<BstIdl::FDInterface>> deployedInterfaces;
    std::list<std::shared_ptr<BstIdl::FDTypes>> deployedTypes;
    std::list<std::shared_ptr<BstIdl::FDExtensionRoot>> deployedProviders;
    std::list<std::shared_ptr<BstIdl::FDInterface>> coreDeployedInterfaces;
    std::list<std::shared_ptr<BstIdl::FDTypes>> coreDeployedTypes;
    std::list<std::string> deployedImports;
    for (const auto &fdmodel : fdmodels.getModelList())
    {
        // fill all deployed interfaces and types.
        for (const auto &item : fdmodel->getDeployments())
        {
            auto spec = item->getSpec();
            if (!spec)
                continue;
            auto spec_name = spec->getName();
            // get core deployed interfaces and typecollections.
            if (contains(spec_name, "core.deployment"))
            {
                if (auto ptr = std::dynamic_pointer_cast<BstIdl::FDInterface>(item))
                    addListItem(coreDeployedInterfaces, ptr);
                else if (auto ptr = std::dynamic_pointer_cast<BstIdl::FDTypes>(item))
                    addListItem(coreDeployedTypes, ptr);
                else
                    continue;
            }
            // get someip deployed interfaces and typecollections, providers.
            if (contains(spec_name, "someip.deployment"))
            {
                if (auto ptr = std::dynamic_pointer_cast<BstIdl::FDInterface>(item))
                    addListItem(deployedInterfaces, ptr);
                else if (auto ptr = std::dynamic_pointer_cast<BstIdl::FDTypes>(item))
                    addListItem(deployedTypes, ptr);
                else if (auto ptr = std::dynamic_pointer_cast<BstIdl::FDExtensionRoot>(item))
                    addListItem(deployedProviders, ptr);
                else
                    continue;
            }
        }
        // get all imported file names.
        for (const auto &item : fdmodel->getImports())
        {
            addListItem(deployedImports, item->getImportURI());
        }
    }
    // merge core deploy and someip deploy
    for (auto t : deployedInterfaces)
        for (auto s : coreDeployedInterfaces)
            FrancaGeneratorExtensions::getInstance().mergeDeployments(s, t);
    for (auto t : deployedTypes)
        for (auto s : coreDeployedTypes)
            FrancaGeneratorExtensions::getInstance().mergeDeployments(s, t);

    std::cout << "Creating accessors...";
    for (const auto &fmodel : fmodels.getModelList())
    {
        auto it = std::find_if(deployedImports.cbegin(), deployedImports.cend(),
                               [fmodel](std::string item) { return contains(fmodel->getFilenames(), item); });
        if (it == deployedImports.cend())
            continue;

        // create accessor
        for (const auto &interface : fmodel->getInterfaces())
        {
            std::shared_ptr<SomeipPropertyAccessor> accessor;
            auto it = std::find_if(deployedInterfaces.cbegin(), deployedInterfaces.cend(),
                                   [interface](const auto &item) { return item->getTarget() == interface; });

            if (it == deployedInterfaces.cend())
                accessor = std::make_shared<SomeipPropertyAccessor>();
            else
                accessor = std::make_shared<SomeipPropertyAccessor>(std::make_shared<BstIdl::FDeployedInterface>(*it));
            FrancaSomeIPGeneratorExtensions::getInstance().insertAccessor(interface, accessor);
        }
        for (const auto &typecollection : fmodel->getTypeCollections())
        {
            std::shared_ptr<SomeipPropertyAccessor> accessor;
            auto it = std::find_if(deployedTypes.cbegin(), deployedTypes.cend(),
                                   [typecollection](const auto &item) { return item->getTarget() == typecollection; });

            if (it == deployedTypes.cend())
                accessor = std::make_shared<SomeipPropertyAccessor>();
            else
            {
                auto target = std::make_shared<BstIdl::FDeployedTypeCollection>(*it);
                accessor = std::make_shared<SomeipPropertyAccessor>(target);
            }

            FrancaSomeIPGeneratorExtensions::getInstance().insertAccessor(typecollection, accessor);
        }
    }
    std::cout << "Done!" << std::endl;
    // generate code.
    std::cout << "Start code generation" << std::endl;
    const auto server_Infs =
        BstASF::GeneralServiceGeneratorExtensions::getInstance().getServerTypeInterfacesIncludingBase();
    const auto client_Infs =
        BstASF::GeneralServiceGeneratorExtensions::getInstance().getClientTypeInterfacesIncludingBase();
    const bool isEmptyServer = BstASF::GeneralServiceGeneratorExtensions::getInstance().isEmptyServer();
    for (const auto &fmodel : fmodels.getModelList())
    {
        auto it = std::find_if(deployedImports.cbegin(), deployedImports.cend(), [fmodel](std::string item) {
            auto ret = contains(fmodel->getFilenames(), item);
            return ret;
        });
        if (it == deployedImports.cend())
            continue;

        for (const auto &typecollection : fmodel->getTypeCollections())
        {
            std::cout << "Generate code for " << typecollection->getName() << "...";
            auto accessor = FrancaSomeIPGeneratorExtensions::getInstance().getSomeIpAccessor(typecollection);
            FTypeCollectionSomeIPDeploymentGenerator::getInstance().setLicense(getLicense());
            FTypeCollectionSomeIPDeploymentGenerator::getInstance().generateTypeCollectionDeployment(
                typecollection, accessor, true, m_destDir);
            std::cout << "Done!" << std::endl;
        }
        for (const auto &interface : fmodel->getInterfaces())
        {
            auto accessor = FrancaSomeIPGeneratorExtensions::getInstance().getSomeIpAccessor(interface);
            std::cout << "Generate code for " << interface->getName() << "...";
            FInterfaceSomeIPDeploymentGenerator::getInstance().setLicense(getLicense());
            FInterfaceSomeIPDeploymentGenerator::getInstance().generateDeployment(interface, accessor, true, m_destDir);
            std::cout << "Done!" << std::endl;
            // client or server?
            if (isEmptyServer)
            {
                std::cout << "Generate proxy code for " << interface->getName() << "...";
                FInterfaceSomeIPProxyGenerator::getInstance().setLicense(getLicense());
                FInterfaceSomeIPProxyGenerator::getInstance().generateProxy(interface, accessor, deployedProviders,
                                                                            true, true, m_destDir);
                std::cout << "Done!" << std::endl;
                std::cout << "Generate stub code for " << interface->getName() << "...";
                FInterfaceSomeIPStubAdapterGenerator::getInstance().setLicense(getLicense());
                FInterfaceSomeIPStubAdapterGenerator::getInstance().generateStubAdapter(
                    interface, accessor, deployedProviders, m_generateStub, m_destDir);
                std::cout << "Done!" << std::endl;
            }
            else
            {
                bool genStub = contains(server_Infs, interface);
                bool genProxy = contains(client_Infs, interface);
                std::cout << "Generate proxy code for " << interface->getName() << "...";
                FInterfaceSomeIPProxyGenerator::getInstance().setLicense(getLicense());
                FInterfaceSomeIPProxyGenerator::getInstance().generateProxy(interface, accessor, deployedProviders,
                                                                            genProxy, true, m_destDir);
                std::cout << "Done!" << std::endl;
                std::cout << "Generate stub code for " << interface->getName() << "...";
                FInterfaceSomeIPStubAdapterGenerator::getInstance().setLicense(getLicense());
                FInterfaceSomeIPStubAdapterGenerator::getInstance().generateStubAdapter(
                    interface, accessor, deployedProviders, genStub, m_destDir);
                std::cout << "Done!" << std::endl;
            }
        }
    }

    return true;
}
} // namespace BstCommonAPI