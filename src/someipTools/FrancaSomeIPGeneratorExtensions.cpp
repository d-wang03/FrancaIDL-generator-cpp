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
#include "someipTools/FrancaSomeIPGeneratorExtensions.h"
#include "fmt/core.h"
#include "model/FModelManager.h"
#include "utilities/list_utility.h"
#include "utilities/string_utility.h"
namespace BstCommonAPI
{
std::shared_ptr<SomeipPropertyAccessor> FrancaSomeIPGeneratorExtensions::getSomeIpAccessor(
    const std::shared_ptr<BstIdl::FTypeCollection> &_tc)
{
    auto accessor = std::dynamic_pointer_cast<SomeipPropertyAccessor>(FrancaGeneratorExtensions::getAccessor(_tc));
    if (!accessor)
        std::cerr << "Could not find someip accessor for " << _tc->getName() << std::endl;
    return accessor;
}

std::string FrancaSomeIPGeneratorExtensions::someipDeploymentHeaderFile(
    const std::shared_ptr<BstIdl::FInterface> &fInterface)
{
    auto ins = FrancaGeneratorExtensions::getInstance();
    return ins.getElementName(fInterface) + "SomeIPDeployment.hpp";
}

std::string FrancaSomeIPGeneratorExtensions::someipDeploymentHeaderPath(
    const std::shared_ptr<BstIdl::FInterface> &fInterface)
{
    auto ins = FrancaGeneratorExtensions::getInstance();
    return ins.getVersionPathPrefix(fInterface) +
           ins.getDirectoryPath(std::dynamic_pointer_cast<BstIdl::FModel>(fInterface->getContainer())) + "/" +
           someipDeploymentHeaderFile(fInterface);
}

std::string FrancaSomeIPGeneratorExtensions::someipDeploymentSourceFile(
    const std::shared_ptr<BstIdl::FInterface> &fInterface)
{
    auto ins = FrancaGeneratorExtensions::getInstance();
    return ins.getElementName(fInterface) + "SomeIPDeployment.cpp";
}

std::string FrancaSomeIPGeneratorExtensions::someipDeploymentSourcePath(
    const std::shared_ptr<BstIdl::FInterface> &fInterface)
{
    auto ins = FrancaGeneratorExtensions::getInstance();
    return ins.getVersionPathPrefix(fInterface) +
           ins.getDirectoryPath(std::dynamic_pointer_cast<BstIdl::FModel>(fInterface->getContainer())) + "/" +
           someipDeploymentSourceFile(fInterface);
}

std::string FrancaSomeIPGeneratorExtensions::someipDeploymentHeaderFile(
    const std::shared_ptr<BstIdl::FTypeCollection> &_tc)
{
    auto ins = FrancaGeneratorExtensions::getInstance();
    return ins.getElementName(_tc) + "SomeIPDeployment.hpp";
}

std::string FrancaSomeIPGeneratorExtensions::someipDeploymentHeaderPath(
    const std::shared_ptr<BstIdl::FTypeCollection> &_tc)
{
    auto ins = FrancaGeneratorExtensions::getInstance();
    return ins.getVersionPathPrefix(_tc) +
           ins.getDirectoryPath(std::dynamic_pointer_cast<BstIdl::FModel>(_tc->getContainer())) + '/' +
           someipDeploymentHeaderFile(_tc);
}

std::string FrancaSomeIPGeneratorExtensions::someipDeploymentSourceFile(
    const std::shared_ptr<BstIdl::FTypeCollection> &_tc)
{
    auto ins = FrancaGeneratorExtensions::getInstance();
    return ins.getElementName(_tc) + "SomeIPDeployment.cpp";
}

std::string FrancaSomeIPGeneratorExtensions::someipDeploymentSourcePath(
    const std::shared_ptr<BstIdl::FTypeCollection> &_tc)
{
    auto ins = FrancaGeneratorExtensions::getInstance();
    return ins.getVersionPathPrefix(_tc) +
           ins.getDirectoryPath(std::dynamic_pointer_cast<BstIdl::FModel>(_tc->getContainer())) + '/' +
           someipDeploymentSourceFile(_tc);
}

std::string FrancaSomeIPGeneratorExtensions::someipProxyHeaderFile(
    const std::shared_ptr<BstIdl::FInterface> &fInterface)
{
    auto ins = FrancaGeneratorExtensions::getInstance();
    return ins.getElementName(fInterface) + "SomeIPProxy.hpp";
}

std::string FrancaSomeIPGeneratorExtensions::someipProxyHeaderPath(
    const std::shared_ptr<BstIdl::FInterface> &fInterface)
{
    auto ins = FrancaGeneratorExtensions::getInstance();
    return ins.getVersionPathPrefix(fInterface) +
           ins.getDirectoryPath(std::dynamic_pointer_cast<BstIdl::FModel>(fInterface->getContainer())) + "/" +
           someipDeploymentHeaderFile(fInterface);
}

std::string FrancaSomeIPGeneratorExtensions::someipProxySourceFile(
    const std::shared_ptr<BstIdl::FInterface> &fInterface)
{
    auto ins = FrancaGeneratorExtensions::getInstance();
    return ins.getElementName(fInterface) + "SomeIPProxy.cpp";
}

std::string FrancaSomeIPGeneratorExtensions::someipProxySourcePath(
    const std::shared_ptr<BstIdl::FInterface> &fInterface)
{
    auto ins = FrancaGeneratorExtensions::getInstance();
    return ins.getVersionPathPrefix(fInterface) +
           ins.getDirectoryPath(std::dynamic_pointer_cast<BstIdl::FModel>(fInterface->getContainer())) + "/" +
           someipDeploymentSourceFile(fInterface);
}

std::string FrancaSomeIPGeneratorExtensions::getSomeIPVersion()
{
    return " 3.2.0.1";
}

std::string FrancaSomeIPGeneratorExtensions::generateCommonApiSomeIPLicenseHeader()
{
    auto ins = FrancaGeneratorExtensions::getInstance();
    std::string header = R"(/*
* This file was generated by the BST IDL Generator.
* Comaptible to org.genivi.commonapi.someip SOMEIP
* Compatible to org.genivi.commonapi.core CORE.
* Compatible to org.franca.core VERSION.
*/)";
    replace_one(header, "SOMEIP", getSomeIPVersion());
    replace_one(header, "CORE", ins.getCoreVersion());
    replace_one(header, "VERSION", ins.getFrancaVersion());
    return header;
}

std::shared_ptr<SomeipPropertyAccessor> FrancaSomeIPGeneratorExtensions::getSpecificAccessor(
    const std::shared_ptr<BstIdl::FObject> &_obj)
{
    auto container = _obj->getContainer();
    while (container)
    {
        if (auto ptr = std::dynamic_pointer_cast<BstIdl::FTypeCollection>(container))
        {
            return getSomeIpAccessor(ptr);
        }
        container = container->getContainer();
    }
    return nullptr;
}

int FrancaSomeIPGeneratorExtensions::getSomeIpArrayMaxLengthHelper(
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor, const std::shared_ptr<BstIdl::FObject> &_obj)
{
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FAttribute>(_obj))
    {
        int maxLength = _accessor->getSomeIpArrayMaxLength(ptr);
        if (maxLength == -1 && ptr->getType()->getDerived())
            maxLength = getSomeIpArrayMaxLengthHelper(_accessor, ptr->getType()->getDerived());
        return maxLength == -1 ? SOMEIP_DEFAULT_MAX_LENGTH : maxLength;
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FArgument>(_obj))
    {
        int maxLength = _accessor->getSomeIpArrayMaxLength(ptr);
        if (maxLength == -1 && ptr->getType()->getDerived())
            maxLength = getSomeIpArrayMaxLengthHelper(_accessor, ptr->getType()->getDerived());
        return maxLength == -1 ? SOMEIP_DEFAULT_MAX_LENGTH : maxLength;
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FField>(_obj))
    {
        int maxLength = _accessor->getSomeIpArrayMaxLength(ptr);
        return maxLength == -1 ? SOMEIP_DEFAULT_MAX_LENGTH : maxLength;
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FTypeDef>(_obj))
    {
        auto derived = ptr->getActualType()->getDerived();
        if (derived)
        {
            auto array = std::dynamic_pointer_cast<BstIdl::FArrayType>(derived);
            if (array)
                return _accessor->getSomeIpArrayMaxLength(array);
        }
        return SOMEIP_DEFAULT_MAX_LENGTH;
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FArrayType>(_obj))
        return _accessor->getSomeIpArrayMaxLength(ptr);

    return SOMEIP_DEFAULT_MAX_LENGTH;
}

int FrancaSomeIPGeneratorExtensions::getSomeIpArrayMinLengthHelper(
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor, const std::shared_ptr<BstIdl::FObject> &_obj)
{
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FAttribute>(_obj))
    {
        auto minLength = _accessor->getSomeIpArrayMinLength(ptr);
        if (minLength == -1 && ptr->getType()->getDerived())
            minLength = getSomeIpArrayMinLengthHelper(_accessor, ptr->getType()->getDerived());
        return minLength == -1 ? SOMEIP_DEFAULT_MIN_LENGTH : minLength;
    }
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FArgument>(_obj))
    {
        auto minLength = _accessor->getSomeIpArrayMinLength(ptr);
        if (minLength == -1 && ptr->getType()->getDerived())
            minLength = getSomeIpArrayMinLengthHelper(_accessor, ptr->getType()->getDerived());
        return minLength == -1 ? SOMEIP_DEFAULT_MIN_LENGTH : minLength;
    }
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FField>(_obj))
    {
        auto minLength = _accessor->getSomeIpArrayMinLength(ptr);
        return minLength == -1 ? SOMEIP_DEFAULT_MIN_LENGTH : minLength;
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FTypeDef>(_obj))
    {
        auto derived = ptr->getActualType()->getDerived();
        if (derived)
        {
            auto array = std::dynamic_pointer_cast<BstIdl::FArrayType>(derived);
            if (array)
                return _accessor->getSomeIpArrayMinLength(array);
        }
        return SOMEIP_DEFAULT_MIN_LENGTH;
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FArrayType>(_obj))
        return _accessor->getSomeIpArrayMinLength(ptr);

    return SOMEIP_DEFAULT_MIN_LENGTH;
}

int FrancaSomeIPGeneratorExtensions::getSomeIpArrayLengthWidthHelper(
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor, const std::shared_ptr<BstIdl::FObject> &_obj)
{
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FAttribute>(_obj))
    {
        auto lengthWidth = _accessor->getSomeIpArrayLengthWidth(ptr);
        if (lengthWidth == -1 && ptr->getType()->getDerived())
            lengthWidth = getSomeIpArrayLengthWidthHelper(_accessor, ptr->getType()->getDerived());
        return lengthWidth == -1 ? SOMEIP_DEFAULT_LENGTH_WIDTH : lengthWidth;
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FArgument>(_obj))
    {
        auto lengthWidth = _accessor->getSomeIpArrayLengthWidth(ptr);
        if (lengthWidth == -1 && ptr->getType()->getDerived())
            lengthWidth = getSomeIpArrayLengthWidthHelper(_accessor, ptr->getType()->getDerived());
        return lengthWidth == -1 ? SOMEIP_DEFAULT_LENGTH_WIDTH : lengthWidth;
    }

    if (std::dynamic_pointer_cast<BstIdl::FField>(_obj))
    {
        int lengthWidth = _accessor->getSomeIpArrayLengthWidth(std::dynamic_pointer_cast<BstIdl::FField>(_obj));
        return lengthWidth == -1 ? SOMEIP_DEFAULT_LENGTH_WIDTH : lengthWidth;
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FTypeDef>(_obj))
    {
        auto derived = ptr->getActualType()->getDerived();
        if (derived)
        {
            auto array = std::dynamic_pointer_cast<BstIdl::FArrayType>(derived);
            if (array)
                return _accessor->getSomeIpArrayLengthWidth(array);
        }
        return SOMEIP_DEFAULT_LENGTH_WIDTH;
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FArrayType>(_obj))
        return _accessor->getSomeIpArrayLengthWidth(ptr);

    return SOMEIP_DEFAULT_LENGTH_WIDTH;
}

int FrancaSomeIPGeneratorExtensions::getSomeIpMapMinLengthHelper(
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor, const std::shared_ptr<BstIdl::FObject> &_obj)
{
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FAttribute>(_obj))
    {
        auto minLength = _accessor->getSomeIpAttrMapMinLength(ptr);
        if (minLength == -1 && ptr->getType()->getDerived())
            minLength = getSomeIpMapMinLengthHelper(_accessor, ptr->getType()->getDerived());
        return minLength == -1 ? SOMEIP_DEFAULT_MIN_LENGTH : minLength;
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FArgument>(_obj))
    {
        auto minLength = _accessor->getSomeIpArgMapMinLength(ptr);
        if (minLength == -1 && ptr->getType()->getDerived())
            minLength = getSomeIpMapMinLengthHelper(_accessor, ptr->getType()->getDerived());
        return minLength == -1 ? SOMEIP_DEFAULT_MIN_LENGTH : minLength;
    }

    return -1;
}

int FrancaSomeIPGeneratorExtensions::getSomeIpMapMaxLengthHelper(
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor, const std::shared_ptr<BstIdl::FObject> &_obj)
{
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FAttribute>(_obj))
    {
        auto maxLength = _accessor->getSomeIpAttrMapMaxLength(ptr);
        if (maxLength == -1 && ptr->getType()->getDerived())
            maxLength = getSomeIpMapMaxLengthHelper(_accessor, ptr->getType()->getDerived());
        return maxLength == -1 ? SOMEIP_DEFAULT_MAX_LENGTH : maxLength;
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FArgument>(_obj))
    {
        auto maxLength = _accessor->getSomeIpArgMapMaxLength(ptr);
        if (maxLength == -1 && ptr->getType()->getDerived())
            maxLength = getSomeIpMapMaxLengthHelper(_accessor, ptr->getType()->getDerived());
        return maxLength == -1 ? SOMEIP_DEFAULT_MAX_LENGTH : maxLength;
    }

    return SOMEIP_DEFAULT_MAX_LENGTH;
}

int FrancaSomeIPGeneratorExtensions::getSomeIpMapLengthWidthHelper(
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor, const std::shared_ptr<BstIdl::FObject> &_obj)
{
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FAttribute>(_obj))
    {
        auto lengthWidth = _accessor->getSomeIpAttrMapLengthWidth(ptr);
        if (lengthWidth == -1 && ptr->getType()->getDerived())
            lengthWidth = getSomeIpMapLengthWidthHelper(_accessor, ptr->getType()->getDerived());
        return lengthWidth == -1 ? SOMEIP_DEFAULT_LENGTH_WIDTH : lengthWidth;
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FArgument>(_obj))
    {
        auto lengthWidth = _accessor->getSomeIpArgMapLengthWidth(ptr);
        if (lengthWidth == -1 && ptr->getType()->getDerived())
            lengthWidth = getSomeIpMapLengthWidthHelper(_accessor, ptr->getType()->getDerived());
        return lengthWidth == -1 ? SOMEIP_DEFAULT_LENGTH_WIDTH : lengthWidth;
    }

    return SOMEIP_DEFAULT_LENGTH_WIDTH;
}

