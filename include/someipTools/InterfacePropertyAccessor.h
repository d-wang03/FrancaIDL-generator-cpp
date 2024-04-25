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
#ifndef F_INTEFACE_PROPERTY_ACCESSOR_H
#define F_INTEFACE_PROPERTY_ACCESSOR_H
#include "capicxx-core-tools/MappingGenericPropertyAccessor.h"
#include "model/FTypeCollection.h"
#include <memory>
namespace BstCommonAPI
{
class InterfacePropertyAccessor
{
public:
    InterfacePropertyAccessor() = default;
    virtual ~InterfacePropertyAccessor() = default;
    InterfacePropertyAccessor(const InterfacePropertyAccessor &) = default;
    InterfacePropertyAccessor(InterfacePropertyAccessor &&) noexcept = default;
    InterfacePropertyAccessor &operator=(const InterfacePropertyAccessor &) = default;
    InterfacePropertyAccessor &operator=(InterfacePropertyAccessor &&) = default;
    InterfacePropertyAccessor(const const std::shared_ptr<BtsDepl::MappingGenericPropertyAccessor> &target)
    {
        m_target = target;
    }

    enum SomeIpDefaultErrorCoding
    {
        Header
    };

    enum SomeIpAttributeEndianess
    {
        le,
        be
    };

    enum SomeIpAttributeCRCWidth
    {
        zero,
        one,
        four
    };

    enum SomeIpMethodEndianess
    {
        le,
        be
    };

    enum SomeIpMethodCRCWidth
    {
        zero,
        one,
        four
    };

    enum SomeIpErrorCoding
    {
        Header
    };

    enum SomeIpBroadcastEndianess
    {
        le,
        be
    };

    enum SomeIpBroadcastCRCWidth
    {
        zero,
        one,
        four
    };

    enum SomeIpStringEncoding
    {
        utf8,
        utf16le,
        utf16be
    };

    // host 'interfaces'
    int getSomeIpServiceID(const std::shared_ptr<BstIdl::FInterface> &obj)
    {
        return m_target->getInteger(obj, "SomeIpServiceID");
    }
    std::list<int> getSomeIpEventGroups(const std::shared_ptr<BstIdl::FInterface> &obj)
    {
        return m_target->getIntegerArray(obj, "SomeIpEventGroups");
    }

