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
#include "model/FQualifiedElementRef.h"
#include "model/FConstantDef.h"
#include "model/FStructType.h"
#include "model/FTypeDef.h"
#include "model/FTypeRef.h"
#include "model/FUnionType.h"
#include <algorithm>

namespace BstIdl
{
void FQualifiedElementRef::validate(std::shared_ptr<FTypeRef> &type, bool isArray)
{
    FExpression::validate(type, isArray);
    std::list<FQualifiedElementRef *> qerList;
    auto previous = this;
    while (previous != nullptr)
    {
        qerList.emplace_front(previous);
        previous = previous->m_qualifier.get();
    }
    auto first = qerList.front();
    auto element = first->getElement();
    if (!element)
        throw initializer_error("QualifiedElementRef:qualified element is null.");
    auto constDef = std::dynamic_pointer_cast<FConstantDef>(element);
    if (!constDef)
        throw initializer_error("QualifiedElementRef:qualified element constDefine is null.");
    auto srcType = constDef->getType();
    if (qerList.size() == 1U)
    {
        if (isTypeMatch(srcType, type))
            return;
        else
            throw initializer_error("QualifiedElementRef: Initilizer not match type.");
    }
    else
    {
        qerList.pop_front();
        for (auto item : qerList)
        {
            auto structType = std::dynamic_pointer_cast<FStructType>(srcType->getDerived());
            auto unionType = std::dynamic_pointer_cast<FUnionType>(srcType->getDerived());
            if (!structType && !unionType)
                throw initializer_error("QualifiedElementRef:Invalid qualified element reference.\tstructType and "
                                        "unionType are in conflict");
            auto elements = structType ? structType->getElements() : unionType->getElements();
            if (item->m_field == nullptr)
                throw initializer_error("QualifiedElementRef:Invalid qualified element reference.m_field is null");
            auto findName = item->m_field->getName();
            auto iter = std::find_if(elements.cbegin(), elements.cend(), [&findName](const auto &i) {
                auto ret = (i->getName() == findName);
                return ret;
            });
            if (iter == elements.cend())
                throw initializer_error("QualifiedElementRef:Invalid qualified element reference.");
            item->setElement(*iter);
            srcType = (*iter)->getType();
        }
        if (!isTypeMatch(srcType, type))
            throw initializer_error("QualifiedElementRef:Initilizer not match type.");
    }
}

void FQualifiedElementRef::EvaluableValidate(std::shared_ptr<FTypeRef> &type, bool isArray, std::string &value,
                                             bool is_init_exp)
{
    FExpression::EvaluableValidate(type, isArray, value, is_init_exp);
    auto element = getElement();
    auto constDef = std::dynamic_pointer_cast<FConstantDef>(element);
    if (constDef != nullptr)
    {
        auto real_exp = constDef->getRhs();
        real_exp->EvaluableValidate(type, isArray, value, is_init_exp);
    }
}
bool FQualifiedElementRef::isTypeMatch(const std::shared_ptr<FTypeRef> &src, const std::shared_ptr<FTypeRef> &dst)
{
    auto srcPredefined = src->getPredefined();
    auto srcInterval = src->getInterval();
    auto srcDerived = src->getDerived();
    auto dstPredefined = dst->getPredefined();
    auto dstInterval = dst->getInterval();
    auto dstDerived = dst->getDerived();

    if (dstDerived)
        return false;

    if (srcDerived)
    {
        auto srcTypeRef = getRealType(src);
        srcPredefined = srcTypeRef->getPredefined();
        srcInterval = srcTypeRef->getInterval();
        srcDerived = srcTypeRef->getDerived();
        if (srcDerived)
            return false;
    }

    if (srcPredefined && srcInterval)
        return false;
    uint32_t srcTypeID = srcPredefined ? srcPredefined->getValue() : FBasicTypeId::INT64;

    if (dstPredefined && dstInterval)
        return false;
    uint32_t dstTypeID = dstPredefined ? dstPredefined->getValue() : FBasicTypeId::INT64;

    if (srcTypeID == dstTypeID)
        return true;
    if (FBasicTypeId::isUnsigned(srcTypeID) && FBasicTypeId::isNumeric(dstTypeID))
        return true;
    if (FBasicTypeId::isSigned(srcTypeID) &&
        (FBasicTypeId::isNumeric(dstTypeID) && !FBasicTypeId::isUnsigned(dstTypeID)))
        return true;
    if (FBasicTypeId::isReal(srcTypeID) && dstTypeID == FBasicTypeId::DOUBLE)
        return true;
    return false;
}
std::shared_ptr<FTypeRef> FQualifiedElementRef::getRealType(const std::shared_ptr<FTypeRef> &src)
{
    auto realType = src;
    auto type = realType->getDerived();
    if (type)
    {
        auto typeDef = std::dynamic_pointer_cast<FTypeDef>(type);
        if (typeDef != nullptr)
        {
            realType = typeDef->getActualType();
            realType = getRealType(realType);
            return realType;
        }
        else
            return realType;
    }
    else
        return realType;
}
} // namespace BstIdl