int FrancaSomeIPGeneratorExtensions::getSomeIpUnionLengthWidthHelper(
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor, const std::shared_ptr<BstIdl::FObject> &_obj)
{
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FAttribute>(_obj))
    {
        if (auto derived = ptr->getType()->getDerived())
            return getSomeIpUnionLengthWidthHelper(_accessor, derived);
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FArgument>(_obj))
    {
        if (auto derived = ptr->getType()->getDerived())
            return getSomeIpUnionLengthWidthHelper(_accessor, derived);
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FField>(_obj))
    {
        if (auto derived = ptr->getType()->getDerived())
            return getSomeIpUnionLengthWidthHelper(_accessor, derived);
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FTypeDef>(_obj))
    {
        auto derived = ptr->getActualType()->getDerived();
        if (derived)
        {
            auto tmp = std::dynamic_pointer_cast<BstIdl::FUnionType>(derived);
            if (tmp)
            {
                auto ret = _accessor->getSomeIpUnionLengthWidth(tmp);
                return ret == -1 ? SOMEIP_DEFAULT_LENGTH_WIDTH : ret;
            }
        }
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FUnionType>(_obj))
    {
        auto ret = _accessor->getSomeIpUnionLengthWidth(ptr);
        return ret == -1 ? SOMEIP_DEFAULT_LENGTH_WIDTH : ret;
    }

    return SOMEIP_DEFAULT_LENGTH_WIDTH;
}

int FrancaSomeIPGeneratorExtensions::getSomeIpUnionTypeWidthHelper(
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor, const std::shared_ptr<BstIdl::FObject> &_obj)
{
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FAttribute>(_obj))
    {
        if (auto derived = ptr->getType()->getDerived())
            return getSomeIpUnionTypeWidthHelper(_accessor, derived);
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FArgument>(_obj))
    {
        if (auto derived = ptr->getType()->getDerived())
            return getSomeIpUnionTypeWidthHelper(_accessor, derived);
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FField>(_obj))
    {
        if (auto derived = ptr->getType()->getDerived())
            return getSomeIpUnionTypeWidthHelper(_accessor, derived);
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FTypeDef>(_obj))
    {
        auto derived = ptr->getActualType()->getDerived();
        if (derived)
        {
            auto tmp = std::dynamic_pointer_cast<BstIdl::FUnionType>(derived);
            if (tmp)
            {
                auto ret = _accessor->getSomeIpUnionTypeWidth(tmp);
                return ret == -1 ? SOMEIP_DEFAULT_LENGTH_WIDTH : ret;
            }
        }
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FUnionType>(_obj))
    {
        auto ret = _accessor->getSomeIpUnionTypeWidth(ptr);
        return ret == -1 ? SOMEIP_DEFAULT_LENGTH_WIDTH : ret;
    }

    return SOMEIP_DEFAULT_LENGTH_WIDTH;
}

bool FrancaSomeIPGeneratorExtensions::getSomeIpUnionDefaultOrderHelper(
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor, const std::shared_ptr<BstIdl::FObject> &_obj)
{
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FAttribute>(_obj))
    {
        if (auto derived = ptr->getType()->getDerived())
            return getSomeIpUnionDefaultOrderHelper(_accessor, derived);
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FArgument>(_obj))
    {
        if (auto derived = ptr->getType()->getDerived())
            return getSomeIpUnionDefaultOrderHelper(_accessor, derived);
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FField>(_obj))
    {
        if (auto derived = ptr->getType()->getDerived())
            return getSomeIpUnionDefaultOrderHelper(_accessor, derived);
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FTypeDef>(_obj))
    {
        auto derived = ptr->getActualType()->getDerived();
        if (derived)
        {
            auto tmp = std::dynamic_pointer_cast<BstIdl::FUnionType>(derived);
            if (tmp)
            {
                auto ret = _accessor->getSomeIpUnionDefaultOrder(tmp);
                return ret == -1 ? SOMEIP_DEFAULT_UNION_DEFAULT_ORDER : ret;
            }
        }
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FUnionType>(_obj))
    {
        auto ret = _accessor->getSomeIpUnionDefaultOrder(ptr);
        return ret == -1 ? SOMEIP_DEFAULT_UNION_DEFAULT_ORDER : ret;
    }

    return SOMEIP_DEFAULT_UNION_DEFAULT_ORDER;
}

int FrancaSomeIPGeneratorExtensions::getSomeIpUnionMaxLengthHelper(
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor, const std::shared_ptr<BstIdl::FObject> &_obj)
{
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FAttribute>(_obj))
    {
        if (auto derived = ptr->getType()->getDerived())
            return getSomeIpUnionMaxLengthHelper(_accessor, derived);
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FArgument>(_obj))
    {
        if (auto derived = ptr->getType()->getDerived())
            return getSomeIpUnionMaxLengthHelper(_accessor, derived);
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FField>(_obj))
    {
        if (auto derived = ptr->getType()->getDerived())
            return getSomeIpUnionMaxLengthHelper(_accessor, derived);
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FTypeDef>(_obj))
    {
        auto derived = ptr->getActualType()->getDerived();
        if (derived)
        {
            auto tmp = std::dynamic_pointer_cast<BstIdl::FUnionType>(derived);
            if (tmp)
            {
                auto ret = _accessor->getSomeIpUnionMaxLength(tmp);
                return ret == -1 ? SOMEIP_DEFAULT_MAX_LENGTH : ret;
            }
        }
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FUnionType>(_obj))
    {
        auto ret = _accessor->getSomeIpUnionMaxLength(ptr);
        return ret == -1 ? SOMEIP_DEFAULT_MAX_LENGTH : ret;
    }

    return SOMEIP_DEFAULT_MAX_LENGTH;
}

int FrancaSomeIPGeneratorExtensions::getSomeIpStructLengthWidthHelper(
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor, const std::shared_ptr<BstIdl::FObject> &_obj)
{
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FAttribute>(_obj))
    {
        if (auto derived = ptr->getType()->getDerived())
            return getSomeIpStructLengthWidthHelper(_accessor, derived);
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FArgument>(_obj))
    {
        if (auto derived = ptr->getType()->getDerived())
            return getSomeIpStructLengthWidthHelper(_accessor, derived);
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FField>(_obj))
    {
        if (auto derived = ptr->getType()->getDerived())
            return getSomeIpStructLengthWidthHelper(_accessor, derived);
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FTypeDef>(_obj))
    {
        auto derived = ptr->getActualType()->getDerived();
        if (derived)
        {
            auto tmp = std::dynamic_pointer_cast<BstIdl::FStructType>(derived);
            if (tmp)
            {
                auto ret = _accessor->getSomeIpStructLengthWidth(tmp);
                return ret == -1 ? SOMEIP_DEFAULT_STRUCT_LENGTH_WIDTH : ret;
            }
        }
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FStructType>(_obj))
    {
        auto ret = _accessor->getSomeIpStructLengthWidth(ptr);
        return ret == -1 ? SOMEIP_DEFAULT_STRUCT_LENGTH_WIDTH : ret;
    }

    return SOMEIP_DEFAULT_STRUCT_LENGTH_WIDTH;
}

int FrancaSomeIPGeneratorExtensions::getSomeIpEnumWidthHelper(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                              const std::shared_ptr<BstIdl::FObject> &_obj)
{
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FEnumerationType>(_obj))
    {
        int enumWidth = _accessor->getSomeIpEnumWidth(ptr);
        int enumBaseWidth = 0;
        if (auto base = ptr->getBase())
        {
            auto baseAccessor =
                getSomeIpAccessor(std::dynamic_pointer_cast<BstIdl::FTypeCollection>(base->getContainer()));
            if (baseAccessor)
                enumBaseWidth = getSomeIpEnumWidthHelper(baseAccessor, base);
        }
        return (enumBaseWidth > 0 && enumBaseWidth > enumWidth ? enumBaseWidth : enumWidth);
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FTypeDef>(_obj))
    {
        auto derived = ptr->getActualType()->getDerived();
        if (derived)
        {
            auto tmp = std::dynamic_pointer_cast<BstIdl::FEnumerationType>(derived);
            if (tmp)
                return getSomeIpEnumWidthHelper(_accessor, tmp);
        }
    }

    return -1;
}

int FrancaSomeIPGeneratorExtensions::getSomeIpEnumBitWidthHelper(
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor, const std::shared_ptr<BstIdl::FObject> &_obj)
{
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FEnumerationType>(_obj))
    {
        int enumWidth = _accessor->getSomeIpEnumWidth(ptr);
        int enumBaseWidth = 0;
        if (auto base = ptr->getBase())
        {
            auto baseAccessor =
                getSomeIpAccessor(std::dynamic_pointer_cast<BstIdl::FTypeCollection>(base->getContainer()));
            if (baseAccessor)
                enumBaseWidth = getSomeIpEnumBitWidthHelper(baseAccessor, base);
        }
        return (enumBaseWidth > 0 && enumBaseWidth > enumWidth ? enumBaseWidth : enumWidth);
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FTypeDef>(_obj))
    {
        auto derived = ptr->getActualType()->getDerived();
        if (derived)
        {
            auto tmp = std::dynamic_pointer_cast<BstIdl::FEnumerationType>(derived);
            if (tmp)
                return getSomeIpEnumBitWidthHelper(_accessor, tmp);
        }
    }

    return -1;
}

int FrancaSomeIPGeneratorExtensions::getSomeIpEnumInvalidValueHelper(
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor, const std::shared_ptr<BstIdl::FObject> &_obj)
{
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FEnumerationType>(_obj))
    {
        int invalidValue = _accessor->getSomeIpEnumInvalidValue(ptr);
        if (invalidValue < 0)
            invalidValue = getSomeIpEnumInvalidValueHelper(_accessor, ptr->getBase());
        return invalidValue;
    }

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FTypeDef>(_obj))
    {
        auto derived = ptr->getActualType()->getDerived();
        if (derived)
        {
            auto tmp = std::dynamic_pointer_cast<BstIdl::FEnumerationType>(derived);
            if (tmp)
                return getSomeIpEnumInvalidValueHelper(_accessor, tmp);
        }
    }

    return -1;
}

int FrancaSomeIPGeneratorExtensions::getSomeIpIntegerBitWidthHelper(
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor, const std::shared_ptr<BstIdl::FObject> &_obj)
{
    return _accessor->getSomeIpIntegerBitWidth(_obj);
}

int FrancaSomeIPGeneratorExtensions::getSomeIpIntegerInvalidValueHelper(
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor, const std::shared_ptr<BstIdl::FObject> &_obj)
{
    return _accessor->getSomeIpIntegerInvalidValue(_obj);
}

bool FrancaSomeIPGeneratorExtensions::hasSomeIpArrayMinLength(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                              const std::shared_ptr<BstIdl::FObject> &_object)
{
    int defaultMinLength = SOMEIP_DEFAULT_MIN_LENGTH;
    if (!_accessor->isProperOverwrite())
    {
        if (auto ptr = std::dynamic_pointer_cast<BstIdl::FTypedElement>(_object))
        {
            if (auto derived = ptr->getType()->getDerived())
            {
                defaultMinLength = getSomeIpArrayMinLengthHelper(_accessor, derived);
                if (defaultMinLength == SOMEIP_DEFAULT_MIN_LENGTH)
                {
                    if (auto newAccessor = getSpecificAccessor(_object))
                        defaultMinLength = getSomeIpArrayMinLengthHelper(newAccessor, derived);
                }
            }
        }
    }
    int minLength = getSomeIpArrayMinLengthHelper(_accessor, _object);
    if (minLength != -1 && minLength != defaultMinLength)
        return true;

    if (auto newAccessor = getSpecificAccessor(_object))
        minLength = getSomeIpArrayMinLengthHelper(newAccessor, _object);

    return minLength != -1 && minLength != defaultMinLength;
}

bool FrancaSomeIPGeneratorExtensions::hasSomeIpArrayMaxLength(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                              const std::shared_ptr<BstIdl::FObject> &_object)
{
    int defaultMaxLength = SOMEIP_DEFAULT_MAX_LENGTH;
    if (!_accessor->isProperOverwrite())
    {
        if (auto ptr = std::dynamic_pointer_cast<BstIdl::FTypedElement>(_object))
        {
            if (auto derived = ptr->getType()->getDerived())
            {
                defaultMaxLength = getSomeIpArrayMaxLengthHelper(_accessor, derived);
                if (defaultMaxLength == SOMEIP_DEFAULT_MAX_LENGTH)
                {
                    if (auto newAccessor = getSpecificAccessor(_object))
                        defaultMaxLength = getSomeIpArrayMaxLengthHelper(newAccessor, derived);
                }
            }
        }
    }
    int maxLength = getSomeIpArrayMaxLengthHelper(_accessor, _object);
    if (maxLength != -1 && maxLength != defaultMaxLength)
        return true;

    if (auto newAccessor = getSpecificAccessor(_object))
        maxLength = getSomeIpArrayMaxLengthHelper(newAccessor, _object);

    return maxLength != -1 && maxLength != defaultMaxLength;
}

bool FrancaSomeIPGeneratorExtensions::hasSomeIpArrayLengthWidth(
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor, const std::shared_ptr<BstIdl::FObject> &_object)
{
    int defaultLengthWidth = SOMEIP_DEFAULT_LENGTH_WIDTH;
    if (!_accessor->isProperOverwrite())
    {
        if (auto ptr = std::dynamic_pointer_cast<BstIdl::FTypedElement>(_object))
        {
            if (auto derived = ptr->getType()->getDerived())
            {
                defaultLengthWidth = getSomeIpArrayLengthWidthHelper(_accessor, derived);
                if (defaultLengthWidth == SOMEIP_DEFAULT_LENGTH_WIDTH)
                {
                    if (auto newAccessor = getSpecificAccessor(_object))
                        defaultLengthWidth = getSomeIpArrayLengthWidthHelper(newAccessor, derived);
                }
            }
        }
    }
    int lengthWidth = getSomeIpArrayLengthWidthHelper(_accessor, _object);
    if (lengthWidth != -1 && lengthWidth != defaultLengthWidth)
        return true;

    if (auto newAccessor = getSpecificAccessor(_object))
        lengthWidth = getSomeIpArrayLengthWidthHelper(newAccessor, _object);

    return lengthWidth != -1 && lengthWidth != defaultLengthWidth;
}

