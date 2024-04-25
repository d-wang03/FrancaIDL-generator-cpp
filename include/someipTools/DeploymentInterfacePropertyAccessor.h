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
#ifndef DEPLOYMENT_INTERFACE_PROPERTY_ACCESSOR
#define DEPLOYMENT_INTERFACE_PROPERTY_ACCESSOR
#include "FDModel/FDeployedInterface.h"
namespace BstCommonAPI
{
class DeploymentInterfacePropertyAccessor
{

public:
    DeploymentInterfacePropertyAccessor() = default;
    virtual ~DeploymentInterfacePropertyAccessor() = default;
    DeploymentInterfacePropertyAccessor(const DeploymentInterfacePropertyAccessor &) = default;
    DeploymentInterfacePropertyAccessor(DeploymentInterfacePropertyAccessor &&) noexcept = default;
    DeploymentInterfacePropertyAccessor &operator=(const DeploymentInterfacePropertyAccessor &) = default;
    DeploymentInterfacePropertyAccessor &operator=(DeploymentInterfacePropertyAccessor &&) = default;
    DeploymentInterfacePropertyAccessor(const std::shared_ptr<BstIdl::FDeployedInterface> &target)
    {
        m_target = target;
    }

    int getSomeIpServiceID(const std::shared_ptr<BstIdl::FInterface> &obj)
    {
        return m_target->getInteger(obj, "SomeIpServiceID");
    }

    std::list<int> getSomeIpEventGroups(const std::shared_ptr<BstIdl::FInterface> &obj)
    {
        return m_target->getIntegerArray(obj, "SomeIpEventGroups");
    }

    int getSomeIpGetterID(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpGetterID");
    }

    int getSomeIpGetterReliable(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        bool isOK = false;
        auto ret = m_target->getBoolean(obj, "SomeIpGetterReliable", isOK);
        return isOK ? ret : -1;
    }

    int getSomeIpGetterPriority(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpGetterPriority");
    }

    int getSomeIpSetterID(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpSetterID");
    }

    int getSomeIpSetterReliable(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        bool isOK = false;
        auto ret = m_target->getBoolean(obj, "SomeIpSetterReliable", isOK);
        return isOK ? ret : -1;
    }

    int getSomeIpSetterPriority(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpSetterPriority");
    }

    int getSomeIpNotifierID(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpNotifierID");
    }

    int getSomeIpNotifierReliable(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        bool isOK = false;
        auto ret = m_target->getBoolean(obj, "SomeIpNotifierReliable", isOK);
        return isOK ? ret : -1;
    }

    int getSomeIpNotifierPriority(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpNotifierPriority");
    }

    int getSomeIpNotifierMulticast(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        bool isOK = false;
        auto ret = m_target->getBoolean(obj, "SomeIpNotifierMulticast", isOK);
        return isOK ? ret : -1;
    }

    std::list<int> getSomeIpEventGroups(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getIntegerArray(obj, "SomeIpEventGroups");
    }

    enum SomeIpAttributeEndianess
    {
        a_e_null,
        a_e_le,
        a_e_be
    };
    SomeIpAttributeEndianess getSomeIpAttributeEndianess(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        std::string e = m_target->getEnum(obj, "SomeIpAttributeEndianess");
        if (e.empty())
            return a_e_null;
        return convertSomeIpAttributeEndianess(e);
    }

    int getSomeIpMethodID(const std::shared_ptr<BstIdl::FMethod> &obj)
    {
        return m_target->getInteger(obj, "SomeIpMethodID");
    }

    int getSomeIpReliable(const std::shared_ptr<BstIdl::FMethod> &obj)
    {
        bool isOK = false;
        auto ret = m_target->getBoolean(obj, "SomeIpReliable", isOK);
        return isOK ? ret : -1;
    }

    int getSomeIpPriority(const std::shared_ptr<BstIdl::FMethod> &obj)
    {
        return m_target->getInteger(obj, "SomeIpPriority");
    }

    enum SomeIpErrorCoding
    {
        e_c_null,
        Header
    };
    SomeIpErrorCoding getSomeIpErrorCoding(const std::shared_ptr<BstIdl::FMethod> &obj)
    {
        std::string e = m_target->getEnum(obj, "SomeIpErrorCoding");
        if (e.empty())
            return e_c_null;
        return convertSomeIpErrorCoding(e);
    }
    SomeIpErrorCoding convertSomeIpErrorCoding(std::string val)
    {
        if (val == "Header")
            return Header;
        return e_c_null;
    }

    enum SomeIpMethodEndianess
    {
        m_e_null,
        m_e_le,
        m_e_be
    };

