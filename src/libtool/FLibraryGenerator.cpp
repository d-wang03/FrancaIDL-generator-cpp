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
#include "libtool/FLibraryGenerator.h"
#include "FDParser/FDParser.h"
#include "parser/FidlParser.h"

namespace BstIdl
{
bool FLibraryGenerator::generate()
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

    for (auto model : fmodels.getModelList())
    {
        auto interfaces = model->getInterfaces();
        for (auto interface : interfaces)
        {
            if (!interface->getVersion())
            {
                std::cerr << "No version defined in interface " << interface->getName() << std::endl;
                return false;
            }
            auto &interface_gen = FInterfaceGenerator::getInstance();
            auto &interface_proxy_gen = FInterfaceProxyGenerator::getInstance();
            auto &interface_stub_gen = FInterfaceStubGenerator::getInstance();
            interface_gen.generateInterface(interface, m_destDir);
            interface_proxy_gen.generateProxy(interface, m_destDir);
            interface_stub_gen.generateStub(interface, m_generateStub, m_destDir);
        }
        auto typeCollections = model->getTypeCollections();
        for (auto typeCollection : typeCollections)
        {
            auto ins = FTypeCollectionGenerator::getInstance();
            ins.generate(typeCollection, m_destDir);
        }
    }

    return true;
}
} // namespace BstIdl