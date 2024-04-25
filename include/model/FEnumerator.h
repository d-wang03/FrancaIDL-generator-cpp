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
#ifndef F_ENUMERATOR_H
#define F_ENUMERATOR_H

#include "FEvaluableElement.h"
#include "FExpression.h"
#include "FIntegerConstant.h"
#include "FOperation.h"
#include "FQualifiedElementRef.h"
#include <iostream>

namespace BstIdl
{
class FEnumerator : public FEvaluableElement
{
public:
    FEnumerator() = default;
    virtual ~FEnumerator() = default;
    FEnumerator(const FEnumerator &) = default;
    FEnumerator(FEnumerator &&) noexcept = default;
    FEnumerator &operator=(const FEnumerator &) = default;
    FEnumerator &operator=(FEnumerator &&) = default;

    std::shared_ptr<FExpression> getValue() const
    {
        return m_value;
    }
    void setValue(const std::shared_ptr<FExpression> &value)
    {
        m_value = value;
    }
    std::string valueToString()
    {
        if (m_value)
        {
            auto integer = std::dynamic_pointer_cast<FIntegerConstant>(m_value);
            auto operation = std::dynamic_pointer_cast<FOperation>(m_value);
            auto qualified = std::dynamic_pointer_cast<FQualifiedElementRef>(m_value);
            if (integer != nullptr)
            {
                return integer->toString();
            }
            else if (operation != nullptr)
            {
                return operation->toString();
            }
            else if (qualified != nullptr)
            {
                return qualified->toString();
            }
            else
            {
                std::cerr << "Invalid format for enumerated values\n";
                return nullptr;
            }
        }
        else
        {
            return std::string();
        }
    }
    std::string toString() const
    {
        if (m_value)
        {
            auto integer = std::dynamic_pointer_cast<FIntegerConstant>(m_value);
            auto operation = std::dynamic_pointer_cast<FOperation>(m_value);
            auto qualified = std::dynamic_pointer_cast<FQualifiedElementRef>(m_value);
            if (integer != nullptr)
            {
                return getName() + " = " + integer->toString();
            }
            else if (operation != nullptr)
            {
                return getName() + " = " + operation->toString();
            }
            else if (qualified != nullptr)
            {
                return getName() + " = " + qualified->toString();
            }
            else
            {
                std::cerr << "Invalid format for enumerated values\n";
                return nullptr;
            }
        }
        else
            return getName();
    }
    std::string transform(std::string func) override
    {
        std::string ret = getName();
        auto trans = FObject::s_transMap[func];
        if (trans && trans->forFEnumerator)
            ret = trans->forFEnumerator(this);
        return ret;
    }

protected:
    std::shared_ptr<FExpression> m_value;
};

} // namespace BstIdl

#endif