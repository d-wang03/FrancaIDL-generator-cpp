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
#include "capicxx-core-tools/CapiGenerator.h"
#include "FDModel/FDExtensionRoot.h"
#include "FDParser/FDParser.h"
#include "asf-tools/GeneralServiceGeneratorExtensions.h"
#include "capicxx-core-tools/FInterfaceGenerator.h"
#include "capicxx-core-tools/FInterfaceProxyGenerator.h"
#include "capicxx-core-tools/FInterfaceStubGenerator.h"
#include "capicxx-core-tools/FTypeCollectionGenerator.h"
#include "capicxx-core-tools/FrancaGeneratorExtensions.h"
#include "parser/FidlParser.h"
#include "utilities/string_utility.h"
#include <experimental/filesystem>

namespace BstCommonAPI
{

bool CommonapiGenerator::generate()
{
    if (m_destDir.empty())
        return false;

    auto &fmodels = BstIdl::FModelManager::getInstance();
    auto &fdmodels = BstIdl::FDModelManager::getInstance();
    if (fmodels.size() == 0 && fdmodels.size() == 0)
    {
        std::cerr << "No valid models." << std::endl;
        return false;
    }

    std::list<std::shared_ptr<BstIdl::FDInterface>> deployedInterfaces;
    std::list<std::shared_ptr<BstIdl::FDTypes>> deployedTypes;
    std::list<std::shared_ptr<BstIdl::FDExtensionRoot>> deployedProviders;
    std::list<std::string> deployedImports;
    for (const auto &fdmodel : fdmodels.getModelList())
    {
        // fill all deployed interfaces and types.
        for (const auto &item : fdmodel->getDeployments())
        {
            if (auto ptr = std::dynamic_pointer_cast<BstIdl::FDExtensionRoot>(item))
            {
                deployedProviders.emplace_back(ptr);
                continue;
            }
            auto spec = item->getSpec();
            if (!spec)
                continue;
            auto spec_name = spec->getName();
            if (contains(spec_name, "core.deployment"))
            {
                if (auto ptr = std::dynamic_pointer_cast<BstIdl::FDInterface>(item))
                    deployedInterfaces.emplace_back(ptr);
                else if (auto ptr = std::dynamic_pointer_cast<BstIdl::FDTypes>(item))
                    deployedTypes.emplace_back(ptr);
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
            std::shared_ptr<CommonapiPropertyAccessor> accessor;
            auto it = std::find_if(deployedInterfaces.cbegin(), deployedInterfaces.cend(),
                                   [interface](const auto &item) { return item->getTarget() == interface; });

            if (it == deployedInterfaces.cend())
                accessor = std::make_shared<CommonapiPropertyAccessor>();
            else
                accessor =
                    std::make_shared<CommonapiPropertyAccessor>(std::make_shared<BstIdl::FDeployedInterface>(*it));
            FrancaGeneratorExtensions::getInstance().insertAccessor(interface, accessor);
        }
        for (const auto &typecollection : fmodel->getTypeCollections())
        {
            std::shared_ptr<CommonapiPropertyAccessor> accessor;
            auto it = std::find_if(deployedTypes.cbegin(), deployedTypes.cend(),
                                   [typecollection](const auto &item) { return item->getTarget() == typecollection; });

            if (it == deployedTypes.cend())
                accessor = std::make_shared<CommonapiPropertyAccessor>();
            else
                accessor =
                    std::make_shared<CommonapiPropertyAccessor>(std::make_shared<BstIdl::FDeployedTypeCollection>(*it));
            FrancaGeneratorExtensions::getInstance().insertAccessor(typecollection, accessor);
        }
    }
    std::cout << "Done!" << std::endl;
    // generate code.
    std::cout << "Start code generation" << std::endl;
    const auto server_Infs =
        BstASF::GeneralServiceGeneratorExtensions::getInstance().getServerTypeInterfacesIncludingBase();
    const auto client_Infs =
        BstASF::GeneralServiceGeneratorExtensions::getInstance().getClientTypeInterfacesIncludingBase();
    const bool isEmptyASFServer = BstASF::GeneralServiceGeneratorExtensions::getInstance().isEmptyServer();
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
            auto accessor = FrancaGeneratorExtensions::getInstance().getAccessor(typecollection);
            FTypeCollectionGenerator::getInstance().setLicense(getLicense());
            FTypeCollectionGenerator::getInstance().generate(typecollection, accessor, true, m_destDir);
            std::cout << "Done!" << std::endl;
        }
        for (const auto &interface : fmodel->getInterfaces())
        {
            auto accessor = FrancaGeneratorExtensions::getInstance().getAccessor(interface);
            std::cout << "Generate code for " << interface->getName() << "...";
            FInterfaceGenerator::getInstance().setLicense(getLicense());
            FInterfaceGenerator::getInstance().generateInterface(interface, accessor, true, m_destDir);
            std::cout << "Done!" << std::endl;
            // client or server
            if (isEmptyASFServer)
            {
                std::cout << "Generate proxy code for " << interface->getName() << "...";
                FInterfaceProxyGenerator::getInstance().setLicense(getLicense());
                FInterfaceProxyGenerator::getInstance().generateProxy(interface, accessor, true, true, m_destDir);
                std::cout << "Done!" << std::endl;
                std::cout << "Generate stub code for " << interface->getName() << "...";
                FInterfaceStubGenerator::getInstance().setLicense(getLicense());
                FInterfaceStubGenerator::getInstance().generateStub(interface, accessor, m_generateStub, true,
                                                                    m_destDir);
                std::cout << "Done!" << std::endl;
            }
            else
            {
                bool genStub = contains(server_Infs, interface);
                bool genProxy = contains(client_Infs, interface);
                std::cout << "Generate proxy code for " << interface->getName() << "...";
                FInterfaceProxyGenerator::getInstance().setLicense(getLicense());
                FInterfaceProxyGenerator::getInstance().generateProxy(interface, accessor, genProxy, true, m_destDir);
                std::cout << "Done!" << std::endl;
                std::cout << "Generate stub code for " << interface->getName() << "...";
                FInterfaceStubGenerator::getInstance().setLicense(getLicense());
                FInterfaceStubGenerator::getInstance().generateStub(interface, accessor, genStub, true, m_destDir);
                std::cout << "Done!" << std::endl;
            }
        }
    }

    return true;
}

} // namespace BstCommonAPI