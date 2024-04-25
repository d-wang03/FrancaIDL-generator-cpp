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
#ifndef F_TYPE_COLLECTION_H
#define F_TYPE_COLLECTION_H

#include "FConstantDef.h"
#include "FModelElement.h"
#include "FType.h"
#include "FVersion.h"

#include <list>
#include <memory>
#include <string>

#include "utilities/string_utility.h"

namespace BstIdl
{
class FTypeCollection : public FModelElement
{
public:
    FTypeCollection() : FModelElement("__Anonymous__", nullptr)
    {
    }
    virtual ~FTypeCollection() = default;
    FTypeCollection(const FTypeCollection &) = default;
    FTypeCollection(FTypeCollection &&) noexcept = default;
    FTypeCollection &operator=(const FTypeCollection &) = default;
    FTypeCollection &operator=(FTypeCollection &&) = default;

    std::shared_ptr<FVersion> getVersion() const
    {
        return m_version;
    }
    void setVersion(const std::shared_ptr<FVersion> &value)
    {
        m_version = value;
    }

    std::list<std::shared_ptr<FType>> &getTypes()
    {
        return m_types;
    }
    std::list<std::shared_ptr<FConstantDef>> &getConstants()
    {
        return m_constants;
    }

    std::string getNameSpace() const
    {
        std::string ret;
        if (auto ptr = getContainer())
        {
            ret = ptr->getName();
            if (!m_version || (m_version->getMajor() == 0 && m_version->getMinor() == 0))
                return replace_all(ret, ".", "::");
            else
                return replace_all(ret.insert(0, "v" + std::to_string(m_version->getMajor()) + "."), ".", "::");
        }
        return ret;
    }
    std::string transform(std::string func) override
    {
        std::string ret = getName();
        auto trans = FObject::s_transMap[func];
        if (trans && trans->forFTypeCollection)
            ret = trans->forFTypeCollection(this);
        return ret;
    }

protected:
    std::shared_ptr<FVersion> m_version;
    std::list<std::shared_ptr<FType>> m_types;
    std::list<std::shared_ptr<FConstantDef>> m_constants;
};

} // namespace BstIdl

#endif