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
#ifndef CAPI_TESTCASE_GENERATOR_H
#define CAPI_TESTCASE_GENERATOR_H
#include "AbstractGenerator.h"
#include "FDModel/FDInterface.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace BstIdl
{
class CapiTestcaseGenerator : public AbstractGenerator
{
public:
    CapiTestcaseGenerator(const std::string &dest_dir) : AbstractGenerator(dest_dir)
    {
    }
    virtual ~CapiTestcaseGenerator() = default;

    bool generate() override;
    void initTypeMap() override;

private:
    bool createDirectory(const std::shared_ptr<FDInterface> &inf);
    bool generateHeader(const std::shared_ptr<FDInterface> &inf);
    bool generateSource(const std::shared_ptr<FDInterface> &inf);
    bool generateCMakeLists(const std::shared_ptr<FDInterface> &inf);

    nlohmann::json getTestCases(const std::shared_ptr<FDElement> &ele);
    std::string getSomeIPDomain(const std::shared_ptr<BstIdl::FInterface> &target, const std::string &instanceId);
    // methodTest-gen
    std::string getMethodArgDecl(const std::shared_ptr<FMethod> &method, const std::shared_ptr<FDInterface> &inf,
                                 const json &tc);
    std::string getMethodSyncCall(const std::shared_ptr<FMethod> &method);
    std::string getMethodAsyncCall(const std::shared_ptr<FMethod> &method);
    std::string getMethodArgCheck(const std::shared_ptr<FMethod> &method, const json &tc);
    // attribueTest-gen
    std::string getAttrDecl(const std::shared_ptr<FAttribute> &attr, const json &tc);
    std::string getAttrChanged(const std::shared_ptr<FAttribute> &attr);
    std::string getAttributeCall(const std::shared_ptr<FAttribute> &attr);
    std::string getAttributeAsyncCall(const std::shared_ptr<FAttribute> &attr);
    std::string getAttrCheck(const std::shared_ptr<FAttribute> &attr, const json &tc);
    std::string getAttributeType(const std::shared_ptr<FAttribute> &attr);
    // broadcastTest-gen
    std::string getBroadcastDeclAndSubscribe(const std::shared_ptr<FBroadcast> &broadcast);
    std::string getBroadcastCheck(const std::shared_ptr<FBroadcast> &broadcast, const json &tc);
    std::string getBroadcastType(const std::shared_ptr<BstIdl::FBroadcast> &bc);

    std::string m_folderPath;
};
} // namespace BstIdl

#endif
