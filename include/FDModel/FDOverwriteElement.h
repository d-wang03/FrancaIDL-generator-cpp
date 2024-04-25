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
#ifndef F_D_OVERWRITE_ELEMENT_H
#define F_D_OVERWRITE_ELEMENT_H
#include "FDTypeOverwrites.h"
namespace BstIdl
{
class FDOverwriteElement : virtual public FDElement
{
protected:
    std::shared_ptr<FDTypeOverwrites> m_overwrites;

public:
    FDOverwriteElement() = default;
    virtual ~FDOverwriteElement() = default;
    FDOverwriteElement(const FDOverwriteElement &) = default;
    FDOverwriteElement(FDOverwriteElement &&) noexcept = default;
    FDOverwriteElement &operator=(const FDOverwriteElement &) = default;
    std::shared_ptr<FDTypeOverwrites> const getOverwrites()
    {
        return m_overwrites;
    }
    void setOverwrites(const std::shared_ptr<FDTypeOverwrites> &new_overwrites)
    {
        m_overwrites = new_overwrites;
    };
};
} // namespace BstIdl
#endif