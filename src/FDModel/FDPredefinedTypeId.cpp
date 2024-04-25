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
#include "FDModel/FDPredefinedTypeId.h"
namespace BstIdl
{
static const std::shared_ptr<FDPredefinedTypeId> s_predefinedTypeTable[] = {
    std::make_shared<FDPredefinedTypeId>(FDPredefinedTypeId::UNDEFINED, "undefined", "undefined"),
    std::make_shared<FDPredefinedTypeId>(FDPredefinedTypeId::INTEGER, "Integer", "Integer"),
    std::make_shared<FDPredefinedTypeId>(FDPredefinedTypeId::STRING, "String", "String"),
    std::make_shared<FDPredefinedTypeId>(FDPredefinedTypeId::BOOLEAN, "Boolean", "Boolean"),
    std::make_shared<FDPredefinedTypeId>(FDPredefinedTypeId::INTERFACE, "Interface", "Interface")};
std::shared_ptr<FDPredefinedTypeId> FDPredefinedTypeId::get(const std::string &literal)
{
    for (int i = 0; i < 5; ++i)
    {
        if (!s_predefinedTypeTable[i]->getLiteral().compare(literal))
            return s_predefinedTypeTable[i];
    }
    return s_predefinedTypeTable[UNDEFINED];
}

std::shared_ptr<FDPredefinedTypeId> FDPredefinedTypeId::getByName(const std::string &name)
{
    for (int i = 0; i < 5; ++i)
    {
        if (!s_predefinedTypeTable[i]->getName().compare(name))
            return s_predefinedTypeTable[i];
    }

    return s_predefinedTypeTable[UNDEFINED];
}

std::shared_ptr<FDPredefinedTypeId> FDPredefinedTypeId::get(uint32_t value)
{
    if (value < 5)
        return s_predefinedTypeTable[value];
    else
        return s_predefinedTypeTable[UNDEFINED];
}

} // namespace BstIdl