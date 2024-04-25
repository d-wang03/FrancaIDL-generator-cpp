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
#ifndef F_ELEMENT_INITIALIZER_H
#define F_ELEMENT_INITIALIZER_H
#include "FInitializerExpression.h"
#include <memory>

namespace BstIdl
{

class FElementInitializer
{
public:
    FElementInitializer() = default;
    virtual ~FElementInitializer() = default;
    FElementInitializer(const FElementInitializer &) = default;
    FElementInitializer(FElementInitializer &&) noexcept = default;
    FElementInitializer &operator=(const FElementInitializer &) = default;
    FElementInitializer &operator=(FElementInitializer &&) = default;

    std::shared_ptr<FInitializerExpression> getFirst() const
    {
        return m_first;
    }
    void setFirst(const std::shared_ptr<FInitializerExpression> &value)
    {
        m_first = value;
    }
    std::shared_ptr<FInitializerExpression> getSecond() const
    {
        return m_second;
    }
    void setSecond(const std::shared_ptr<FInitializerExpression> &value)
    {
        m_second = value;
    }

protected:
    std::shared_ptr<FInitializerExpression> m_first;
    std::shared_ptr<FInitializerExpression> m_second;
};

} // namespace BstIdl

#endif