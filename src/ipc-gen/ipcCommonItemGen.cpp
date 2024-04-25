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
#include "ipc-gen/ipcCommonItemGen.h"
namespace BstIdl
{
const std::string BstIdl::IPCCommonGenerator::c_include_file = R"(
#ifdef __aarch64__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define IPC_PRINT printf
#else
//consider a embedded firmware runtime
//TODO: user should add types header, like stdint.h, and add define print macro.
//#include <stdint.h>
//#define IPC_PRINT print_xxx
#endif

#include "ipc.h"
    )";
const std::string BstIdl::IPCCommonGenerator::c_r5_include_file = R"(
#include <stdio.h>
#include <string.h>
#include "R5ipc.h"
    )";
const std::string BstIdl::IPCCommonGenerator::c_annotation_block = R"(
/************
   ANNOTATION
 ***********/
    )";

// Composite data type
const std::string BstIdl::IPCCommonGenerator::c_arrayType = "typedef TYPEB * TYPEA\n";
const std::string BstIdl::IPCCommonGenerator::c_enumType = R"(
enum NAME
{
  ENUMRATORS
};
)";
const std::string BstIdl::IPCCommonGenerator::c_StructType = R"(
struct NAME
{
  ENUMRATORS
};
)";
const std::string BstIdl::IPCCommonGenerator::c_UnionType = R"(
union NAME
{
  ENUMRATORS
};
)";
const std::string BstIdl::IPCCommonGenerator::c_typedef = "typedef TYPE NAME\n";
const std::string BstIdl::IPCCommonGenerator::c_attribute = "TYPE NAME;\n";

/***method***/
// ipc_init ipc_deinit declaration
const std::string BstIdl::IPCCommonGenerator::c_ipc_init_part = R"(
int32_t ipc_init();
int32_t ipc_deinit();
extern int32_t session_id;
)";
const std::string BstIdl::IPCCommonGenerator::c_r5_ipc_init_part = R"(
sint32 ipc_init();
sint32 ipc_deinit();
sint32 session_id;
)";
void IPCCommonGenerator::setClientAndServerID(const int &client_id, const int &server_id)
{
    client_number = client_id;
    server_number = server_id;
}

std::pair<int, int> IPCCommonGenerator::getCurrClientAndServerID() const
{
    return std::make_pair(client_number, server_number);
}

std::string IPCCommonGenerator::getTypeTransName()
{
    return type_transName;
}

void IPCCommonGenerator::setTypeTransName(bool is_server)
{
    if (server_number == 8 && is_server)
        type_transName = std::string("r5_get_name");
    else
        type_transName = std::string("get_name");
}

bool IPCCommonGenerator::isR5Server() const
{
    if (server_number == 8)
        return true;
    else
        return false;
}

std::string IPCCommonGenerator::getElementName(const std::shared_ptr<BstIdl::FModelElement> &fModelElement)
{
    if ((fModelElement->getName().empty() || fModelElement->getName() == "") &&
        std::dynamic_pointer_cast<BstIdl::FTypeCollection>(fModelElement) != nullptr)
    {
        return "__Anonymous__";
    }
    else
    {
        return fModelElement->getName();
    }
}
// get client & server file Path
std::string IPCCommonGenerator::getClientIncludeFile(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection)
{
    return getElementName(fTypeCollection) + "Client.h";
}

std::string IPCCommonGenerator::getClientIncludePath(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection)
{
    return getVersionPathPrefix(fTypeCollection) +
           getDirectoryPath(std::dynamic_pointer_cast<BstIdl::FModel>((fTypeCollection->getContainer()))) + "/" +
           getClientIncludeFile(fTypeCollection);
}

std::string IPCCommonGenerator::getClientSourceFile(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection)
{
    return getElementName(fTypeCollection) + "Client.c";
}

std::string IPCCommonGenerator::getClientSourcePath(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection)
{
    return getVersionPathPrefix(fTypeCollection) +
           getDirectoryPath(std::dynamic_pointer_cast<BstIdl::FModel>((fTypeCollection->getContainer()))) + "/" +
           getClientSourceFile(fTypeCollection);
}
std::string IPCCommonGenerator::getServerIncludeFile(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection)
{
    return getElementName(fTypeCollection) + "Server.h";
}

