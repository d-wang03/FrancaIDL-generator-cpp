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
#ifndef F_BRACKET_INITIALIZAER_H
#define F_BRACKET_INITIALIZAER_H

#include "FElementInitializer.h"
#include "FInitializer.h"
#include <list>
#include <memory>

namespace BstIdl
{

class FBracketInitializer : public FInitializer
{
public:
    FBracketInitializer() = default;
    virtual ~FBracketInitializer() = default;
    FBracketInitializer(const FBracketInitializer &) = default;
    FBracketInitializer(FBracketInitializer &&) noexcept = default;
    FBracketInitializer &operator=(const FBracketInitializer &) = default;
    FBracketInitializer &operator=(FBracketInitializer &&) = default;

    std::list<std::shared_ptr<FElementInitializer>> &getElements()
    {
        return m_elements;
    }

    void validate(std::shared_ptr<FTypeRef> &type, bool isArray) override;
    void EvaluableValidate(std::shared_ptr<FTypeRef> &type, bool isArray, std::string &value,
                           bool is_init_exp) override;

protected:
    std::list<std::shared_ptr<FElementInitializer>> m_elements;
};

} // namespace BstIdl

#endif