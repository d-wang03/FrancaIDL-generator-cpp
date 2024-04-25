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
#include "ipc-gen/ipcClientIncludeGen.h"
#include "utilities/string_utility.h"

namespace BstIdl
{
void IPCClientIncludeGenerator::generateClientInclude(const std::shared_ptr<FDInterface> &fdInterface,
                                                      const std::string &dir)
{
    auto fInterface = fdInterface->getTarget();
    auto include_path = IPCCommonGenerator::getInstance().getClientIncludePath(fInterface);
    auto s_path = mk_dir(dir, include_path);
    if (s_path.empty())
        return;
    auto pos = include_path.find_last_of("/") + 1;
    auto file_name = include_path.substr(pos, include_path.size() - pos);
    std::ofstream OsWriteS(s_path + "/" + file_name, std::ofstream::out);
    setPrefix(IPCCommonGenerator::getInstance().getPrefix());
    auto source = generator(fdInterface);
    OsWriteS << source;
    OsWriteS.close();
}

std::string IPCClientIncludeGenerator::generator(const std::shared_ptr<FDInterface> &fdInterface)
{
    auto &common_gen = IPCCommonGenerator::getInstance();
    std::string ret = getLicense();
    ret += common_gen.c_include_file;
    ret += common_gen.c_ipc_init_part;
    ret += common_gen.generateFTypes(fdInterface);
    ret += common_gen.generateDeclareFAttribute(fdInterface);
    ret += generateErrorEnum(fdInterface);
    ret += generateMethodsInterface(fdInterface);
    ret += generateBroadcastsInterface(fdInterface);
    return ret;
}

std::string IPCClientIncludeGenerator::generateErrorEnum(const std::shared_ptr<FDInterface> &fdInterface)
{
    std::string error_enum_string = "";
    error_enum_string += c_common_error;
    auto interface = fdInterface->getTarget();
    auto methods = interface->getMethods();
    auto broadcasts = interface->getBroadcasts();
    for (auto it : methods)
    {
        if (it->getInArgs().empty())
        {
            std::cerr << "method:" << it->getName() << " don't not have in argument!\n";
            return error_enum_string;
        }
        if (!it->getOutArgs().empty())
        {
            error_enum_string += generateMethodErrorEnum(it);
        }
        else
        {
            error_enum_string += generateFireAndForgetMethodErrorEnum(it);
        }
    }
    for (auto it : broadcasts)
    {
        if (!it->getOutArgs().empty())
        {
            error_enum_string += generateBroadcastErrorEnum(it);
        }
        else
        {
            std::cerr << "broadcast:" << it->getName() << " don't not have out argument!\n";
            return error_enum_string;
        }
    }
    return error_enum_string;
}

std::string IPCClientIncludeGenerator::generateMethodErrorEnum(const std::shared_ptr<FMethod> &method)
{
    auto name = method->getName();
    auto error_enum = c_method_error;
    error_enum = error_enum.replace(error_enum.find("METHODID_error"), 8, getPrefix() + name);
    error_enum = replace_all(error_enum, "METHODID", toUpper(name));

    auto &common_gen = IPCCommonGenerator::getInstance();
    auto method_error_enum = method->getErrorEnum();
    auto method_error = method->getErrors();
    if (method_error_enum != nullptr)
    {
        std::string separator = ",\n\t";
        auto method_error_enumerators_list = common_gen.findAllEnumerators(method_error_enum.get(), name);
        auto method_error_enumerators_string = join(method_error_enumerators_list, separator);
        error_enum = error_enum.replace(error_enum.find("OTHERS"), 6, method_error_enumerators_string);
    }
    else if (method_error != nullptr)
    {
        std::string separator = ",\n\t\t";
        auto method_error_list = common_gen.findAllEnumerators(method_error.get(), name);
        auto method_error_string = join(method_error_list, separator);
        error_enum = error_enum.replace(error_enum.find("OTHERS"), 6, method_error_string);
    }
    else
    {
        error_enum = error_enum.replace(error_enum.find("OTHERS"), 6, "");
    }
    return error_enum;
}

std::string IPCClientIncludeGenerator::generateAsyncMethodErrorEnum(const std::shared_ptr<FMethod> &method)
{
    auto name = method->getName();
    auto error_enum = c_async_method_error;
    error_enum = error_enum.replace(error_enum.find("METHODID_error"), 8, getPrefix() + "async_" + name);
    error_enum = replace_all(error_enum, "METHODID", toUpper(name));

    auto &common_gen = IPCCommonGenerator::getInstance();
    auto method_error_enum = method->getErrorEnum();
    auto method_error = method->getErrors();
    if (method_error_enum != nullptr)
    {
        std::string separator = ",\n\t";
        auto method_error_enumerators_list = common_gen.findAllEnumerators(method_error_enum.get(), name);
        auto method_error_enumerators_string = join(method_error_enumerators_list, separator);
        error_enum = error_enum.replace(error_enum.find("OTHERS"), 6, method_error_enumerators_string);
    }
    else if (method_error != nullptr)
    {
        std::string separator = ",\n\t";
        auto method_error_list = common_gen.findAllEnumerators(method_error.get(), name);
        for (auto it = method_error_list.begin(); it != method_error_list.end(); it++)
        {
            *it = "ASYNC_" + *it;
        }

        auto method_error_string = join(method_error_list, separator);
        error_enum = error_enum.replace(error_enum.find("OTHERS"), 6, method_error_string);
    }
    else
    {
        error_enum = error_enum.replace(error_enum.find("OTHERS"), 6, "");
    }
    return error_enum;
}

std::string IPCClientIncludeGenerator::generateSyncMethodErrorEnum(const std::shared_ptr<FMethod> &method)
{
    auto name = method->getName();
    auto error_enum = c_sync_method_error;
    error_enum = error_enum.replace(error_enum.find("METHODID_error"), 8, getPrefix() + "sync_" + name);
    error_enum = replace_all(error_enum, "METHODID", toUpper(name));

    auto &common_gen = IPCCommonGenerator::getInstance();
    auto method_error_enum = method->getErrorEnum();
    auto method_error = method->getErrors();
    if (method_error_enum != nullptr)
    {
        std::string separator = ",\n\t";
        auto method_error_enumerators_list = common_gen.findAllEnumerators(method_error_enum.get(), name);
        auto method_error_enumerators_string = join(method_error_enumerators_list, separator);
        error_enum = error_enum.replace(error_enum.find("OTHERS"), 6, method_error_enumerators_string);
    }
    else if (method_error != nullptr)
    {
        std::string separator = ",\n\t";
        auto method_error_list = common_gen.findAllEnumerators(method_error.get(), name);
        for (auto it = method_error_list.begin(); it != method_error_list.end(); it++)
        {
            *it = "SYNC_" + *it;
        }
        auto method_error_string = join(method_error_list, separator);
        error_enum = error_enum.replace(error_enum.find("OTHERS"), 6, method_error_string);
    }
    else
    {
        error_enum = error_enum.replace(error_enum.find("OTHERS"), 6, "");
    }
    return error_enum;
}

std::string IPCClientIncludeGenerator::generateFireAndForgetMethodErrorEnum(const std::shared_ptr<FMethod> &method)
{
    auto name = method->getName();
    auto error_enum = c_fire_and_forget_error;
    error_enum = error_enum.replace(error_enum.find("METHODID_error"), 8, getPrefix() + name);
    error_enum = replace_all(error_enum, "METHODID", toUpper(name));

    auto &common_gen = IPCCommonGenerator::getInstance();
    auto method_error_enum = method->getErrorEnum();
    auto method_error = method->getErrors();
    if (method_error_enum != nullptr)
    {
        std::string separator = ",\n\t";
        auto method_error_enumerators_list = common_gen.findAllEnumerators(method_error_enum.get(), name);
        auto method_error_enumerators_string = join(method_error_enumerators_list, separator);
        error_enum = error_enum.replace(error_enum.find("OTHERS"), 6, method_error_enumerators_string);
    }
    else if (method_error != nullptr)
    {
        std::string separator = ",\n\t";
        auto method_error_list = common_gen.findAllEnumerators(method_error.get(), name);
        auto method_error_string = join(method_error_list, separator);
        error_enum = error_enum.replace(error_enum.find("OTHERS"), 6, method_error_string);
    }
    else
    {
        error_enum = error_enum.replace(error_enum.find("OTHERS"), 6, "");
    }
    return error_enum;
}

std::string IPCClientIncludeGenerator::generateBroadcastErrorEnum(const std::shared_ptr<FBroadcast> &broadcast)
{
    auto name = broadcast->getName();
    auto error_enum = c_broadcast_error;
    error_enum = error_enum.replace(error_enum.find("BROADCASTID_error"), 11, getPrefix() + name);
    error_enum = replace_all(error_enum, "BROADCASTID", toUpper(name));
    return error_enum;
}

std::string IPCClientIncludeGenerator::generateMethodsInterface(const std::shared_ptr<FDInterface> &fdInterface)
{
    std::string method_string = "";
    auto fdModel = std::dynamic_pointer_cast<FDModel>(fdInterface->getContainer());
    auto mapper = fdModel->getMapper(fdInterface);
    auto interface = fdInterface->getTarget();
    auto methods = interface->getMethods();
    for (auto it : methods)
    {
        auto fdmethod = std::dynamic_pointer_cast<FDMethod>(mapper->getFDElement(it));
        if (fdmethod != nullptr)
        {
            for (auto property : fdmethod->getProperties())
            {
                if (property->getName() == "ID")
                {
                    auto fdvalue = property->getValue();
                    if (!fdvalue->isSingle())
                    {
                        std::cerr << "method:" << it->getName() << " didn't have method number\n";
                        return method_string;
                    }
                    auto value = std::dynamic_pointer_cast<FDInteger>(fdvalue->getSingle());
                    if (value == nullptr)
                    {
                        std::cerr << "method:" << it->getName() << " didn't have method number\n";
                        return method_string;
                    }
                    method_string += generateMethodInterface(it);
                    break;
                }
            }
        }
        else
        {
            std::cerr << "method:" << it->getName() << " didn't connect to fdmethod\n";
            return method_string;
        }
    }
    method_string += c_async_process_declaration_block + c_callback_process_declaration;
    return method_string;
}

std::string IPCClientIncludeGenerator::generateMethodInterface(const std::shared_ptr<FMethod> &method)
{
    std::string method_inteface = "";
    auto in_args = method->getInArgs();
    auto out_args = method->getOutArgs();
    auto method_name = method->getName();
    if (in_args.empty())
    {
        std::cerr << "method " << method_name << " didn't have input argument!\n";
        return method_inteface;
    }
    auto &common_gen = IPCCommonGenerator::getInstance();
    auto in_args_string = join(common_gen.getArgsList(in_args), ",");
    if (!out_args.empty())
    {
        std::string error_name = getPrefix() + method_name + "_error";
        std::list<std::string> out_args_list;
        common_gen.getMethodOutArgsStrList(out_args, out_args_list);
        auto out_args_string = join(out_args_list, ",");

        std::string tmp_string;
        tmp_string = c_callback_pointer_type_define;
        if (method->getErrorEnum() != nullptr || method->getErrors() != nullptr)
            tmp_string = tmp_string.replace(tmp_string.find("void"), 4, "enum " + error_name);
        tmp_string = tmp_string.replace(tmp_string.find("METHODID"), 8, getPrefix() + method_name);
        tmp_string = tmp_string.replace(tmp_string.find("ARGS"), 4, out_args_string);
        method_inteface += tmp_string;
        tmp_string = c_async_method_register_declaration;
        tmp_string = tmp_string.replace(tmp_string.find("ERROR_NAME"), 10, error_name);
        tmp_string = tmp_string.replace(tmp_string.find("METHODID_Async_Register"), 8, getPrefix() + method_name);
        tmp_string = tmp_string.replace(tmp_string.find("MethodID_ASyncCallback_t"), 8, getPrefix() + method_name);
        tmp_string = tmp_string.replace(tmp_string.find("ARGS"), 4, in_args_string);
        method_inteface += tmp_string;
        tmp_string = c_callback_pointer;
        method_inteface += replace_all(tmp_string, "ID", getPrefix() + method_name);
        tmp_string = c_async_method_declaration;
        tmp_string = tmp_string.replace(tmp_string.find("ERROR_NAME"), 10, error_name);
        tmp_string = replace_all(tmp_string, "METHODID", getPrefix() + method_name);
        method_inteface += tmp_string;
        tmp_string = c_sync_method_declaration;
        tmp_string = tmp_string.replace(tmp_string.find("ERROR_NAME"), 10, error_name);
        tmp_string = tmp_string.replace(tmp_string.find("METHODID"), 8, getPrefix() + method_name);
        tmp_string = tmp_string.replace(tmp_string.find("ARGS"), 4, in_args_string + "," + out_args_string);
        method_inteface += tmp_string;
    }
    else
    {
        std::string tmp_string = c_fire_and_forget_declaration;
        std::string error_name = getPrefix() + method_name + "_error";
        tmp_string = tmp_string.replace(tmp_string.find("ERROR_NAME"), 10, error_name);
        tmp_string = tmp_string.replace(tmp_string.find("METHODID"), 8, getPrefix() + method_name);
        tmp_string = tmp_string.replace(tmp_string.find("ARGS"), 4, in_args_string);
        method_inteface += tmp_string;
    }
    return method_inteface;
}

std::string IPCClientIncludeGenerator::generateBroadcastsInterface(const std::shared_ptr<FDInterface> &fdInterface)
{
    std::string broadcast_string = "";
    auto fdModel = std::dynamic_pointer_cast<FDModel>(fdInterface->getContainer());
    auto mapper = fdModel->getMapper(fdInterface);
    auto interface = fdInterface->getTarget();
    auto broadcasts = interface->getBroadcasts();
    for (auto it : broadcasts)
    {
        auto fdbroadcast = std::dynamic_pointer_cast<FDBroadcast>(mapper->getFDElement(it));
        if (fdbroadcast != nullptr)
        {
            for (auto property : fdbroadcast->getProperties())
            {
                if (property->getName() == "ID")
                {
                    auto fdvalue = property->getValue();
                    if (!fdvalue->isSingle())
                    {
                        std::cerr << "broadcast:" << it->getName() << " didn't have broadcast number\n";
                        return broadcast_string;
                    }
                    auto value = std::dynamic_pointer_cast<FDInteger>(fdvalue->getSingle());
                    if (value == nullptr)
                    {
                        std::cerr << "broadcast:" << it->getName() << " didn't have broadcast number\n";
                        return broadcast_string;
                    }
                    broadcast_string += generateBroadcastInterface(it);
                    break;
                }
            }
        }
        else
        {
            std::cerr << "broadcast:" << it->getName() << " didn't connect to fdbroadcast\n";
            return broadcast_string;
        }
    }
    broadcast_string += c_reply_broadcast_process_declaration;
    return broadcast_string;
}
std::string IPCClientIncludeGenerator::generateBroadcastInterface(const std::shared_ptr<FBroadcast> &broadcast)
{
    std::string broadcast_inteface = "";
    auto out_args = broadcast->getOutArgs();
    auto broadcast_name = broadcast->getName();
    if (out_args.empty())
    {
        std::cerr << "broadcast " << broadcast_name << " didn't have output argument!\n";
        return broadcast_inteface;
    }

    std::list<std::string> out_args_list;
    IPCCommonGenerator::getInstance().getClientBroadcastArgsStrList(out_args, out_args_list);
    auto out_args_string = join(out_args_list, ",");
    std::string error_name = getPrefix() + broadcast_name + "_error";
    // c_callback_pointer_type_define & c_callback_pointer
    std::string tmp_string;
    tmp_string = c_callback_pointer_type_define;
    tmp_string = tmp_string.replace(tmp_string.find("METHODID"), 8, getPrefix() + broadcast_name);
    tmp_string = tmp_string.replace(tmp_string.find("ARGS"), 4, out_args_string);
    broadcast_inteface += tmp_string;
    tmp_string = c_callback_pointer;
    broadcast_inteface += replace_all(tmp_string, "ID", getPrefix() + broadcast_name);
    // c_broadcast_subscribe_declaration
    tmp_string = c_broadcast_subscribe_declaration;
    tmp_string = tmp_string.replace(tmp_string.find("ERROR_NAME"), 10, error_name);
    tmp_string = replace_all(tmp_string, "%ID", getPrefix() + broadcast_name);
    broadcast_inteface += tmp_string;
    return broadcast_inteface;
}
} // namespace BstIdl