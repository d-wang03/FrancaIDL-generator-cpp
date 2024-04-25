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
#ifndef SOMEIP_OVERWRITE_ACCESSOR_H
#define SOMEIP_OVERWRITE_ACCESSOR_H
#include "SomeipIDataPropertyAccessor.h"
#include "capicxx-core-tools/OverwriteAccessor.h"
namespace BstCommonAPI
{
class SomeipOverwriteAccessor : virtual public OverwriteAccessor, virtual public SomeipIDataPropertyAccessor
{
public:
    SomeipOverwriteAccessor() = default;
    virtual ~SomeipOverwriteAccessor() = default;
    SomeipOverwriteAccessor(const SomeipOverwriteAccessor &) = default;
    SomeipOverwriteAccessor(SomeipOverwriteAccessor &&) noexcept = default;
    SomeipOverwriteAccessor &operator=(const SomeipOverwriteAccessor &) = default;
    SomeipOverwriteAccessor &operator=(SomeipOverwriteAccessor &&rhs)
    {
        m_target = std::move(m_target);
        return *this;
    }
    SomeipOverwriteAccessor(const std::shared_ptr<BstIdl::FDTypeOverwrites> &overwrites,
                            const std::shared_ptr<SomeipIDataPropertyAccessor> &delegate,
                            const std::shared_ptr<BstIdl::MappingGenericPropertyAccessor> &genericAccessor);

    // host 'arrays'

    int getSomeIpArrayMinLength(const std::shared_ptr<BstIdl::FArrayType> &obj) override
    {
        if (m_overwrites != nullptr)
        {
            auto ptr = std::static_pointer_cast<BstIdl::GenericPropertyAccessor>(m_target);
            int v = ptr->getInteger(m_overwrites, "SomeIpArrayMinLength");
            if (v != -1)
                return v;
        }
        return m_someipDelegate->getSomeIpArrayMinLength(obj);
    }

    int getSomeIpArrayMinLength(const std::shared_ptr<BstIdl::FField> &obj) override
    {
        // check if this field is overwritten
        if (m_mappedFields.count(obj))
        {
            auto fo = m_mappedFields[obj];
            auto ptr = std::static_pointer_cast<BstIdl::GenericPropertyAccessor>(m_target);
            int v = ptr->getInteger(fo, "SomeIpArrayMinLength");
            if (v != -1)
                return v;
        }
        return m_someipDelegate->getSomeIpArrayMinLength(obj);
    }

    int getSomeIpArrayMaxLength(const std::shared_ptr<BstIdl::FArrayType> &obj) override
    {
        if (m_overwrites != nullptr)
        {
            auto ptr = std::static_pointer_cast<BstIdl::GenericPropertyAccessor>(m_target);
            int v = ptr->getInteger(m_overwrites, "SomeIpArrayMaxLength");
            if (v != -1)
                return v;
        }
        return m_someipDelegate->getSomeIpArrayMaxLength(obj);
    }

    int getSomeIpArrayMaxLength(const std::shared_ptr<BstIdl::FField> &obj) override
    {
        // check if this field is overwritten
        if (m_mappedFields.count(obj))
        {
            auto fo = m_mappedFields[obj];
            auto ptr = std::static_pointer_cast<BstIdl::GenericPropertyAccessor>(m_target);
            int v = ptr->getInteger(fo, "SomeIpArrayMaxLength");
            if (v != -1)
                return v;
        }
        return m_someipDelegate->getSomeIpArrayMaxLength(obj);
    }

    int getSomeIpArrayLengthWidth(const std::shared_ptr<BstIdl::FArrayType> &obj) override
    {
        if (m_overwrites != nullptr)
        {
            auto ptr = std::static_pointer_cast<BstIdl::GenericPropertyAccessor>(m_target);
            int v = ptr->getInteger(m_overwrites, "SomeIpArrayLengthWidth");
            if (v != -1)
                return v;
        }
        return m_someipDelegate->getSomeIpArrayLengthWidth(obj);
    }

