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
#ifndef F_D_GENERIC_H
#define F_D_GENERIC_H
#include "FDValue.h"
#include <iostream>
namespace BstIdl
{
class FDGeneric : public FDValue
{
    // FDGeneric.value might be one of:
    //   - [FDEnumerator|ID]
    //   - [FDInterfaceInstance|FQN]
public:
    FDGeneric() = default;
    virtual ~FDGeneric() = default;
    FDGeneric(const FDGeneric &) = default;
    FDGeneric(FDGeneric &&) noexcept = default;
    FDGeneric &operator=(const FDGeneric &) = default;
    FDGeneric &operator=(FDGeneric &&) = default;

    std::shared_ptr<FDObject> const getValue()
    {
        return m_value;
    }
    void setValue(const std::shared_ptr<FDObject> &newValue)
    {
        m_value = newValue;
    }
    bool isEnumerator() const override
    {
        return std::dynamic_pointer_cast<FDEnumerator>(m_value) != nullptr;
    }

    std::shared_ptr<FDEnumerator> getEnumerator() const override
    {
        return std::dynamic_pointer_cast<FDEnumerator>(m_value);
    }

protected:
    std::shared_ptr<FDObject> m_value;
};
} // namespace BstIdl
#endif