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
#include "validator/ReferenceManager.h"
#include "fmt/core.h"
#include "model/FModelManager.h"
#include "utilities/list_utility.h"
#include "validator/BstIdlError.h"
#include <algorithm>

namespace BstIdl
{
ReferenceManager &ReferenceManager::getInstance()
{
    static ReferenceManager ins;
    return ins;
}

void ReferenceManager::clear()
{
    m_interfaceExtendsRefs.clear();
    m_interfaceManagesRefs.clear();
    m_methodErrorRefs.clear();
    m_typeRefDerivedRefs.clear();
    m_structTypeExtendsRefs.clear();
    m_unionTypeExtendsRefs.clear();
    m_enumerationTypeExtendsRefs.clear();
    m_qerElementRefs.clear();
}

void ReferenceManager::addInterfaceExtendsRef(const std::shared_ptr<FInterface> &interface, const std::string &fqn)
{
    if (!exists(m_interfaceExtendsRefs, interface, fqn))
        m_interfaceExtendsRefs.emplace_back(std::make_tuple(interface, fqn));
}

void ReferenceManager::addInterfaceManagesRef(const std::shared_ptr<FInterface> &interface, const std::string &fqn)
{
    if (!exists(m_interfaceManagesRefs, interface, fqn))
        m_interfaceManagesRefs.emplace_back(std::make_tuple(interface, fqn));
}

void ReferenceManager::addMethodErrorRef(const std::shared_ptr<FMethod> &method, const std::string &fqn,
                                         const std::shared_ptr<FInterface> &belonged)
{
    if (!exists(m_methodErrorRefs, method, fqn, belonged))
        m_methodErrorRefs.emplace_back(std::make_tuple(method, fqn, belonged));
}

void ReferenceManager::addTypeRefDerivedRef(const std::shared_ptr<FTypeRef> &typeRef, const std::string &fqn,
                                            const std::shared_ptr<FInterface> &belonged)
{
    if (!exists(m_typeRefDerivedRefs, typeRef, fqn, belonged))
        m_typeRefDerivedRefs.emplace_back(std::make_tuple(typeRef, fqn, belonged));
}

void ReferenceManager::addStructTypeExtendsRef(const std::shared_ptr<FStructType> &structType, const std::string &fqn,
                                               const std::shared_ptr<FInterface> &belonged)
{
    if (!exists(m_structTypeExtendsRefs, structType, fqn, belonged))
        m_structTypeExtendsRefs.emplace_back(std::make_tuple(structType, fqn, belonged));
}

void ReferenceManager::addUnionTypeExtendsRef(const std::shared_ptr<FUnionType> &unionType, const std::string &fqn,
                                              const std::shared_ptr<FInterface> &belonged)
{
    if (!exists(m_unionTypeExtendsRefs, unionType, fqn, belonged))
        m_unionTypeExtendsRefs.emplace_back(std::make_tuple(unionType, fqn, belonged));
}

void ReferenceManager::addEnumerationTypeExtendsRef(const std::shared_ptr<FEnumerationType> &enumType,
                                                    const std::string &fqn, const std::shared_ptr<FInterface> &belonged)
{
    if (!exists(m_enumerationTypeExtendsRefs, enumType, fqn, belonged))
        m_enumerationTypeExtendsRefs.emplace_back(std::make_tuple(enumType, fqn, belonged));
}

void ReferenceManager::addQerElementRef(const std::shared_ptr<FQualifiedElementRef> &qre, const std::string &fqn,
                                        const std::shared_ptr<FInterface> &belonged)
{
    if (!exists(m_qerElementRefs, qre, fqn, belonged))
        m_qerElementRefs.emplace_back(std::make_tuple(qre, fqn, belonged));
}

std::list<std::string> ReferenceManager::resolveInterfaceExtendsRef()
{
    auto &ins = FModelManager::getInstance();
    std::list<std::string> errorList;
    for (const auto &item : m_interfaceExtendsRefs)
    {
        auto interface = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto ref = ins.findFInterface(fqn);
        if (ref != nullptr)
        {
            if (interface->getBase() != nullptr)
            {
                auto base = interface->getBase();
                interface->setBase(nullptr);
                errorList.emplace_back(fmt::format("duplicate reference: {1}, previous is {2}, used by {0}.",
                                                   interface->getFQN(), fqn, base->getFQN()));
            }
            else
                interface->setBase(std::move(ref));
        }
    }
    for (const auto &item : m_interfaceExtendsRefs)
    {
        auto interface = std::get<0>(item);
        auto fqn = std::get<1>(item);
        if (interface->getBase() == nullptr)
            errorList.emplace_back(fmt::format("unresolved reference: {1} extended by {0}", interface->getFQN(), fqn));
    }
    return errorList;
}

std::list<std::string> ReferenceManager::resolveInterfaceManagesRef()
{
    auto &ins = FModelManager::getInstance();
    std::list<std::string> errorList;
    for (const auto &item : m_interfaceManagesRefs)
    {
        auto interface = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto ref = ins.findFInterface(fqn);
        if (ref != nullptr)
        {
            auto &interfaces = interface->getManagedInterfaces();
            auto iter = std::find(interfaces.cbegin(), interfaces.cend(), ref);
            if (iter != interfaces.cend())
            {
                interfaces.erase(iter);
                errorList.emplace_back(fmt::format("duplicate reference: {1}, previous is {2}, used by {0}.",
                                                   interface->getFQN(), fqn, ref->getFQN()));
            }
            else
            {
                interfaces.emplace_back(ref);
            }
        }
    }
    for (const auto &item : m_interfaceManagesRefs)
    {
        auto interface = std::get<0>(item);
        auto fqn = std::get<1>(item);
        if (interface->getManagedInterfaces().empty())
            errorList.emplace_back(fmt::format("unresolved reference: {1} extended by {0}", interface->getFQN(), fqn));
    }
    return errorList;
}

std::list<std::string> ReferenceManager::resolveMethodErrorRef()
{
    auto &ins = FModelManager::getInstance();
    std::list<std::string> errorList;
    for (const auto &item : m_methodErrorRefs)
    {
        auto method = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto parent = std::get<2>(item);
        // find fqn in pulic area
        auto ref = ins.findPublicFEnumerationType(fqn);
        if (ref == nullptr)
        {
            // find fqn in base interfaces.
            while (parent != nullptr)
            {
                ref = ins.findFEnumerationTypeInInterface(parent, fqn);
                if (ref != nullptr)
                    break;
                parent = parent->getBase();
            }
        }

        if (ref != nullptr)
        {
            auto tmp = method->getErrorEnum();
            if (tmp != nullptr)
            {
                method->setErrorEnum(nullptr);
                errorList.emplace_back(fmt::format("duplicate reference: {1}, previous is {2}, used by {0}.",
                                                   method->getFQN(), fqn, tmp->getFQN()));
            }
            else
            {
                method->setErrorEnum(ref);
            }
        }
    }
    for (const auto &item : m_methodErrorRefs)
    {
        auto method = std::get<0>(item);
        auto fqn = std::get<1>(item);
        if (method->getErrorEnum() == nullptr)
        {
            errorList.emplace_back(fmt::format("unresolved reference: {1} used by {0}", method->getFQN(), fqn));
        }
    }
    return errorList;
}

std::list<std::string> ReferenceManager::resolveTypeRefDerviedRef()
{
    auto &ins = FModelManager::getInstance();
    std::list<std::string> errorList;
    for (const auto &item : m_typeRefDerivedRefs)
    {
        auto typeRef = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto interface = std::get<2>(item);

        // find type in pulic area.
        auto ref = ins.findPublicFType(fqn);
        if (ref == nullptr)
        {
            // find in interface.
            ref = ins.findFTypeInInterface(interface, fqn);
        }
        if (ref != nullptr)
        {
            auto tmp = typeRef->getDerived();
            if (tmp != nullptr)
            {
                typeRef->setDerived(nullptr);
                errorList.emplace_back(fmt::format("duplicate reference: {1}, previous is {2}, used by {0}.", "TypeRef",
                                                   ref->getFQN(), tmp->getFQN()));
            }
            else
            {
                typeRef->setDerived(ref);
            }
        }
    }
    for (const auto &item : m_typeRefDerivedRefs)
    {
        auto typeRef = std::get<0>(item);
        auto fqn = std::get<1>(item);
        if (typeRef->getDerived() == nullptr)
        {
            errorList.emplace_back(fmt::format("unresolved reference: {1} used by {0}", "TypeRef", fqn));
        }
    }
    return errorList;
}

std::list<std::string> ReferenceManager::resolveStructTypeExtendsRef()
{
    auto &ins = FModelManager::getInstance();
    std::list<std::string> errorList;
    for (const auto &item : m_structTypeExtendsRefs)
    {
        auto structType = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto interface = std::get<2>(item);

        // find derived type in pulic area.
        auto ref = ins.findPublicFStructType(fqn);
        if (ref == nullptr)
        {
            // find in interfaces.
            ref = ins.findFStructTypeInInterface(interface, fqn);
        }

        if (ref != nullptr)
        {
            auto tmp = structType->getBase();
            if (tmp != nullptr)
            {
                structType->setBase(nullptr);
                errorList.emplace_back(fmt::format("duplicate reference: {1}, previous is {2}, used by {0}.",
                                                   structType->getFQN(), fqn, tmp->getFQN()));
            }
            else
            {
                structType->setBase(ref);
            }
        }
    }
    for (const auto &item : m_structTypeExtendsRefs)
    {
        auto structType = std::get<0>(item);
        auto fqn = std::get<1>(item);
        if (structType->getBase() == nullptr)
        {
            errorList.emplace_back(fmt::format("unresolved reference: {1} used by {0}", structType->getFQN(), fqn));
        }
    }
    return errorList;
}

std::list<std::string> ReferenceManager::resolveUnionTypeExtendsRef()
{
    auto &ins = FModelManager::getInstance();
    std::list<std::string> errorList;
    for (const auto &item : m_unionTypeExtendsRefs)
    {
        auto unionType = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto interface = std::get<2>(item);

        // find derived type in pulic area.
        auto ref = ins.findPublicFUnionType(fqn);
        if (ref == nullptr)
        {
            // find in base interfaces.
            ref = ins.findFUnionTypeInInterface(interface, fqn);
        }

        if (ref != nullptr)
        {
            auto tmp = unionType->getBase();
            if (tmp != nullptr)
            {
                unionType->setBase(nullptr);
                errorList.emplace_back(fmt::format("duplicate reference: {1}, previous is {2}, used by {0}.",
                                                   unionType->getFQN(), fqn, tmp->getFQN()));
            }
            else
            {
                unionType->setBase(ref);
            }
        }
    }
    for (const auto &item : m_unionTypeExtendsRefs)
    {
        auto unionType = std::get<0>(item);
        auto fqn = std::get<1>(item);
        if (unionType->getBase() == nullptr)
        {
            errorList.emplace_back(fmt::format("unresolved reference: {1} used by {0}", unionType->getFQN(), fqn));
        }
    }
    return errorList;
}

std::list<std::string> ReferenceManager::resolveEnumerationTypeExtendsRef()
{
    auto &ins = FModelManager::getInstance();
    std::list<std::string> errorList;
    for (const auto &item : m_enumerationTypeExtendsRefs)
    {
        auto enumType = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto interface = std::get<2>(item);

        // find derived type in pulic area.
        auto ref = ins.findPublicFEnumerationType(fqn);
        if (ref == nullptr)
        {
            // find in base interfaces.
            ref = ins.findFEnumerationTypeInInterface(interface, fqn);
        }

        if (ref != nullptr)
        {
            auto tmp = enumType->getBase();
            if (tmp != nullptr)
            {
                enumType->setBase(nullptr);
                errorList.emplace_back(fmt::format("duplicate reference: {1}, previous is {2}, used by {0}.",
                                                   enumType->getFQN(), fqn, tmp->getFQN()));
            }
            else
            {
                enumType->setBase(ref);
            }
        }
    }
    for (const auto &item : m_enumerationTypeExtendsRefs)
    {
        auto enumType = std::get<0>(item);
        auto fqn = std::get<1>(item);
        if (enumType->getBase() == nullptr)
        {
            errorList.emplace_back(fmt::format("unresolved reference: {1} used by {0}", enumType->getFQN(), fqn));
        }
    }
    return errorList;
}

std::list<std::string> ReferenceManager::resolveQerElementRef()
{
    auto &ins = FModelManager::getInstance();
    std::list<std::string> errorList;
    for (const auto &item : m_qerElementRefs)
    {
        auto qer = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto interface = std::get<2>(item);

        // find derived type in pulic area.
        auto ref = ins.findPublicConstant(fqn);
        if (ref == nullptr)
        {
            // find in base interfaces.
            ref = ins.findFConstantDefInInterface(interface, fqn);
        }

        if (ref != nullptr)
        {
            auto tmp = qer->getElement();
            if (tmp != nullptr && tmp != ref)
            {
                qer->setElement(nullptr);
                errorList.emplace_back(fmt::format("duplicate reference: {1}, previous is {2}, used by {0}.",
                                                   "Qualified Element Reference", fqn, tmp->getFQN()));
            }
            else
            {
                qer->setElement(ref);
            }
        }
    }
    for (const auto &item : m_qerElementRefs)
    {
        auto qer = std::get<0>(item);
        auto fqn = std::get<1>(item);
        if (qer->getElement() == nullptr)
        {
            errorList.emplace_back(
                fmt::format("unresolved reference: {1} used by {0}", "Qualified Element Reference", fqn));
        }
    }
    return errorList;
}
bool ReferenceManager::resolveAllRef()
{
    std::list<std::string> errorList;
    std::cout << "Resolving all references in models..." << std::endl;
    errorList.splice(errorList.end(), resolveInterfaceExtendsRef());
    errorList.splice(errorList.end(), resolveInterfaceManagesRef());
    errorList.splice(errorList.end(), resolveMethodErrorRef());
    errorList.splice(errorList.end(), resolveTypeRefDerviedRef());
    errorList.splice(errorList.end(), resolveStructTypeExtendsRef());
    errorList.splice(errorList.end(), resolveUnionTypeExtendsRef());
    errorList.splice(errorList.end(), resolveEnumerationTypeExtendsRef());
    errorList.splice(errorList.end(), resolveQerElementRef());

    bool ret = errorList.empty();
    if (ret)
        std::cout << "Success!" << std::endl;
    else
        std::cerr << join(errorList, "\n") << std::endl;
    return ret;
}

} // namespace BstIdl