    int getSomeIpArrayLengthWidth(const std::shared_ptr<BstIdl::FField> &obj) override
    {
        // check if this field is overwritten
        if (m_mappedFields.count(obj))
        {
            auto fo = m_mappedFields[obj];
            auto ptr = std::static_pointer_cast<BstIdl::GenericPropertyAccessor>(m_target);
            int v = ptr->getInteger(fo, "SomeIpArrayLengthWidth");
            if (v != -1)
                return v;
        }
        return m_someipDelegate->getSomeIpArrayLengthWidth(obj);
    }

    // host 'unions'

    int getSomeIpUnionLengthWidth(const std::shared_ptr<BstIdl::FUnionType> &obj) override
    {
        if (m_overwrites != nullptr)
        {
            auto ptr = std::static_pointer_cast<BstIdl::GenericPropertyAccessor>(m_target);
            int v = ptr->getInteger(m_overwrites, "SomeIpUnionLengthWidth");
            if (v != -1)
                return v;
        }
        return m_someipDelegate->getSomeIpUnionLengthWidth(obj);
    }

    int getSomeIpUnionTypeWidth(const std::shared_ptr<BstIdl::FUnionType> &obj) override
    {
        if (m_overwrites != nullptr)
        {
            auto ptr = std::static_pointer_cast<BstIdl::GenericPropertyAccessor>(m_target);
            int v = ptr->getInteger(m_overwrites, "SomeIpUnionTypeWidth");
            if (v != -1)
                return v;
        }
        return m_someipDelegate->getSomeIpUnionTypeWidth(obj);
    }

    int getSomeIpUnionDefaultOrder(const std::shared_ptr<BstIdl::FUnionType> &obj) override
    {
        if (m_overwrites != nullptr)
        {
            auto ptr = std::static_pointer_cast<BstIdl::GenericPropertyAccessor>(m_target);
            bool isOK = false;
            bool v = ptr->getBoolean(m_overwrites, "SomeIpUnionDefaultOrder", isOK);
            if (isOK)
                return v;
        }
        return m_someipDelegate->getSomeIpUnionDefaultOrder(obj);
    }

    int getSomeIpUnionMaxLength(const std::shared_ptr<BstIdl::FUnionType> &obj) override
    {
        if (m_overwrites != nullptr)
        {
            auto ptr = std::static_pointer_cast<BstIdl::GenericPropertyAccessor>(m_target);
            int v = ptr->getInteger(m_overwrites, "SomeIpUnionMaxLength");
            if (v != -1)
                return v;
        }
        return m_someipDelegate->getSomeIpUnionMaxLength(obj);
    }

    // host 'structs'

    int getSomeIpStructLengthWidth(const std::shared_ptr<BstIdl::FStructType> &obj) override
    {
        if (m_overwrites != nullptr)
        {
            auto ptr = std::static_pointer_cast<BstIdl::GenericPropertyAccessor>(m_target);
            int v = ptr->getInteger(m_overwrites, "SomeIpStructLengthWidth");
            if (v != -1)
                return v;
        }
        return m_someipDelegate->getSomeIpStructLengthWidth(obj);
    }

    // host 'enumerations'

    int getSomeIpEnumWidth(const std::shared_ptr<BstIdl::FEnumerationType> &obj) override
    {
        if (m_overwrites != nullptr)
        {
            auto ptr = std::static_pointer_cast<BstIdl::GenericPropertyAccessor>(m_target);
            int v = ptr->getInteger(m_overwrites, "SomeIpEnumWidth");
            if (v != -1)
                return v;
        }
        return m_someipDelegate->getSomeIpEnumWidth(obj);
    }

    int getSomeIpEnumBitWidth(const std::shared_ptr<BstIdl::FEnumerationType> &obj) override
    {
        if (m_overwrites != nullptr)
        {
            auto ptr = std::static_pointer_cast<BstIdl::GenericPropertyAccessor>(m_target);
            int v = ptr->getInteger(m_overwrites, "SomeIpEnumBitWidth");
            if (v != -1)
                return v;
        }
        return m_someipDelegate->getSomeIpEnumBitWidth(obj);
    }

