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
#ifndef F_D_DELARATION_H
#define F_D_DELARATION_H
#include "FDObject.h"
#include "FDPropertyDecl.h"
#include "FDPropertyHost.h"
namespace BstIdl
{
class FDDeclaration : public FDObject
{
protected:
    std::shared_ptr<FDPropertyHost> m_host;
    std::list<std::shared_ptr<FDPropertyDecl>> m_properties;

public:
    FDDeclaration() = default;
    virtual ~FDDeclaration() = default;
    FDDeclaration(const FDDeclaration &) = default;
    FDDeclaration(FDDeclaration &&) noexcept = default;
    FDDeclaration &operator=(const FDDeclaration &) = default;
    FDDeclaration &operator=(FDDeclaration &&) = default;
    std::shared_ptr<FDPropertyHost> getHost() const
    {
        return m_host;
    }
    void setHost(std::shared_ptr<FDPropertyHost> host)
    {
        m_host = host;
    }
    std::list<std::shared_ptr<FDPropertyDecl>> &getProperties()
    {
        return m_properties;
    }
    std::string toGraph() override
    {
        std::string ret = FDObject::toGraph();
        ret += getInstanceName() + " : host=" + m_host->getName() + "\n";
        for (const auto &item : m_properties)
        {
            ret += item->toGraph();
            ret += getInstanceName() + "-->" + item->getInstanceName() + ":deployment\n";
        }
        return ret;
    }
};
} // namespace BstIdl
#endif