std::string IPCCommonGenerator::getServerIncludePath(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection)
{
    return getVersionPathPrefix(fTypeCollection) +
           getDirectoryPath(std::dynamic_pointer_cast<BstIdl::FModel>((fTypeCollection->getContainer()))) + "/" +
           getServerIncludeFile(fTypeCollection);
}

std::string IPCCommonGenerator::getServerSourceFile(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection)
{
    return getElementName(fTypeCollection) + "Server.c";
}

std::string IPCCommonGenerator::getServerSourcePath(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection)
{
    return getVersionPathPrefix(fTypeCollection) +
           getDirectoryPath(std::dynamic_pointer_cast<BstIdl::FModel>((fTypeCollection->getContainer()))) + "/" +
           getServerSourceFile(fTypeCollection);
}

std::string IPCCommonGenerator::getDirectoryPath(const std::shared_ptr<BstIdl::FModel> &fModel)
{
    auto name = fModel->getName();
    while (name.find(".") != name.npos)
        name = name.replace(name.find("."), 1, "/");
    return name;
}

std::string IPCCommonGenerator::getVersionPathPrefix(const std::shared_ptr<BstIdl::FTypeCollection> &_tc)
{
    std::string prefix = "";
    auto itsVersion = _tc->getVersion();
    if (itsVersion != nullptr && (itsVersion->getMajor() != 0 || itsVersion->getMinor() != 0))
    {
        prefix = "v" + std::to_string(itsVersion->getMajor()) + "/";
    }
    return prefix;
}

void IPCCommonGenerator::generatePrefix(const std::shared_ptr<FDInterface> &fdInterface)
{
    prefix = "";
    auto interface = fdInterface->getTarget();
    auto fdModel = std::dynamic_pointer_cast<FDModel>(fdInterface->getContainer());
    if (fdInterface != nullptr)
    {
        bool prefix_flag = false;
        for (auto property : fdInterface->getProperties())
        {
            if (property->getName() == "scopePrefix")
            {
                if (prefix_flag)
                {
                    std::cerr << "Interface:" << fdInterface->getName() << "'s scopePrefix have already defined!\n";
                    return;
                }
                auto fdvalue = property->getValue();
                if (!fdvalue->isSingle())
                {
                    std::cerr << "Interface:" << fdInterface->getName() << " have wrong scopePrefix format!\n";
                    return;
                }
                auto boolean = std::dynamic_pointer_cast<FDBoolean>(fdvalue->getSingle());
                if (boolean == nullptr)
                {
                    std::cerr << "Interface:" << fdInterface->getName() << " have wrong scopePrefix format!\n";
                    return;
                }
                auto fd_prefix_value = boolean->getValue();
                if (fd_prefix_value.empty())
                {
                    std::cerr << "interface:" << interface->getName() << "prefix is null\n";
                }
                if (fd_prefix_value == "true")
                {
                    auto file_name = std::dynamic_pointer_cast<FModel>(interface->getContainer())->getName();
                    prefix = replace_all(file_name, ".", "_") + "_v" +
                             std::to_string(interface->getVersion()->getMajor()) + "_";
                }
            }
        }
    }
    else
    {
        std::cerr << "fdInterface is nullptr\n";
    }
}
std::string IPCCommonGenerator::transformFBasicType(FBasicTypeId *type)
{
    auto value = type->getValue();
    switch (value)
    {
    case FBasicTypeId::UNDEFINED:
        return "";
        break;
    case FBasicTypeId::INT8:
        return c_Int8;
        break;
    case FBasicTypeId::UINT8:
        return c_UInt8;
        break;
    case FBasicTypeId::INT16:
        return c_Int16;
        break;
    case FBasicTypeId::UINT16:
        return c_UInt16;
        break;
    case FBasicTypeId::INT32:
        return c_Int32;
        break;
    case FBasicTypeId::UINT32:
        return c_UInt32;
        break;
    case FBasicTypeId::INT64:
        return c_Int64;
        break;
    case FBasicTypeId::UINT64:
        return c_UInt64;
        break;
    case FBasicTypeId::BOOLEAN:
        return c_Boolean;
        break;
    case FBasicTypeId::STRING:
        return c_String;
        break;
    case FBasicTypeId::FLOAT:
        return c_Float;
        break;
    case FBasicTypeId::DOUBLE:
        return c_Double;
        break;
    case FBasicTypeId::BYTE_BUFFER:
        return c_ByteBuffer;
        break;
    default:
        std::cerr << "Don't have such basic type:" << value << std::endl;
        return "";
        break;
    }
}

