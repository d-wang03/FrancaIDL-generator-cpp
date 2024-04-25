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
#ifndef F_D_VALUE_ARRAY_H
#define F_D_VALUE_ARRAY_H
#include "FDValue.h"
#include <list>
#include <memory>
namespace BstIdl
{
class FDValueArray : public FDObject
{
public:
    FDValueArray() : FDObject()
    {
        setName(__func__);
    }
    virtual ~FDValueArray() = default;
    FDValueArray(const FDValueArray &) = default;
    FDValueArray(FDValueArray &&) noexcept = default;
    FDValueArray &operator=(const FDValueArray &) = default;
    FDValueArray &operator=(FDValueArray &&) = default;

    std::list<std::shared_ptr<FDValue>> const getValues()
    {
        return values;
    }
    void setValues(const std::shared_ptr<FDValue> &value)
    {
        values.emplace_back(value);
    }

    std::string toString() const
    {
        std::string ret;
        for (const auto &item : values)
        {
            ret += item->getName() + ",";
        }
        if (!ret.empty())
            ret.pop_back();
        else
            ret = "EmptyArray";
        return ret;
    }

protected:
    std::list<std::shared_ptr<FDValue>> values;
};
} // namespace BstIdl

#endif