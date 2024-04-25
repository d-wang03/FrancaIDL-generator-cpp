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
#ifndef F_ATTRIBUTE_H
#define F_ATTRIBUTE_H

#include "FTypedElement.h"

namespace BstIdl
{

class FAttribute : public FTypedElement
{
public:
    FAttribute() = default;
    virtual ~FAttribute() = default;
    FAttribute(const FAttribute &) = default;
    FAttribute(FAttribute &&) noexcept = default;
    FAttribute &operator=(const FAttribute &) = default;
    FAttribute &operator=(FAttribute &&) = default;

    bool isReadonly() const
    {
        return m_isReadOnly;
    }
    void setReadonly(bool value)
    {
        m_isReadOnly = value;
    }
    bool isNoRead() const
    {
        return m_isNoRead;
    }
    void setNoRead(bool value)
    {
        m_isNoRead = value;
    }
    bool isNoSubscriptions() const
    {
        return m_isNoSubscriptions;
    }
    void setNoSubscriptions(bool value)
    {
        m_isNoSubscriptions = value;
    }
    std::string transform(std::string func) override
    {
        std::string ret = getName();
        auto trans = FObject::s_transMap[func];
        if (trans && trans->forFAttribute)
            ret = trans->forFAttribute(this);
        return ret;
    }

protected:
    bool m_isReadOnly;
    bool m_isNoRead;
    bool m_isNoSubscriptions;
};

} // namespace BstIdl

#endif