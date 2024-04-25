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
#ifndef F_D_INTERFACE_REF_H
#define F_D_INTERFACE_REF_H
#include "FDValue.h"
#include "model/FInterface.h"
#include <iostream>
#include <memory>
namespace BstIdl
{
class FDInterfaceRef : public FDValue
{
public:
    FDInterfaceRef() = default;
    virtual ~FDInterfaceRef() = default;
    FDInterfaceRef(const FDInterfaceRef &) = default;
    FDInterfaceRef(FDInterfaceRef &&) noexcept = default;
    FDInterfaceRef &operator=(const FDInterfaceRef &) = default;
    FDInterfaceRef &operator=(FDInterfaceRef &&) = default;

    std::shared_ptr<BstIdl::FInterface> const getTarget()
    {
        return m_target;
    }
    void setTarget(const std::shared_ptr<BstIdl::FInterface> &new_target)
    {
        m_target = new_target;
        if (getName().empty() && new_target)
            setName(new_target->getName());
    }

    bool isInterfaceRef() const override
    {
        return true;
    }

    std::shared_ptr<BstIdl::FInterface> getInterfaceRef() const override
    {
        return m_target;
    }

protected:
    std::shared_ptr<BstIdl::FInterface> m_target;
};
} // namespace BstIdl
#endif