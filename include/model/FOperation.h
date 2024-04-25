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
#ifndef F_OPERATION_H
#define F_OPERATION_H

#include "FExpression.h"
#include "FOperator.h"
#include <memory>

namespace BstIdl
{

class FOperation : public FExpression
{
public:
    FOperation() = default;
    virtual ~FOperation() = default;
    FOperation(const FOperation &) = default;
    FOperation(FOperation &&) noexcept = default;
    FOperation &operator=(const FOperation &) = default;
    FOperation &operator=(FOperation &&) = default;

    std::shared_ptr<FOperator> getOp() const
    {
        return m_op;
    }
    void setOp(const std::shared_ptr<FOperator> &value)
    {
        m_op = value;
    }
    std::string toString() const override
    {
        return "";
    }

protected:
    std::shared_ptr<FOperator> m_op;
};

} // namespace BstIdl

#endif