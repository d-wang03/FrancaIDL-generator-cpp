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
#ifndef F_INTERFACE_PROXY_GENERATOR
#define F_INTERFACE_PROXY_GENERATOR
#include "CommonapiPropertyAccessor.h"
#include "FTypeGenerator.h"
#include "generator/AbstractGenerator.h"
#include "model/FInterface.h"
namespace BstCommonAPI
{
class FInterfaceProxyGenerator : public BstIdl::AbstractGenerator
{
public:
    FInterfaceProxyGenerator() = default;
    virtual ~FInterfaceProxyGenerator() = default;
    FInterfaceProxyGenerator(const FInterfaceProxyGenerator &) = default;
    FInterfaceProxyGenerator(FInterfaceProxyGenerator &&) noexcept = default;
    FInterfaceProxyGenerator &operator=(const FInterfaceProxyGenerator &) = default;
    FInterfaceProxyGenerator &operator=(FInterfaceProxyGenerator &&) = default;

    bool m_generateSyncCalls = true;

    static FInterfaceProxyGenerator &getInstance()
    {
        static FInterfaceProxyGenerator instance;
        return instance;
    }
    bool generate() override
    {
        return true;
    }
    void generateProxy(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                       const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor, bool generate_code,
                       bool generateSyncCalls, const std::string &dir);

private:
    std::string generateProxyBaseHeader(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                        const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor);
    std::string generateAsyncCallbackTypedefs(const std::shared_ptr<BstIdl::FInterface> &fInterface);
    std::string generateProxyHeader(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                    const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor);
    std::string getCommonApiBaseClassname(const std::shared_ptr<BstIdl::FAttribute> &fAttribute);
    std::string getProxyDefaultClassName(const std::shared_ptr<BstIdl::FInterface> &fInterface);
    std::string getExtensionClassName(const std::shared_ptr<BstIdl::FAttribute> &fAttribute);
    std::string getExtensionsSubnamespace(const std::shared_ptr<BstIdl::FInterface> &fInterface);
    std::string generateExtension(const std::shared_ptr<BstIdl::FAttribute> &fAttribute,
                                  const std::shared_ptr<BstIdl::FInterface> &fInterface);
    std::string generateSyncVariableList(const std::shared_ptr<BstIdl::FMethod> &fMethod);
    std::string generateASyncVariableList(const std::shared_ptr<BstIdl::FMethod> &fMethod);
};
} // namespace BstCommonAPI
#endif