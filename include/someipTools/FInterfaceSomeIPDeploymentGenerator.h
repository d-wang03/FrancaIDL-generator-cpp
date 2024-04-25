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
#ifndef F_INTERFACE_SOME_I_P_DEPLOYMENT_GENERATOR
#define F_INTERFACE_SOME_I_P_DEPLOYMENT_GENERATOR
#include "FTypeCollectionSomeIPDeploymentGenerator.h"
#include "model/FInterface.h"
namespace BstCommonAPI
{
class FInterfaceSomeIPDeploymentGenerator : public FTypeCollectionSomeIPDeploymentGenerator
{
public:
    FInterfaceSomeIPDeploymentGenerator() = default;
    virtual ~FInterfaceSomeIPDeploymentGenerator() = default;
    FInterfaceSomeIPDeploymentGenerator(const FInterfaceSomeIPDeploymentGenerator &) = default;
    FInterfaceSomeIPDeploymentGenerator(FInterfaceSomeIPDeploymentGenerator &&) noexcept = default;
    FInterfaceSomeIPDeploymentGenerator &operator=(const FInterfaceSomeIPDeploymentGenerator &) = default;
    FInterfaceSomeIPDeploymentGenerator &operator=(FInterfaceSomeIPDeploymentGenerator &&) = default;

    void generateDeployment(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                            const std::shared_ptr<SomeipPropertyAccessor> &_accessor, bool generate_code_someip,
                            std::string &dir);
    static FInterfaceSomeIPDeploymentGenerator &getInstance()
    {
        static FInterfaceSomeIPDeploymentGenerator instance;
        return instance;
    }
    bool generate() override
    {
        return true;
    }

private:
    std::string generateDeploymentHeader(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                         const std::shared_ptr<SomeipPropertyAccessor> &_accessor);
    std::string generateDeploymentSource(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                         const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

protected:
    std::string generateDeploymentDeclaration(const std::shared_ptr<BstIdl::FAttribute> &_attribute,
                                              const std::shared_ptr<BstIdl::FInterface> &_interface,
                                              const std::shared_ptr<SomeipPropertyAccessor> &_accessor);
    std::string generateDeploymentDeclaration(const std::shared_ptr<BstIdl::FArgument> &_argument,
                                              const std::shared_ptr<BstIdl::FMethod> &_method,
                                              const std::shared_ptr<BstIdl::FInterface> &_interface,
                                              const std::shared_ptr<SomeipPropertyAccessor> &_accessor);
    std::string generateDeploymentDeclaration(const std::shared_ptr<BstIdl::FArgument> &_argument,
                                              const std::shared_ptr<BstIdl::FBroadcast> &_broadcast,
                                              const std::shared_ptr<BstIdl::FInterface> &_interface,
                                              const std::shared_ptr<SomeipPropertyAccessor> &_accessor);
    std::string generateDeploymentDefinition(const std::shared_ptr<BstIdl::FAttribute> &_attribute,
                                             const std::shared_ptr<BstIdl::FInterface> &_interface,
                                             const std::shared_ptr<SomeipPropertyAccessor> &_accessor);
    std::string generateDeploymentDefinition(const std::shared_ptr<BstIdl::FArgument> &_argument,
                                             const std::shared_ptr<BstIdl::FMethod> &_method,
                                             const std::shared_ptr<BstIdl::FInterface> &_interface,
                                             const std::shared_ptr<SomeipPropertyAccessor> &_accessor);
    std::string generateDeploymentDefinition(const std::shared_ptr<BstIdl::FArgument> &_argument,
                                             const std::shared_ptr<BstIdl::FBroadcast> &_broadcast,
                                             const std::shared_ptr<BstIdl::FInterface> &_interface,
                                             const std::shared_ptr<SomeipPropertyAccessor> &_accessor);
};

} // namespace BstCommonAPI
#endif
