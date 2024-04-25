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
#ifndef F_INTERFACE_GENERATOR
#define F_INTERFACE_GENERATOR
#include "CommonapiPropertyAccessor.h"
#include "FTypeGenerator.h"
#include "generator/AbstractGenerator.h"
#include "model/FInterface.h"
namespace BstCommonAPI
{
class FInterfaceGenerator : public BstIdl::AbstractGenerator
{
public:
    FInterfaceGenerator() = default;
    virtual ~FInterfaceGenerator() = default;
    FInterfaceGenerator(const FInterfaceGenerator &) = default;
    FInterfaceGenerator(FInterfaceGenerator &&) noexcept = default;
    FInterfaceGenerator &operator=(const FInterfaceGenerator &) = default;
    FInterfaceGenerator &operator=(FInterfaceGenerator &&) = default;

    static FInterfaceGenerator &getInstance()
    {
        static FInterfaceGenerator instance;
        return instance;
    }
    bool generate() override
    {
        return true;
    }
    void generateInterface(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                           const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor, bool generate_code,
                           const std::string &dir);
    void getRequiredHeaderFiles(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                std::list<std::string> &generatedHeaders, std::list<std::string> &libraryHeaders);

private:
    bool hasSourceFile(const std::shared_ptr<BstIdl::FInterface> &fInterface);
    bool needsSourceComment(std::shared_ptr<BstIdl::FType> &_type);
    std::string generateHeader(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                               const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor);
    std::string generateSource(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                               const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor);
};
} // namespace BstCommonAPI
#endif