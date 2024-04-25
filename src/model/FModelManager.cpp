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
#include "model/FModelManager.h"
#include "model/FFactory.h"
#include <algorithm>
#include <iostream>

namespace BstIdl
{
FModelManager &FModelManager::getInstance()
{
    static FModelManager ins;
    return ins;
}

std::shared_ptr<FModel> FModelManager::addModel(const std::string &fqn)
{
    if (fqn.empty())
        return nullptr;

    auto ret = findModel(fqn);
    if (ret == nullptr)
    {
        ret = FFactory::getInstance().createFModel();
        ret->setName(fqn);
        m_models.emplace_back(ret);
    }

    return ret;
}
std::shared_ptr<FModel> FModelManager::findModel(const std::string &fqn)
{
    auto it = std::find_if(m_models.begin(), m_models.end(),
                           [fqn](const std::shared_ptr<FModel> &model) { return (model && model->getName() == fqn); });
    return it != m_models.end() ? *it : nullptr;
}

const std::list<std::shared_ptr<FModel>> &FModelManager::getModelList() const
{
    return m_models;
}

size_t FModelManager::size() const
{
    return m_models.size();
}

bool FModelManager::empty() const
{
    return m_models.empty();
}

void FModelManager::clear()
{
    m_models.clear();
    m_pendingFidls.clear();
    m_processedFidls.clear();
    m_globalSymTbl.clear();
}

std::shared_ptr<FObject> FModelManager::findFObject(const std::string &fqn)
{
    if (auto ret = findFTypeCollection(fqn))
        return ret;
    if (auto ret = findFInterface(fqn))
        return ret;
    if (auto ret = findPublicFType(fqn))
        return ret;

    return nullptr;
}

std::shared_ptr<FTypeCollection> FModelManager::findFTypeCollection(const std::string &fqn)
{
    if (fqn.empty())
        return nullptr;

    auto pos = fqn.find_last_of(".");
    if (pos == std::string::npos)
        return nullptr;
    auto element_name = fqn.substr(pos + 1);
    auto model_name = fqn.substr(0, pos);
    auto model = findModel(model_name);
    if (!model || model_name.empty())
        return nullptr;
    for (auto &item : model->getTypeCollections())
    {
        if (item->getName().compare(element_name) == 0)
            return item;
    }
    return nullptr;
}

std::shared_ptr<FType> FModelManager::findPublicFType(const std::string &fqn)
{
    if (fqn.empty())
        return nullptr;

    auto pos = fqn.find_last_of(".");
    if (pos == std::string::npos)
        return nullptr;

    auto element_name = fqn.substr(pos + 1);
    auto tc_name = fqn.substr(0, pos);
    auto tc = findFTypeCollection(tc_name);
    if (tc && !element_name.empty())
    {
        for (auto element : tc->getTypes())
        {
            if (element->getName() == element_name)
                return element;
        }
    }
    return nullptr;
}

std::shared_ptr<FType> FModelManager::findPrivateFType(const std::string &fqn)
{
    return findInterfaceElement(fqn, &FInterface::getTypes);
}

std::shared_ptr<FType> FModelManager::findPublicAndPrivateFType(const std::string &fqn)
{
    auto ret = findPublicFType(fqn);
    if (!ret)
        ret = findPrivateFType(fqn);
    return ret;
}

std::shared_ptr<FInterface> FModelManager::findFInterface(const std::string &fqn)
{
    if (fqn.empty())
        return nullptr;

    auto pos = fqn.find_last_of(".");
    if (pos == std::string::npos)
        return nullptr;
    auto element_name = fqn.substr(pos + 1);
    auto model_name = fqn.substr(0, pos);
    auto model = findModel(model_name);
    if (!model || model_name.empty())
        return nullptr;
    for (auto &item : model->getInterfaces())
    {
        if (item->getName().compare(element_name) == 0)
            return item;
    }
    return nullptr;
}

std::shared_ptr<FEnumerationType> FModelManager::findPublicFEnumerationType(const std::string &fqn)
{
    return std::dynamic_pointer_cast<FEnumerationType>(findPublicFType(fqn));
}

std::shared_ptr<FStructType> FModelManager::findPublicFStructType(const std::string &fqn)
{
    return std::dynamic_pointer_cast<FStructType>(findPublicFType(fqn));
}

std::shared_ptr<FUnionType> FModelManager::findPublicFUnionType(const std::string &fqn)
{
    return std::dynamic_pointer_cast<FUnionType>(findPublicFType(fqn));
}

std::shared_ptr<FConstantDef> FModelManager::findPublicConstant(const std::string &fqn)
{
    if (fqn.empty())
        return nullptr;

    auto pos = fqn.find_last_of(".");
    if (pos == std::string::npos)
        return nullptr;
    auto element_name = fqn.substr(pos + 1);
    auto parent_name = fqn.substr(0, pos);
    auto parent = findFTypeCollection(parent_name);
    if (!parent || element_name.empty())
        return nullptr;

    for (auto &item : parent->getConstants())
    {
        if (item->getName().compare(element_name) == 0)
            return item;
    }
    return nullptr;
}

std::shared_ptr<FType> FModelManager::findFTypeInInterface(const std::shared_ptr<FInterface> &interface,
                                                           const std::string &fqn)
{
    if (interface == nullptr || fqn.empty() || !contains(fqn, interface->getFQN() + "."))
        return nullptr;

    auto pos = fqn.find_last_of(".");
    if (pos == std::string::npos)
        return nullptr;
    auto element_name = fqn.substr(pos + 1);
    return findInterfaceElement(interface, element_name, &FInterface::getTypes);
}

std::shared_ptr<FEnumerationType> FModelManager::findFEnumerationTypeInInterface(
    const std::shared_ptr<FInterface> &interface, const std::string &fqn)
{
    return std::dynamic_pointer_cast<FEnumerationType>(findFTypeInInterface(interface, fqn));
}

std::shared_ptr<FStructType> FModelManager::findFStructTypeInInterface(const std::shared_ptr<FInterface> &interface,
                                                                       const std::string &fqn)
{
    return std::dynamic_pointer_cast<FStructType>(findFTypeInInterface(interface, fqn));
}

std::shared_ptr<FUnionType> FModelManager::findFUnionTypeInInterface(const std::shared_ptr<FInterface> &interface,
                                                                     const std::string &fqn)
{
    return std::dynamic_pointer_cast<FUnionType>(findFTypeInInterface(interface, fqn));
}

std::shared_ptr<FConstantDef> FModelManager::findFConstantDefInInterface(const std::shared_ptr<FInterface> &interface,
                                                                         const std::string &fqn)
{
    if (interface == nullptr || fqn.empty() || !contains(fqn, interface->getFQN()))
        return nullptr;

    auto pos = fqn.find_last_of(".");
    if (pos == std::string::npos)
        return nullptr;
    auto element_name = fqn.substr(pos + 1);
    return findInterfaceElement(interface, element_name, &FInterface::getConstants);
}

bool FModelManager::addPendingFidl(const std::string &filename)
{
    if (filename.empty())
        return false;

    // duplication is ok.
    (void)addListItem(m_pendingFidls, filename);
    return true;
}

bool FModelManager::addProcessedFidl(const std::string &filename)
{
    if (filename.empty())
        return false;

    addListItem(m_processedFidls, filename);
    return true;
}

void FModelManager::getDerivedFStructTypes(const std::shared_ptr<FModel> &model,
                                           const std::shared_ptr<FStructType> &object,
                                           std::list<std::shared_ptr<FStructType>> &struct_types)
{
    if (object == nullptr || model == nullptr)
        return;
    auto interfaces = model->getInterfaces();
    for (auto interface : interfaces)
    {
        const auto &types = interface->getTypes();
        for (auto type : types)
        {
            if (std::dynamic_pointer_cast<FStructType>(type) != nullptr &&
                std::dynamic_pointer_cast<FStructType>(type)->getBase() == object)
                struct_types.emplace_back(std::dynamic_pointer_cast<FStructType>(type));
        }
    }
    auto typeCollections = model->getTypeCollections();
    for (auto typeCollection : typeCollections)
    {
        const auto &types = typeCollection->getTypes();
        for (auto type : types)
        {
            if (std::dynamic_pointer_cast<FStructType>(type) != nullptr &&
                std::dynamic_pointer_cast<FStructType>(type)->getBase() == object)
                struct_types.emplace_back(std::dynamic_pointer_cast<FStructType>(type));
        }
    }
    return;
}

std::list<std::shared_ptr<BstIdl::FInterface>> FModelManager::getBaseInterfaces(
    const std::shared_ptr<BstIdl::FInterface> &interface)
{
    std::list<std::shared_ptr<BstIdl::FInterface>> ret;
    auto base = interface;
    while (base != nullptr)
    {
        ret.emplace_back(base);
        base = base->getBase();
    }
    return ret;
}

std::shared_ptr<BstIdl::FAttribute> FModelManager::findFAttribute(const std::string &fqn)
{
    return findInterfaceElement(fqn, &FInterface::getAttributes);
}

std::shared_ptr<BstIdl::FMethod> FModelManager::findFMethod(const std::string &fqn)
{
    return findInterfaceElement(fqn, &FInterface::getMethods);
}

std::shared_ptr<BstIdl::FBroadcast> FModelManager::findFBroadcast(const std::string &fqn)
{
    return findInterfaceElement(fqn, &FInterface::getBroadcasts);
}

std::shared_ptr<FConstantDef> FModelManager::findFConstantDef(const std::string &fqn)
{
    return findInterfaceElement(fqn, &FInterface::getConstants);
}

std::shared_ptr<BstIdl::FArgument> FModelManager::findFArgument(const std::string &fqn)
{
    auto pos = fqn.find_last_of(".");
    if (pos == std::string::npos)
        return nullptr;

    auto arg_name = fqn.substr(pos + 1);
    auto parent_name = fqn.substr(0, pos);
    auto method = findFMethod(parent_name);
    if (method)
    {
        for (const auto &item : method->getInArgs())
        {
            if (item->getName() == arg_name)
                return item;
        }
        for (const auto &item : method->getOutArgs())
        {
            if (item->getName() == arg_name)
                return item;
        }
    }
    auto broadcast = findFBroadcast(parent_name);
    if (broadcast)
    {
        for (const auto &item : broadcast->getOutArgs())
        {
            if (item->getName() == arg_name)
                return item;
        }
    }
    return nullptr;
}

bool FModelManager::buildSymbolTable()
{
    m_globalSymTbl.clear();
    std::cout << "Building symbolTable ..." << std::endl;
    auto &mgr = FModelManager::getInstance();
    std::list<std::string> errorList;
    for (const auto &model : mgr.getModelList())
    {
        addSymbol(model->getFQN(), model, errorList);
        for (const auto &item : model->getTypeCollections())
        {
            addSymbol(item->getFQN(), item, errorList);
            for (const auto &type : item->getTypes())
            {
                addSymbol(type->getFQN(), type, errorList);
            }
            for (const auto &constant : item->getConstants())
            {
                addSymbol(constant->getFQN(), constant, errorList);
            }
        }
        for (const auto &item : model->getInterfaces())
        {
            addSymbol(item->getFQN(), item, errorList);
            for (const auto &type : item->getTypes())
            {
                addSymbol(type->getFQN(), type, errorList);
            }
            for (const auto &constant : item->getConstants())
            {
                addSymbol(constant->getFQN(), constant, errorList);
            }
            for (const auto &i : item->getAttributes())
            {
                addSymbol(i->getFQN(), i, errorList);
            }
            for (const auto &i : item->getMethods())
            {
                addSymbol(i->getFQN(), i, errorList);
                for (auto a : i->getInArgs())
                    addSymbol(a->getFQN(), a, errorList);
                for (auto a : i->getOutArgs())
                    addSymbol(a->getFQN(), a, errorList);
            }
            for (const auto &i : item->getBroadcasts())
            {
                addSymbol(i->getFQN(), i, errorList);
                for (auto a : i->getOutArgs())
                    addSymbol(a->getFQN(), a, errorList);
            }
        }
    }

    bool ret = errorList.empty();
    if (ret)
        std::cout << "Success!" << std::endl;
    else
        std::cerr << join(errorList, "\n") << std::endl;
    return ret;
}

std::shared_ptr<BstIdl::FObject> FModelManager::findSymbol(const std::string &fqn)
{
    if (fqn.empty() || m_globalSymTbl.empty())
        return nullptr;

    auto subFqn = fqn;
    while (!subFqn.empty())
    {
        auto iter = m_globalSymTbl.find(subFqn);
        if (iter != m_globalSymTbl.end())
            return iter->second;
        auto pos = subFqn.find_last_of('.');
        if (pos == std::string::npos)
            pos = 0;
        subFqn = subFqn.substr(0, pos);
    }
    return nullptr;
}

std::shared_ptr<BstIdl::FObject> FModelManager::findElement(const std::string &fqn)
{
    auto obj = findSymbol(fqn);
    if (!obj)
    {
        std::cerr << fmt::format("Wrong FQN: {0}", fqn) << std::endl;
        return nullptr;
    }
    auto objFqn = obj->getFQN();
    if (objFqn.length() == fqn.length())
    {
        // the element is found in global symbol table.
        return obj;
    }

    // find in interface's bases
    auto restFqn = fqn.substr(objFqn.size() + 1);
    auto splittedFqn = split(restFqn, ".");

    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FInterface>(obj))
    {
        auto next = splittedFqn.front();
        splittedFqn.pop_front();
        auto nextObj = objFqn + "." + next;
        auto &mgr = BstIdl::FModelManager::getInstance();
        obj = mgr.findInterfaceElement(ptr, next, &BstIdl::FInterface::getTypes);
        if (!obj)
        {
            std::cerr << fmt::format("overwrited element not found:{0}", nextObj) << std::endl;
            return nullptr;
        }
    }

    // pre-treatment
    std::shared_ptr<BstIdl::FType> type = nullptr;
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FArgument>(obj))
        type = ptr->getType()->getDerived();
    else if (auto ptr = std::dynamic_pointer_cast<BstIdl::FAttribute>(obj))
        type = ptr->getType()->getDerived();
    else if (auto ptr = std::dynamic_pointer_cast<BstIdl::FType>(obj))
        type = ptr;
    else if (auto ptr = std::dynamic_pointer_cast<BstIdl::FField>(obj))
        type = ptr->getType()->getDerived();
    else if (auto ptr = std::dynamic_pointer_cast<BstIdl::FType>(obj))
        type = ptr;

    if (!type)
    {
        std::cerr << fmt::format("name of overwrite Element is wrong:{0} is not in {1}",
                                 fqn.substr(obj->getFQN().size() + 1), objFqn)
                  << std::endl;
        return nullptr;
    }

    // find in FType
    auto element = type->findElement(splittedFqn);
    if (!element)
    {
        std::cerr << fmt::format("name of overwrite Element is wrong:{0} ", fqn) << std::endl;
        return nullptr;
    }
    return element;
}

} // namespace BstIdl