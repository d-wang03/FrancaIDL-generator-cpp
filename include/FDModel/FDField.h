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
#ifndef F_D_FIELD_H
#define F_D_FIELD_H
#include "FDOverwriteElement.h"
#include "model/FField.h"
namespace BstIdl
{
class FDUnion;
class FDStruct;
class FDField : virtual public FDOverwriteElement
{

protected:
    std::shared_ptr<BstIdl::FField> m_target;

public:
    FDField() : FDOverwriteElement()
    {
        setHost("undefined");
    }
    virtual ~FDField() = default;
    FDField(const FDField &) = default;
    FDField(FDField &&) noexcept = default;
    FDField &operator=(const FDField &) = default;
    FDField &operator=(FDField &&) = default;
    std::shared_ptr<BstIdl::FField> const getTarget()
    {
        return m_target;
    }
    void setTarget(const std::shared_ptr<BstIdl::FField> &target)
    {
        m_target = target;
        if (getName().empty() && target)
            setName(target->getName());
    }
    void setContainer(const std::shared_ptr<FDObject> &value)
    {
        FDOverwriteElement::setContainer(value);
        if (std::dynamic_pointer_cast<FDUnion>(value) != nullptr)
        {
            setHost("union_fields");
        }
        else if (std::dynamic_pointer_cast<FDStruct>(value) != nullptr)
        {
            setHost("struct_fields");
        }
    }
};
} // namespace BstIdl
#endif