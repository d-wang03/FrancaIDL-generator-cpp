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
#include "model/FInitializerExpression.h"
#include "model/FTypeDef.h"

namespace BstIdl
{
void FInitializerExpression::validate(std::shared_ptr<FTypeRef> &type, bool isArray)
{
    if (type == nullptr)
        throw initializer_error("InitializerExpression:Null type to validate.");
    auto tmp_type = type->getDerived();
    if (tmp_type != nullptr)
    {
        auto typeDef = std::dynamic_pointer_cast<FTypeDef>(tmp_type);
        if (typeDef != nullptr)
        {
            type = typeDef->getActualType();
            validate(type, isArray);
        }
    }
}

void FInitializerExpression::EvaluableValidate(std::shared_ptr<FTypeRef> &type, bool isArray, std::string &value,
                                               bool is_init_exp)
{
    if (auto tmp_type = type->getDerived())
    {
        if (auto typeDef = std::dynamic_pointer_cast<FTypeDef>(tmp_type))
        {
            type = typeDef->getActualType();
            EvaluableValidate(type, isArray, value, false);
        }
    }
}
} // namespace BstIdl