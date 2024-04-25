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
#ifndef F_INTERFACE_SOME_I_P_PROXY_GENERATOR
#define F_INTERFACE_SOME_I_P_PROXY_GENERATOR
#include "FDModel/FDExtensionRoot.h"
#include "SomeipPropertyAccessor.h"
#include "generator/AbstractGenerator.h"
#include "model/FInterface.h"
namespace BstCommonAPI
{
class FInterfaceSomeIPProxyGenerator : public BstIdl::AbstractGenerator
{
public:
    FInterfaceSomeIPProxyGenerator() = default;
    virtual ~FInterfaceSomeIPProxyGenerator() = default;
    FInterfaceSomeIPProxyGenerator(const FInterfaceSomeIPProxyGenerator &) = default;
    FInterfaceSomeIPProxyGenerator(FInterfaceSomeIPProxyGenerator &&) noexcept = default;
    FInterfaceSomeIPProxyGenerator &operator=(const FInterfaceSomeIPProxyGenerator &) = default;
    FInterfaceSomeIPProxyGenerator &operator=(FInterfaceSomeIPProxyGenerator &&) = default;
    bool m_generateSyncCalls = true;
    void generateProxy(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                       const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                       std::list<std::shared_ptr<BstIdl::FDExtensionRoot>> &providers, bool generate__someip_code,
                       bool generateSyncCalls, std::string &dir);
    static FInterfaceSomeIPProxyGenerator &getInstance()
    {
        static FInterfaceSomeIPProxyGenerator instance;
        return instance;
    }
    bool generate() override
    {
        return true;
    }

private:
    std::string generateProxySource(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                    const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                    std::list<std::shared_ptr<BstIdl::FDExtensionRoot>> &providers);

    std::string generateProxyHeader(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                    const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getSomeIpServiceIDForInterface(std::list<std::shared_ptr<BstIdl::FDExtensionRoot>> &providers,
                                               const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string someipClassVariableName(const std::shared_ptr<BstIdl::FModelElement> &fModelElement);

    std::string someipClassVariableName(const std::shared_ptr<BstIdl::FBroadcast> &fBroadcast);

    std::string generateProxyHelperDeployments(const std::shared_ptr<BstIdl::FMethod> &_method,
                                               const std::shared_ptr<BstIdl::FInterface> &fInterface, bool _isAsync,
                                               const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateProxyHelperClass(const std::shared_ptr<BstIdl::FMethod> &_method,
                                         const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                         const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateInParams(const std::shared_ptr<BstIdl::FMethod> &_method,
                                 const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateOutParams(const std::shared_ptr<BstIdl::FMethod> &_method,
                                  const std::shared_ptr<SomeipPropertyAccessor> &_accessor, bool _instantiate);

    std::string generateOutParamsValue(const std::shared_ptr<BstIdl::FMethod> &_method,
                                       const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateCallback(const std::shared_ptr<BstIdl::FMethod> &_method,
                                 const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                 const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateCallbackParameter(const std::shared_ptr<BstIdl::FMethod> &_method,
                                          const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                          const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string someipClassName(const std::shared_ptr<BstIdl::FAttribute> &_attribute,
                                const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateVariableInit(const std::shared_ptr<BstIdl::FAttribute> &_attribute,
                                     const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                     const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string someipClassName(const std::shared_ptr<BstIdl::FBroadcast> &_broadcast,
                                const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                const std::shared_ptr<SomeipPropertyAccessor> &_accessor);
};
} // namespace BstCommonAPI
#endif