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
#include "validator/FDReferenceManager.h"
#include "FDModel/FDModelManager.h"
#include "fmt/core.h"
#include "utilities/string_utility.h"
#include "validator/BstIdlError.h"
#include "validator/ModelValidator.h"
#include <algorithm>

namespace BstIdl
{
FDReferenceManager &FDReferenceManager::getInstance()
{
    static FDReferenceManager ins;
    return ins;
}

void FDReferenceManager::clear()
{
    m_propertyDeclRefs.clear();
    m_specificationExtendRefs.clear();
    m_extensionRootSpecificationRefs.clear();
    m_extensionRootTargetRefs.clear();
    m_extensionRootUseRefs.clear();
    m_extensionElementTargetRefs.clear();
    m_typeSpecificationsRefs.clear();
    m_typesTypeCollectionRefs.clear();
    m_typesUseRefs.clear();
    m_interfaceSpecificationRefs.clear();
    m_interfaceInterfaceRefs.clear();
    m_interfaceUseRefs.clear();
    m_attributeAttributeRefs.clear();
    m_methodTargetRefs.clear();
    m_argumentTargetRefs.clear();
    m_arrayTargetRefs.clear();
    m_structTargetRefs.clear();
    m_unionTargetRefs.clear();
    m_typeDefTargetRefs.clear();
    m_fieldTargetRefs.clear();
    m_enumerationTargetRefs.clear();
    m_enumValueTargetRefs.clear();
    m_mapTargetRefs.clear();
    m_interfaceValueRefs.clear();
    m_broadcastTargetRefs.clear();
}

void FDReferenceManager::addFDPropertyDeclRef(const std::shared_ptr<FDProperty> &property, const std::string &fqn)
{
    if (!exists(m_propertyDeclRefs, property, fqn))
        m_propertyDeclRefs.emplace_back(std::make_tuple(property, fqn));
}

void FDReferenceManager::addExtensionRootSpecificationRef(const std::shared_ptr<FDExtensionRoot> &extensionRoot,
                                                          const std::string &fqn)
{
    if (!exists(m_extensionRootSpecificationRefs, extensionRoot, fqn))
        m_extensionRootSpecificationRefs.emplace_back(std::make_tuple(extensionRoot, fqn));
}

void FDReferenceManager::addSpecificationExtendsRef(const std::shared_ptr<FDSpecification> &spec,
                                                    const std::string &fqn)
{
    if (!exists(m_specificationExtendRefs, spec, fqn))
        m_specificationExtendRefs.emplace_back(std::make_tuple(spec, fqn));
}

void FDReferenceManager::addExtensionRootTargetRef(const std::shared_ptr<FDExtensionRoot> &extensionRoot,
                                                   const std::string &fqn)
{
    if (!exists(m_extensionRootTargetRefs, extensionRoot, fqn))
        m_extensionRootTargetRefs.emplace_back(std::make_tuple(extensionRoot, fqn));
}

void FDReferenceManager::addExtensionRootUseRef(const std::shared_ptr<FDExtensionRoot> &extensionRoot,
                                                const std::string &fqn)
{
    if (!exists(m_extensionRootUseRefs, extensionRoot, fqn))
        m_extensionRootUseRefs.emplace_back(std::make_tuple(extensionRoot, fqn));
}

void FDReferenceManager::addExtensionElementTargetRef(const std::shared_ptr<FDExtensionElement> &extensionElement,
                                                      const std::string &fqn)
{
    if (!exists(m_extensionElementTargetRefs, extensionElement, fqn))
        m_extensionElementTargetRefs.emplace_back(std::make_tuple(extensionElement, fqn));
}

void FDReferenceManager::addTypesSpecificationRef(const std::shared_ptr<FDTypes> &types, const std::string &fqn)
{
    if (!exists(m_typeSpecificationsRefs, types, fqn))
        m_typeSpecificationsRefs.emplace_back(std::make_tuple(types, fqn));
}

void FDReferenceManager::addTypesTypeCollectionRef(const std::shared_ptr<FDTypes> &types, const std::string &fqn)
{
    if (!exists(m_typesTypeCollectionRefs, types, fqn))
        m_typesTypeCollectionRefs.emplace_back(std::make_tuple(types, fqn));
}

void FDReferenceManager::addTypesUseRef(const std::shared_ptr<FDTypes> &types, const std::string &fqn)
{
    if (!exists(m_typesUseRefs, types, fqn))
        m_typesUseRefs.emplace_back(std::make_tuple(types, fqn));
}

void FDReferenceManager::addInterfaceSpecificationRef(const std::shared_ptr<FDInterface> &interface,
                                                      const std::string &fqn)
{
    if (!exists(m_interfaceSpecificationRefs, interface, fqn))
        m_interfaceSpecificationRefs.emplace_back(std::make_tuple(interface, fqn));
}
void FDReferenceManager::addInterfaceInterfaceRef(const std::shared_ptr<FDInterface> &interface, const std::string &fqn)
{
    if (!exists(m_interfaceInterfaceRefs, interface, fqn))
        m_interfaceInterfaceRefs.emplace_back(std::make_tuple(interface, fqn));
}
void FDReferenceManager::addInterfaceUseRef(const std::shared_ptr<FDInterface> &interface, const std::string &fqn)
{
    if (!exists(m_interfaceUseRefs, interface, fqn))
        m_interfaceUseRefs.emplace_back(std::make_tuple(interface, fqn));
}
void FDReferenceManager::addAttributeAttributeRef(const std::shared_ptr<FDAttribute> &attribute, const std::string &fqn)
{
    if (!exists(m_attributeAttributeRefs, attribute, fqn))
        m_attributeAttributeRefs.emplace_back(std::make_tuple(attribute, fqn));
}
void FDReferenceManager::addMethodTargetRef(const std::shared_ptr<FDMethod> &method, const std::string &fqn)
{
    if (!exists(m_methodTargetRefs, method, fqn))
        m_methodTargetRefs.emplace_back(std::make_tuple(method, fqn));
}
void FDReferenceManager::addArgumentTargetRef(const std::shared_ptr<FDArgument> &argument, const std::string &fqn)
{
    if (!exists(m_argumentTargetRefs, argument, fqn))
        m_argumentTargetRefs.emplace_back(std::make_tuple(argument, fqn));
}
void FDReferenceManager::addArrayTargetRef(const std::shared_ptr<FDArray> &array, const std::string &fqn)
{
    if (!exists(m_arrayTargetRefs, array, fqn))
        m_arrayTargetRefs.emplace_back(std::make_tuple(array, fqn));
}
void FDReferenceManager::addFDStructTargetRef(const std::shared_ptr<FDStruct> &new_struct, const std::string &fqn)
{
    if (!exists(m_structTargetRefs, new_struct, fqn))
        m_structTargetRefs.emplace_back(std::make_tuple(new_struct, fqn));
}
void FDReferenceManager::addFDUnionTargetRef(const std::shared_ptr<FDUnion> &new_union, const std::string &fqn)
{
    if (!exists(m_unionTargetRefs, new_union, fqn))
        m_unionTargetRefs.emplace_back(std::make_tuple(new_union, fqn));
}
void FDReferenceManager::addFDTypedefTargetRef(const std::shared_ptr<FDTypedef> &new_typedef, const std::string &fqn)
{
    if (!exists(m_typeDefTargetRefs, new_typedef, fqn))
        m_typeDefTargetRefs.emplace_back(std::make_tuple(new_typedef, fqn));
}
void FDReferenceManager::addFDFieldTargetRef(const std::shared_ptr<FDField> &field, const std::string &fqn)
{
    if (!exists(m_fieldTargetRefs, field, fqn))
        m_fieldTargetRefs.emplace_back(std::make_tuple(field, fqn));
}
void FDReferenceManager::addFDEnumerationTargetRef(const std::shared_ptr<FDEnumeration> &enumeration,
                                                   const std::string &fqn)
{
    if (!exists(m_enumerationTargetRefs, enumeration, fqn))
        m_enumerationTargetRefs.emplace_back(std::make_tuple(enumeration, fqn));
}
void FDReferenceManager::addFDEnumValueTargetRef(const std::shared_ptr<FDEnumValue> &enumValue, const std::string &fqn)
{
    if (!exists(m_enumValueTargetRefs, enumValue, fqn))
        m_enumValueTargetRefs.emplace_back(std::make_tuple(enumValue, fqn));
}
void FDReferenceManager::addFDMapTargetRef(const std::shared_ptr<FDMap> &map, const std::string &fqn)
{
    if (!exists(m_mapTargetRefs, map, fqn))
        m_mapTargetRefs.emplace_back(std::make_tuple(map, fqn));
}
void FDReferenceManager::addFDInterfaceValueRef(const std::shared_ptr<FDInterfaceRef> &interfaceRef,
                                                const std::string &fqn)
{
    if (!exists(m_interfaceValueRefs, interfaceRef, fqn))
        m_interfaceValueRefs.emplace_back(std::make_tuple(interfaceRef, fqn));
}
void FDReferenceManager::addBroadcastTargetRef(const std::shared_ptr<FDBroadcast> &broadcast, const std::string &fqn)
{
    if (!exists(m_broadcastTargetRefs, broadcast, fqn))
        m_broadcastTargetRefs.emplace_back(std::make_tuple(broadcast, fqn));
}

std::list<std::string> FDReferenceManager::resolveFDPropertyDeclRef()
{
    auto &fdmgr = FDModelManager::getInstance();
    std::list<std::string> errorList;
    for (const auto &item : m_propertyDeclRefs)
    {
        auto property = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto ref = fdmgr.findFDPropertyDecl(fqn);
        if (ref != nullptr)
        {
            if (property->getDecl() != nullptr)
            {
                auto decl = property->getDecl();
                property->setDecl(nullptr);
                errorList.emplace_back(fmt::format("Duplicate reference: {0}, previous is {1}.", fqn, decl->getFQN()));
            }
            else
                property->setDecl(ref);
        }
        else
            errorList.emplace_back(fmt::format("Undefined reference: {0} in {1}", fqn, property->getFQN()));
    }
    return errorList;
}

std::list<std::string> FDReferenceManager::resolveExtensionRootSpecificationRef()
{
    auto &fdmgr = FDModelManager::getInstance();
    std::list<std::string> errorList;
    for (const auto &item : m_extensionRootSpecificationRefs)
    {
        auto extRoot = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto ref = fdmgr.findFDSpecification(fqn);
        if (ref && extRoot)
        {
            if (auto spec = extRoot->getSpec())
            {
                extRoot->setSpec(nullptr);
                errorList.emplace_back(fmt::format("Duplicate reference: {1}, previous is {2}, used by {0}.",
                                                   extRoot->getFQN(), fqn, spec->getFQN()));
            }
            else
            {
                extRoot->setSpec(ref);
            }
        }
        else
            errorList.emplace_back(fmt::format("Undefined reference: {0}", fqn));
    }
    return errorList;
}

std::list<std::string> FDReferenceManager::resolveExtensionRootTargetRef()
{
    auto &fmgr = FModelManager::getInstance();
    std::list<std::string> errorList;
    auto &fdmgr = FDModelManager::getInstance();
    for (const auto &item : m_extensionRootTargetRefs)
    {
        auto extRoot = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto ref = fmgr.findFObject(fqn);
        if (ref && extRoot)
        {
            if (auto target = extRoot->getTarget())
            {
                extRoot->setTarget(nullptr);
                errorList.emplace_back(fmt::format("Duplicate reference: {1}, previous is {2}, used by {0}.",
                                                   extRoot->getFQN(), fqn, target->getFQN()));
            }
            else
            {
                extRoot->setTarget(ref);
            }
        }
        else
            errorList.emplace_back(fmt::format("Undefined reference: {0}", fqn));
    }
    return errorList;
}

std::list<std::string> FDReferenceManager::resolveExtensionRootUseRef()
{
    auto &fdmgr = FDModelManager::getInstance();
    std::list<std::string> errorList;
    for (const auto &item : m_extensionRootUseRefs)
    {
        auto extRoot = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto ref = fdmgr.findFDRootElement(fqn);
        if (ref && extRoot)
        {
            extRoot->addUse(ref);
        }
        else
            errorList.emplace_back(fmt::format("Undefined reference: {0}", fqn));
    }
    return errorList;
}

std::list<std::string> FDReferenceManager::resolveExtensionElementTargetRef()
{
    auto &fmgr = FModelManager::getInstance();
    std::list<std::string> errorList;
    auto &fdmgr = FDModelManager::getInstance();
    for (const auto &item : m_extensionElementTargetRefs)
    {
        auto element = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto ref = fmgr.findFObject(fqn);
        if (ref && element)
        {
            if (auto target = element->getTarget())
            {
                element->setTarget(nullptr);
                errorList.emplace_back(
                    fmt::format("Duplicate reference: FDExtensionElement {1}, previous is {2}, used by {0}.",
                                element->getName(), fqn, target->getFQN()));
            }
            else
            {
                element->setTarget(ref);
            }
        }
        else
            errorList.emplace_back(fmt::format("Undefined reference: {0}", fqn));
    }
    return errorList;
}

std::list<std::string> FDReferenceManager::resolveSpecificationExtendsRef()
{
    auto &fdmgr = FDModelManager::getInstance();
    std::list<std::string> errorList;
    for (const auto &item : m_specificationExtendRefs)
    {
        auto specification = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto ref = fdmgr.findFDSpecification(fqn);
        if (ref != nullptr)
        {
            if (specification->getBase() != nullptr)
            {
                auto base = specification->getBase();
                specification->setBase(nullptr);
                errorList.emplace_back(fmt::format("Duplicate reference: {1}, previous is {2}, used by {0}.",
                                                   specification->getFQN(), fqn, base->getFQN()));
            }
            else
                specification->setBase(ref);
        }
        else
            errorList.emplace_back(fmt::format("Undefined reference: {0}", fqn));
    }
    return errorList;
}

std::list<std::string> FDReferenceManager::resolveTypesSpecificationRef()
{
    auto &fdmgr = FDModelManager::getInstance();
    std::list<std::string> errorList;
    for (const auto &item : m_typeSpecificationsRefs)
    {
        auto types = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto ref = fdmgr.findFDSpecification(fqn);
        if (ref != nullptr)
        {
            if (types->getSpec() != nullptr)
            {
                auto spec = types->getSpec();
                types->setSpec(nullptr);
                errorList.emplace_back(fmt::format("Duplicate reference: {1}, previous is {2}, used by {0}.",
                                                   types->getFQN(), fqn, spec->getFQN()));
            }
            else
                types->setSpec(ref);
        }
        else
            errorList.emplace_back(fmt::format("Undefined reference in type collection deployment: {0}", fqn));
    }
    return errorList;
}

std::list<std::string> FDReferenceManager::resolveTypesTypeCollectionRef()
{
    auto &fmgr = FModelManager::getInstance();
    std::list<std::string> errorList;
    auto &fdmgr = FDModelManager::getInstance();
    for (const auto &item : m_typesTypeCollectionRefs)
    {
        auto types = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto ref = fmgr.findFTypeCollection(fqn);
        if (ref != nullptr)
        {
            if (types->getTarget() != nullptr)
            {
                auto target = types->getTarget();
                types->setTarget(nullptr);
                errorList.emplace_back(fmt::format("Duplicate reference: {1}, previous is {2}, used by {0}.",
                                                   types->getFQN(), fqn, target->getFQN()));
            }
            else
                types->setTarget(ref);
        }
        else
            errorList.emplace_back(fmt::format("Undefined reference in type collection deployment: {0}", fqn));
    }
    return errorList;
}

std::list<std::string> FDReferenceManager::resolveTypesUseRef()
{
    auto &fdmgr = FDModelManager::getInstance();
    std::list<std::string> errorList;
    for (const auto &item : m_typesUseRefs)
    {
        auto types = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto ref = fdmgr.findFDRootElement(fqn);
        if (ref != nullptr)
        {
            auto uses = types->getUse();
            for (const auto &use : uses)
            {
                if (use == ref)
                {
                    errorList.emplace_back(fmt::format("Duplicate reference: {1}, previous is {2}, used by {0}.",
                                                       types->getFQN(), fqn, use->getFQN()));
                }
            }
            types->addUse(ref);
        }
        else
            errorList.emplace_back(fmt::format("Undefined reference: {0}", fqn));
    }
    return errorList;
}

std::list<std::string> FDReferenceManager::resolveInterfaceSpecificationRef()
{
    auto &fdmgr = FDModelManager::getInstance();
    std::list<std::string> errorList;
    for (const auto &item : m_interfaceSpecificationRefs)
    {
        auto interface = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto ref = fdmgr.findFDSpecification(fqn);
        if (ref != nullptr)
        {
            if (interface->getSpec() != nullptr)
            {
                auto spec = interface->getSpec();
                interface->setSpec(nullptr);
                errorList.emplace_back(fmt::format("Duplicate reference: {1}, previous is {2}, used by {0}.",
                                                   interface->getFQN(), fqn, spec->getFQN()));
            }
            else
                interface->setSpec(ref);
        }
        else
            errorList.emplace_back(fmt::format("Undefined reference: {0}", fqn));
    }
    return errorList;
}

std::list<std::string> FDReferenceManager::resolveInterfaceTargetRef()
{
    auto &fdmgr = FDModelManager::getInstance();
    auto &fmgr = FModelManager::getInstance();
    std::list<std::string> errorList;
    for (const auto &item : m_interfaceInterfaceRefs)
    {
        auto interface = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto ref = fmgr.findFInterface(fqn);
        if (ref != nullptr)
        {
            if (interface->getTarget() != nullptr)
            {
                auto target = interface->getTarget();
                interface->setTarget(nullptr);
                errorList.emplace_back(fmt::format("Duplicate reference: {1}, previous is {2}, used by {0}.",
                                                   interface->getFQN(), fqn, target->getFQN()));
            }
            else
                interface->setTarget(ref);
        }
        else
            errorList.emplace_back(fmt::format("Undefined reference: {0}", fqn));
    }
    return errorList;
}

std::list<std::string> FDReferenceManager::resolveInterfaceUseRef()
{
    auto &fdmgr = FDModelManager::getInstance();
    std::list<std::string> errorList;
    for (const auto &item : m_interfaceUseRefs)
    {
        auto interface = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto ref = fdmgr.findFDRootElement(fqn);
        if (ref != nullptr)
        {
            auto uses = interface->getUse();
            for (const auto &use : uses)
            {
                if (use == ref)
                {
                    errorList.emplace_back(fmt::format("Duplicate reference: {1}, previous is {2}, used by {0}.",
                                                       interface->getFQN(), fqn, use->getFQN()));
                }
            }
            interface->addUse(ref);
        }
        else
            errorList.emplace_back(fmt::format("Undefined reference: {0}", fqn));
    }
    return errorList;
}

std::list<std::string> FDReferenceManager::resolveAttributeTargetRef()
{
    auto &fdmgr = FDModelManager::getInstance();
    auto &fmgr = FModelManager::getInstance();
    std::list<std::string> errorList;
    for (const auto &item : m_attributeAttributeRefs)
    {
        auto attribute = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto ref = fmgr.findFAttribute(fqn);
        if (ref != nullptr)
        {
            if (attribute->getTarget() != nullptr)
            {
                auto target = attribute->getTarget();
                attribute->setTarget(nullptr);
                errorList.emplace_back(fmt::format("Duplicate reference: {1}, previous is {2}, used by {0}.",
                                                   attribute->getFQN(), fqn, target->getFQN()));
            }
            else
                attribute->setTarget(ref);
        }
        else
            errorList.emplace_back(fmt::format("Undefined reference: {0}", fqn));
    }
    return errorList;
}

std::list<std::string> FDReferenceManager::resolveMethodTargetRef()
{
    auto &fmgr = FModelManager::getInstance();
    std::list<std::string> errorList;
    for (const auto &item : m_methodTargetRefs)
    {
        auto method = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto ref = fmgr.findSymbol(fqn);
        if (ref != nullptr)
        {
            if (method->getTarget() != nullptr)
            {
                auto target = method->getTarget();
                method->setTarget(nullptr);
                errorList.emplace_back(fmt::format("Duplicate reference: {1}, previous is {2}, used by {0}.",
                                                   method->getFQN(), fqn, target->getFQN()));
            }
            else
                method->setTarget(std::dynamic_pointer_cast<BstIdl::FMethod>(ref));
        }
        else
            errorList.emplace_back(fmt::format("Undefined reference: fdmodel2.interface2.{0}", fqn));
    }
    return errorList;
}
std::list<std::string> FDReferenceManager::resolveArgumentTargetRef()
{
    auto &fmgr = FModelManager::getInstance();
    std::list<std::string> errorList;
    for (const auto &item : m_argumentTargetRefs)
    {
        auto argument = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto ref = fmgr.findSymbol(fqn);
        if (ref != nullptr)
        {
            if (argument->getTarget() != nullptr)
            {
                auto target = argument->getTarget();
                argument->setTarget(nullptr);
                errorList.emplace_back(fmt::format("Duplicate reference: {1}, previous is {2}, used by {0}.",
                                                   argument->getFQN(), fqn, target->getFQN()));
            }
            else
                argument->setTarget(std::dynamic_pointer_cast<BstIdl::FArgument>(ref));
        }
        else
            errorList.emplace_back(fmt::format("Undefined reference: {0}", fqn));
    }
    return errorList;
}
std::list<std::string> FDReferenceManager::resolveArrayTargetRef()
{
    auto &fdmgr = FDModelManager::getInstance();
    auto &fmgr = FModelManager::getInstance();
    std::list<std::string> errorList;
    for (const auto &item : m_arrayTargetRefs)
    {
        auto array = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto ref = std::dynamic_pointer_cast<FArrayType>(fmgr.findPublicAndPrivateFType(fqn));
        if (ref != nullptr)
        {
            if (array->getTarget() != nullptr)
            {
                auto target = array->getTarget();
                array->setTarget(nullptr);
                errorList.emplace_back(fmt::format("Duplicate reference: {1}, previous is {2}, used by {0}.",
                                                   array->getFQN(), fqn, target->getFQN()));
            }
            else
                array->setTarget(ref);
        }
        else
            errorList.emplace_back(fmt::format("Undefined reference: {0}", fqn));
    }
    return errorList;
}
std::list<std::string> FDReferenceManager::resolveFDStructTargetRef()
{
    auto &fdmgr = FDModelManager::getInstance();
    auto &fmgr = FModelManager::getInstance();
    std::list<std::string> errorList;
    for (const auto &item : m_structTargetRefs)
    {
        auto struct_ = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto ref = std::dynamic_pointer_cast<FStructType>(fmgr.findPublicAndPrivateFType(fqn));
        if (ref != nullptr)
        {
            if (struct_->getTarget() != nullptr)
            {
                auto target = struct_->getTarget();
                struct_->setTarget(nullptr);
                errorList.emplace_back(fmt::format("Duplicate reference: {1}, previous is {2}, used by {0}.",
                                                   struct_->getFQN(), fqn, target->getFQN()));
            }
            else
                struct_->setTarget(ref);
        }
        else
            errorList.emplace_back(fmt::format("Undefined reference: {0}", fqn));
    }
    return errorList;
}
std::list<std::string> FDReferenceManager::resolveFDUnionTargetRef()
{
    auto &fdmgr = FDModelManager::getInstance();
    auto &fmgr = FModelManager::getInstance();
    std::list<std::string> errorList;
    for (const auto &item : m_unionTargetRefs)
    {
        auto union_ = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto ref = std::dynamic_pointer_cast<FUnionType>(fmgr.findPublicAndPrivateFType(fqn));
        if (ref != nullptr)
        {
            if (union_->getTarget() != nullptr)
            {
                auto target = union_->getTarget();
                union_->setTarget(nullptr);
                errorList.emplace_back(fmt::format("Duplicate reference: {1}, previous is {2}, used by {0}.",
                                                   union_->getFQN(), fqn, target->getFQN()));
            }
            else
                union_->setTarget(ref);
        }
        else
            errorList.emplace_back(fmt::format("Undefined reference: {0}", fqn));
    }
    return errorList;
}
std::list<std::string> FDReferenceManager::resolveFDTypedefTargetRef()
{
    auto &fdmgr = FDModelManager::getInstance();
    auto &fmgr = FModelManager::getInstance();
    std::list<std::string> errorList;
    for (const auto &item : m_typeDefTargetRefs)
    {
        auto typeDef = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto ref = std::dynamic_pointer_cast<FTypeDef>(fmgr.findPublicAndPrivateFType(fqn));
        if (ref != nullptr)
        {
            if (typeDef->getTarget() != nullptr)
            {
                auto target = typeDef->getTarget();
                typeDef->setTarget(nullptr);
                errorList.emplace_back(fmt::format("Duplicate reference: {1}, previous is {2}, used by {0}.",
                                                   typeDef->getFQN(), fqn, target->getFQN()));
            }
            else
                typeDef->setTarget(ref);
        }
        else
            errorList.emplace_back(fmt::format("Undefined reference: {0}", fqn));
    }
    return errorList;
}
std::list<std::string> FDReferenceManager::resolveFDFieldTargetRef()
{
    auto &fmgr = FModelManager::getInstance();
    std::list<std::string> errorList;
    for (const auto &item : m_fieldTargetRefs)
    {
        auto field = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto ref = std::dynamic_pointer_cast<BstIdl::FField>(fmgr.findElement(fqn));
        if (ref != nullptr)
        {
            if (field->getTarget() != nullptr)
            {
                auto target = field->getTarget();
                field->setTarget(nullptr);
                errorList.emplace_back(
                    fmt::format("Duplicate reference: {0}, previous is {1}.", fqn, target->getFQN()));
            }
            else
                field->setTarget(ref);
        }
        else
            errorList.emplace_back(fmt::format("Undefined reference: {0}", fqn));
    }
    return errorList;
}
std::list<std::string> FDReferenceManager::resolveFDEnumerationTargetRef()
{
    auto &fdmgr = FDModelManager::getInstance();
    auto &fmgr = FModelManager::getInstance();
    std::list<std::string> errorList;
    for (const auto &item : m_enumerationTargetRefs)
    {
        auto enumeration = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto ref = std::dynamic_pointer_cast<FEnumerationType>(fmgr.findPublicAndPrivateFType(fqn));
        if (ref != nullptr)
        {
            if (enumeration->getTarget() != nullptr)
            {
                auto target = enumeration->getTarget();
                enumeration->setTarget(nullptr);
                errorList.emplace_back(fmt::format("Duplicate reference: {1}, previous is {2}, used by {0}.",
                                                   enumeration->getFQN(), fqn, target->getFQN()));
            }
            else
                enumeration->setTarget(ref);
        }
        else
            errorList.emplace_back(fmt::format("Undefined reference: {0}", fqn));
    }
    return errorList;
}
std::list<std::string> FDReferenceManager::resolveFDEnumValueTargetRef()
{
    auto &fmgr = FModelManager::getInstance();
    std::list<std::string> errorList;
    for (const auto &item : m_enumValueTargetRefs)
    {
        auto enumValue = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto ref = std::dynamic_pointer_cast<BstIdl::FEnumerator>(fmgr.findElement(fqn));
        if (ref != nullptr)
        {
            if (enumValue->getTarget() != nullptr)
            {
                auto target = enumValue->getTarget();
                enumValue->setTarget(nullptr);
                errorList.emplace_back(fmt::format("Duplicate reference: {1}, previous is {2}, used by {0}.",
                                                   enumValue->getFQN(), fqn, target->getFQN()));
            }
            else
                enumValue->setTarget(ref);
        }
        else
            errorList.emplace_back(fmt::format("Undefined reference: {0}", fqn));
    }
    return errorList;
}
std::list<std::string> FDReferenceManager::resolveFDMapTargetRef()
{
    auto &fmgr = FModelManager::getInstance();
    std::list<std::string> errorList;
    for (const auto &item : m_mapTargetRefs)
    {
        auto map_ = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto ref = std::dynamic_pointer_cast<FMapType>(fmgr.findPublicAndPrivateFType(fqn));
        if (ref != nullptr)
        {
            if (map_->getTarget() != nullptr)
            {
                auto target = map_->getTarget();
                map_->setTarget(nullptr);
                errorList.emplace_back(fmt::format("Duplicate reference: {1}, previous is {2}, used by {0}.",
                                                   map_->getFQN(), fqn, target->getFQN()));
            }
            else
                map_->setTarget(ref);
        }
        else
            errorList.emplace_back(fmt::format("Undefined reference: {0}", fqn));
    }
    return errorList;
}
std::list<std::string> FDReferenceManager::resolveFDInterfaceValueRef()
{
    auto &fmgr = FModelManager::getInstance();
    std::list<std::string> errorList;
    for (const auto &item : m_interfaceValueRefs)
    {
        auto interfaceValueRef = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto ref = fmgr.findFInterface(fqn);
        if (ref != nullptr)
        {
            if (interfaceValueRef->getTarget() != nullptr)
            {
                auto value = interfaceValueRef->getTarget();
                interfaceValueRef->setTarget(nullptr);
                errorList.emplace_back(fmt::format("Duplicate reference: {1}, previous is {2}, used by {0}.",
                                                   interfaceValueRef->getFQN(), fqn, value->getFQN()));
            }
            else
                interfaceValueRef->setTarget(ref);
        }
        else
            errorList.emplace_back(fmt::format("Undefined reference: {0}", fqn));
    }
    return errorList;
}
std::list<std::string> FDReferenceManager::resolveBroadcastTargetRef()
{
    auto &fmgr = FModelManager::getInstance();
    std::list<std::string> errorList;
    for (const auto &item : m_broadcastTargetRefs)
    {
        auto broadcast = std::get<0>(item);
        auto fqn = std::get<1>(item);
        auto ref = fmgr.findSymbol(fqn);
        if (ref != nullptr)
        {
            if (broadcast->getTarget() != nullptr)
            {
                auto target = broadcast->getTarget();
                broadcast->setTarget(nullptr);
                errorList.emplace_back(fmt::format("Duplicate reference: {1}, previous is {2}, used by {0}.",
                                                   broadcast->getFQN(), fqn, target->getFQN()));
            }
            else
                broadcast->setTarget(std::dynamic_pointer_cast<BstIdl::FBroadcast>(ref));
        }
        else
            errorList.emplace_back(fmt::format("Undefined reference: {0}", fqn));
    }
    return errorList;
}
bool FDReferenceManager::resolveAllRef()
{
    std::list<std::string> errorList;
    std::cout << "Resolving all references in deployments..." << std::endl;
    errorList.splice(errorList.end(), resolveSpecificationExtendsRef());
    errorList.splice(errorList.end(), resolveExtensionRootSpecificationRef());
    errorList.splice(errorList.end(), resolveExtensionRootTargetRef());
    errorList.splice(errorList.end(), resolveExtensionRootUseRef());
    errorList.splice(errorList.end(), resolveExtensionElementTargetRef());
    errorList.splice(errorList.end(), resolveTypesSpecificationRef());
    errorList.splice(errorList.end(), resolveTypesTypeCollectionRef());
    errorList.splice(errorList.end(), resolveTypesUseRef());
    errorList.splice(errorList.end(), resolveInterfaceSpecificationRef());
    errorList.splice(errorList.end(), resolveInterfaceTargetRef());
    errorList.splice(errorList.end(), resolveInterfaceUseRef());
    errorList.splice(errorList.end(), resolveAttributeTargetRef());
    errorList.splice(errorList.end(), resolveMethodTargetRef());
    errorList.splice(errorList.end(), resolveArgumentTargetRef());
    errorList.splice(errorList.end(), resolveArrayTargetRef());
    errorList.splice(errorList.end(), resolveFDStructTargetRef());
    errorList.splice(errorList.end(), resolveFDUnionTargetRef());
    errorList.splice(errorList.end(), resolveFDTypedefTargetRef());
    errorList.splice(errorList.end(), resolveFDFieldTargetRef());
    errorList.splice(errorList.end(), resolveFDEnumerationTargetRef());
    errorList.splice(errorList.end(), resolveFDEnumValueTargetRef());
    errorList.splice(errorList.end(), resolveFDMapTargetRef());
    errorList.splice(errorList.end(), resolveFDInterfaceValueRef());
    errorList.splice(errorList.end(), resolveBroadcastTargetRef());
    errorList.splice(errorList.end(), resolveFDPropertyDeclRef());

    bool ret = errorList.empty();
    if (ret)
        std::cout << "Success!" << std::endl;
    else
        std::cerr << join(errorList, "\n") << std::endl;
    return ret;
}

} // namespace BstIdl