    SomeIpMethodEndianess getSomeIpMethodEndianess(const std::shared_ptr<BstIdl::FMethod> &obj)
    {
        std::string e = m_target->getEnum(obj, "SomeIpMethodEndianess");
        if (e.empty())
            return SomeIpMethodEndianess::m_e_null;
        return convertSomeIpMethodEndianess(e);
    }

    SomeIpMethodEndianess convertSomeIpMethodEndianess(std::string val)
    {
        if (val == "le")
            return SomeIpMethodEndianess::m_e_le;
        else if (val == "be")
            return SomeIpMethodEndianess::m_e_be;
        return SomeIpMethodEndianess::m_e_null;
    }

    int getSomeIpEventID(const std::shared_ptr<BstIdl::FBroadcast> &obj)
    {
        return m_target->getInteger(obj, "SomeIpEventID");
    }

    int getSomeIpReliable(const std::shared_ptr<BstIdl::FBroadcast> &obj)
    {
        bool isOK = false;
        auto ret = m_target->getBoolean(obj, "SomeIpReliable", isOK);
        return isOK ? ret : -1;
    }

    int getSomeIpPriority(const std::shared_ptr<BstIdl::FBroadcast> &obj)
    {
        return m_target->getInteger(obj, "SomeIpPriority");
    }

    int getSomeIpMulticast(const std::shared_ptr<BstIdl::FBroadcast> &obj)
    {
        bool isOK = false;
        auto ret = m_target->getBoolean(obj, "SomeIpMulticast", isOK);
        return isOK ? ret : -1;
    }

    std::list<int> getSomeIpEventGroups(const std::shared_ptr<BstIdl::FBroadcast> &obj)
    {
        return m_target->getIntegerArray(obj, "SomeIpEventGroups");
    }

    enum SomeIpBroadcastEndianess
    {
        b_e_null,
        b_e_le,
        b_e_be
    };
    SomeIpBroadcastEndianess getSomeIpBroadcastEndianess(const std::shared_ptr<BstIdl::FBroadcast> &obj)
    {
        std::string e = m_target->getEnum(obj, "SomeIpBroadcastEndianess");
        if (e.empty())
            return SomeIpBroadcastEndianess::b_e_null;
        return convertSomeIpBroadcastEndianess(e);
    }

    SomeIpBroadcastEndianess convertSomeIpBroadcastEndianess(std::string val)
    {
        if (val == "le")
            return SomeIpBroadcastEndianess::b_e_le;
        else if (val == "be")
            return SomeIpBroadcastEndianess::b_e_be;
        return SomeIpBroadcastEndianess::b_e_null;
    }

    int getSomeIpArrayMinLength(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return m_target->getInteger(obj, "SomeIpArrayMinLength");
    }

    int getSomeIpArrayMaxLength(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return m_target->getInteger(obj, "SomeIpArrayMaxLength");
    }

    int getSomeIpArrayLengthWidth(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return m_target->getInteger(obj, "SomeIpArrayLengthWidth");
    }

    int getSomeIpUnionLengthWidth(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return m_target->getInteger(obj, "SomeIpUnionLengthWidth");
    }

    int getSomeIpUnionTypeWidth(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return m_target->getInteger(obj, "SomeIpUnionTypeWidth");
    }

    int getSomeIpUnionDefaultOrder(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        bool isOK = false;
        auto ret = m_target->getBoolean(obj, "SomeIpUnionDefaultOrder", isOK);
        return isOK ? ret : -1;
    }

    int getSomeIpUnionMaxLength(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return m_target->getInteger(obj, "SomeIpUnionMaxLength");
    }

    int getSomeIpStructLengthWidth(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return m_target->getInteger(obj, "SomeIpStructLengthWidth");
    }

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

    int getSomeIpStringLength(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return m_target->getInteger(obj, "SomeIpStringLength");
    }

    int getSomeIpStringLengthWidth(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return m_target->getInteger(obj, "SomeIpStringLengthWidth");
    }

    enum SomeIpStringEncoding
    {
        null,
        utf8,
        utf16le,
        utf16be
    };
    SomeIpStringEncoding getSomeIpStringEncoding(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        std::string e = m_target->getEnum(obj, "SomeIpStringEncoding");
        if (e.empty())
            return SomeIpStringEncoding::null;
        return convertSomeIpStringEncoding(e);
    }

    SomeIpStringEncoding convertSomeIpStringEncoding(std::string val)
    {
        if (val == "utf8")
            return SomeIpStringEncoding::utf8;
        else if (val == "utf16le")
            return SomeIpStringEncoding::utf16le;
        else if (val == "utf16be")
            return SomeIpStringEncoding::utf16be;
        return SomeIpStringEncoding::null;
    }

