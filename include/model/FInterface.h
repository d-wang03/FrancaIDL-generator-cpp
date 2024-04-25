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
#ifndef F_INTERFACE_H
#define F_INTERFACE_H

#include "FAttribute.h"
#include "FBroadcast.h"
#include "FMethod.h"
#include "FTypeCollection.h"

namespace BstIdl
{
class FModel;
class FInterface : public FTypeCollection
{
public:
    FInterface() = default;
    virtual ~FInterface() = default;
    FInterface(const FInterface &) = default;
    FInterface(FInterface &&) noexcept = default;
    FInterface &operator=(const FInterface &) = default;
    FInterface &operator=(FInterface &&) = default;

    std::list<std::shared_ptr<FAttribute>> &getAttributes()
    {
        return m_attributes;
    }
    std::list<std::shared_ptr<FMethod>> &getMethods()
    {
        return m_methods;
    }
    std::list<std::shared_ptr<FBroadcast>> &getBroadcasts()
    {
        return m_broadcasts;
    }
    std::shared_ptr<FInterface> getBase() const
    {
        return m_base;
    }
    void setBase(const std::shared_ptr<FInterface> &value)
    {
        m_base = value;
    }
    std::list<std::shared_ptr<FInterface>> &getManagedInterfaces()
    {
        return m_managedInterfaces;
    }
    std::string transform(std::string func) override
    {
        std::string ret = getName();
        auto trans = FObject::s_transMap[func];
        if (trans && trans->forFInterface)
            ret = trans->forFInterface(this);
        return ret;
    }

protected:
    std::list<std::shared_ptr<FAttribute>> m_attributes;
    std::list<std::shared_ptr<FMethod>> m_methods;
    std::list<std::shared_ptr<FBroadcast>> m_broadcasts;
    std::shared_ptr<FInterface> m_base;
    std::list<std::shared_ptr<FInterface>> m_managedInterfaces;
};

} // namespace BstIdl

#endif