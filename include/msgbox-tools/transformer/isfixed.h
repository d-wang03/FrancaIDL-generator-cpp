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
#ifndef TRANSFORMER_IS_FIX_SIZE_H
#define TRANSFORMER_IS_FIX_SIZE_H

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
static inline std::string isFixedFBasicType(FBasicTypeId *type)
{
    std::string ret = "false";
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
        ret = "true";
        break;
    case FBasicTypeId::STRING:
    case FBasicTypeId::BYTE_BUFFER:
        ret = "false";
        break;
    case FBasicTypeId::UNDEFINED:
    default:
        std::cerr << "Undefined basic type: " << type->getValue() << std::endl;
        return "false";
    }

    return ret;
}

static inline std::string isFixedFIntegerInterval(FIntegerInterval *type)
{
    return "true";
}

static inline std::string isFixed(const std::shared_ptr<FObject> &obj);
static inline std::string isFixedFArrayType(FArrayType *type)
{
    if (!type || !type->getElementType())
        return "false";

    if (!type->isFixedSize())
        return "false";
    
    return isFixed(type->getElementType());
}

static inline std::string isFixedFEnumerationType(FEnumerationType *type)
{
    if (!type)
        return "false";

    return "true";
}

static inline std::string isFixedFStructType(FStructType *type)
{
    if (!type)
        return "false";

    std::string ret;
    for (const auto &e : type->getElements())
    {
        if (e->isArray() && !e->isFixedSize())
            return "false";

        std::string ele = isFixed(e->getType());
        if (ele == "false")
            return "false";
    }
    return "true";
}

static inline std::string isFixedFUnionType(FUnionType *type)
{
    if (!type)
        return "false";

    return "true";
}

static inline std::string isFixedFTypeRef(FTypeRef *type)
{
    if (!type)
        return "false";

    if (auto ptr = type->getPredefined())
        return ptr->transform("is_fixed_size");
    if (auto ptr = type->getDerived())
        return ptr->transform("is_fixed_size");
    if (auto ptr = type->getInterval())
        return ptr->transform("is_fixed_size");
    
    //empty typeref
    return "false";
}

static inline std::string isFixedFTypeDef(FTypeDef *type)
{
    if (!type || !type->getActualType())
        return "false";

    return type->getActualType()->transform("is_fixed_size");
}

static inline std::string isFixedFArgument(FArgument *type)
{
    if (!type)
        return "false";
    
    if (type->isArray() && !type->isFixedSize())
        return "false";

    auto t = type->getType();
    return isFixed(t);
}

static inline bool registerIsFixed()
{
    if (BstIdl::FObject::hasTransformer("is_fixed_size"))
        return false;
    
    auto t = new BstIdl::FTransformer();
    t->forFBasicTypeId = isFixedFBasicType;
    t->forFIntegerInterval = isFixedFIntegerInterval;
    t->forFArrayType = isFixedFArrayType;
    t->forFEnumerationType = isFixedFEnumerationType;
    t->forFStructType = isFixedFStructType;
    t->forFUnionType = isFixedFUnionType;
    t->forFTypeRef = isFixedFTypeRef;
    t->forFTypeDef = isFixedFTypeDef;
    t->forFArgument = isFixedFArgument;
    return BstIdl::FObject::addTransformer("is_fixed_size", t);
}

static inline std::string isFixed(const std::shared_ptr<FObject> &obj)
{
    if (!BstIdl::FObject::hasTransformer("is_fixed_size"))
        (void)registerIsFixed();
    return obj->transform("is_fixed_size");
}

static inline std::string isFixed(FObject *obj)
{
    if (!BstIdl::FObject::hasTransformer("is_fixed_size"))
        (void)registerIsFixed();
    return obj->transform("is_fixed_size");
}

} // namespace MsgBoxGen

} // namespace Transformer

} // namespace BstIdl

#endif