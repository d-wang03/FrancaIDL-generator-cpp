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
#ifndef TRANSFORMER_BAREMETAL_TYPENAME_H
#define TRANSFORMER_BAREMETAL_TYPENAME_H

#include "model/FObject.h"
#include "model/FTransformer.h"
#include <string>

namespace BstIdl
{
namespace Transformer
{
namespace MsgBoxGen
{
static inline std::string getFBasicTypeName(FBasicTypeId *type)
{
    auto value = type->getValue();
    switch (value)
    {
    case FBasicTypeId::INT8:
        return "int8_t";
    case FBasicTypeId::UINT8:
        return "uint8_t";
    case FBasicTypeId::INT16:
        return "int16_t";
    case FBasicTypeId::UINT16:
        return "uint16_t";
    case FBasicTypeId::INT32:
        return "int32_t";
    case FBasicTypeId::UINT32:
        return "uint32_t";
    case FBasicTypeId::INT64:
        return "int64_t";
    case FBasicTypeId::UINT64:
        return "uint64_t";
    case FBasicTypeId::BOOLEAN:
        return "bool";
    case FBasicTypeId::STRING:
        return "char *";
    case FBasicTypeId::FLOAT:
        return "float";
    case FBasicTypeId::DOUBLE:
        return "double";
    case FBasicTypeId::BYTE_BUFFER:
        return "byte_buffer_t";
    case FBasicTypeId::UNDEFINED:
    default:
        std::cerr << "Undefined basic type: " << value << std::endl;
        return "";
    }
}

static inline std::string getFIntegerIntervalName(FIntegerInterval *type)
{
    return "int64_t";
}

template <typename T> std::string getFObjectName(T *type)
{
    if (!type)
        return "";
    auto container = type->getContainer();
    while (container)
        if (auto ptr = std::dynamic_pointer_cast<FTypeCollection>(container))
            return container->getName() + "_" + type->getName() + "_t";
        else
            container = container->getContainer();

    return type->getName();
}

static inline std::string getFTypeRefName(FTypeRef *type)
{
    if (auto ptr = type->getPredefined())
        return ptr->transform("baremetal_get_type_name");
    if (auto ptr = type->getDerived())
        return ptr->transform("baremetal_get_type_name");
    if (auto ptr = type->getInterval())
        return ptr->transform("baremetal_get_type_name");

    // empty typeref
    return "";
}

static inline std::string getEnumeratorName(FEnumerator *type)
{
    if (!type)
        return "";
    auto container = type->getContainer();
    while (container)
        if (auto ptr = std::dynamic_pointer_cast<FTypeCollection>(container))
            return toUpper(container->getName()) + "_" + type->toString();
        else
            container = container->getContainer();

    return type->getName();
}

static inline bool registerGetTypeName()
{
    if (BstIdl::FObject::hasTransformer("baremetal_get_type_name"))
        return false;

    auto t = new BstIdl::FTransformer();
    t->forFBasicTypeId = getFBasicTypeName;
    t->forFIntegerInterval = getFIntegerIntervalName;
    t->forFArrayType = getFObjectName<FArrayType>;
    t->forFEnumerationType = getFObjectName<FEnumerationType>;
    t->forFEnumerator = getEnumeratorName;
    t->forFStructType = getFObjectName<FStructType>;
    t->forFUnionType = getFObjectName<FUnionType>;
    t->forFTypeDef = getFObjectName<FTypeDef>;
    t->forFTypeRef = getFTypeRefName;
    t->forFMethod = getFObjectName<FMethod>;
    return BstIdl::FObject::addTransformer("baremetal_get_type_name", t);
}

static inline std::string getTypeName(const std::shared_ptr<FObject> &obj)
{
    if (!BstIdl::FObject::hasTransformer("baremetal_get_type_name"))
        (void)registerGetTypeName();
    return obj->transform("baremetal_get_type_name");
}

static inline std::string getTypeName(FObject *obj)
{
    if (!BstIdl::FObject::hasTransformer("baremetal_get_type_name"))
        (void)registerGetTypeName();
    return obj->transform("baremetal_get_type_name");
}

} // namespace MsgBoxGen

} // namespace Transformer

} // namespace BstIdl

#endif