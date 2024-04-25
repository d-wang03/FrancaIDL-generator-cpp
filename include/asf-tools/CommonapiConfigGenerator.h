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
#ifndef COMMON_API_CONFIG_GENERATOR
#define COMMON_API_CONFIG_GENERATOR
#include "capicxx-core-tools/FTypeGenerator.h"
#include "generator/AbstractGenerator.h"
#include "model/FInterface.h"
#include "model/FMethod.h"
#include <map>
#include <memory>

namespace BstASF
{

class CommonapiConfigGenerator : public BstIdl::AbstractGenerator
{
private:
    CommonapiConfigGenerator() = default;
    virtual ~CommonapiConfigGenerator() = default;

    std::string generateConfigHeader();
    std::string generateConfigSource();
    std::string generateInitialize();
    std::string getTargetName(std::shared_ptr<BstIdl::FDExtensionElement> instance);

public:
    bool generate() override
    {
        return true;
    }
    static CommonapiConfigGenerator &getInstance()
    {
        static CommonapiConfigGenerator instance;
        return instance;
    }

    CommonapiConfigGenerator(const CommonapiConfigGenerator &) = delete;
    CommonapiConfigGenerator(CommonapiConfigGenerator &&) noexcept = delete;
    CommonapiConfigGenerator &operator=(const CommonapiConfigGenerator &) = delete;
    CommonapiConfigGenerator &operator=(CommonapiConfigGenerator &&) = delete;

    void generateConfig(const std::string &dir);
};

} // namespace BstASF
#endif