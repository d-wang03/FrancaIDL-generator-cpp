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
#ifndef INTERFACE_PROPERTY_ACCESSOR_H
#define INTERFACE_PROPERTY_ACCESSOR_H
#include "DataPropertyAccessorHelper.h"
#include "IDataPropertyAccessor.h"
#include <memory>
namespace BstCommonAPI
{
class InterfacePropertyAccessor : public IDataPropertyAccessor,
                                  public std::enable_shared_from_this<InterfacePropertyAccessor>
{
public:
    InterfacePropertyAccessor() = default;
    virtual ~InterfacePropertyAccessor() = default;
    InterfacePropertyAccessor(const InterfacePropertyAccessor &) = default;
    InterfacePropertyAccessor(InterfacePropertyAccessor &&) noexcept = default;
    InterfacePropertyAccessor &operator=(const InterfacePropertyAccessor &) = default;
    InterfacePropertyAccessor(const std::shared_ptr<BstIdl::MappingGenericPropertyAccessor> &target)
        : InterfacePropertyAccessor()
    {
        m_target = target;
    }

    // host 'interfaces'
    int getDefaultTimeout(const std::shared_ptr<BstIdl::FInterface> &obj)
    {
        return m_target->getInteger(obj, "DefaultTimeout");
    }

    // host 'methods'
    int getMethodTimeout(const std::shared_ptr<BstIdl::FMethod> &obj)
    {
        return m_target->getInteger(obj, "MethodTimeout");
    }
    std::list<std::string> getErrors(const std::shared_ptr<BstIdl::FMethod> &obj)
    {
        return m_target->getStringArray(obj, "Errors");
    }

    // host 'attributes'
    int getAttributeSetterTimeout(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "AttributeSetterTimeout");
    }
    int getAttributeGetterTimeout(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "AttributeGetterTimeout");
    }
    int getAttributeNotifierTimeout(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        return m_target->getInteger(obj, "AttributeNotifierTimeout");
    }
    AttributeNotifierUpdateMode getAttributeNotifierUpdateMode(const std::shared_ptr<BstIdl::FAttribute> &obj)
    {
        std::string e = m_target->getEnum(obj, "AttributeNotifierUpdateMode");
        if (e.empty())
            return AttributeNotifierUpdateMode::Invalid;
        return DataPropertyAccessorHelper::convertAttributeNotifierUpdateMode(e);
    }

    // host 'enumerations'
    EnumBackingType getEnumBackingType(const std::shared_ptr<BstIdl::FEnumerationType> &obj) override
    {
        std::string e = m_target->getEnum(obj, "EnumBackingType");
        if (e.empty())
            return EnumBackingType::Default;
        return DataPropertyAccessorHelper::convertEnumBackingType(e);
    }

    // host 'enumerators'
    ErrorType getErrorType(const std::shared_ptr<BstIdl::FEnumerator> &obj) override
    {
        std::string e = m_target->getEnum(obj, "ErrorType");
        if (e.empty())
            return ErrorType::Invalid;
        return DataPropertyAccessorHelper::convertErrorType(e);
    }

    // host 'broadcasts'
    BroadcastType getBroadcastType(const std::shared_ptr<BstIdl::FBroadcast> &obj)
    {
        std::string e = m_target->getEnum(obj, "BroadcastType");
        if (e.empty())
            return BroadcastType::Invalid;

        return DataPropertyAccessorHelper::convertBroadcastType(e);
    }
    std::string getErrorName(const std::shared_ptr<BstIdl::FBroadcast> &obj)
    {
        return m_target->getString(obj, "ErrorName");
    }

    /**
     * Get an overwrite-aware accessor for deployment properties.</p>
     *
     * This accessor will return overwritten property values in the context
     * of a Franca FAttribute object. I.e., the const std::shared_ptr<BstIdl::FAttribute> &obj has a datatype
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
        return DataPropertyAccessorHelper::getOverwriteAccessorAux(obj, m_target, shared_from_this());
    }

    /**
     * Get an overwrite-aware accessor for deployment properties.</p>
     *
     * This accessor will return overwritten property values in the context
     * of a Franca FArgument object. I.e., the const std::shared_ptr<BstIdl::FArgument> &obj has a datatype
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
        return DataPropertyAccessorHelper::getOverwriteAccessorAux(obj, m_target, shared_from_this());
    }

    std::shared_ptr<IDataPropertyAccessor> getOverwriteAccessor(const std::shared_ptr<BstIdl::FField> &obj) override
    {
        return DataPropertyAccessorHelper::getOverwriteAccessorAux(obj, m_target, shared_from_this());
    }

    std::shared_ptr<IDataPropertyAccessor> getOverwriteAccessor(const std::shared_ptr<BstIdl::FArrayType> &obj) override
    {
        return DataPropertyAccessorHelper::getOverwriteAccessorAux(obj, m_target, shared_from_this());
    }

protected:
    std::shared_ptr<BstIdl::MappingGenericPropertyAccessor> m_target;
};
} // namespace BstCommonAPI
#endif