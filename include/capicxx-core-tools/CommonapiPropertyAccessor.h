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
#ifndef F_COMMONAPI_PROPERTY_ACCESSOR_H
#define F_COMMONAPI_PROPERTY_ACCESSOR_H
#include "FDModel/FDFactory.h"
#include "FDModel/FDeployedInterface.h"
#include "FDModel/FDeployedProvider.h"
#include "FDModel/FDeployedTypeCollection.h"
#include "InterfacePropertyAccessor.h"
#include "ProviderPropertyAccessor.h"
#include "TypeCollectionPropertyAccessor.h"
#include "model/FBroadcast.h"
#include "model/FEnumerationType.h"
#include "model/FField.h"
#include "model/FInterface.h"
#include "model/FMethod.h"
namespace BstCommonAPI
{
class CommonapiPropertyAccessor
{
protected:
    enum class DeploymentType
    {
        NONE,
        INTERFACE,
        TYPE_COLLECTION,
        PROVIDER,
        OVERWRITE
    };
    DeploymentType type_;
    std::shared_ptr<ProviderPropertyAccessor> provider_;
    std::shared_ptr<IDataPropertyAccessor> dataAccessor_;
    int defaultTimeout_ = 0;

public:
    CommonapiPropertyAccessor() = default;
    virtual ~CommonapiPropertyAccessor() = default;
    CommonapiPropertyAccessor(const CommonapiPropertyAccessor &) = default;
    CommonapiPropertyAccessor(CommonapiPropertyAccessor &&) noexcept = default;
    CommonapiPropertyAccessor &operator=(const CommonapiPropertyAccessor &) = default;
    CommonapiPropertyAccessor &operator=(CommonapiPropertyAccessor &&) = default;

    CommonapiPropertyAccessor(const std::shared_ptr<BstIdl::FDeployedInterface> &_target) : CommonapiPropertyAccessor()
    {
        type_ = (_target == nullptr ? DeploymentType::NONE : DeploymentType::INTERFACE);
        dataAccessor_ = std::make_shared<InterfacePropertyAccessor>(_target);
    }

    CommonapiPropertyAccessor(const std::shared_ptr<BstIdl::FDeployedTypeCollection> &_target)
        : CommonapiPropertyAccessor()
    {
        type_ = (_target == nullptr ? DeploymentType::NONE : DeploymentType::TYPE_COLLECTION);
        dataAccessor_ = std::make_shared<TypeCollectionPropertyAccessor>(_target);
    }

    CommonapiPropertyAccessor(const std::shared_ptr<BstIdl::FDeployedProvider> &_target) : CommonapiPropertyAccessor()
    {
        type_ = (_target == nullptr ? DeploymentType::NONE : DeploymentType::PROVIDER);
        provider_ = std::make_shared<ProviderPropertyAccessor>(_target);
    }

    CommonapiPropertyAccessor(const std::shared_ptr<CommonapiPropertyAccessor> &_parent,
                              const std::shared_ptr<BstIdl::FField> &_element)
        : CommonapiPropertyAccessor()
    {
        type_ = DeploymentType::OVERWRITE;
        provider_ = nullptr;
        if (_parent->type_ != DeploymentType::PROVIDER)
            dataAccessor_ = _parent->dataAccessor_->getOverwriteAccessor(_element);
        else
            dataAccessor_ = nullptr;
    }
    CommonapiPropertyAccessor(const std::shared_ptr<CommonapiPropertyAccessor> &_parent,
                              const std::shared_ptr<BstIdl::FArrayType> &_element)
        : CommonapiPropertyAccessor()
    {
        type_ = DeploymentType::OVERWRITE;
        provider_ = nullptr;
        if (_parent->type_ != DeploymentType::PROVIDER)
            dataAccessor_ = _parent->dataAccessor_->getOverwriteAccessor(_element);
    }
    CommonapiPropertyAccessor(const std::shared_ptr<CommonapiPropertyAccessor> &_parent,
                              const std::shared_ptr<BstIdl::FArgument> &_element)
        : CommonapiPropertyAccessor()
    {
        type_ = DeploymentType::OVERWRITE;
        provider_ = nullptr;
        if (_parent->type_ == DeploymentType::INTERFACE)
        {
            auto ipa = std::dynamic_pointer_cast<InterfacePropertyAccessor>(_parent->dataAccessor_);
            if (ipa)
                dataAccessor_ = ipa->getOverwriteAccessor(_element);
        }
    }
    CommonapiPropertyAccessor(const std::shared_ptr<CommonapiPropertyAccessor> &_parent,
                              const std::shared_ptr<BstIdl::FAttribute> &_element)
        : CommonapiPropertyAccessor()
    {
        type_ = DeploymentType::OVERWRITE;
        provider_ = nullptr;
        if (_parent->type_ == DeploymentType::INTERFACE)
        {
            auto ipa = std::dynamic_pointer_cast<InterfacePropertyAccessor>(_parent->dataAccessor_);
            if (ipa)
                dataAccessor_ = ipa->getOverwriteAccessor(_element);
        }
    }

