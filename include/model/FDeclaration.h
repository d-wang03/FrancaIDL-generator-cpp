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
#ifndef F_DECLARATION_H
#define F_DECLARATION_H

#include "FInitializerExpression.h"
#include "FTypedElement.h"

namespace BstIdl
{

class FDeclaration : public FTypedElement
{
public:
    FDeclaration() = default;
    virtual ~FDeclaration() = default;
    FDeclaration(const FDeclaration &) = default;
    FDeclaration(FDeclaration &&) noexcept = default;
    FDeclaration &operator=(const FDeclaration &) = default;
    FDeclaration &operator=(FDeclaration &&) = default;

    std::shared_ptr<FInitializerExpression> getRhs() const
    {
        return m_rhs;
    }
    void setRhs(const std::shared_ptr<FInitializerExpression> &value)
    {
        m_rhs = value;
    }
    std::string transform(std::string func) override
    {
        std::string ret = getName();
        auto trans = FObject::s_transMap[func];
        if (trans && trans->forFDeclaration)
            ret = trans->forFDeclaration(this);
        return ret;
    }

protected:
    std::shared_ptr<FInitializerExpression> m_rhs;
};

} // namespace BstIdl

#endif