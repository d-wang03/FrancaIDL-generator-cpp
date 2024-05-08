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
#include "asf-tools/vsomeip-config/VSomeIPConfigGenerator.h"
#include "FDModel/FDeployedProvider.h"
#include "asf-tools/GeneralServiceGeneratorExtensions.h"
#include "utilities/string_utility.h"
#include <fstream>

namespace BstASF
{
void VSomeIPConfigGenerator::generateConfigFiles(const std::shared_ptr<BstIdl::FDExtensionRoot> &deployedProvider,
                                                 const std::shared_ptr<BstIdl::FDExtensionRoot> &someipDeployedProvider,
                                                 std::string &dir)
{
    if (dir.empty())
        return;

    std::shared_ptr<ASFPropertyAccessor> _accessor =
        std::make_shared<ASFPropertyAccessor>(std::make_shared<BstIdl::FDeployedProvider>(deployedProvider));
    auto server_path = dir + "/vsomeip-server.json";
    std::ofstream ServerWrite(server_path, std::ofstream::out);
    std::cout << "Generate config file for server"
              << "..." << std::endl;
    auto server_config = generateServerConfigJson(deployedProvider, someipDeployedProvider, _accessor);
    ServerWrite << replace_all(server_config, "\t", "    ");
    ServerWrite.close();

    auto client_path = dir + "/vsomeip-client.json";
    std::ofstream ClientWrite(client_path, std::ofstream::out);
    std::cout << "Generate config file for client"
              << "..." << std::endl;
    auto client_config = generateClientConfigJson(deployedProvider, _accessor);
    ClientWrite << replace_all(client_config, "\t", "    ");
    ClientWrite.close();
}
std::string VSomeIPConfigGenerator::generateClientConfigJson(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                                             const std::shared_ptr<ASFPropertyAccessor> &_accessor)
{
    auto &gen = BstASF::GeneralServiceGeneratorExtensions::getInstance();
    std::string content("{");
    std::list<std::string> modules;
    modules.emplace_back(gen.getClientHostMessage(_provider, _accessor));
    modules.emplace_back(gen.getLogging(_provider, _accessor));
    modules.emplace_back(gen.getApplications(_provider, _accessor));
    modules.emplace_back(gen.getServiceDiscovery(_provider, _accessor));
    content.append(join(modules, ",") + "\n}");
    return content;
}
std::string VSomeIPConfigGenerator::generateServerConfigJson(
    const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
    const std::shared_ptr<BstIdl::FDExtensionRoot> &_someipProvider,
    const std::shared_ptr<ASFPropertyAccessor> &_accessor)
{
    auto &gen = BstASF::GeneralServiceGeneratorExtensions::getInstance();
    std::string content("{");
    std::list<std::string> modules;

    modules.emplace_back(gen.getServerHostMessage(_provider, _accessor));
    modules.emplace_back(gen.getLogging(_provider, _accessor));
    modules.emplace_back(gen.getApplications(_provider, _accessor));
    modules.emplace_back(gen.getServices(_provider, _someipProvider, _accessor));
    modules.emplace_back(gen.getServiceDiscovery(_provider, _accessor));
    content.append(join(modules, ",") + "\n}");
    return content;
}

} // namespace BstASF