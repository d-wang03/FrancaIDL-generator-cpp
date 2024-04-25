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
#ifndef F_D_EXTENTION_ROOT_H
#define F_D_EXTENTION_ROOT_H
#include "FDExtensionElement.h"
#include "FDRootElement.h"
#include "model/FObject.h"
#include "utilities/string_utility.h"

namespace BstIdl
{
class FDExtensionRoot : virtual public FDRootElement
{

public:
    FDExtensionRoot() = default;
    virtual ~FDExtensionRoot() = default;
    FDExtensionRoot(const FDExtensionRoot &) = default;
    FDExtensionRoot(FDExtensionRoot &&) noexcept = default;
    FDExtensionRoot &operator=(const FDExtensionRoot &) = default;
    FDExtensionRoot &operator=(FDExtensionRoot &&) = default;

    std::string getTag() const
    {
        return m_tag;
    }

    void setTag(const std::string &newTag)
    {
        m_tag = newTag;
        setHost(newTag + "s");
    }

    std::list<std::shared_ptr<FDExtensionElement>> &getElements()
    {
        return m_elements;
    }

    std::shared_ptr<BstIdl::FObject> getTarget() const
    {
        return m_target;
    }

    void setTarget(const std::shared_ptr<BstIdl::FObject> &target)
    {
        m_target = target;
        if (getName().empty() && target)
            setName(target->getName() + "_" + m_tag);
    }

    std::string toGraph() override
    {
        auto ret = FDRootElement::toGraph();
        if (!m_tag.empty())
            ret += getInstanceName() + " : tag=" + m_tag + "\n";
        if (m_target)
            ret += getInstanceName() + " : target=" + m_target->getName() + "\n";
        for (const auto &item : m_elements)
        {
            ret += item->toGraph();
            ret += getInstanceName() + "-->" + item->getInstanceName() + ":element\n";
        }
        return ret;
    }

    std::list<std::shared_ptr<FDExtensionElement>> getInstances() const
    {
        std::list<std::shared_ptr<FDExtensionElement>> ret;
        for (auto e : m_elements)
        {
            if (e && e->getTag() == "instance")
                ret.emplace_back(e);
        }
        return ret;
    }

    std::shared_ptr<FDExtensionElement> getInstance(const std::string &targetName) const
    {
        for (auto e : getInstances())
        {
            if (e && e->getTarget() && contains(e->getTarget()->getName(), targetName))
                return e;
        }
        return nullptr;
    }

protected:
    std::string m_tag;
    std::list<std::shared_ptr<FDExtensionElement>> m_elements;
    std::shared_ptr<BstIdl::FObject> m_target;
};
} // namespace BstIdl
#endif