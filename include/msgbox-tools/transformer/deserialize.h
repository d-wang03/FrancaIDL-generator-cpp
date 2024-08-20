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
static inline std::string deserializeFBasicType(FBasicTypeId *type)
{
    std::string ret;
    switch (type->getValue())
    {
    case FBasicTypeId::BOOLEAN:
    case FBasicTypeId::INT8:
    case FBasicTypeId::UINT8:
    {
        ret = R"(if (ret >= 0)
    ret = deserialize_8(buf, (uint8_t *)&$NAME);
)";
        replace_all(ret, "$TYPE", getTypeName(type));
        break;
    }
    case FBasicTypeId::INT16:
    case FBasicTypeId::UINT16:
    {
        ret = R"(if (ret >= 0)
    ret = deserialize_16(buf, (uint16_t *)&$NAME);
)";
        replace_all(ret, "$TYPE", getTypeName(type));
        break;
    }
    case FBasicTypeId::INT32:
    case FBasicTypeId::UINT32:
    case FBasicTypeId::FLOAT:
    {
        ret = R"(if (ret >= 0)
    ret = deserialize_32(buf, (uint32_t *)&$NAME);
)";
        replace_all(ret, "$TYPE", getTypeName(type));
        break;
    }
    case FBasicTypeId::INT64:
    case FBasicTypeId::UINT64:
    case FBasicTypeId::DOUBLE:
    {
        ret = R"(if (ret >= 0)
    ret = deserialize_64(buf, (uint64_t *)&$NAME);
)";
        replace_all(ret, "$TYPE", getTypeName(type));
        break;
    }
    case FBasicTypeId::STRING:
    {
        ret = R"(if (ret >= 0)
    ret = deserialize_string(buf, &$NAME);
)";
        break;
    }
    case FBasicTypeId::BYTE_BUFFER:
    {
        ret = R"(if (ret >= 0)
    ret = deserialize_byte_buffer(buf, &$NAME);
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
    return R"(if (ret >= 0)
    ret = deserialize_64(buf, (uint64_t *)&$NAME);
)";
}

static inline std::string deserializeFArrayType(FArrayType *type)
{
    if (!type)
        return "";

    std::string ret = R"(if (ret >= 0)
    ret = deserialize_$TYPENAME(buf, &$NAME);
)";
    replace_all(ret, "$TYPENAME", remove_last(getTypeName(type), 2));
    return ret;
}

static inline std::string deserializeFEnumerationType(FEnumerationType *type)
{
    if (!type)
        return "";

    std::string ret = R"(if (ret >= 0)
    ret = deserialize_$TYPENAME(buf, &$NAME);
)";
    replace_all(ret, "$TYPENAME", remove_last(getTypeName(type), 2));

    return ret;
}

static inline std::string deserialize(const std::shared_ptr<FObject> &obj);
static inline std::string deserializeFStructType(FStructType *type)
{
    if (!type)
        return "";

    std::string ret = R"(if (ret >= 0)
    ret = deserialize_$TYPENAME(buf, &$NAME);
)";
    replace_all(ret, "$TYPENAME", remove_last(getTypeName(type), 2));
    return ret;
}

static inline std::string deserializeFUnionType(FUnionType *type)
{
    if (!type)
        return "";

    std::string ret = R"(if (ret >= 0)
    ret = deserialize_$TYPENAME(buf, &$NAME);
)";
    replace_all(ret, "$TYPENAME", remove_last(getTypeName(type), 2));
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
    if (auto pred = type->getActualType()->getPredefined())
        return pred->transform("baremetal_deserialize");

    std::string ret = R"(if (ret >= 0)
    ret = deserialize_$TYPENAME(buf, &$NAME);
)";
    replace_all(ret, "$TYPENAME", remove_last(getTypeName(type), 2));
    return ret;
}

static inline std::string deserializeFArgument(FArgument *type)
{
    if (!type)
        return "";

    auto t = type->getType();
    std::string ret = deserialize(t);
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

} // namespace MsgBoxGen

} // namespace Transformer

} // namespace BstIdl

#endif