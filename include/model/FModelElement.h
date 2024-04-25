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
#ifndef F_MODEL_ELEMENT_H
#define F_MODEL_ELEMENT_H

#include "FAnnotationBlock.h"
#include "FObject.h"
#include <memory>

namespace BstIdl
{
class FTypeCollection;
class FModelElement : public FObject
{
public:
    FModelElement(const std::string &name, const std::shared_ptr<FAnnotationBlock> &comment)
    {
        m_name = name;
        m_comment = comment;
    }
    FModelElement() = default;
    virtual ~FModelElement() = default;
    FModelElement(const FModelElement &) = default;
    FModelElement(FModelElement &&) noexcept = default;
    FModelElement &operator=(const FModelElement &) = default;
    FModelElement &operator=(FModelElement &&) = default;

    std::shared_ptr<FAnnotationBlock> getComment() const
    {
        return m_comment;
    }

    void setComment(const std::shared_ptr<FAnnotationBlock> &comment)
    {
        m_comment = comment;
    }

protected:
    std::shared_ptr<FAnnotationBlock> m_comment;
};

} // namespace BstIdl

#endif