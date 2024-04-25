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
#ifndef F_D_ROOT_ELEMENT_H
#define F_D_ROOT_ELEMENT_H
#include "FDElement.h"
#include "FDSpecification.h"
#include "utilities/list_utility.h"
namespace BstIdl
{
class FDRootElement : virtual public FDElement
{
protected:
    std::shared_ptr<FDSpecification> m_spec;
    std::list<std::shared_ptr<FDRootElement>> m_uses;

public:
    FDRootElement() = default;
    virtual ~FDRootElement() = default;
    FDRootElement(const FDRootElement &) = default;
    FDRootElement(FDRootElement &&) noexcept = default;
    FDRootElement &operator=(const FDRootElement &) = default;

    std::shared_ptr<FDSpecification> getSpec() const
    {
        return m_spec;
    }
    void setSpec(const std::shared_ptr<FDSpecification> &spec)
    {
        m_spec = spec;
    }
    std::list<std::shared_ptr<FDRootElement>> getUse() const
    {
        return m_uses;
    }
    void addUse(const std::shared_ptr<FDRootElement> &new_use)
    {
        addListItem(m_uses, new_use);
    }
    std::string toGraph() override
    {
        std::string ret = FDElement::toGraph();
        if (m_spec)
            ret += getInstanceName() + " : spec=" + m_spec->getName() + "\n";
        for (const auto &item : m_uses)
        {
            ret += getInstanceName() + " : use " + item->getName() + "\n";
        }
        return ret;
    }
};
} // namespace BstIdl
#endif