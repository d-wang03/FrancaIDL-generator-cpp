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
#ifndef SOMEIP_PROPERTY_ACCESSOR_H
#define SOMEIP_PROPERTY_ACCESSOR_H
#include "DeploymentInterfacePropertyAccessor.h"
#include "DeploymentTypeCollectionPropertyAccessor.h"
#include "FDModel/FDeployedProvider.h"
#include "SomeipIDataPropertyAccessor.h"
#include "SomeipInterfacePropertyAccessor.h"
#include "SomeipProviderPropertyAccessor.h"
#include "SomeipTypeCollectionPropertyAccessor.h"
#include "VSomeipPropertyAccessor.h"
#include "capicxx-core-tools/CommonapiPropertyAccessor.h"
#include <functional>

#define INTERFACE_OVERWRITE_CODE(fun)                                                                                  \
    int default_value = -1;                                                                                            \
    if (type_ == DeploymentType::INTERFACE)                                                                            \
    {                                                                                                                  \
        auto ipa = std::dynamic_pointer_cast<SomeipInterfacePropertyAccessor>(someipDataAccessor_);                    \
        return ipa ? ipa->get##fun(obj) : default_value;                                                               \
    }                                                                                                                  \
    if (type_ == DeploymentType::OVERWRITE)                                                                            \
    {                                                                                                                  \
        return parent_ ? parent_->get##fun(obj) : default_value;                                                       \
    }                                                                                                                  \
    return default_value;

