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
#ifndef F_COMPOUND_INITIALIZAER_H
#define F_COMPOUND_INITIALIZAER_H

#include "FFieldInitializer.h"
#include "FInitializer.h"
#include <list>
#include <memory>

namespace BstIdl
{

class FCompoundInitializer : public FInitializer
{
public:
    FCompoundInitializer() = default;
    virtual ~FCompoundInitializer() = default;
    FCompoundInitializer(const FCompoundInitializer &) = default;
    FCompoundInitializer(FCompoundInitializer &&) noexcept = default;
    FCompoundInitializer &operator=(const FCompoundInitializer &) = default;
    FCompoundInitializer &operator=(FCompoundInitializer &&) = default;

    std::list<std::shared_ptr<FFieldInitializer>> &getElements()
    {
        return m_elements;
    }

    void validate(std::shared_ptr<FTypeRef> &type, bool isArray) override;
    void EvaluableValidate(std::shared_ptr<FTypeRef> &type, bool isArray, std::string &value,
                           bool is_init_exp) override;

protected:
    std::list<std::shared_ptr<FFieldInitializer>> m_elements;
};

} // namespace BstIdl

#endif