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
#ifndef F_D_TYPE_ELEMENT_H
#define F_D_TYPE_ELEMENT_H
#include "FDElement.h"
namespace BstIdl
{
class FDTypeElement : virtual public FDElement
{
public:
    FDTypeElement() = default;
    virtual ~FDTypeElement() = default;
    FDTypeElement(const FDTypeElement &) = default;
    FDTypeElement(FDTypeElement &&) noexcept = default;
    FDTypeElement &operator=(const FDTypeElement &) = default;

    std::shared_ptr<BstIdl::FTypeRef> getTarget() const
    {
        return m_target;
    }
    void setTarget(const std::shared_ptr<BstIdl::FTypeRef> &target)
    {
        m_target = target;
        if (getName().empty() && target)
            setName(target->getName());
    }

private:
    std::shared_ptr<BstIdl::FTypeRef> m_target;
};
} // namespace BstIdl
#endif