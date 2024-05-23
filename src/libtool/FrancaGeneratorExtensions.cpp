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
#include "libtool/FrancaGeneratorExtensions.h"
#include "utilities/string_utility.h"
#include <map>
namespace BstIdl
{
void FrancaGeneratorExtensions::addFTypeDirectlyReferencedTypes(std::list<std::shared_ptr<FType>> &list,
                                                                std::shared_ptr<FType> fType)
{
    if (std::dynamic_pointer_cast<FStructType>(fType) != nullptr)
    {
        auto elements = std::dynamic_pointer_cast<FStructType>(fType)->getElements();
        for (auto element : elements)
        {
            auto derived = element->getType()->getDerived();
            if (derived != nullptr)
            {
                list.push_back(derived);
            }
        }
        return;
    }
    if (std::dynamic_pointer_cast<FEnumerationType>(fType) != nullptr)
    {
        auto base = std::dynamic_pointer_cast<FEnumerationType>(fType)->getBase();
        if (base != nullptr)
        {
            list.push_back(base);
        }
        return;
    }
    if (std::dynamic_pointer_cast<FArrayType>(fType) != nullptr)
    {
        auto derived = std::dynamic_pointer_cast<FArrayType>(fType)->getElementType()->getDerived();
        if (derived != nullptr)
        {
            list.push_back(derived);
        }
        return;
    }
    if (std::dynamic_pointer_cast<FUnionType>(fType) != nullptr)
    {
        auto elements = std::dynamic_pointer_cast<FUnionType>(fType)->getElements();
        for (auto element : elements)
        {
            auto derived = element->getType()->getDerived();
            if (derived != nullptr)
            {
                list.push_back(derived);
            }
        }
        return;
    }
    if (std::dynamic_pointer_cast<FMapType>(fType) != nullptr)
    {
        auto keyDerived = std::dynamic_pointer_cast<FMapType>(fType)->getKeyType()->getDerived();
        if (keyDerived != nullptr)
        {
            list.push_back(keyDerived);
        }
        auto valueDerived = std::dynamic_pointer_cast<FMapType>(fType)->getValueType()->getDerived();
        if (valueDerived != nullptr)
        {
            list.push_back(valueDerived);
        }
        return;
    }
    if (std::dynamic_pointer_cast<FTypeDef>(fType) != nullptr)
    {
        auto derived = std::dynamic_pointer_cast<FTypeDef>(fType)->getActualType()->getDerived();
        if (derived != nullptr)
        {
            list.push_back(derived);
        }
        return;
    }
}

std::list<std::string> FrancaGeneratorExtensions::getNamespaceAsList(const std::shared_ptr<FModel> &fModel)
{
    auto name = fModel->getName();
    std::list<std::string> list;
    uint32_t pos = 0;
    while (name.find(".") < (name.size() - 1))
    {
        list.push_back(name.substr(pos, name.find(".")));
        pos = name.find(".") + 1;
        name = name.substr(pos);
        pos = 0;
    }
    list.push_back(name);
    return list;
}

std::list<std::string> FrancaGeneratorExtensions::getNamespaceAsList(
    const std::shared_ptr<FModelElement> &fModelElement)
{
    std::list<std::string> namespaceList;
    if (std::dynamic_pointer_cast<FTypeCollection>(fModelElement) != nullptr)
    {
        auto model = std::dynamic_pointer_cast<FModel>(
            std::dynamic_pointer_cast<FTypeCollection>(fModelElement)->getContainer());
        namespaceList = getNamespaceAsList(model);
    }
    else
    {
        auto element = std::dynamic_pointer_cast<FModelElement>(fModelElement->getContainer());
        namespaceList = getNamespaceAsList(element);
        namespaceList.emplace_back(element->getName());
    }
    return namespaceList;
}

int64_t FrancaGeneratorExtensions::getMaximumEnumerationValue(const std::shared_ptr<FEnumerationType> &_enumeration)
{
    int64_t maximum = 0;
    for (auto literal : _enumeration->getEnumerators())
    {
        if (literal->getValue() != nullptr)
        {
            auto value = literal->getValue();
            auto str = literal->valueToString();
            int64_t literalValue = atoi(literal->valueToString().c_str());
            if (maximum < literalValue)
                maximum = literalValue;
        }
    }
    return maximum;
}

std::list<std::string> FrancaGeneratorExtensions::getSubnamespaceList(const std::shared_ptr<FModelElement> &destination,
                                                                      const std::shared_ptr<FObject> &source)
{
    auto model = std::dynamic_pointer_cast<FModel>(source);
    auto modelElement = std::dynamic_pointer_cast<FModelElement>(source);
    std::list<std::string> sourceNamespaceList;
    if (model != nullptr)
        sourceNamespaceList = getNamespaceAsList(model);
    else if (modelElement != nullptr)
        sourceNamespaceList = getNamespaceAsList(modelElement);
    auto destinationNamespaceList = getNamespaceAsList(destination);
    int maxCount;
    if (sourceNamespaceList.size() < destinationNamespaceList.size())
        maxCount = sourceNamespaceList.size();
    else
    {
        maxCount = destinationNamespaceList.size();
    }
    auto dropCount = 0;
    while (dropCount < maxCount && (sourceNamespaceList.front() == destinationNamespaceList.front()))
    {
        sourceNamespaceList.pop_front();
        destinationNamespaceList.pop_front();
        dropCount += 1;
    }
    return destinationNamespaceList;
}

std::list<std::shared_ptr<FMethod>> FrancaGeneratorExtensions::getMethodsWithError(
    const std::shared_ptr<FInterface> &_interface)
{
    std::map<std::string, std::shared_ptr<FMethod>> itsMethods;
    for (auto method : _interface->getMethods())
    {
        if (method->getErrors() != nullptr)
        {
            auto existing = itsMethods.find(method->getName());
            if (existing == itsMethods.end())
            {
                itsMethods[method->getName()] = method;
            }
            else
            {
                std::list<std::shared_ptr<FEnumerator>> itsAdditionals;

                for (auto e : method->getErrors()->getEnumerators())
                {
                    auto found = false;
                    for (auto f : itsMethods[method->getName()]->getErrors()->getEnumerators())
                    {
                        if (f->getName() == e->getName())
                            found = true;
                    }
                    if (!found)
                        itsAdditionals.emplace_back(e);
                }
                for (auto itsAdditional : itsAdditionals)
                {
                    addListItem(itsMethods[method->getName()]->getErrors()->getEnumerators(), std::move(itsAdditional));
                }
            }
        }
    }
    std::list<std::shared_ptr<FMethod>> methods;
    for (auto it : itsMethods)
    {
        methods.emplace_back(it.second);
    }
    return methods;
}

std::string FrancaGeneratorExtensions::doCast(std::string _value, std::string _backingType)
{
    if (isSignedBackingType(_backingType))
    {
        int64_t itsValue = atoll(_value.c_str());
        int64_t itsSignedMax;
        int64_t itsUnsignedMax;
        if (_backingType == "int8_t")
        {
            itsSignedMax = 127;
            itsUnsignedMax = 255;
        }
        else if (_backingType == "int16_t")
        {
            itsSignedMax = 32767;
            itsUnsignedMax = 65535;
        }
        else if (_backingType == "int32_t")
        {
            itsSignedMax = 2147483647;
            itsUnsignedMax = 4294967295;
        }
        else
        {
            itsSignedMax = 9223372036854775807UL;
            itsUnsignedMax = 18446744073709551615UL;
        }
        if (itsValue > itsSignedMax && itsValue <= itsUnsignedMax)
        {
            itsValue = itsValue - itsUnsignedMax - 1;
            return std::to_string(itsValue);
        }
    }
    return _value;
}
bool FrancaGeneratorExtensions::isSignedBackingType(std::string _backingType)
{
    return (_backingType == "int8_t" || _backingType == "int16_t" || _backingType == "int32_t" ||
            _backingType == "int64_t");
}

std::string FrancaGeneratorExtensions::getConstantValue(const std::shared_ptr<FModelElement> &expression)
{
    if (std::dynamic_pointer_cast<FConstantDef>(expression) != nullptr)
    {
        return getConstantType(std::dynamic_pointer_cast<FConstantDef>(expression)->getRhs());
    }
    else
    {
        return nullptr;
    }
}

std::string FrancaGeneratorExtensions::getConstantType(const std::shared_ptr<FInitializerExpression> &expression)
{
    if (std::dynamic_pointer_cast<FIntegerConstant>(expression))
        return std::dynamic_pointer_cast<FIntegerConstant>(expression)->toString();
    else
        return nullptr;
}

void FrancaGeneratorExtensions::setEnumerationValues(const std::shared_ptr<FEnumerationType> &_enumeration)
{
    int64_t currentValue = 0;
    std::list<std::string> predefineEnumValues;
    // collect all predefined enum values
    for (auto literal : _enumeration->getEnumerators())
    {
        if (literal->getValue().get() != nullptr)
        {
            auto str = literal->valueToString();
            predefineEnumValues.push_back(str);
        }
    }
    if (_enumeration->getBase() != nullptr)
    {
        auto base = _enumeration->getBase();
        setEnumerationValues(base);
        currentValue = getMaximumEnumerationValue(base) + 1;
    }
    for (auto literal : _enumeration->getEnumerators())
    {
        if (literal->getValue() == nullptr || literal->valueToString().empty())
        {
            for (auto it : predefineEnumValues)
            {
                if (atoi(it.c_str()) == currentValue)
                {
                    currentValue += 1;
                }
            }
            auto obj = FFactory::getInstance().createFIntegerConstant();
            obj->setUnsignedVal(currentValue);
            literal->setValue(obj);
        }
        currentValue += 1;
    }
}

std::shared_ptr<FBasicTypeId> FrancaGeneratorExtensions::getBackingType(
    const std::shared_ptr<FEnumerationType> &fEnumerationType)
{
    return FBasicTypeId::get(FBasicTypeId::INT32);
}

std::string FrancaGeneratorExtensions::getFullyQualifiedName(const std::shared_ptr<FModelElement> &fModelElement)
{
    auto &mgr = FModelManager::getInstance();
    if (std::dynamic_pointer_cast<FTypeCollection>(fModelElement))
        return std::dynamic_pointer_cast<FTypeCollection>(fModelElement)->getContainer()->getName() + "." +
               fModelElement->getName();
    else
        return getFullyQualifiedName(std::dynamic_pointer_cast<FModelElement>(fModelElement->getContainer())) + "." +
               fModelElement->getName();
}

std::string FrancaGeneratorExtensions::getFullyQualifiedCppName(
    const std::shared_ptr<BstIdl::FModelElement> &fModelElement)
{
    if (!fModelElement || !fModelElement->getContainer())
        return "";
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FModel>(fModelElement->getContainer()))
    {
        std::string containerName = ptr->getName();
        std::string prefix = "::";
        if (auto tc = std::dynamic_pointer_cast<BstIdl::FTypeCollection>(fModelElement))
        {
            if (tc->getVersion())
                prefix = getVersionPrefix(tc);
        }
        std::string ret = prefix + containerName + "::" + getElementName(fModelElement);
        return replace_all(ret, ".", "::");
    }
    std::string ret =
        getFullyQualifiedCppName(std::dynamic_pointer_cast<BstIdl::FModelElement>(fModelElement->getContainer())) +
        "::" + getElementName(fModelElement);
    return replace_all(ret, ".", "::");
}

