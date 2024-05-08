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
#ifndef TRANSFORMER_REPLACE_ENUM_H
#define TRANSFORMER_REPLACE_ENUM_H

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

static inline std::shared_ptr<FTypeRef> createInt32Type()
{
    auto ret = std::make_shared<FTypeRef>();
    ret->setPredefined(FBasicTypeId::get(FBasicTypeId::INT32));
    return ret;
}
static inline std::string replaceEnumFBasicType(FBasicTypeId *type)
{
    return "";
}

static inline std::string replaceEnumFIntegerInterval(FIntegerInterval *type)
{
    return "";
}

static inline std::string replaceEnum(const std::shared_ptr<FObject> &obj);
static inline std::string replaceEnumFArrayType(FArrayType *type)
{
    if (!type || !type->getElementType())
        return "";

    auto ele_type = type->getElementType();
    if (std::dynamic_pointer_cast<FEnumerationType>(ele_type->getDerived()))
    {
        CWRN << "Use enumeration type " << ele_type->getName() << " in array " 
             << type->getName() << ", replace with int32_t" << ENDL;
        type->setElementType(createInt32Type());
    }
    else
        (void)replaceEnum(type->getElementType());
    return "";
}

static inline std::string replaceEnumFEnumerationType(FEnumerationType *type)
{
    return "";
}

static inline std::string replaceEnumFStructType(FStructType *type)
{
    if (!type)
        return "";

    for (const auto &e : type->getElements())
    {
        if (std::dynamic_pointer_cast<FEnumerationType>(e->getType()->getDerived()))
        {
            CWRN << "Use enumeration type " << e->getType()->getName() << " in struct "
                 << type->getName() << " as " << e->getName() << ", replace with int32_t" << ENDL;
            e->setType(createInt32Type());
        }
        else
            (void)replaceEnum(e->getType());
    }
    return "";
}

static inline std::string replaceEnumFUnionType(FUnionType *type)
{
    if (!type)
        return "";

    for (const auto &e : type->getElements())
    {
        if (std::dynamic_pointer_cast<FEnumerationType>(e->getType()->getDerived()))
        {
            CWRN << "Use enumeration type " << e->getType()->getName() << " in union " 
                 << type->getName() << " as " << e->getName() << ", replace with int32_t" << ENDL;
            e->setType(createInt32Type());
        }
        else
            (void)replaceEnum(e->getType());
    }
    return "";
}

static inline std::string replaceEnumFTypeRef(FTypeRef *type)
{
    if (!type)
        return "1";

    if (auto ptr = type->getPredefined())
        return ptr->transform("replace_enum");
    if (auto ptr = type->getDerived())
        return ptr->transform("replace_enum");
    if (auto ptr = type->getInterval())
        return ptr->transform("replace_enum");
    
    //empty typeref
    return "1";
}

static inline std::string replaceEnumFTypeDef(FTypeDef *type)
{
    if (!type || !type->getActualType())
        return "1";

    return type->getActualType()->transform("replace_enum");
}

static inline std::string replaceEnumFArgument(FArgument *type)
{
    if (!type)
        return "";
    
    auto t = type->getType();
    return replaceEnum(t);
}

static inline bool registerReplaceEnum()
{
    if (BstIdl::FObject::hasTransformer("replace_enum"))
        return false;
    
    auto t = new BstIdl::FTransformer();
    t->forFBasicTypeId = replaceEnumFBasicType;
    t->forFIntegerInterval = replaceEnumFIntegerInterval;
    t->forFArrayType = replaceEnumFArrayType;
    t->forFEnumerationType = replaceEnumFEnumerationType;
    t->forFStructType = replaceEnumFStructType;
    t->forFUnionType = replaceEnumFUnionType;
    t->forFTypeRef = replaceEnumFTypeRef;
    t->forFTypeDef = replaceEnumFTypeDef;
    t->forFArgument = replaceEnumFArgument;
    return BstIdl::FObject::addTransformer("replace_enum", t);
}

static inline std::string replaceEnum(const std::shared_ptr<FObject> &obj)
{
    if (!BstIdl::FObject::hasTransformer("replace_enum"))
        (void)registerReplaceEnum();
    return obj->transform("replace_enum");
}

static inline std::string replaceEnum(FObject *obj)
{
    if (!BstIdl::FObject::hasTransformer("replace_enum"))
        (void)registerReplaceEnum();
    return obj->transform("replace_enum");
}

} // namespace MsgBoxGen

} // namespace Transformer

} // namespace BstIdl

#endif