bool FrancaSomeIPGeneratorExtensions::hasSomeIpByteBufferMinLength(
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor, const std::shared_ptr<BstIdl::FObject> &_object)
{
    int defaultMinLength = SOMEIP_DEFAULT_MIN_LENGTH;
    if (!_accessor->isProperOverwrite())
    {
        if (auto ptr = std::dynamic_pointer_cast<BstIdl::FTypedElement>(_object))
        {
            if (auto derived = ptr->getType()->getDerived())
            {
                defaultMinLength = _accessor->getSomeIpByteBufferMinLength(derived);
                defaultMinLength = defaultMinLength == -1 ? SOMEIP_DEFAULT_MIN_LENGTH : defaultMinLength;
                if (defaultMinLength == SOMEIP_DEFAULT_MIN_LENGTH)
                {
                    if (auto newAccessor = getSpecificAccessor(_object))
                        defaultMinLength = newAccessor->getSomeIpByteBufferMinLength(derived);
                }
            }
        }
    }
    int minLength = _accessor->getSomeIpByteBufferMinLength(_object);
    minLength = minLength == -1 ? SOMEIP_DEFAULT_MIN_LENGTH : minLength;
    if (minLength != defaultMinLength)
        return true;

    if (auto newAccessor = getSpecificAccessor(_object))
        minLength = newAccessor->getSomeIpByteBufferMinLength(_object);

    minLength = minLength == -1 ? SOMEIP_DEFAULT_MIN_LENGTH : minLength;
    return minLength != defaultMinLength;
}

bool FrancaSomeIPGeneratorExtensions::hasSomeIpByteBufferMaxLength(
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor, const std::shared_ptr<BstIdl::FObject> &_object)
{
    int defaultMaxLength = SOMEIP_DEFAULT_MAX_LENGTH;
    if (!_accessor->isProperOverwrite())
    {
        if (auto ptr = std::dynamic_pointer_cast<BstIdl::FTypedElement>(_object))
        {
            if (auto derived = ptr->getType()->getDerived())
            {
                defaultMaxLength = _accessor->getSomeIpByteBufferMaxLength(derived);
                if (defaultMaxLength == -1 || defaultMaxLength == SOMEIP_DEFAULT_MAX_LENGTH)
                {
                    if (auto newAccessor = getSpecificAccessor(_object))
                        defaultMaxLength = newAccessor->getSomeIpByteBufferMaxLength(derived);
                }
            }
        }
    }
    int maxLength = _accessor->getSomeIpByteBufferMaxLength(_object);
    if (maxLength != -1 && maxLength != defaultMaxLength)
        return true;

    if (auto newAccessor = getSpecificAccessor(_object))
        maxLength = newAccessor->getSomeIpByteBufferMaxLength(_object);

    return maxLength != -1 && maxLength != defaultMaxLength;
}

bool FrancaSomeIPGeneratorExtensions::hasSomeIpByteBufferLengthWidth(
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor, const std::shared_ptr<BstIdl::FObject> &_object)
{
    int defaultLengthWidth = SOMEIP_DEFAULT_LENGTH_WIDTH;
    if (!_accessor->isProperOverwrite())
    {
        if (auto ptr = std::dynamic_pointer_cast<BstIdl::FTypedElement>(_object))
        {
            if (auto derived = ptr->getType()->getDerived())
            {
                defaultLengthWidth = _accessor->getSomeIpByteBufferLengthWidth(derived);
                if (defaultLengthWidth == -1 || defaultLengthWidth == SOMEIP_DEFAULT_LENGTH_WIDTH)
                {
                    if (auto newAccessor = getSpecificAccessor(_object))
                        defaultLengthWidth = newAccessor->getSomeIpByteBufferLengthWidth(derived);
                }
            }
        }
    }
    int lengthWidth = _accessor->getSomeIpByteBufferLengthWidth(_object);
    if (lengthWidth != -1 && lengthWidth != defaultLengthWidth)
        return true;

    if (auto newAccessor = getSpecificAccessor(_object))
        lengthWidth = newAccessor->getSomeIpByteBufferLengthWidth(_object);

    return lengthWidth != -1 && lengthWidth != defaultLengthWidth;
}

bool FrancaSomeIPGeneratorExtensions::isDefaultWidth(int _width)
{
    return (_width == 0 || _width == 8 || _width == 16 || _width == 32 || _width == 64);
}

bool FrancaSomeIPGeneratorExtensions::hasSomeIpIntegerBitWidth(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                               const std::shared_ptr<BstIdl::FObject> &_object)
{
    int width = getSomeIpIntegerBitWidthHelper(_accessor, _object);
    if (width != -1 && !isDefaultWidth(width))
        return true;

    auto newAccessor = getSpecificAccessor(_object);
    if (newAccessor)
    {
        width = getSomeIpIntegerBitWidthHelper(newAccessor, _object);
        if (width != -1 && !isDefaultWidth(width))
            return true;
    }
    return false;
}

bool FrancaSomeIPGeneratorExtensions::hasSomeIpIntegerInvalidValue(
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor, const std::shared_ptr<BstIdl::FObject> &_object)
{
    int invalidValue = getSomeIpIntegerInvalidValueHelper(_accessor, _object);
    if (invalidValue != -1)
        return true;

    auto newAccessor = getSpecificAccessor(_object);
    if (newAccessor)
    {
        invalidValue = getSomeIpIntegerInvalidValueHelper(newAccessor, _object);
        if (invalidValue != -1)
            return true;
    }
    return false;
}

bool FrancaSomeIPGeneratorExtensions::hasSomeIpStringLength(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                            const std::shared_ptr<BstIdl::FObject> &_object)
{
    auto length = _accessor->getSomeIpStringLength(_object);
    if (length != -1 && length != SOMEIP_DEFAULT_MIN_LENGTH)
    {
        return true;
    }
    auto newAccessor = getSpecificAccessor(_object);
    if (newAccessor)
    {
        length = newAccessor->getSomeIpStringLength(_object);
        return length != -1 && length != SOMEIP_DEFAULT_MIN_LENGTH;
    }
    return false;
}

bool FrancaSomeIPGeneratorExtensions::hasSomeIpStringLengthWidth(
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor, const std::shared_ptr<BstIdl::FObject> &_object)
{
    auto lengthWidth = _accessor->getSomeIpStringLengthWidth(_object);
    if (lengthWidth != -1 && lengthWidth != SOMEIP_DEFAULT_LENGTH_WIDTH)
    {
        return true;
    }
    auto newAccessor = getSpecificAccessor(_object);
    if (newAccessor)
    {
        lengthWidth = newAccessor->getSomeIpStringLengthWidth(_object);
        return lengthWidth != -1 && lengthWidth != SOMEIP_DEFAULT_LENGTH_WIDTH;
    }
    return false;
}

bool FrancaSomeIPGeneratorExtensions::hasSomeIpStructLengthWidth(
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor, const std::shared_ptr<BstIdl::FObject> &_object)
{
    int defaultLengthWidth = SOMEIP_DEFAULT_STRUCT_LENGTH_WIDTH;
    if (!_accessor->isProperOverwrite())
    {
        if (auto ptr = std::dynamic_pointer_cast<BstIdl::FTypedElement>(_object))
        {
            if (auto derived = ptr->getType()->getDerived())
            {
                defaultLengthWidth = getSomeIpStructLengthWidthHelper(_accessor, derived);
                if (defaultLengthWidth == -1 || defaultLengthWidth == SOMEIP_DEFAULT_STRUCT_LENGTH_WIDTH)
                {
                    if (auto newAccessor = getSpecificAccessor(_object))
                        defaultLengthWidth = getSomeIpStructLengthWidthHelper(newAccessor, derived);
                }
            }
        }
    }
    int lengthWidth = getSomeIpStructLengthWidthHelper(_accessor, _object);
    if (lengthWidth != -1 && lengthWidth != defaultLengthWidth)
        return true;

    if (auto newAccessor = getSpecificAccessor(_object))
        lengthWidth = getSomeIpStructLengthWidthHelper(newAccessor, _object);

    return lengthWidth != -1 && lengthWidth != defaultLengthWidth;
}

bool FrancaSomeIPGeneratorExtensions::hasSomeIpStringEncoding(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                              const std::shared_ptr<BstIdl::FObject> &_object)
{
    auto encoding = _accessor->getSomeIpStringEncoding(_object);
    if (encoding != SomeIpStringEncoding::Invalid && encoding != SOMEIP_DEFAULT_STRING_ENCODING)
    {
        return true;
    }
    auto newAccessor = getSpecificAccessor(_object);
    if (newAccessor)
    {
        encoding = newAccessor->getSomeIpStringEncoding(_object);
        return encoding != SomeIpStringEncoding::Invalid && encoding != SOMEIP_DEFAULT_STRING_ENCODING;
    }
    return false;
}

bool FrancaSomeIPGeneratorExtensions::hasSomeIpMapMinLength(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                            const std::shared_ptr<BstIdl::FObject> &_object)
{
    int defaultMinLength = SOMEIP_DEFAULT_MIN_LENGTH;
    if (!_accessor->isProperOverwrite())
    {
        if (auto ptr = std::dynamic_pointer_cast<BstIdl::FTypedElement>(_object))
        {
            if (auto derived = ptr->getType()->getDerived())
            {
                defaultMinLength = getSomeIpMapMinLengthHelper(_accessor, derived);
                if (defaultMinLength == SOMEIP_DEFAULT_MIN_LENGTH)
                {
                    if (auto newAccessor = getSpecificAccessor(_object))
                        defaultMinLength = getSomeIpMapMinLengthHelper(newAccessor, derived);
                }
            }
        }
    }
    int minLength = getSomeIpMapMinLengthHelper(_accessor, _object);
    if (minLength != -1 && minLength != defaultMinLength)
        return true;

    if (auto newAccessor = getSpecificAccessor(_object))
        minLength = getSomeIpMapMinLengthHelper(newAccessor, _object);

    return minLength != -1 && minLength != defaultMinLength;
}

bool FrancaSomeIPGeneratorExtensions::hasSomeIpMapMaxLength(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                            const std::shared_ptr<BstIdl::FObject> &_object)
{
    int defaultMaxLength = SOMEIP_DEFAULT_MAX_LENGTH;
    if (!_accessor->isProperOverwrite())
    {
        if (auto ptr = std::dynamic_pointer_cast<BstIdl::FTypedElement>(_object))
        {
            if (auto derived = ptr->getType()->getDerived())
            {
                defaultMaxLength = getSomeIpMapMaxLengthHelper(_accessor, derived);
                if (defaultMaxLength == SOMEIP_DEFAULT_MAX_LENGTH)
                {
                    if (auto newAccessor = getSpecificAccessor(_object))
                        defaultMaxLength = getSomeIpMapMaxLengthHelper(newAccessor, derived);
                }
            }
        }
    }
    int maxLength = getSomeIpMapMaxLengthHelper(_accessor, _object);
    if (maxLength != defaultMaxLength)
        return true;

    if (auto newAccessor = getSpecificAccessor(_object))
        maxLength = getSomeIpMapMaxLengthHelper(newAccessor, _object);

    return maxLength != defaultMaxLength;
}

bool FrancaSomeIPGeneratorExtensions::hasSomeIpMapLengthWidth(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                              const std::shared_ptr<BstIdl::FObject> &_object)
{
    int defaultLengthWidth = SOMEIP_DEFAULT_LENGTH_WIDTH;
    if (!_accessor->isProperOverwrite())
    {
        if (auto ptr = std::dynamic_pointer_cast<BstIdl::FTypedElement>(_object))
        {
            if (auto derived = ptr->getType()->getDerived())
            {
                defaultLengthWidth = getSomeIpMapLengthWidthHelper(_accessor, derived);
                if (defaultLengthWidth == SOMEIP_DEFAULT_LENGTH_WIDTH)
                {
                    if (auto newAccessor = getSpecificAccessor(_object))
                        defaultLengthWidth = getSomeIpMapLengthWidthHelper(newAccessor, derived);
                }
            }
        }
    }
    int lengthWidth = getSomeIpMapLengthWidthHelper(_accessor, _object);
    if (lengthWidth != defaultLengthWidth)
        return true;

    if (auto newAccessor = getSpecificAccessor(_object))
        lengthWidth = getSomeIpMapLengthWidthHelper(newAccessor, _object);

    return lengthWidth != defaultLengthWidth;
}

bool FrancaSomeIPGeneratorExtensions::hasSomeIpUnionLengthWidth(
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor, const std::shared_ptr<BstIdl::FObject> &_object)
{
    int defaultLengthWidth = SOMEIP_DEFAULT_LENGTH_WIDTH;
    if (!_accessor->isProperOverwrite())
    {
        if (auto ptr = std::dynamic_pointer_cast<BstIdl::FTypedElement>(_object))
        {
            if (auto derived = ptr->getType()->getDerived())
            {
                defaultLengthWidth = getSomeIpUnionLengthWidthHelper(_accessor, derived);
                if (defaultLengthWidth == SOMEIP_DEFAULT_LENGTH_WIDTH)
                {
                    if (auto newAccessor = getSpecificAccessor(_object))
                        defaultLengthWidth = getSomeIpUnionLengthWidthHelper(newAccessor, derived);
                }
            }
        }
    }
    int lengthWidth = getSomeIpUnionLengthWidthHelper(_accessor, _object);
    if (lengthWidth != defaultLengthWidth)
        return true;

    if (auto newAccessor = getSpecificAccessor(_object))
        lengthWidth = getSomeIpUnionLengthWidthHelper(newAccessor, _object);

    return lengthWidth != defaultLengthWidth;
}

bool FrancaSomeIPGeneratorExtensions::hasSomeIpUnionTypeWidth(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                              const std::shared_ptr<BstIdl::FObject> &_object)
{
    int defaultTypeWidth = SOMEIP_DEFAULT_UNION_TYPE_WIDTH;
    if (!_accessor->isProperOverwrite())
    {
        if (auto ptr = std::dynamic_pointer_cast<BstIdl::FTypedElement>(_object))
        {
            if (auto derived = ptr->getType()->getDerived())
            {
                defaultTypeWidth = getSomeIpUnionTypeWidthHelper(_accessor, derived);
                if (defaultTypeWidth == SOMEIP_DEFAULT_UNION_TYPE_WIDTH)
                {
                    if (auto newAccessor = getSpecificAccessor(_object))
                        defaultTypeWidth = getSomeIpUnionTypeWidthHelper(newAccessor, derived);
                }
            }
        }
    }
    int typeWidth = getSomeIpUnionTypeWidthHelper(_accessor, _object);
    if (typeWidth != defaultTypeWidth)
        return true;

    if (auto newAccessor = getSpecificAccessor(_object))
        typeWidth = getSomeIpUnionTypeWidthHelper(newAccessor, _object);

    return typeWidth != defaultTypeWidth;
}