std::string IPCCommonGenerator::transformFBasicTypeForR5(FBasicTypeId *type)
{
    auto value = type->getValue();
    switch (value)
    {
    case FBasicTypeId::UNDEFINED:
        return "";
        break;
    case FBasicTypeId::INT8:
        return r5_c_Int8;
        break;
    case FBasicTypeId::UINT8:
        return r5_c_UInt8;
        break;
    case FBasicTypeId::INT16:
        return r5_c_Int16;
        break;
    case FBasicTypeId::UINT16:
        return r5_c_UInt16;
        break;
    case FBasicTypeId::INT32:
        return r5_c_Int32;
        break;
    case FBasicTypeId::UINT32:
        return r5_c_UInt32;
        break;
    case FBasicTypeId::INT64:
        return r5_c_Int64;
        break;
    case FBasicTypeId::UINT64:
        return r5_c_UInt64;
        break;
    case FBasicTypeId::BOOLEAN:
        return r5_c_Boolean;
        break;
    case FBasicTypeId::STRING:
        return r5_c_String;
        break;
    case FBasicTypeId::FLOAT:
        return r5_c_Float;
        break;
    case FBasicTypeId::DOUBLE:
        return r5_c_Double;
        break;
    case FBasicTypeId::BYTE_BUFFER:
        return r5_c_ByteBuffer;
        break;
    default:
        std::cerr << "Don't have such basic type:" << value << std::endl;
        return "";
        break;
    }
}

std::string IPCCommonGenerator::transformFArrayType(FArrayType *type)
{
    auto array_name = prefix + type->getName();
    auto type_name = prefix + type->getElementType()->transform(type_transName);
    auto ret = c_arrayType;
    ret = ret.replace(ret.find("TYPEB"), 5, type_name);
    ret = ret.replace(ret.find("TYPEA"), 5, array_name);
    return ret;
}
std::string IPCCommonGenerator::transformFEnumerationType(FEnumerationType *type)
{
    auto enumration_name = prefix + type->getName();
    auto enumerator_name = findAllEnumerators(type);
    std::string separator = ",\n\t";
    auto enumerators = join(enumerator_name, separator);
    auto ret = c_enumType;
    ret = ret.replace(ret.find("NAME"), 4, enumration_name);
    ret = ret.replace(ret.find("ENUMRATORS"), 10, enumerators);
    return ret;
}
std::list<std::string> IPCCommonGenerator::findAllEnumerators(FEnumerationType *type, std::string method_name)
{
    std::list<std::string> ret;
    auto son = type->getEnumerators();
    auto base = type->getBase();
    auto method_prefix = toUpper(method_name) + "_";
    if (base)
    {
        auto father = findAllEnumerators(base.get());
        for (auto it : father)
        {
            ret.push_back(method_prefix + it);
        }
    }
    for (auto it : son)
    {
        ret.push_back(method_prefix + it->toString());
    }
    return ret;
}
std::list<std::string> IPCCommonGenerator::findAllEnumerators(FEnumerationType *type)
{
    std::list<std::string> ret;
    auto son = type->getEnumerators();
    auto base = type->getBase();
    if (base)
    {
        auto father = findAllEnumerators(base.get());
        for (auto it : father)
        {
            ret.push_back(it);
        }
    }
    for (auto it : son)
    {
        ret.push_back(it->toString());
    }
    return ret;
}
std::string IPCCommonGenerator::transformFStructType(FStructType *type)
{
    auto ret = c_StructType;
    auto name = prefix + type->getName();
    auto elements = findAllStructElement(type);
    std::string separator = "\n\t";
    auto elements_segment = join(elements, separator);
    ret = ret.replace(ret.find("NAME"), 4, name);
    ret = ret.replace(ret.find("ENUMRATORS"), 10, elements_segment);
    return ret;
}
std::list<std::string> IPCCommonGenerator::findAllStructElement(FStructType *type)
{
    std::list<std::string> ret;
    auto son = type->getElements();
    auto base = type->getBase();
    if (base)
    {
        auto father = findAllStructElement(type);
        for (auto it : father)
        {
            ret.push_back(it);
        }
    }
    for (auto it : son)
    {
        ret.push_back(it->transform("transform_type"));
    }
    return ret;
}
std::string IPCCommonGenerator::transformFField(FField *field)
{
    std::string ret;
    auto type_name = field->getType()->transform(type_transName);
    auto name = field->getName();
    if (field->isArray())
    {
        ret = type_name + " *" + name + ";";
    }
    else
    {
        ret = type_name + " " + name + ";";
    }
    return ret;
}

