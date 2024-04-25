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
#ifndef F_D_SPECIFICATION_H
#define F_D_SPECIFICATION_H
#include "FDDeclaration.h"
#include <iostream>
#include <list>
namespace BstIdl
{
class FDSpecification : public FDObject
{
public:
    FDSpecification() = default;
    virtual ~FDSpecification() = default;
    FDSpecification(const FDSpecification &) = default;
    FDSpecification(FDSpecification &&) noexcept = default;
    FDSpecification &operator=(const FDSpecification &) = default;
    FDSpecification &operator=(FDSpecification &&) = default;

    std::shared_ptr<FDSpecification> getBase() const
    {
        return m_base;
    }
    void setBase(const std::shared_ptr<FDSpecification> &base)
    {
        m_base = base;
    }
    std::list<std::shared_ptr<FDDeclaration>> &getDeclarations()
    {
        return m_declarations;
    }
    std::list<std::string> getSpecNames() const
    {
        std::list<std::string> ret;
        ret.emplace_back(getName());
        auto ptr = m_base;
        while (ptr)
        {
            ret.emplace_front(ptr->getName());
            ptr = ptr->getBase();
        }
        return ret;
    }
    std::list<std::shared_ptr<FDDeclaration>> getAllDeclarations()
    {
        auto ret = m_declarations;
        if (m_base)
            ret.splice(ret.begin(), m_base->getAllDeclarations());
        return ret;
    }

    std::list<std::shared_ptr<FDDeclaration>> findDeclarations(const std::shared_ptr<FDPropertyHost> &host)
    {
        std::list<std::shared_ptr<FDDeclaration>> ret;
        for (auto d : getAllDeclarations())
        {
            auto h = d->getHost();
            if (*host == *h)
                ret.emplace_back(d);
        }
        return ret;
    }

    std::string toGraph() override
    {
        std::string ret = FDObject::toGraph();

        // base
        if (m_base)
            ret += getInstanceName() + "-->" + m_base->getInstanceName() + ":base spec\n";
        // declarations
        for (const auto &item : m_declarations)
        {
            ret += item->toGraph();
            ret += getInstanceName() + "-->" + item->getInstanceName() + ":declaration\n";
        }
        return ret;
    }

protected:
    std::shared_ptr<FDSpecification> m_base;
    std::list<std::shared_ptr<FDDeclaration>> m_declarations;
};
} // namespace BstIdl
#endif