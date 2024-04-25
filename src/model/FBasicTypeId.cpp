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
#include "model/FBasicTypeId.h"

namespace BstIdl
{
static const std::shared_ptr<FBasicTypeId> s_basicTypeTable[] = {
    std::make_shared<FBasicTypeId>(FBasicTypeId::UNDEFINED, "undefined", "undefined"),
    std::make_shared<FBasicTypeId>(FBasicTypeId::INT8, "Int8", "Int8"),
    std::make_shared<FBasicTypeId>(FBasicTypeId::UINT8, "UInt8", "UInt8"),
    std::make_shared<FBasicTypeId>(FBasicTypeId::INT16, "Int16", "Int16"),
    std::make_shared<FBasicTypeId>(FBasicTypeId::UINT16, "UInt16", "UInt16"),
    std::make_shared<FBasicTypeId>(FBasicTypeId::INT32, "Int32", "Int32"),
    std::make_shared<FBasicTypeId>(FBasicTypeId::UINT32, "UInt32", "UInt32"),
    std::make_shared<FBasicTypeId>(FBasicTypeId::INT64, "Int64", "Int64"),
    std::make_shared<FBasicTypeId>(FBasicTypeId::UINT64, "UInt64", "UInt64"),
    std::make_shared<FBasicTypeId>(FBasicTypeId::BOOLEAN, "Boolean", "Boolean"),
    std::make_shared<FBasicTypeId>(FBasicTypeId::STRING, "String", "String"),
    std::make_shared<FBasicTypeId>(FBasicTypeId::FLOAT, "Float", "Float"),
    std::make_shared<FBasicTypeId>(FBasicTypeId::DOUBLE, "Double", "Double"),
    std::make_shared<FBasicTypeId>(FBasicTypeId::BYTE_BUFFER, "ByteBuffer", "ByteBuffer"),
};

std::shared_ptr<FBasicTypeId> FBasicTypeId::get(const std::string &literal)
{
    for (int i = 0; i < BASIC_TYPEID_MAX; ++i)
    {
        if (!s_basicTypeTable[i]->getLiteral().compare(literal))
            return s_basicTypeTable[i];
    }

    return s_basicTypeTable[UNDEFINED];
}

std::shared_ptr<FBasicTypeId> FBasicTypeId::getByName(const std::string &name)
{
    for (int i = 0; i < BASIC_TYPEID_MAX; ++i)
    {
        if (!s_basicTypeTable[i]->getName().compare(name))
            return s_basicTypeTable[i];
    }

    return s_basicTypeTable[UNDEFINED];
}

std::shared_ptr<FBasicTypeId> FBasicTypeId::get(uint32_t value)
{
    if (value < BASIC_TYPEID_MAX)
        return s_basicTypeTable[value];
    else
        return s_basicTypeTable[UNDEFINED];
}

} // namespace BstIdl