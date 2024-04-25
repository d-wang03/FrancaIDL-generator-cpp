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
#ifndef SOMEIP_DATE_PROPERTY_ACCESSOR_HELPER_H
#define SOMEIP_DATE_PROPERTY_ACCESSOR_HELPER_H
#include "SomeipOverwriteAccessor.h"
#include "capicxx-core-tools/DataPropertyAccessorHelper.h"
namespace BstCommonAPI
{
class SomeipDataPropertyAccessorHelper
{
public:
    SomeipDataPropertyAccessorHelper() = delete;
    virtual ~SomeipDataPropertyAccessorHelper() = delete;
    SomeipDataPropertyAccessorHelper(const SomeipDataPropertyAccessorHelper &) = delete;
    SomeipDataPropertyAccessorHelper(SomeipDataPropertyAccessorHelper &&) noexcept = delete;
    SomeipDataPropertyAccessorHelper &operator=(const SomeipDataPropertyAccessorHelper &) = delete;
    SomeipDataPropertyAccessorHelper &operator=(SomeipDataPropertyAccessorHelper &&) = delete;

    static SomeIpDefaultErrorCoding convertSomeIpDefaultErrorCoding(std::string val)
    {
        if (val == "Header")
            return SomeIpDefaultErrorCoding::Header;
        return SomeIpDefaultErrorCoding::Invalid;
    }

    static SomeIpAttributeEndianess convertSomeIpAttributeEndianess(std::string val)
    {
        if (val == "le")
            return SomeIpAttributeEndianess::le;
        else if (val == "be")
            return SomeIpAttributeEndianess::be;
        return SomeIpAttributeEndianess::Invalid;
    }

    static SomeIpAttributeCRCWidth convertSomeIpAttributeCRCWidth(std::string val)
    {
        if (val == "zero")
            return SomeIpAttributeCRCWidth::zero;
        else if (val == "one")
            return SomeIpAttributeCRCWidth::one;
        else if (val == "four")
            return SomeIpAttributeCRCWidth::four;
        return SomeIpAttributeCRCWidth::Invalid;
    }

    static SomeIpMethodEndianess convertSomeIpMethodEndianess(std::string val)
    {
        if (val == "le")
            return SomeIpMethodEndianess::le;
        else if (val == "be")
            return SomeIpMethodEndianess::be;
        return SomeIpMethodEndianess::Invalid;
    }

    static SomeIpMethodCRCWidth convertSomeIpMethodCRCWidth(std::string val)
    {
        if (val == "zero")
            return SomeIpMethodCRCWidth::zero;
        else if (val == "one")
            return SomeIpMethodCRCWidth::one;
        else if (val == "four")
            return SomeIpMethodCRCWidth::four;
        return SomeIpMethodCRCWidth::Invalid;
    }

    static SomeIpErrorCoding convertSomeIpErrorCoding(std::string val)
    {
        if (val == "Header")
            return SomeIpErrorCoding::Header;
        return SomeIpErrorCoding::Invalid;
    }

    static SomeIpBroadcastEndianess convertSomeIpBroadcastEndianess(std::string val)
    {
        if (val == "le")
            return SomeIpBroadcastEndianess::le;
        else if (val == "be")
            return SomeIpBroadcastEndianess::be;
        return SomeIpBroadcastEndianess::Invalid;
    }

    static SomeIpBroadcastCRCWidth convertSomeIpBroadcastCRCWidth(std::string val)
    {
        if (val == "zero")
            return SomeIpBroadcastCRCWidth::zero;
        else if (val == "one")
            return SomeIpBroadcastCRCWidth::one;
        else if (val == "four")
            return SomeIpBroadcastCRCWidth::four;
        return SomeIpBroadcastCRCWidth::Invalid;
    }

    static SomeIpStringEncoding convertSomeIpStringEncoding(std::string val)
    {
        if (val == "utf8")
            return SomeIpStringEncoding::utf8;
        else if (val == "utf16le")
            return SomeIpStringEncoding::utf16le;
        else if (val == "utf16be")
            return SomeIpStringEncoding::utf16be;
        return SomeIpStringEncoding::Invalid;
    }

    static std::shared_ptr<IDataPropertyAccessor> getSomeipOverwriteAccessorAux(
        const std::shared_ptr<BstIdl::FModelElement> &obj,
        const std::shared_ptr<BstIdl::MappingGenericPropertyAccessor> &target,
        const std::shared_ptr<IDataPropertyAccessor> &owner)
    {
        if (!obj || !target || !owner)
            return nullptr;

        auto fd = std::dynamic_pointer_cast<BstIdl::FDOverwriteElement>(target->getFDElement(obj));
        if (fd)
        {
            auto overwrites = fd->getOverwrites();
            if (overwrites == nullptr)
                return owner;
            else
            {
                auto ret = std::make_shared<SomeipOverwriteAccessor>(
                    overwrites, std::dynamic_pointer_cast<SomeipIDataPropertyAccessor>(owner), target);
                return std::static_pointer_cast<IDataPropertyAccessor>(ret);
            }
        }
        else
            return nullptr;
    }
};
} // namespace BstCommonAPI
#endif