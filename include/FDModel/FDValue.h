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
#ifndef F_D_VALUE_H
#define F_D_VALUE_H
#include "FDObject.h"
namespace BstIdl
{
class FInterface;
class FDEnumerator;
class FDValue : public FDObject
{
public:
    FDValue() = default;
    virtual ~FDValue() = default;
    FDValue(const FDValue &) = default;
    FDValue(FDValue &&) noexcept = default;
    FDValue &operator=(const FDValue &) = default;
    FDValue &operator=(FDValue &&) = default;

    virtual bool isBoolean() const
    {
        return false;
    }
    virtual bool isInteger() const
    {
        return false;
    }
    virtual bool isInterfaceRef() const
    {
        return false;
    }
    virtual bool isString() const
    {
        return false;
    }
    virtual bool isEnumerator() const
    {
        return false;
    }

    virtual bool getBoolean() const
    {
        return false;
    }
    virtual int getInteger() const
    {
        return 0;
    }
    virtual std::shared_ptr<BstIdl::FInterface> getInterfaceRef() const
    {
        return nullptr;
    }
    virtual std::string getString() const
    {
        return std::string();
    }
    virtual std::shared_ptr<FDEnumerator> getEnumerator() const
    {
        return nullptr;
    }
};
} // namespace BstIdl
#endif