    int getSomeIpEnumInvalidValue(const std::shared_ptr<BstIdl::FEnumerationType> &obj) override
    {
        if (m_overwrites != nullptr)
        {
            auto ptr = std::static_pointer_cast<BstIdl::GenericPropertyAccessor>(m_target);
            int v = ptr->getInteger(m_overwrites, "SomeIpEnumInvalidValue");
            if (v != -1)
                return v;
        }
        return m_someipDelegate->getSomeIpEnumInvalidValue(obj);
    }

    // host 'strings'

    int getSomeIpStringLength(const std::shared_ptr<BstIdl::FObject> &obj) override
    {
        if (std::dynamic_pointer_cast<BstIdl::FField>(obj))
        {
            // check if this field is overwritten
            if (m_mappedFields.count(std::dynamic_pointer_cast<BstIdl::FField>(obj)))
            {
                auto fo = m_mappedFields[std::dynamic_pointer_cast<BstIdl::FField>(obj)];
                auto ptr = std::static_pointer_cast<BstIdl::GenericPropertyAccessor>(m_target);
                int v = ptr->getInteger(fo, "SomeIpStringLength");
                if (v != -1)
                    return v;
            }
        }
        else
        {
            if (m_overwrites != nullptr)
            {
                // this is some model element which might be overwritten
                int v = m_target->getInteger(obj, "SomeIpStringLength");
                if (v != -1)
                    return v;
            }
        }
        return m_someipDelegate->getSomeIpStringLength(obj);
    }

    int getSomeIpStringLengthWidth(const std::shared_ptr<BstIdl::FObject> &obj) override
    {
        if (std::dynamic_pointer_cast<BstIdl::FField>(obj))
        {
            // check if this field is overwritten
            if (m_mappedFields.count(std::dynamic_pointer_cast<BstIdl::FField>(obj)))
            {
                auto fo = m_mappedFields[std::dynamic_pointer_cast<BstIdl::FField>(obj)];
                auto ptr = std::static_pointer_cast<BstIdl::GenericPropertyAccessor>(m_target);
                int v = ptr->getInteger(fo, "SomeIpStringLengthWidth");
                if (v != -1)
                    return v;
            }
        }
        else
        {
            if (m_overwrites != nullptr)
            {
                // this is some model element which might be overwritten
                int v = m_target->getInteger(obj, "SomeIpStringLengthWidth");
                if (v != -1)
                    return v;
            }
        }
        return m_someipDelegate->getSomeIpStringLengthWidth(obj);
    }

    SomeIpStringEncoding getSomeIpStringEncoding(const std::shared_ptr<BstIdl::FObject> &obj) override;

    // host 'byte_buffers'

    int getSomeIpByteBufferMaxLength(const std::shared_ptr<BstIdl::FObject> &obj) override
    {
        if (std::dynamic_pointer_cast<BstIdl::FField>(obj))
        {
            // check if this field is overwritten
            if (m_mappedFields.count(std::dynamic_pointer_cast<BstIdl::FField>(obj)))
            {
                auto fo = m_mappedFields[std::dynamic_pointer_cast<BstIdl::FField>(obj)];
                auto ptr = std::static_pointer_cast<BstIdl::GenericPropertyAccessor>(m_target);
                int v = ptr->getInteger(fo, "SomeIpByteBufferMaxLength");
                if (v != -1)
                    return v;
            }
        }
        else
        {
            if (m_overwrites != nullptr)
            {
                // this is some model element which might be overwritten
                int v = m_target->getInteger(obj, "SomeIpByteBufferMaxLength");
                if (v != -1)
                    return v;
            }
        }
        return m_someipDelegate->getSomeIpByteBufferMaxLength(obj);
    }