bool FrancaSomeIPGeneratorExtensions::hasSomeIpUnionDefaultOrder(
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor, const std::shared_ptr<BstIdl::FObject> &_object)
{
    int defaultDefaultOrder = SOMEIP_DEFAULT_UNION_DEFAULT_ORDER;
    if (!_accessor->isProperOverwrite())
    {
        if (auto ptr = std::dynamic_pointer_cast<BstIdl::FTypedElement>(_object))
        {
            if (auto derived = ptr->getType()->getDerived())
            {
                defaultDefaultOrder = getSomeIpUnionDefaultOrderHelper(_accessor, derived);
                if (defaultDefaultOrder == SOMEIP_DEFAULT_UNION_DEFAULT_ORDER)
                {
                    if (auto newAccessor = getSpecificAccessor(_object))
                        defaultDefaultOrder = getSomeIpUnionDefaultOrderHelper(newAccessor, derived);
                }
            }
        }
    }
    int defaultOrder = getSomeIpUnionDefaultOrderHelper(_accessor, _object);
    if (defaultOrder != defaultDefaultOrder)
        return true;

    if (auto newAccessor = getSpecificAccessor(_object))
        defaultOrder = getSomeIpUnionDefaultOrderHelper(newAccessor, _object);

    return defaultOrder != defaultDefaultOrder;
}

bool FrancaSomeIPGeneratorExtensions::hasSomeIpUnionMaxLength(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                              const std::shared_ptr<BstIdl::FObject> &_object)
{
    int defaultMaxLength = SOMEIP_DEFAULT_MAX_LENGTH;
    if (!_accessor->isProperOverwrite())
    {
        if (auto ptr = std::dynamic_pointer_cast<BstIdl::FTypedElement>(_object))
        {
            if (auto derived = ptr->getType()->getDerived())
            {
                defaultMaxLength = getSomeIpUnionMaxLengthHelper(_accessor, derived);
                if (defaultMaxLength == SOMEIP_DEFAULT_MAX_LENGTH)
                {
                    if (auto newAccessor = getSpecificAccessor(_object))
                        defaultMaxLength = getSomeIpUnionMaxLengthHelper(newAccessor, derived);
                }
            }
        }
    }
    int maxLength = getSomeIpUnionMaxLengthHelper(_accessor, _object);
    if (maxLength != defaultMaxLength)
        return true;

    if (auto newAccessor = getSpecificAccessor(_object))
        maxLength = getSomeIpUnionMaxLengthHelper(newAccessor, _object);

    return maxLength != defaultMaxLength;
}

bool FrancaSomeIPGeneratorExtensions::hasSomeIpEnumWidth(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                         const std::shared_ptr<BstIdl::FObject> &_object)
{
    int defaultEnumWidth = SOMEIP_DEFAULT_ENUM_LENGTH_WIDTH;
    if (!_accessor->isProperOverwrite())
    {
        if (auto ptr = std::dynamic_pointer_cast<BstIdl::FTypedElement>(_object))
        {
            if (auto derived = ptr->getType()->getDerived())
            {
                defaultEnumWidth = getSomeIpEnumWidthHelper(_accessor, derived);
                if (defaultEnumWidth == SOMEIP_DEFAULT_ENUM_LENGTH_WIDTH)
                {
                    if (auto newAccessor = getSpecificAccessor(_object))
                        defaultEnumWidth = getSomeIpEnumWidthHelper(newAccessor, derived);
                }
            }
        }
    }
    int lengthWidth = getSomeIpEnumWidthHelper(_accessor, _object);
    if (lengthWidth != -1 && lengthWidth != defaultEnumWidth)
        return true;

    if (auto newAccessor = getSpecificAccessor(_object))
        lengthWidth = getSomeIpEnumWidthHelper(newAccessor, _object);

    return lengthWidth != -1 && lengthWidth != defaultEnumWidth;
}

bool FrancaSomeIPGeneratorExtensions::hasSomeIpEnumBitWidth(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                            const std::shared_ptr<BstIdl::FObject> &_object)
{
    int width = getSomeIpEnumBitWidthHelper(_accessor, _object);
    if (width != -1 && !isDefaultWidth(width))
        return true;
    auto newAccessor = getSpecificAccessor(_object);
    if (newAccessor)
    {
        width = getSomeIpEnumBitWidthHelper(newAccessor, _object);
        if (width != -1 && !isDefaultWidth(width))
            return true;
    }

    return false;
}

bool FrancaSomeIPGeneratorExtensions::hasSomeIpEnumInvalidValue(
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor, const std::shared_ptr<BstIdl::FObject> &_object)
{
    int invalidValue = getSomeIpEnumInvalidValueHelper(_accessor, _object);
    if (invalidValue != -1)
        return true;

    auto newAccessor = getSpecificAccessor(_object);
    if (newAccessor)
    {
        invalidValue = getSomeIpEnumInvalidValueHelper(newAccessor, _object);
        if (invalidValue != -1)
            return true;
    }
    return false;
}

bool FrancaSomeIPGeneratorExtensions::hasDeployment(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                    const std::shared_ptr<BstIdl::FTypedElement> &_element)
{
    if (_accessor == nullptr)
        return false;
    bool ret = false;
    ret = ret && hasSomeIpArrayMinLength(_accessor, _element);
    ret = ret && hasSomeIpArrayMaxLength(_accessor, _element);
    ret = ret && hasSomeIpArrayLengthWidth(_accessor, _element);
    ret = ret && hasSomeIpMapMinLength(_accessor, _element);
    ret = ret && hasSomeIpMapMaxLength(_accessor, _element);
    ret = ret && hasSomeIpMapLengthWidth(_accessor, _element);
    ret = ret && hasSomeIpByteBufferMinLength(_accessor, _element);
    ret = ret && hasSomeIpByteBufferMaxLength(_accessor, _element);
    ret = ret && hasSomeIpStringLength(_accessor, _element);
    ret = ret && hasSomeIpStringLengthWidth(_accessor, _element);
    ret = ret && hasSomeIpStringEncoding(_accessor, _element);
    ret = ret && hasSomeIpStructLengthWidth(_accessor, _element);
    ret = ret && hasSomeIpUnionLengthWidth(_accessor, _element);
    ret = ret && hasSomeIpUnionTypeWidth(_accessor, _element);
    ret = ret && hasSomeIpUnionDefaultOrder(_accessor, _element);
    ret = ret && hasSomeIpUnionMaxLength(_accessor, _element);
    ret = ret && hasSomeIpEnumWidth(_accessor, _element);
    ret = ret && hasSomeIpEnumBitWidth(_accessor, _element);
    ret = ret && hasSomeIpEnumInvalidValue(_accessor, _element);
    ret = ret && hasSomeIpIntegerBitWidth(_accessor, _element);
    ret = ret && hasSomeIpIntegerInvalidValue(_accessor, _element);
    if (ret)
    {
        return true;
    }
    return hasDeployment(_accessor, _element->getType());
}

bool FrancaSomeIPGeneratorExtensions::hasDeployment(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                    const std::shared_ptr<BstIdl::FArrayType> &_array)
{
    if (hasSomeIpArrayMinLength(_accessor, _array) || hasSomeIpArrayMaxLength(_accessor, _array) ||
        hasSomeIpArrayLengthWidth(_accessor, _array))
        return true;

    auto overwriteAccessor = _accessor->getOverwriteAccessor(_array);
    if (hasDeployment(overwriteAccessor, _array->getElementType()))
        return true;
    return false;
}

bool FrancaSomeIPGeneratorExtensions::hasDeployment(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                    const std::shared_ptr<BstIdl::FMapType> &_map)
{
    if (hasSomeIpMapMinLength(_accessor, _map) || hasSomeIpMapMaxLength(_accessor, _map) ||
        hasSomeIpMapLengthWidth(_accessor, _map))
        return true;
    if (hasDeployment(_accessor, _map->getKeyType()) || hasDeployment(_accessor, _map->getValueType()))
        return true;
    return false;
}

bool FrancaSomeIPGeneratorExtensions::hasDeployment(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                    const std::shared_ptr<BstIdl::FEnumerationType> &_enum)
{
    if (hasSomeIpEnumWidth(_accessor, _enum) || hasSomeIpEnumBitWidth(_accessor, _enum) ||
        hasSomeIpEnumInvalidValue(_accessor, _enum))
        return true;

    if (auto base = _enum->getBase())
    {
        if (auto baseAccessor =
                getSomeIpAccessor(std::dynamic_pointer_cast<BstIdl::FTypeCollection>(base->getContainer())))
            return hasDeployment(baseAccessor, base);
    }
    return false;
}

bool FrancaSomeIPGeneratorExtensions::hasDeployment(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                    const std::shared_ptr<BstIdl::FStructType> &_struct)
{
    if (hasSomeIpStructLengthWidth(_accessor, _struct))
        return true;
    for (auto element : _struct->getElements())
    {
        auto overwriteAccessor = _accessor->getOverwriteAccessor(element);
        if (hasDeployment(overwriteAccessor, element))
        {
            return true;
        }
    }
    if (_struct->getBase())
    {
        return hasDeployment(_accessor, _struct->getBase());
    }
    return false;
}

bool FrancaSomeIPGeneratorExtensions::hasDeployment(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                    const std::shared_ptr<BstIdl::FUnionType> &_union)
{
    if (hasSomeIpUnionDefaultOrder(_accessor, _union) || hasSomeIpUnionLengthWidth(_accessor, _union) ||
        hasSomeIpUnionTypeWidth(_accessor, _union) || hasSomeIpUnionMaxLength(_accessor, _union))
    {
        return true;
    }

    for (auto element : _union->getElements())
    {
        auto overwriteAccessor = _accessor->getOverwriteAccessor(element);
        if (hasDeployment(overwriteAccessor, element))
        {
            return true;
        }
    }
    return false;
}

bool FrancaSomeIPGeneratorExtensions::hasDeployment(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                    const std::shared_ptr<BstIdl::FTypeDef> &_typeDef)
{
    return hasDeployment(_accessor, _typeDef->getActualType());
}

bool FrancaSomeIPGeneratorExtensions::hasDeployment(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                    const std::shared_ptr<BstIdl::FIntegerInterval> &_type)
{
    return false;
}

bool FrancaSomeIPGeneratorExtensions::hasDeployment(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                    const std::shared_ptr<BstIdl::FBasicTypeId> &_type)
{
    return false;
}

bool FrancaSomeIPGeneratorExtensions::hasDeployment(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                    const std::shared_ptr<BstIdl::FType> &_type)
{
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FArrayType>(_type))
        return hasDeployment(_accessor, ptr);
    else if (auto ptr = std::dynamic_pointer_cast<BstIdl::FEnumerationType>(_type))
        return hasDeployment(_accessor, ptr);
    else if (auto ptr = std::dynamic_pointer_cast<BstIdl::FMapType>(_type))
        return hasDeployment(_accessor, ptr);
    else if (auto ptr = std::dynamic_pointer_cast<BstIdl::FTypeDef>(_type))
        return hasDeployment(_accessor, ptr);
    else if (auto ptr = std::dynamic_pointer_cast<BstIdl::FIntegerInterval>(_type))
        return hasDeployment(_accessor, ptr);
    else if (auto ptr = std::dynamic_pointer_cast<BstIdl::FBasicTypeId>(_type))
        return hasDeployment(_accessor, ptr);
    else if (auto ptr = std::dynamic_pointer_cast<BstIdl::FStructType>(_type))
        return hasDeployment(_accessor, ptr);
    else if (auto ptr = std::dynamic_pointer_cast<BstIdl::FUnionType>(_type))
        return hasDeployment(_accessor, ptr);
    else
        return false;
}

bool FrancaSomeIPGeneratorExtensions::hasDeployment(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                    const std::shared_ptr<BstIdl::FTypeRef> &_type)
{
    if (_type->getDerived())
        return hasDeployment(_accessor, _type->getDerived());

    if (_type->getPredefined())
        return hasDeployment(_accessor, _type->getPredefined());

    if (_type->getInterval())
        return hasDeployment(_accessor, _type->getInterval());

    return false;
}

bool FrancaSomeIPGeneratorExtensions::isFull(const std::shared_ptr<BstIdl::FBasicTypeId> &_type, int _width)
{
    if (_type == nullptr)
        return false;

    if (_type->getValue() == BstIdl::FBasicTypeId::INT8 || _type->getValue() == BstIdl::FBasicTypeId::INT8)
        return _width == 8;

    if (_type->getValue() == BstIdl::FBasicTypeId::INT16 || _type->getValue() == BstIdl::FBasicTypeId::INT16)
        return _width == 16;

    if (_type->getValue() == BstIdl::FBasicTypeId::INT32 || _type->getValue() == BstIdl::FBasicTypeId::INT32)
        return _width == 32;

    if (_type->getValue() == BstIdl::FBasicTypeId::INT64 || _type->getValue() == BstIdl::FBasicTypeId::INT64)
        return _width == 64;

    return false;
}

