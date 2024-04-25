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
#ifndef FMODEL_MANAGER_H
#define FMODEL_MANAGER_H

#include "FArrayType.h"
#include "FAttribute.h"
#include "FBroadcast.h"
#include "FConstantDef.h"
#include "FEnumerationType.h"
#include "FInterface.h"
#include "FMethod.h"
#include "FModel.h"
#include "FStructType.h"
#include "FType.h"
#include "FTypeDef.h"
#include "FUnionType.h"
#include <list>
#include <map>
#include <memory>

namespace BstIdl
{

class FModelManager
{
public:
    FModelManager(const FModelManager &) = delete;
    FModelManager(FModelManager &&) noexcept = delete;

    static FModelManager &getInstance();

    std::shared_ptr<FModel> addModel(const std::string &fqn);
    std::shared_ptr<FModel> findModel(const std::string &fqn);
    const std::list<std::shared_ptr<FModel>> &getModelList() const;
    size_t size() const;
    bool empty() const;
    void clear();

    // find object from FQN (Full Qualified Name)
    std::shared_ptr<FObject> findFObject(const std::string &fqn);
    std::shared_ptr<FTypeCollection> findFTypeCollection(const std::string &fqn);
    std::shared_ptr<FInterface> findFInterface(const std::string &fqn);
    // find item in public type collections.
    std::shared_ptr<FType> findPublicFType(const std::string &fqn);
    std::shared_ptr<FEnumerationType> findPublicFEnumerationType(const std::string &fqn);
    std::shared_ptr<FStructType> findPublicFStructType(const std::string &fqn);
    std::shared_ptr<FUnionType> findPublicFUnionType(const std::string &fqn);
    std::shared_ptr<FConstantDef> findPublicConstant(const std::string &fqn);
    // find item in specified interface.
    std::shared_ptr<FType> findFTypeInInterface(const std::shared_ptr<FInterface> &interface, const std::string &fqn);
    std::shared_ptr<FEnumerationType> findFEnumerationTypeInInterface(const std::shared_ptr<FInterface> &interface,
                                                                      const std::string &fqn);
    std::shared_ptr<FStructType> findFStructTypeInInterface(const std::shared_ptr<FInterface> &interface,
                                                            const std::string &fqn);
    std::shared_ptr<FUnionType> findFUnionTypeInInterface(const std::shared_ptr<FInterface> &interface,
                                                          const std::string &fqn);
    std::shared_ptr<FConstantDef> findFConstantDefInInterface(const std::shared_ptr<FInterface> &interface,
                                                              const std::string &fqn);
    // find item in interfaces.
    std::shared_ptr<BstIdl::FAttribute> findFAttribute(const std::string &fqn);
    std::shared_ptr<BstIdl::FMethod> findFMethod(const std::string &fqn);
    std::shared_ptr<BstIdl::FBroadcast> findFBroadcast(const std::string &fqn);
    std::shared_ptr<BstIdl::FArgument> findFArgument(const std::string &fqn);
    std::shared_ptr<FConstantDef> findFConstantDef(const std::string &fqn);
    std::shared_ptr<FType> findPrivateFType(const std::string &fqn);
    // find item in all area: Public and Private (TypeCollections and Interfaces)
    std::shared_ptr<FType> findPublicAndPrivateFType(const std::string &fqn);

    bool addPendingFidl(const std::string &file);
    std::list<std::string> &getPendingFidls()
    {
        return m_pendingFidls;
    }
    bool addProcessedFidl(const std::string &file);

    void getDerivedFStructTypes(const std::shared_ptr<FModel> &model, const std::shared_ptr<FStructType> &object,
                                std::list<std::shared_ptr<FStructType>> &struct_types);

    std::list<std::shared_ptr<BstIdl::FInterface>> getBaseInterfaces(
        const std::shared_ptr<BstIdl::FInterface> &interface);

    template <typename T, typename D>
    std::shared_ptr<T> findInterfaceElement(const std::string &fqn, std::list<std::shared_ptr<T>> &(D::*func)())
    {
        if (fqn.empty())
            return nullptr;
        auto pos = fqn.find_last_of(".");
        if (pos == std::string::npos)
            return nullptr;

        auto element_name = fqn.substr(pos + 1);
        auto interface_name = fqn.substr(0, pos);
        auto interface = findFInterface(interface_name);
        return findInterfaceElement(interface, element_name, func);
    }

    template <typename T, typename D>
    std::shared_ptr<T> findInterfaceElement(const std::shared_ptr<FInterface> &interface,
                                            const std::string &element_name,
                                            std::list<std::shared_ptr<T>> &(D::*func)())
    {
        if (!interface || element_name.empty())
            return nullptr;

        auto interfaces = getBaseInterfaces(interface);
        for (auto inf : interfaces)
        {
            for (auto element : (inf.get()->*func)())
            {
                auto eleName = element->getName();
                auto md = std::dynamic_pointer_cast<BstIdl::FMethod>(element);
                auto bc = std::dynamic_pointer_cast<BstIdl::FBroadcast>(element);
                if (md && !md->getSelector().empty())
                {
                    eleName += ":" + md->getSelector();
                }
                else if (bc && !bc->getSelector().empty())
                {
                    eleName += ":" + bc->getSelector();
                }

                if (eleName == element_name)
                    return element;
            }
        }
        return nullptr;
    }

    bool buildSymbolTable();
    // This funtion will return the longest matching object. e.g. Package.Interface.Method.Argument.Field will return
    // the argument object.
    std::shared_ptr<BstIdl::FObject> findSymbol(const std::string &fqn);
    std::shared_ptr<BstIdl::FObject> findElement(const std::string &fqn);

private:
    FModelManager() = default;
    ~FModelManager() = default;

    std::list<std::shared_ptr<FModel>> m_models;
    std::map<std::string, std::shared_ptr<BstIdl::FObject>> m_globalSymTbl;
    std::list<std::string> m_pendingFidls;
    std::list<std::string> m_processedFidls;

    bool addSymbol(const std::string &fqn, const std::shared_ptr<BstIdl::FObject> &object)
    {
        if (fqn.empty() || !object || m_globalSymTbl.count(fqn))
            return false;
        m_globalSymTbl[fqn] = object;
        return true;
    }

    void addSymbol(const std::string &fqn, const std::shared_ptr<BstIdl::FObject> &object,
                   std::list<std::string> &errorList)
    {
        if (fqn.empty() || !object)
        {
            errorList.emplace_back("fqn is empty, or object is nullptr.");
        }
        if (!addSymbol(fqn, object))
            errorList.emplace_back("Duplicate Item : " + fqn);
    }
};
} // namespace BstIdl

#endif