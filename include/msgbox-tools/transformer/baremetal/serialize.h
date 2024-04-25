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
#ifndef TRANSFORMER_BAREMETAL_SERIALIZE_H
#define TRANSFORMER_BAREMETAL_SERIALIZE_H

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
static inline std::string serializeFBasicType(FBasicTypeId *type)
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
        ret = R"(ret = ipc_ser_put(ser, (uint8_t*)&$NAME, sizeof($TYPE));
if (ret < 0)
    return -1;
)";
        replace_all(ret, "$TYPE", getTypeName(type));
        break;
    }
    case FBasicTypeId::STRING: {
        ret = R"(ret = ipc_ser_put_string(ser, $NAME);
if (ret < 0)
    return -1;
)";
        break;
    }
    case FBasicTypeId::BYTE_BUFFER: {
        ret = R"(ret = ipc_ser_put(ser, (uint8_t*)&$NAME.size, sizeof(uint32_t));
if (ret < 0)
    return -1;
ret = ipc_ser_put(ser, $NAME.data, $NAME.size);
if (ret < 0)
    return -1;
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

static inline std::string serializeFIntegerInterval(FIntegerInterval *type)
{
    return R"(ret = ipc_ser_put(ser, (uint8_t*)&$NAME, sizeof(int64_t));
if (ret < 0)
    return -1;
)";
}

static inline std::string serializeFArrayType(FArrayType *type)
{
    if (!type)
        return "";

    std::string ret = R"(ret = ipc_ser_put(ser, (uint8_t*)&$NAME.size, sizeof(uint32_t));
if (ret < 0)
    return -1;
ret = ipc_ser_put(ser, (uint8_t*)$NAME.data, $NAME.size * sizeof($ELE_TYPE));
if (ret < 0)
    return -1;
)";
    replace_all(ret, "$ELE_TYPE", getTypeName(type->getElementType()));
    return ret;
}

static inline std::string serializeFEnumerationType(FEnumerationType *type)
{
    if (!type)
        return "";

    std::string ret = R"(int32_t $NAME_val = $NAME;
ret = ipc_ser_put(ser, (uint8_t*)&$NAME_val, sizeof(int32_t));
if (ret < 0)
    return -1;
)";
    return ret;
}

static inline std::string serialize(const std::shared_ptr<FObject> &obj);
static inline std::string serializeFStructType(FStructType *type)
{
    if (!type)
        return "";

    std::string ret;
    for (const auto &e : type->getElements())
    {
        auto ele = serialize(e->getType());
        replace_all(ele, "$NAME", std::string("$NAME.").append(e->getName()));
        ret.append(ele);
    }
    return ret;
}

static inline std::string serializeFUnionType(FUnionType *type)
{
    if (!type)
        return "";

    std::string ret = R"(ret = ipc_ser_put(ser, (uint8_t*)&$NAME, sizeof($TYPE));
if (ret < 0)
    return -1;
)";
    replace_all(ret, "$TYPE", getTypeName(type));
    return ret;
}

static inline std::string serializeFTypeRef(FTypeRef *type)
{
    if (!type)
        return "";

    if (auto ptr = type->getPredefined())
        return ptr->transform("baremetal_serialize");
    if (auto ptr = type->getDerived())
        return ptr->transform("baremetal_serialize");
    if (auto ptr = type->getInterval())
        return ptr->transform("baremetal_serialize");

    // empty typeref
    return "";
}

static inline std::string serializeFTypeDef(FTypeDef *type)
{
    if (!type || !type->getActualType())
        return "";

    return type->getActualType()->transform("baremetal_serialize");
}

static inline std::string serializeFArgument(FArgument *type)
{
    if (!type)
        return "";

    auto ret = serialize(type->getType());
    replace_all(ret, "$NAME", type->getName());

    return ret;
}

static inline bool registerSerialize()
{
    if (BstIdl::FObject::hasTransformer("baremetal_serialize"))
        return false;

    auto t = new BstIdl::FTransformer();
    t->forFBasicTypeId = serializeFBasicType;
    t->forFIntegerInterval = serializeFIntegerInterval;
    t->forFArrayType = serializeFArrayType;
    t->forFEnumerationType = serializeFEnumerationType;
    t->forFStructType = serializeFStructType;
    t->forFUnionType = serializeFUnionType;
    t->forFTypeRef = serializeFTypeRef;
    t->forFTypeDef = serializeFTypeDef;
    t->forFArgument = serializeFArgument;
    return BstIdl::FObject::addTransformer("baremetal_serialize", t);
}

static inline std::string serialize(const std::shared_ptr<FObject> &obj)
{
    if (!BstIdl::FObject::hasTransformer("baremetal_serialize"))
        (void)registerSerialize();
    return obj->transform("baremetal_serialize");
}

static inline std::string serialize(FObject *obj)
{
    if (!BstIdl::FObject::hasTransformer("baremetal_serialize"))
        (void)registerSerialize();
    return obj->transform("baremetal_serialize");
}

} // namespace BareMetal

} // namespace Transformer

} // namespace BstIdl

#endif