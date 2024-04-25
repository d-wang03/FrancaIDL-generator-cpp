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
#ifndef F_MAP_TYPE_H
#define F_MAP_TYPE_H

#include "FType.h"
#include "FTypeRef.h"
#include <memory>

namespace BstIdl
{

class FMapType : public FType
{
public:
    FMapType() = default;
    virtual ~FMapType() = default;
    FMapType(const FMapType &) = default;
    FMapType(FMapType &&) noexcept = default;
    FMapType &operator=(const FMapType &) = default;
    FMapType &operator=(FMapType &&) = default;

    std::shared_ptr<FTypeRef> getKeyType() const
    {
        return m_keyType;
    }
    void setKeyType(const std::shared_ptr<FTypeRef> &value)
    {
        m_keyType = value;
    }
    std::shared_ptr<FTypeRef> getValueType() const
    {
        return m_valueType;
    }
    void setValueType(const std::shared_ptr<FTypeRef> &value)
    {
        m_valueType = value;
    }
    std::string transform(std::string func) override
    {
        std::string ret = getName();
        auto trans = FObject::s_transMap[func];
        if (trans && trans->forFMapType)
            ret = trans->forFMapType(this);
        return ret;
    }

protected:
    std::shared_ptr<FTypeRef> m_keyType;
    std::shared_ptr<FTypeRef> m_valueType;
};

} // namespace BstIdl

#endif
