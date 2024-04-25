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
#ifndef TRANSFORMER_BAREMETAL_DESERIALIZE_H
#define TRANSFORMER_BAREMETAL_DESERIALIZE_H

#include "model/FObject.h"
#include "model/FTransformer.h"
#include "msgbox-tools/transformer/baremetal/initvar.h"
#include "msgbox-tools/transformer/baremetal/typename.h"
#include "utilities/string_utility.h"
#include <string>

namespace BstIdl
{
namespace Transformer
{
namespace BareMetal
{
static inline std::string deserializeFBasicType(FBasicTypeId *type)
{
    std::string ret;
    switch (type->getValue())
    {
    case FBasicTypeId::INT8:
    case FBasicTypeId::UINT8:
    case FBasicTypeId::INT16:
    case FBasicTypeId::UINT16:
    case FBasicTypeId::INT32:
    case FBasicTypeId::UINT32:
    case FBasicTypeId::INT64:
    case FBasicTypeId::UINT64:
    case FBasicTypeId::BOOLEAN:
    case FBasicTypeId::FLOAT:
    case FBasicTypeId::DOUBLE: {
        ret = R"(ret = ipc_des_get(des, (uint8_t *)&$NAME, sizeof($TYPE));
if (ret < 0)
{
    return -1;
}
else
{
    length += sizeof($TYPE);
    if (length >= IPC_MAX_DATA_SIZE)
        return -1;
}
)";
        replace_all(ret, "$TYPE", getTypeName(type));
        break;
    }
    case FBasicTypeId::STRING: {
        ret = R"($NAME = &s_recv_buffer[length];
ret = ipc_des_get_string(des, $NAME, IPC_MAX_DATA_SIZE - length);
if (ret < 0)
{
    return -1;
}
else
{
    length += ret;
    if (length >= IPC_MAX_DATA_SIZE)
        return -1;
}
)";
        break;
    }
    case FBasicTypeId::BYTE_BUFFER: {
        ret = R"(ret = ipc_des_get(des, (uint8_t *)&$NAME.size, sizeof(uint32_t));
if (ret < 0)
{
    return -1;
}
else
{
    length += sizeof(uint32_t);
    if (length >= IPC_MAX_DATA_SIZE)
        return -1;
}
$NAME.data = (uint8_t *)&s_recv_buffer[length];
ret = ipc_des_get(des, $NAME.data, $NAME.size);
if (ret < 0)
{
    return -1;
}
else
{
    length += $NAME.size;
    if (length >= IPC_MAX_DATA_SIZE)
        return -1;
}
)";
        break;
    }
    case FBasicTypeId::UNDEFINED:
    default:
        std::cerr << "Undefined basic type: " << type->getValue() << std::endl;
        return "";
    }

    return ret;
}

static inline std::string deserializeFIntegerInterval(FIntegerInterval *type)
{
    return R"(ret = ipc_des_get(des, (uint8_t *)&$NAME, sizeof(int64_t));
if (ret < 0)
{
    return -1;
}
else
{
    length += sizeof(int64_t);
    if (length >= IPC_MAX_DATA_SIZE)
        return -1;
}
)";
}

static inline std::string deserializeFArrayType(FArrayType *type)
{
    if (!type)
        return "";

    std::string ret = R"(ret = ipc_des_get(des, (uint8_t *)&$NAME.size, sizeof(uint32_t));
if (ret < 0)
{
    return -1;
}
else
{
    length += sizeof(uint32_t);
    if (length >= IPC_MAX_DATA_SIZE)
        return -1;
}
$NAME.data = ($ELE_TYPE *)&s_recv_buffer[length];
ret = ipc_des_get(des, (uint8_t *)$NAME.data, $NAME.size * sizeof($ELE_TYPE));
if (ret < 0)
{
    return -1;
}
else
{
    length += $NAME.size * sizeof($ELE_TYPE);
    if (length >= IPC_MAX_DATA_SIZE)
        return -1;
}
)";
    replace_all(ret, "$ELE_TYPE", getTypeName(type->getElementType()));
    return ret;
}

static inline std::string deserializeFEnumerationType(FEnumerationType *type)
{
    if (!type)
        return "";

    std::string ret = R"(int32_t $NAME_val = 0;
ret = ipc_des_get(des, (uint8_t *)&$NAME_val, sizeof(int32_t));
if (ret < 0)
{
    return -1;
}
else
{
    $NAME = $NAME_val;
    length += sizeof(int32_t);
    if (length >= IPC_MAX_DATA_SIZE)
        return -1;
}
)";

    return ret;
}

static inline std::string deserialize(const std::shared_ptr<FObject> &obj);
static inline std::string deserializeFStructType(FStructType *type)
{
    if (!type)
        return "";

    std::string ret;
    for (const auto &e : type->getElements())
    {
        auto ele = deserialize(e->getType());
        replace_all(ele, "$NAME", std::string("$NAME.").append(e->getName()));
        ret.append(ele);
    }
    return ret;
}

static inline std::string deserializeFUnionType(FUnionType *type)
{
    if (!type)
        return "";

    std::string ret = R"(ret = ipc_des_get(des, (uint8_t *)&$NAME, sizeof($TYPE));
if (ret < 0)
{
    return -1;
}
else
{
    length += sizeof($TYPE);
    if (length >= IPC_MAX_DATA_SIZE)
        return -1;
}
)";
    replace_all(ret, "$TYPE", getTypeName(type));
    return ret;
}

static inline std::string deserializeFTypeRef(FTypeRef *type)
{
    if (!type)
        return "";

    if (auto ptr = type->getPredefined())
        return ptr->transform("baremetal_deserialize");
    if (auto ptr = type->getDerived())
        return ptr->transform("baremetal_deserialize");
    if (auto ptr = type->getInterval())
        return ptr->transform("baremetal_deserialize");

    // empty typeref
    return "";
}

static inline std::string deserializeFTypeDef(FTypeDef *type)
{
    if (!type || !type->getActualType())
        return "";

    return type->getActualType()->transform("baremetal_deserialize");
}

static inline std::string deserializeFArgument(FArgument *type)
{
    if (!type)
        return "";

    auto t = type->getType();
    std::string ret = initVar(t);
    ret.append(deserialize(t));
    replace_all(ret, "$NAME", type->getName());

    return ret;
}

static inline bool registerDeserialize()
{
    if (BstIdl::FObject::hasTransformer("baremetal_deserialize"))
        return false;

    auto t = new BstIdl::FTransformer();
    t->forFBasicTypeId = deserializeFBasicType;
    t->forFIntegerInterval = deserializeFIntegerInterval;
    t->forFArrayType = deserializeFArrayType;
    t->forFEnumerationType = deserializeFEnumerationType;
    t->forFStructType = deserializeFStructType;
    t->forFUnionType = deserializeFUnionType;
    t->forFTypeRef = deserializeFTypeRef;
    t->forFTypeDef = deserializeFTypeDef;
    t->forFArgument = deserializeFArgument;
    return BstIdl::FObject::addTransformer("baremetal_deserialize", t);
}

static inline std::string deserialize(const std::shared_ptr<FObject> &obj)
{
    if (!BstIdl::FObject::hasTransformer("baremetal_deserialize"))
        (void)registerDeserialize();
    return obj->transform("baremetal_deserialize");
}

static inline std::string deserialize(FObject *obj)
{
    if (!BstIdl::FObject::hasTransformer("baremetal_deserialize"))
        (void)registerDeserialize();
    return obj->transform("baremetal_deserialize");
}

} // namespace BareMetal

} // namespace Transformer

} // namespace BstIdl

#endif