    int getSomeIpByteBufferMinLength(const std::shared_ptr<BstIdl::FObject> &obj) override
    {
        if (std::dynamic_pointer_cast<BstIdl::FField>(obj))
        {
            // check if this field is overwritten
            if (m_mappedFields.count(std::dynamic_pointer_cast<BstIdl::FField>(obj)))
            {
                auto fo = m_mappedFields[std::dynamic_pointer_cast<BstIdl::FField>(obj)];
                auto ptr = std::static_pointer_cast<BstIdl::GenericPropertyAccessor>(m_target);
                int v = ptr->getInteger(fo, "SomeIpByteBufferMinLength");
                if (v != -1)
                    return v;
            }
        }
        else
        {
            if (m_overwrites != nullptr)
            {
                // this is some model element which might be overwritten
                int v = m_target->getInteger(obj, "SomeIpByteBufferMinLength");
                if (v != -1)
                    return v;
            }
        }
        return m_someipDelegate->getSomeIpByteBufferMinLength(obj);
    }

    int getSomeIpByteBufferLengthWidth(const std::shared_ptr<BstIdl::FObject> &obj) override
    {
        if (std::dynamic_pointer_cast<BstIdl::FField>(obj))
        {
            // check if this field is overwritten
            if (m_mappedFields.count(std::dynamic_pointer_cast<BstIdl::FField>(obj)))
            {
                auto fo = m_mappedFields[std::dynamic_pointer_cast<BstIdl::FField>(obj)];
                auto ptr = std::static_pointer_cast<BstIdl::GenericPropertyAccessor>(m_target);
                int v = ptr->getInteger(fo, "SomeIpByteBufferLengthWidth");
                if (v != -1)
                    return v;
            }
        }
        else
        {
            if (m_overwrites != nullptr)
            {
                // this is some model element which might be overwritten
                int v = m_target->getInteger(obj, "SomeIpByteBufferLengthWidth");
                if (v != -1)
                    return v;
            }
        }
        return m_someipDelegate->getSomeIpByteBufferLengthWidth(obj);
    }

    // host 'integers'

    int getSomeIpIntegerBitWidth(const std::shared_ptr<BstIdl::FObject> &obj) override
    {
        if (std::dynamic_pointer_cast<BstIdl::FField>(obj))
        {
            // check if this field is overwritten
            if (m_mappedFields.count(std::dynamic_pointer_cast<BstIdl::FField>(obj)))
            {
                auto fo = m_mappedFields[std::dynamic_pointer_cast<BstIdl::FField>(obj)];
                auto ptr = std::static_pointer_cast<BstIdl::GenericPropertyAccessor>(m_target);
                int v = ptr->getInteger(fo, "SomeIpIntegerBitWidth");
                if (v != -1)
                    return v;
            }
        }
        else
        {
            if (m_overwrites != nullptr)
            {
                // this is some model element which might be overwritten
                int v = m_target->getInteger(obj, "SomeIpIntegerBitWidth");
                if (v != -1)
                    return v;
            }
        }
        return m_someipDelegate->getSomeIpIntegerBitWidth(obj);
    }

    int getSomeIpIntegerInvalidValue(const std::shared_ptr<BstIdl::FObject> &obj) override
    {
        if (std::dynamic_pointer_cast<BstIdl::FField>(obj))
        {
            // check if this field is overwritten
            if (m_mappedFields.count(std::dynamic_pointer_cast<BstIdl::FField>(obj)))
            {
                auto fo = m_mappedFields[std::dynamic_pointer_cast<BstIdl::FField>(obj)];
                auto ptr = std::static_pointer_cast<BstIdl::GenericPropertyAccessor>(m_target);
                int v = ptr->getInteger(fo, "SomeIpIntegerInvalidValue");
                if (v != -1)
                    return v;
            }
        }
        else
        {
            if (m_overwrites != nullptr)
            {
                // this is some model element which might be overwritten
                int v = m_target->getInteger(obj, "SomeIpIntegerInvalidValue");
                if (v != -1)
                    return v;
            }
        }
        return m_someipDelegate->getSomeIpIntegerInvalidValue(obj);
    }

    std::shared_ptr<IDataPropertyAccessor> getOverwriteAccessor(const std::shared_ptr<BstIdl::FField> &obj) override;

    std::shared_ptr<IDataPropertyAccessor> getOverwriteAccessor(
        const std::shared_ptr<BstIdl::FArrayType> &obj) override;

private:
    std::shared_ptr<SomeipIDataPropertyAccessor> m_someipDelegate;
};
} // namespace BstCommonAPI
#endif