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
#ifndef F_D_ELEMENT_H
#define F_D_ELEMENT_H
#include "FDObject.h"
#include "FDProperty.h"
#include "FDPropertyHost.h"
#include "utilities/list_utility.h"

namespace BstIdl
{
class FDElement : virtual public FDObject
{
public:
    FDElement() = default;
    virtual ~FDElement() = default;
    FDElement(const FDElement &) = default;
    FDElement &operator=(const FDElement &) = default;

    bool addProperty(const std::shared_ptr<FDProperty> &property)
    {
        return addListItemByName(m_properties, property);
    }

    std::list<std::shared_ptr<FDProperty>> &getProperties()
    {
        return m_properties;
    }
    std::list<std::shared_ptr<FDProperty>> getProperties() const
    {
        return m_properties;
    }
    std::string toGraph() override
    {
        auto ret = FDObject::toGraph();
        for (const auto &item : m_properties)
        {
            ret += item->toGraph();
            ret += getInstanceName() + "-->" + item->getInstanceName() + ":property\n";
        }
        return ret;
    }
    void setHost(const std::shared_ptr<FDPropertyHost> &host)
    {
        m_host = host;
    }
    void setHost(const std::string &host)
    {
        m_host = std::make_shared<FDPropertyHost>(host);
    }
    std::shared_ptr<FDPropertyHost> getHost() const
    {
        return m_host;
    }

    std::list<std::shared_ptr<FDPropertyDecl>> getDefaultProps() const
    {
        return m_defaultProps;
    }

    std::list<std::shared_ptr<FDPropertyDecl>> getOptionalProps() const
    {
        return m_optionalProps;
    }

    std::list<std::shared_ptr<FDPropertyDecl>> getMandatoryProps() const
    {
        return m_mandatoryProps;
    }

    void updatePropsFromSpec();

    std::shared_ptr<FDComplexValue> getValue(const std::string &property)
    {
        // find in properties.
        for (const auto &p : m_properties)
        {
            if (p && p->getName() == property)
                return p->getValue();
        }
        // find in default properties.
        for (const auto &p : m_defaultProps)
        {
            if (p && p->getName() == property)
            {
                return p->getDefault();
            }
        }
        return nullptr;
    }

    std::shared_ptr<FDValue> getSingleValue(const std::string &property)
    {
        auto ret = getValue(property);
        if (ret && ret->isSingle())
            return ret->getSingle();
        else
            return nullptr;
    }

    std::shared_ptr<FDValueArray> getArrayValue(const std::string &property)
    {
        auto ret = getValue(property);
        if (ret && ret->isArray())
            return ret->getArray();
        else
            return nullptr;
    }

protected:
    std::list<std::shared_ptr<FDProperty>> m_properties;
    std::list<std::shared_ptr<FDPropertyDecl>> m_defaultProps;
    std::list<std::shared_ptr<FDPropertyDecl>> m_optionalProps;
    std::list<std::shared_ptr<FDPropertyDecl>> m_mandatoryProps;

private:
    std::shared_ptr<FDPropertyHost> m_host;
};

} // namespace BstIdl

#endif
