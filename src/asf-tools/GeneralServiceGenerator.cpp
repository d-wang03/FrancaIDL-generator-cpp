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
#include "asf-tools/GeneralServiceGenerator.h"
#include "FDModel/FDModelManager.h"
#include "asf-tools/AutoStartUpConfigGenerator.h"
#include "asf-tools/CMakeGenerator.h"
#include "asf-tools/CommonapiConfigGenerator.h"
#include "asf-tools/FInterfaceLogicDefaultGenerator.h"
#include "asf-tools/FInterfaceStubImplGenerator.h"
#include "asf-tools/GeneralServiceGeneratorExtensions.h"
#include "asf-tools/MainFileGenerator.h"
#include "asf-tools/TypeConversionGenerator.h"
#include "model/FireAndForgetLimits.h"
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

namespace BstASF
{

bool GeneralServiceGenerator::generate()
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

    auto &gen = GeneralServiceGeneratorExtensions::getInstance();
    gen.clearServer();
    std::list<std::shared_ptr<BstIdl::FDInterface>> deployedInterfaces;
    std::list<std::shared_ptr<BstIdl::FDTypes>> deployedTypes;
    std::list<std::shared_ptr<BstIdl::FDInterface>> coreDeployedInterfaces;
    std::list<std::shared_ptr<BstIdl::FDTypes>> coreDeployedTypes;
    std::list<std::shared_ptr<BstIdl::FDExtensionRoot>> deployedProviders;
    std::list<std::string> deployedImports;
    std::list<std::string> spec_names;
    for (const auto &fdmodel : fdmodels.getModelList())
    {
        // get m_mappers root mapper
        for (const auto &item : fdmodel->getDeployments())
        {
            auto spec = item->getSpec();
            if (!spec)
                continue;
            auto spec_name = spec->getName();
            spec_names.emplace_back(spec_name);

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
                {
                    addListItem(deployedProviders, ptr);
                    gen.initSomeipProviderAccessor(ptr);
                }
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
            gen.mergeDeployments(s, t);
    for (auto t : deployedTypes)
        for (auto s : coreDeployedTypes)
            gen.mergeDeployments(s, t);

    std::cout << "Creating accessors...";
    for (const auto &fmodel : fmodels.getModelList())
    {
        auto it = std::find_if(deployedImports.cbegin(), deployedImports.cend(),
                               [fmodel](std::string item) { return contains(fmodel->getFilenames(), item); });
        if (it == deployedImports.cend())
            continue;
        // generate Access
        for (const auto &interface : fmodel->getInterfaces())
        {
            std::shared_ptr<BstCommonAPI::SomeipPropertyAccessor> accessor;
            auto it = std::find_if(deployedInterfaces.cbegin(), deployedInterfaces.cend(),
                                   [interface](const auto &item) { return item->getTarget() == interface; });

            if (it == deployedInterfaces.cend())
                accessor = std::make_shared<BstCommonAPI::SomeipPropertyAccessor>();
            else
                accessor = std::make_shared<BstCommonAPI::SomeipPropertyAccessor>(
                    std::make_shared<BstIdl::FDeployedInterface>(*it));
            gen.insertSomeipAccessor(interface, accessor);
        }
    }
    if (auto asf_depl = fdmodels.findProviderDeployment("com.bst.os.idl.asf.deployment"))
    {
        // get all servers of asf depl
        auto &method_limits = BstIdl::FireAndForgetLimits::getInstance();
        addListItem(deployedProviders, asf_depl);
        gen.initAccessor(asf_depl);
        for (const auto &instance : asf_depl->getElements())
        {
            if (!instance)
                continue;
            if (auto inf = std::dynamic_pointer_cast<BstIdl::FInterface>(instance->getTarget()))
            {
                gen.addServer(inf, instance);
                method_limits.addAsfInterface(inf);
                // record & simulate
                gen.addRecordAndSimulateInterface(instance);
            }
        }
    }
    std::cout << "Done!" << std::endl;

    // generate code.
    std::cout << "Start code generation" << std::endl;

    std::cout << "Generate auto-startup config files ..." << std::endl;
    auto asf_provider = fdmodels.findProviderDeployment("com.bst.os.idl.asf.deployment");
    auto asf_accessor = gen.getAccessor();
    AutoStartUpConfigGenerator::getInstance().setLicense(getLicense());
    AutoStartUpConfigGenerator::getInstance().generateConfigFiles(asf_provider, asf_accessor, m_destDir);
    std::cout << "Done!" << std::endl;

    auto server_infs = gen.getServerTypeInterfaces();
    if (server_infs.empty())
        return true;

    for (const auto &fmodel : fmodels.getModelList())
    {
        auto it = std::find_if(deployedImports.cbegin(), deployedImports.cend(), [fmodel](std::string item) {
            auto ret = contains(fmodel->getFilenames(), item);
            return ret;
        });
        if (it == deployedImports.cend())
            continue;

        for (const auto &interface : fmodel->getInterfaces())
        {
            // prepare slots
            gen.getSlotList(interface);

            auto isServer = contains(server_infs, interface);
            auto instances = gen.getServer(interface);
            auto accessor = gen.getSomeipAccessor(interface);
            if (isServer)
            {
                BstASF::FInterfaceStubImplGenerator &stubImpl = FInterfaceStubImplGenerator::getInstance();
                std::cout << "Generate stubImpl code for " << interface->getName() << "...";
                stubImpl.setLicense(getLicense());
                stubImpl.generateStubImpl(interface, accessor, m_destDir);
                std::cout << "Done!" << std::endl;
                std::cout << "Generate LogicDefault code for server: " << interface->getName() << "...";
                FInterfaceLogicDefaultGenerator::getInstance().setLicense(getLicense());
                FInterfaceLogicDefaultGenerator::getInstance().generateLogicDefault(interface, accessor, m_destDir);
                std::cout << "Done!" << std::endl;
            }
        }
    }

    std::cout << "Generate main code for all these models "
              << "...";
    MainFileGenerator::getInstance().setLicense(getLicense());
    MainFileGenerator::getInstance().generateMain(m_destDir);
    std::cout << "Done!" << std::endl;

    std::cout << "Generate config code for all these models "
              << "...";
    CommonapiConfigGenerator::getInstance().setLicense(getLicense());
    CommonapiConfigGenerator::getInstance().generateConfig(m_destDir);
    std::cout << "Done!" << std::endl;

    std::cout << "Generate CMakeLists.txt "
              << "...";
    auto iftest_gen = contains(join(spec_names, ","), "com.bst.os.idl.test.deployment");
    CMakeGenerator::getInstance().generateCMake(m_destDir, iftest_gen);
    std::cout << "Done!" << std::endl;

    std::cout << "Generate typeconversion code for interfaces..." << std::endl;
    TypeConversionGenerator::getInstance().setLicense(getLicense());
    TypeConversionGenerator::getInstance().generateConversionFile(m_destDir);
    std::cout << "Done!" << std::endl;

    return true;
}

} // namespace BstASF
