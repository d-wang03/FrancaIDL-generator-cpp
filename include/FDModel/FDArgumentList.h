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
#ifndef F_D_ARGUMENT_LIST_H
#define F_D_ARGUMENT_LIST_H
#include "FDArgument.h"
#include <list>
#include <memory>
namespace BstIdl
{
class FDArgumentList : public FDObject
{
private:
    std::list<std::shared_ptr<FDArgument>> m_arguments;

public:
    FDArgumentList() = default;
    virtual ~FDArgumentList() = default;
    FDArgumentList(const FDArgumentList &) = default;
    FDArgumentList(FDArgumentList &&) noexcept = default;
    FDArgumentList &operator=(const FDArgumentList &) = default;
    FDArgumentList &operator=(FDArgumentList &&) = default;
    std::list<std::shared_ptr<FDArgument>> &getArguments()
    {
        return m_arguments;
    }
    void setArguments(const std::shared_ptr<FDArgument> &argument)
    {
        m_arguments.emplace_back(argument);
    }
    std::string toGraph() override
    {
        if (getName().empty())
            setName("FDArgumentList");
        auto ret = FDObject::toGraph();
        for (const auto &item : m_arguments)
        {
            ret += item->toGraph();
            ret += getInstanceName() + "-->" + item->getInstanceName() + ":argument\n";
        }
        return ret;
    }
};
} // namespace BstIdl
#endif