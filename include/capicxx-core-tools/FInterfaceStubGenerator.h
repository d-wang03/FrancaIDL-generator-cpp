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
#ifndef F_INTERFACE_STUB_GENERATOR
#define F_INTERFACE_STUB_GENERATOR
#include "CommonapiPropertyAccessor.h"
#include "FTypeGenerator.h"
#include "generator/AbstractGenerator.h"
#include "model/FInterface.h"
namespace BstCommonAPI
{
class FInterfaceStubGenerator : public BstIdl::AbstractGenerator
{
public:
    std::map<std::string, int> m_counterMap;
    std::map<std::shared_ptr<BstIdl::FMethod>, std::map<std::string, bool>> m_methodrepliesMap;
    FInterfaceStubGenerator() = default;
    virtual ~FInterfaceStubGenerator() = default;
    FInterfaceStubGenerator(const FInterfaceStubGenerator &) = default;
    FInterfaceStubGenerator(FInterfaceStubGenerator &&) noexcept = default;
    FInterfaceStubGenerator &operator=(const FInterfaceStubGenerator &) = default;
    FInterfaceStubGenerator &operator=(FInterfaceStubGenerator &&) = default;

    static FInterfaceStubGenerator &getInstance()
    {
        static FInterfaceStubGenerator instance;
        return instance;
    }
    bool generate() override
    {
        return true;
    }
    void generateStub(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                      const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor, bool generate_code,
                      bool generate_skeleton, const std::string &dir);

private:
    std::string generateStubHeader(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                   const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor);
    std::string generateStubDefaultHeader(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                          const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor);
    std::string generateStubDefaultSource(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                          const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor);
    std::string getStubAdapterAttributeMutexName(const std::shared_ptr<BstIdl::FAttribute> &fAttribute);
    std::string getStubDefaultClassSetMethodName(const std::shared_ptr<BstIdl::FAttribute> &fAttribute);
    std::string generateMethodReplyDeclarations(
        const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor,
        const std::shared_ptr<BstIdl::FMethod> &fMethod, const std::shared_ptr<BstIdl::FInterface> &fInterface,
        std::map<std::string, int> &counterMap,
        std::map<std::shared_ptr<BstIdl::FMethod>, std::map<std::string, bool>> &methodrepliesMap);
    std::string getStubDefaultClassTrySetMethodName(const std::shared_ptr<BstIdl::FAttribute> &fAttribute);
    std::string getStubDefaultClassValidateMethodName(const std::shared_ptr<BstIdl::FAttribute> &fAttribute);
    std::string getStubDefaultClassVariableName(const std::shared_ptr<BstIdl::FAttribute> &fAttribute);
};
} // namespace BstCommonAPI
#endif