std::string IPCCommonGenerator::transformFUnionType(FUnionType *type)
{
    auto ret = c_UnionType;
    auto name = prefix + type->getName();
    auto elements = findAllUnionElement(type);
    std::string separator = "\n\t";
    auto elements_segment = join(elements, separator);
    ret = c_enumType;
    ret = ret.replace(ret.find("NAME"), 4, name);
    ret = ret.replace(ret.find("ENUMRATORS"), 10, elements_segment);
    return ret;
}

std::list<std::string> IPCCommonGenerator::findAllUnionElement(FUnionType *type)
{
    std::list<std::string> ret;
    auto son = type->getElements();
    auto base = type->getBase();
    if (base)
    {
        auto father = findAllUnionElement(type);
        for (auto it : father)
        {
            ret.push_back(it);
        }
    }
    for (auto it : son)
    {
        ret.push_back(it->transform("transform_type"));
    }
    return ret;
}
std::string IPCCommonGenerator::transformFTypeDef(FTypeDef *type)
{
    auto type_name = type->getActualType()->transform(type_transName);
    auto name = prefix + type->getName();
    auto ret = c_typedef;
    ret = ret.replace(ret.find("NAME"), 4, name);
    ret = ret.replace(ret.find("TYPE"), 4, type_name);
    return ret;
}
std::string IPCCommonGenerator::transformDeclareFAttribute(FAttribute *attribute)
{
    std::string ret;
    auto type_name = attribute->getType()->transform(type_transName);
    auto name = prefix + attribute->getName();
    // r5 without extern
    std::string decl_prefix("");
    if (type_transName == "get_name")
        decl_prefix = "extern ";
    if (attribute->isArray())
    {
        ret = decl_prefix + type_name + " *" + name + ";\n";
    }
    else
    {
        ret = decl_prefix + type_name + " " + name + ";\n";
    }
    return ret;
}
std::string IPCCommonGenerator::transformDefineFAttribute(FAttribute *attribute)
{
    std::string ret;
    auto type = attribute->getType();
    if (!type)
    {
        std::cerr << "type is null!\n";
        return "";
    }
    auto type_name = type->transform(type_transName);
    auto name = prefix + attribute->getName();
    if (attribute->isArray())
    {
        ret = type_name + " *" + name + "=" + assignAttribute(type) + ";\n";
    }
    else
    {
        ret = type_name + " " + name + "=" + assignAttribute(type) + ";\n";
    }
    return ret;
}

