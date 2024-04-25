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
#include "FTypeGenerator.h"
#include "FrancaGeneratorExtensions.h"
#include "generator/AbstractGenerator.h"
#include "model/FInterface.h"
#include <algorithm>
#include <dirent.h>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
namespace BstIdl
{
class FInterfaceStubGenerator : public AbstractGenerator
{
private:
    FInterfaceStubGenerator() = default;
    virtual ~FInterfaceStubGenerator() = default;
    std::string generateStubHeader(const std::shared_ptr<FInterface> &fInterface);
    std::string generateStubSource(const std::shared_ptr<FInterface> &fInterface);

public:
    static FInterfaceStubGenerator &getInstance()
    {
        static FInterfaceStubGenerator instance;
        return instance;
    }

    FInterfaceStubGenerator(const FInterfaceStubGenerator &) = delete;
    FInterfaceStubGenerator(FInterfaceStubGenerator &&) noexcept = delete;
    FInterfaceStubGenerator &operator=(const FInterfaceStubGenerator &) = delete;
    FInterfaceStubGenerator &operator=(FInterfaceStubGenerator &&) = delete;

    bool generate() override
    {
        return true;
    }
    void generateStub(const std::shared_ptr<FInterface> &fInterface, bool generate_code, const std::string &dir);
};
} // namespace BstIdl
#endif