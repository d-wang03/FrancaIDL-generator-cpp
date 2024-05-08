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
#include "FTypeGenerator.h"
#include "FrancaGeneratorExtensions.h"
#include "generator/AbstractGenerator.h"
#include "model/FInterface.h"
#include <algorithm>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
namespace BstIdl
{
class FInterfaceGenerator : public AbstractGenerator
{
private:
    FInterfaceGenerator() = default;
    virtual ~FInterfaceGenerator() = default;

    std::string generateHeader(const std::shared_ptr<FInterface> fInterface);

    std::string generateSource(const std::shared_ptr<FInterface> fInterface);

    bool needsSourceComment(std::shared_ptr<FType> &_type);

    bool hasSourceFile(const std::shared_ptr<FInterface> &fInterface);

public:
    FInterfaceGenerator(const FInterfaceGenerator &) = delete;
    FInterfaceGenerator(FInterfaceGenerator &&) noexcept = delete;
    FInterfaceGenerator &operator=(const FInterfaceGenerator &) = delete;
    FInterfaceGenerator &operator=(FInterfaceGenerator &&) = delete;

    static FInterfaceGenerator &getInstance()
    {
        static FInterfaceGenerator instance;
        return instance;
    }
    bool generate() override
    {
        return true;
    };
    void generateInterface(const std::shared_ptr<FInterface> &fInterface, const std::string &dir);

    std::string generateInstanceHeader(const std::shared_ptr<FInterface> fInterface,
                                       std::list<std::string> &deployedInstances);

    void getRequiredHeaderFiles(const std::shared_ptr<FInterface> &fInterface, std::list<std::string> &generatedHeaders,
                                std::list<std::string> &libraryHeaders);
};
} // namespace BstIdl
#endif