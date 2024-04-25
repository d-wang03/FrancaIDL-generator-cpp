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
#ifndef F_BLOCK_H
#define F_BLOCK_H

#include "FStatement.h"
#include <list>
#include <memory>

namespace BstIdl
{

class FBlock : public FStatement
{
public:
    FBlock() = default;
    virtual ~FBlock() = default;
    FBlock(const FBlock &) = default;
    FBlock(FBlock &&) noexcept = default;
    FBlock &operator=(const FBlock &) = default;
    FBlock &operator=(FBlock &&) = default;

    std::list<std::shared_ptr<FStatement>> &getStatements()
    {
        return m_statements;
    }

protected:
    std::list<std::shared_ptr<FStatement>> m_statements;
};

} // namespace BstIdl

#endif