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
#ifndef DEPLOYMENT_TYPE_COLLECTION_PROPERTY_ACCESSOR
#define DEPLOYMENT_TYPE_COLLECTION_PROPERTY_ACCESSOR
#include "FDModel/FDeployedTypeCollection.h"
namespace BstCommonAPI
{
class DeploymentTypeCollectionPropertyAccessor
{
private:
    std::shared_ptr<BstIdl::FDeployedTypeCollection> m_target;

public:
    DeploymentTypeCollectionPropertyAccessor() = default;
    virtual ~DeploymentTypeCollectionPropertyAccessor() = default;
    DeploymentTypeCollectionPropertyAccessor(const DeploymentTypeCollectionPropertyAccessor &) = default;
    DeploymentTypeCollectionPropertyAccessor(DeploymentTypeCollectionPropertyAccessor &&) noexcept = default;
    DeploymentTypeCollectionPropertyAccessor &operator=(const DeploymentTypeCollectionPropertyAccessor &) = default;
    DeploymentTypeCollectionPropertyAccessor &operator=(DeploymentTypeCollectionPropertyAccessor &&) = default;
    DeploymentTypeCollectionPropertyAccessor(const std::shared_ptr<BstIdl::FDeployedTypeCollection> &target)
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

    int getSomeIpUnionUnionDefaultOrder(const std::shared_ptr<BstIdl::FObject> &obj)
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
};
} // namespace BstCommonAPI
#endif