std::string IPCCommonGenerator::assignAttribute(const std::shared_ptr<FTypeRef> type)
{
    auto predefined_type = type->getPredefined();
    auto derived_type = type->getDerived();
    if (predefined_type)
    {
        auto value = predefined_type->getValue();
        if (FBasicTypeId::isBoolean(value))
            return "false";
        else if (FBasicTypeId::isInteger(value))
        {
            return "0";
        }
        else if (FBasicTypeId::isReal(value))
        {
            return "0.0";
        }
        else if (FBasicTypeId::isString(value))
        {
            return "\" \"";
        }
        else
        {
            std::cerr << "Wrong FBasicType!\nCorrect type can only be:\nInt8 UInt8 Int16 UInt16 Int32 UInt32 Int64 "
                         "UInt64 Boolean String Float Double\n";
            return "";
        }
    }
    else if (derived_type)
    {
        auto array_type = std::dynamic_pointer_cast<FArrayType>(derived_type);
        auto compound_type = std::dynamic_pointer_cast<FCompoundType>(derived_type);
        auto enumration_type = std::dynamic_pointer_cast<FEnumerationType>(derived_type);
        auto type_def = std::dynamic_pointer_cast<FTypeDef>(derived_type);
        if (array_type || compound_type || enumration_type)
        {
            return "{0}";
        }
        else if (type_def)
        {
            return assignAttribute(type_def->getActualType());
        }
        else
        {
            std::cerr << "Wrong FDerivedType!\nCorrect type can only be:\nFTypeDef FArrayType FStructType FUnionType "
                         "FEnumrationType\n";
            return "";
        }
    }
    else
    {
        std::cerr << "Wrong Type!\n";
        return "";
    }
}
std::string IPCCommonGenerator::getFTypeDefName(FTypeDef *type)
{
    return prefix + type->getName();
}
std::string IPCCommonGenerator::getFArrayTypeName(FArrayType *type)
{
    return prefix + type->getName();
}
std::string IPCCommonGenerator::getFStructTypeName(FStructType *type)
{
    return "struct " + prefix + type->getName();
}
std::string IPCCommonGenerator::getFUnionTypeName(FUnionType *type)
{
    return "union " + prefix + type->getName();
}
std::string IPCCommonGenerator::getFEnumrationTypeName(FEnumerationType *type)
{
    return "enum " + prefix + type->getName();
}
std::string IPCCommonGenerator::generateFTypes(const std::shared_ptr<FDInterface> &fdInterface)
{
    std::string type_string = "";
    auto interface = fdInterface->getTarget();
    auto types = interface->getTypes();
    for (auto it : types)
    {
        type_string += it->transform("transform_type");
    }
    return type_string;
}
std::string IPCCommonGenerator::generateDeclareFAttribute(const std::shared_ptr<FDInterface> &fdInterface)
{
    std::string attribute_string = "";
    auto interface = fdInterface->getTarget();
    auto attributes = interface->getAttributes();
    for (auto it : attributes)
    {
        attribute_string += it->transform("transform_declare_attribute");
    }
    return attribute_string;
}
std::string IPCCommonGenerator::generateDefineFAttribute(const std::shared_ptr<FDInterface> &fdInterface)
{
    std::string attribute_string = "";
    auto interface = fdInterface->getTarget();
    auto attributes = interface->getAttributes();
    for (auto it : attributes)
    {
        attribute_string += it->transform("transform_define_attribute");
    }
    return attribute_string;
}
std::string IPCCommonGenerator::generateIncludeFile(FModel *model)
{
    std::string ret = "";
    auto &mgr = FModelManager::getInstance();
    for (auto it : model->getImports())
    {
        auto import_name = it->getImportedNamespace();
        size_t length = import_name.size();
        if (import_name.at(length - 2) == '.' && import_name.at(length - 1) == '*')
        {
            auto model = mgr.findModel(import_name.substr(0, length - 1));
            if (model)
            {
                auto model_name = model->getName();
                model_name = replace_all(model_name, ".", "_");
                auto type_collections = model->getTypeCollections();
                if (type_collections.empty())
                {
                    std::cerr << "model :" << model_name << "type collection list is empty!\n";
                }
                for (auto it : type_collections)
                {
                    ret += "#include \"" + model_name + "_" + it->getName() + ".h\"\n";
                }
            }
            else
            {
                std::cerr << "can not find import model :" << import_name.substr(0, length - 1) << "\n";
                return ret;
            }
        }
        else
        {
            auto model = mgr.findModel(import_name);
            auto type_colletion = mgr.findModel(import_name);
            if (model)
            {
                if (model)
                {
                    auto model_name = model->getName();
                    model_name = replace_all(model_name, ".", "_");
                    auto type_collections = model->getTypeCollections();
                    if (type_collections.empty())
                    {
                        std::cerr << "model :" << model_name << "type collection list is empty!\n";
                    }
                    for (auto it : type_collections)
                    {
                        ret += "#include \"" + model_name + "_" + it->getName() + ".h\"\n";
                    }
                }
                else
                {
                    std::cerr << "can not find import model :" << import_name << "\n";
                    return ret;
                }
            }
            else if (type_colletion)
            {
                ret += "#include \"" + replace_all(import_name, ".", "_") + ".h\"\n";
            }
            else
            {
                std::cerr << "can not find import file :" << import_name << "\n";
                return ret;
            }
        }
    }
    return ret;
}
void IPCCommonGenerator::getServerBroadcastArgsStrList(const std::list<std::shared_ptr<BstIdl::FArgument>> &out_args,
                                                       std::list<std::string> &out_args_list) const
{
    for (const auto &it : out_args)
    {
        auto basic_type = it->getType()->getPredefined();
        auto out_arg_string = it->getType()->transform(type_transName);
        if (it->isArray())
        {
            out_arg_string += "*";
        }
        if (basic_type != nullptr && basic_type->getName() == "ByteBuffer")
        {
            out_arg_string.replace(out_arg_string.find("NAME"), 4, it->getName());
            out_arg_string.replace(out_arg_string.find("SIZE"), 4, it->getName() + "_size");
        }
        else
            out_arg_string += " " + it->getName();
        out_args_list.push_back(out_arg_string);
    }
}
void IPCCommonGenerator::getClientBroadcastArgsStrList(const std::list<std::shared_ptr<BstIdl::FArgument>> &out_args,
                                                       std::list<std::string> &out_args_list) const
{
    getMethodOutArgsStrList(out_args, out_args_list);
}
void IPCCommonGenerator::getMethodOutArgsStrList(const std::list<std::shared_ptr<BstIdl::FArgument>> &out_args,
                                                 std::list<std::string> &out_args_list) const
{
    for (const auto &it : out_args)
    {
        auto basic_type = it->getType()->getPredefined();
        auto out_arg_string = it->getType()->transform(type_transName);
        if (it->isArray())
        {
            out_arg_string += "*";
        }
        if (basic_type != nullptr && basic_type->getName() == "ByteBuffer")
        {
            out_arg_string.replace(out_arg_string.find("NAME"), 4, it->getName());
            out_arg_string.replace(out_arg_string.find("SIZE"), 4, it->getName() + "_size");
        }
        else
            out_arg_string += "* " + it->getName();
        out_args_list.push_back(out_arg_string);
    }
}
std::list<std::string> IPCCommonGenerator::getArgsList(const std::list<std::shared_ptr<BstIdl::FArgument>> &args) const
{
    std::list<std::string> args_list;
    for (const auto &it : args)
    {
        auto basic_type = it->getType()->getPredefined();
        std::string arg_string;
        if (basic_type != nullptr && basic_type->getName() == "ByteBuffer")
        {
            arg_string = it->getType()->transform(type_transName);
        }
        else
            arg_string = "const " + it->getType()->transform(type_transName);
        if (it->isArray())
        {
            arg_string += "*";
        }

        if (basic_type != nullptr && basic_type->getName() == "ByteBuffer")
        {
            arg_string.replace(arg_string.find("NAME"), 4, it->getName());
            arg_string.replace(arg_string.find("SIZE"), 4, it->getName() + "_size");
            arg_string.replace(arg_string.find(","), 1, ",const ");
        }
        else
            arg_string += " " + it->getName();
        args_list.push_back(arg_string);
    }
    return args_list;
}

