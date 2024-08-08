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
#ifndef ABSTARCT_GENERATOR_H
#define ABSTARCT_GENERATOR_H

#include "utilities/list_utility.h"
#include <string>
#include <unordered_map>

namespace BstIdl
{

static auto sBstLic = R"(/*
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
)";

class AbstractGenerator
{
public:
    AbstractGenerator(const std::string &dest_dir = std::string()) : m_destDir(dest_dir), m_license(sBstLic)
    {
    }
    virtual ~AbstractGenerator() = default;
    virtual bool generate() = 0;
    virtual void initTypeMap()
    {
    }
    std::string getMappingType(const std::string &type) const
    {
        return m_typeMap.count(type) ? m_typeMap.at(type) : type;
    }

    std::string getLicense() const
    {
        return m_license;
    }
    void setLicense(const std::string &license)
    {
        if (!license.empty())
            m_license = license;
    }
    static std::string commit_hash();

protected:
    std::string mk_dir(const std::string &dir, const std::string &interface_path);
    std::string m_destDir;
    std::unordered_map<std::string, std::string> m_typeMap;
    std::string m_license;
};
} // namespace BstIdl

#endif