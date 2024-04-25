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
#ifndef MAIN_FILE_GENERATOR
#define MAIN_FILE_GENERATOR
#include "capicxx-core-tools/FTypeGenerator.h"
#include "generator/AbstractGenerator.h"
#include "model/FInterface.h"
#include "model/FMethod.h"
#include <map>
#include <memory>

namespace BstASF
{

class MainFileGenerator : public BstIdl::AbstractGenerator
{
private:
    MainFileGenerator() = default;
    virtual ~MainFileGenerator() = default;

    std::string generateMainSource();
    std::string generateConfigDriverStr();
    std::string generateConfigDispatcherStr(std::string &configLogic, std::string &connectObjects,
                                            std::string &addObjects);
    std::string generateInitializeServersStr(const std::list<std::shared_ptr<BstIdl::FDExtensionElement>> &instances);

public:
    bool generate() override
    {
        return true;
    }

    static MainFileGenerator &getInstance()
    {
        static MainFileGenerator instance;
        return instance;
    }

    MainFileGenerator(const MainFileGenerator &) = delete;
    MainFileGenerator(MainFileGenerator &&) noexcept = delete;
    MainFileGenerator &operator=(const MainFileGenerator &) = delete;
    MainFileGenerator &operator=(MainFileGenerator &&) = delete;

    void generateMain(const std::string &dir);
};

} // namespace BstASF
#endif