    int getSomeIpByteBufferMaxLength(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return m_target->getInteger(obj, "SomeIpByteBufferMaxLength");
    }

    int getSomeIpByteBufferMinLength(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return m_target->getInteger(obj, "SomeIpByteBufferMinLength");
    }

    int getSomeIpAttrArrayMinLength(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpAttrArrayMinLength");
    }

    int getSomeIpAttrArrayMaxLength(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpAttrArrayMaxLength");
    }

    int getSomeIpAttrArrayLengthWidth(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpAttrArrayLengthWidth");
    }

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

    int getSomeIpAttrUnionLengthWidth(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpAttrUnionLengthWidth");
    }

    int getSomeIpAttrUnionTypeWidth(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpAttrUnionTypeWidth");
    }

    int getSomeIpAttrUnionDefaultOrder(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        bool isOK = false;
        auto ret = m_target->getBoolean(obj, "SomeIpAttrUnionDefaultOrder", isOK);
        return isOK ? ret : -1;
    }

    int getSomeIpAttrUnionMaxLength(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpAttrUnionMaxLength");
    }

    int getSomeIpAttrStructLengthWidth(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpAttrStructLengthWidth");
    }

    int getSomeIpAttrEnumWidth(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpAttrEnumWidth");
    }

    int getSomeIpAttrEnumBitWidth(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpAttrEnumBitWidth");
    }

    int getSomeIpAttrIntegerBitWidth(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "SomeIpAttrIntegerBitWidth");
    }

    int getSomeIpArgArrayMinLength(const std::shared_ptr<BstIdl::FArgument> &obj)
    {
        return m_target->getInteger(obj, "SomeIpArgArrayMinLength");
    }

    int getSomeIpArgArrayMaxLength(const std::shared_ptr<BstIdl::FArgument> &obj)
    {
        return m_target->getInteger(obj, "SomeIpArgArrayMaxLength");
    }

    int getSomeIpArgArrayLengthWidth(const std::shared_ptr<BstIdl::FArgument> &obj)
    {
        return m_target->getInteger(obj, "SomeIpArgArrayLengthWidth");
    }

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

    int getSomeIpArgUnionLengthWidth(const std::shared_ptr<BstIdl::FArgument> &obj)
    {
        return m_target->getInteger(obj, "SomeIpArgUnionLengthWidth");
    }

    int getSomeIpArgUnionTypeWidth(const std::shared_ptr<BstIdl::FArgument> &obj)
    {
        return m_target->getInteger(obj, "SomeIpArgUnionTypeWidth");
    }

    int getSomeIpArgUnionDefaultOrder(const std::shared_ptr<BstIdl::FArgument> &obj)
    {
        bool isOK = false;
        auto ret = m_target->getBoolean(obj, "SomeIpArgUnionDefaultOrder", isOK);
        return isOK ? ret : -1;
    }

    int getSomeIpArgUnionMaxLength(const std::shared_ptr<BstIdl::FArgument> &obj)
    {
        return m_target->getInteger(obj, "SomeIpArgUnionMaxLength");
    }

    int getSomeIpArgStructLengthWidth(const std::shared_ptr<BstIdl::FArgument> &obj)
    {
        return m_target->getInteger(obj, "SomeIpArgStructLengthWidth");
    }

    int getSomeIpArgEnumWidth(const std::shared_ptr<BstIdl::FArgument> &obj)
    {
        return m_target->getInteger(obj, "SomeIpArgEnumWidth");
    }

    int getSomeIpArgEnumBitWidth(const std::shared_ptr<BstIdl::FArgument> &obj)
    {
        return m_target->getInteger(obj, "SomeIpArgEnumBitWidth");
    }

    int getSomeIpArgEnumInvalidValue(const std::shared_ptr<BstIdl::FArgument> &obj)
    {
        return m_target->getInteger(obj, "SomeIpArgEnumInvalidValue");
    }

    int getSomeIpArgIntegerBitWidth(const std::shared_ptr<BstIdl::FArgument> &obj)
    {
        return m_target->getInteger(obj, "SomeIpArgIntegerBitWidth");
    }

    int getSomeIpArgIntegerInvalidValue(const std::shared_ptr<BstIdl::FArgument> &obj)
    {
        return m_target->getInteger(obj, "SomeIpArgIntegerInvalidValue");
    }

    int getSomeIpStructArrayMinLength(const std::shared_ptr<BstIdl::FField> &obj)
    {
        return m_target->getInteger(obj, "SomeIpStructArrayMinLength");
    }

