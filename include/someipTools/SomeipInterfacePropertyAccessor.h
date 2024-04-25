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
#include "SomeipDataPropertyAccessorHelper.h"
#include "SomeipIDataPropertyAccessor.h"
#include "capicxx-core-tools/InterfacePropertyAccessor.h"
namespace BstCommonAPI
{
class SomeipInterfacePropertyAccessor : virtual public InterfacePropertyAccessor,
                                        virtual public SomeipIDataPropertyAccessor
{
public:
    SomeipInterfacePropertyAccessor() = default;
    virtual ~SomeipInterfacePropertyAccessor() = default;
    SomeipInterfacePropertyAccessor(const SomeipInterfacePropertyAccessor &) = default;
    SomeipInterfacePropertyAccessor(SomeipInterfacePropertyAccessor &&) noexcept = default;
    SomeipInterfacePropertyAccessor &operator=(const SomeipInterfacePropertyAccessor &) = default;
    SomeipInterfacePropertyAccessor &operator=(SomeipInterfacePropertyAccessor &&rhs)
    {
        m_target = std::move(rhs.m_target);
        return *this;
    }
    SomeipInterfacePropertyAccessor(const std::shared_ptr<BstIdl::FDeployedInterface> &target)
        : InterfacePropertyAccessor(target)
    {
    }

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
    SomeIpAttributeEndianess getSomeIpAttributeEndianess(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        std::string e = m_target->getEnum(obj, "SomeIpAttributeEndianess");
        if (e.empty())
            return SomeIpAttributeEndianess::Invalid;
        return SomeipDataPropertyAccessorHelper::convertSomeIpAttributeEndianess(e);
    }
    SomeIpAttributeCRCWidth getSomeIpAttributeCRCWidth(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        std::string e = m_target->getEnum(obj, "SomeIpAttributeCRCWidth");
        if (e.empty())
            return SomeIpAttributeCRCWidth::Invalid;
        return SomeipDataPropertyAccessorHelper::convertSomeIpAttributeCRCWidth(e);
    }
    // host 'methods'
    bool getSomeIpReliable(const std::shared_ptr<BstIdl::FMethod> &obj)
    {
        bool isOK = false;
        auto ret = m_target->getBoolean(obj, "SomeIpReliable", isOK);
        return isOK ? ret : false;
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

    SomeIpMethodEndianess getSomeIpMethodEndianess(const std::shared_ptr<BstIdl::FMethod> &obj)
    {
        std::string e = m_target->getEnum(obj, "SomeIpMethodEndianess");
        if (e.empty())
            return SomeIpMethodEndianess::Invalid;
        return SomeipDataPropertyAccessorHelper::convertSomeIpMethodEndianess(e);
    }
    SomeIpMethodCRCWidth getSomeIpMethodCRCWidth(const std::shared_ptr<BstIdl::FMethod> &obj)
    {
        std::string e = m_target->getEnum(obj, "SomeIpMethodCRCWidth");
        if (e.empty())
            return SomeIpMethodCRCWidth::Invalid;
        return SomeipDataPropertyAccessorHelper::convertSomeIpMethodCRCWidth(e);
    }
    SomeIpErrorCoding getSomeIpErrorCoding(const std::shared_ptr<BstIdl::FMethod> &obj)
    {
        std::string e = m_target->getEnum(obj, "SomeIpErrorCoding");
        if (e.empty())
            return SomeIpErrorCoding::Invalid;
        return SomeipDataPropertyAccessorHelper::convertSomeIpErrorCoding(e);
    }

    // host 'broadcasts'
    bool getSomeIpReliable(const std::shared_ptr<BstIdl::FBroadcast> &obj)
    {
        bool isOK = false;
        auto ret = m_target->getBoolean(obj, "SomeIpReliable", isOK);
        return isOK ? ret : false;
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

    SomeIpBroadcastEndianess getSomeIpBroadcastEndianess(const std::shared_ptr<BstIdl::FBroadcast> &obj)
    {
        std::string e = m_target->getEnum(obj, "SomeIpBroadcastEndianess");
        if (e.empty())
            return SomeIpBroadcastEndianess::Invalid;
        return SomeipDataPropertyAccessorHelper::convertSomeIpBroadcastEndianess(e);
    }
    SomeIpBroadcastCRCWidth getSomeIpBroadcastCRCWidth(const std::shared_ptr<BstIdl::FBroadcast> &obj)
    {
        std::string e = m_target->getEnum(obj, "SomeIpBroadcastCRCWidth");
        if (e.empty())
            return SomeIpBroadcastCRCWidth::Invalid;
        return SomeipDataPropertyAccessorHelper::convertSomeIpBroadcastCRCWidth(e);
    }

    // host 'arrays'

    int getSomeIpArrayMinLength(const std::shared_ptr<BstIdl::FArrayType> &obj) override
    {
        return m_target->getInteger(obj, "SomeIpArrayMinLength");
    }

    int getSomeIpArrayMinLength(const std::shared_ptr<BstIdl::FField> &obj) override
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

    int getSomeIpArrayMaxLength(const std::shared_ptr<BstIdl::FArrayType> &obj) override
    {
        return m_target->getInteger(obj, "SomeIpArrayMaxLength");
    }

    int getSomeIpArrayMaxLength(const std::shared_ptr<BstIdl::FField> &obj) override
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

    int getSomeIpArrayLengthWidth(const std::shared_ptr<BstIdl::FArrayType> &obj) override
    {
        return m_target->getInteger(obj, "SomeIpArrayLengthWidth");
    }

    int getSomeIpArrayLengthWidth(const std::shared_ptr<BstIdl::FField> &obj) override
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
        auto e = m_target->getEnum(obj, "SomeIpStringEncoding");
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

    /**
     * Get an overwrite-aware accessor for deployment properties.</p>
     *
     * This accessor will return overwritten property values in the context
     * of a Franca FAttribute object. I.e., the FAttribute obj has a datatype
     * which can be overwritten in the deployment definition (e.g., Franca array,
     * struct, union or enumeration). The accessor will return the overwritten values.
     * If the deployment definition didn't overwrite the value, this accessor will
     * delegate to its parent accessor.</p>
     *
     * @param obj a Franca FAttribute which is the context for the accessor
     * @return the overwrite-aware accessor
     */
    std::shared_ptr<IDataPropertyAccessor> getOverwriteAccessor(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return SomeipDataPropertyAccessorHelper::getSomeipOverwriteAccessorAux(obj, m_target, shared_from_this());
    }

    /**
     * Get an overwrite-aware accessor for deployment properties.</p>
     *
     * This accessor will return overwritten property values in the context
     * of a Franca FArgument object. I.e., the FArgument obj has a datatype
     * which can be overwritten in the deployment definition (e.g., Franca array,
     * struct, union or enumeration). The accessor will return the overwritten values.
     * If the deployment definition didn't overwrite the value, this accessor will
     * delegate to its parent accessor.</p>
     *
     * @param obj a Franca FArgument which is the context for the accessor
     * @return the overwrite-aware accessor
     */

    std::shared_ptr<IDataPropertyAccessor> getOverwriteAccessor(const std::shared_ptr<BstIdl::FArgument> &obj)
    {
        return SomeipDataPropertyAccessorHelper::getSomeipOverwriteAccessorAux(obj, m_target, shared_from_this());
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
        return InterfacePropertyAccessor::getEnumBackingType(obj);
    }

    ErrorType getErrorType(const std::shared_ptr<BstIdl::FEnumerator> &obj) override
    {
        return InterfacePropertyAccessor::getErrorType(obj);
    }
};
} // namespace BstCommonAPI
#endif