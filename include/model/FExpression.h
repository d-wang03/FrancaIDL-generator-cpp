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
#ifndef F_EXPRESSION_H
#define F_EXPRESSION_H

#include "FInitializerExpression.h"

namespace BstIdl
{
class FExpression : public FInitializerExpression
{
public:
    FExpression() = default;
    virtual ~FExpression() = default;
    FExpression(const FExpression &) = default;
    FExpression(FExpression &&) noexcept = default;
    FExpression &operator=(const FExpression &) = default;
    FExpression &operator=(FExpression &&) = default;

    void validate(std::shared_ptr<FTypeRef> &type, bool isArray) override;
    void EvaluableValidate(std::shared_ptr<FTypeRef> &type, bool isArray, std::string &value,
                           bool is_init_exp) override;
    void boundaryCheck(uint32_t typeID, const std::string &value);
    void boundaryCheck(int64_t upper_bound, int64_t lower_bound, const std::string &value);
    virtual std::string toString() const
    {
        return "";
    }
};

} // namespace BstIdl

#endif