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
#ifndef TRANSFORMER_POSIX_OUT_ASGN_H
#define TRANSFORMER_POSIX_OUT_ASGN_H

#include "model/FObject.h"
#include "model/FTransformer.h"
#include "msgbox-tools/transformer/typename.h"
#include "msgbox-tools/transformer/isfixed.h"
#include "utilities/string_utility.h"
#include <string>

namespace BstIdl
{
namespace Transformer
{
namespace MsgBoxGen
{
static inline std::string outAsgnFBasicType(FBasicTypeId *type)
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
    case FBasicTypeId::DOUBLE:
    {
        ret = "*out->$NAME = $NAME;\n";
        break;
    }
    case FBasicTypeId::STRING:
    {
        ret = "*out->$NAME = (char *)$NAME;\n";
        break;
    }
    case FBasicTypeId::BYTE_BUFFER:
    {
        ret = R"(out->$NAME->size = $NAME.size;
out->$NAME->data = $NAME.data;
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

static inline std::string outAsgnFIntegerInterval(FIntegerInterval *type)
{
    return "*out->$NAME = $NAME;\n";
}

static inline std::string outAsgnFArrayType(FArrayType *type)
{
    if (!type)
        return "";

    std::string ret;
    if (type->isFixedSize())
        ret = "*out->$NAME = ($TYPENAME *)$NAME;\n";
    else
        ret = R"(out->$NAME->size = $NAME.size;
out->$NAME->data = $NAME.data;
)";
    replace_all(ret, "$TYPENAME", getTypeName(type));
    replace_all(ret, "$ELE_TYPE", getTypeName(type->getElementType()));
    return ret;
}

static inline std::string outAsgnFEnumerationType(FEnumerationType *type)
{
    if (!type)
        return "";

    return "*out->$NAME = $NAME;\n";
}

static inline std::string outAsgn(const std::shared_ptr<FObject> &obj);
static inline std::string outAsgnFStructType(FStructType *type)
{
    if (!type)
        return "";

    std::string ret;
    if (isFixed(type) == "true")
    {
        ret = "*out->$NAME = ($TYPENAME *)$NAME;\n";
        replace_all(ret, "$TYPENAME", getTypeName(type));
    }
    else
    {
        for (const auto &e : type->getElements())
        {
            auto ele = outAsgn(e->getType());
            replace_all(ele, "$NAME", std::string("$NAME.").append(e->getName()));
            replace_all(ele, e->getName().append("->"), e->getName().append("."));
            replace_all(ele, "*out->", "out->");
            ret.append(ele);
        }
    }
    return ret;
}

static inline std::string outAsgnFUnionType(FUnionType *type)
{
    if (!type)
        return "";

    return "*out->$NAME = $NAME;\n";
}

static inline std::string outAsgnFTypeRef(FTypeRef *type)
{
    if (!type)
        return "";

    if (auto ptr = type->getPredefined())
        return ptr->transform("posix_out_asgn");
    if (auto ptr = type->getDerived())
        return ptr->transform("posix_out_asgn");
    if (auto ptr = type->getInterval())
        return ptr->transform("posix_out_asgn");
    
    //empty typeref
    return "";
}

static inline std::string outAsgnFTypeDef(FTypeDef *type)
{
    if (!type || !type->getActualType())
        return "";

    return type->getActualType()->transform("posix_out_asgn");
}

static inline std::string outAsgnFArgument(FArgument *type)
{
    if (!type)
        return "";
    
    auto t = type->getType();
    std::string ret = outAsgn(t);
    replace_all(ret, "out->$NAME.", "out->$NAME->");
    replace_all(ret, "$NAME", type->getName());

    return ret;
}

static inline bool registerOutAsgn()
{
    if (BstIdl::FObject::hasTransformer("posix_out_asgn"))
        return false;
    
    auto t = new BstIdl::FTransformer();
    t->forFBasicTypeId = outAsgnFBasicType;
    t->forFIntegerInterval = outAsgnFIntegerInterval;
    t->forFArrayType = outAsgnFArrayType;
    t->forFEnumerationType = outAsgnFEnumerationType;
    t->forFStructType = outAsgnFStructType;
    t->forFUnionType = outAsgnFUnionType;
    t->forFTypeRef = outAsgnFTypeRef;
    t->forFTypeDef = outAsgnFTypeDef;
    t->forFArgument = outAsgnFArgument;
    return BstIdl::FObject::addTransformer("posix_out_asgn", t);
}

static inline std::string outAsgn(const std::shared_ptr<FObject> &obj)
{
    if (!BstIdl::FObject::hasTransformer("posix_out_asgn"))
        (void)registerOutAsgn();
    return obj->transform("posix_out_asgn");
}

static inline std::string outAsgn(FObject *obj)
{
    if (!BstIdl::FObject::hasTransformer("posix_out_asgn"))
        (void)registerOutAsgn();
    return obj->transform("posix_out_asgn");
}

} // namespace MsgBoxGen

} // namespace Transformer

} // namespace BstIdl

#endif