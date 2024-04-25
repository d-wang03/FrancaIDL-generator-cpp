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
#ifndef SOMEIP_I_DATE_PROPERTY_ACCESSOR_H
#define SOMEIP_I_DATE_PROPERTY_ACCESSOR_H
#include "capicxx-core-tools/IDataPropertyAccessor.h"
#include "model/FStructType.h"
#include "model/FUnionType.h"

namespace BstCommonAPI
{
enum class SomeIpDefaultErrorCoding
{
    Invalid,
    Header,
    MAX
};

enum class SomeIpAttributeEndianess
{
    Invalid,
    le,
    be,
    MAX
};

enum class SomeIpAttributeCRCWidth
{
    Invalid,
    zero,
    one,
    four,
    MAX
};

enum class SomeIpMethodEndianess
{
    Invalid,
    le,
    be,
    MAX
};

enum class SomeIpMethodCRCWidth
{
    Invalid,
    zero,
    one,
    four,
    MAX
};

enum class SomeIpErrorCoding
{
    Invalid,
    Header,
    MAX
};

enum class SomeIpBroadcastEndianess
{
    Invalid,
    le,
    be,
    MAX
};

enum class SomeIpBroadcastCRCWidth
{
    Invalid,
    zero,
    one,
    four,
    MAX
};

enum class SomeIpStringEncoding
{
    Invalid,
    utf8,
    utf16le,
    utf16be,
    MAX
};

class SomeipIDataPropertyAccessor : virtual public IDataPropertyAccessor
{
public:
    SomeipIDataPropertyAccessor() = default;
    virtual ~SomeipIDataPropertyAccessor() = default;
    SomeipIDataPropertyAccessor(const SomeipIDataPropertyAccessor &) = default;
    SomeipIDataPropertyAccessor(SomeipIDataPropertyAccessor &&) noexcept = default;
    SomeipIDataPropertyAccessor &operator=(const SomeipIDataPropertyAccessor &) = default;
    SomeipIDataPropertyAccessor &operator=(SomeipIDataPropertyAccessor &&)
    {
        return *this;
    }

    // host 'arrays'
    virtual int getSomeIpArrayMinLength(const std::shared_ptr<BstIdl::FArrayType> &obj) = 0;
    virtual int getSomeIpArrayMinLength(const std::shared_ptr<BstIdl::FField> &obj) = 0;
    virtual int getSomeIpArrayMaxLength(const std::shared_ptr<BstIdl::FArrayType> &obj) = 0;
    virtual int getSomeIpArrayMaxLength(const std::shared_ptr<BstIdl::FField> &obj) = 0;
    virtual int getSomeIpArrayLengthWidth(const std::shared_ptr<BstIdl::FArrayType> &obj) = 0;
    virtual int getSomeIpArrayLengthWidth(const std::shared_ptr<BstIdl::FField> &obj) = 0;

    // host 'unions'
    virtual int getSomeIpUnionLengthWidth(const std::shared_ptr<BstIdl::FUnionType> &obj) = 0;
    virtual int getSomeIpUnionTypeWidth(const std::shared_ptr<BstIdl::FUnionType> &obj) = 0;
    virtual int getSomeIpUnionDefaultOrder(const std::shared_ptr<BstIdl::FUnionType> &obj) = 0;
    virtual int getSomeIpUnionMaxLength(const std::shared_ptr<BstIdl::FUnionType> &obj) = 0;

    // host 'structs'
    virtual int getSomeIpStructLengthWidth(const std::shared_ptr<BstIdl::FStructType> &obj) = 0;

    // host 'enumerations'
    virtual int getSomeIpEnumWidth(const std::shared_ptr<BstIdl::FEnumerationType> &obj) = 0;
    virtual int getSomeIpEnumBitWidth(const std::shared_ptr<BstIdl::FEnumerationType> &obj) = 0;
    virtual int getSomeIpEnumInvalidValue(const std::shared_ptr<BstIdl::FEnumerationType> &obj) = 0;

    // host 'strings'
    virtual int getSomeIpStringLength(const std::shared_ptr<BstIdl::FObject> &obj) = 0;
    virtual int getSomeIpStringLengthWidth(const std::shared_ptr<BstIdl::FObject> &obj) = 0;
    virtual SomeIpStringEncoding getSomeIpStringEncoding(const std::shared_ptr<BstIdl::FObject> &obj) = 0;

    // host 'byte_buffers'
    virtual int getSomeIpByteBufferMaxLength(const std::shared_ptr<BstIdl::FObject> &obj) = 0;
    virtual int getSomeIpByteBufferMinLength(const std::shared_ptr<BstIdl::FObject> &obj) = 0;
    virtual int getSomeIpByteBufferLengthWidth(const std::shared_ptr<BstIdl::FObject> &obj) = 0;

    // host 'integers'
    virtual int getSomeIpIntegerBitWidth(const std::shared_ptr<BstIdl::FObject> &obj) = 0;
    virtual int getSomeIpIntegerInvalidValue(const std::shared_ptr<BstIdl::FObject> &obj) = 0;
};
} // namespace BstCommonAPI
#endif