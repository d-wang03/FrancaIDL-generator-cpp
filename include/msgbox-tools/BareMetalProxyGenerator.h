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

#include "msgbox-tools/MsgBoxInfGenerator.h"

namespace BstIdl
{

class ProxyGenerator : public MsgBoxInfGenerator
{
public:
    ProxyGenerator(const std::string &dest_dir, const std::string &src_id, const std::shared_ptr<FDExtensionElement> &instance)
        : MsgBoxInfGenerator(dest_dir, src_id, instance)
    {
        m_bCheckDstEndID = true;
    }
    ~ProxyGenerator() = default;

private:
    bool generateHeader();
    bool generateSource();
};

} // namespace BstIdl

#endif