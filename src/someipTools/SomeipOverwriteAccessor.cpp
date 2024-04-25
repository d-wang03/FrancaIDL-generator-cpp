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
#include "someipTools/SomeipOverwriteAccessor.h"
#include "someipTools/SomeipDataPropertyAccessorHelper.h"
namespace BstCommonAPI
{
SomeipOverwriteAccessor::SomeipOverwriteAccessor(
    const std::shared_ptr<BstIdl::FDTypeOverwrites> &overwrites,
    const std::shared_ptr<SomeipIDataPropertyAccessor> &delegate,
    const std::shared_ptr<BstIdl::MappingGenericPropertyAccessor> &genericAccessor)
    : OverwriteAccessor(overwrites, delegate, genericAccessor), m_someipDelegate(delegate)
{
}

SomeIpStringEncoding SomeipOverwriteAccessor::getSomeIpStringEncoding(const std::shared_ptr<BstIdl::FObject> &obj)
{
    if (std::dynamic_pointer_cast<BstIdl::FField>(obj))
    {
        // check if this field is overwritten
        if (m_mappedFields.count(std::dynamic_pointer_cast<BstIdl::FField>(obj)))
        {
            auto fo = m_mappedFields[std::dynamic_pointer_cast<BstIdl::FField>(obj)];
            std::string e = (std::dynamic_pointer_cast<BstIdl::GenericPropertyAccessor>(m_target))
                                ->getEnum(fo, "SomeIpStringEncoding");
            if (!e.empty())
                return SomeipDataPropertyAccessorHelper::convertSomeIpStringEncoding(e);
        }
    }
    else
    {
        if (m_overwrites != nullptr)
        {
            // this is some model element which might be overwritten
            std::string e = m_target->getEnum(obj, "SomeIpStringEncoding");
            if (!e.empty())
                return SomeipDataPropertyAccessorHelper::convertSomeIpStringEncoding(e);
        }
    }
    return m_someipDelegate->getSomeIpStringEncoding(obj);
}

std::shared_ptr<IDataPropertyAccessor> SomeipOverwriteAccessor::getOverwriteAccessor(
    const std::shared_ptr<BstIdl::FField> &obj)
{
    // check if this field is overwritten
    if (m_mappedFields.count(obj))
    {
        auto fo = m_mappedFields[obj];
        auto overwrites = fo->getOverwrites();
        if (overwrites == nullptr)
            return shared_from_this();
        else
        {
            auto ptr = std::dynamic_pointer_cast<SomeipOverwriteAccessor>(shared_from_this());
            auto ret = std::make_shared<SomeipOverwriteAccessor>(m_overwrites, ptr, m_target);
            return std::static_pointer_cast<IDataPropertyAccessor>(ret);
        }
    }
    return m_someipDelegate->getOverwriteAccessor(obj);
}

std::shared_ptr<IDataPropertyAccessor> SomeipOverwriteAccessor::getOverwriteAccessor(
    const std::shared_ptr<BstIdl::FArrayType> &obj)
{
    // check if this array is overwritten
    if (m_overwrites != nullptr)
    {
        auto ret = std::make_shared<SomeipOverwriteAccessor>(
            m_overwrites, std::dynamic_pointer_cast<SomeipOverwriteAccessor>(shared_from_this()), m_target);
        return std::static_pointer_cast<IDataPropertyAccessor>(ret);
    }
    return m_someipDelegate->getOverwriteAccessor(obj);
}
} // namespace BstCommonAPI