namespace BstCommonAPI
{
class SomeipPropertyAccessor : public CommonapiPropertyAccessor,
                               public std::enable_shared_from_this<SomeipPropertyAccessor>
{
private:
    std::shared_ptr<SomeipIDataPropertyAccessor> someipDataAccessor_;
    std::shared_ptr<SomeipProviderPropertyAccessor> someipProvider_;
    std::shared_ptr<SomeipPropertyAccessor> parent_;
    std::string name_;

    void setName(const std::shared_ptr<BstIdl::FField> &_element)
    {
        std::string containername = "";
        if (_element->getContainer())
            containername = _element->getContainer()->getName() + "_";
        std::string parentname = parent_->name_;
        name_ = parentname + containername + _element->getName() + "_";
    }

    void setName(const std::shared_ptr<BstIdl::FArgument> &_element)
    {
        if (_element->getContainer())
            name_ = _element->getContainer()->getName() + "_" + _element->getName() + "_";
    }

    void setName(const std::shared_ptr<BstIdl::FAttribute> &_element)
    {
        name_ = _element->getName() + "_";
    }

    void setName(const std::shared_ptr<BstIdl::FArrayType> &_element)
    {
        if (someipDataAccessor_ != parent_->someipDataAccessor_)
        {
            std::string parentname = parent_->getName();
            name_ = parentname + _element->getName() + "_";
        }
        else
        {
            name_ = parent_->getName();
        }
        return;
    }

    template <typename T1, typename T2, typename T3> auto interface_method(T1 &&func_ptr, T2 &&obj, T3 default_value)
    {
        if (type_ == DeploymentType::INTERFACE)
        {
            auto ipa = std::dynamic_pointer_cast<SomeipInterfacePropertyAccessor>(someipDataAccessor_);
            return ipa ? (ipa.get()->*func_ptr)(obj) : default_value;
        }
        return default_value;
    }

    template <typename T1, typename T2, typename T3> auto accessor_method(T1 &&func_ptr, T2 &&obj, T3 default_value)
    {
        return someipDataAccessor_ ? (someipDataAccessor_.get()->*func_ptr)(obj) : default_value;
    }

    template <typename T1, typename T2, typename T3> auto provider_method(T1 &&func_ptr, T2 &&obj, T3 default_value)
    {
        if (type_ == DeploymentType::PROVIDER)
            return someipProvider_ ? (someipProvider_.get()->*func_ptr)(obj) : default_value;
        return default_value;
    }

public:
    SomeipPropertyAccessor() = default;
    virtual ~SomeipPropertyAccessor() = default;
    SomeipPropertyAccessor(const SomeipPropertyAccessor &) = default;
    SomeipPropertyAccessor(SomeipPropertyAccessor &&) noexcept = default;
    SomeipPropertyAccessor &operator=(const SomeipPropertyAccessor &) = default;
    SomeipPropertyAccessor &operator=(SomeipPropertyAccessor &&) = default;

    SomeipPropertyAccessor(const std::shared_ptr<BstIdl::FDeployedInterface> &_target)
        : CommonapiPropertyAccessor(_target),
          someipDataAccessor_(std::make_shared<SomeipInterfacePropertyAccessor>(_target)), someipProvider_(nullptr),
          parent_(nullptr), name_("")
    {
    }

    SomeipPropertyAccessor(const std::shared_ptr<BstIdl::FDeployedTypeCollection> &_target)
        : CommonapiPropertyAccessor(_target),
          someipDataAccessor_(std::make_shared<SomeipTypeCollectionPropertyAccessor>(_target)),
          someipProvider_(nullptr), parent_(nullptr), name_("")
    {
    }

    SomeipPropertyAccessor(const std::shared_ptr<BstIdl::FDeployedProvider> &_target)
        : CommonapiPropertyAccessor(_target), someipDataAccessor_(nullptr),
          someipProvider_(std::make_shared<SomeipProviderPropertyAccessor>(_target)), parent_(nullptr), name_("")
    {
    }

    SomeipPropertyAccessor(const std::shared_ptr<SomeipPropertyAccessor> &_parent,
                           const std::shared_ptr<BstIdl::FField> &_element)
        : CommonapiPropertyAccessor(), someipDataAccessor_(nullptr), someipProvider_(nullptr), parent_(_parent),
          name_("")
    {
        if (_parent && _parent->type_ != DeploymentType::PROVIDER && _parent->someipDataAccessor_)
        {
            someipDataAccessor_ = std::dynamic_pointer_cast<SomeipIDataPropertyAccessor>(
                _parent->someipDataAccessor_->getOverwriteAccessor(_element));
            type_ = DeploymentType::OVERWRITE;
        }
        setName(_element);
    }

    SomeipPropertyAccessor(const std::shared_ptr<SomeipPropertyAccessor> &_parent,
                           const std::shared_ptr<BstIdl::FArrayType> &_element)
        : CommonapiPropertyAccessor(), someipDataAccessor_(nullptr), someipProvider_(nullptr), parent_(_parent),
          name_("")
    {
        if (_parent && _parent->type_ != DeploymentType::PROVIDER && _parent->someipDataAccessor_)
        {
            someipDataAccessor_ = std::dynamic_pointer_cast<SomeipIDataPropertyAccessor>(
                _parent->someipDataAccessor_->getOverwriteAccessor(_element));
            type_ = DeploymentType::OVERWRITE;
        }
        setName(_element);
    }

    SomeipPropertyAccessor(const std::shared_ptr<SomeipPropertyAccessor> &_parent,
                           const std::shared_ptr<BstIdl::FArgument> &_element)
        : CommonapiPropertyAccessor(), someipDataAccessor_(nullptr), someipProvider_(nullptr), parent_(_parent),
          name_("")
    {
        type_ = DeploymentType::OVERWRITE;
        if (_parent->type_ == DeploymentType::INTERFACE)
        {
            auto ipa = std::dynamic_pointer_cast<SomeipInterfacePropertyAccessor>(_parent->someipDataAccessor_);
            someipDataAccessor_ =
                std::dynamic_pointer_cast<SomeipIDataPropertyAccessor>(ipa->getOverwriteAccessor(_element));
        }
        else
            someipDataAccessor_ = nullptr;
        setName(_element);
    }

    SomeipPropertyAccessor(const std::shared_ptr<SomeipPropertyAccessor> &_parent,
                           const std::shared_ptr<BstIdl::FAttribute> &_element)
        : CommonapiPropertyAccessor(), someipDataAccessor_(nullptr), someipProvider_(nullptr), parent_(_parent),
          name_("")
    {
        type_ = DeploymentType::OVERWRITE;
        if (_parent->type_ == DeploymentType::INTERFACE)
        {
            auto ipa = std::dynamic_pointer_cast<SomeipInterfacePropertyAccessor>(_parent->someipDataAccessor_);
            someipDataAccessor_ =
                std::dynamic_pointer_cast<SomeipIDataPropertyAccessor>(ipa->getOverwriteAccessor(_element));
        }
        else
            someipDataAccessor_ = nullptr;
        setName(_element);
    }

    static SomeipPropertyAccessor &getInstance()
    {
        static SomeipPropertyAccessor ins;
        return ins;
    }

    std::string getName() const
    {
        return name_;
    }

    std::shared_ptr<SomeipPropertyAccessor> getParent() const
    {
        return parent_;
    }

    std::shared_ptr<SomeipPropertyAccessor> getOverwriteAccessor(const std::shared_ptr<BstIdl::FObject> &_object)
    {
        if (auto ptr = std::dynamic_pointer_cast<BstIdl::FArgument>(_object))
            return std::make_shared<SomeipPropertyAccessor>(shared_from_this(), ptr);
        if (auto ptr = std::dynamic_pointer_cast<BstIdl::FAttribute>(_object))
            return std::make_shared<SomeipPropertyAccessor>(shared_from_this(), ptr);
        if (auto ptr = std::dynamic_pointer_cast<BstIdl::FField>(_object))
            return std::make_shared<SomeipPropertyAccessor>(shared_from_this(), ptr);
        if (auto ptr = std::dynamic_pointer_cast<BstIdl::FArrayType>(_object))
            return std::make_shared<SomeipPropertyAccessor>(shared_from_this(), ptr);

        return nullptr;
    }

    bool isValidOverwirte() const
    {
        return (isProperOverwrite() && someipProvider_);
    }

    bool isProperOverwrite() const
    {
        // is proper overwrite if we are overwrite and none of my parents is the same accessor
        return (type_ == DeploymentType::OVERWRITE && someipDataAccessor_ && !hasSameAccessor(someipDataAccessor_));
    }

    bool hasSameAccessor(const std::shared_ptr<SomeipIDataPropertyAccessor> &_accessor) const
    {
        if (!parent_)
            return false;
        if (_accessor && parent_->someipDataAccessor_ == _accessor)
            return true;
        return parent_->hasSameAccessor(_accessor);
    }

    int getSomeIpServiceID(const std::shared_ptr<BstIdl::FInterface> &obj)
    {
        int ret = -1;
        return interface_method(&SomeipInterfacePropertyAccessor::getSomeIpServiceID, obj, ret);
    }
    std::list<int> getSomeIpEventGroups(const std::shared_ptr<BstIdl::FInterface> &obj)
    {
        std::list<int> ret;
        auto f = static_cast<std::list<int> (SomeipInterfacePropertyAccessor::*)(
            const std::shared_ptr<BstIdl::FInterface> &obj)>(&SomeipInterfacePropertyAccessor::getSomeIpEventGroups);
        return interface_method(f, obj, ret);
    }

    int getSomeIpGetterID(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        int ret = -1;
        return interface_method(&SomeipInterfacePropertyAccessor::getSomeIpGetterID, obj, ret);
    }
    bool getSomeIpGetterReliable(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        int ret = -1;
        ret = interface_method(&SomeipInterfacePropertyAccessor::getSomeIpAttributeReliable, obj, ret);
        return ret == 1 ? true : false;
    }
    int getSomeIpSetterID(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        int ret = -1;
        return interface_method(&SomeipInterfacePropertyAccessor::getSomeIpSetterID, obj, ret);
    }
    bool getSomeIpSetterReliable(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        int ret = -1;
        ret = interface_method(&SomeipInterfacePropertyAccessor::getSomeIpAttributeReliable, obj, ret);
        return ret == 1 ? true : false;
    }
    int getSomeIpNotifierID(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        int ret = -1;
        return interface_method(&SomeipInterfacePropertyAccessor::getSomeIpNotifierID, obj, ret);
    }
    bool getSomeIpNotifierReliable(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        int ret = -1;
        ret = interface_method(&SomeipInterfacePropertyAccessor::getSomeIpAttributeReliable, obj, ret);
        return ret == 1 ? true : false;
    }
    std::list<int> getSomeIpEventGroups(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        std::list<int> ret;
        if (type_ == DeploymentType::INTERFACE)
        {
            auto ipa = std::dynamic_pointer_cast<SomeipInterfacePropertyAccessor>(someipDataAccessor_);
            if (ipa)
            {
                ret = ipa->getSomeIpNotifierEventGroups(obj);
                if (ret.empty())
                    ret = ipa->getSomeIpEventGroups(obj);
            }
        }
        return ret;
    }
    std::string getSomeIpEndianess(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        SomeIpAttributeEndianess defaultValue = SomeIpAttributeEndianess::Invalid;
        return interface_method(&SomeipInterfacePropertyAccessor::getSomeIpAttributeEndianess, obj, defaultValue) ==
                       SomeIpAttributeEndianess::le
                   ? "true"
                   : "false";
    }

    int getSomeIpMethodID(const std::shared_ptr<BstIdl::FMethod> &obj)
    {
        int ret = -1;
        return interface_method(&SomeipInterfacePropertyAccessor::getSomeIpMethodID, obj, ret);
    }
    bool getSomeIpReliable(const std::shared_ptr<BstIdl::FMethod> &obj)
    {
        bool ret = false;
        auto f = static_cast<bool (SomeipInterfacePropertyAccessor::*)(const std::shared_ptr<BstIdl::FMethod> &obj)>(
            &SomeipInterfacePropertyAccessor::getSomeIpReliable);
        return interface_method(f, obj, ret);
    }
    std::string getSomeIpEndianess(const std::shared_ptr<BstIdl::FMethod> &obj)
    {
        SomeIpMethodEndianess defaultValue = SomeIpMethodEndianess::Invalid;
        return interface_method(&SomeipInterfacePropertyAccessor::getSomeIpMethodEndianess, obj, defaultValue) ==
                       SomeIpMethodEndianess::le
                   ? "true"
                   : "false";
    }
    int getSomeIpEventID(const std::shared_ptr<BstIdl::FBroadcast> &obj)
    {
        int ret = -1;
        return interface_method(&SomeipInterfacePropertyAccessor::getSomeIpEventID, obj, ret);
    }
    bool getSomeIpReliable(const std::shared_ptr<BstIdl::FBroadcast> &obj)
    {
        bool ret = false;
        auto f = static_cast<bool (SomeipInterfacePropertyAccessor::*)(const std::shared_ptr<BstIdl::FBroadcast> &obj)>(
            &SomeipInterfacePropertyAccessor::getSomeIpReliable);
        return interface_method(f, obj, ret);
    }
    std::list<int> getSomeIpEventGroups(const std::shared_ptr<BstIdl::FBroadcast> &obj)
    {
        std::list<int> ret;
        auto f = static_cast<std::list<int> (SomeipInterfacePropertyAccessor::*)(
            const std::shared_ptr<BstIdl::FBroadcast> &obj)>(&SomeipInterfacePropertyAccessor::getSomeIpEventGroups);
        return interface_method(f, obj, ret);
    }

    std::string getSomeIpEndianess(const std::shared_ptr<BstIdl::FBroadcast> &obj)
    {
        auto defaultValue = SomeIpBroadcastEndianess::Invalid;
        return interface_method(&SomeipInterfacePropertyAccessor::getSomeIpBroadcastEndianess, obj, defaultValue) ==
                       SomeIpBroadcastEndianess::le
                   ? "true"
                   : "false";
    }

    EnumBackingType getEnumBackingType(const std::shared_ptr<BstIdl::FEnumerationType> &obj)
    {
        if (!obj || !parent_)
            return EnumBackingType::UInt8;
        switch (type_)
        {
        case DeploymentType::OVERWRITE:
            return parent_->getEnumBackingType(obj);
        default:
            return CommonapiPropertyAccessor::getEnumBackingType(obj);
        }
        return EnumBackingType::UInt8;
    }

    int getSomeIpArrayMinLength(const std::shared_ptr<BstIdl::FArrayType> &obj)
    {
        return someipDataAccessor_ ? someipDataAccessor_->getSomeIpArrayMinLength(obj) : -1;
    }

    int getSomeIpArrayMinLength(const std::shared_ptr<BstIdl::FField> &obj)
    {
        return someipDataAccessor_ ? someipDataAccessor_->getSomeIpArrayMinLength(obj) : -1;
    }

    int getSomeIpArrayMinLength(const std::shared_ptr<BstIdl::FArgument> &obj)
    {
        INTERFACE_OVERWRITE_CODE(SomeIpArrayMinLength)
    }

    int getSomeIpArrayMinLength(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        INTERFACE_OVERWRITE_CODE(SomeIpArrayMinLength)
    }

    int getSomeIpArrayMaxLength(const std::shared_ptr<BstIdl::FArrayType> &obj)
    {
        return someipDataAccessor_ ? someipDataAccessor_->getSomeIpArrayMaxLength(obj) : -1;
    }

    int getSomeIpArrayMaxLength(const std::shared_ptr<BstIdl::FField> &obj)
    {
        return someipDataAccessor_ ? someipDataAccessor_->getSomeIpArrayMaxLength(obj) : -1;
    }

    int getSomeIpArrayMaxLength(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        INTERFACE_OVERWRITE_CODE(SomeIpArrayMaxLength)
    }

    int getSomeIpArrayMaxLength(const std::shared_ptr<BstIdl::FArgument> &obj)
    {
        INTERFACE_OVERWRITE_CODE(SomeIpArrayMaxLength)
    }

    int getSomeIpArrayLengthWidth(const std::shared_ptr<BstIdl::FArrayType> &obj)
    {
        return someipDataAccessor_ ? someipDataAccessor_->getSomeIpArrayLengthWidth(obj) : -1;
    }

    int getSomeIpArrayLengthWidth(const std::shared_ptr<BstIdl::FField> &obj)
    {
        return someipDataAccessor_ ? someipDataAccessor_->getSomeIpArrayLengthWidth(obj) : -1;
    }

    int getSomeIpArrayLengthWidth(const std::shared_ptr<BstIdl::FArgument> &obj)
    {
        INTERFACE_OVERWRITE_CODE(SomeIpArrayLengthWidth)
    }

    int getSomeIpArrayLengthWidth(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        INTERFACE_OVERWRITE_CODE(SomeIpArrayLengthWidth)
    }

    int getSomeIpUnionLengthWidth(const std::shared_ptr<BstIdl::FUnionType> &obj)
    {
        return someipDataAccessor_ ? someipDataAccessor_->getSomeIpUnionLengthWidth(obj) : -1;
    }

    int getSomeIpUnionTypeWidth(const std::shared_ptr<BstIdl::FUnionType> &obj)
    {
        return someipDataAccessor_ ? someipDataAccessor_->getSomeIpUnionTypeWidth(obj) : -1;
    }

    int getSomeIpUnionDefaultOrder(const std::shared_ptr<BstIdl::FUnionType> &obj)
    {
        return someipDataAccessor_ ? someipDataAccessor_->getSomeIpUnionDefaultOrder(obj) : -1;
    }

    int getSomeIpUnionMaxLength(const std::shared_ptr<BstIdl::FUnionType> &obj)
    {
        return someipDataAccessor_ ? someipDataAccessor_->getSomeIpUnionMaxLength(obj) : -1;
    }

    int getSomeIpStructLengthWidth(const std::shared_ptr<BstIdl::FStructType> &obj)
    {
        return someipDataAccessor_ ? someipDataAccessor_->getSomeIpStructLengthWidth(obj) : -1;
    }

    int getSomeIpEnumWidth(const std::shared_ptr<BstIdl::FEnumerationType> &obj)
    {
        return someipDataAccessor_ ? someipDataAccessor_->getSomeIpEnumWidth(obj) : -1;
    }

    int getSomeIpEnumBitWidth(const std::shared_ptr<BstIdl::FEnumerationType> &obj)
    {
        return someipDataAccessor_ ? someipDataAccessor_->getSomeIpEnumBitWidth(obj) : -1;
    }

    int getSomeIpEnumInvalidValue(const std::shared_ptr<BstIdl::FEnumerationType> &obj)
    {
        return someipDataAccessor_ ? someipDataAccessor_->getSomeIpEnumInvalidValue(obj) : -1;
    }

    int getSomeIpStringLength(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return someipDataAccessor_ ? someipDataAccessor_->getSomeIpStringLength(obj) : -1;
    }

    int getSomeIpByteBufferMaxLength(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return someipDataAccessor_ ? someipDataAccessor_->getSomeIpByteBufferMaxLength(obj) : -1;
    }

    int getSomeIpByteBufferMinLength(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return someipDataAccessor_ ? someipDataAccessor_->getSomeIpByteBufferMinLength(obj) : -1;
    }

    int getSomeIpByteBufferLengthWidth(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return someipDataAccessor_ ? someipDataAccessor_->getSomeIpByteBufferLengthWidth(obj) : -1;
    }

    int getSomeIpStringLengthWidth(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return someipDataAccessor_ ? someipDataAccessor_->getSomeIpStringLengthWidth(obj) : -1;
    }

    SomeIpStringEncoding getSomeIpStringEncoding(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return someipDataAccessor_ ? someipDataAccessor_->getSomeIpStringEncoding(obj) : SomeIpStringEncoding::Invalid;
    }

    int getSomeIpIntegerBitWidth(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return someipDataAccessor_ ? someipDataAccessor_->getSomeIpIntegerBitWidth(obj) : -1;
    }

    int getSomeIpIntegerInvalidValue(const std::shared_ptr<BstIdl::FObject> &obj)
    {
        return someipDataAccessor_ ? someipDataAccessor_->getSomeIpIntegerInvalidValue(obj) : -1;
    }

    int getSomeIpArgMapMinLength(const std::shared_ptr<BstIdl::FArgument> &obj)
    {
        INTERFACE_OVERWRITE_CODE(SomeIpArgMapMinLength)
    }
    int getSomeIpArgMapMaxLength(const std::shared_ptr<BstIdl::FArgument> &obj)
    {
        INTERFACE_OVERWRITE_CODE(SomeIpArgMapMaxLength)
    }
    int getSomeIpArgMapLengthWidth(const std::shared_ptr<BstIdl::FArgument> &obj)
    {
        INTERFACE_OVERWRITE_CODE(SomeIpArgMapLengthWidth)
    }
    int getSomeIpAttrMapMinLength(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        INTERFACE_OVERWRITE_CODE(SomeIpAttrMapMinLength)
    }
    int getSomeIpAttrMapMaxLength(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        INTERFACE_OVERWRITE_CODE(SomeIpAttrMapMaxLength)
    }
    int getSomeIpAttrMapLengthWidth(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        INTERFACE_OVERWRITE_CODE(SomeIpAttrMapLengthWidth)
    }
    int getSomeIpInstanceID(const std::shared_ptr<BstIdl::FDExtensionElement> &obj)
    {
        int ret = -1;
        return provider_method(&SomeipProviderPropertyAccessor::getSomeIpInstanceID, obj, ret);
    }
    std::string getSomeIpUnicastAddress(const std::shared_ptr<BstIdl::FDExtensionElement> &obj)
    {
        std::string ret;
        return provider_method(&SomeipProviderPropertyAccessor::getSomeIpUnicastAddress, obj, ret);
    }
    int getSomeIpReliableUnicastPort(const std::shared_ptr<BstIdl::FDExtensionElement> &obj)
    {
        int ret = -1;
        return provider_method(&SomeipProviderPropertyAccessor::getSomeIpReliableUnicastPort, obj, ret);
    }
    int getSomeIpUnreliableUnicastPort(const std::shared_ptr<BstIdl::FDExtensionElement> &obj)
    {
        int ret = -1;
        return provider_method(&SomeipProviderPropertyAccessor::getSomeIpUnreliableUnicastPort, obj, ret);
    }
    std::list<std::string> getSomeIpMulticastAddresses(const std::shared_ptr<BstIdl::FDExtensionElement> &obj)
    {
        std::list<std::string> ret;
        return provider_method(&SomeipProviderPropertyAccessor::getSomeIpMulticastAddresses, obj, ret);
    }
    std::list<int> getSomeIpMulticastPorts(const std::shared_ptr<BstIdl::FDExtensionElement> &obj)
    {
        std::list<int> ret;
        return provider_method(&SomeipProviderPropertyAccessor::getSomeIpMulticastPorts, obj, ret);
    }

    // vsomeip
    bool getSomeIpConfigGenEnable(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj, bool &isOK)
    {
        return someipProvider_ ? someipProvider_->getSomeIpConfigGenEnable(obj, isOK) : isOK = false;
    }

    std::string getSomeIpClientHostUnicast(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        std::string ret;
        return provider_method(&SomeipProviderPropertyAccessor::getSomeIpClientHostUnicast, obj, ret);
    }
    std::string getSomeIpServerHostUnicast(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        std::string ret;
        return provider_method(&SomeipProviderPropertyAccessor::getSomeIpServerHostUnicast, obj, ret);
    }
    // logging
    std::string getSomeIpLoggingLevel(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        std::string ret;
        return provider_method(&SomeipProviderPropertyAccessor::getSomeIpLoggingLevel, obj, ret);
    }
    bool getSomeIpLoggingViaConsole(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj, bool &isOK)
    {
        return someipProvider_ ? someipProvider_->getSomeIpLoggingViaConsole(obj, isOK) : isOK = false;
    }
    bool getSomeIpCreateLogFile(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj, bool &isOK)
    {
        return someipProvider_ ? someipProvider_->getSomeIpCreateLogFile(obj, isOK) : isOK = false;
    }
    std::string getSomeIpLogFileAbsPath(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        std::string ret;
        return provider_method(&SomeipProviderPropertyAccessor::getSomeIpLogFileAbsPath, obj, ret);
    }
    bool getSomeIpLoggingDLT(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj, bool &isOK)
    {
        return someipProvider_ ? someipProvider_->getSomeIpLoggingDLT(obj, isOK) : isOK = false;
    }
    bool getSomeIpVersionCyclicLogEable(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj, bool &isOK)
    {
        return someipProvider_ ? someipProvider_->getSomeIpVersionCyclicLogEable(obj, isOK) : isOK = false;
    }
    int getSomeIpVersionLogInterval(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        int ret = -1;
        return provider_method(&SomeipProviderPropertyAccessor::getSomeIpVersionLogInterval, obj, ret);
    }
    int getSomeIpMemoryLogInterval(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        int ret = -1;
        return provider_method(&SomeipProviderPropertyAccessor::getSomeIpMemoryLogInterval, obj, ret);
    }
    int getSomeIpStatusLogInterval(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        int ret = -1;
        return provider_method(&SomeipProviderPropertyAccessor::getSomeIpStatusLogInterval, obj, ret);
    }
    // applications
    std::list<std::string> getSomeIpApplicationNames(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        std::list<std::string> ret;
        return provider_method(&SomeipProviderPropertyAccessor::getSomeIpApplicationNames, obj, ret);
    }
    std::list<std::string> getSomeIpApplicationIDs(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        std::list<std::string> ret;
        return provider_method(&SomeipProviderPropertyAccessor::getSomeIpApplicationIDs, obj, ret);
    }
    // service-discovery
    bool getSomeIpServiceDiscoveryEnable(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj, bool &isOK)
    {
        return someipProvider_ ? someipProvider_->getSomeIpServiceDiscoveryEnable(obj, isOK) : isOK = false;
    }
    std::string getSomeIpServiceDiscoveryMulticastAddress(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        std::string ret;
        return provider_method(&SomeipProviderPropertyAccessor::getSomeIpServiceDiscoveryMulticastAddress, obj, ret);
    }
    int getSomeIpServiceDiscoveryPort(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        int ret = -1;
        return provider_method(&SomeipProviderPropertyAccessor::getSomeIpServiceDiscoveryPort, obj, ret);
    }

    std::string getSomeIpServiceDiscoveryProtocol(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        std::string ret;
        return provider_method(&SomeipProviderPropertyAccessor::getSomeIpServiceDiscoveryProtocol, obj, ret);
    }
    int getSomeIpServiceDiscoveryInitialDelayMin(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        int ret = -1;
        return provider_method(&SomeipProviderPropertyAccessor::getSomeIpServiceDiscoveryInitialDelayMin, obj, ret);
    }
    int getSomeIpServiceDiscoveryInitialDelayMax(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        int ret = -1;
        return provider_method(&SomeipProviderPropertyAccessor::getSomeIpServiceDiscoveryInitialDelayMax, obj, ret);
    }
    int getSomeIpServiceDiscoveryRepetitionsBaseDelay(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        int ret = -1;
        return provider_method(&SomeipProviderPropertyAccessor::getSomeIpServiceDiscoveryRepetitionsBaseDelay, obj,
                               ret);
    }
    int getSomeIpServiceDiscoveryRepetitionsMax(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        int ret = -1;
        return provider_method(&SomeipProviderPropertyAccessor::getSomeIpServiceDiscoveryRepetitionsMax, obj, ret);
    }
    int getSomeIpServiceDiscoveryttl(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        int ret = -1;
        return provider_method(&SomeipProviderPropertyAccessor::getSomeIpServiceDiscoveryttl, obj, ret);
    }
    int getSomeIpServiceDiscoveryCyclicOfferDelay(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        int ret = -1;
        return provider_method(&SomeipProviderPropertyAccessor::getSomeIpServiceDiscoveryCyclicOfferDelay, obj, ret);
    }
    int getSomeIpServiceDiscoveryRequestResponseDelay(const std::shared_ptr<BstIdl::FDExtensionRoot> &obj)
    {
        int ret = -1;
        return provider_method(&SomeipProviderPropertyAccessor::getSomeIpServiceDiscoveryRequestResponseDelay, obj,
                               ret);
    }
};
} // namespace BstCommonAPI
#endif