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
#ifndef F_BINARY_OPERATION_H
#define F_BINARY_OPERATION_H

#include "FExpression.h"
#include "FOperation.h"
#include <memory>

namespace BstIdl
{

class FBinaryOperation : public FOperation
{
public:
    FBinaryOperation() = default;
    virtual ~FBinaryOperation() = default;
    FBinaryOperation(const FBinaryOperation &) = default;
    FBinaryOperation(FBinaryOperation &&) noexcept = default;
    FBinaryOperation &operator=(const FBinaryOperation &) = default;
    FBinaryOperation &operator=(FBinaryOperation &&) = default;

    std::shared_ptr<FExpression> getLeft() const
    {
        return m_left;
    }
    void setLeft(const std::shared_ptr<FExpression> &value)
    {
        m_left = value;
    }
    std::shared_ptr<FExpression> getRight() const
    {
        return m_right;
    }
    void setRight(const std::shared_ptr<FExpression> &value)
    {
        m_right = value;
    }

    void validate(std::shared_ptr<FTypeRef> &type, bool isArray) override;
    void EvaluableValidate(std::shared_ptr<FTypeRef> &type, bool isArray, std::string &value,
                           bool is_init_exp) override;
    void getStringValue(const uint32_t typeID, const std::string &l_value, const std::string &r_value,
                        std::string &value);
    std::string toString() const override
    {
        return getLeft()->toString() + getOp()->getLiteral() + getRight()->toString();
    }

protected:
    std::shared_ptr<FExpression> m_left;
    std::shared_ptr<FExpression> m_right;
};

} // namespace BstIdl

#endif