std::string FrancaGeneratorExtensions::getInterfaceVersion(const std::shared_ptr<FInterface> &_interface)
{
    return "v" + std::to_string(_interface->getVersion()->getMajor()) + "_" +
           std::to_string(_interface->getVersion()->getMinor());
}

std::string FrancaGeneratorExtensions::getFullyQualifiedNameWithVersion(const std::shared_ptr<FInterface> &_interface)
{
    return getFullyQualifiedName(_interface) + ":" + getInterfaceVersion(_interface);
}

std::list<std::shared_ptr<FType>> FrancaGeneratorExtensions::getDirectlyReferencedTypes(
    const std::shared_ptr<FType> &type)
{
    directlyReferencedTypes.clear();
    addFTypeDirectlyReferencedTypes(directlyReferencedTypes, type);
    return directlyReferencedTypes;
}

std::string FrancaGeneratorExtensions::getDefineName(const std::shared_ptr<FModelElement> &fModelElement)
{
    std::string definePrefix;
    if (std::dynamic_pointer_cast<FTypeCollection>(fModelElement) != nullptr)
    {
        auto tc = std::dynamic_pointer_cast<FTypeCollection>(fModelElement);
        if (tc->getVersion() != nullptr)
        {
            definePrefix = "V" + std::to_string(tc->getVersion()->getMajor()) + "_";
        }
    }
    auto words = splitCamelCase(fModelElement->getName());
    std::string defineSuffix;
    while (words.size() != 0)
    {
        defineSuffix += "_" + words.front();
        words.pop_front();
    }
    if (std::dynamic_pointer_cast<FTypeCollection>(fModelElement))
    {
        auto &mgr = FModelManager::getInstance();
        auto model = std::dynamic_pointer_cast<FModel>(fModelElement->getContainer());
        return definePrefix + getDefineName(model) + defineSuffix;
    }
    else
    {
        return definePrefix + getDefineName(std::dynamic_pointer_cast<FModelElement>(fModelElement)) + defineSuffix;
    }
}