bool FrancaSomeIPGeneratorExtensions::hasSpecificDeployment(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                            const std::shared_ptr<BstIdl::FTypedElement> &_element)
{
    std::shared_ptr<SomeipPropertyAccessor> itsBaseAccessor = nullptr;
    if (_accessor->isProperOverwrite())
        itsBaseAccessor = _accessor->getParent();

    auto itsSpecificArrayMinLength = getSomeIpArrayMinLengthHelper(_accessor, _element);
    auto itsArrayMinLength = SOMEIP_DEFAULT_MIN_LENGTH;
    if (itsBaseAccessor)
        itsArrayMinLength = getSomeIpArrayMinLengthHelper(_accessor, _element);

    if (itsSpecificArrayMinLength != itsArrayMinLength && itsSpecificArrayMinLength != SOMEIP_DEFAULT_MIN_LENGTH)
        return true;

    auto itsSpecificArrayMaxLength = getSomeIpArrayMaxLengthHelper(_accessor, _element);
    auto itsArrayMaxLength = SOMEIP_DEFAULT_MAX_LENGTH;
    if (itsBaseAccessor)
        itsArrayMaxLength = getSomeIpArrayMaxLengthHelper(_accessor, _element);

    if (itsSpecificArrayMaxLength != itsArrayMaxLength && itsSpecificArrayMaxLength != SOMEIP_DEFAULT_MAX_LENGTH)
        return true;

    auto itsSpecificArrayLengthWidth = getSomeIpArrayLengthWidthHelper(_accessor, _element);
    auto itsArrayLengthWidth = SOMEIP_DEFAULT_LENGTH_WIDTH;
    if (itsBaseAccessor)
        itsArrayLengthWidth = getSomeIpArrayLengthWidthHelper(_accessor, _element);
    if (itsSpecificArrayLengthWidth != itsArrayLengthWidth &&
        itsSpecificArrayLengthWidth != SOMEIP_DEFAULT_LENGTH_WIDTH)
        return true;

    auto itsSpecificMapMinLength = getSomeIpMapMinLengthHelper(_accessor, _element);
    auto itsMapMinLength = SOMEIP_DEFAULT_MIN_LENGTH;
    if (itsBaseAccessor)
        itsMapMinLength = getSomeIpMapMinLengthHelper(_accessor, _element);

    if (itsSpecificMapMinLength != -1 && itsSpecificMapMinLength != itsMapMinLength &&
        itsSpecificMapMinLength != SOMEIP_DEFAULT_MIN_LENGTH)
        return true;

    auto itsSpecificMapMaxLength = getSomeIpMapMaxLengthHelper(_accessor, _element);
    auto itsMapMaxLength = SOMEIP_DEFAULT_MAX_LENGTH;
    if (itsBaseAccessor)
        itsMapMaxLength = getSomeIpMapMaxLengthHelper(_accessor, _element);

    if (itsSpecificMapMaxLength != itsMapMaxLength && itsSpecificMapMaxLength != SOMEIP_DEFAULT_MAX_LENGTH)
        return true;

    auto itsSpecificMapLengthWidth = getSomeIpMapLengthWidthHelper(_accessor, _element);
    auto itsMapLengthWidth = SOMEIP_DEFAULT_LENGTH_WIDTH;
    if (itsBaseAccessor)
        itsMapLengthWidth = getSomeIpMapLengthWidthHelper(_accessor, _element);

    if (itsSpecificMapLengthWidth != itsMapLengthWidth && itsSpecificMapLengthWidth != SOMEIP_DEFAULT_LENGTH_WIDTH)
        return true;

    auto itsSpecificByteBufferMinLength = _accessor->getSomeIpByteBufferMinLength(_element);
    if (itsSpecificByteBufferMinLength == -1)
        itsSpecificByteBufferMinLength = SOMEIP_DEFAULT_MIN_LENGTH;
    auto itsByteBufferMinLength = SOMEIP_DEFAULT_MIN_LENGTH;
    if (itsBaseAccessor)
        itsByteBufferMinLength = _accessor->getSomeIpByteBufferMinLength(_element);

    if (itsSpecificByteBufferMinLength != itsByteBufferMinLength &&
        itsSpecificByteBufferMinLength != SOMEIP_DEFAULT_MIN_LENGTH)
        return true;

    auto itsSpecificByteBufferMaxLength = _accessor->getSomeIpByteBufferMaxLength(_element);
    if (itsSpecificByteBufferMaxLength == -1)
        itsSpecificByteBufferMaxLength = SOMEIP_DEFAULT_MAX_LENGTH;
    auto itsByteBufferMaxLength = SOMEIP_DEFAULT_MAX_LENGTH;
    if (itsBaseAccessor)
        itsByteBufferMaxLength = _accessor->getSomeIpByteBufferMaxLength(_element);

    if (itsSpecificByteBufferMaxLength != itsByteBufferMaxLength &&
        itsSpecificByteBufferMaxLength != SOMEIP_DEFAULT_MAX_LENGTH)
        return true;

    auto itsSpecificByteBufferLengthWidth = _accessor->getSomeIpByteBufferLengthWidth(_element);
    if (itsSpecificByteBufferLengthWidth == -1)
        itsSpecificByteBufferLengthWidth = SOMEIP_DEFAULT_LENGTH_WIDTH;
    auto itsByteBufferLengthWidth = SOMEIP_DEFAULT_LENGTH_WIDTH;
    if (itsBaseAccessor)
        itsByteBufferLengthWidth = itsBaseAccessor->getSomeIpByteBufferLengthWidth(_element);

    if (itsSpecificByteBufferLengthWidth != itsByteBufferLengthWidth &&
        itsSpecificByteBufferLengthWidth != SOMEIP_DEFAULT_LENGTH_WIDTH)
        return true;

    auto itsSpecificStringLength = _accessor->getSomeIpStringLength(_element);
    if (itsSpecificStringLength == -1)
        itsSpecificStringLength = SOMEIP_DEFAULT_STRING_LENGTH;
    auto itsStringLength = SOMEIP_DEFAULT_STRING_LENGTH;
    if (itsBaseAccessor)
        itsStringLength = itsBaseAccessor->getSomeIpStringLength(_element);

    if (itsSpecificStringLength != itsStringLength && itsSpecificStringLength != SOMEIP_DEFAULT_STRING_LENGTH)
        return true;

    auto itsSpecificStringLengthWidth = _accessor->getSomeIpStringLengthWidth(_element);
    if (itsSpecificStringLengthWidth == -1)
        itsSpecificStringLengthWidth = SOMEIP_DEFAULT_LENGTH_WIDTH;
    auto itsStringLengthWidth = SOMEIP_DEFAULT_LENGTH_WIDTH;
    if (itsBaseAccessor)
        itsStringLengthWidth = itsBaseAccessor->getSomeIpStringLengthWidth(_element);

    if (itsSpecificStringLengthWidth != itsStringLengthWidth &&
        itsSpecificStringLengthWidth != SOMEIP_DEFAULT_LENGTH_WIDTH)
        return true;

    auto itsSpecificStringEncoding = _accessor->getSomeIpStringEncoding(_element);
    if (itsSpecificStringEncoding == SomeIpStringEncoding::Invalid)
        itsSpecificStringEncoding = SOMEIP_DEFAULT_STRING_ENCODING;
    auto itsStringEncoding = SOMEIP_DEFAULT_STRING_ENCODING;
    if (itsBaseAccessor)
        itsStringEncoding = itsBaseAccessor->getSomeIpStringEncoding(_element);

    if (itsSpecificStringEncoding != itsStringEncoding && itsSpecificStringEncoding != SOMEIP_DEFAULT_STRING_ENCODING)
        return true;

    auto itsSpecificStructLengthWidth = getSomeIpStructLengthWidthHelper(_accessor, _element);
    auto itsStructLengthWidth = SOMEIP_DEFAULT_STRUCT_LENGTH_WIDTH;
    if (itsBaseAccessor)
        itsStructLengthWidth = getSomeIpStructLengthWidthHelper(_accessor, _element);

    if (itsSpecificStructLengthWidth != itsStructLengthWidth &&
        itsSpecificStructLengthWidth != SOMEIP_DEFAULT_STRUCT_LENGTH_WIDTH)
        return true;

    auto itsSpecificUnionLengthWidth = getSomeIpUnionLengthWidthHelper(_accessor, _element);
    auto itsUnionLengthWidth = SOMEIP_DEFAULT_LENGTH_WIDTH;
    if (itsBaseAccessor)
        itsUnionLengthWidth = getSomeIpUnionLengthWidthHelper(_accessor, _element);

    if (itsSpecificUnionLengthWidth != itsUnionLengthWidth &&
        itsSpecificUnionLengthWidth != SOMEIP_DEFAULT_LENGTH_WIDTH)
        return true;

    auto itsSpecificUnionTypeWidth = getSomeIpUnionTypeWidthHelper(_accessor, _element);
    auto itsUnionTypeWidth = SOMEIP_DEFAULT_UNION_TYPE_WIDTH;
    if (itsBaseAccessor)
        itsUnionTypeWidth = getSomeIpUnionTypeWidthHelper(_accessor, _element);

    if (itsSpecificUnionTypeWidth != itsUnionTypeWidth && itsSpecificUnionTypeWidth != SOMEIP_DEFAULT_UNION_TYPE_WIDTH)
        return true;

    auto itsSpecificUnionDefaultOrder = getSomeIpUnionDefaultOrderHelper(_accessor, _element);
    auto itsUnionDefaultOrder = SOMEIP_DEFAULT_UNION_DEFAULT_ORDER;
    if (itsBaseAccessor)
        itsUnionDefaultOrder = getSomeIpUnionDefaultOrderHelper(_accessor, _element);

    if (itsSpecificUnionDefaultOrder != itsUnionDefaultOrder &&
        itsSpecificUnionDefaultOrder != SOMEIP_DEFAULT_UNION_DEFAULT_ORDER)
        return true;

    auto itsSpecificUnionMaxLength = getSomeIpUnionMaxLengthHelper(_accessor, _element);
    auto itsUnionMaxLength = SOMEIP_DEFAULT_MAX_LENGTH;
    if (itsBaseAccessor)
        itsUnionMaxLength = getSomeIpUnionMaxLengthHelper(_accessor, _element);

    if (itsSpecificUnionMaxLength != itsUnionMaxLength && itsSpecificUnionMaxLength != SOMEIP_DEFAULT_MAX_LENGTH)
        return true;

    auto itsSpecificEnumWidth = getSomeIpEnumWidthHelper(_accessor, _element);
    auto itsEnumWidth = SOMEIP_DEFAULT_ENUM_WIDTH;
    if (itsBaseAccessor)
        itsEnumWidth = getSomeIpEnumWidthHelper(_accessor, _element);

    if (itsSpecificEnumWidth != -1 && itsSpecificEnumWidth != itsEnumWidth &&
        itsSpecificEnumWidth != SOMEIP_DEFAULT_ENUM_WIDTH)
        return true;

    auto itsSpecificEnumBitWidth = getSomeIpEnumBitWidthHelper(_accessor, _element);
    auto itsEnumBitWidth = 0;
    itsEnumBitWidth = (itsEnumWidth << 3);

    if (itsBaseAccessor)
        itsEnumBitWidth = getSomeIpEnumBitWidthHelper(_accessor, _element);

    if (itsSpecificEnumBitWidth != -1 && itsSpecificEnumBitWidth != itsEnumBitWidth &&
        itsSpecificEnumBitWidth != (itsEnumWidth << 3))
        return true;

    auto itsSpecificEnumInvalidValue = getSomeIpEnumInvalidValueHelper(_accessor, _element);
    auto itsEnumInvalidValue = 0;
    if (itsBaseAccessor)
        itsEnumInvalidValue = getSomeIpEnumInvalidValueHelper(_accessor, _element);

    if (itsSpecificEnumInvalidValue != -1 && itsSpecificEnumInvalidValue != itsEnumInvalidValue)
        return true;

    auto itsSpecificIntegerBitWidth = getSomeIpIntegerBitWidthHelper(_accessor, _element);
    auto itsIntegerBitWidth = 0;
    if (itsBaseAccessor)
        itsIntegerBitWidth = getSomeIpIntegerBitWidthHelper(_accessor, _element);

    if (itsSpecificIntegerBitWidth != -1 && itsSpecificIntegerBitWidth != itsIntegerBitWidth)
        return !isFull(_element->getType()->getPredefined(), itsSpecificIntegerBitWidth);

    auto itsSpecificIntegerInvalidValue = getSomeIpIntegerInvalidValueHelper(_accessor, _element);
    auto itsIntegerInvalidValue = 0;
    if (itsBaseAccessor)
        itsIntegerInvalidValue = getSomeIpIntegerInvalidValueHelper(_accessor, _element);

    if (itsSpecificIntegerInvalidValue != -1 && itsSpecificIntegerInvalidValue != itsIntegerInvalidValue)
        return true;

    // also check for overwrites
    if (_accessor->isProperOverwrite())
        return true;

    return false;
}

bool FrancaSomeIPGeneratorExtensions::hasNonArrayDeployment(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                            const std::shared_ptr<BstIdl::FTypedElement> &_attribute)
{
    if (_attribute->getType()->getDerived() &&
        std::dynamic_pointer_cast<BstIdl::FMapType>(_attribute->getType()->getDerived()))
    {
        if (hasSomeIpMapMinLength(_accessor, _attribute))
        {
            return true;
        }
        if (hasSomeIpMapMaxLength(_accessor, _attribute))
        {
            return true;
        }
        if (hasSomeIpMapLengthWidth(_accessor, _attribute))
        {
            return true;
        }
    }

    if (_attribute->getType()->getPredefined() &&
        _attribute->getType()->getPredefined()->getValue() == BstIdl::FBasicTypeId::BYTE_BUFFER)
    {
        if (hasSomeIpByteBufferMinLength(_accessor, _attribute))
        {
            return true;
        }
        if (hasSomeIpByteBufferMaxLength(_accessor, _attribute))
        {
            return true;
        }
    }

    if (_attribute->getType()->getPredefined() &&
        _attribute->getType()->getPredefined()->getValue() == BstIdl::FBasicTypeId::STRING)
    {
        if (hasSomeIpStringLength(_accessor, _attribute))
        {
            return true;
        }
        if (hasSomeIpStringLengthWidth(_accessor, _attribute))
        {
            return true;
        }
        if (hasSomeIpStringEncoding(_accessor, _attribute))
        {
            return true;
        }
    }

    if (_attribute->getType()->getDerived() &&
        std::dynamic_pointer_cast<BstIdl::FStructType>(_attribute->getType()->getDerived()))
    {
        if (hasSomeIpStructLengthWidth(_accessor, _attribute))
        {
            return true;
        }
        auto tmp = std::dynamic_pointer_cast<BstIdl::FStructType>(_attribute->getType()->getDerived());
        if (_accessor->isProperOverwrite())
        {
            for (auto element : tmp->getElements())
            {
                if (hasSpecificDeployment(_accessor, element))
                    return true;
            }
        }
    }

    if (_attribute->getType()->getDerived() &&
        std::dynamic_pointer_cast<BstIdl::FUnionType>(_attribute->getType()->getDerived()))
    {
        if (hasSomeIpUnionLengthWidth(_accessor, _attribute))
        {
            return true;
        }
        if (hasSomeIpUnionTypeWidth(_accessor, _attribute))
        {
            return true;
        }
        if (hasSomeIpUnionDefaultOrder(_accessor, _attribute))
        {
            return true;
        }
        if (hasSomeIpUnionMaxLength(_accessor, _attribute))
        {
            return true;
        }

        auto tmp = std::dynamic_pointer_cast<BstIdl::FUnionType>(_attribute->getType()->getDerived());
        if (_accessor->isProperOverwrite())
        {
            for (auto element : tmp->getElements())
            {
                if (hasSpecificDeployment(_accessor, element))
                    return true;
            }
        }
    }

    if (_attribute->getType()->getDerived() &&
        std::dynamic_pointer_cast<BstIdl::FEnumerationType>(_attribute->getType()->getDerived()))
    {
        if (hasSomeIpEnumWidth(_accessor, _attribute))
        {
            return true;
        }
        if (hasSomeIpEnumBitWidth(_accessor, _attribute))
        {
            return true;
        }
        if (hasSomeIpEnumInvalidValue(_accessor, _attribute))
        {
            return true;
        }
    }

    if (_attribute->getType()->getPredefined() &&
        (_attribute->getType()->getPredefined()->getValue() == BstIdl::FBasicTypeId::INT8 ||
         _attribute->getType()->getPredefined()->getValue() == BstIdl::FBasicTypeId::INT16 ||
         _attribute->getType()->getPredefined()->getValue() == BstIdl::FBasicTypeId::INT32 ||
         _attribute->getType()->getPredefined()->getValue() == BstIdl::FBasicTypeId::INT64 ||
         _attribute->getType()->getPredefined()->getValue() == BstIdl::FBasicTypeId::UINT8 ||
         _attribute->getType()->getPredefined()->getValue() == BstIdl::FBasicTypeId::UINT16 ||
         _attribute->getType()->getPredefined()->getValue() == BstIdl::FBasicTypeId::UINT32 ||
         _attribute->getType()->getPredefined()->getValue() == BstIdl::FBasicTypeId::UINT64))
    {
        if (hasSomeIpIntegerBitWidth(_accessor, _attribute))
        {
            return true;
        }
        if (hasSomeIpIntegerInvalidValue(_accessor, _attribute))
        {
            return true;
        }
    }

    return false;
}

