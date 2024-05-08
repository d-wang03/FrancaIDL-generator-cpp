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
#include "asf-tools/ASFPropertyAccessor.h"
#include "generator/AbstractGenerator.h"
#include "model/FInterface.h"

namespace BstASF
{
class VSomeIPConfigGenerator : public BstIdl::AbstractGenerator
{

public:
    VSomeIPConfigGenerator() = default;
    virtual ~VSomeIPConfigGenerator() = default;
    VSomeIPConfigGenerator(const VSomeIPConfigGenerator &) = default;
    VSomeIPConfigGenerator(VSomeIPConfigGenerator &&) noexcept = default;
    VSomeIPConfigGenerator &operator=(const VSomeIPConfigGenerator &) = default;
    VSomeIPConfigGenerator &operator=(VSomeIPConfigGenerator &&) = default;

    void generateConfigFiles(const std::shared_ptr<BstIdl::FDExtensionRoot> &deployedProvider,
                             const std::shared_ptr<BstIdl::FDExtensionRoot> &someipDeployedProvider, std::string &dir);
    static VSomeIPConfigGenerator &getInstance()
    {
        static VSomeIPConfigGenerator instance;
        return instance;
    }
    bool generate() override
    {
        return true;
    }

private:
    std::string generateClientConfigJson(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                         const std::shared_ptr<ASFPropertyAccessor> &_accessor);
    std::string generateServerConfigJson(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                         const std::shared_ptr<BstIdl::FDExtensionRoot> &someipDeployedProvider,
                                         const std::shared_ptr<ASFPropertyAccessor> &_accessor);

protected:
};

} // namespace BstASF
#endif
