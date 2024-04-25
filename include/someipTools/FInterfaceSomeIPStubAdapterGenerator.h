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
#ifndef F_INTERFACE_SOME_I_P_STUB_ADAPTER_GENERATOR
#define F_INTERFACE_SOME_I_P_STUB_ADAPTER_GENERATOR
#include "FDModel/FDExtensionRoot.h"
#include "SomeipPropertyAccessor.h"
#include "generator/AbstractGenerator.h"
#include "model/FInterface.h"
namespace BstCommonAPI
{
class FInterfaceSomeIPStubAdapterGenerator : public BstIdl::AbstractGenerator
{
public:
    FInterfaceSomeIPStubAdapterGenerator() = default;
    virtual ~FInterfaceSomeIPStubAdapterGenerator() = default;
    FInterfaceSomeIPStubAdapterGenerator(const FInterfaceSomeIPStubAdapterGenerator &) = default;
    FInterfaceSomeIPStubAdapterGenerator(FInterfaceSomeIPStubAdapterGenerator &&) noexcept = default;
    FInterfaceSomeIPStubAdapterGenerator &operator=(const FInterfaceSomeIPStubAdapterGenerator &) = default;
    FInterfaceSomeIPStubAdapterGenerator &operator=(FInterfaceSomeIPStubAdapterGenerator &&) = default;
    static FInterfaceSomeIPStubAdapterGenerator &getInstance()
    {
        static FInterfaceSomeIPStubAdapterGenerator instance;
        return instance;
    }
    bool generate() override
    {
        return true;
    }
    void generateStubAdapter(const std::shared_ptr<BstIdl::FInterface> &_interface,
                             const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                             std::list<std::shared_ptr<BstIdl::FDExtensionRoot>> &providers, bool generate_someip_code,
                             std::string &dir);

    std::string dispatcherTableEntry(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                     const std::string &identifierAsHexString, const std::string &memberFunctionName);

private:
    std::string generateStubAdapterHeader(const std::shared_ptr<BstIdl::FInterface> &_interface,
                                          const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateAttributeDispatcherDeclarations(const std::shared_ptr<BstIdl::FInterface> &_interface,
                                                        const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateMethodDispatcherDeclarations(const std::shared_ptr<BstIdl::FInterface> &_interface,
                                                     const std::shared_ptr<BstIdl::FInterface> &_container,
                                                     std::map<std::string, int> &_counters,
                                                     std::map<std::shared_ptr<BstIdl::FMethod>, int> &_methods,
                                                     const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateAttributeDispatcherDefinitions(const std::shared_ptr<BstIdl::FAttribute> &_attribute,
                                                       const std::shared_ptr<BstIdl::FInterface> &_interface,
                                                       const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateMethodDispatcherDefinitions(const std::shared_ptr<BstIdl::FMethod> &_method,
                                                    const std::shared_ptr<BstIdl::FInterface> &_interface,
                                                    const std::shared_ptr<BstIdl::FInterface> &_thisInterface,
                                                    const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                    std::map<std::string, int> &counterMap,
                                                    std::map<std::shared_ptr<BstIdl::FMethod>, int> &methodnumberMap);

    std::string getDeployedTypeName(const std::shared_ptr<BstIdl::FArgument> &_arg,
                                    const std::shared_ptr<BstIdl::FInterface> &_interface,
                                    const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getDeployedElementName(const std::shared_ptr<BstIdl::FArgument> &_arg,
                                       const std::shared_ptr<BstIdl::FInterface> &_interface,
                                       const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getInterfaceHierarchy(const std::shared_ptr<BstIdl::FInterface> &_interface);

    std::string generateStubAdapterSource(const std::shared_ptr<BstIdl::FInterface> &_interface,
                                          const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                          std::list<std::shared_ptr<BstIdl::FDExtensionRoot>> &providers);

    std::string generateAttributeDispatcherTableContent(const std::shared_ptr<BstIdl::FInterface> &_interface);

    std::string generateMethodDispatcherTableContent(const std::shared_ptr<BstIdl::FInterface> &_interface,
                                                     std::map<std::string, int> &_counters,
                                                     std::map<std::shared_ptr<BstIdl::FMethod>, int> &_methods);

    std::string someipStubAdapterHeaderFile(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string someipStubAdapterHeaderPath(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string someipStubAdapterSourceFile(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string someipStubAdapterSourcePath(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string someipStubAdapterClassName(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string someipStubAdapterClassNameInternal(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string someipStubAdapterHelperClassName(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string getAllInTypes(const std::shared_ptr<BstIdl::FMethod> &fMethod);

    std::string getAllOutTypes(const std::shared_ptr<BstIdl::FMethod> &fMethod);

    std::string someipStubDispatcherVariable(const std::shared_ptr<BstIdl::FMethod> &fMethod);

    std::string someipGetStubDispatcherVariable(const std::shared_ptr<BstIdl::FAttribute> &fAttribute);

    std::string someipSetStubDispatcherVariable(const std::shared_ptr<BstIdl::FAttribute> &fAttribute);

    std::string generateFireChangedMethodBody(const std::shared_ptr<BstIdl::FAttribute> &_attribute,
                                              const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                              const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateStubAttributeTableInitializer(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                                      const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateErrorReplyCallback(const std::shared_ptr<BstIdl::FBroadcast> &_broadcast,
                                           const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                           const std::shared_ptr<BstIdl::FMethod> &fMethod,
                                           const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string recursivelyGenerateMethodDispatcherDeclarations(
        const std::shared_ptr<BstIdl::FInterface> &_interface, const std::shared_ptr<BstIdl::FInterface> &_container,
        std::map<std::string, int> &_counters, std::map<std::shared_ptr<BstIdl::FMethod>, int> &_methods,
        const std::shared_ptr<SomeipPropertyAccessor> &_accessor);
};
} // namespace BstCommonAPI
#endif