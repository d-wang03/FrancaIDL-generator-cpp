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
#ifndef F_D_MAP_H
#define F_D_MAP_H
#include "FDMapKey.h"
#include "FDMapValue.h"
#include "FDTypeDefinition.h"
#include "model/FMapType.h"
namespace BstIdl
{
class FDMap : public FDTypeDefinition
{
protected:
    std::shared_ptr<BstIdl::FMapType> m_target;
    std::shared_ptr<FDMapKey> m_key;
    std::shared_ptr<FDMapValue> m_value;

public:
    FDMap() : FDTypeDefinition()
    {
        setHost("maps");
    }
    virtual ~FDMap() = default;
    FDMap(const FDMap &) = default;
    FDMap(FDMap &&) noexcept = default;
    FDMap &operator=(const FDMap &) = default;
    FDMap &operator=(FDMap &&) = default;
    std::shared_ptr<BstIdl::FMapType> getTarget() const
    {
        return m_target;
    }
    void setTarget(const std::shared_ptr<BstIdl::FMapType> &target)
    {
        m_target = target;
        if (getName().empty() && target)
            setName(target->getName());
    }
    std::shared_ptr<FDMapKey> const getKey()
    {
        return m_key;
    }
    void setKey(const std::shared_ptr<FDMapKey> &key)
    {
        m_key = key;
    }
    std::shared_ptr<FDMapValue> const getValue()
    {
        return m_value;
    }
    void setValue(const std::shared_ptr<FDMapValue> &value)
    {
        m_value = value;
    }
};
} // namespace BstIdl
#endif