std::string FrancaGeneratorExtensions::getDefineName(const std::shared_ptr<FModel> &model)
{
    auto name = model->getName();
    while (name.find(".") != name.npos)
        name = name.replace(name.find("."), 1, "_");
    transform(name.begin(), name.end(), name.begin(), ::toupper);
    return name;
}

std::list<std::shared_ptr<FField>> FrancaGeneratorExtensions::getAllElements(
    const std::shared_ptr<FStructType> &_struct)
{
    if (_struct->getBase() == nullptr)
        return _struct->getElements();
    auto elements = getAllElements(_struct->getBase());
    for (auto element : _struct->getElements())
    {
        elements.emplace_back(element);
    }
    return elements;
}

std::list<std::shared_ptr<FField>> FrancaGeneratorExtensions::getAllElements(const std::shared_ptr<FUnionType> &_union)
{
    if (_union->getBase() == nullptr)
        return _union->getElements();
    auto elements = getAllElements(_union->getBase());
    for (auto element : _union->getElements())
    {
        elements.emplace_back(element);
    }
    return elements;
}

std::string FrancaGeneratorExtensions::generateDummyValue(const std::shared_ptr<FTypeRef> &typeRef)
{
    std::string retval;
    if (typeRef->getDerived() == nullptr && typeRef->getPredefined())
    {
        switch (typeRef->getPredefined()->getValue())
        {
        case FBasicTypeId::BOOLEAN:
            retval = "false";
            break;
        case FBasicTypeId::INT8:
            retval = "0";
            break;
        case FBasicTypeId::UINT8:
            retval = "0u";
            break;
        case FBasicTypeId::INT16:
            retval = "0";
            break;
        case FBasicTypeId::UINT16:
            retval = "0u";
            break;
        case FBasicTypeId::INT32:
            retval = "0";
            break;
        case FBasicTypeId::UINT32:
            retval = "0ul";
            break;
        case FBasicTypeId::INT64:
            retval = "0";
            break;
        case FBasicTypeId::UINT64:
            retval = "0ull";
            break;
        case FBasicTypeId::FLOAT:
            retval = "0.0f";
            break;
        case FBasicTypeId::DOUBLE:
            retval = "0.0";
            break;
        case FBasicTypeId::STRING:
            retval = "\"\"";
            break;
        case FBasicTypeId::BYTE_BUFFER:
            retval = "nullptr";
            break;
        default:
            retval = "";
            break;
        }
    }
    return retval;
}

std::list<std::string> FrancaGeneratorExtensions::splitCamelCase(const std::string &str)
{
    std::list<uint32_t> pos;
    for (auto it = str.begin(); it != str.end(); it++)
    {
        if (((*it) - 'A') >= 0 && ('Z' - (*it)) >= 0)
        {
            pos.push_back(it - str.begin());
        }
    }
    std::list<std::string> words;
    uint32_t begin = pos.front();
    // if the first character is lowercase
    if (begin != 0)
    {
        words.push_back(str.substr(0, begin));
    }
    while (pos.size() > 1)
    {
        pos.pop_front();
        uint32_t end = pos.front();
        words.push_back(str.substr(begin, end - begin));
        begin = end;
    }
    if (pos.size() == 1)
        words.push_back(str.substr(begin, str.size() - begin));
    // the whole world is lowercase
    if (words.empty())
    {
        words.push_back(str);
    }
    return words;
}
std::string FrancaGeneratorExtensions::getEnumPrefix()
{
    return "";
}

