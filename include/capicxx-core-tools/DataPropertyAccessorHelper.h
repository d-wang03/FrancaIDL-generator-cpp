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
#ifndef DATA_PROPERTY_ACCESSOR_HELPER_H
#define DATA_PROPERTY_ACCESSOR_HELPER_H

#include "FDModel/MappingGenericPropertyAccessor.h"
#include "capicxx-core-tools/OverwriteAccessor.h"
#include "model/FModelElement.h"
namespace BstCommonAPI
{
class DataPropertyAccessorHelper
{
public:
    DataPropertyAccessorHelper() = delete;
    virtual ~DataPropertyAccessorHelper() = delete;
    DataPropertyAccessorHelper(const DataPropertyAccessorHelper &) = delete;
    DataPropertyAccessorHelper(DataPropertyAccessorHelper &&) noexcept = delete;
    DataPropertyAccessorHelper &operator=(const DataPropertyAccessorHelper &) = delete;
    DataPropertyAccessorHelper &operator=(DataPropertyAccessorHelper &&) = delete;

    static AttributeNotifierUpdateMode convertAttributeNotifierUpdateMode(std::string val)
    {
        if (val == "Cyclic")
            return AttributeNotifierUpdateMode::Cyclic;
        else if (val == "OnChange")
            return AttributeNotifierUpdateMode::OnChange;
        return AttributeNotifierUpdateMode::Invalid;
    }

    static EnumBackingType convertEnumBackingType(std::string val)
    {
        if (val == "UInt8")
            return EnumBackingType::UInt8;
        else if (val == "UInt16")
            return EnumBackingType::UInt16;
        else if (val == "UInt32")
            return EnumBackingType::UInt32;
        else if (val == "UInt64")
            return EnumBackingType::UInt64;
        else if (val == "Int8")
            return EnumBackingType::Int8;
        else if (val == "Int16")
            return EnumBackingType::Int16;
        else if (val == "Int32")
            return EnumBackingType::Int32;
        else if (val == "Int64")
            return EnumBackingType::Int64;
        return EnumBackingType::Default;
    }

    static ErrorType convertErrorType(std::string val)
    {
        if (val == "Error")
            return ErrorType::Error;
        else if (val == "Warning")
            return ErrorType::Warning;
        else if (val == "Info")
            return ErrorType::Info;
        else if (val == "NoError")
            return ErrorType::NoError;
        return ErrorType::Invalid;
    }

    static BroadcastType convertBroadcastType(std::string val)
    {
        if (val == "signal")
            return BroadcastType::signal;
        else if (val == "error")
            return BroadcastType::error;
        return BroadcastType::Invalid;
    }

    static std::shared_ptr<IDataPropertyAccessor> getOverwriteAccessorAux(
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
                return std::make_shared<OverwriteAccessor>(overwrites, owner, target);
        }
        else
            return nullptr;
    }
};
} // namespace BstCommonAPI
#endif