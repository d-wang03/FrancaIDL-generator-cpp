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
#ifndef TRANSFORMER_GET_ALIGN_H
#define TRANSFORMER_GET_ALIGN_H

#include "model/FObject.h"
#include "model/FTransformer.h"
#include "msgbox-tools/transformer/typename.h"
#include "utilities/string_utility.h"
#include <string>

namespace BstIdl
{
namespace Transformer
{
namespace MsgBoxGen
{
static inline std::string getAlignFBasicType(FBasicTypeId *type)
{
    std::string ret = "1";
    switch (type->getValue())
    {
    case FBasicTypeId::BOOLEAN:
    case FBasicTypeId::INT8:
    case FBasicTypeId::UINT8:
        return "1";
    case FBasicTypeId::INT16:
    case FBasicTypeId::UINT16:
        return "2";
    case FBasicTypeId::INT32:
    case FBasicTypeId::UINT32:
    case FBasicTypeId::FLOAT:
        return "4";
    case FBasicTypeId::INT64:
    case FBasicTypeId::UINT64:
    case FBasicTypeId::DOUBLE:
        return "8";
    case FBasicTypeId::STRING:
        return "1";
    case FBasicTypeId::BYTE_BUFFER:
        return "4";
    case FBasicTypeId::UNDEFINED:
    default:
        std::cerr << "Undefined basic type: " << type->getValue() << std::endl;
        return "1";
    }

    return ret;
}

static inline std::string getAlignFIntegerInterval(FIntegerInterval *type)
{
    return "8";
}

static inline std::string getAlign(const std::shared_ptr<FObject> &obj);
static inline std::string getAlignFArrayType(FArrayType *type)
{
    if (!type || !type->getElementType())
        return "1";

    auto ret = getAlign(type->getElementType());
    if (type->isFixedSize())
        return ret;
    
    return ret > "4" ? ret : "4";
}

static inline std::string getAlignFEnumerationType(FEnumerationType *type)
{
    if (!type)
        return "1";

    return "4";
}

static inline std::string getAlignFStructType(FStructType *type)
{
    if (!type)
        return "1";

    std::string ret = "1";
    for (const auto &e : type->getElements())
    {
        std::string ele = getAlign(e->getType());
        if (ele > ret)
            ret = ele;
    }
    return ret;
}

static inline std::string getAlignFUnionType(FUnionType *type)
{
    if (!type)
        return "1";

    std::string ret = "1";
    for (const auto &e : type->getElements())
    {
        std::string ele = getAlign(e->getType());
        if (ele > ret)
            ret = ele;
    }
    return ret;
}

static inline std::string getAlignFTypeRef(FTypeRef *type)
{
    if (!type)
        return "1";

    if (auto ptr = type->getPredefined())
        return ptr->transform("get_align");
    if (auto ptr = type->getDerived())
        return ptr->transform("get_align");
    if (auto ptr = type->getInterval())
        return ptr->transform("get_align");
    
    //empty typeref
    return "1";
}

static inline std::string getAlignFTypeDef(FTypeDef *type)
{
    if (!type || !type->getActualType())
        return "1";

    return type->getActualType()->transform("get_align");
}

static inline std::string getAlignFArgument(FArgument *type)
{
    if (!type)
        return "1";
    
    auto t = type->getType();
    return getAlign(t);
}

static inline bool registerGetAlign()
{
    if (BstIdl::FObject::hasTransformer("get_align"))
        return false;
    
    auto t = new BstIdl::FTransformer();
    t->forFBasicTypeId = getAlignFBasicType;
    t->forFIntegerInterval = getAlignFIntegerInterval;
    t->forFArrayType = getAlignFArrayType;
    t->forFEnumerationType = getAlignFEnumerationType;
    t->forFStructType = getAlignFStructType;
    t->forFUnionType = getAlignFUnionType;
    t->forFTypeRef = getAlignFTypeRef;
    t->forFTypeDef = getAlignFTypeDef;
    t->forFArgument = getAlignFArgument;
    return BstIdl::FObject::addTransformer("get_align", t);
}

static inline std::string getAlign(const std::shared_ptr<FObject> &obj)
{
    if (!BstIdl::FObject::hasTransformer("get_align"))
        (void)registerGetAlign();
    return obj->transform("get_align");
}

static inline std::string getAlign(FObject *obj)
{
    if (!BstIdl::FObject::hasTransformer("get_align"))
        (void)registerGetAlign();
    return obj->transform("get_align");
}

} // namespace MsgBoxGen

} // namespace Transformer

} // namespace BstIdl

#endif