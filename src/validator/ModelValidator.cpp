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
#include "validator/ModelValidator.h"
#include "FDModel/FDModelManager.h"
#include "fmt/core.h"
#include "model/FInterface.h"
#include "model/FModelManager.h"
#include <iostream>
#include <list>

namespace BstIdl
{
ModelValidator &ModelValidator::getInstance()
{
    static ModelValidator ins;
    return ins;
}

bool ModelValidator::validate()
{
    return validateFModels() && validateFDModels();
}
bool ModelValidator::validateFModels()
{
    std::cout << "Validating models ..." << std::endl;
    auto &mgr = FModelManager::getInstance();
    mgr.buildSymbolTable();
    std::list<std::string> errorList;
    for (const auto &model : mgr.getModelList())
    {
        for (const auto &item : model->getTypeCollections())
        {
            for (const auto &type : item->getTypes())
            {
                errorList.splice(errorList.end(), validate(type));
            }
            for (const auto &constant : item->getConstants())
            {
                errorList.splice(errorList.end(), validate(constant));
            }
        }
        for (const auto &item : model->getInterfaces())
        {
            for (const auto &type : item->getTypes())
            {
                errorList.splice(errorList.end(), validate(type));
            }
            for (const auto &constant : item->getConstants())
            {
                errorList.splice(errorList.end(), validate(constant));
            }
            for (const auto &i : item->getMethods())
            {
                errorList.splice(errorList.end(), validate(i->getErrorType(), i));
            }
            std::string location = fmt::format("for {0}", item->getFQN());
            errorList.splice(errorList.end(), validate(item->getBroadcasts(), location));
            errorList.splice(errorList.end(), validate(item->getMethods(), location));
            // validate fixed size attributes
            errorList.splice(errorList.end(), validate(item->getMethods(), location));
        }
    }

    bool ret = errorList.empty();
    if (ret)
        std::cout << "Success!" << std::endl;
    else
        std::cerr << join(errorList, "\n") << std::endl;
    return ret;
}

bool ModelValidator::validateFDModels()
{
    std::cout << "Validating deployment models ..." << std::endl;
    const auto &fdmgr = FDModelManager::getInstance();
    std::list<std::string> errorList;
    std::list<std::string> labels;
    for (const auto &fdmodel : fdmgr.getModelList())
    {
        for (const auto &deployment : fdmodel->getDeployments())
        {
            if (!deployment)
                continue;
            // validate specification
            auto spec = deployment->getSpec();
            if (!spec)
            {
                errorList.emplace_back(fmt::format("No specification in deployment {0}", deployment->getName()));
            }

            // validate interface deployment labels.
            if (auto ptr = std::dynamic_pointer_cast<FDInterface>(deployment))
            {
                if (spec && ptr->getTarget())
                {
                    auto label = spec->getName() + "_" + ptr->getTarget()->getFQN();
                    if (contains(labels, label))
                        errorList.emplace_back(
                            fmt::format("Duplicate interface deployment for specification {0} and interface {1}",
                                        spec->getName(), ptr->getTarget()->getFQN()));
                    else
                        labels.emplace_back(label);
                    errorList.splice(errorList.end(), validate(ptr));
                }
            }

            // validate type collection deployment labels.
            if (auto ptr = std::dynamic_pointer_cast<FDTypes>(deployment))
            {
                if (spec && ptr->getTarget())
                {
                    auto label = spec->getName() + "_" + ptr->getTarget()->getFQN();
                    if (contains(labels, label))
                        errorList.emplace_back(fmt::format(
                            "Duplicate type collection deployment for specification {0} and type collection {1}",
                            spec->getName(), ptr->getTarget()->getFQN()));
                    else
                        labels.emplace_back(label);
                    errorList.splice(errorList.end(), validate(ptr));
                }
            }

            // validate extension deployment labels.
            if (auto ptr = std::dynamic_pointer_cast<FDExtensionRoot>(deployment))
            {
                if (spec && !ptr->getTag().empty())
                {
                    auto label = spec->getName() + "_" + ptr->getTag();
                    if (contains(labels, label))
                        errorList.emplace_back(
                            fmt::format("Duplicate extension deployment for specification {0} and tag {1}",
                                        spec->getName(), ptr->getTag()));
                    else
                        labels.emplace_back(label);
                    errorList.splice(errorList.end(), validate(ptr));
                }
            }
        }
    }
    bool ret = errorList.empty();
    if (ret)
        std::cout << "Success!" << std::endl;
    else
        std::cerr << join(errorList, "\n") << std::endl;
    return ret;
}

std::list<std::string> ModelValidator::validate(const std::shared_ptr<FType> &type)
{
    std::list<std::string> errorList;
    errorList.splice(errorList.end(), validate(std::dynamic_pointer_cast<FEnumerationType>(type), nullptr));
    errorList.splice(errorList.end(), validate(std::dynamic_pointer_cast<FStructType>(type)));
    errorList.splice(errorList.end(), validate(std::dynamic_pointer_cast<FUnionType>(type)));
    errorList.splice(errorList.end(), validate(std::dynamic_pointer_cast<FTypeDef>(type)));
    errorList.splice(errorList.end(), validate(std::dynamic_pointer_cast<FArrayType>(type)));

    return errorList;
}

std::list<std::string> ModelValidator::validate(const std::shared_ptr<FConstantDef> &constant)
{
    std::list<std::string> errorList;
    if (!constant)
        return errorList;
    auto rhs = constant->getRhs();
    auto type = constant->getType();
    std::string value;
    try
    {
        rhs->validate(type, constant->isArray());
        rhs->EvaluableValidate(type, constant->isArray(), value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        errorList.emplace_back(e.what());
    }
    return errorList;
}

std::list<std::string> ModelValidator::validate(const std::shared_ptr<FEnumerationType> &type,
                                                const std::shared_ptr<BstIdl::FObject> &object)
{
    std::list<std::string> errorList;
    if (!type)
        return errorList;

    std::list<std::string> itemList;
    auto method = std::dynamic_pointer_cast<BstIdl::FMethod>(object);
    for (const auto &item : type->getEnumerators())
    {
        itemList.emplace_back(item->getName());
    }
    auto parent = type->getBase();
    while (parent != nullptr)
    {
        for (const auto &item : parent->getEnumerators())
        {
            itemList.emplace_back(item->getName());
        }
        parent = parent->getBase();
    }

    itemList.sort();

    std::string previous;
    for (const auto &item : itemList)
    {
        if (item == previous)
            errorList.emplace_back(
                fmt::format("Duplicate enumerator \"{0}\" in enumeration \"{1}\"", item, type->getFQN()));
        previous = item;
    }

    return errorList;
}

std::list<std::string> ModelValidator::validate(const std::shared_ptr<FStructType> &type)
{
    std::list<std::string> errorList;
    if (!type)
        return errorList;
    std::list<std::string> itemList;
    for (const auto &item : type->getElements())
    {
        itemList.emplace_back(item->getName());
        // validate fixed size field
        errorList.splice(errorList.end(), validate(item));
    }
    auto parent = type->getBase();
    while (parent != nullptr)
    {
        for (const auto &item : parent->getElements())
        {
            itemList.emplace_back(item->getName());
        }
        parent = parent->getBase();
    }
    itemList.sort();

    auto &mgr = BstIdl::FModelManager::getInstance();
    std::string previous;
    for (const auto &item : itemList)
    {
        if (item == previous)
            errorList.emplace_back(fmt::format("Duplicate element \"{0}\" in struct \"{1}\"", item, type->getFQN()));
        previous = item;
    }

    return errorList;
}

std::list<std::string> ModelValidator::validate(const std::shared_ptr<FUnionType> &type)
{
    std::list<std::string> errorList;
    if (!type)
        return errorList;
    std::list<std::string> nameList, typeList;
    for (const auto &item : type->getElements())
    {
        nameList.emplace_back(item->getName());
        typeList.emplace_back(item->getType()->toString());
        // validate fixed size field
        errorList.splice(errorList.end(), validate(item));
    }

    auto parent = type->getBase();
    while (parent != nullptr)
    {
        for (const auto &item : parent->getElements())
        {
            nameList.emplace_back(item->getName());
            typeList.emplace_back(item->getType()->toString());
        }
        parent = parent->getBase();
    }
    nameList.sort();
    typeList.sort();

    std::string previous;
    for (const auto &item : nameList)
    {
        if (item == previous)
            errorList.emplace_back(fmt::format("Duplicate element \"{0}\" in union \"{1}\"", item, type->getFQN()));
        previous = item;
    }
    previous.clear();
    for (const auto &item : typeList)
    {
        if (item == previous)
            errorList.emplace_back(
                fmt::format("Duplicate element type \"{0}\" in union \"{1}\"", item, type->getFQN()));
        previous = item;
    }

    return errorList;
}

std::list<std::string> ModelValidator::validate(const std::shared_ptr<FTypeDef> &type)
{
    std::list<std::string> errorList;
    if (!type)
        return errorList;
    auto actual = type->getActualType();
    if (actual->getDerived() == type)
        errorList.emplace_back(fmt::format("Invalid typedef {0}, pointing to itself", type->getFQN()));
    return errorList;
}

std::list<std::string> ModelValidator::validate(const std::shared_ptr<FArrayType> &fArray)
{
    std::list<std::string> errorList;
    if (!fArray || !fArray->isFixedSize())
        return errorList;

    // validate fixed size logicalOrExpression :BinaryOperation/UnaryOperation/QER/ConstantRef
    auto rhs = fArray->getFixedSize();
    auto typeRef = std::make_shared<FTypeRef>();
    auto type = BstIdl::FBasicTypeId::get(2U);
    typeRef->setPredefined(type);
    std::string value;
    try
    {
        rhs->EvaluableValidate(typeRef, true, value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        errorList.emplace_back(e.what());
    }
    return errorList;
}

std::list<std::string> ModelValidator::validate(const std::shared_ptr<FArgument> &fArg)
{
    std::list<std::string> errorList;
    if (!fArg || !fArg->isArray() || !fArg->isFixedSize())
        return errorList;

    // validate fixed size logicalOrExpression :BinaryOperation/UnaryOperation/QER/ConstantRef
    auto rhs = fArg->getFixedSize();
    auto typeRef = std::make_shared<FTypeRef>();
    auto type = BstIdl::FBasicTypeId::get(2U);
    typeRef->setPredefined(type);
    std::string value;
    try
    {
        rhs->EvaluableValidate(typeRef, true, value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        errorList.emplace_back(e.what());
    }
    return errorList;
}

std::list<std::string> ModelValidator::validate(const std::shared_ptr<FField> &field)
{
    std::list<std::string> errorList;
    if (!field || !field->isArray() || !field->isFixedSize())
        return errorList;

    // validate fixed size logicalOrExpression :BinaryOperation/UnaryOperation/QER/ConstantRef
    auto rhs = field->getFixedSize();
    auto typeRef = std::make_shared<FTypeRef>();
    auto type = BstIdl::FBasicTypeId::get(2U);
    typeRef->setPredefined(type);
    std::string value;
    try
    {
        rhs->EvaluableValidate(typeRef, true, value, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        errorList.emplace_back(e.what());
    }
    return errorList;
}

std::list<std::string> ModelValidator::validate(const std::list<std::shared_ptr<BstIdl::FBroadcast>> &elements,
                                                const std::string &location)
{
    std::list<std::string> errorList;
    std::list<std::string> labels;
    // this list to avoid same bc def.
    std::list<std::string> bc_msg_check;
    for (auto item : elements)
    {
        if (!item)
            continue;
        else
        {
            auto fqn = item->getFQN();
            auto label = fqn.substr(fqn.find_last_of(".") + 1);
            if (label.empty())
                label = item->getName();
            auto bc_msg = label.substr(0, label.find_last_of(":"));
            for (const auto &out : item->getOutArgs())
                bc_msg.append("\nOUT_ARGS: " + out->getType()->toStringRecursively());

            if (contains(labels, label))
                errorList.emplace_back(fmt::format("Duplicate {0} in {1}.", label, location));
            else if (contains(bc_msg_check, bc_msg))
                errorList.emplace_back(fmt::format("Duplicate overrided broadcast {0} {1}.", label, location));
            else
            {
                bc_msg_check.emplace_back(bc_msg);
                labels.emplace_back(label);
            }
            // validate fixed size argument
            for (const auto &out : item->getOutArgs())
                errorList.splice(errorList.end(), validate(out));
        }
    }
    return errorList;
}

std::list<std::string> ModelValidator::validate(const std::list<std::shared_ptr<BstIdl::FMethod>> &elements,
                                                const std::string &location)
{
    std::list<std::string> errorList;
    std::list<std::string> labels;
    // this list to avoid same md def.
    std::list<std::string> md_msg_check;
    for (auto item : elements)
    {
        if (!item)
            continue;
        else
        {
            auto fqn = item->getFQN();
            auto label = fqn.substr(fqn.find_last_of(".") + 1);
            if (label.empty())
                label = item->getName();
            // override msg check
            auto md_msg = label.substr(0, label.find_last_of(":"));
            if (auto err = item->getErrorType())
            {
                std::string ret("base: ");
                if (auto base = err->getBase())
                    ret.append(base->getFQN());
                ret.append("\nelements: ");
                for (const auto &ele : err->getEnumerators())
                {
                    ret.append("\n");
                    ret.append(ele->toString());
                }
                md_msg.append(ret);
            }
            for (const auto &in : item->getInArgs())
                md_msg.append("\nIN_ARGS: " + in->getType()->toStringRecursively());
            for (const auto &out : item->getOutArgs())
                md_msg.append("\nOUT_ARGS: " + out->getType()->toStringRecursively());

            if (contains(labels, label))
                errorList.emplace_back(fmt::format("Duplicate {0} in {1}.", label, location));
            else if (contains(md_msg_check, md_msg))
                errorList.emplace_back(fmt::format("Duplicate overrided method {0} {1}.", label, location));
            else
            {
                md_msg_check.emplace_back(md_msg);
                labels.emplace_back(label);
            }
            // validate fixed size argument
            for (const auto &in : item->getInArgs())
                errorList.splice(errorList.end(), validate(in));
            for (const auto &out : item->getOutArgs())
                errorList.splice(errorList.end(), validate(out));
        }
    }
    return errorList;
}

template <typename T>
std::list<std::string> validateElement(const std::list<std::shared_ptr<T>> &elements, const std::string &location)
{
    std::list<std::string> errorList;
    std::list<std::string> labels;
    for (auto item : elements)
    {
        if (!item)
            continue;
        if (!item->getTarget())
            errorList.emplace_back(fmt::format("Undefined {0} in {1}.", item->getName(), location));
        else
        {
            auto fqn = item->getTarget()->getFQN();
            auto label = fqn.substr(fqn.find_last_of(".") + 1);
            if (label.empty())
                label = item->getName();
            if (contains(labels, label))
                errorList.emplace_back(fmt::format("Duplicate {0} in {1}.", label, location));
            else
                labels.emplace_back(label);
        }
    }
    return errorList;
}

std::list<std::string> ModelValidator::validate(const std::shared_ptr<FDInterface> &fdInf)
{
    std::list<std::string> errorList;
    if (fdInf && fdInf->getSpec() && fdInf->getTarget())
    {
        std::string location = fmt::format("{0} for {1}", fdInf->getSpec()->getName(), fdInf->getTarget()->getFQN());
        errorList.splice(errorList.end(), validateElement(fdInf->getAttributes(), location));
        errorList.splice(errorList.end(), validateElement(fdInf->getMethods(), location));
        errorList.splice(errorList.end(), validateElement(fdInf->getBroadcasts(), location));
        errorList.splice(errorList.end(), validateTypeDefinition(fdInf, fdInf->getTypes()));
        for (auto i : fdInf->getMethods())
        {
            auto loc = fmt::format("{0} in {1}", i->getName(), location);
            if (auto args = i->getInArguments())
                errorList.splice(errorList.end(), validateElement(args->getArguments(), loc));
            if (auto args = i->getOutArguments())
                errorList.splice(errorList.end(), validateElement(args->getArguments(), loc));
        }
        for (auto i : fdInf->getBroadcasts())
        {
            auto loc = fmt::format("{0} in {1}", i->getName(), location);
            if (auto args = i->getOutArguments())
                errorList.splice(errorList.end(), validateElement(args->getArguments(), loc));
        }
    }
    return errorList;
}
std::list<std::string> ModelValidator::validate(const std::shared_ptr<FDTypes> &fdTc)
{
    std::list<std::string> errorList;
    if (fdTc && fdTc->getSpec() && fdTc->getTarget())
        errorList.splice(errorList.end(), validateTypeDefinition(fdTc, fdTc->getTypes()));
    return errorList;
}

std::list<std::string> validateExtensionElements(const std::list<std::shared_ptr<FDExtensionElement>> &elements,
                                                 const std::string &parentName)
{
    std::list<std::string> errorList;
    std::list<std::string> labels;
    for (auto element : elements)
    {
        auto label = element->getTag() + "_" + element->getName();
        if (contains(labels, label))
            errorList.emplace_back(fmt::format("Duplicate deployment {0} with tag {1} in {2}", element->getName(),
                                               element->getTag(), parentName));
        else
            labels.emplace_back(label);

        errorList.splice(errorList.end(), validateExtensionElements(element->getElements(), element->getName()));
    }
    return errorList;
}

std::list<std::string> ModelValidator::validate(const std::shared_ptr<FDExtensionRoot> &fdEr)
{
    std::list<std::string> errorList;
    if (fdEr)
        errorList.splice(errorList.end(), validateExtensionElements(fdEr->getElements(), fdEr->getName()));
    return errorList;
}

template <typename T>
std::string validateType(const std::shared_ptr<T> &type, std::list<std::string> &labels, const std::string &location)
{
    if (!type)
        return "";

    std::string ret;
    if (!type->getTarget())
        ret = fmt::format("Undefined {0} in {1}.", type->getFQN(), location);
    else
    {
        auto label = type->getTarget()->getFQN();
        if (contains(labels, label))
        {
            if (type->getFQN() == label)
                ret = fmt::format("Duplicate deployment {0} in {1}.", label, location);
        }
        else
            labels.emplace_back(label);
    }
    return ret;
}

std::list<std::string> ModelValidator::validateTypeDefinition(
    const std::shared_ptr<FDRootElement> &parent, const std::list<std::shared_ptr<FDTypeDefinition>> &elements)
{
    std::list<std::string> errorList;
    if (!parent || !parent->getSpec())
        return errorList;

    auto specName = parent->getSpec()->getName();
    auto parentName = parent->getName();
    auto location = fmt::format("{0} for {1}", specName, parentName);
    std::list<std::string> labels;
    for (auto item : elements)
    {
        if (auto ptr = std::dynamic_pointer_cast<FDArray>(item))
        {
            auto error = validateType(ptr, labels, location);
            if (!error.empty())
                errorList.emplace_back(std::move(error));
        }
        else if (auto ptr = std::dynamic_pointer_cast<FDUnion>(item))
        {
            auto error = validateType(ptr, labels, location);
            if (!error.empty())
                errorList.emplace_back(std::move(error));
            for (auto f : ptr->getFields())
            {
                error = validateType(f, labels, location);
                if (!error.empty())
                    errorList.emplace_back(std::move(error));
            }
        }
        else if (auto ptr = std::dynamic_pointer_cast<FDStruct>(item))
        {
            auto error = validateType(ptr, labels, location);
            if (!error.empty())
                errorList.emplace_back(std::move(error));
            for (auto f : ptr->getFields())
            {
                error = validateType(f, labels, location);
                if (!error.empty())
                    errorList.emplace_back(std::move(error));
            }
        }
        else if (auto ptr = std::dynamic_pointer_cast<FDEnumeration>(item))
        {
            auto error = validateType(ptr, labels, location);
            if (!error.empty())
                errorList.emplace_back(std::move(error));
            for (auto e : ptr->getEnumerators())
            {
                error = validateType(e, labels, location);
                if (!error.empty())
                    errorList.emplace_back(std::move(error));
            }
        }
        else if (auto ptr = std::dynamic_pointer_cast<FDTypedef>(item))
        {
            auto error = validateType(ptr, labels, location);
            if (!error.empty())
                errorList.emplace_back(std::move(error));
        }
        else if (auto ptr = std::dynamic_pointer_cast<FDMap>(item))
        {
            auto error = validateType(ptr, labels, location);
            if (!error.empty())
                errorList.emplace_back(std::move(error));
        }
    }
    return errorList;
}

} // namespace BstIdl