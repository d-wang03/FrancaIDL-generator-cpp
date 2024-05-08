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
#ifndef F_QUALIFIED_ELEMENT_REF_H
#define F_QUALIFIED_ELEMENT_REF_H

#include "FEvaluableElement.h"
#include "FExpression.h"
#include "FField.h"

#include <memory>

namespace BstIdl
{

class FQualifiedElementRef : public FExpression, public std::enable_shared_from_this<FQualifiedElementRef>
{
public:
    FQualifiedElementRef() = default;
    virtual ~FQualifiedElementRef() = default;
    FQualifiedElementRef(const FQualifiedElementRef &) = default;
    FQualifiedElementRef(FQualifiedElementRef &&) noexcept = default;
    FQualifiedElementRef &operator=(const FQualifiedElementRef &) = default;
    FQualifiedElementRef &operator=(FQualifiedElementRef &&) = default;

    auto getElement() const
    {
        return m_element;
    }
    void setElement(const std::shared_ptr<FEvaluableElement> &value)
    {
        m_element = value;
    }

    std::shared_ptr<FQualifiedElementRef> getQualifier() const
    {
        return m_qualifier;
    }
    void setQualifier(const std::shared_ptr<FQualifiedElementRef> &value)
    {
        m_qualifier = value;
    }

    std::shared_ptr<FField> getField()
    {
        return m_field;
    }
    void setField(const std::shared_ptr<FField> &value)
    {
        m_field = value;
    }

    void validate(std::shared_ptr<FTypeRef> &type, bool isArray) override;
    void EvaluableValidate(std::shared_ptr<FTypeRef> &type, bool isArray, std::string &value,
                           bool is_init_exp) override;

    std::shared_ptr<FTypeRef> getRealType(const std::shared_ptr<FTypeRef> &dst);
    std::string toString() const override
    {
        if (m_qualifier != nullptr)
        {
            if (m_field != nullptr)
            {
                return getQualifier()->toString() + "->" + m_field->getName();
            }
            else
            {
                return getQualifier()->toString();
            }
        }
        return m_element->getName();
    }

protected:
    std::shared_ptr<FEvaluableElement> m_element;
    std::shared_ptr<FQualifiedElementRef> m_qualifier;
    std::shared_ptr<FField> m_field;

    bool isTypeMatch(const std::shared_ptr<FTypeRef> &src, const std::shared_ptr<FTypeRef> &dst);
    std::shared_ptr<FEvaluableElement> getQerElementAndFQN(std::list<std::string> &splittedFqn);
    std::shared_ptr<FTypeRef> getFieldType(const std::list<std::string> &_splitted_fqn,
                                           std::shared_ptr<FConstantDef> &constDef);
    std::shared_ptr<FInitializerExpression> getFieldExpression(const std::list<std::string> &_splitted_fqn,
                                                               std::shared_ptr<FConstantDef> &constDef);
};

} // namespace BstIdl

#endif