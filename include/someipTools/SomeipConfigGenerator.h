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
#ifndef V_SOME_I_P_CONFIG_GENERATOR
#define V_SOME_I_P_CONFIG_GENERATOR

#include "FDModel/FDExtensionRoot.h"
#include "generator/AbstractGenerator.h"
#include "model/FInterface.h"
#include "someipTools/SomeipPropertyAccessor.h"

namespace BstCommonAPI
{
class SomeipConfigGenerator : public BstIdl::AbstractGenerator
{

public:
    SomeipConfigGenerator() = default;
    virtual ~SomeipConfigGenerator() = default;
    SomeipConfigGenerator(const SomeipConfigGenerator &) = default;
    SomeipConfigGenerator(SomeipConfigGenerator &&) noexcept = default;
    SomeipConfigGenerator &operator=(const SomeipConfigGenerator &) = default;
    SomeipConfigGenerator &operator=(SomeipConfigGenerator &&) = default;

    void generateConfigFiles(const std::list<std::shared_ptr<BstIdl::FDExtensionRoot>> &deployedProviders,
                             std::string &dir);
    static SomeipConfigGenerator &getInstance()
    {
        static SomeipConfigGenerator instance;
        return instance;
    }
    bool generate() override
    {
        return true;
    }

private:
    std::string generateClientConfigJson(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                         const std::shared_ptr<SomeipPropertyAccessor> &_accessor);
    std::string generateServerConfigJson(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                         const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

protected:
};

} // namespace BstCommonAPI
#endif
