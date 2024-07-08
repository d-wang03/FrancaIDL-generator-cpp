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
#ifndef MSG_BOX_GENERATOR_H
#define MSG_BOX_GENERATOR_H

#include "generator/AbstractGenerator.h"
#include "FDModel/FDExtensionRoot.h"

namespace BstIdl
{

#define MSG_BOX_VERSION "1.2.0"

class MsgBoxGenerator : public AbstractGenerator
{
public:
    MsgBoxGenerator(const std::string &dest_dir) : AbstractGenerator(dest_dir)
    {
        setLicense(R"(/* SPDX-License-Identifier: GPL-2.0 OR Apache 2.0
 *
 * Copyright (c) 2024 Black Sesame Technologies
 *
 * This program is also distributed under the terms of the Apache 2.0
 * License.
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
 */)");
    }
    ~MsgBoxGenerator() override = default;

    bool generate() override;

    static std::string version(){return MSG_BOX_VERSION;}

    static std::string commit_hash();

private:
    bool generateStubHeader(const std::shared_ptr<FDExtensionRoot> &provider);
    bool generateStubSource(const std::shared_ptr<FDExtensionRoot> &provider);
    bool generateProxyHeader(const std::shared_ptr<FDExtensionRoot> &provider);
    bool generateProxySource(const std::shared_ptr<FDExtensionRoot> &provider);

    bool validateProvider(const std::shared_ptr<FDExtensionRoot> &provider);
    std::string getVersionComment();
    std::string m_providerType;
    std::string m_rte;
    std::string m_srcEndID;
    uint8_t m_sid;
    std::string m_fid;
    std::list<std::string> m_infs;
    std::string m_folderPath;
};
} // namespace BstIdl

#endif