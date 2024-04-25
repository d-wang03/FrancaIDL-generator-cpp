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
#ifndef TRANSFORMER_BAREMETAL_TYPEDECL_H
#define TRANSFORMER_BAREMETAL_TYPEDECL_H

#include "model/FArrayType.h"
#include "model/FConstantDef.h"
#include "model/FTransformer.h"
#include "msgbox-tools/transformer/baremetal/typename.h"
#include "utilities/string_utility.h"
#include <string>

namespace BstIdl
{
namespace Transformer
{
namespace BareMetal
{

static inline std::string getFArrayTypeDecl(FArrayType *type)
{
    if (!type)
        return "";

    std::string ret = R"(
typedef struct
{
    TYPENAME *data;
    uint32_t size;
}NAME;
)";
    replace_all(ret, "TYPENAME", getTypeName(type->getElementType()));
    replace_all(ret, "NAME", getTypeName(type));
    return ret;
}

static inline std::string getFEnumerationTypeDecl(FEnumerationType *type)
{
    if (!type)
        return "";

    std::string ret = R"(
typedef enum
{
    ENUMERATORS
}NAME;
)";
    std::list<std::string> enumerators;
    for (const auto &e : type->getEnumerators())
    {
        enumerators.emplace_back(e->toString());
    }
    replace_all(ret, "ENUMERATORS", join(enumerators, ",\n\t"));
    replace_all(ret, "NAME", getTypeName(type));
    return ret;
}

static inline std::string getFFieldDecl(FField *type)
{
    if (!type)
        return "";
    std::string ret = getTypeName(type->getType());
    ret += " " + getTypeName(type) + ";";
    return ret;
}

static inline std::string getFStructTypeDecl(FStructType *type)
{
    if (!type)
        return "";

    std::string ret = R"(
typedef struct
{
    FIELDS
}NAME;
)";
    std::list<std::string> fields;
    for (const auto &e : type->getElements())
    {
        fields.emplace_back(getFFieldDecl(e.get()));
    }
    replace_all(ret, "FIELDS", join(fields, "\n\t"));
    replace_all(ret, "NAME", getTypeName(type));
    return ret;
}

static inline std::string getFUnionTypeDecl(FUnionType *type)
{
    if (!type)
        return "";

    std::string ret = R"(
typedef union
{
    FIELDS
}NAME;
)";
    std::list<std::string> fields;
    for (const auto &e : type->getElements())
    {
        fields.emplace_back(getFFieldDecl(e.get()));
    }
    replace_all(ret, "FIELDS", join(fields, "\n\t"));
    replace_all(ret, "NAME", getTypeName(type));
    return ret;
}

static inline std::string getFTypeDefDecl(FTypeDef *type)
{
    if (!type)
        return "";
    return "\ntypedef " + getTypeName(type->getActualType()) + " " + getTypeName(type) + ";";
}

static inline std::string getFConstantDefDecl(FConstantDef *type)
{
    if (!type)
        return "";

    auto rhs = type->getRhs();
    auto exp = std::dynamic_pointer_cast<FExpression>(rhs);
    if (!exp)
        return "";

    std::string ret =
        "static const " + getTypeName(type->getType()) + " " + type->getName() + " = " + exp->toString() + ";\n";

    return ret;
}

static inline bool registerGetTypeDecl()
{
    if (BstIdl::FObject::hasTransformer("baremetal_get_type_decl"))
        return false;

    auto t = new BstIdl::FTransformer();
    t->forFArrayType = getFArrayTypeDecl;
    t->forFEnumerationType = getFEnumerationTypeDecl;
    t->forFStructType = getFStructTypeDecl;
    t->forFUnionType = getFUnionTypeDecl;
    t->forFTypeDef = getFTypeDefDecl;
    t->forFField = getFFieldDecl;
    t->forFConstantDef = getFConstantDefDecl;
    return BstIdl::FObject::addTransformer("baremetal_get_type_decl", t);
}

static inline std::string getTypeDecl(const std::shared_ptr<FObject> &obj)
{
    if (!BstIdl::FObject::hasTransformer("baremetal_get_type_decl"))
        (void)registerGetTypeDecl();
    return obj->transform("baremetal_get_type_decl");
}

static inline std::string getTypeDecl(FObject *obj)
{
    if (!BstIdl::FObject::hasTransformer("baremetal_get_type_decl"))
        (void)registerGetTypeDecl();
    return obj->transform("baremetal_get_type_decl");
}

} // namespace BareMetal

} // namespace Transformer

} // namespace BstIdl

#endif