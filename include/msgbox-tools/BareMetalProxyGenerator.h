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
#ifndef MSG_BOX_BAREMETAL_PROXY_GENERATOR_H
#define MSG_BOX_BAREMETAL_PROXY_GENERATOR_H

#include "FDModel/FDExtensionElement.h"
#include "FDModel/FDInterface.h"
#include "generator/AbstractGenerator.h"

namespace BstIdl
{

class BareMetalProxyGenerator : public AbstractGenerator
{
public:
    BareMetalProxyGenerator(const std::string &dest_dir, const std::string &src_id,
                            const std::shared_ptr<FDExtensionElement> &instance)
        : AbstractGenerator(dest_dir), m_srcEndID(src_id), m_instance(instance)
    {
    }
    ~BareMetalProxyGenerator() = default;
    bool generate() override;

private:
    bool validate();
    bool generateHeader();
    bool generateSource();
    std::string getMethodAsyncArgs(const std::shared_ptr<FMethod> &method);
    std::string getMethodAsyncSignature(const std::shared_ptr<FMethod> &method);
    std::string getBroadcastSignature(const std::shared_ptr<FBroadcast> &broadcast);
    bool getIntegerProperty(const std::shared_ptr<FDElement> &element, const std::string &property, int32_t *value);

    std::shared_ptr<FDExtensionElement> m_instance;
    std::shared_ptr<FDInterface> m_interface;
    std::string m_infName;
    std::string m_srcEndID;
    std::string m_dstEndID;
    std::string m_fid;
    std::string m_folderPath;
    uint8_t m_sid;
    int32_t m_maxMethodNum;
    int32_t m_maxBroadcastNum;
};

} // namespace BstIdl

#endif