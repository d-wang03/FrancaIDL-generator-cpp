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
#ifndef I_DATE_PROPERTY_ACCESSOR_H
#define I_DATE_PROPERTY_ACCESSOR_H
#include "model/FArrayType.h"
#include "model/FEnumerationType.h"
#include "model/FField.h"
#include "model/FTypeCollection.h"
#include <memory>

namespace BstCommonAPI
{
enum class AttributeNotifierUpdateMode
{
    Invalid,
    Cyclic,
    OnChange,
    MAX
};

enum class EnumBackingType
{
    Default,
    UInt8,
    UInt16,
    UInt32,
    UInt64,
    Int8,
    Int16,
    Int32,
    Int64,
    MAX
};

enum class ErrorType
{
    Invalid,
    Error,
    Warning,
    Info,
    NoError,
    MAX
};

enum class BroadcastType
{
    Invalid,
    signal,
    error,
    MAX
};

class IDataPropertyAccessor
{
public:
    IDataPropertyAccessor() = default;
    virtual ~IDataPropertyAccessor() = default;
    IDataPropertyAccessor(const IDataPropertyAccessor &) = default;
    IDataPropertyAccessor(IDataPropertyAccessor &&) noexcept = default;
    IDataPropertyAccessor &operator=(const IDataPropertyAccessor &) = default;

    // host 'enumerations'
    virtual EnumBackingType getEnumBackingType(const std::shared_ptr<BstIdl::FEnumerationType> &obj) = 0;

    // host 'enumerators'
    virtual ErrorType getErrorType(const std::shared_ptr<BstIdl::FEnumerator> &obj) = 0;

    virtual std::shared_ptr<IDataPropertyAccessor> getOverwriteAccessor(const std::shared_ptr<BstIdl::FField> &obj) = 0;

    virtual std::shared_ptr<IDataPropertyAccessor> getOverwriteAccessor(
        const std::shared_ptr<BstIdl::FArrayType> &obj) = 0;
};
} // namespace BstCommonAPI
#endif