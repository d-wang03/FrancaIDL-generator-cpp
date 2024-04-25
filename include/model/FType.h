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
#ifndef F_TYPE_H
#define F_TYPE_H

#include "FModelElement.h"

namespace BstIdl
{
class FTypeCollection;
class FEvaluableElement;
class FType : public FModelElement
{
public:
    FType() = default;
    virtual ~FType() = default;
    FType(const FType &) = default;
    FType(FType &&) noexcept = default;
    FType &operator=(const FType &) = default;
    FType &operator=(FType &&) = default;

    bool isPublic() const
    {
        return m_isPublic;
    }
    void setPublic(bool value)
    {
        m_isPublic = value;
    }
    virtual std::shared_ptr<FEvaluableElement> findElement(std::list<std::string> &fqn)
    {
        return nullptr;
    }

protected:
    bool m_isPublic;
};

} // namespace BstIdl

#endif