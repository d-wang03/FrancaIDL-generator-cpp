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
#ifndef F_D_COMPLEX_VALUE_H
#define F_D_COMPLEX_VALUE_H
#include "FDValueArray.h"
namespace BstIdl
{
class FDComplexValue : public FDObject
{
public:
    FDComplexValue() : FDObject()
    {
        setName(__func__);
    }
    virtual ~FDComplexValue() = default;
    FDComplexValue(const FDComplexValue &) = default;
    FDComplexValue(FDComplexValue &&) noexcept = default;
    FDComplexValue &operator=(const FDComplexValue &) = default;
    FDComplexValue &operator=(FDComplexValue &&) = default;

    bool isSingle() const
    {
        return single != nullptr;
    }
    std::shared_ptr<FDValue> getSingle() const
    {
        return single;
    }
    void setSingle(const std::shared_ptr<FDValue> &newSingle)
    {
        single = newSingle;
        array = nullptr;
    }
    bool isArray() const
    {
        return array != nullptr;
    }
    std::shared_ptr<FDValueArray> getArray() const
    {
        return array;
    }
    void setArray(const std::shared_ptr<FDValueArray> &newArray)
    {
        array = newArray;
        single = nullptr;
    }

    std::string toString() const
    {
        if (single)
            return single->getName();

        if (array)
            return array->toString();

        return "Unknown";
    }

protected:
    std::shared_ptr<FDValue> single;
    std::shared_ptr<FDValueArray> array;
};

} // namespace BstIdl
#endif