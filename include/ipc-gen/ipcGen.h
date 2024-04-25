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
#ifndef IPC_GEN_H
#define IPC_GEN_H
#include "generator/AbstractGenerator.h"
#include "ipcClientIncludeGen.h"
#include "ipcClientSourceGen.h"
#include "ipcServerIncludeGen.h"
#include "ipcServerSourceGen.h"
namespace BstIdl
{
class IPCGenerator : public AbstractGenerator
{
public:
    IPCGenerator(const std::string &dest_dir, bool generateStub)
        : AbstractGenerator(dest_dir), m_generateStub(generateStub)
    {
    }
    virtual ~IPCGenerator() = default;

    IPCGenerator(const IPCGenerator &) = delete;
    IPCGenerator(IPCGenerator &&) noexcept = delete;
    IPCGenerator &operator=(const IPCGenerator &) = delete;
    IPCGenerator &operator=(IPCGenerator &&) = delete;

    bool generate() override;
    void transformerDefine();
    void getClientAndServerID(const std::shared_ptr<FDInterface> &fdInterface, int &client_number, int &server_number);

private:
    bool m_generateStub;
};

} // namespace BstIdl
#endif