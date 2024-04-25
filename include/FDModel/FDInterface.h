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
#ifndef F_D_INTERFACE_H
#define F_D_INTERFACE_H
#include "FDAttribute.h"
#include "FDBroadcast.h"
#include "FDMethod.h"
#include "FDRootElement.h"
#include "FDTypeDefinition.h"
#include "model/FInterface.h"
#include <iostream>

namespace BstIdl
{
class FDInterface : public FDRootElement
{
public:
    FDInterface() : FDRootElement()
    {
        setHost(std::make_shared<FDPropertyHost>("interfaces"));
    }
    virtual ~FDInterface() = default;
    FDInterface(const FDInterface &) = default;
    FDInterface(FDInterface &&) noexcept = default;
    FDInterface &operator=(const FDInterface &) = default;
    FDInterface &operator=(FDInterface &&) = default;

    std::shared_ptr<BstIdl::FInterface> getTarget() const
    {
        return m_target;
    }
    void setTarget(const std::shared_ptr<BstIdl::FInterface> &target)
    {
        m_target = target;
        if (getName().empty() && target)
            setName(target->getName());
    }
    std::list<std::shared_ptr<FDAttribute>> &getAttributes()
    {
        return m_attributes;
    }
    void setAttributes(std::shared_ptr<FDAttribute> &attribute)
    {
        m_attributes.emplace_back(attribute);
    }
    std::list<std::shared_ptr<FDMethod>> &getMethods()
    {
        return m_methods;
    }
    void setMethods(std::shared_ptr<FDMethod> &method)
    {
        m_methods.emplace_back(method);
    }
    std::list<std::shared_ptr<FDBroadcast>> &getBroadcasts()
    {
        return m_broadcasts;
    }
    void setBroadcasts(std::shared_ptr<FDBroadcast> &broadcast)
    {
        m_broadcasts.emplace_back(broadcast);
    }
    std::list<std::shared_ptr<FDTypeDefinition>> &getTypes()
    {
        return m_types;
    }
    void setTypes(std::shared_ptr<FDTypeDefinition> &type)
    {
        m_types.emplace_back(type);
    }
    std::string toGraph() override
    {
        auto ret = FDRootElement::toGraph();
        if (m_target)
            ret += getInstanceName() + " : target=" + m_target->getName() + "\n";
        for (const auto &item : m_attributes)
        {
            ret += item->toGraph();
            ret += getInstanceName() + "-->" + item->getInstanceName() + ":attribute\n";
        }
        for (const auto &item : m_methods)
        {
            ret += item->toGraph();
            ret += getInstanceName() + "-->" + item->getInstanceName() + ":method\n";
        }
        for (const auto &item : m_broadcasts)
        {
            ret += item->toGraph();
            ret += getInstanceName() + "-->" + item->getInstanceName() + ":broadcast\n";
        }
        for (const auto &item : m_types)
        {
            ret += item->toGraph();
            ret += getInstanceName() + "-->" + item->getInstanceName() + ":type\n";
        }
        return ret;
    }

protected:
    std::shared_ptr<BstIdl::FInterface> m_target;
    std::list<std::shared_ptr<FDAttribute>> m_attributes;
    std::list<std::shared_ptr<FDMethod>> m_methods;
    std::list<std::shared_ptr<FDBroadcast>> m_broadcasts;
    std::list<std::shared_ptr<FDTypeDefinition>> m_types;
};
} // namespace BstIdl
#endif