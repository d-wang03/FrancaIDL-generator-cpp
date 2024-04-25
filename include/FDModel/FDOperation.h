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
#ifndef F_D_OPERATION
#define F_D_OPERATION
#include "FDArgumentList.h"
#include "FDElement.h"
namespace BstIdl
{
class FDOperation : public FDElement
{
protected:
    std::shared_ptr<FDArgumentList> m_outArguments;

public:
    FDOperation() : FDElement(), m_outArguments(std::make_shared<FDArgumentList>())
    {
    }
    virtual ~FDOperation() = default;
    FDOperation(const FDOperation &) = default;
    FDOperation(FDOperation &&) noexcept = default;
    FDOperation &operator=(const FDOperation &) = default;
    FDOperation &operator=(FDOperation &&) = default;
    std::shared_ptr<FDArgumentList> const getOutArguments()
    {
        return m_outArguments;
    }
    void setOutArguments(const std::shared_ptr<FDArgumentList> &outArguments)
    {
        m_outArguments = outArguments;
    }

    std::string toGraph() override
    {
        auto ret = FDElement::toGraph();
        if (m_outArguments)
        {
            ret += m_outArguments->toGraph();
            ret += getInstanceName() + "-->" + m_outArguments->getInstanceName() + ":out_args\n";
        }
        return ret;
    }
};
} // namespace BstIdl
#endif