std::string FrancaGeneratorExtensions::getTypeName(const std::shared_ptr<FTypedElement> &_element,
                                                   const std::shared_ptr<FModelElement> &_source, bool _isOther)
{
    std::string typeName = getElementType(_element->getType(), _source, _isOther);
    if (std::dynamic_pointer_cast<FStructType>(_element->getType()->getDerived()) != nullptr &&
        std::dynamic_pointer_cast<FStructType>(_element->getType()->getDerived())->isPolymorphic())
        typeName = "std::shared_ptr<" + typeName + ">";
    if (_element->isArray())
        typeName = typeName + "*";
    return typeName;
}

std::string FrancaGeneratorExtensions::generateBSTLicenseHeader()
{

    std::string header = R"(/*
 * This file was generated by the BST Exported Library Generators.
 */)";
    return header;
}

std::string FrancaGeneratorExtensions::getCommentedString(std::string str)
{
    if (str == "")
        return str;
    std::list<std::string> lines;
    std::string comment;
    uint32_t pos = 0;
    while (str.find("\n") < (str.size() - 1))
    {
        lines.push_back(str.substr(pos, str.find("\n")));
        pos = str.find("\n") + 1;
    }
    lines.push_back(str.substr(pos + 1));
    for (auto line : lines)
    {
        comment += "*" + line + "\n";
    }
    return comment;
}

std::string FrancaGeneratorExtensions::getLicenseHeader()
{
    std::string str = "";
    return str;
}

std::string FrancaGeneratorExtensions::getCoreVersion()
{
    std::string bundleName = "com.bst.libtool.core";
    bundleName += getBundleVersion(bundleName);
    return bundleName;
}
std::string FrancaGeneratorExtensions::getFrancaVersion()
{
    std::string bundleName = "org.franca.core";
    bundleName += getBundleVersion(bundleName);
    return bundleName;
}
std::string FrancaGeneratorExtensions::getBundleVersion(std::string bundleName)
{
    return "0.1"; // focus
}

std::string FrancaGeneratorExtensions::getVersionPathPrefix(const std::shared_ptr<FTypeCollection> &_tc)
{
    std::string prefix = "";
    auto itsVersion = _tc->getVersion();
    if (itsVersion != nullptr && (itsVersion->getMajor() != 0 || itsVersion->getMinor() != 0))
    {
        prefix = "v" + std::to_string(itsVersion->getMajor()) + "/";
    }
    return prefix;
}

std::string FrancaGeneratorExtensions::getDirectoryPath(const std::shared_ptr<FModel> &fModel)
{
    auto name = fModel->getName();
    while (name.find(".") != name.npos)
        name = name.replace(name.find("."), 1, "/");
    return name;
}

std::string FrancaGeneratorExtensions::getHeaderFile(const std::shared_ptr<FTypeCollection> &fTypeCollection)
{
    return fTypeCollection->getName() + ".hpp";
}

std::string FrancaGeneratorExtensions::getInstanceHeaderFile(const std::shared_ptr<FTypeCollection> &fTypeCollection)
{
    return "";
}

std::string FrancaGeneratorExtensions::getHeaderPath(const std::shared_ptr<FTypeCollection> &fTypeCollection)
{
    FModelManager &mgr = FModelManager::getInstance();
    return getVersionPathPrefix(fTypeCollection) +
           getDirectoryPath(std::dynamic_pointer_cast<FModel>((fTypeCollection->getContainer()))) + "/" +
           getHeaderFile(fTypeCollection);
}

std::string FrancaGeneratorExtensions::getSourceFile(const std::shared_ptr<FTypeCollection> &fTypeCollection)
{
    return fTypeCollection->getName() + ".cpp";
}

std::string FrancaGeneratorExtensions::getSourcePath(const std::shared_ptr<FTypeCollection> &fTypeCollection)
{
    FModelManager &mgr = FModelManager::getInstance();
    return getVersionPathPrefix(fTypeCollection) +
           getDirectoryPath(std::dynamic_pointer_cast<FModel>((fTypeCollection->getContainer()))) + "/" +
           getSourceFile(fTypeCollection);
}

std::string FrancaGeneratorExtensions::getProxyHeaderFile(const std::shared_ptr<FInterface> &fInterface)
{
    return fInterface->getName() + "Proxy.hpp";
}

std::string FrancaGeneratorExtensions::getProxyHeaderPath(const std::shared_ptr<FInterface> &fInterface)
{
    return getVersionPathPrefix(fInterface) +
           getDirectoryPath(std::dynamic_pointer_cast<FModel>((fInterface->getContainer()))) + "/" +
           getProxyHeaderFile(fInterface);
}

std::string FrancaGeneratorExtensions::getProxySourceFile(const std::shared_ptr<FInterface> &fInterface)
{
    return fInterface->getName() + "Proxy.cpp";
}

std::string FrancaGeneratorExtensions::getProxySourcePath(const std::shared_ptr<FInterface> &fInterface)
{
    return getVersionPathPrefix(fInterface) +
           getDirectoryPath(std::dynamic_pointer_cast<FModel>((fInterface->getContainer()))) + "/" +
           getProxySourceFile(fInterface);
}

