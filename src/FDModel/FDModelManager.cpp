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
#include "FDModel/FDModelManager.h"
#include "FDModel/FDFactory.h"
#include "utilities/list_utility.h"
#include <iostream>
#include <memory>

namespace BstIdl
{
FDModelManager &FDModelManager::getInstance()
{
    static FDModelManager ins;
    return ins;
}
std::string FDModelManager::makeModelName(std::string name)
{
    if (findModel(name))
        return name + std::to_string(anonymousModel++);
    return name;
}

std::shared_ptr<FDModel> FDModelManager::addModel(const std::string &fqn)
{
    if (fqn.empty())
        return nullptr;

    auto ret = findModel(fqn);
    if (ret == nullptr)
    {
        ret = FDFactory::getInstance().createFDModel();
        ret->setName(fqn);
        m_models.emplace_back(ret);
    }

    return ret;
}
std::shared_ptr<FDModel> FDModelManager::findModel(const std::string &fqn)
{
    auto it = std::find_if(m_models.begin(), m_models.end(),
                           [fqn](const std::shared_ptr<FDModel> &model) { return (model && model->getName() == fqn); });
    return it != m_models.end() ? *it : nullptr;
}

std::shared_ptr<FDSpecification> FDModelManager::findFDSpecification(const std::string &fqn)
{
    if (fqn.empty())
        return nullptr;

    for (auto &model : m_models)
    {
        if (!model)
            continue;

        for (auto &specification : model->getSpecifications())
        {
            if (specification->getName().compare(fqn) == 0)
                return specification;
        }
    }
    return nullptr;
}

std::shared_ptr<FDPropertyDecl> FDModelManager::findFDPropertyDecl(const std::string &fqn)
{
    if (fqn.empty())
        return nullptr;
    auto last_position = fqn.find_last_of(".");
    auto specName = fqn.substr(0, last_position);
    auto propertyDeclName = fqn.substr(last_position + 1);
    auto spec = findFDSpecification(specName);
    if (spec != nullptr)
    {
        auto decls = spec->getAllDeclarations();
        for (auto decl : decls)
        {
            auto propertyDecls = decl->getProperties();
            for (auto propertyDecl : propertyDecls)
            {
                auto propertyDeclName_ = propertyDecl->getName();
                if (propertyDeclName == propertyDeclName_)
                    return propertyDecl;
            }
        }
        return nullptr;
    }
    else
    {
        return nullptr;
    }
}

std::shared_ptr<FDRootElement> FDModelManager::findFDRootElement(const std::string &fqn)
{
    if (fqn.empty())
        return nullptr;

    for (auto &model : m_models)
    {
        if (!model)
            continue;

        for (auto &deployment : model->getDeployments())
        {
            if (deployment->getName().compare(fqn) == 0)
                return deployment;
        }
    }
    return nullptr;
}

std::shared_ptr<FDInterface> FDModelManager::findInterfaceDeployment(
    const std::string &spec, const std::shared_ptr<BstIdl::FInterface> &target) const
{
    if (spec.empty() || !target)
        return nullptr;

    for (const auto &model : m_models)
    {
        auto fdInterface = model->getInterfaceDeployment(spec, target->getName());
        if (fdInterface != nullptr && fdInterface->getTarget() == target)
            return fdInterface;
    }
    return nullptr;
}

std::shared_ptr<FDExtensionRoot> FDModelManager::findProviderDeployment(const std::string &spec) const
{
    if (spec.empty())
        return nullptr;

    for (const auto &model : m_models)
    {
        auto fdprovider = model->getProviderDeployment(spec);
        if (fdprovider != nullptr)
            return fdprovider;
    }
    return nullptr;
}

std::shared_ptr<FDExtensionElement> FDModelManager::findInstance(const std::string &spec,
                                                                 const std::string &targetName) const
{
    if (spec.empty() || targetName.empty())
        return nullptr;

    auto provider = findProviderDeployment(spec);
    if (!provider)
        return nullptr;
    return provider->getInstance(targetName);
}

std::string FDModelManager::getFQN(const std::shared_ptr<FDInterfaceRef> &object) const
{
    auto interface = object->getTarget();
    return interface ? interface->getFQN() : std::string();
}

const std::list<std::shared_ptr<FDModel>> &FDModelManager::getModelList() const
{
    return m_models;
}

size_t FDModelManager::size() const
{
    return m_models.size();
}

bool FDModelManager::empty() const
{
    return m_models.empty();
}

void FDModelManager::clear()
{
    m_models.clear();
    pendingModels.clear();
    processedModels.clear();
    anonymousModel = 0;
}

bool FDModelManager::addPendingModel(const std::string &file)
{
    if (file.empty())
    {
        std::cerr << "file name is null\n";
        return false;
    }

    auto ret = findProcessedModel(file);
    if (ret == false)
    {
        addListItem(pendingModels, file);
    }
    return true;
}

std::list<std::string> &FDModelManager::getPendingModels()
{
    return pendingModels;
}

bool FDModelManager::addProcessedModel(const std::string &file)
{
    if (file.empty())
    {
        std::cerr << "file name is null\n";
        return false;
    }

    addListItem(processedModels, file);
    return true;
}

bool FDModelManager::findProcessedModel(const std::string &file)
{
    if (file.empty())
    {
        std::cerr << "file name is null\n";
        return false;
    }
    auto it = std::find(processedModels.begin(), processedModels.end(), file);
    if (it != processedModels.end())
        return true;
    return false;
}

void FDModelManager::createMapper()
{
    for (auto m : m_models)
    {
        m->buildMappers();
    }
}

} // namespace BstIdl