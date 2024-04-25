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
#ifndef F_INTERFACE_LOGIC_DEFAULT_GENERATOR
#define F_INTERFACE_LOGIC_DEFAULT_GENERATOR
#include "capicxx-core-tools/CommonapiPropertyAccessor.h"
#include "capicxx-core-tools/FTypeGenerator.h"
#include "generator/AbstractGenerator.h"
#include "model/FInterface.h"
#include <map>
#include <memory>
namespace BstASF
{
class FInterfaceLogicDefaultGenerator : public BstIdl::AbstractGenerator
{
private:
    FInterfaceLogicDefaultGenerator() = default;
    virtual ~FInterfaceLogicDefaultGenerator() = default;
    std::list<std::string> slotDecls;
    std::string generateLogicDefaultHeader(
        const std::shared_ptr<BstIdl::FInterface> &fInterface,
        const std::shared_ptr<BstCommonAPI::CommonapiPropertyAccessor> &deploymentAccessor);
    std::string generateLogicDefaultSource(
        const std::shared_ptr<BstIdl::FInterface> &fInterface,
        const std::shared_ptr<BstCommonAPI::CommonapiPropertyAccessor> &deploymentAccessor);
    std::string generateLogicHeader(const std::shared_ptr<BstIdl::FDExtensionElement> &instance,
                                    const std::shared_ptr<BstCommonAPI::CommonapiPropertyAccessor> &deploymentAccessor);

    std::string generateLogicSource(const std::shared_ptr<BstIdl::FDExtensionElement> &instance,
                                    const std::shared_ptr<BstCommonAPI::CommonapiPropertyAccessor> &deploymentAccessor);

public:
    bool generate() override
    {
        return true;
    }
    static FInterfaceLogicDefaultGenerator &getInstance()
    {
        static FInterfaceLogicDefaultGenerator instance;
        return instance;
    }

    FInterfaceLogicDefaultGenerator(const FInterfaceLogicDefaultGenerator &) = delete;
    FInterfaceLogicDefaultGenerator(FInterfaceLogicDefaultGenerator &&) noexcept = delete;
    FInterfaceLogicDefaultGenerator &operator=(const FInterfaceLogicDefaultGenerator &) = delete;
    FInterfaceLogicDefaultGenerator &operator=(FInterfaceLogicDefaultGenerator &&) = delete;

    void generateLogicDefault(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                              const std::shared_ptr<BstCommonAPI::CommonapiPropertyAccessor> &deploymentAccessor,
                              const std::string &dir);

    std::list<std::string> &getSlotDecls()
    {
        return slotDecls;
    }
};
} // namespace BstASF
#endif
