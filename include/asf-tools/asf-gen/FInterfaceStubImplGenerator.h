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
#ifndef F_INTERFACE_STUB_IMPL_GENERATOR
#define F_INTERFACE_STUB_IMPL_GENERATOR
#include "capicxx-core-tools/FTypeGenerator.h"
#include "generator/AbstractGenerator.h"
#include "model/FInterface.h"
#include "model/FMethod.h"
#include <map>
#include <memory>

namespace BstASF
{

class FInterfaceStubImplGenerator : public BstIdl::AbstractGenerator
{
private:
    FInterfaceStubImplGenerator() = default;
    virtual ~FInterfaceStubImplGenerator() = default;

    std::string generateStubImplHeader(
        const std::shared_ptr<BstIdl::FDExtensionElement> &instance,
        const std::shared_ptr<BstCommonAPI::CommonapiPropertyAccessor> &deploymentAccessor);
    std::string generateStubImplSource(
        const std::shared_ptr<BstIdl::FDExtensionElement> &instance,
        const std::shared_ptr<BstCommonAPI::CommonapiPropertyAccessor> &deploymentAccessor);
    std::string generateMethodReplyDeclarations(
        const std::shared_ptr<BstCommonAPI::CommonapiPropertyAccessor> &deploymentAccessor,
        const std::shared_ptr<BstIdl::FMethod> &fMethod, const std::shared_ptr<BstIdl::FInterface> &fInterface);
    /*
        std::string getBroadcastDefine(const std::shared_ptr<BstIdl::FBroadcast> &broadcast,
                                       const std::shared_ptr<BstIdl::FDExtensionElement> &instance);
        std::string getGetAttributeDefine(const std::shared_ptr<BstIdl::FAttribute> &attribute,
                                          const std::shared_ptr<BstIdl::FDExtensionElement> &instance);
        std::string getSetAttributeDefine(const std::shared_ptr<BstIdl::FAttribute> &attribute,
                                          const std::shared_ptr<BstIdl::FDExtensionElement> &instance);
    */
public:
public:
    std::map<std::shared_ptr<BstIdl::FMethod>, std::map<std::string, bool>> m_methodrepliesMap;
    std::map<std::string, int> m_counterMap;

    bool generate() override
    {
        return true;
    }

    static FInterfaceStubImplGenerator &getInstance()
    {
        static FInterfaceStubImplGenerator instance;
        return instance;
    }

    FInterfaceStubImplGenerator(const FInterfaceStubImplGenerator &) = delete;
    FInterfaceStubImplGenerator(FInterfaceStubImplGenerator &&) noexcept = delete;
    FInterfaceStubImplGenerator &operator=(const FInterfaceStubImplGenerator &) = delete;
    FInterfaceStubImplGenerator &operator=(FInterfaceStubImplGenerator &&) = delete;

    void generateStubImpl(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                          const std::shared_ptr<BstCommonAPI::CommonapiPropertyAccessor> &deploymentAccessor,
                          const std::string &dir);
    void clear()
    {
        m_counterMap.clear();
        m_methodrepliesMap.clear();
    }
};

} // namespace BstASF
#endif
