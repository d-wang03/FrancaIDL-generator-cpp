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
#ifndef F_D_PROPERTY_HOST
#define F_D_PROPERTY_HOST
#include "FDBuiltInPropertyHost.h"
namespace BstIdl
{
class FDPropertyHost
{
private:
    std::shared_ptr<FDBuiltInPropertyHost> m_builtinHost;
    std::string m_extensionHost;

public:
    FDPropertyHost(const std::shared_ptr<FDBuiltInPropertyHost> &builtinHost)
    {
        m_builtinHost = builtinHost;
    }
    FDPropertyHost(const std::string &host)
    {
        m_builtinHost = FDBuiltInPropertyHost::get(host);
        if (m_builtinHost->getValue() == FDBuiltInPropertyHost::UNDEFINED)
            m_extensionHost = host;
    }
    FDPropertyHost(FDBuiltInPropertyHost::BuiltInPropertyHost host)
    {
        m_builtinHost = FDBuiltInPropertyHost::get(host);
    }
    virtual ~FDPropertyHost() = default;
    FDPropertyHost(const FDPropertyHost &) = default;
    FDPropertyHost(FDPropertyHost &&) noexcept = default;
    FDPropertyHost &operator=(const FDPropertyHost &) = default;
    FDPropertyHost &operator=(FDPropertyHost &&) = default;

    std::string getName() const
    {
        return m_extensionHost.empty() ? m_builtinHost->getName() : m_extensionHost;
    }
    bool isBuiltIn() const
    {
        return m_builtinHost->getValue() != FDBuiltInPropertyHost::UNDEFINED ? true : false;
    }
    void setBuiltIn(const std::shared_ptr<FDBuiltInPropertyHost> &asBuiltin)
    {
        m_builtinHost = asBuiltin;
    }
    std::shared_ptr<FDBuiltInPropertyHost> const getBuiltIn()
    {
        return m_builtinHost;
    }
    bool operator==(const FDPropertyHost &other)
    {
        return m_builtinHost == other.m_builtinHost && m_extensionHost == other.m_extensionHost;
    }
};
} // namespace BstIdl
#endif