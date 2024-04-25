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
#ifndef F_D_ARRAY_H
#define F_D_ARRAY_H
#include "FDOverwriteElement.h"
#include "FDTypeDefinition.h"
#include "model/FArrayType.h"
namespace BstIdl
{
class FDArray : public FDTypeDefinition, public FDOverwriteElement
{
protected:
    std::shared_ptr<BstIdl::FArrayType> m_target;

public:
    FDArray() : FDTypeDefinition(), FDOverwriteElement()
    {
        setHost("arrays");
    }
    virtual ~FDArray() = default;
    FDArray(const FDArray &) = default;
    FDArray(FDArray &&) noexcept = default;
    FDArray &operator=(const FDArray &) = default;
    FDArray &operator=(FDArray &&) = default;

    std::shared_ptr<BstIdl::FArrayType> getTarget() const
    {
        return m_target;
    }
    void setTarget(const std::shared_ptr<BstIdl::FArrayType> &new_target)
    {
        m_target = new_target;
        if (getName().empty() && new_target)
            setName(new_target->getName());
    }
};
} // namespace BstIdl
#endif