std::string FrancaGeneratorExtensions::getStubHeaderFile(const std::shared_ptr<FInterface> &fInterface)
{
    return fInterface->getName() + "Stub.hpp";
}

std::string FrancaGeneratorExtensions::getStubSourcePath(const std::shared_ptr<FInterface> &fInterface)
{
    return getVersionPathPrefix(fInterface) +
           getDirectoryPath(std::dynamic_pointer_cast<FModel>((fInterface->getContainer()))) + "/" +
           getStubSourceFile(fInterface);
}

std::string FrancaGeneratorExtensions::getStubSourceFile(const std::shared_ptr<FInterface> &fInterface)
{
    return fInterface->getName() + "Stub.cpp";
}

std::string FrancaGeneratorExtensions::getStubHeaderPath(const std::shared_ptr<FInterface> &fInterface)
{
    return getVersionPathPrefix(fInterface) +
           getDirectoryPath(std::dynamic_pointer_cast<FModel>((fInterface->getContainer()))) + "/" +
           getStubHeaderFile(fInterface);
}

bool FrancaGeneratorExtensions::hasPolymorphicBase(const std::shared_ptr<FStructType> &fStructType)
{
    if (fStructType->isPolymorphic())
    {
        return true;
    }
    return fStructType->getBase() != nullptr && fStructType->getBase()->isPolymorphic();
}

bool FrancaGeneratorExtensions::hasDerivedTypes(const std::shared_ptr<FStructType> &fStructType)
{
    std::list<std::shared_ptr<FStructType>> struct_types;
    getDerivedFStructTypes(fStructType, struct_types);
    return !struct_types.empty();
}
void FrancaGeneratorExtensions::getDerivedFStructTypes(const std::shared_ptr<FStructType> &fStructType,
                                                       std::list<std::shared_ptr<FStructType>> &struct_types)
{
    if (!fStructType)
        return;

    BstIdl::FModelManager &mgr = BstIdl::FModelManager::getInstance();
    auto parent = fStructType->getContainer();
    if (!parent)
        return;
    auto model = std::dynamic_pointer_cast<BstIdl::FModel>(parent->getContainer());
    if (model)
        mgr.getDerivedFStructTypes(model, fStructType, struct_types);
}

std::string FrancaGeneratorExtensions::generateVersionNamespaceBegin(const std::shared_ptr<FTypeCollection> &_tc)
{
    auto itsVersion = _tc->getVersion();
    if (itsVersion != nullptr && (itsVersion->getMajor() != 0 || itsVersion->getMinor() != 0))
    {
        return "namespace v" + std::to_string(itsVersion->getMajor()) + "\n{";
    }
    return "";
}

std::string FrancaGeneratorExtensions::generateVersionNamespaceEnd(const std::shared_ptr<FTypeCollection> &_tc)
{
    auto itsVersion = _tc->getVersion();
    if (itsVersion != nullptr && (itsVersion->getMajor() != 0 || itsVersion->getMinor() != 0))
    {
        return "} // namespace v" + std::to_string(itsVersion->getMajor());
    }
    return "";
}

