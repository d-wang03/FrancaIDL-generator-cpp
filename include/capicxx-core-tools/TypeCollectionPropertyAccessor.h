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
#ifndef TYPE_COLLECTION_PROPERTY_ACCESSOR_H
#define TYPE_COLLECTION_PROPERTY_ACCESSOR_H
#include "DataPropertyAccessorHelper.h"
#include "IDataPropertyAccessor.h"
#include <memory>
namespace BstCommonAPI
{
class TypeCollectionPropertyAccessor : public IDataPropertyAccessor,
                                       public std::enable_shared_from_this<TypeCollectionPropertyAccessor>
{
public:
    TypeCollectionPropertyAccessor() = default;
    virtual ~TypeCollectionPropertyAccessor() = default;
    TypeCollectionPropertyAccessor(const TypeCollectionPropertyAccessor &) = default;
    TypeCollectionPropertyAccessor(TypeCollectionPropertyAccessor &&) noexcept = default;
    TypeCollectionPropertyAccessor &operator=(const TypeCollectionPropertyAccessor &) = default;
    TypeCollectionPropertyAccessor &operator=(TypeCollectionPropertyAccessor &&rhs)
    {
        m_target = std::move(rhs.m_target);
        return *this;
    }

    TypeCollectionPropertyAccessor(const std::shared_ptr<BstIdl::MappingGenericPropertyAccessor> &target)
        : TypeCollectionPropertyAccessor()
    {
        m_target = target;
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