void IPCCommonGenerator::getAllServerArgsLists(const std::list<std::shared_ptr<BstIdl::FArgument>> &args,
                                               const std::string &method_name, const bool &is_inArgs,
                                               std::list<std::string> &args_list,
                                               std::list<std::string> &args_define_list,
                                               std::list<std::string> &args_ref_list) const
{
    std::string in_arg_prefix("");
    std::string out_arg_prefix("");
    std::string ref_prefix("");
    if (is_inArgs)
    {
        in_arg_prefix = "const ";
        out_arg_prefix = "* ";
    }
    else
    {
        out_arg_prefix = " ";
        ref_prefix = "&";
    }

    for (const auto &it : args)
    {
        auto basic_type = it->getType()->getPredefined();
        auto arg_string = in_arg_prefix + it->getType()->transform(type_transName);
        auto arg_define_string = it->getType()->transform(type_transName);
        // args list
        if (it->isArray())
        {
            arg_string += "*";
            if (is_inArgs)
                arg_define_string += "*";
        }
        if (basic_type != nullptr && basic_type->getName() == "ByteBuffer")
        {
            arg_string.replace(arg_string.find("NAME"), 4, it->getName());
            arg_string.replace(arg_string.find("SIZE"), 4, it->getName() + "_size");
        }
        else
            arg_string += out_arg_prefix + it->getName();
        args_list.push_back(arg_string);
        // args define
        if (basic_type != nullptr && basic_type->getName() == "ByteBuffer")
        {
            arg_define_string.replace(arg_define_string.find("NAME"), 4, method_name + "_" + it->getName());
            arg_define_string.replace(arg_define_string.find("SIZE"), 4, method_name + "_" + it->getName() + "_size");
            auto malloc_size = "=(char *)malloc(" + method_name + "_" + it->getName() + "_size)";
            std::cout << arg_define_string << std::endl;
            arg_define_string.replace(arg_define_string.find(","), 1, "=100;\n\t\t\t");
            arg_define_string += malloc_size;
        }
        else
            arg_define_string += " " + method_name + "_" + it->getName();
        args_define_list.push_back(arg_define_string);
        // args ref
        if (basic_type != nullptr && basic_type->getName() == "ByteBuffer")
        {
            args_ref_list.push_back(method_name + "_" + it->getName() + "_size," + method_name + "_" + it->getName());
        }
        else
            args_ref_list.push_back(ref_prefix + method_name + "_" + it->getName());
    }
}
void IPCCommonGenerator::getAllClientOutArgsLists(const std::list<std::shared_ptr<BstIdl::FArgument>> &args,
                                                  std::list<std::string> &args_list,
                                                  std::list<std::string> &args_define_list,
                                                  std::list<std::string> &args_ref_list) const
{
    for (const auto &it : args)
    {
        // arg str
        auto arg_string = it->getType()->transform(type_transName);
        if (it->isArray())
        {
            arg_string += "*";
        }
        auto basic_type = it->getType()->getPredefined();
        if (basic_type != nullptr && basic_type->getName() == "ByteBuffer")
        {
            arg_string.replace(arg_string.find("NAME"), 4, it->getName());
            arg_string.replace(arg_string.find("SIZE"), 4, it->getName() + "_size");
        }
        else
            arg_string += "* " + it->getName();
        args_list.push_back(arg_string);
        // arg define
        if (basic_type != nullptr && basic_type->getName() == "ByteBuffer")
        {
            auto malloc_size = "=(char *)malloc(" + it->getName() + "_size)";
            std::cout << arg_string << std::endl;
            arg_string.replace(arg_string.find(","), 1, "=100;\n\t\t");
            arg_string += malloc_size;
        }
        else
        {
            arg_string.replace(arg_string.find("*"), 1, "");
        }
        args_define_list.push_back(arg_string);
        // arg reference
        if (basic_type != nullptr && basic_type->getName() == "ByteBuffer")
        {
            args_ref_list.push_back(it->getName() + "_size," + it->getName());
        }
        else
        {
            args_ref_list.push_back("&" + it->getName());
        }
    }
}
std::string IPCCommonGenerator::getArgsPayloadSize(const std::list<std::shared_ptr<BstIdl::FArgument>> &args,
                                                   std::string buffer_prefix) const
{
    // payload_size
    std::string payload_size = "";
    int flag = 0;
    if (!buffer_prefix.empty())
        buffer_prefix.append("_");

    for (auto it : args)
    {
        auto basic_type = it->getType()->getPredefined();
        if (basic_type != nullptr && basic_type->getName() == "ByteBuffer")
        {
            auto arg_string = buffer_prefix + it->getName() + "_size";
            if (flag == 0)
            {
                flag = 1;
                payload_size += arg_string;
            }
            else
            {
                payload_size += "+" + arg_string;
            }
        }
        else
        {
            auto arg_string = it->getType()->transform(type_transName);
            if (flag == 0)
            {
                flag = 1;
                payload_size += "sizeof(" + arg_string + ")";
            }
            else
            {
                payload_size += "+sizeof(" + arg_string + ")";
            }
        }
    }
    return payload_size;
}

} // namespace BstIdl