    EnumBackingType getEnumBackingType(const std::shared_ptr<BstIdl::FEnumerationType> &obj)
    {
        if (!dataAccessor_)
            return EnumBackingType::UInt8;
        switch (type_)
        {
        case DeploymentType::INTERFACE:
        case DeploymentType::TYPE_COLLECTION:
            return dataAccessor_->getEnumBackingType(obj);
        case DeploymentType::PROVIDER:
        case DeploymentType::NONE:
        default:
            return EnumBackingType::Default;
        }
    }

    int getTimeout(const std::shared_ptr<BstIdl::FMethod> &obj)
    {
        if (type_ == DeploymentType::INTERFACE)
        {
            auto ipa = std::dynamic_pointer_cast<InterfacePropertyAccessor>(dataAccessor_);
            return ipa ? ipa->getMethodTimeout(obj) : defaultTimeout_;
        }
        return defaultTimeout_;
    }

    BroadcastType getBroadcastType(const std::shared_ptr<BstIdl::FBroadcast> &obj)
    {
        if (type_ == DeploymentType::INTERFACE)
        {
            auto ipa = std::dynamic_pointer_cast<InterfacePropertyAccessor>(dataAccessor_);
            return ipa ? ipa->getBroadcastType(obj) : BroadcastType::signal;
        }
        return BroadcastType::signal;
    }

    std::list<std::string> getErrors(const std::shared_ptr<BstIdl::FMethod> &obj)
    {
        if (type_ == DeploymentType::INTERFACE)
        {
            auto ipa = std::dynamic_pointer_cast<InterfacePropertyAccessor>(dataAccessor_);
            return ipa ? ipa->getErrors(obj) : std::list<std::string>();
        }
        return std::list<std::string>();
    }

    std::string getInstanceId(const std::shared_ptr<BstIdl::FDExtensionElement> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
            return provider_->getInstanceId(obj);

        for (auto property : obj->getProperties())
        {
            if (property && property->getContainer() && property->getContainer()->getContainer())
            {
                if (std::dynamic_pointer_cast<BstIdl::FDExtensionElement>(property->getContainer()->getContainer()))
                {
                    if (property->getValue())
                    {
                        auto fdVal = property->getValue()->getSingle();
                        if (std::dynamic_pointer_cast<BstIdl::FDString>(fdVal))
                        {
                            auto value = std::dynamic_pointer_cast<BstIdl::FDString>(fdVal)->getValue();
                            return value;
                        }
                    }
                }
            }
        }

        return std::string();
    }

    std::string getDomain(const std::shared_ptr<BstIdl::FDExtensionElement> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
            return provider_->getDomain(obj);

        return "";
    }

    std::list<std::string> getPreregisteredProperties(const std::shared_ptr<BstIdl::FDExtensionElement> &obj)
    {
        if (type_ == DeploymentType::PROVIDER && provider_)
            return provider_->getPreregisteredProperties(obj);

        return std::list<std::string>();
    }

    std::string getErrorName(const std::shared_ptr<BstIdl::FBroadcast> &obj)
    {
        if (type_ == DeploymentType::INTERFACE)
        {
            auto ipa = std::dynamic_pointer_cast<InterfacePropertyAccessor>(dataAccessor_);
            return ipa ? ipa->getErrorName(obj) : "";
        }
        return "";
    }
};
} // namespace BstCommonAPI

#endif