std::string FrancaGeneratorExtensions::generateNamespaceBeginDeclaration(const std::shared_ptr<FModel> &model)
{
    std::string str;
    for (auto subnamespace : getNamespaceAsList(model))
    {
        str += "namespace " + subnamespace + "\n{\n";
    }
    return str;
}
std::string FrancaGeneratorExtensions::generateNamespaceEndDeclaration(const std::shared_ptr<FModel> &model)
{
    std::list<std::string> list = getNamespaceAsList(model);
    std::string str;
    reverse(list.begin(), list.end());
    for (auto subnamespace : list)
    {
        str += "} // namespace " + subnamespace + "\n";
    }
    return str;
}
std::string FrancaGeneratorExtensions::generateMajorVersionNamespace(const std::shared_ptr<FTypeCollection> &_tc)
{
    std::string str;
    auto itsVersion = _tc->getVersion();
    if (itsVersion != nullptr && (itsVersion->getMajor() != 0 || itsVersion->getMinor() != 0))
    {
        str += "// Compatibility\nnamespace v" + std::to_string(itsVersion->getMajor()) + "_" +
               std::to_string(itsVersion->getMinor()) + "= v" + std::to_string(itsVersion->getMajor()) + ";\n";
    }
    return str;
}
bool FrancaGeneratorExtensions::isTheSameVersion(const std::shared_ptr<FVersion> _mine,
                                                 const std::shared_ptr<FVersion> _other)
{
    return ((_mine == nullptr && _other == nullptr) ||
            (_mine != nullptr && _other != nullptr && _mine->getMajor() == _other->getMajor() &&
             _mine->getMinor() == _other->getMinor()));
}
std::string FrancaGeneratorExtensions::getTypeCollectionName(const std::shared_ptr<FTypeCollection> &_me,
                                                             const std::shared_ptr<FTypeCollection> &_other)
{
    auto &mgr = FModelManager::getInstance();
    if (_other == nullptr || _me->getContainer() != _other->getContainer() ||
        isTheSameVersion(_me->getVersion(), _other->getVersion()))
    {
        return getFullName(_me);
    }
    return getContainerName(_me);
}
std::string FrancaGeneratorExtensions::getContainerName(const std::shared_ptr<FObject> &_container)
{
    std::string name;
    if (std::dynamic_pointer_cast<FModel>(_container) || std::dynamic_pointer_cast<FModelElement>(_container))
    {
        name = _container->getName();
    }

    return name;
}
std::string FrancaGeneratorExtensions::getPrimitiveTypeName(const std::shared_ptr<FBasicTypeId> &fBasicTypeId)
{
    auto value = fBasicTypeId->getValue();
    switch (value)
    {
    case FBasicTypeId::BOOLEAN:
        return "bool";
    case FBasicTypeId::INT8:
        return "int8_t";
    case FBasicTypeId::UINT8:
        return "uint8_t";
    case FBasicTypeId::INT16:
        return "int16_t";
    case FBasicTypeId::UINT16:
        return "uint16_t";
    case FBasicTypeId::INT32:
        return "int32_t";
    case FBasicTypeId::UINT32:
        return "uint32_t";
    case FBasicTypeId::INT64:
        return "int64_t";
    case FBasicTypeId::UINT64:
        return "uint64_t";
    case FBasicTypeId::FLOAT:
        return "float";
    case FBasicTypeId::DOUBLE:
        return "double";
    case FBasicTypeId::STRING:
        return "std::string";
    case FBasicTypeId::BYTE_BUFFER:
        return "uint8_t*";
    default:
        std::cout << "Unsupported basic type: " << fBasicTypeId->getName() << std::endl;
        return std::string();
    }
}
std::shared_ptr<FObject> FrancaGeneratorExtensions::getCommonContainer(const std::shared_ptr<FObject> &_me,
                                                                       const std::shared_ptr<FObject> &_other)
{
    auto &mgr = FModelManager::getInstance();
    if (_other == nullptr)
        return nullptr;
    if (_me == _other)
        return _me;
    auto me = std::dynamic_pointer_cast<FTypeCollection>(_me);
    auto other = std::dynamic_pointer_cast<FTypeCollection>(_other);
    if (me != nullptr && other != nullptr)
    {
        if (me->getContainer() == _other->getContainer() &&
            ((me->getVersion() == nullptr && other->getVersion() == nullptr) ||
             (me->getVersion() != nullptr && other->getVersion() != nullptr &&
              me->getVersion()->getMajor() == other->getVersion()->getMajor() &&
              me->getVersion()->getMinor() == other->getVersion()->getMinor())))
        {
            return _me;
        }
    }
    return getCommonContainer(_me, _other->getContainer());
}
std::string FrancaGeneratorExtensions::getElementType(const std::shared_ptr<FTypeRef> &_typeRef,
                                                      const std::shared_ptr<FModelElement> &_container, bool _isOther)
{
    std::string typeName;
    if (_typeRef->getDerived() != nullptr)
    {
        typeName = getElementName(_typeRef->getDerived(), _container, _isOther);
    }
    else if (_typeRef->getPredefined() != nullptr)
    {
        typeName = getPrimitiveTypeName(_typeRef->getPredefined());
    }
    return typeName;
}

std::string FrancaGeneratorExtensions::getElementName(const std::shared_ptr<BstIdl::FModelElement> &fModelElement)
{
    if ((fModelElement->getName().empty() || fModelElement->getName() == "") &&
        std::dynamic_pointer_cast<BstIdl::FTypeCollection>(fModelElement) != nullptr)
    {
        return "__Anonymous__";
    }
    else
    {
        return fModelElement->getName();
    }
}

std::string FrancaGeneratorExtensions::getElementName(const std::shared_ptr<FModelElement> &_me,
                                                      const std::shared_ptr<FModelElement> &_other, bool _isOther)
{
    if (_other == nullptr)
        return getContainerName(_me);
    std::shared_ptr<FVersion> myVersion;
    if (std::dynamic_pointer_cast<FTypeCollection>(_me))
        myVersion = std::dynamic_pointer_cast<FTypeCollection>(_me)->getVersion();
    std::shared_ptr<FVersion> otherVersion;
    if (std::dynamic_pointer_cast<FTypeCollection>(_other))
        otherVersion = std::dynamic_pointer_cast<FTypeCollection>(_other)->getVersion();
    if (isTheSameVersion(myVersion, otherVersion))
    {
        auto &mgr = FModelManager::getInstance();
        auto myContainer = _me->getContainer();
        auto otherContainer = _other->getContainer();
        if (myContainer == otherContainer)
        {
            std::string name = getContainerName(_me);
            if (_isOther)
            {
                name = getContainerName(myContainer) + "::" + getContainerName(_me);
            }
            return name;
        }
        if (getCommonContainer(myContainer, otherContainer) != nullptr)
        {
            return getPartialName(_me, myContainer->getContainer());
        }
        if (getCommonContainer(otherContainer, myContainer) != nullptr)
        {
            std::string name = getContainerName(_me);
            if (_isOther)
            {
                name = getContainerName(myContainer) + "::" + name;
            }
            return name;
        }
    }
    return getFullName(_me);
}
std::string FrancaGeneratorExtensions::getPartialName(const std::shared_ptr<FModelElement> &_me,
                                                      const std::shared_ptr<FObject> &_until)
{
    std::string name = _me->getName();
    auto &mgr = FModelManager::getInstance();
    auto container = _me->getContainer();
    while (container != nullptr && container != _until)
    {
        if (std::dynamic_pointer_cast<FModel>(container) || std::dynamic_pointer_cast<BstIdl::FModelElement>(container))
        {
            name = getContainerName(container) + "::" + name;
        }
        container = container->getContainer();
    }
    while (name.find(".") != name.npos)
        name = name.replace(name.find("."), 1, "::");
    return name;
}
std::string FrancaGeneratorExtensions::getFullName(const std::shared_ptr<FObject> &_me)
{
    std::string name;
    if (std::dynamic_pointer_cast<FModelElement>(_me) != nullptr)
    {
        std::string prefix;
        std::shared_ptr<FObject> container = _me;
        while (container != nullptr)
        {
            if (std::dynamic_pointer_cast<FTypeCollection>(container) != nullptr)
                prefix = getVersionPrefix(std::dynamic_pointer_cast<FTypeCollection>(container));
            auto containerName = getContainerName(container);
            if (!containerName.empty())
            {
                if (name != "")
                {
                    name = containerName + "::" + name;
                }
                else
                {
                    name = containerName;
                }
            }
            container = container->getContainer();
        }
        name = prefix + name;
    }
    return replace_all(name, ".", "::");
}
std::string FrancaGeneratorExtensions::getVersionPrefix(const std::shared_ptr<FTypeCollection> &_tc)
{
    std::string prefix = "::";
    auto itsVersion = _tc->getVersion();
    if (itsVersion != nullptr && (itsVersion->getMajor() != 0 || itsVersion->getMinor() != 0))
    {
        prefix += "v" + std::to_string(itsVersion->getMajor()) + "::";
    }
    return prefix;
}
std::string FrancaGeneratorExtensions::getClassName(const std::shared_ptr<FAttribute> &fAttribute)
{
    auto name = toFirstUpper(fAttribute->getName());
    return name;
}

