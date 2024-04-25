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
#ifndef F_INITIALIZER_EXPRESSION_H
#define F_INITIALIZER_EXPRESSION_H
#include <memory>
#include <stdexcept>
#include <string>

namespace BstIdl
{
class FTypeRef;
class FInitializerExpression
{
public:
    struct initializer_error : std::runtime_error
    {
        initializer_error(const std::string &m) : std::runtime_error(m)
        {
        }

        initializer_error(const initializer_error &s) : std::runtime_error(s.what())
        {
        }

        ~initializer_error() noexcept = default;
    };

    FInitializerExpression() = default;
    virtual ~FInitializerExpression() = default;
    FInitializerExpression(const FInitializerExpression &) = default;
    FInitializerExpression(FInitializerExpression &&) noexcept = default;
    FInitializerExpression &operator=(const FInitializerExpression &) = default;
    FInitializerExpression &operator=(FInitializerExpression &&) = default;

    virtual void validate(std::shared_ptr<FTypeRef> &type, bool isArray);
    virtual void EvaluableValidate(std::shared_ptr<FTypeRef> &type, bool isArray, std::string &value, bool is_init_exp);
};

} // namespace BstIdl

#endif