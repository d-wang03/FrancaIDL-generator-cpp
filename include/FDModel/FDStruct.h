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
#ifndef F_D_STRUCT_H
#define F_D_STRUCT_H
#include "FDCompound.h"
#include "model/FStructType.h"
namespace BstIdl
{
class FDStruct : public FDCompound
{
public:
    FDStruct() : FDCompound()
    {
        setHost("structs");
    }
    virtual ~FDStruct() = default;
    FDStruct(const FDStruct &) = default;
    FDStruct(FDStruct &&) noexcept = default;
    FDStruct &operator=(const FDStruct &) = default;
    FDStruct &operator=(FDStruct &&) = default;

    std::shared_ptr<BstIdl::FStructType> const getTarget()
    {
        return m_target;
    }
    void setTarget(const std::shared_ptr<BstIdl::FStructType> &target)
    {
        m_target = target;
        if (getName().empty() && target)
            setName(target->getName());
    }

protected:
    std::shared_ptr<BstIdl::FStructType> m_target;
};
} // namespace BstIdl
#endif