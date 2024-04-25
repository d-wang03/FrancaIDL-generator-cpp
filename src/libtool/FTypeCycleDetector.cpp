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
#include "libtool/FTypeCycleDetector.h"
namespace BstIdl
{
bool FTypeCycleDetector::tarjan(const std::shared_ptr<FType> &type)
{
    indices.emplace(type, index);
    lowlink.emplace(type, index);
    index = index + 1;
    stack.push_back(type);
    auto generatorExtentions = std::make_shared<FrancaGeneratorExtensions>();
    auto directlyReferencedTypes = generatorExtentions->getDirectlyReferencedTypes(type);
    for (auto referencedType : directlyReferencedTypes)
    {
        outErrorString = outErrorString + referencedType->getName() + "->";
        if (!indices.count(referencedType))
        {
            if (tarjan(referencedType))
                return true;
            lowlink.emplace(type, std::min(lowlink[type], lowlink[referencedType]));
        }
        else if (find(stack.begin(), stack.end(), referencedType) != stack.end())
        {
            lowlink.emplace(type, std::min(lowlink[type], indices[referencedType]));
        }
    }

    if (lowlink[type] == indices[type] && !(stack.back() == type))
    {
        stack.pop_back();
        outErrorString = outErrorString.substr(0, outErrorString.size() - 2);
        return true;
    }
    return false;
}

bool FTypeCycleDetector::hasCycle(const std::shared_ptr<FType> &type)
{
    indices.clear();
    lowlink.clear();
    while (!stack.empty())
        stack.pop_back();
    index = 0;
    outErrorString = type->getName() + "->";
    return tarjan(type);
}

bool FTypeCycleDetector::hasCycle(const std::list<std::shared_ptr<FType>> types)
{
    indices.clear();
    lowlink.clear();
    while (!stack.empty())
        stack.pop_back();
    index = 0;
    for (auto type : types)
    {
        if (!indices.count(type) && tarjan(type))
            return true;
    }
    return false;
}
} // namespace BstIdl