    int getSomeIpStructArrayMaxLength(const std::shared_ptr<BstIdl::FField> &obj)
    {
        return m_target->getInteger(obj, "SomeIpStructArrayMaxLength");
    }

    int getSomeIpStructArrayLengthWidth(const std::shared_ptr<BstIdl::FField> &obj)
    {
        return m_target->getInteger(obj, "SomeIpStructArrayLengthWidth");
    }

    int getSomeIpStructUnionLengthWidth(const std::shared_ptr<BstIdl::FField> &obj)
    {
        return m_target->getInteger(obj, "SomeIpStructUnionLengthWidth");
    }

    int getSomeIpStructUnionTypeWidth(const std::shared_ptr<BstIdl::FField> &obj)
    {
        return m_target->getInteger(obj, "SomeIpStructUnionTypeWidth");
    }

    int getSomeIpStructUnionDefaultOrder(const std::shared_ptr<BstIdl::FField> &obj)
    {
        bool isOK = false;
        auto ret = m_target->getBoolean(obj, "SomeIpStructUnionDefaultOrder", isOK);
        return isOK ? ret : -1;
    }

    int getSomeIpStructUnionMaxLength(const std::shared_ptr<BstIdl::FField> &obj)
    {
        return m_target->getInteger(obj, "SomeIpStructUnionMaxLength");
    }

    int getSomeIpStructStructLengthWidth(const std::shared_ptr<BstIdl::FField> &obj)
    {
        return m_target->getInteger(obj, "SomeIpStructStructLengthWidth");
    }

    int getSomeIpStructEnumWidth(const std::shared_ptr<BstIdl::FField> &obj)
    {
        return m_target->getInteger(obj, "SomeIpStructEnumWidth");
    }

    int getSomeIpStructEnumBitWidth(const std::shared_ptr<BstIdl::FField> &obj)
    {
        return m_target->getInteger(obj, "SomeIpStructEnumBitWidth");
    }

    int getSomeIpStructEnumInvalidValue(const std::shared_ptr<BstIdl::FField> &obj)
    {
        return m_target->getInteger(obj, "SomeIpStructEnumInvalidValue");
    }

    int getSomeIpStructIntegerBitWidth(const std::shared_ptr<BstIdl::FField> &obj)
    {
        return m_target->getInteger(obj, "SomeIpStructIntegerBitWidth");
    }

    int getSomeIpStructIntegerInvalidValue(const std::shared_ptr<BstIdl::FField> &obj)
    {
        return m_target->getInteger(obj, "SomeIpStructIntegerInvalidValue");
    }

    int getSomeIpUnionArrayMinLength(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return m_target->getInteger(obj, "SomeIpUnionArrayMinLength");
    }

    int getSomeIpUnionArrayMaxLength(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return m_target->getInteger(obj, "SomeIpUnionArrayMaxLength");
    }

    int getSomeIpUnionArrayLengthWidth(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return m_target->getInteger(obj, "SomeIpUnionArrayLengthWidth");
    }

    int getSomeIpUnionUnionLengthWidth(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return m_target->getInteger(obj, "SomeIpUnionUnionLengthWidth");
    }

    int getSomeIpUnionUnionTypeWidth(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return m_target->getInteger(obj, "SomeIpUnionUnionTypeWidth");
    }

    bool getSomeIpUnionUnionDefaultOrder(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        bool isOK = false;
        auto ret = m_target->getBoolean(obj, "SomeIpUnionUnionDefaultOrder", isOK);
        return isOK ? ret : -1;
    }

    int getSomeIpUnionUnionMaxLength(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return m_target->getInteger(obj, "SomeIpUnionUnionMaxLength");
    }

    int getSomeIpUnionStructLengthWidth(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return m_target->getInteger(obj, "SomeIpUnionStructLengthWidth");
    }

    int getSomeIpUnionEnumWidth(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return m_target->getInteger(obj, "SomeIpUnionEnumWidth");
    }

    int getSomeIpUnionEnumBitWidth(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return m_target->getInteger(obj, "SomeIpUnionEnumBitWidth");
    }

    int getSomeIpUnionIntegerBitWidth(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return m_target->getInteger(obj, "SomeIpUnionIntegerBitWidth");
    }

private:
    std::shared_ptr<BstIdl::FDeployedInterface> m_target;

    SomeIpAttributeEndianess convertSomeIpAttributeEndianess(std::string val)
    {
        if (val == "le")
            return SomeIpAttributeEndianess::a_e_le;
        else if (val == "be")
            return SomeIpAttributeEndianess::a_e_be;
        return a_e_null;
    }
};
} // namespace BstCommonAPI
#endif