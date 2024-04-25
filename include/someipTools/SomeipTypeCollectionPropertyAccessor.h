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
#ifndef SOMEIP_TYPE_COLLECTION_PROPERTY_ACCESSOR_H
#define SOMEIP_TYPE_COLLECTION_PROPERTY_ACCESSOR_H
#include "SomeipDataPropertyAccessorHelper.h"
#include "SomeipIDataPropertyAccessor.h"
#include "capicxx-core-tools/TypeCollectionPropertyAccessor.h"
#include <memory>
namespace BstCommonAPI
{
class SomeipTypeCollectionPropertyAccessor : virtual public TypeCollectionPropertyAccessor,
                                             virtual public SomeipIDataPropertyAccessor
{
public:
    SomeipTypeCollectionPropertyAccessor() = default;
    virtual ~SomeipTypeCollectionPropertyAccessor() = default;
    SomeipTypeCollectionPropertyAccessor(const SomeipTypeCollectionPropertyAccessor &) = default;
    SomeipTypeCollectionPropertyAccessor(SomeipTypeCollectionPropertyAccessor &&) noexcept = default;
    SomeipTypeCollectionPropertyAccessor &operator=(const SomeipTypeCollectionPropertyAccessor &) = default;
    SomeipTypeCollectionPropertyAccessor &operator=(SomeipTypeCollectionPropertyAccessor &&rhs)
    {
        m_target = std::move(rhs.m_target);
        return *this;
    }

    SomeipTypeCollectionPropertyAccessor(const std::shared_ptr<BstIdl::MappingGenericPropertyAccessor> &target)
        : TypeCollectionPropertyAccessor(target)
    {
    }

    int getSomeIpArrayMinLength(const std::shared_ptr<BstIdl::FArrayType> &obj) override
    {
        return m_target->getInteger(obj, "SomeIpArrayMinLength");
    }

    int getSomeIpArrayMinLength(const std::shared_ptr<BstIdl::FField> &obj) override
    {
        return m_target->getInteger(obj, "SomeIpArrayMinLength");
    }

    int getSomeIpArrayMaxLength(const std::shared_ptr<BstIdl::FArrayType> &obj) override
    {
        return m_target->getInteger(obj, "SomeIpArrayMaxLength");
    }

    int getSomeIpArrayMaxLength(const std::shared_ptr<BstIdl::FField> &obj) override
    {
        return m_target->getInteger(obj, "SomeIpArrayMaxLength");
    }

    int getSomeIpArrayLengthWidth(const std::shared_ptr<BstIdl::FArrayType> &obj) override
    {
        return m_target->getInteger(obj, "SomeIpArrayLengthWidth");
    }

    int getSomeIpArrayLengthWidth(const std::shared_ptr<BstIdl::FField> &obj) override
    {
        return m_target->getInteger(obj, "SomeIpArrayLengthWidth");
    }

    // host 'unions'

    int getSomeIpUnionLengthWidth(const std::shared_ptr<BstIdl::FUnionType> &obj) override
    {
        return m_target->getInteger(obj, "SomeIpUnionLengthWidth");
    }

    int getSomeIpUnionTypeWidth(const std::shared_ptr<BstIdl::FUnionType> &obj) override
    {
        return m_target->getInteger(obj, "SomeIpUnionTypeWidth");
    }

    int getSomeIpUnionDefaultOrder(const std::shared_ptr<BstIdl::FUnionType> &obj) override
    {
        bool isOK = false;
        auto ret = m_target->getBoolean(obj, "SomeIpUnionDefaultOrder", isOK);
        return isOK ? ret : -1;
    }

    int getSomeIpUnionMaxLength(const std::shared_ptr<BstIdl::FUnionType> &obj) override
    {
        return m_target->getInteger(obj, "SomeIpUnionMaxLength");
    }

    // host 'structs'

    int getSomeIpStructLengthWidth(const std::shared_ptr<BstIdl::FStructType> &obj) override
    {
        return m_target->getInteger(obj, "SomeIpStructLengthWidth");
    }

    // host 'enumerations'

    int getSomeIpEnumWidth(const std::shared_ptr<BstIdl::FEnumerationType> &obj) override
    {
        return m_target->getInteger(obj, "SomeIpEnumWidth");
    }

    int getSomeIpEnumBitWidth(const std::shared_ptr<BstIdl::FEnumerationType> &obj) override
    {
        return m_target->getInteger(obj, "SomeIpEnumBitWidth");
    }

    int getSomeIpEnumInvalidValue(const std::shared_ptr<BstIdl::FEnumerationType> &obj) override
    {
        return m_target->getInteger(obj, "SomeIpEnumInvalidValue");
    }

    // host 'strings'

    int getSomeIpStringLength(const std::shared_ptr<BstIdl::FObject> &obj) override
    {
        return m_target->getInteger(obj, "SomeIpStringLength");
    }

    int getSomeIpStringLengthWidth(const std::shared_ptr<BstIdl::FObject> &obj) override
    {
        return m_target->getInteger(obj, "SomeIpStringLengthWidth");
    }

    SomeIpStringEncoding getSomeIpStringEncoding(const std::shared_ptr<BstIdl::FObject> &obj) override
    {
        std::string e = m_target->getEnum(obj, "SomeIpStringEncoding");
        if (e.empty())
            return SomeIpStringEncoding::Invalid;
        return SomeipDataPropertyAccessorHelper::convertSomeIpStringEncoding(e);
    }

    // host 'byte_buffers'

    int getSomeIpByteBufferMaxLength(const std::shared_ptr<BstIdl::FObject> &obj) override
    {
        return m_target->getInteger(obj, "SomeIpByteBufferMaxLength");
    }

    int getSomeIpByteBufferMinLength(const std::shared_ptr<BstIdl::FObject> &obj) override
    {
        return m_target->getInteger(obj, "SomeIpByteBufferMinLength");
    }

    int getSomeIpByteBufferLengthWidth(const std::shared_ptr<BstIdl::FObject> &obj) override
    {
        return m_target->getInteger(obj, "SomeIpByteBufferLengthWidth");
    }

    // host 'integers'

    int getSomeIpIntegerBitWidth(const std::shared_ptr<BstIdl::FObject> &obj) override
    {
        return m_target->getInteger(obj, "SomeIpIntegerBitWidth");
    }

    int getSomeIpIntegerInvalidValue(const std::shared_ptr<BstIdl::FObject> &obj) override
    {
        return m_target->getInteger(obj, "SomeIpIntegerInvalidValue");
    }

    std::shared_ptr<IDataPropertyAccessor> getOverwriteAccessor(const std::shared_ptr<BstIdl::FField> &obj) override
    {
        return SomeipDataPropertyAccessorHelper::getSomeipOverwriteAccessorAux(obj, m_target, shared_from_this());
    }

    std::shared_ptr<IDataPropertyAccessor> getOverwriteAccessor(const std::shared_ptr<BstIdl::FArrayType> &obj) override
    {
        return SomeipDataPropertyAccessorHelper::getSomeipOverwriteAccessorAux(obj, m_target, shared_from_this());
    }

    EnumBackingType getEnumBackingType(const std::shared_ptr<BstIdl::FEnumerationType> &obj) override
    {
        return TypeCollectionPropertyAccessor::getEnumBackingType(obj);
    }

    ErrorType getErrorType(const std::shared_ptr<BstIdl::FEnumerator> &obj) override
    {
        return TypeCollectionPropertyAccessor::getErrorType(obj);
    }
};
} // namespace BstCommonAPI
#endif