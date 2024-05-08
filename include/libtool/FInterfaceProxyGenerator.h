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
#ifndef F_INTERFACE_PROXY_GENERATOR
#define F_INTERFACE_PROXY_GENERATOR
#include "FTypeGenerator.h"
#include "FrancaGeneratorExtensions.h"
#include "generator/AbstractGenerator.h"
#include "model/FInterface.h"

namespace BstIdl
{
class FInterfaceProxyGenerator : public AbstractGenerator
{
private:
    FInterfaceProxyGenerator() = default;
    virtual ~FInterfaceProxyGenerator() = default;

    std::string generateProxyHeader(const std::shared_ptr<FInterface> &fInterface);
    std::string generateProxySource(const std::shared_ptr<FInterface> &fInterface);
    std::string getProxyClassName(const std::shared_ptr<FInterface> &fInterface);

public:
    static FInterfaceProxyGenerator &getInstance()
    {
        static FInterfaceProxyGenerator instance;
        return instance;
    }

    FInterfaceProxyGenerator(const FInterfaceProxyGenerator &) = delete;
    FInterfaceProxyGenerator(FInterfaceProxyGenerator &&) noexcept = delete;
    FInterfaceProxyGenerator &operator=(const FInterfaceProxyGenerator &) = delete;
    FInterfaceProxyGenerator &operator=(FInterfaceProxyGenerator &&) = delete;

    bool generate() override
    {
        return true;
    }
    void generateProxy(const std::shared_ptr<FInterface> &fInterface, const std::string &dir);
};
} // namespace BstIdl
#endif