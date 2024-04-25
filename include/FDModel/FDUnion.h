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
#ifndef F_D_UNION_H
#define F_D_UNION_H
#include "FDCompound.h"
#include "model/FUnionType.h"
namespace BstIdl
{
class FDUnion : public FDCompound
{
protected:
    std::shared_ptr<BstIdl::FUnionType> m_target;

public:
    FDUnion() : FDCompound()
    {
        setHost("unions");
    }
    virtual ~FDUnion() = default;
    FDUnion(const FDUnion &) = default;
    FDUnion(FDUnion &&) noexcept = default;
    FDUnion &operator=(const FDUnion &) = default;
    FDUnion &operator=(FDUnion &&) = default;
    std::shared_ptr<BstIdl::FUnionType> const getTarget()
    {
        return m_target;
    }
    void setTarget(const std::shared_ptr<BstIdl::FUnionType> &target)
    {
        m_target = target;
        if (getName().empty() && target)
            setName(target->getName());
    }
};
} // namespace BstIdl
#endif