std::string FrancaGeneratorExtensions::getClassName(const std::shared_ptr<FBroadcast> &fBroadcast)
{
    auto name = toFirstUpper(fBroadcast->getName()) + toFirstUpper(fBroadcast->getSelector());
    return name;
}

std::string FrancaGeneratorExtensions::getStubClassName(const std::shared_ptr<FInterface> &fInterface)
{
    return fInterface->getName() + "Stub";
}

std::string FrancaGeneratorExtensions::generateBroadcastCallback(const std::shared_ptr<FBroadcast> &_broadcast)
{

    std::list<std::string> args;
    for (auto outArg : _broadcast->getOutArgs())
    {
        if (outArg->isArray())
            args.emplace_back(getTypeName(outArg, _broadcast, true));
        else
            args.emplace_back("const " + getTypeName(outArg, _broadcast, true) + "&");
    }
    std::string signature = "\ntypedef std::function<void (" + join(args, ", ") + ")>";
    signature += getClassName(_broadcast) + "Callback;\n";
    return signature;
}

void FrancaGeneratorExtensions::generateSelectiveBroadcastStubIncludes(const std::shared_ptr<FInterface> &fInterface,
                                                                       std::list<std::string> &generatedHeaders,
                                                                       std::list<std::string> &libraryHeaders)
{
    for (auto broadcast : fInterface->getBroadcasts())
    {
        if (!broadcast->isSelective())
        {
            addListItem(libraryHeaders, std::string("unordered_set"));
            break;
        }
    }
}

std::string FrancaGeneratorExtensions::generateGetMethodDefinition(const std::shared_ptr<FAttribute> &fAttribute)
{
    std::string definition = "const " + getTypeName(fAttribute, fAttribute, false);
    fAttribute->isArray() ? definition.append(" ") : definition.append(" &");
    definition += "get" + getClassName(fAttribute) + "() const";
    return definition;
}

std::string FrancaGeneratorExtensions::generateSetMethodDefinition(const std::shared_ptr<FAttribute> &fAttribute)
{
    std::string type = getTypeName(fAttribute, fAttribute, false) + " &";
    fAttribute->isArray() ? type.pop_back(), type.append("const") : type = "const " + type;
    auto definition = "void set" + getClassName(fAttribute) + "(" + type + fAttribute->getName() + "_)";
    return definition;
}

std::string FrancaGeneratorExtensions::generateSetCallbackDefinition(const std::shared_ptr<FBroadcast> &fBroadcast)
{
    auto callback = getClassName(fBroadcast) + "Callback";
    auto definition = "void set" + callback + "(const " + callback + " &callback_)";
    return definition;
}

std::string FrancaGeneratorExtensions::generateDefinition(const std::shared_ptr<FMethod> &fMethod, bool _isDefault)
{
    return generateDefinitionWithin(fMethod, "", _isDefault);
}

std::string FrancaGeneratorExtensions::generateDefinitionWithin(const std::shared_ptr<FMethod> &fMethod,
                                                                std::string parentClassName, bool _isDefault)
{
    std::string definition = "void ";
    if (parentClassName != "")
        definition = definition + parentClassName + "::";
    definition += fMethod->getName() + "(" + generateDefinitionSignature(fMethod) + ")";
    return definition;
}

std::string FrancaGeneratorExtensions::generateDefinitionSignature(const std::shared_ptr<FMethod> &fMethod)
{
    std::string signature;
    std::list<std::string> args;
    for (auto inArg : fMethod->getInArgs())
    {
        if (inArg->isArray())
            args.emplace_back(getTypeName(inArg, fMethod, true) + " _" + inArg->getName());
        else
            args.emplace_back("const " + getTypeName(inArg, fMethod, true) + " &_" + inArg->getName());
    }

    for (auto outArg : fMethod->getOutArgs())
    {
        args.emplace_back(getTypeName(outArg, fMethod, true) + " &_" + outArg->getName());
    }

    if (hasError(fMethod))
    {
        args.emplace_back(getErrorNameReference(fMethod, fMethod->getContainer()) + " &_error");
    }
    return join(args, ", ");
}
bool FrancaGeneratorExtensions::hasError(const std::shared_ptr<FMethod> &fMethod)
{
    if (fMethod->getErrorEnum() != nullptr || fMethod->getErrors() != nullptr)
    {
        return true;
    }
    return false;
}

