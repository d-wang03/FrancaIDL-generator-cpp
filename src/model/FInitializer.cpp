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
#include "model/FInitializer.h"
#include "model/FTypeRef.h"

namespace BstIdl
{
void FInitializer::validate(std::shared_ptr<FTypeRef> &type, bool isArray)
{
    FInitializerExpression::validate(type, isArray);
    if (!isArray && type->getDerived() == nullptr)
        throw initializer_error("Initializer:Initilizer not match type.\nactual type:" +
                                type->getPredefined()->getName() + "\texpect:   1:typeA[] 2:derived type");
}
void FInitializer::EvaluableValidate(std::shared_ptr<FTypeRef> &type, bool isArray, std::string &value,
                                     bool is_init_exp)
{
    FInitializerExpression::EvaluableValidate(type, isArray, value, is_init_exp);
}
} // namespace BstIdl