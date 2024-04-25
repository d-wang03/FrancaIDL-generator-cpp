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
#include "capicxx-core-tools/OverwriteAccessor.h"
#include "FDModel/FDCompoundOverwrites.h"
#include "FDModel/FDEnumerationOverwrites.h"
#include "capicxx-core-tools/DataPropertyAccessorHelper.h"
namespace BstCommonAPI
{
OverwriteAccessor::OverwriteAccessor(const std::shared_ptr<BstIdl::FDTypeOverwrites> &overwrites,
                                     const std::shared_ptr<IDataPropertyAccessor> &delegate,
                                     const std::shared_ptr<BstIdl::MappingGenericPropertyAccessor> &genericAccessor)
    : m_target(genericAccessor), m_delegate(delegate), m_overwrites(overwrites)
{
    if (overwrites != nullptr)
    {
        if (auto ptr = std::dynamic_pointer_cast<BstIdl::FDCompoundOverwrites>(overwrites))
        {
            // build mapping for compound fields
            for (auto f : ptr->getFields())
            {
                m_mappedFields[f->getTarget()] = f;
            }
        }
        if (auto ptr = std::dynamic_pointer_cast<BstIdl::FDEnumerationOverwrites>(overwrites))
        {
            // build mapping for enumerators
            for (auto e : ptr->getEnumerators())
            {
                m_mappedEnumerators[e->getTarget()] = e;
            }
        }
    }
}
EnumBackingType OverwriteAccessor::getEnumBackingType(const std::shared_ptr<BstIdl::FEnumerationType> &obj)
{
    if (m_overwrites)
    {
        auto e = std::static_pointer_cast<BstIdl::GenericPropertyAccessor>(m_target)->getEnum(m_overwrites,
                                                                                              "EnumBackingType");
        if (!e.empty())
        {
            return DataPropertyAccessorHelper::convertEnumBackingType(e);
        }
    }
    return m_delegate->getEnumBackingType(obj);
}

ErrorType OverwriteAccessor::getErrorType(const std::shared_ptr<BstIdl::FEnumerator> &obj)
{
    if (m_mappedEnumerators.count(obj))
    {
        auto fo = m_mappedEnumerators[obj];
        auto e = std::static_pointer_cast<BstIdl::GenericPropertyAccessor>(m_target)->getEnum(fo, "ErrorType");
        if (!e.empty())
            return DataPropertyAccessorHelper::convertErrorType(e);
    }
    return m_delegate->getErrorType(obj);
}
} // namespace BstCommonAPI