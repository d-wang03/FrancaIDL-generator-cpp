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
#include "FDModel/FDModel.h"
#include "utilities/list_utility.h"
#include "utilities/string_utility.h"

namespace BstIdl
{
std::string FDModel::toGraph()
{
    std::string ret = "@startuml ";
    setName(getName().empty() ? "FDModel" : getName());
    ret += getName() + "\n";
    ret += "object \"" + getName() + "\" as model\n";

    for (const auto &item : m_specifications)
    {
        ret += item->toGraph();
        ret += "model-->" + item->getInstanceName() + ":specification\n";
    }
    for (const auto &item : m_deployments)
    {
        ret += item->toGraph();
        ret += "model-->" + item->getInstanceName() + ":deployment\n";
    }
    ret += "@enduml\n";
    return ret;
}

void FDModel::buildMappers()
{
    for (auto d : m_deployments)
    {
        m_mappers[d] = std::make_shared<FDMapper>(d);
    }
}

std::shared_ptr<FDMapper> FDModel::getMapper(const std::shared_ptr<FDRootElement> &deploy) const
{
    if (m_mappers.count(deploy))
        return m_mappers.at(deploy);
    else
        return nullptr;
}

std::shared_ptr<FDInterface> FDModel::getInterfaceDeployment(const std::string &spec, const std::string &name) const
{
    for (auto d : m_deployments)
    {
        auto ptr = std::dynamic_pointer_cast<FDInterface>(d);
        if (!ptr)
            continue;
        auto specNames = d->getSpec()->getSpecNames();
        if (!contains(join(specNames, ","), spec))
            continue;
        auto depl_name = d->getName().substr(d->getName().find_last_of(".") + 1);
        auto target_name = name;
        if (contains(name, "."))
            target_name = name.substr(name.find_last_of(".") + 1);
        if (target_name.compare(depl_name) == 0)
            return ptr;
    }
    return nullptr;
}

std::shared_ptr<FDTypes> FDModel::getTypeCollectionDeployment(const std::string &spec, const std::string &name) const
{
    for (auto d : m_deployments)
    {
        auto ptr = std::dynamic_pointer_cast<FDTypes>(d);
        if (!ptr)
            continue;
        auto specNames = d->getSpec()->getSpecNames();
        if (!contains(join(specNames, ","), spec))
            continue;
        auto depl_name = d->getName().substr(d->getName().find_last_of(".") + 1);
        auto target_name = name;
        if (contains(name, "."))
            target_name = name.substr(name.find_last_of(".") + 1);
        if (target_name.compare(depl_name) == 0)
            return ptr;
    }
    return nullptr;
}

std::shared_ptr<FDExtensionRoot> FDModel::getProviderDeployment(const std::string &spec) const
{
    for (auto d : m_deployments)
    {
        auto ptr = std::dynamic_pointer_cast<FDExtensionRoot>(d);
        if (!ptr || ptr->getTag() != "provider")
            continue;
        auto specNames = d->getSpec()->getSpecNames();
        if (contains(join(specNames, ","), spec))
            return ptr;
    }
    return nullptr;
}

std::list<std::shared_ptr<FDInterface>> FDModel::getInterfaceDeployments(const std::string &spec) const
{
    std::list<std::shared_ptr<FDInterface>> ret;
    for (auto d : m_deployments)
    {
        auto ptr = std::dynamic_pointer_cast<FDInterface>(d);
        if (!ptr)
            continue;
        auto specNames = d->getSpec()->getSpecNames();
        if (contains(join(specNames, ","), spec))
            ret.emplace_back(ptr);
    }
    return ret;
}

} // namespace BstIdl