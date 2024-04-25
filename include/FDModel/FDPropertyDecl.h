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
#ifndef F_D_PROPERTY_DECL_H
#define F_D_PROPERTY_DECL_H
#include "FDObject.h"
#include "FDPropertyFlag.h"
#include "FDTypeRef.h"
#include <list>
#include <memory>
namespace BstIdl
{
class FDPropertyDecl : public FDObject
{
public:
    FDPropertyDecl() = default;
    virtual ~FDPropertyDecl() = default;
    FDPropertyDecl(const FDPropertyDecl &) = default;
    FDPropertyDecl(FDPropertyDecl &&) noexcept = default;
    FDPropertyDecl &operator=(const FDPropertyDecl &) = default;
    FDPropertyDecl &operator=(FDPropertyDecl &&) = default;

    std::shared_ptr<FDTypeRef> const getType()
    {
        return m_type;
    }

    void setType(const std::shared_ptr<FDTypeRef> &new_type)
    {
        m_type = new_type;
    }

    std::list<std::shared_ptr<FDPropertyFlag>> &getFlags()
    {
        return m_flags;
    }
    void setFlags(const std::shared_ptr<FDPropertyFlag> &flag)
    {
        m_flags.emplace_back(flag);
    }

    bool isDefault() const
    {
        for (auto f : m_flags)
        {
            if (f->isDefault())
                return true;
        }
        return false;
    }

    std::shared_ptr<FDComplexValue> getDefault() const
    {
        for (auto f : m_flags)
        {
            if (f && f->isDefault())
                return f->getDefault();
        }
        return nullptr;
    }

    bool isOptional() const
    {
        for (auto f : m_flags)
        {
            if (f->isOptional())
                return true;
        }
        return false;
    }

    std::string toGraph() override
    {
        std::string ret = FDObject::toGraph();
        ret += getInstanceName() + " : type=" + m_type->getName() + "\n";
        for (const auto &item : m_flags)
        {
            ret += getInstanceName() + " : " + item->getName() + "\n";
        }

        return ret;
    }

protected:
    std::shared_ptr<FDTypeRef> m_type;
    std::list<std::shared_ptr<FDPropertyFlag>> m_flags;
};
} // namespace BstIdl
#endif