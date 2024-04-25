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
#ifndef F_OVERWRITE_ACCESSOR_H
#define F_OVERWRITE_ACCESSOR_H
#include "FDModel/FDTypeOverwrites.h"
#include "FDModel/MappingGenericPropertyAccessor.h"
#include "capicxx-core-tools/IDataPropertyAccessor.h"
#include <map>
#include <memory>

namespace BstCommonAPI
{
class OverwriteAccessor : virtual public IDataPropertyAccessor, public std::enable_shared_from_this<OverwriteAccessor>
{
public:
    OverwriteAccessor() = default;
    virtual ~OverwriteAccessor() = default;
    OverwriteAccessor(const OverwriteAccessor &) = default;
    OverwriteAccessor(OverwriteAccessor &&) noexcept = default;
    OverwriteAccessor &operator=(const OverwriteAccessor &) = default;
    OverwriteAccessor(const std::shared_ptr<BstIdl::FDTypeOverwrites> &overwrites,
                      const std::shared_ptr<IDataPropertyAccessor> &delegate,
                      const std::shared_ptr<BstIdl::MappingGenericPropertyAccessor> &genericAccessor);

    EnumBackingType getEnumBackingType(const std::shared_ptr<BstIdl::FEnumerationType> &obj) override;

    // host 'enumerators'
    ErrorType getErrorType(const std::shared_ptr<BstIdl::FEnumerator> &obj) override;

    std::shared_ptr<IDataPropertyAccessor> getOverwriteAccessor(const std::shared_ptr<BstIdl::FField> &obj) override
    {
        // check if this field is overwritten
        if (m_mappedFields.count(obj))
        {
            auto fo = m_mappedFields[obj];
            auto overwrites = fo->getOverwrites();
            if (overwrites == nullptr)
                return shared_from_this();
            else
                return std::make_shared<OverwriteAccessor>(overwrites, shared_from_this(), m_target);
        }
        return m_delegate->getOverwriteAccessor(obj);
    }

    std::shared_ptr<IDataPropertyAccessor> getOverwriteAccessor(const std::shared_ptr<BstIdl::FArrayType> &obj) override
    {
        // check if this array is overwritten
        if (m_overwrites != nullptr)
        {
            return std::make_shared<OverwriteAccessor>(m_overwrites, shared_from_this(), m_target);
        }
        return m_delegate->getOverwriteAccessor(obj);
    }

protected:
    std::shared_ptr<BstIdl::MappingGenericPropertyAccessor> m_target;
    std::shared_ptr<IDataPropertyAccessor> m_delegate;
    std::shared_ptr<BstIdl::FDTypeOverwrites> m_overwrites;
    std::map<std::shared_ptr<BstIdl::FField>, std::shared_ptr<BstIdl::FDField>> m_mappedFields;
    std::map<std::shared_ptr<BstIdl::FEnumerator>, std::shared_ptr<BstIdl::FDEnumValue>> m_mappedEnumerators;
};
} // namespace BstCommonAPI
#endif