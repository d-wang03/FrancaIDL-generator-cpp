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
#ifndef TRANSFORMER_BAREMETAL_INITVAR_H
#define TRANSFORMER_BAREMETAL_INITVAR_H

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
static inline std::string initVarFBasicType(FBasicTypeId *type)
{
    std::string ret;
    switch (type->getValue())
    {
    case FBasicTypeId::INT8:
        return "int8_t $NAME = 0;\n";
    case FBasicTypeId::UINT8:
        return "uint8_t $NAME = 0;\n";
    case FBasicTypeId::INT16:
        return "int16_t $NAME = 0;\n";
    case FBasicTypeId::UINT16:
        return "uint16_t $NAME = 0;\n";
    case FBasicTypeId::INT32:
        return "int32_t $NAME = 0;\n";
    case FBasicTypeId::UINT32:
        return "uint32_t $NAME = 0;\n";
    case FBasicTypeId::INT64:
        return "int64_t $NAME = 0;\n";
    case FBasicTypeId::UINT64:
        return "uint64_t $NAME = 0;\n";
    case FBasicTypeId::BOOLEAN:
        return "bool $NAME = 0;\n";
    case FBasicTypeId::FLOAT:
        return "float $NAME = 0.0;\n";
    case FBasicTypeId::DOUBLE:
        return "double $NAME = 0.0;\n";
    case FBasicTypeId::STRING:
        return "char *$NAME = NULL;\n";
    case FBasicTypeId::BYTE_BUFFER:
        return "byte_buffer_t $NAME = { 0 };\n";
    case FBasicTypeId::UNDEFINED:
    default:
        std::cerr << "Undefined basic type: " << type->getValue() << std::endl;
        return "";
    }
}

static inline std::string initVarFIntegerInterval(FIntegerInterval *type)
{
    return "int64_t $NAME = 0;\n";

}

static inline std::string initVarFEnumerationType(FEnumerationType *type)
{
    if (!type)
        return "";
    return getTypeName(type) + " $NAME = 0;\n";
}

static inline std::string initVarFArrayType(FArrayType *type)
{
    if (!type)
        return "";

    if (type->isFixedSize())
        return getTypeName(type) + " *$NAME = NULL;\n";
    else
        return getTypeName(type) + " $NAME = { 0 };\n";
}

static inline std::string initVarFStructType(FStructType *type)
{
    if (!type)
        return "";

    if (isFixed(type) == "true")
        return getTypeName(type) + " *$NAME = NULL;\n";
    else
        return getTypeName(type) + " $NAME = { 0 };\n";
}

template<typename T>
std::string initVarFObject(T* type)
{
    if (!type)
        return "";
    return getTypeName(type) + " $NAME = { 0 };\n";
}

static inline std::string initVarFTypeRef(FTypeRef *type)
{
    if (!type)
        return "";

    if (auto ptr = type->getPredefined())
        return ptr->transform("baremetal_initVar");
    if (auto ptr = type->getDerived())
        return ptr->transform("baremetal_initVar");
    if (auto ptr = type->getInterval())
        return ptr->transform("baremetal_initVar");
    
    //empty typeref
    return "";
}

static inline std::string initVarFTypeDef(FTypeDef *type)
{
    if (!type)
        return "";

    if (auto ptr = type->getActualType())
        return ptr->transform("baremetal_initVar");
    
    return "";
}

static inline std::string initVar(const std::shared_ptr<FObject> &obj);
static inline std::string initVarFArgument(FArgument *type)
{
    if (!type)
        return "";
    
    auto ret = initVar(type->getType());
    replace_all(ret, "$NAME", type->getName());
    return ret;
}

static inline bool registerInitVar()
{
    if (BstIdl::FObject::hasTransformer("baremetal_initVar"))
        return false;
    
    auto t = new BstIdl::FTransformer();
    t->forFBasicTypeId = initVarFBasicType;
    t->forFIntegerInterval = initVarFIntegerInterval;
    t->forFArrayType = initVarFArrayType;
    t->forFEnumerationType = initVarFEnumerationType;
    t->forFStructType = initVarFStructType;
    t->forFUnionType = initVarFObject<FUnionType>;
    t->forFTypeRef = initVarFTypeRef;
    t->forFTypeDef = initVarFTypeDef;
    t->forFArgument = initVarFArgument;
    return BstIdl::FObject::addTransformer("baremetal_initVar", t);
}

static inline std::string initVar(const std::shared_ptr<FObject> &obj)
{
    if (!BstIdl::FObject::hasTransformer("baremetal_initVar"))
        (void)registerInitVar();
    return obj->transform("baremetal_initVar");
}

static inline std::string initVar(FObject *obj)
{
    if (!BstIdl::FObject::hasTransformer("baremetal_initVar"))
        (void)registerInitVar();
    return obj->transform("baremetal_initVar");
}

} // namespace MsgBoxGen

} // namespace Transformer

} // namespace BstIdl

#endif