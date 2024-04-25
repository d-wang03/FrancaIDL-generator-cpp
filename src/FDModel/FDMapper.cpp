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
#include "FDModel/FDMapper.h"
#include "FDModel/FDArray.h"
#include "FDModel/FDEnumeration.h"
#include "FDModel/FDMap.h"
#include "FDModel/FDStruct.h"
#include "FDModel/FDTypedef.h"
#include "FDModel/FDUnion.h"

#include <algorithm>
namespace BstIdl
{
void FDMapper::init(const std::shared_ptr<FDInterface> &fdi)
{
    if (!fdi || !fdi->getTarget())
        return;
    // init type collections
    initTypes(fdi->getTypes());

    // init global types referenced by 'use'
    auto use = fdi->getUse();
    initReferenced(use);

    // init existing deployed elements.
    m_mapping[fdi->getTarget()] = fdi;
    fdi->updatePropsFromSpec();
    for (auto e : fdi->getAttributes())
    {
        m_mapping[e->getTarget()] = e;
        e->updatePropsFromSpec();
    }
    for (auto e : fdi->getMethods())
    {
        m_mapping[e->getTarget()] = e;
        e->updatePropsFromSpec();
        initArguments(e->getInArguments());
        initArguments(e->getOutArguments());
    }
    for (auto e : fdi->getBroadcasts())
    {
        m_mapping[e->getTarget()] = e;
        e->updatePropsFromSpec();
        initArguments(e->getOutArguments());
    }

    // init missing deployed elements.
    auto inf = fdi->getTarget();
    for (auto e : inf->getAttributes())
    {
        if (m_mapping.count(e) == 0)
        {
            auto fdt = std::make_shared<FDAttribute>();
            fdt->setContainer(fdi);
            fdt->setTarget(e);
            addListItem(fdi->getAttributes(), fdt);
            m_mapping[e] = fdt;
            fdt->updatePropsFromSpec();
        }
    }
    for (auto e : inf->getMethods())
    {
        if (m_mapping.count(e) == 0)
        {
            auto fdt = std::make_shared<FDMethod>();
            fdt->setContainer(fdi);
            fdt->setTarget(e);
            addListItem(fdi->getMethods(), fdt);
            m_mapping[e] = fdt;
            fdt->updatePropsFromSpec();
        }
        auto fdm = std::dynamic_pointer_cast<FDMethod>(m_mapping[e]);
        initArguments(e->getInArgs(), fdm->getInArguments());
        initArguments(e->getOutArgs(), fdm->getOutArguments());
    }
    for (auto e : inf->getBroadcasts())
    {
        if (m_mapping.count(e) == 0)
        {
            auto fdt = std::make_shared<FDBroadcast>();
            fdt->setContainer(fdi);
            fdt->setTarget(e);
            addListItem(fdi->getBroadcasts(), fdt);
            m_mapping[e] = fdt;
            fdt->updatePropsFromSpec();
        }
        auto fdm = std::dynamic_pointer_cast<FDBroadcast>(m_mapping[e]);
        initArguments(e->getOutArgs(), fdm->getOutArguments());
    }
}
void FDMapper::initReferenced(std::list<std::shared_ptr<FDRootElement>> &used)
{
    std::list<std::shared_ptr<FDRootElement>> work;
    std::list<std::shared_ptr<FDTypes>> visited;
    for (auto it : used)
    {
        work.emplace_back(it);
    }
    while (!work.empty())
    {
        auto e = work.front();
        work.pop_front();
        if (std::dynamic_pointer_cast<FDTypes>(e) != nullptr)
        {
            auto fdTypes = std::dynamic_pointer_cast<FDTypes>(e);
            auto iter = std::find(visited.begin(), visited.end(), fdTypes);
            if (iter == visited.end())
            {
                visited.emplace_back(fdTypes);

                // init global types in this root element
                initTypeCollection(fdTypes);

                // add its referenced root elements to queue
                for (auto it : e->getUse())
                {
                    work.emplace_back(it);
                }
            }
        }
    }
}
void FDMapper::initTypeCollection(const std::shared_ptr<FDTypes> &fdTypes)
{
    if (!fdTypes || !fdTypes->getTarget())
        return;
    m_mapping[fdTypes->getTarget()] = fdTypes;
    fdTypes->updatePropsFromSpec();
    initTypes(fdTypes->getTypes());
    auto types = fdTypes->getTarget();
    initTypes(types->getTypes(), fdTypes);
}
void FDMapper::initTypes(const std::list<std::shared_ptr<FDTypeDefinition>> &fdTypes)
{
    for (auto t : fdTypes)
    {
        if (std::dynamic_pointer_cast<FDArray>(t) != nullptr)
        {
            m_mapping[std::dynamic_pointer_cast<FDArray>(t)->getTarget()] = t;
            t->updatePropsFromSpec();
        }
        else if (std::dynamic_pointer_cast<FDStruct>(t) != nullptr)
        {
            m_mapping[std::dynamic_pointer_cast<FDStruct>(t)->getTarget()] = t;
            for (auto f : std::dynamic_pointer_cast<FDStruct>(t)->getFields())
            {
                m_mapping[f->getTarget()] = f;
                f->updatePropsFromSpec();
            }
        }
        else if (std::dynamic_pointer_cast<FDUnion>(t) != nullptr)
        {
            m_mapping[std::dynamic_pointer_cast<FDUnion>(t)->getTarget()] = t;
            for (auto f : std::dynamic_pointer_cast<FDUnion>(t)->getFields())
            {
                m_mapping[f->getTarget()] = f;
                f->updatePropsFromSpec();
            }
        }
        else if (std::dynamic_pointer_cast<FDEnumeration>(t) != nullptr)
        {
            m_mapping[std::dynamic_pointer_cast<FDEnumeration>(t)->getTarget()] = t;
            for (auto e : std::dynamic_pointer_cast<FDEnumeration>(t)->getEnumerators())
            {
                m_mapping[e->getTarget()] = e;
                e->updatePropsFromSpec();
            }
        }
        else if (std::dynamic_pointer_cast<FDMap>(t) != nullptr)
        {
            m_mapping[std::dynamic_pointer_cast<FDMap>(t)->getTarget()] = t;
            t->updatePropsFromSpec();
        }
        else if (std::dynamic_pointer_cast<FDTypedef>(t) != nullptr)
        {
            m_mapping[std::dynamic_pointer_cast<FDTypedef>(t)->getTarget()] = t;
            t->updatePropsFromSpec();
        }
    }
}

void FDMapper::initTypes(const std::list<std::shared_ptr<FType>> &ftypes, const std::shared_ptr<FDTypes> &fdTypes)
{
    for (auto t : ftypes)
    {
        if (auto ptr = std::dynamic_pointer_cast<FArrayType>(t))
        {
            if (m_mapping.count(t) == 0)
            {
                auto fdt = std::make_shared<FDArray>();
                fdt->setContainer(fdTypes);
                fdt->setTarget(ptr);
                addListItem(fdTypes->getTypes(), fdt);
                m_mapping[t] = fdt;
                fdt->updatePropsFromSpec();
            }
        }
        else if (auto ptr = std::dynamic_pointer_cast<FStructType>(t))
        {
            if (m_mapping.count(t) == 0)
            {
                auto fdt = std::make_shared<FDStruct>();
                fdt->setContainer(fdTypes);
                fdt->setTarget(ptr);
                addListItem(fdTypes->getTypes(), fdt);
                m_mapping[t] = fdt;
                fdt->updatePropsFromSpec();
            }
            auto e = std::dynamic_pointer_cast<FDStruct>(m_mapping[t]);
            for (auto f : ptr->getElements())
            {
                if (m_mapping.count(f) == 0)
                {
                    auto fdt = std::make_shared<FDField>();
                    fdt->setContainer(e);
                    fdt->setTarget(f);
                    addListItem(e->getFields(), fdt);
                    m_mapping[f] = fdt;
                    fdt->updatePropsFromSpec();
                }
            }
        }
        else if (auto ptr = std::dynamic_pointer_cast<FUnionType>(t))
        {
            if (m_mapping.count(t) == 0)
            {
                auto fdt = std::make_shared<FDUnion>();
                fdt->setContainer(fdTypes);
                fdt->setTarget(ptr);
                addListItem(fdTypes->getTypes(), fdt);
                m_mapping[t] = fdt;
                fdt->updatePropsFromSpec();
            }
            auto e = std::dynamic_pointer_cast<FDUnion>(m_mapping[t]);
            for (auto f : ptr->getElements())
            {
                if (m_mapping.count(f) == 0)
                {
                    auto fdt = std::make_shared<FDField>();
                    fdt->setContainer(e);
                    fdt->setTarget(f);
                    addListItem(e->getFields(), fdt);
                    m_mapping[f] = fdt;
                    fdt->updatePropsFromSpec();
                }
            }
        }
        else if (auto ptr = std::dynamic_pointer_cast<FEnumerationType>(t))
        {
            if (m_mapping.count(t) == 0)
            {
                auto fdt = std::make_shared<FDEnumeration>();
                fdt->setContainer(fdTypes);
                fdt->setTarget(ptr);
                addListItem(fdTypes->getTypes(), fdt);
                m_mapping[t] = fdt;
                fdt->updatePropsFromSpec();
            }
            auto e = std::dynamic_pointer_cast<FDEnumeration>(m_mapping[t]);
            for (auto f : ptr->getEnumerators())
            {
                if (m_mapping.count(f) == 0)
                {
                    auto fdt = std::make_shared<FDEnumValue>();
                    fdt->setContainer(e);
                    fdt->setTarget(f);
                    addListItem(e->getEnumerators(), fdt);
                    m_mapping[f] = fdt;
                    fdt->updatePropsFromSpec();
                }
            }
        }
        else if (auto ptr = std::dynamic_pointer_cast<FMapType>(t))
        {
            if (m_mapping.count(t) == 0)
            {
                auto fdt = std::make_shared<FDMap>();
                fdt->setContainer(fdTypes);
                fdt->setTarget(ptr);
                addListItem(fdTypes->getTypes(), fdt);
                m_mapping[t] = fdt;
                fdt->updatePropsFromSpec();
            }
            auto e = std::dynamic_pointer_cast<FDMap>(m_mapping[t]);
            auto key = ptr->getKeyType();
            if (m_mapping.count(key) == 0)
            {
                auto fdt = std::make_shared<FDMapKey>();
                fdt->setContainer(e);
                fdt->setTarget(key);
                e->setKey(fdt);
                m_mapping[key] = fdt;
                fdt->updatePropsFromSpec();
            }
            auto val = ptr->getValueType();
            if (m_mapping.count(val) == 0)
            {
                auto fdt = std::make_shared<FDMapKey>();
                fdt->setContainer(e);
                fdt->setTarget(val);
                e->setKey(fdt);
                m_mapping[val] = fdt;
                fdt->updatePropsFromSpec();
            }
        }
        else if (auto ptr = std::dynamic_pointer_cast<FTypeDef>(t))
        {
            if (m_mapping.count(t) == 0)
            {
                auto fdt = std::make_shared<FDTypedef>();
                fdt->setContainer(fdTypes);
                fdt->setTarget(ptr);
                addListItem(fdTypes->getTypes(), fdt);
                m_mapping[t] = fdt;
                fdt->updatePropsFromSpec();
            }
        }
    }
}
void FDMapper::initArguments(const std::shared_ptr<FDArgumentList> &args)
{
    if (args == nullptr)
        return;
    for (auto arg : args->getArguments())
    {
        m_mapping[arg->getTarget()] = arg;
        arg->updatePropsFromSpec();
    }
}

void FDMapper::initArguments(const std::list<std::shared_ptr<FArgument>> &args,
                             const std::shared_ptr<FDArgumentList> &fdArgs)
{
    for (auto arg : args)
    {
        if (m_mapping.count(arg) == 0)
        {
            auto fdt = std::make_shared<FDArgument>();
            fdt->setContainer(fdArgs);
            fdt->setTarget(arg);
            addListItem(fdArgs->getArguments(), fdt);
            m_mapping[arg] = fdt;
            fdt->updatePropsFromSpec();
        }
    }
}

void FDMapper::initExtension(const std::shared_ptr<FDExtensionRoot> &fdExt)
{
    if (!fdExt)
        return;
    fdExt->updatePropsFromSpec();
    for (auto e : fdExt->getElements())
        if (e)
            e->updatePropsForAll();
}

} // namespace BstIdl