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
#include "generator/HTMLGenerator.h"
#include "utilities/string_utility.h"
#include <experimental/filesystem>
#include <iostream>
namespace fs = std::experimental::filesystem;

namespace BstIdl
{
bool HTMLGenerator::generate()
{
    // make doc dir
    if (m_destDir.empty())
        m_destDir = fs::current_path().string();
    std::error_code ec;
    if (!fs::create_directories(m_destDir + "/doc", ec) && ec)
    {
        std::cerr << "Creating directory failed! " << ec.message() << std::endl;
        return false;
    }

    FModelManager &idl_mgr = FModelManager::getInstance();
    auto models = idl_mgr.getModelList();
    for (const auto &model : models)
    {
        std::string outputFileName = m_destDir + "/doc/";
        auto infs = model->getInterfaces();
        if (infs.size() == 1)
            outputFileName += model->getName() + "." + infs.front()->getName() + ".html";
        else
            outputFileName += model->getName() + ".html";
        std::ofstream OsWrite(outputFileName, std::ofstream::out);
        auto html = makeModel(model);
        OsWrite << html;
        OsWrite << std::endl;
        OsWrite.close();
    }
    return true;
}
std::string HTMLGenerator::makeModel(const std::shared_ptr<FModel> &model)
{
    auto name = model->getName();
    auto interfaces = makeInterfaces(model->getInterfaces());
    auto typeCollections = makeTypeCollections(model->getTypeCollections());
    auto ret = t_model;
    ret = ret.replace(ret.find("NAME"), 4, name);
    ret = ret.replace(ret.find("INTERFACES"), 10, interfaces);
    ret = ret.replace(ret.find("TYPECOLLECTIONS"), 15, typeCollections);
    return ret;
}
std::string HTMLGenerator::makeInterfaces(const std::list<std::shared_ptr<FInterface>> &interfaces)
{
    std::string ret;
    for (auto interface : interfaces)
    {
        ret += makeInterface(interface);
    }
    return ret;
}
std::string HTMLGenerator::makeInterface(const std::shared_ptr<FInterface> &interface)
{
    auto name = interface->getName();
    auto version = interface->getVersion();
    auto version_ = t_interface_version;
    if (version != nullptr)
    {
        auto major_version = std::to_string(interface->getVersion()->getMajor());
        auto minor_version = std::to_string(interface->getVersion()->getMinor());
        version_ = version_.replace(version_.find("VERSION_MAJOR"), 13, major_version);
        version_ = version_.replace(version_.find("VERSION_MINOR"), 13, minor_version);
    }
    else
    {
        version_ = "";
    }

    auto comment = makeComment(interface, true);
    auto attributes = makeAttributes(interface->getAttributes());
    auto methods = makeMethods(interface->getMethods());
    auto broadcasts = makeBroadcasts(interface->getBroadcasts());
    auto types = makeTypes(interface->getTypes());
    auto ret = t_interface;
    ret = ret.replace(ret.find("NAME"), 4, name);
    ret = ret.replace(ret.find("COMMENT"), 7, comment);
    ret = ret.replace(ret.find("VERSION"), 7, version_);
    ret = ret.replace(ret.find("ATTRIBUTES"), 10, attributes);
    ret = ret.replace(ret.find("METHODS"), 7, methods);
    ret = ret.replace(ret.find("BROADCASTS"), 10, broadcasts);
    ret = ret.replace(ret.find("TYPES"), 5, types);
    return ret;
}
std::string HTMLGenerator::makeTypeCollections(const std::list<std::shared_ptr<FTypeCollection>> &typeCollections)
{
    std::string ret;
    for (auto typeCollection : typeCollections)
    {
        ret += makeTypeCollection(typeCollection);
    }
    return ret;
}
std::string HTMLGenerator::makeTypeCollection(const std::shared_ptr<FTypeCollection> &typeCollection)
{
    auto name = typeCollection->getName();
    auto version = typeCollection->getVersion();
    auto version_ = t_interface_version;
    if (version != nullptr)
    {
        auto major_version = std::to_string(version->getMajor());
        auto minor_version = std::to_string(version->getMinor());
        version_ = version_.replace(version_.find("VERSION_MAJOR"), 13, major_version);
        version_ = version_.replace(version_.find("VERSION_MINOR"), 13, minor_version);
    }
    else
    {
        version_ = "";
    }

    auto comment = makeComment(typeCollection, true);
    auto types = makeTypes(typeCollection->getTypes());
    auto ret = t_type_collection;
    ret = ret.replace(ret.find("NAME"), 4, name);
    ret = ret.replace(ret.find("COMMENT"), 7, comment);
    ret = ret.replace(ret.find("VERSION"), 7, version_);
    ret = ret.replace(ret.find("TYPES"), 5, types);
    return ret;
}
std::string HTMLGenerator::makeAttributes(const std::list<std::shared_ptr<FAttribute>> &attributes)
{
    std::string ret;
    std::string attributes_;
    for (auto it = attributes.begin(); it != attributes.end(); it++)
    {
        attributes_ += makeAttribute(*it);
    }
    if (!attributes_.empty())
    {
        ret = t_attribute;
        ret = ret.replace(ret.find("ATTRIBUTE_ITEM"), 14, attributes_);
    }
    return ret;
}
std::string HTMLGenerator::makeAttribute(const std::shared_ptr<FAttribute> &attribute)
{
    auto name = attribute->getName();
    auto type = attribute->toString();
    auto comment = makeComment(attribute, false);
    auto ret = t_attribute_item;
    ret = ret.replace(ret.find("NAME"), 4, name);
    ret = ret.replace(ret.find("COMMENT"), 7, comment);
    ret = ret.replace(ret.find("TYPE"), 4, type);
    return ret;
}
std::string HTMLGenerator::makeMethods(const std::list<std::shared_ptr<FMethod>> &methods)
{
    std::string ret;
    std::string methods_;
    for (auto it = methods.begin(); it != methods.end(); it++)
    {
        methods_ += makeMethod(*it);
    }
    if (!methods_.empty())
    {
        ret = t_method;
        ret = ret.replace(ret.find("METHOD_ITEM"), 11, methods_);
    }
    return ret;
}
std::string HTMLGenerator::makeMethod(const std::shared_ptr<FMethod> &method)
{
    auto name = method->getName();
    auto comment = makeMethodComment(method);
    auto in_args = makeArguments(method->getInArgs(), "in");
    auto out_args = makeArguments(method->getOutArgs(), "out");
    auto errors_ = method->getErrors();
    auto errors = makeError(errors_);

    auto method_gen = t_method_gen;
    if (in_args.empty() && out_args.empty())
        method_gen.clear();
    else
    {
        method_gen = method_gen.replace(method_gen.find("INARGS"), 6, in_args);
        method_gen = method_gen.replace(method_gen.find("OUTARGS"), 7, out_args);
    }
    auto ret = t_method_item;
    ret = ret.replace(ret.find("NAME"), 4, name);
    ret = ret.replace(ret.find("COMMENT"), 7, comment);
    ret = ret.replace(ret.find("METHOD_GEN"), 10, method_gen);
    return ret;
}
std::string HTMLGenerator::makeMethodComment(const std::shared_ptr<FMethod> &method)
{
    if (!method)
        return "";

    auto comments_list = method->getComment();
    if (comments_list == nullptr)
        return "";
    auto &comments = comments_list->getElements();
    comments.remove_if([&method](const auto &comment) {
        if (comment->getType().getValue() != FAnnotationType::PARAM_VALUE)
            return false;

        // get param id;
        auto content = comment->getComment();
        auto sep = content.find_first_of(':');
        if (sep == std::string::npos)
            sep = content.find_first_of(' ');
        if (sep == std::string::npos)
            sep = content.find_first_of('\t');
        if (sep == std::string::npos)
            return false;
        if (sep + 1 >= content.size())
            return false;
        auto id = content.substr(0, sep);

        // replace argment comment.
        for (auto &arg : method->getInArgs())
        {
            if (id == arg->getName() && !arg->getComment())
            {
                auto block = FFactory::getInstance().createFAnnotationBlock();
                auto anno = std::make_shared<FAnnotation>("@description:" + content.substr(sep + 1));
                block->getElements().emplace_back(std::move(anno));
                arg->setComment(std::move(block));
                return true;
            }
        }
        for (auto &arg : method->getOutArgs())
        {
            if (id == arg->getName() && !arg->getComment())
            {
                auto block = FFactory::getInstance().createFAnnotationBlock();
                auto anno = std::make_shared<FAnnotation>("@description:" + content.substr(sep));
                block->getElements().emplace_back(std::move(anno));
                arg->setComment(std::move(block));
                return true;
            }
        }

        return false;
    });
    return makeComment(method, true);
}
std::string HTMLGenerator::makeBroadcasts(const std::list<std::shared_ptr<FBroadcast>> &broadcasts)
{
    std::string ret;
    std::string broadcasts_;
    for (auto it = broadcasts.begin(); it != broadcasts.end(); it++)
    {
        broadcasts_ += makeBroadcast(*it);
    }
    if (!broadcasts_.empty())
    {
        ret = t_broadcast;
        ret = ret.replace(ret.find("BROADCAST_ITEM"), 14, broadcasts_);
    }
    return ret;
}
std::string HTMLGenerator::makeBroadcast(const std::shared_ptr<FBroadcast> &broadcast)
{
    auto name = broadcast->getName();
    auto comment = makeComment(broadcast, true);
    std::string broadcast_gen = t_broadcast_gen;
    auto out_args = broadcast->getOutArgs();
    if (out_args.empty())
        broadcast_gen.clear();
    else
        broadcast_gen = broadcast_gen.replace(broadcast_gen.find("OUTARGS"), 7, makeArguments(out_args, "out"));
    auto ret = t_broadcast_item;
    ret = ret.replace(ret.find("NAME"), 4, name);
    ret = ret.replace(ret.find("COMMENT"), 7, comment);
    ret = ret.replace(ret.find("BROADCAST_GEN"), 13, broadcast_gen);
    return ret;
}
std::string HTMLGenerator::makeError(const std::shared_ptr<FEnumerationType> &errors)
{
    std::string ret;
    if (errors == nullptr)
    {
        return ret;
    }
    auto error_base = errors->getBase();
    if (error_base != nullptr)
    {
        ret = t_error_base;
        ret = ret.replace(ret.find("ERROR_EXTENDS"), 13, makeExtend(errors));
    }
    auto error_enumerators = errors->getEnumerators();
    for (auto it = error_enumerators.begin(); it != error_enumerators.end(); it++)
    {
        ret += t_error_enumrators;
        auto name = (*it)->toString();
        ret = ret.replace(ret.find("NAME"), 4, name);
        ret = ret.replace(ret.find("COMMENT"), 7, makeComment(*it, false));
    }
    return ret;
}
std::string HTMLGenerator::makeErrorEnumerator(const std::shared_ptr<FEnumerationType> &error_enum)
{
    auto ret = t_error_enum;
    if (error_enum != nullptr)
        ret = ret.replace(ret.find("ERROR_ENUM_LINK"), 15, makeLabelLink(error_enum));
    else
    {
        ret = "";
    }
    return ret;
}
std::string HTMLGenerator::makeArguments(const std::list<std::shared_ptr<FArgument>> &arg, const std::string &tag)
{
    std::string ret;
    for (auto it : arg)
    {
        auto name = it->getName();
        ret += t_argument;
        ret = ret.replace(ret.find(">NAME<"), 6, ">" + name + "<");
        ret = ret.replace(ret.find(">TAG<"), 5, ">" + tag + "<");
        ret = ret.replace(ret.find(">TYPE<"), 6, ">" + makeLabelLink(it->getType()) + "<");
        ret = ret.replace(ret.find(">COMMENT<"), 9, ">" + makeComment(it, false) + "<");
    }
    return ret;
}
std::string HTMLGenerator::makeTypes(const std::list<std::shared_ptr<FType>> &types)
{
    std::string ret;
    std::string types_;
    for (auto it = types.begin(); it != types.end(); it++)
    {
        types_ += makeTypesItem(*it);
    }
    if (!types_.empty())
    {
        ret = t_types_gen;
        ret = ret.replace(ret.find("TYPE_ITEM"), 9, types_);
    }
    return ret;
}
std::string HTMLGenerator::makeTypesItem(const std::shared_ptr<FType> type)
{
    std::string ret = t_type_item;
    auto name = type->getName();
    auto base = makeExtend(type);
    ret = ret.replace(ret.find("NAME"), 4, name);
    ret = ret.replace(ret.find("NAME"), 4, name);
    ret = ret.replace(ret.find("COMMENT"), 7, makeComment(type, false));
    ret = ret.replace(ret.find("BASE"), 4, base);
    auto array_type = std::dynamic_pointer_cast<FArrayType>(type);
    auto compound_type = std::dynamic_pointer_cast<FCompoundType>(type);
    auto enumeration_type = std::dynamic_pointer_cast<FEnumerationType>(type);
    auto integerInterval = std::dynamic_pointer_cast<FIntegerInterval>(type);
    auto mapType = std::dynamic_pointer_cast<FMapType>(type);
    auto typeDef = std::dynamic_pointer_cast<FTypeDef>(type);
    if (array_type != nullptr)
    {
        auto element_type = array_type->getElementType()->toString();
        auto array_ = t_array_type_def;
        array_ = array_.replace(array_.find("ELEMENT_TYPE"), 12, element_type);
        ret = ret.replace(ret.find("GENERATE_DEFINITION"), 19, array_);
    }
    else if (compound_type != nullptr)
    {
        auto compound_ = makeCompound(compound_type);
        ret = ret.replace(ret.find("GENERATE_DEFINITION"), 19, compound_);
    }
    else if (enumeration_type != nullptr)
    {
        auto enumration_ = makeEnumration(enumeration_type);
        ret = ret.replace(ret.find("GENERATE_DEFINITION"), 19, enumration_);
    }
    else if (integerInterval != nullptr)
    {
        auto upper = integerInterval->getUpperBound();
        auto lower = integerInterval->getLowerBound();
        auto integer_ = t_integerInterval_def;
        integer_ = integer_.replace(integer_.find("UPPER"), 5, std::to_string(upper));
        integer_ = integer_.replace(integer_.find("LOWER"), 5, std::to_string(lower));
        ret = ret.replace(ret.find("GENERATE_DEFINITION"), 19, integer_);
    }
    else if (mapType != nullptr)
    {
        auto key = mapType->getKeyType()->toString();
        auto value = mapType->getValueType()->toString();
        auto map_ = t_map_type_def;
        map_ = map_.replace(t_map_type_def.find("KEY"), 3, key);
        map_ = map_.replace(map_.find("VALUE"), 5, value);
        ret = ret.replace(ret.find("GENERATE_DEFINITION"), 19, map_);
    }
    else if (typeDef != nullptr)
    {
        auto realType_ = t_type_def_def;
        realType_ = realType_.replace(realType_.find("ACTUALTYPE"), 10, makeLabelLink(typeDef->getActualType()));
        ret = ret.replace(ret.find("GENERATE_DEFINITION"), 19, realType_);
    }
    else
    {
        ret = nullptr;
    }
    return ret;
}
std::string HTMLGenerator::makeCompound(const std::shared_ptr<FCompoundType> &compount)
{
    std::string ret = t_compound_type_def;
    std::string elements;
    auto elements_ = compount->getElements();
    for (auto element : elements_)
    {
        elements += makeCompoundItem(element);
    }
    ret = ret.replace(ret.find("COMPOUND_TYPE_ITEM"), 18, elements);
    return ret;
}
std::string HTMLGenerator::makeCompoundItem(const std::shared_ptr<FField> &element)
{
    std::string ret = t_compound_type_item;
    auto name = element->getName();
    auto type = element->getType()->toString();
    ret = ret.replace(ret.find("NAME"), 4, name);
    ret = ret.replace(ret.find("TYPE"), 4, type);
    ret = ret.replace(ret.find("COMMENT"), 7, makeComment(element, false));
    return ret;
}
std::string HTMLGenerator::makeEnumration(const std::shared_ptr<FEnumerationType> &enumeration)
{
    std::string ret = t_enum_type_def;
    std::string enumerators;
    auto enumrators_ = enumeration->getEnumerators();
    for (auto enumerator : enumrators_)
    {
        enumerators += makeEnumrator(enumerator);
    }
    ret = ret.replace(ret.find("ENUMERATORS"), 11, enumerators);
    return ret;
}
std::string HTMLGenerator::makeEnumrator(const std::shared_ptr<FEnumerator> &enumerator)
{
    std::string ret = t_enumrator;
    auto comment = enumerator->getComment();
    auto name = enumerator->getName();
    auto value = enumerator->valueToString();
    ret = ret.replace(ret.find("NAME"), 4, name);
    ret = ret.replace(ret.find("VALUE"), 5, value);
    ret = ret.replace(ret.find("COMMENT"), 7, makeComment(enumerator, false));
    return ret;
}
std::string HTMLGenerator::makeComment(const std::shared_ptr<FModelElement> &element, bool paragraph)
{
    std::string ret;
    auto comments_list = element->getComment();
    if (comments_list == nullptr)
        return "";
    auto comments = comments_list->getElements();
    if (comments.size() == 1)
    {
        auto comment = comments.front();
        if (comment->getType().getValue() == FAnnotationType::DESCRIPTION)
        {
            ret = t_single_line_comment_des;
            makeSingleLineComment(comment, ret, true);
        }
        else
        {
            ret = t_single_line_comment;
            makeSingleLineComment(comment, ret, false);
        }
    }
    else if (comments.size() > 1)
    {
        for (auto comment : comments)
        {
            ret += t_multi_line_comment;
            makeSingleLineComment(comment, ret, false);
        }
        ret = "<table border=1>" + ret + "</table>";
    }
    else
    {
        ret = "";
    }
    if (paragraph)
    {
        ret = "<p>" + ret + "</p>";
    }
    return ret;
}
std::string HTMLGenerator::makeExtend(const std::shared_ptr<FType> &type)
{
    std::string ret;
    auto struct_type = std::dynamic_pointer_cast<FStructType>(type);
    auto union_type = std::dynamic_pointer_cast<FUnionType>(type);
    auto enumration_type = std::dynamic_pointer_cast<FEnumerationType>(type);
    if (struct_type != nullptr)
    {
        ret = t_extends;
        auto base = struct_type->getBase();
        if (base == nullptr)
            ret = "";
        else
            ret = ret.replace(ret.find("EXTEND"), 6, struct_type->getBase()->getName());
    }
    else if (union_type != nullptr)
    {
        ret = t_extends;
        auto base = union_type->getBase();
        if (base == nullptr)
            ret = "";
        else
            ret = ret.replace(ret.find("EXTEND"), 6, union_type->getBase()->getName());
    }
    else if (enumration_type != nullptr)
    {
        ret = t_extends;
        auto base = enumration_type->getBase();
        if (base == nullptr)
            ret = "";
        else
            ret = ret.replace(ret.find("EXTEND"), 6, enumration_type->getBase()->getName());
    }
    else
    {
        ret = "";
    }
    return ret;
}
std::string HTMLGenerator::makeLabelLink(const std::shared_ptr<FTypeRef> &type)
{
    auto derived = type->getDerived();
    auto predefined = type->getPredefined();
    auto interval = type->getInterval();
    if (derived != nullptr)
        return makeLabelLink(derived);
    else if (predefined != nullptr)
        return predefined->getName();
    else if (interval != nullptr)
        return interval->toString();
    else
        return nullptr;
}
std::string HTMLGenerator::makeLabelLink(const std::shared_ptr<FType> &type)
{
    auto ret = t_type_label_linked;
    replace_all(ret, "NAME", type->getName());
    return ret;
}
void HTMLGenerator::makeSingleLineComment(const std::shared_ptr<BstIdl::FAnnotation> &annotation,
                                          std::string &single_comment, bool isDescription)
{
    if (!annotation)
        return;

    if (!isDescription)
    {
        replace_one(single_comment, "TYPE_LITERAL", annotation->getType().getName());
    }
    replace_one(single_comment, "COMMENT", annotation->getComment());
    replace_all(single_comment, "\n", "<br/>");
}
} // namespace BstIdl