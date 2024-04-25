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
#ifndef F_BROADCAST_H
#define F_BROADCAST_H

#include "FArgument.h"
#include "FModelElement.h"
#include <list>
#include <memory>
#include <string>

namespace BstIdl
{

class FBroadcast : public FModelElement
{
public:
    FBroadcast() = default;
    virtual ~FBroadcast() = default;
    FBroadcast(const FBroadcast &) = default;
    FBroadcast(FBroadcast &&) noexcept = default;
    FBroadcast &operator=(const FBroadcast &) = default;
    FBroadcast &operator=(FBroadcast &&) = default;

    std::string getFQN() const override
    {
        if (getSelector().empty())
            return getContainer() ? (getContainer()->getFQN() + "." + getName()) : getName();
        else
            return getContainer() ? (getContainer()->getFQN() + "." + getName() + ":" + m_selector)
                                  : getName() + ":" + m_selector;
    }

    std::list<std::shared_ptr<FArgument>> &getOutArgs()
    {
        return m_outArgs;
    }
    bool isSelective() const
    {
        return m_isSelective;
    }
    void setSelective(bool value)
    {
        m_isSelective = value;
    }
    std::string getSelector() const
    {
        return m_selector;
    }
    void setSelector(const std::string &value)
    {
        m_selector = value;
    }
    std::string transform(std::string func) override
    {
        std::string ret = getName();
        auto trans = FObject::s_transMap[func];
        if (trans && trans->forFBroadcast)
            ret = trans->forFBroadcast(this);
        return ret;
    }

protected:
    std::list<std::shared_ptr<FArgument>> m_outArgs;
    bool m_isSelective;
    std::string m_selector;
};

} // namespace BstIdl

#endif