    // host 'attributes'
    int getSomeIpAttributeReliable(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        bool isOK = false;
        auto ret = m_target->getBoolean(obj, "SomeIpAttributeReliable", isOK);
        return isOK ? ret : -1;
    }
    int getSomeIpGetterID(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpGetterID");
    }
    int getSomeIpSetterID(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpSetterID");
    }
    int getSomeIpNotifierID(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpNotifierID");
    }
    std::list<int> getSomeIpNotifierEventGroups(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getIntegerArray(obj, "SomeIpNotifierEventGroups");
    }
    std::list<int> getSomeIpEventGroups(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getIntegerArray(obj, "SomeIpEventGroups");
    }
    int getSomeIpGetterMaxRetentionTime(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpGetterMaxRetentionTime");
    }
    int getSomeIpSetterMaxRetentionTime(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpSetterMaxRetentionTime");
    }
    int getSomeIpNotifierMaxRetentionTime(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpNotifierMaxRetentionTime");
    }
    int getSomeIpGetterResponseMaxRetentionTime(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpGetterResponseMaxRetentionTime");
    }
    int getSomeIpSetterResponseMaxRetentionTime(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpSetterResponseMaxRetentionTime");
    }
    int getSomeIpGetterRequestDebounceTime(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpGetterRequestDebounceTime");
    }
    int getSomeIpSetterRequestDebounceTime(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpSetterRequestDebounceTime");
    }
    int getSomeIpNotifierDebounceTime(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpNotifierDebounceTime");
    }
    int getSomeIpGetterResponseDebounceTime(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpGetterResponseDebounceTime");
    }
    int getSomeIpSetterResponseDebounceTime(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpSetterResponseDebounceTime");
    }
    int getSomeIpSetterSegmentLength(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpSetterSegmentLength");
    }
    int getSomeIpSetterSeparationTime(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpSetterSeparationTime");
    }
    int getSomeIpGetterSegmentLengthResponse(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpGetterSegmentLengthResponse");
    }
    int getSomeIpGetterSeparationTimeResponse(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpGetterSeparationTimeResponse");
    }
    int getSomeIpNotifierSegmentLength(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpNotifierSegmentLength");
    }
    int getSomeIpNotifierSeparationTime(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpNotifierSeparationTime");
    }
    // host 'methods'
    bool getSomeIpReliable(const std::shared_ptr<BstIdl::FMethod> &obj)
    {
        bool isOK = false;
        auto ret = m_target->getBoolean(obj, "SomeIpReliable", isOK);
        return isOK ? ret : -1;
    }
    int getSomeIpMethodID(const std::shared_ptr<BstIdl::FMethod> &obj)
    {
        return m_target->getInteger(obj, "SomeIpMethodID");
    }
    int getSomeIpMethodMaxRetentionTime(const std::shared_ptr<BstIdl::FMethod> &obj)
    {
        return m_target->getInteger(obj, "SomeIpMethodMaxRetentionTime");
    }
    int getSomeIpMethodResponseMaxRetentionTime(const std::shared_ptr<BstIdl::FMethod> &obj)
    {
        return m_target->getInteger(obj, "SomeIpMethodResponseMaxRetentionTime");
    }
    int getSomeIpMethodRequestDebounceTime(const std::shared_ptr<BstIdl::FMethod> &obj)
    {
        return m_target->getInteger(obj, "SomeIpMethodRequestDebounceTime");
    }
    int getSomeIpMethodResponseDebounceTime(const std::shared_ptr<BstIdl::FMethod> &obj)
    {
        return m_target->getInteger(obj, "SomeIpMethodResponseDebounceTime");
    }
    int getSomeIpMethodSegmentLength(const std::shared_ptr<BstIdl::FMethod> &obj)
    {
        return m_target->getInteger(obj, "SomeIpMethodSegmentLength");
    }
    int getSomeIpMethodSeparationTime(const std::shared_ptr<BstIdl::FMethod> &obj)
    {
        return m_target->getInteger(obj, "SomeIpMethodSeparationTime");
    }
    int getSomeIpMethodSegmentLengthResponse(const std::shared_ptr<BstIdl::FMethod> &obj)
    {
        return m_target->getInteger(obj, "SomeIpMethodSegmentLengthResponse");
    }
    int getSomeIpMethodSeparationTimeResponse(const std::shared_ptr<BstIdl::FMethod> &obj)
    {
        return m_target->getInteger(obj, "SomeIpMethodSeparationTimeResponse");
    }

    // host 'broadcasts'
    bool getSomeIpReliable(const std::shared_ptr<BstIdl::FBroadcast> &obj)
    {
        bool isOK = false;
        auto ret = m_target->getBoolean(obj, "SomeIpReliable", isOK);
        return isOK ? ret : -1;
    }
    int getSomeIpEventID(const std::shared_ptr<BstIdl::FBroadcast> &obj)
    {
        return m_target->getInteger(obj, "SomeIpEventID");
    }
    std::list<int> getSomeIpEventGroups(const std::shared_ptr<BstIdl::FBroadcast> &obj)
    {
        return m_target->getIntegerArray(obj, "SomeIpEventGroups");
    }
    int getSomeIpBroadcastMaxRetentionTime(const std::shared_ptr<BstIdl::FBroadcast> &obj)
    {
        return m_target->getInteger(obj, "SomeIpBroadcastMaxRetentionTime");
    }
    int getSomeIpBroadcastDebounceTime(const std::shared_ptr<BstIdl::FBroadcast> &obj)
    {
        return m_target->getInteger(obj, "SomeIpBroadcastDebounceTime");
    }
    int getSomeIpBroadcastSegmentLength(const std::shared_ptr<BstIdl::FBroadcast> &obj)
    {
        return m_target->getInteger(obj, "SomeIpBroadcastSegmentLength");
    }
    int getSomeIpBroadcastSeparationTime(const std::shared_ptr<BstIdl::FBroadcast> &obj)
    {
        return m_target->getInteger(obj, "SomeIpBroadcastSeparationTime");
    }

    // host 'arrays'

    int getSomeIpArrayMinLength(const std::shared_ptr<BstIdl::FArrayType> &obj)
    {
        return m_target->getInteger(obj, "SomeIpArrayMinLength");
    }

    int getSomeIpArrayMinLength(const std::shared_ptr<BstIdl::FField> &obj)
    {
        return m_target->getInteger(obj, "SomeIpArrayMinLength");
    }
    int getSomeIpArrayMinLength(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpArrayMinLength");
    }
    int getSomeIpArrayMinLength(const std::shared_ptr<BstIdl::FArgument> &obj)
    {
        return m_target->getInteger(obj, "SomeIpArrayMinLength");
    }

