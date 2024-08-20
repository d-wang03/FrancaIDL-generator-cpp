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
#include "msgbox-tools/transformer/typename.h"
#include "msgbox-tools/transformer/initvar.h"
#include "utilities/string_utility.h"
#include <string>

namespace BstIdl
{
namespace Transformer
{
namespace MsgBoxGen
{
static inline std::string serializeFBasicType(FBasicTypeId *type)
{
    std::string ret;
    switch (type->getValue())
    {
    case FBasicTypeId::BOOLEAN:
    case FBasicTypeId::INT8:
    case FBasicTypeId::UINT8:
    {
        ret = R"(if (ret >= 0)
    ret = ipc_ser_put_8(ser, (uint8_t *)&$NAME);
)";
        replace_all(ret, "$TYPE", getTypeName(type));
        break;
    }
    case FBasicTypeId::INT16:
    case FBasicTypeId::UINT16:
    {
        ret = R"(if (ret >= 0)
    ret = ipc_ser_put_16(ser, (uint16_t *)&$NAME);
)";
        replace_all(ret, "$TYPE", getTypeName(type));
        break;
    }
    case FBasicTypeId::INT32:
    case FBasicTypeId::UINT32:
    case FBasicTypeId::FLOAT:
    {
        ret = R"(if (ret >= 0)
    ret = ipc_ser_put_32(ser, (uint32_t *)&$NAME);
)";
        replace_all(ret, "$TYPE", getTypeName(type));
        break;
    }
    case FBasicTypeId::INT64:
    case FBasicTypeId::UINT64:
    case FBasicTypeId::DOUBLE:
    {
        ret = R"(if (ret >= 0)
    ret = ipc_ser_put_64(ser, (uint64_t *)&$NAME);
)";
        replace_all(ret, "$TYPE", getTypeName(type));
        break;
    }
    case FBasicTypeId::STRING:
    {
        ret = R"(if (ret >= 0)
    ret = serialize_string(ser, $NAME);
)";
        break;
    }
    case FBasicTypeId::BYTE_BUFFER:
    {
        ret = R"(if (ret >= 0)
    ret = serialize_byte_buffer(ser, &$NAME);
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
    return R"(if (ret >= 0)
    ret = ipc_ser_put_64(ser, (uint64_t*)&$NAME);
)";
}

static inline std::string serializeFArrayType(FArrayType *type)
{
    if (!type)
        return "";

    std::string ret = R"(if (ret >= 0)
    ret = serialize_$TYPE(ser, &$NAME);
)";
    if (type->isFixedSize())
        replace_one(ret, "&$NAME", "$NAME");
    replace_all(ret, "$TYPE", remove_last(getTypeName(type), 2));
    return ret;
}

static inline std::string serializeFEnumerationType(FEnumerationType *type)
{
    if (!type)
        return "";

    std::string ret = R"(if (ret >= 0)
    ret = serialize_$TYPE(ser, &$NAME);
)";
    replace_all(ret, "$TYPE", remove_last(getTypeName(type), 2));
    return ret;
}

static inline std::string serialize(const std::shared_ptr<FObject> &obj);
static inline std::string serializeFStructType(FStructType *type)
{
    if (!type)
        return "";

    std::string ret = R"(if (ret >= 0)
    ret = serialize_$TYPE(ser, &$NAME);
)";
    if (isFixed(type) == "true")
        replace_all(ret, "&$NAME", "$NAME");
    replace_all(ret, "$TYPE", remove_last(getTypeName(type), 2));
    return ret;
}

static inline std::string serializeFUnionType(FUnionType *type)
{
    if (!type)
        return "";

    std::string ret = R"(if (ret >= 0)
    ret = serialize_$TYPE(ser, &$NAME);
)";
    replace_all(ret, "$TYPE", remove_last(getTypeName(type), 2));
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
    
    //empty typeref
    return "";
}

static inline std::string serializeFTypeDef(FTypeDef *type)
{
    if (!type || !type->getActualType())
        return "";
    if (auto pred = type->getActualType()->getPredefined())
        return pred->transform("baremetal_serialize");

    std::string ret = R"(if (ret >= 0)
    ret = serialize_$TYPE(ser, &$NAME);
)";
    if (isFixed(type) == "true")
        replace_all(ret, "&$NAME", "$NAME");
    replace_all(ret, "$TYPE", remove_last(getTypeName(type), 2));
    return ret;
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

} // namespace MsgBoxGen

} // namespace Transformer

} // namespace BstIdl

#endif