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
#ifndef F_ASSIGNMENT_H
#define F_ASSIGNMENT_H

#include "FExpression.h"
#include "FQualifiedElementRef.h"
#include "FStatement.h"

namespace BstIdl
{

class FAssignment : public FStatement
{
public:
    FAssignment() = default;
    virtual ~FAssignment() = default;
    FAssignment(const FAssignment &) = default;
    FAssignment(FAssignment &&) noexcept = default;
    FAssignment &operator=(const FAssignment &) = default;
    FAssignment &operator=(FAssignment &&) = default;

    std::shared_ptr<FQualifiedElementRef> getLhs() const
    {
        return m_lhs;
    }
    void setLhs(const std::shared_ptr<FQualifiedElementRef> &value)
    {
        m_lhs = value;
    }

    std::shared_ptr<FExpression> getRhs() const
    {
        return m_rhs;
    }
    void setRhs(const std::shared_ptr<FExpression> &value)
    {
        m_rhs = value;
    }

protected:
    std::shared_ptr<FQualifiedElementRef> m_lhs;
    std::shared_ptr<FExpression> m_rhs;
};

} // namespace BstIdl

#endif