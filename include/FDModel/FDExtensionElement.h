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
#ifndef F_D_EXTENSION_ELEMENT_H
#define F_D_EXTENSION_ELEMENT_H
#include "FDAbstractExtensionElement.h"
#include "FDElement.h"
#include "model/FObject.h"

namespace BstIdl
{
class FDExtensionElement : public FDElement, public FDAbstractExtensionElement
{
public:
    FDExtensionElement() = default;
    virtual ~FDExtensionElement() = default;
    FDExtensionElement(const FDExtensionElement &) = default;
    FDExtensionElement(FDExtensionElement &&) noexcept = default;
    FDExtensionElement &operator=(const FDExtensionElement &) = default;
    FDExtensionElement &operator=(FDExtensionElement &&) = default;
    void setTag(const std::string &newTag)
    {
        m_tag = newTag;
        setHost(newTag + "s");
    }
    void setContainer(const std::shared_ptr<FDElement> &container)
    {
        FDElement::setContainer(container);
    }
    std::shared_ptr<FDElement> getContainer() const
    {
        return std::dynamic_pointer_cast<FDElement>(FDElement::getContainer());
    }
    std::list<std::shared_ptr<FDExtensionElement>> &getElements()
    {
        return m_elements;
    }

    std::string toGraph() override
    {
        auto ret = FDElement::toGraph();
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

    void updatePropsForAll()
    {
        updatePropsFromSpec();
        for (const auto &item : m_elements)
            item->updatePropsForAll();
    }
};
} // namespace BstIdl
#endif