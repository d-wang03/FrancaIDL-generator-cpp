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
#ifndef F_UNARY_OPERATION_H
#define F_UNARY_OPERATION_H

#include "FExpression.h"
#include "FOperation.h"
#include <memory>

namespace BstIdl
{

class FUnaryOperation : public FOperation
{
public:
    FUnaryOperation() = default;
    virtual ~FUnaryOperation() = default;
    FUnaryOperation(const FUnaryOperation &) = default;
    FUnaryOperation(FUnaryOperation &&) noexcept = default;
    FUnaryOperation &operator=(const FUnaryOperation &) = default;
    FUnaryOperation &operator=(FUnaryOperation &&) = default;

    std::shared_ptr<FExpression> getOperand() const
    {
        return m_operand;
    }
    void setOperand(const std::shared_ptr<FExpression> &value)
    {
        m_operand = value;
    }

    void validate(std::shared_ptr<FTypeRef> &type, bool isArray) override;
    void EvaluableValidate(std::shared_ptr<FTypeRef> &type, bool isArray, std::string &value, bool is_init_exp);
    void getStringValue(const uint32_t typeID, std::string &value);
    std::string toString() const override
    {
        return getOp()->getLiteral() + getOperand()->toString();
    }

protected:
    std::shared_ptr<FExpression> m_operand;
};

} // namespace BstIdl

#endif