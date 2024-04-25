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
#ifndef F_D_METHOD_H
#define F_D_METHOD_H
#include "FDArgumentList.h"
#include "FDOperation.h"
#include "model/FMethod.h"
namespace BstIdl
{
class FDMethod : public FDOperation
{
protected:
    std::shared_ptr<BstIdl::FMethod> m_target;
    std::shared_ptr<FDArgumentList> m_inArguments;

public:
    FDMethod() : FDOperation(), m_inArguments(std::make_shared<FDArgumentList>())
    {
        setHost("methods");
    }
    virtual ~FDMethod() = default;
    FDMethod(const FDMethod &) = default;
    FDMethod(FDMethod &&) noexcept = default;
    FDMethod &operator=(const FDMethod &) = default;
    FDMethod &operator=(FDMethod &&) = default;
    std::shared_ptr<BstIdl::FMethod> const getTarget()
    {
        return m_target;
    }
    void setTarget(std::shared_ptr<BstIdl::FMethod> target)
    {
        m_target = target;
        if (getName().empty() && target)
            setName(target->getName());
    }
    std::shared_ptr<FDArgumentList> const getInArguments()
    {
        return m_inArguments;
    }
    void setInArguments(std::shared_ptr<FDArgumentList> inArguments)
    {
        m_inArguments = inArguments;
    }

    std::string toGraph() override
    {
        auto ret = FDOperation::toGraph();
        if (m_target)
            ret += getInstanceName() + " : target=" + m_target->getName() + "\n";
        if (m_inArguments)
        {
            ret += m_inArguments->toGraph();
            ret += getInstanceName() + "-->" + m_inArguments->getInstanceName() + ":in_args\n";
        }
        return ret;
    }
};
} // namespace BstIdl
#endif