bool FrancaSomeIPGeneratorExtensions::hasDeployedArgument(const std::shared_ptr<BstIdl::FBroadcast> &_broadcast,
                                                          const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    for (auto a : _broadcast->getOutArgs())
    {
        auto overwriteAccessor = _accessor->getOverwriteAccessor(a);
        if (overwriteAccessor && hasDeployment(overwriteAccessor, a))
        {
            return true;
        }
    }
    return false;
}

bool FrancaSomeIPGeneratorExtensions::hasDeployedArgument(const std::shared_ptr<BstIdl::FMethod> &_method,
                                                          const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                          bool _in, bool _out)
{
    if (_in)
    {
        for (auto a : _method->getInArgs())
        {
            if (hasDeployment(_accessor, a))
            {
                return true;
            }
        }
    }

    if (_out)
    {
        for (auto a : _method->getOutArgs())
        {
            if (hasDeployment(_accessor, a))
            {
                return true;
            }
        }
    }

    return false;
}

std::list<std::string> FrancaSomeIPGeneratorExtensions::getDeploymentInputIncludes(
    const std::shared_ptr<BstIdl::FType> &_type, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::list<std::string> ret;
    if (_type)
        ret.emplace_back(
            someipDeploymentHeaderPath(std::dynamic_pointer_cast<BstIdl::FTypeCollection>(_type->getContainer())));
    return ret;
}

std::list<std::string> FrancaSomeIPGeneratorExtensions::getDeploymentInputIncludes(
    const std::shared_ptr<BstIdl::FTypeDef> &_typeDef, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::list<std::string> ret;
    ret.emplace_back(
        someipDeploymentHeaderPath(std::dynamic_pointer_cast<BstIdl::FTypeCollection>(_typeDef->getContainer())));
    if (_typeDef->getActualType())
    {
        for (auto it : getDeploymentInputIncludes(_typeDef->getActualType(), _accessor))
            ret.emplace_back(it);
    }
    return ret;
}

std::list<std::string> FrancaSomeIPGeneratorExtensions::getDeploymentInputIncludes(
    const std::shared_ptr<BstIdl::FTypeRef> &_typeRef, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::list<std::string> ret;
    if (_typeRef->getDerived())
    {
        for (auto it : getDeploymentInputIncludes(_typeRef->getDerived(), _accessor))
            ret.emplace_back(it);
    }
    return ret;
}

std::list<std::string> FrancaSomeIPGeneratorExtensions::getDeploymentInputIncludes(
    const std::shared_ptr<BstIdl::FStructType> &_struct, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::list<std::string> ret;
    ret.emplace_back(
        someipDeploymentHeaderPath(std::dynamic_pointer_cast<BstIdl::FTypeCollection>(_struct->getContainer())));
    auto itsStruct = _struct;
    while (itsStruct)
    {
        for (auto e : itsStruct->getElements())
        {
            auto etype = e->getType()->getDerived();
            if (etype)
            {
                for (auto it : getDeploymentInputIncludes(etype, _accessor->getOverwriteAccessor(e)))
                {
                    ret.emplace_back(it);
                }
            }
        }
        itsStruct = itsStruct->getBase();
    }
    return ret;
}

std::list<std::string> FrancaSomeIPGeneratorExtensions::getDeploymentInputIncludes(
    const std::shared_ptr<BstIdl::FUnionType> &_union, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::list<std::string> ret;
    ret.emplace_back(
        someipDeploymentHeaderPath(std::dynamic_pointer_cast<BstIdl::FTypeCollection>(_union->getContainer())));
    auto itsUnion = _union;
    while (itsUnion)
    {
        for (auto e : itsUnion->getElements())
        {
            auto etype = e->getType()->getDerived();
            if (etype)
            {
                for (auto it : getDeploymentInputIncludes(etype, _accessor->getOverwriteAccessor(e)))
                {
                    ret.emplace_back(it);
                }
            }
        }
        itsUnion = itsUnion->getBase();
    }
    return ret;
}

std::list<std::string> FrancaSomeIPGeneratorExtensions::getDeploymentInputIncludes(
    const std::shared_ptr<BstIdl::FMapType> &_map, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::list<std::string> ret;
    for (auto it : getDeploymentInputIncludes(_map->getKeyType(), _accessor))
    {
        ret.emplace_back(it);
    }
    for (auto it : getDeploymentInputIncludes(_map->getValueType(), _accessor))
    {
        ret.emplace_back(it);
    }
    return ret;
}

std::list<std::string> FrancaSomeIPGeneratorExtensions::getDeploymentInputIncludes(
    const std::shared_ptr<BstIdl::FTypeCollection> &_tc, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::list<std::string> ret;
    for (auto t : _tc->getTypes())
    {
        for (auto it : getDeploymentInputIncludes(t, _accessor))
        {
            addListItem(ret, it);
        }
    }
    return ret;
}

std::list<std::string> FrancaSomeIPGeneratorExtensions::getDeploymentInputIncludes(
    const std::shared_ptr<BstIdl::FInterface> &_interface, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::list<std::string> ret;
    addListItem(ret, someipDeploymentHeaderPath(_interface));
    for (auto x : _interface->getAttributes())
    {
        if (x->getType()->getDerived())
        {
            for (auto it : getDeploymentInputIncludes(x->getType()->getDerived(), _accessor))
            {
                addListItem(ret, it);
            }
        }
    }
    for (auto x : _interface->getBroadcasts())
    {
        for (auto y : x->getOutArgs())
        {
            if (y->getType()->getDerived())
            {
                for (auto it : getDeploymentInputIncludes(y->getType()->getDerived(), _accessor))
                {
                    addListItem(ret, it);
                }
            }
        }
        if (hasDeployedArgument(x, _accessor))
        {
            addListItem(ret, someipDeploymentHeaderPath(_interface));
        }
    }
    for (auto x : _interface->getMethods())
    {
        for (auto y : x->getOutArgs())
        {
            if (y->getType()->getDerived())
            {
                for (auto it : getDeploymentInputIncludes(y->getType()->getDerived(), _accessor))
                {
                    addListItem(ret, it);
                }
            }
        }
        for (auto y : x->getInArgs())
        {
            if (y->getType()->getDerived())
            {
                for (auto it : getDeploymentInputIncludes(y->getType()->getDerived(), _accessor))
                {
                    addListItem(ret, it);
                }
            }
        }
        if (hasError(x))
        {
            for (auto it : getDeploymentInputIncludes(x->getErrorEnum(), _accessor))
            {
                addListItem(ret, it);
            }
        }
        if (hasDeployedArgument(x, _accessor, true, true))
        {
            addListItem(ret, someipDeploymentHeaderPath(_interface));
        }
    }
    return ret;
}

std::string FrancaSomeIPGeneratorExtensions::getDeployment(const std::shared_ptr<BstIdl::FBasicTypeId> &_type)
{
    std::string itsDeployment = "CommonAPI::SomeIP::IntegerDeployment<";
    std::string str = _type->getName();
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    itsDeployment += str + "_t>";
    return itsDeployment;
}

std::string FrancaSomeIPGeneratorExtensions::getDeploymentRef(
    const std::shared_ptr<BstIdl::FTypedElement> &_typedElement, bool _isArray,
    const std::shared_ptr<BstIdl::FModelElement> &_element, const std::shared_ptr<BstIdl::FInterface> &_interface,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto name = getDeploymentName(_typedElement, _element, _interface, _accessor);
    if (!name.empty())
        return "&" + name;

    std::string deployment = "static_cast< ";
    deployment += getDeploymentType(_typedElement, _interface, true);
    deployment += "* >(nullptr)";
    return deployment;
}

std::string FrancaSomeIPGeneratorExtensions::getDeploymentRef(
    const std::shared_ptr<BstIdl::FTypedElement> &_typedElement, const std::shared_ptr<BstIdl::FModelElement> &_element,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::string name = getDeploymentName(_typedElement, _element, nullptr, _accessor);
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    if (name != "")
        return "&" + name;

    std::string elemType = "";

    if (_typedElement->getType()->getDerived())
    {
        auto containerName = genExtention.getFullName(_typedElement->getType()->getDerived()->getContainer());
        auto typeName = _typedElement->getType()->getDerived()->getName();
        elemType = containerName + "_::" + typeName + "Deployment_t";
        if (_typedElement->isArray())
            elemType = "CommonAPI::SomeIP::ArrayDeployment< " + elemType + " >";
    }
    else
    {
        elemType = getDeploymentType(_typedElement, nullptr, false);
    }

    return "static_cast< " + elemType + "* >(nullptr)";
}

std::string FrancaSomeIPGeneratorExtensions::getDeploymentRef(const std::shared_ptr<BstIdl::FTypeRef> &_typeRef,
                                                              const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    std::string name = getDeploymentName(_typeRef, nullptr, _accessor);
    if (name != "")
        return "&" + name;

    if (_typeRef->getDerived())
    {
        if (std::dynamic_pointer_cast<BstIdl::FEnumerationType>(_typeRef->getDerived()))
        {
            return "static_cast< " + getDeploymentType(_typeRef->getDerived(), nullptr, true) + "* >(nullptr)";
        }
        auto containerName = genExtention.getFullName(_typeRef->getDerived()->getContainer());
        auto typeName = _typeRef->getDerived()->getName();
        return "static_cast< " + containerName + "_::" + typeName + "Deployment_t* >(nullptr)";
    }

    return "static_cast< " + getDeploymentType(_typeRef, nullptr, false) + "* >(nullptr)";
}

std::string FrancaSomeIPGeneratorExtensions::getDeploymentRef(const std::shared_ptr<BstIdl::FType> &_type,
                                                              const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::string name = getDeploymentName(_type, nullptr, _accessor);
    if (name != "")
        return "&" + name;

    return "static_cast< " + getDeploymentType(_type, nullptr, false) + "* >(nullptr)";
}

std::string FrancaSomeIPGeneratorExtensions::getDeploymentRef(const std::shared_ptr<BstIdl::FBasicTypeId> &_typeId,
                                                              const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::string name = getDeploymentName(_typeId, nullptr, _accessor);
    if (name != "")
        return "&" + name;

    return "static_cast< " + getDeploymentType(_typeId, nullptr, false) + "* >(nullptr)";
}

std::string FrancaSomeIPGeneratorExtensions::getDeploymentName(
    const std::shared_ptr<BstIdl::FTypedElement> &_typedElement, const std::shared_ptr<BstIdl::FModelElement> &_element,
    const std::shared_ptr<BstIdl::FInterface> &_interface, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    if (hasSpecificDeployment(_accessor, _typedElement) ||
        _typedElement->isArray() && hasDeployment(_accessor, _typedElement))
    {
        std::string deployment = "";
        std::string accessorName = _accessor->getName();
        accessorName.pop_back();
        deployment = getFullName(_interface) + "_::" + accessorName + "Deployment";
        return deployment;
    }
    else
    {
        return getDeploymentName(_typedElement->getType(), _interface, _accessor);
    }
}

std::string FrancaSomeIPGeneratorExtensions::getDeploymentName(
    const std::shared_ptr<BstIdl::FTypeDef> &_typeDef, const std::shared_ptr<BstIdl::FTypeCollection> &_interface,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    return getDeploymentName(_typeDef->getActualType(), _interface, _accessor);
}

std::string FrancaSomeIPGeneratorExtensions::getDeploymentName(
    const std::shared_ptr<BstIdl::FTypeRef> &_typeRef, const std::shared_ptr<BstIdl::FTypeCollection> &_interface,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    if (_typeRef->getDerived())
    {
        return getDeploymentName(_typeRef->getDerived(), _interface, _accessor);
    }
    return getDeploymentName(_typeRef->getPredefined(), _interface, _accessor);
}

std::string FrancaSomeIPGeneratorExtensions::getDeploymentName(
    const std::shared_ptr<BstIdl::FType> &_type, const std::shared_ptr<BstIdl::FTypeCollection> &_interface,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    if (hasDeployment(_accessor, _type))
    {
        auto container = std::dynamic_pointer_cast<BstIdl::FTypeCollection>(_type->getContainer());
        return genExtention.getFullName(container) + "_::" + _type->getName() + "Deployment";
    }
    return "";
}

std::string FrancaSomeIPGeneratorExtensions::getDeploymentName(
    const std::shared_ptr<BstIdl::FBasicTypeId> &_typeId, const std::shared_ptr<BstIdl::FTypeCollection> &_interface,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    return "";
}

std::string FrancaSomeIPGeneratorExtensions::someipProxyClassName(const std::shared_ptr<BstIdl::FInterface> &fInterface)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    return genExtention.getElementName(fInterface) + "SomeIPProxy";
}

std::string FrancaSomeIPGeneratorExtensions::getSomeIpServiceID(const std::shared_ptr<BstIdl::FInterface> &fInterface)
{
    auto serviceid = getSomeIpAccessor(fInterface)->getSomeIpServiceID(fInterface);
    if (serviceid != -1)
    {
        return fmt::format("{0:#x}", serviceid);
    }
    return "UNDEFINED_SERVICE_ID";
}

std::list<std::string> FrancaSomeIPGeneratorExtensions::getNotifierEventGroups(
    const std::shared_ptr<BstIdl::FAttribute> &_attribute, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::list<std::string> strs;
    std::list<int> value = _accessor->getSomeIpEventGroups(_attribute);
    if (!value.empty())
        for (auto it : value)
            strs.emplace_back(fmt::format("CommonAPI::SomeIP::eventgroup_id_t({0:#x})", it));
    else
        strs.emplace_back("UNDEFINED_EVENTGROUP_ID");
    return strs;
}