    int getSomeIpArrayMaxLength(const std::shared_ptr<BstIdl::FArrayType> &obj)
    {
        return m_target->getInteger(obj, "SomeIpArrayMaxLength");
    }

    int getSomeIpArrayMaxLength(const std::shared_ptr<BstIdl::FField> &obj)
    {
        return m_target->getInteger(obj, "SomeIpArrayMaxLength");
    }
    int getSomeIpArrayMaxLength(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpArrayMaxLength");
    }
    int getSomeIpArrayMaxLength(const std::shared_ptr<BstIdl::FArgument> &obj)
    {
        return m_target->getInteger(obj, "SomeIpArrayMaxLength");
    }

    int getSomeIpArrayLengthWidth(const std::shared_ptr<BstIdl::FArrayType> &obj)
    {
        return m_target->getInteger(obj, "SomeIpArrayLengthWidth");
    }

    int getSomeIpArrayLengthWidth(const std::shared_ptr<BstIdl::FField> &obj)
    {
        return m_target->getInteger(obj, "SomeIpArrayLengthWidth");
    }
    int getSomeIpArrayLengthWidth(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpArrayLengthWidth");
    }
    int getSomeIpArrayLengthWidth(const std::shared_ptr<BstIdl::FArgument> &obj)
    {
        return m_target->getInteger(obj, "SomeIpArrayLengthWidth");
    }

    // host 'unions'

    int getSomeIpUnionLengthWidth(const std::shared_ptr<BstIdl::FUnionType> &obj)
    {
        return m_target->getInteger(obj, "SomeIpUnionLengthWidth");
    }

    int getSomeIpUnionTypeWidth(const std::shared_ptr<BstIdl::FUnionType> &obj)
    {
        return m_target->getInteger(obj, "SomeIpUnionTypeWidth");
    }

    bool getSomeIpUnionDefaultOrder(const std::shared_ptr<BstIdl::FUnionType> &obj)
    {
        bool isOK = false;
        auto ret = m_target->getBoolean(obj, "SomeIpUnionDefaultOrder", isOK);
        return isOK ? ret : -1;
    }

    int getSomeIpUnionMaxLength(const std::shared_ptr<BstIdl::FUnionType> &obj)
    {
        return m_target->getInteger(obj, "SomeIpUnionMaxLength");
    }

    // host 'structs'

    int getSomeIpStructLengthWidth(const std::shared_ptr<BstIdl::FStructType> &obj)
    {
        return m_target->getInteger(obj, "SomeIpStructLengthWidth");
    }

    // host 'enumerations'

    int getSomeIpEnumWidth(const std::shared_ptr<BstIdl::FEnumerationType> &obj)
    {
        return m_target->getInteger(obj, "SomeIpEnumWidth");
    }

    int getSomeIpEnumBitWidth(const std::shared_ptr<BstIdl::FEnumerationType> &obj)
    {
        return m_target->getInteger(obj, "SomeIpEnumBitWidth");
    }

    int getSomeIpEnumInvalidValue(const std::shared_ptr<BstIdl::FEnumerationType> &obj)
    {
        return m_target->getInteger(obj, "SomeIpEnumInvalidValue");
    }

    // host 'strings'

    int getSomeIpStringLength(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return m_target->getInteger(obj, "SomeIpStringLength");
    }

    int getSomeIpStringLengthWidth(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return m_target->getInteger(obj, "SomeIpStringLengthWidth");
    }

    SomeIpAttributeEndianess getSomeIpAttributeEndianess(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        std::string e = target->getEnum(obj, "SomeIpAttributeEndianess");
        if (!e.empty())
            return SomeIpAttributeEndianess::Invalid;
        return convertSomeIpAttributeEndianess(e);
    }

    SomeIpStringEncoding getSomeIpStringEncoding(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        auto e = m_target->getEnum(obj, "SomeIpStringEncoding");
        if (e.empty())
            return SomeIpStringEncoding::Invalid;
        return DataPropertyAccessorHelper.convertSomeIpStringEncoding(e);
    }

    // host 'byte_buffers'

    int getSomeIpByteBufferMaxLength(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return m_target->getInteger(obj, "SomeIpByteBufferMaxLength");
    }

    int getSomeIpByteBufferMinLength(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return m_target->getInteger(obj, "SomeIpByteBufferMinLength");
    }

