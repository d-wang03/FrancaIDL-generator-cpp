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
#ifndef F_D_ABSTRUCT_EXTENTION_ELEMENT_H
#define F_D_ABSTRUCT_EXTENTION_ELEMENT_H
#include "FDObject.h"
#include "model/FObject.h"
#include <iostream>
#include <list>
#include <memory>

namespace BstIdl
{
class FDExtensionElement;
class FDAbstractExtensionElement : virtual public FDObject
{
protected:
    std::string m_tag;
    std::list<std::shared_ptr<FDExtensionElement>> m_elements;
    std::shared_ptr<FObject> m_target;

public:
    FDAbstractExtensionElement() = default;
    virtual ~FDAbstractExtensionElement() = default;
    FDAbstractExtensionElement(const FDAbstractExtensionElement &) = default;
    FDAbstractExtensionElement(FDAbstractExtensionElement &&) noexcept = default;
    FDAbstractExtensionElement &operator=(const FDAbstractExtensionElement &) = default;
    std::string const getTag()
    {
        return m_tag;
    }
    void setTag(const std::string &new_tag)
    {
        m_tag = new_tag;
    }
    std::list<std::shared_ptr<FDExtensionElement>> const &getElements()
    {
        return m_elements;
    }

    std::shared_ptr<FObject> const getTarget()
    {
        return m_target;
    }
    void setTarget(const std::shared_ptr<FObject> &new_target)
    {
        m_target = new_target;
        if (getName().empty() && new_target)
            setName(new_target->getName() + "_" + m_tag);
    }
};
} // namespace BstIdl
#endif