std::string FrancaSomeIPGeneratorExtensions::getNotifierIdentifier(
    const std::shared_ptr<BstIdl::FAttribute> &_attribute, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto value = _accessor->getSomeIpNotifierID(_attribute);
    if (value != -1)
    {
        return fmt::format("CommonAPI::SomeIP::event_id_t({0:#x})", value);
    }
    return "UNDEFINED_NOTIFIER_ID";
}

std::string FrancaSomeIPGeneratorExtensions::getGetterIdentifier(
    const std::shared_ptr<BstIdl::FAttribute> &_attribute, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto value = _accessor->getSomeIpGetterID(_attribute);
    std::string getter = value == -1 ? "0x0" : fmt::format("{0:#x}", value);
    return "CommonAPI::SomeIP::method_id_t(" + getter + ")";
}

std::string FrancaSomeIPGeneratorExtensions::isGetterReliable(const std::shared_ptr<BstIdl::FAttribute> &_attribute,
                                                              const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto value = _accessor->getSomeIpGetterReliable(_attribute);
    return value ? "true" : "false";
}

std::string FrancaSomeIPGeneratorExtensions::isNotifierReliable(
    const std::shared_ptr<BstIdl::FAttribute> &_attribute, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    bool value = _accessor->getSomeIpNotifierReliable(_attribute);
    return value ? "true" : "false";
}

std::string FrancaSomeIPGeneratorExtensions::getNotifierReliabilityType(
    const std::shared_ptr<BstIdl::FAttribute> &_attribute, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    if (isNotifierReliable(_attribute, _accessor) == "true")
    {
        return "CommonAPI::SomeIP::reliability_type_e::RT_RELIABLE";
    }
    else
    {
        return "CommonAPI::SomeIP::reliability_type_e::RT_UNRELIABLE";
    }
}

std::string FrancaSomeIPGeneratorExtensions::getEndianess(const std::shared_ptr<BstIdl::FAttribute> &_attribute,
                                                          const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    return _accessor->getSomeIpEndianess(_attribute);
}

std::string FrancaSomeIPGeneratorExtensions::getEndianess(const std::shared_ptr<BstIdl::FBroadcast> &_broadcast,
                                                          const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    return _accessor->getSomeIpEndianess(_broadcast);
}

std::string FrancaSomeIPGeneratorExtensions::getSetterIdentifier(
    const std::shared_ptr<BstIdl::FAttribute> &_attribute, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto value = _accessor->getSomeIpSetterID(_attribute);
    if (value != -1)
    {
        return fmt::format("CommonAPI::SomeIP::method_id_t({0:#x})", value);
    }
    return "UNDEFINED_SETTER_ID";
}

std::string FrancaSomeIPGeneratorExtensions::isSetterReliable(const std::shared_ptr<BstIdl::FAttribute> &_attribute,
                                                              const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    bool value = _accessor->getSomeIpSetterReliable(_attribute);
    return value ? "true" : "false";
}

std::list<std::string> FrancaSomeIPGeneratorExtensions::getEventGroups(
    const std::shared_ptr<BstIdl::FBroadcast> &_broadcast, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::list<std::string> strs;
    std::list<int> value = _accessor->getSomeIpEventGroups(_broadcast);
    if (!value.empty())
    {
        for (auto it : value)
        {
            strs.emplace_back(fmt::format("{0:#x}", it));
        }
    }
    else
    {
        strs.emplace_back("UNDEFINED_EVENTGROUP_ID");
    }
    return strs;
}

std::string FrancaSomeIPGeneratorExtensions::getEventIdentifier(
    const std::shared_ptr<BstIdl::FBroadcast> &_broadcast, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    int value = _accessor->getSomeIpEventID(_broadcast);
    if (value != -1)
    {
        return fmt::format("CommonAPI::SomeIP::event_id_t({0:#x})", value);
    }
    return "UNDEFINED_EVENT_ID";
}

std::string FrancaSomeIPGeneratorExtensions::getReliabilityType(
    const std::shared_ptr<BstIdl::FBroadcast> &_broadcast, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    if (isReliable(_broadcast, _accessor) == "true")
    {
        return "CommonAPI::SomeIP::reliability_type_e::RT_RELIABLE";
    }
    else
    {
        return "CommonAPI::SomeIP::reliability_type_e::RT_UNRELIABLE";
    }
}

std::string FrancaSomeIPGeneratorExtensions::isReliable(const std::shared_ptr<BstIdl::FBroadcast> &_broadcast,
                                                        const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto value = _accessor->getSomeIpReliable(_broadcast);
    if (value != -1)
        if (value == 0)
            return "false";
        else
        {
            return "true";
        }
    return "false";
}

std::string FrancaSomeIPGeneratorExtensions::isReliable(const std::shared_ptr<BstIdl::FMethod> &_method,
                                                        const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto value = _accessor->getSomeIpReliable(_method);
    if (value != -1)
        if (value == 0)
            return "false";
        else
        {
            return "true";
        }
    return "false";
}

std::string FrancaSomeIPGeneratorExtensions::isLittleEndian(const std::shared_ptr<BstIdl::FMethod> &_method,
                                                            const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    return _accessor->getSomeIpEndianess(_method);
}

std::string FrancaSomeIPGeneratorExtensions::getDeployments(const std::shared_ptr<BstIdl::FBroadcast> &_broadcast,
                                                            const std::shared_ptr<BstIdl::FInterface> &_interface,
                                                            const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::list<std::string> args;
    for (auto it : _broadcast->getOutArgs())
    {
        args.emplace_back(
            getDeploymentRef(it, it->isArray(), _broadcast, _interface, _accessor->getOverwriteAccessor(it)));
    }
    return "std::make_tuple(" + join(args, ", ") + ")";
}

std::string FrancaSomeIPGeneratorExtensions::getErrorDeploymentType(const std::shared_ptr<BstIdl::FMethod> &_method,
                                                                    bool _isArgument)
{
    std::string deploymentType = "";
    auto gen = FrancaGeneratorExtensions::getInstance();
    if (gen.hasError(_method))
    {
        if (_method->getErrorEnum())
            deploymentType = getDeploymentType(_method->getErrorEnum(), gen.getContainingInterface(_method), true);
        if (_method->getErrors())
            if (_method->getErrors()->getBase())
                deploymentType =
                    getDeploymentType(_method->getErrors()->getBase(), gen.getContainingInterface(_method), true);
            else
                deploymentType = "CommonAPI::EmptyDeployment";
        if (_isArgument && !_method->getOutArgs().empty())
            deploymentType = deploymentType + ", ";
    }
    return deploymentType;
}

std::string FrancaSomeIPGeneratorExtensions::getErrorDeploymentRef(
    const std::shared_ptr<BstIdl::FMethod> &_method, const std::shared_ptr<BstIdl::FInterface> &_interface,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::string name = "";
    if (_method->getErrorEnum())
    {
        name += getDeploymentName(_method->getErrorEnum(), _interface, _accessor);
        if (name != "")
            return "&" + name;
    }
    return "static_cast< " + getErrorDeploymentType(_method, false) + " * >(nullptr)";
}

std::string FrancaSomeIPGeneratorExtensions::getDeployments(const std::shared_ptr<BstIdl::FMethod> &_method,
                                                            const std::shared_ptr<BstIdl::FInterface> &_interface,
                                                            const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                            bool _withInArgs, bool _withOutArgs)
{
    auto gen = FrancaGeneratorExtensions::getInstance();
    std::string inArgsDeployments = "";
    if (_withInArgs)
    {
        for (auto it : _method->getInArgs())
        {
            if (it == _method->getInArgs().front())
                inArgsDeployments +=
                    getDeploymentRef(it, it->isArray(), _method, _interface, _accessor->getOverwriteAccessor(it));
            else
            {
                inArgsDeployments += ", " + getDeploymentRef(it, it->isArray(), _method, _interface,
                                                             _accessor->getOverwriteAccessor(it));
            }
        }
    }

    std::string outArgsDeployments = "";
    if (_withOutArgs)
    {

        for (auto it : _method->getOutArgs())
        {
            if (it == _method->getOutArgs().front())
                outArgsDeployments +=
                    getDeploymentRef(it, it->isArray(), _method, _interface, _accessor->getOverwriteAccessor(it));
            else
            {
                outArgsDeployments += ", " + getDeploymentRef(it, it->isArray(), _method, _interface,
                                                              _accessor->getOverwriteAccessor(it));
            }
        }

        if (gen.hasError(_method))
        {
            std::string errorDeployment = getErrorDeploymentRef(_method, _interface, _accessor);
            if (outArgsDeployments != "")
                outArgsDeployments = errorDeployment + ", " + outArgsDeployments;
            else
                outArgsDeployments = errorDeployment;
        }
    }

    std::string deployments = inArgsDeployments;
    if (outArgsDeployments != "")
    {
        if (deployments != "")
            deployments += ", ";
        deployments += outArgsDeployments;
    }

    return "std::make_tuple(" + deployments + ")";
}

std::string FrancaSomeIPGeneratorExtensions::getMethodIdentifier(
    const std::shared_ptr<BstIdl::FMethod> &_method, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    int value = _accessor->getSomeIpMethodID(_method);
    if (value != -1)
        return fmt::format("CommonAPI::SomeIP::method_id_t({0:#x})", value);
    return "UNDEFINED_METHOD_ID";
}

std::string FrancaSomeIPGeneratorExtensions::getDeployable(const std::shared_ptr<BstIdl::FArgument> &_argument,
                                                           const std::shared_ptr<BstIdl::FInterface> &_interface,
                                                           const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto gen = FrancaGeneratorExtensions::getInstance();
    return "CommonAPI::Deployable< " + gen.getTypeName(_argument, _interface, true) + ", " +
           getDeploymentType(_argument, _interface, true) + " >";
}

std::string FrancaSomeIPGeneratorExtensions::getDeployables(std::list<std::shared_ptr<BstIdl::FArgument>> &_arguments,
                                                            const std::shared_ptr<BstIdl::FInterface> &_interface,
                                                            const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::string str;
    for (auto it : _arguments)
    {
        if (it == _arguments.front())
            str += getDeployable(it, _interface, _accessor);
        else
        {
            str += "," + getDeployable(it, _interface, _accessor);
        }
    }

    return str;
}

std::string FrancaSomeIPGeneratorExtensions::getMethodName(const std::shared_ptr<BstIdl::FAttribute> &_attribute)
{
    auto gen = FrancaGeneratorExtensions::getInstance();
    return "get" + gen.getClassName(_attribute);
}

std::string FrancaSomeIPGeneratorExtensions::setMethodName(const std::shared_ptr<BstIdl::FAttribute> &_attribute)
{
    auto gen = FrancaGeneratorExtensions::getInstance();
    return "set" + gen.getClassName(_attribute);
}

std::string FrancaSomeIPGeneratorExtensions::getDeploymentTypes(
    std::list<std::shared_ptr<BstIdl::FArgument>> &_arguments, const std::shared_ptr<BstIdl::FInterface> &_interface,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::string str;
    for (auto it : _arguments)
    {
        if (it == _arguments.front())
        {
            str += getDeploymentType(it, _interface, true);
        }
        else
            str += ", " + getDeploymentType(it, _interface, true);
    }
    return str;
}

std::string FrancaSomeIPGeneratorExtensions::getDeploymentType(
    const std::shared_ptr<BstIdl::FTypeDef> &_typeDef, const std::shared_ptr<BstIdl::FTypeCollection> &_interface,
    bool _useTc)
{
    if (!_typeDef || !_interface)
        return "CommonAPI::EmptyDeployment";
    return getDeploymentType(_typeDef->getActualType(), _interface, _useTc);
}

std::string FrancaSomeIPGeneratorExtensions::getDeploymentType(
    const std::shared_ptr<BstIdl::FTypedElement> &_typedElement,
    const std::shared_ptr<BstIdl::FTypeCollection> &_interface, bool _useTc)
{
    if (!_typedElement)
        return "CommonAPI::EmptyDeployment";
    if (_typedElement->isArray())
        return "CommonAPI::SomeIP::ArrayDeployment< " +
               getDeploymentType(_typedElement->getType(), _interface, _useTc) + " >";
    return getDeploymentType(_typedElement->getType(), _interface, _useTc);
}

std::string FrancaSomeIPGeneratorExtensions::getDeploymentType(
    const std::shared_ptr<BstIdl::FTypeRef> &_typeRef, const std::shared_ptr<BstIdl::FTypeCollection> &_interface,
    bool _useTc)
{
    if (!_typeRef)
        return "CommonAPI::EmptyDeployment";
    if (_typeRef->getDerived())
        return getDeploymentType(_typeRef->getDerived(), _interface, _useTc);
    if (_typeRef->getInterval())
        return getDeploymentType(_typeRef->getInterval(), _interface, _useTc);
    if (_typeRef->getPredefined())
        return getDeploymentType(_typeRef->getPredefined(), _interface, _useTc);

    return "CommonAPI::EmptyDeployment";
}

std::string FrancaSomeIPGeneratorExtensions::getDeploymentType(
    const std::shared_ptr<BstIdl::FIntegerInterval> &_type, const std::shared_ptr<BstIdl::FTypeCollection> &_interface,
    bool _useTc)
{
    if (!_type)
        return "CommonAPI::EmptyDeployment";

    return "CommonAPI::SomeIP::IntegerDeployment<int32_t>";
}

std::string FrancaSomeIPGeneratorExtensions::getDeploymentType(
    const std::shared_ptr<BstIdl::FBasicTypeId> &_type, const std::shared_ptr<BstIdl::FTypeCollection> &_interface,
    bool _useTc)
{
    if (!_type)
        return "CommonAPI::EmptyDeployment";

    switch (_type->getValue())
    {
    case BstIdl::FBasicTypeId::STRING:
        return "CommonAPI::SomeIP::StringDeployment";
    case BstIdl::FBasicTypeId::BYTE_BUFFER:
        return "CommonAPI::SomeIP::ByteBufferDeployment";
    case BstIdl::FBasicTypeId::INT8:
    case BstIdl::FBasicTypeId::INT16:
    case BstIdl::FBasicTypeId::INT32:
    case BstIdl::FBasicTypeId::INT64:
    case BstIdl::FBasicTypeId::UINT8:
    case BstIdl::FBasicTypeId::UINT16:
    case BstIdl::FBasicTypeId::UINT32:
    case BstIdl::FBasicTypeId::UINT64:
        return getDeployment(_type);
    default:
        return "CommonAPI::EmptyDeployment";
    }
}