std::string FrancaGeneratorExtensions::getErrorNameReference(const std::shared_ptr<FMethod> &fMethod,
                                                             const std::shared_ptr<FObject> &source)
{
    if (!hasError(fMethod))
    {
        std::cout << "FMethod has no error: " + fMethod->getName() + "\n";
        return "";
    }
    if (fMethod->getErrorEnum() != nullptr)
    {
        auto errorEnum = fMethod->getErrorEnum();
        return getElementName(errorEnum, fMethod, true);
    }
    auto errors = fMethod->getErrors();
    auto errorNameReference = getErrorName(errors);
    errorNameReference =
        getRelativeNameReference(std::dynamic_pointer_cast<FModelElement>(fMethod->getContainer()), source) +
        "::" + errorNameReference;
    return errorNameReference;
}

std::string FrancaGeneratorExtensions::getErrorName(const std::shared_ptr<FEnumerationType> &fMethodErrors)
{
    if (std::dynamic_pointer_cast<FMethod>(fMethodErrors->getContainer()) == nullptr)
    {
        std::cout << "Not FMethod errors\n";
        return "";
    }
    return std::dynamic_pointer_cast<FMethod>(fMethodErrors->getContainer())->getName() + "Error";
}

std::string FrancaGeneratorExtensions::getCallbackName(const std::shared_ptr<FBroadcast> &fBroadcast)
{
    return getClassName(fBroadcast) + "Callback";
}

std::string FrancaGeneratorExtensions::getRelativeNameReference(const std::shared_ptr<FModelElement> &destination,
                                                                const std::shared_ptr<FObject> &source)
{
    auto nameReference = destination->getName();
    std::list<std::string> subnamespaceList;
    if (destination != nullptr && destination->getContainer() != nullptr && destination->getContainer() == source)
    {
        subnamespaceList = getSubnamespaceList(destination, source);
    }
    if (!subnamespaceList.empty())
    {
        std::string preName;
        for (auto it : subnamespaceList)
        {
            preName += it + "::";
        }
        nameReference = preName + nameReference;
    }
    return nameReference;
}

std::string FrancaGeneratorExtensions::generateGetMethodImpl(const std::shared_ptr<FAttribute> &fAttribute,
                                                             std::string className)
{
    std::string definition("const ");
    if (fAttribute->getType()->getDerived() != nullptr)
        definition.append(std::dynamic_pointer_cast<BstIdl::FInterface>(fAttribute->getContainer())->getName() +
                          "::" + getTypeName(fAttribute, fAttribute, false));
    else
        definition.append(getTypeName(fAttribute, fAttribute, false));
    fAttribute->isArray() ? definition.append(" ") : definition.append(" &");
    definition = definition + className + "::get" + getClassName(fAttribute) + "() const";
    return definition;
}

std::string FrancaGeneratorExtensions::generateSetMethodImpl(const std::shared_ptr<FAttribute> &fAttribute,
                                                             std::string className)
{
    auto type = getTypeName(fAttribute, fAttribute, false) + " &";
    fAttribute->isArray() ? type.pop_back(), type.append("const") : type = "const " + type;
    auto definition =
        "void " + className + "::set" + getClassName(fAttribute) + "(" + type + fAttribute->getName() + "_)";
    return definition;
}

std::string FrancaGeneratorExtensions::generateSetCallbackImpl(const std::shared_ptr<FBroadcast> &fBroadcast,
                                                               std::string className)
{
    auto callback = getClassName(fBroadcast) + "Callback";
    auto definition = "void " + className + "::set" + callback + "(const " + callback + " &callback_)";
    return definition;
}

std::string FrancaGeneratorExtensions::generateMethodImpl(const std::shared_ptr<FMethod> &fMethod,
                                                          std::string parentClassName)
{
    std::string definition = "void ";
    if (!parentClassName.empty())
        definition = definition + parentClassName + "::";
    definition = definition + fMethod->getName() + "(" + generateDefinitionSignature(fMethod) + ")";
    return definition;
}

std::string FrancaGeneratorExtensions::generateArgList(const std::shared_ptr<FMethod> &fMethod)
{
    std::list<std::string> args;
    for (auto inArg : fMethod->getInArgs())
    {
        args.emplace_back("_" + inArg->getName());
    }

    for (auto outArg : fMethod->getOutArgs())
    {
        args.emplace_back("_" + outArg->getName());
    }

    if (hasError(fMethod))
    {
        args.emplace_back("_error");
    }
    return join(args, ", ");
}

std::string FrancaGeneratorExtensions::generateFireBroadcastDefinition(const std::shared_ptr<FBroadcast> &fBroadcast)
{
    std::list<std::string> args;
    for (auto arg : fBroadcast->getOutArgs())
    {
        if (arg->isArray())
            args.emplace_back(getTypeName(arg, fBroadcast, true) + " " + arg->getName());
        else
            args.emplace_back("const " + getTypeName(arg, fBroadcast, true) + " &" + arg->getName());
    }
    return "void " + getStubAdapterClassFireEventMethodName(fBroadcast) + "(" + join(args, ", ") + ")";
}

std::string FrancaGeneratorExtensions::getStubAdapterClassFireEventMethodName(
    const std::shared_ptr<FBroadcast> &fBroadcast)
{
    std::string str = toFirstUpper(fBroadcast->getName());
    str = "fire" + str + "Event";
    return str;
}
} // namespace BstIdl
