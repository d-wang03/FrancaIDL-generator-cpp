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
#include "asf-tools/MsgType.h"
#include "utilities/string_utility.h"
#include <memory>

namespace BstASF
{

static const std::shared_ptr<MsgType> s_msgTypeTable[] = {
    std::make_shared<MsgType>(MsgType::Method, "Method", "on_TCAPIMsg_Method"),
    std::make_shared<MsgType>(MsgType::Broadcast, "Broadcast", "on_TCAPIMsg_Broadcast"),
    std::make_shared<MsgType>(MsgType::Attribute, "Attribute", "on_TCAPIMsg_Attribute"),
    std::make_shared<MsgType>(MsgType::Reply, "Reply", "on_TCAPIMsg_Reply"),
    std::make_shared<MsgType>(MsgType::AvailStatus, "AvailStatus", "on_TCAPIMsg_AvailStatus"),
};

std::shared_ptr<MsgType> MsgType::get(const std::string &literal)
{
    for (int i = 0; i < MsgType::MAX_VALUE; ++i)
    {
        if (!s_msgTypeTable[i]->getLiteral().compare(literal))
            return s_msgTypeTable[i];
    }
    return nullptr;
}

std::shared_ptr<MsgType> MsgType::getByName(const std::string &name)
{
    for (int i = 0; i < MsgType::MAX_VALUE; ++i)
    {
        if (!s_msgTypeTable[i]->getName().compare(name))
            return s_msgTypeTable[i];
    }
    return nullptr;
}

std::shared_ptr<MsgType> get(uint32_t value)
{
    if (value < MsgType::MAX_VALUE)
    {
        return s_msgTypeTable[value];
    }
    return nullptr;
}

} // namespace BstASF