std::string FrancaSomeIPGeneratorExtensions::getDeploymentType(
    const std::shared_ptr<BstIdl::FType> &_type, const std::shared_ptr<BstIdl::FTypeCollection> &_interface,
    bool _useTc)
{
    if (!_type)
        return "CommonAPI::EmptyDeployment";

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FTypeDef>(_type))
        return getDeploymentType(ptr, _interface, _useTc);

    std::string deploymentType;

    if (_useTc)
        deploymentType = FrancaGeneratorExtensions::getInstance().getFullName(_type->getContainer()) + "_::";

    deploymentType += _type->getName() + "Deployment_t";

    return deploymentType;
}

// vsomeip extensions
std::string FrancaSomeIPGeneratorExtensions::getServerHostMessage(
    const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    if (!_provider || !_accessor)
        return std::string();
    else
        return std::string("\n\t\"unicast\": \"" + _accessor->getSomeIpServerHostUnicast(_provider) + "\"");
}

std::string FrancaSomeIPGeneratorExtensions::getClientHostMessage(
    const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    if (!_provider || !_accessor)
        return std::string();
    else
        return std::string("\n\t\"unicast\": \"" + _accessor->getSomeIpClientHostUnicast(_provider) + "\"");
}

std::string FrancaSomeIPGeneratorExtensions::getLogging(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                                        const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    if (!_provider || !_accessor)
        return std::string();

    std::string logging("");
    auto level = _accessor->getSomeIpLoggingLevel(_provider);
    if (level.empty())
        return logging;
    std::list<std::string> content;
    content.emplace_back("\n\t\t\"level\" : \"" + level + "\"");
    bool isOK;
    auto consoleEnable =
        _accessor->getSomeIpLoggingViaConsole(_provider, isOK) ? std::string("true") : std::string("false");
    if (isOK)
        content.emplace_back("\n\t\t\"consolve\" : \"" + consoleEnable + "\"");
    auto logFileEnable =
        _accessor->getSomeIpCreateLogFile(_provider, isOK) ? std::string("true") : std::string("false");
    if (isOK)
    {
        auto logFileAbsPath = _accessor->getSomeIpLogFileAbsPath(_provider);
        content.emplace_back("\n\t\t\"file\" : { \"enable\" : \"" + logFileEnable + "\", \"path\" : \"" +
                             logFileAbsPath + "\" }");
    }
    auto diagnoseAndTraceEnable =
        _accessor->getSomeIpLoggingDLT(_provider, isOK) ? std::string("true") : std::string("false");
    if (isOK)
        content.emplace_back("\n\t\t\"dlt\" : \"" + diagnoseAndTraceEnable + "\"");
    auto versionCyclicLogEable =
        _accessor->getSomeIpVersionCyclicLogEable(_provider, isOK) ? std::string("true") : std::string("false");
    auto versionLogInterval = _accessor->getSomeIpVersionLogInterval(_provider);
    content.emplace_back("\n\t\t\"version\" : {\"enable\" : " + versionCyclicLogEable + ", \"interval\" : \"" +
                         std::to_string(versionLogInterval) + "\"}");
    auto memoryLogInterval = _accessor->getSomeIpMemoryLogInterval(_provider);
    auto statusLogInterval = _accessor->getSomeIpStatusLogInterval(_provider);
    if (memoryLogInterval > 0)
        content.emplace_back("\n\t\t\"memory_log_interval\" : \"" + std::to_string(memoryLogInterval) + "\"");
    if (statusLogInterval > 0)
        content.emplace_back("\n\t\t\"status_log_interval\" : \"" + std::to_string(statusLogInterval) + "\"");

    logging.append("\n\t\"logging\" : \n\t{" + join(content, ",") + "\n\t}");
    return logging;
}

std::string FrancaSomeIPGeneratorExtensions::getServiceDiscovery(
    const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    if (!_provider || !_accessor)
        return std::string();

    bool isOK;
    auto sd_enable = _accessor->getSomeIpServiceDiscoveryEnable(_provider, isOK);
    if (!isOK)
        return std::string();
    if (!sd_enable)
        return std::string("\n\t\"service-discovery\" : \n\t{\n\t\t\"enable\" : \"false\"\n\t}");
    auto multicast = _accessor->getSomeIpServiceDiscoveryMulticastAddress(_provider);
    auto port = _accessor->getSomeIpServiceDiscoveryPort(_provider);
    auto protocol = _accessor->getSomeIpServiceDiscoveryProtocol(_provider);
    auto initial_delay_min = _accessor->getSomeIpServiceDiscoveryInitialDelayMin(_provider);
    auto initial_delay_max = _accessor->getSomeIpServiceDiscoveryInitialDelayMax(_provider);
    auto repetitions_base_delay = _accessor->getSomeIpServiceDiscoveryRepetitionsBaseDelay(_provider);
    auto repetitions_max = _accessor->getSomeIpServiceDiscoveryRepetitionsMax(_provider);
    auto ttl = _accessor->getSomeIpServiceDiscoveryttl(_provider);
    auto cyclic_offer_delay = _accessor->getSomeIpServiceDiscoveryCyclicOfferDelay(_provider);
    auto request_response_delay = _accessor->getSomeIpServiceDiscoveryRequestResponseDelay(_provider);

    std::list<std::string> content;
    content.emplace_back("\n\t\t\"enable\" : \"false\"");
    content.emplace_back("\n\t\t\"multicast\" : \"" + multicast + "\"");
    content.emplace_back("\n\t\t\"port\" : \"" + std::to_string(port) + "\"");
    content.emplace_back("\n\t\t\"protocol\" : \"" + protocol + "\"");
    content.emplace_back("\n\t\t\"initial_delay_min\" : \"" + std::to_string(initial_delay_min) + "\"");
    content.emplace_back("\n\t\t\"initial_delay_max\" : \"" + std::to_string(initial_delay_max) + "\"");
    content.emplace_back("\n\t\t\"repetitions_base_delay\" : \"" + std::to_string(repetitions_base_delay) + "\"");
    content.emplace_back("\n\t\t\"repetitions_max\" : \"" + std::to_string(repetitions_max) + "\"");
    content.emplace_back("\n\t\t\"ttl\" : \"" + std::to_string(ttl) + "\"");
    content.emplace_back("\n\t\t\"cyclic_offer_delay\" : \"" + std::to_string(cyclic_offer_delay) + "\"");
    content.emplace_back("\n\t\t\"request_response_delay\" : \"" + std::to_string(request_response_delay) + "\"");
    std::string service_discovery("\n\t\"service-discovery\" : \n\t{" + join(content, ",") + "\n\t}");
    return service_discovery;
}

std::string FrancaSomeIPGeneratorExtensions::getApplications(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                                             const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    if (!_provider || !_accessor)
        return std::string();
    auto appList = getApplication(_provider, _accessor);
    std::string applications("\n\t\"applications\" : \n\t[" + join(appList, ",") + "\n\t]");
    return applications;
}

std::list<std::string> FrancaSomeIPGeneratorExtensions::getApplication(
    const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::list<std::string> apps;
    if (!_provider || !_accessor)
        return apps;
    auto names = _accessor->getSomeIpApplicationNames(_provider);
    auto ids = _accessor->getSomeIpApplicationIDs(_provider);
    if (ids.size() != names.size() || ids.empty())
        return apps;

    while (!names.empty() && !ids.empty())
    {
        auto name = names.front();
        auto id = ids.front();
        names.pop_front();
        ids.pop_front();
        std::string content("");
        content.append("\n\t\t\t\"name\" : \"" + name + "\",");
        content.append("\n\t\t\t\"id\" : \"" + id + "\"");
        apps.emplace_back("\n\t\t{" + content + "\n\t\t}");
    }
    return apps;
}
// vsomeip services
std::string FrancaSomeIPGeneratorExtensions::getServices(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                                         const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    if (!_provider || !_accessor)
        return std::string();
    auto serviceList = getService(_provider, _accessor);
    std::string services("\n\t\"services\" : \n\t[" + join(serviceList, ",") + "\n\t]");
    return services;
}
std::list<std::string> FrancaSomeIPGeneratorExtensions::getService(
    const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::list<std::string> serviceList;
    if (!_provider || !_accessor)
        return serviceList;

    for (const auto &ins : _provider->getInstances())
    {
        if (!ins)
            continue;
        std::list<std::string> content;
        auto fInterface = std::dynamic_pointer_cast<BstIdl::FInterface>(ins->getTarget());
        auto inf_accessor = getSomeIpAccessor(fInterface);
        auto service = inf_accessor->getSomeIpServiceID(fInterface);
        auto instance = _accessor->getSomeIpInstanceID(ins);
        auto reliable = _accessor->getSomeIpReliableUnicastPort(ins);
        auto unreliable = _accessor->getSomeIpUnreliableUnicastPort(ins);
        auto events = getEvents(ins, inf_accessor);
        auto eventgroups = getEventGroups(ins, inf_accessor);

        content.emplace_back("\n\t\t\t\"service\" : \"" + std::to_string(service) + "\"");
        content.emplace_back("\n\t\t\t\"instance\" : \"" + std::to_string(instance) + "\"");
        content.emplace_back("\n\t\t\t\"reliable\" : \"" + std::to_string(reliable) + "\"");
        content.emplace_back("\n\t\t\t\"unreliable\" : \"" + std::to_string(unreliable) + "\"");
        if (!events.empty())
            content.emplace_back(events);
        if (!eventgroups.empty())
            content.emplace_back(eventgroups);
        serviceList.emplace_back("\n\t\t{" + join(content, ",") + "\n\t\t}");
    }

    return serviceList;
}
std::string FrancaSomeIPGeneratorExtensions::getEvents(const std::shared_ptr<BstIdl::FDExtensionElement> &_instance,
                                                       const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::string events("\n\t\t\t\"events\" : ");
    auto eventList = getEvent(_instance, _accessor);
    if (eventList.empty())
        return std::string();
    events.append("\n\t\t\t[" + join(eventList, ",") + "\n\t\t\t]");
    return events;
}
std::list<std::string> FrancaSomeIPGeneratorExtensions::getEvent(
    const std::shared_ptr<BstIdl::FDExtensionElement> &_instance,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::list<std::string> eventList;
    auto fInterface = std::dynamic_pointer_cast<BstIdl::FInterface>(_instance->getTarget());

    for (const auto &bc : fInterface->getBroadcasts())
    {
        auto event_id = getEventID(bc, _accessor);
        std::string event("\n\t\t\t\t{\n\t\t\t\t\t\"event\" : \"" + event_id + "\",");
        event.append("\n\t\t\t\t\t\"is_field\" : \"false\",");
        event.append("\n\t\t\t\t\t\"is_reliable\" : \"" + isReliable(bc, _accessor) + "\"\n\t\t\t\t}");
        eventList.emplace_back(event);
    }
    for (const auto &attr : fInterface->getAttributes())
    {
        auto event_id = getNotifierID(attr, _accessor);
        std::string event("\n\t\t\t\t{\n\t\t\t\t\t\"event\" : \"" + event_id + "\",");
        event.append("\n\t\t\t\t\t\"is_field\" : \"true\",");
        event.append("\n\t\t\t\t\t\"is_reliable\" : \"" + isNotifierReliable(attr, _accessor) + "\"\n\t\t\t\t}");
        eventList.emplace_back(event);
    }
    return eventList;
}
std::string FrancaSomeIPGeneratorExtensions::getEventGroups(
    const std::shared_ptr<BstIdl::FDExtensionElement> &_instance,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::string eventgroups("\n\t\t\t\"eventgroups\" : ");
    auto eventgroupList = getEventGroup(_instance, _accessor);
    if (eventgroupList.empty())
        return std::string();
    eventgroups.append("\n\t\t\t[" + join(eventgroupList, ",") + "\n\t\t\t]");
    return eventgroups;
}
std::list<std::string> FrancaSomeIPGeneratorExtensions::getEventGroup(
    const std::shared_ptr<BstIdl::FDExtensionElement> &_instance,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::list<std::string> eventgroups;
    auto fInterface = std::dynamic_pointer_cast<BstIdl::FInterface>(_instance->getTarget());

    for (const auto &bc : fInterface->getBroadcasts())
    {
        auto events_id = getEventID(bc, _accessor);
        for (const auto &id : getEventGroupsID(bc, _accessor))
        {
            std::string eventgroup("\n\t\t\t\t{\n\t\t\t\t\t\"eventgroup\" : \"" + id + "\",");
            eventgroup.append("\n\t\t\t\t\t\"events\" : [\"" + events_id + "\"]\n\t\t\t\t}");
            eventgroups.emplace_back(eventgroup);
        }
    }
    for (const auto &attr : fInterface->getAttributes())
    {
        auto events_id = getNotifierID(attr, _accessor);
        for (const auto &id : getNotifierEventGroupsID(attr, _accessor))
        {
            std::string eventgroup("\n\t\t\t\t{\n\t\t\t\t\t\"eventgroup\" : \"" + id + "\",");
            eventgroup.append("\n\t\t\t\t\t\"events\" : [\"" + events_id + "\"]\n\t\t\t\t}");
            eventgroups.emplace_back(eventgroup);
        }
    }

    return eventgroups;
}

std::string FrancaSomeIPGeneratorExtensions::getEventID(const std::shared_ptr<BstIdl::FBroadcast> &_broadcast,
                                                        const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    int value = _accessor->getSomeIpEventID(_broadcast);
    if (value != -1)
    {
        return std::to_string(value);
    }
    return "UNDEFINED_EVENT_ID";
}

std::string FrancaSomeIPGeneratorExtensions::getNotifierID(const std::shared_ptr<BstIdl::FAttribute> &_attribute,
                                                           const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto value = _accessor->getSomeIpNotifierID(_attribute);
    if (value != -1)
    {
        return std::to_string(value);
    }
    return "UNDEFINED_NOTIFIER_ID";
}

std::list<std::string> FrancaSomeIPGeneratorExtensions::getEventGroupsID(
    const std::shared_ptr<BstIdl::FBroadcast> &_broadcast, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::list<std::string> strs;
    std::list<int> value = _accessor->getSomeIpEventGroups(_broadcast);
    if (!value.empty())
    {
        for (const auto &it : value)
        {
            strs.emplace_back(std::to_string(it));
        }
    }
    return strs;
}

std::list<std::string> FrancaSomeIPGeneratorExtensions::getNotifierEventGroupsID(
    const std::shared_ptr<BstIdl::FAttribute> &_attribute, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::list<std::string> strs;
    std::list<int> value = _accessor->getSomeIpEventGroups(_attribute);
    if (!value.empty())
        for (const auto &it : value)
            strs.emplace_back(std::to_string(it));
    return strs;
}

} // namespace BstCommonAPI