    int getSomeIpByteBufferLengthWidth(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return m_target->getInteger(obj, "SomeIpByteBufferLengthWidth");
    }

    // host 'integers'

    int getSomeIpIntegerBitWidth(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return m_target->getInteger(obj, "SomeIpIntegerBitWidth");
    }

    int getSomeIpIntegerInvalidValue(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return m_target->getInteger(obj, "SomeIpIntegerInvalidValue");
    }

    // host 'attributes'
    int getSomeIpAttrMapMinLength(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpAttrMapMinLength");
    }
    int getSomeIpAttrMapMaxLength(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpAttrMapMaxLength");
    }
    int getSomeIpAttrMapLengthWidth(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpAttrMapLengthWidth");
    }

    // host 'arguments'
    int getSomeIpArgMapMinLength(const std::shared_ptr<BstIdl::FArgument> &obj)
    {
        return m_target->getInteger(obj, "SomeIpArgMapMinLength");
    }
    int getSomeIpArgMapMaxLength(const std::shared_ptr<BstIdl::FArgument> &obj)
    {
        return m_target->getInteger(obj, "SomeIpArgMapMaxLength");
    }
    int getSomeIpArgMapLengthWidth(const std::shared_ptr<BstIdl::FArgument> &obj)
    {
        return m_target->getInteger(obj, "SomeIpArgMapLengthWidth");
    }

    SomeIpDefaultErrorCoding convertSomeIpDefaultErrorCoding(std::string val)
    {
        if (val == "Header")
            return SomeIpDefaultErrorCoding::Header;
        return SomeIpDefaultErrorCoding::Invalid;
    }

    SomeIpAttributeEndianess convertSomeIpAttributeEndianess(std::string val)
    {
        if (val == "le")
            return SomeIpAttributeEndianess::le;
        else if (val == "be")
            return SomeIpAttributeEndianess::be;
        return SomeIpAttributeEndianess::Invalid;
    }

    SomeIpAttributeCRCWidth convertSomeIpAttributeCRCWidth(std::string val)
    {
        if (val == "zero")
            return SomeIpAttributeCRCWidth::zero;
        else if (val == "one")
            return SomeIpAttributeCRCWidth::one;
        else if (val == "four")
            return SomeIpAttributeCRCWidth::four;
        return SomeIpAttributeCRCWidth::Invalid;
    }

    SomeIpMethodEndianess convertSomeIpMethodEndianess(std::string val)
    {
        if (val == "le")
            return SomeIpMethodEndianess::le;
        else if (val == "be")
            return SomeIpMethodEndianess::be;
        return SomeIpMethodEndianess::Invalid;
    }

    SomeIpMethodCRCWidth convertSomeIpMethodCRCWidth(std::string val)
    {
        if (val == "zero")
            return SomeIpMethodCRCWidth::zero;
        else if (val == "one")
            return SomeIpMethodCRCWidth::one;
        else if (val == "four")
            return SomeIpMethodCRCWidth::four;
        return SomeIpMethodCRCWidth::Invalid;
    }

    SomeIpErrorCoding convertSomeIpErrorCoding(std::string val)
    {
        if (val == "Header")
            return SomeIpErrorCoding::Header;
        return SomeIpErrorCoding::Invalid;
    }

    SomeIpBroadcastEndianess convertSomeIpBroadcastEndianess(std::string val)
    {
        if (val == "le")
            return SomeIpBroadcastEndianess::le;
        else if (val == "be")
            return SomeIpBroadcastEndianess::be;
        return SomeIpBroadcastEndianess::Invalid;
    }

    SomeIpBroadcastCRCWidth convertSomeIpBroadcastCRCWidth(std::string val)
    {
        if (val == "zero")
            return SomeIpBroadcastCRCWidth::zero;
        else if (val == "one")
            return SomeIpBroadcastCRCWidth::one;
        else if (val == "four")
            return SomeIpBroadcastCRCWidth::four;
        return SomeIpBroadcastCRCWidth::Invalid;
    }

    SomeIpStringEncoding convertSomeIpStringEncoding(std::string val)
    {
        if (val == "utf8")
            return SomeIpStringEncoding::utf8;
        else if (val == "utf16le")
            return SomeIpStringEncoding::utf16le;
        else if (val == "utf16be")
            return SomeIpStringEncoding::utf16be;
        return SomeIpStringEncoding::Invalid;
    }

private:
    const std::shared_ptr<BtsDepl::MappingGenericPropertyAccessor> m_target;
};
} // namespace BstCommonAPI