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
#include "FDModel/FDBuiltInPropertyHost.h"
namespace BstIdl
{
static const std::shared_ptr<FDBuiltInPropertyHost> s_builtInPropertyHostTable[] = {
    std::make_shared<FDBuiltInPropertyHost>(FDBuiltInPropertyHost::UNDEFINED, "undefined", "undefined"),
    std::make_shared<FDBuiltInPropertyHost>(FDBuiltInPropertyHost::TYPE_COLLECTIONS, "type_collections",
                                            "type_collections"),
    std::make_shared<FDBuiltInPropertyHost>(FDBuiltInPropertyHost::INTERFACES, "interfaces", "interfaces"),
    std::make_shared<FDBuiltInPropertyHost>(FDBuiltInPropertyHost::ATTRIBUTES, "attributes", "attributes"),
    std::make_shared<FDBuiltInPropertyHost>(FDBuiltInPropertyHost::METHODS, "methods", "methods"),
    std::make_shared<FDBuiltInPropertyHost>(FDBuiltInPropertyHost::BROADCASTS, "broadcasts", "broadcasts"),
    std::make_shared<FDBuiltInPropertyHost>(FDBuiltInPropertyHost::ARGUMENTS, "arguments", "arguments"),
    std::make_shared<FDBuiltInPropertyHost>(FDBuiltInPropertyHost::STRUCT_FIELDS, "struct_fields", "struct_fields"),
    std::make_shared<FDBuiltInPropertyHost>(FDBuiltInPropertyHost::UNION_FIELDS, "union_fields", "union_fields"),
    std::make_shared<FDBuiltInPropertyHost>(FDBuiltInPropertyHost::ARRAYS, "arrays", "arrays"),
    std::make_shared<FDBuiltInPropertyHost>(FDBuiltInPropertyHost::STRUCTS, "structs", "structs"),
    std::make_shared<FDBuiltInPropertyHost>(FDBuiltInPropertyHost::UNIONS, "unions", "unions"),
    std::make_shared<FDBuiltInPropertyHost>(FDBuiltInPropertyHost::ENUMERATIONS, "enumerations", "enumerations"),
    std::make_shared<FDBuiltInPropertyHost>(FDBuiltInPropertyHost::ENUMERATORS, "enumerators", "enumerators"),
    std::make_shared<FDBuiltInPropertyHost>(FDBuiltInPropertyHost::TYPEDEFS, "typedefs", "typedefs"),
    std::make_shared<FDBuiltInPropertyHost>(FDBuiltInPropertyHost::STRINGS, "strings", "strings"),
    std::make_shared<FDBuiltInPropertyHost>(FDBuiltInPropertyHost::NUMBERS, "numbers", "numbers"),
    std::make_shared<FDBuiltInPropertyHost>(FDBuiltInPropertyHost::INTEGERS, "integers", "integers"),
    std::make_shared<FDBuiltInPropertyHost>(FDBuiltInPropertyHost::FLOATS, "floats", "floats"),
    std::make_shared<FDBuiltInPropertyHost>(FDBuiltInPropertyHost::BOOLEANS, "booleans", "booleans"),
    std::make_shared<FDBuiltInPropertyHost>(FDBuiltInPropertyHost::BYTE_BUFFERS, "byte_buffers", "byte_buffers"),
    std::make_shared<FDBuiltInPropertyHost>(FDBuiltInPropertyHost::FIELDS, "fields", "fields"),
    std::make_shared<FDBuiltInPropertyHost>(FDBuiltInPropertyHost::MAPS, "maps", "maps"),
    std::make_shared<FDBuiltInPropertyHost>(FDBuiltInPropertyHost::MAP_KEYS, "map_keys", "map_keys"),
    std::make_shared<FDBuiltInPropertyHost>(FDBuiltInPropertyHost::MAP_VALUES, "map_values", "map_values")};
std::shared_ptr<FDBuiltInPropertyHost> FDBuiltInPropertyHost::get(const std::string &literal)
{
    for (int i = 0; i < FDBuiltInPropertyHost::MAX_VALUE; ++i)
    {
        if (!s_builtInPropertyHostTable[i]->getLiteral().compare(literal))
            return s_builtInPropertyHostTable[i];
    }
    return s_builtInPropertyHostTable[UNDEFINED];
}

std::shared_ptr<FDBuiltInPropertyHost> FDBuiltInPropertyHost::getByName(const std::string &name)
{
    for (int i = 0; i < FDBuiltInPropertyHost::MAX_VALUE; ++i)
    {
        if (!s_builtInPropertyHostTable[i]->getName().compare(name))
            return s_builtInPropertyHostTable[i];
    }

    return s_builtInPropertyHostTable[UNDEFINED];
}

std::shared_ptr<FDBuiltInPropertyHost> FDBuiltInPropertyHost::get(uint32_t value)
{
    if (value < FDBuiltInPropertyHost::MAX_VALUE)
        return s_builtInPropertyHostTable[value];
    else
        return s_builtInPropertyHostTable[UNDEFINED];
}

} // namespace BstIdl