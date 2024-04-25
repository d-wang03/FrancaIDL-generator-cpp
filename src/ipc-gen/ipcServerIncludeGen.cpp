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
#include "ipc-gen/ipcServerIncludeGen.h"
#include "utilities/string_utility.h"

namespace BstIdl
{
void IPCServerIncludeGenerator::generateServerInclude(const std::shared_ptr<FDInterface> &fdInterface,
                                                      const std::string &dir)
{
    auto fInterface = fdInterface->getTarget();
    auto include_path = IPCCommonGenerator::getInstance().getServerIncludePath(fInterface);
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

std::string IPCServerIncludeGenerator::generator(const std::shared_ptr<FDInterface> &fdInterface)
{
    auto &common_gen = IPCCommonGenerator::getInstance();
    std::string ret = getLicense();
    if (IPCCommonGenerator::getInstance().isR5Server())
    {
        ret += common_gen.c_r5_include_file;
        ret += common_gen.c_r5_ipc_init_part;
    }
    else
    {
        ret += common_gen.c_include_file;
        ret += common_gen.c_ipc_init_part;
    }
    ret += common_gen.generateFTypes(fdInterface) + common_gen.generateDeclareFAttribute(fdInterface) +
           generateErrorEnum(fdInterface);
    ret += generateMethodsInterface(fdInterface) + generateBroadcastsInterface(fdInterface);
    return ret;
}

std::string IPCServerIncludeGenerator::generateErrorEnum(const std::shared_ptr<FDInterface> &fdInterface)
{
    std::string ret;
    auto interface = fdInterface->getTarget();
    auto broadcasts = interface->getBroadcasts();
    auto methods = interface->getMethods();
    ret += c_method_common_error;
    for (auto it : broadcasts)
    {
        ret += generateBroadcastErrorEnum(it);
    }
    for (auto it : methods)
    {
        ret += generateMethodErrorEnum(it);
    }
    return ret;
}

std::string IPCServerIncludeGenerator::generateMethodErrorEnum(const std::shared_ptr<FMethod> &method)
{
    auto name = method->getName();
    auto error_enum = c_method_error;
    auto &common_gen = IPCCommonGenerator::getInstance();
    error_enum = error_enum.replace(error_enum.find("METHODID_error"), 8, getPrefix() + name);
    error_enum = replace_all(error_enum, "METHODID", toUpper(name));
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

std::string IPCServerIncludeGenerator::generateBroadcastErrorEnum(const std::shared_ptr<FBroadcast> &broadcast)
{
    auto name = broadcast->getName();
    auto error_enum = c_broadcast_error;
    error_enum = error_enum.replace(error_enum.find("BROADCASTID_error"), 11, getPrefix() + name);
    error_enum = replace_all(error_enum, "BROADCASTID", toUpper(name));
    return error_enum;
}

std::string IPCServerIncludeGenerator::generateMethodsInterface(const std::shared_ptr<FDInterface> &fdInterface)
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
                    method_string += "\n//TODO: This method need to be implemented in external user code.\n";
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

    if (IPCCommonGenerator::getInstance().isR5Server())
        method_string += c_r5_subscribe_callback_declaration + c_r5_reply_method_process_declaration;
    else
        method_string += c_message_process_declaration + c_subscribe_callback_declaration +
                         c_callback_process_declaration + c_reply_method_process_declaration;
    return method_string;
}

std::string IPCServerIncludeGenerator::generateMethodInterface(const std::shared_ptr<FMethod> &method)
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

    std::string error_name = getPrefix() + method_name + "_error";
    if (!out_args.empty())
    {
        std::string tmp_string;
        tmp_string = c_method_declaration;
        tmp_string = tmp_string.replace(tmp_string.find("ERROR_NAME"), 10, error_name);
        tmp_string = tmp_string.replace(tmp_string.find("METHODNAME"), 10, getPrefix() + method_name);
        std::list<std::string> out_args_list;
        common_gen.getMethodOutArgsStrList(out_args, out_args_list);
        auto out_args_string = join(out_args_list, ",");
        tmp_string = tmp_string.replace(tmp_string.find("ARGS"), 4, in_args_string + "," + out_args_string);
        method_inteface += tmp_string;
    }
    else
    {
        std::string tmp_string = c_method_declaration;
        tmp_string = tmp_string.replace(tmp_string.find("ERROR_NAME"), 10, error_name);
        tmp_string = tmp_string.replace(tmp_string.find("METHODNAME"), 10, getPrefix() + method_name);
        tmp_string = tmp_string.replace(tmp_string.find("ARGS"), 4, in_args_string);
        method_inteface += tmp_string;
    }
    return method_inteface;
}

std::string IPCServerIncludeGenerator::generateBroadcastsInterface(const std::shared_ptr<FDInterface> &fdInterface)
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
    return broadcast_string;
}
std::string IPCServerIncludeGenerator::generateBroadcastInterface(const std::shared_ptr<FBroadcast> &broadcast)
{
    std::string broadcast_inteface = "";
    auto out_args = broadcast->getOutArgs();
    auto broadcast_name = broadcast->getName();
    if (out_args.empty())
    {
        std::cerr << "broadcast " << broadcast_name << " didn't have output argument!\n";
        return broadcast_inteface;
    }

    std::string error_name = getPrefix() + broadcast_name + "_error";
    auto &common_gen = IPCCommonGenerator::getInstance();
    std::list<std::string> out_args_list;
    common_gen.getServerBroadcastArgsStrList(out_args, out_args_list);
    auto out_args_string = join(out_args_list, ",");

    std::string tmp_string;
    if (common_gen.isR5Server())
        tmp_string = c_r5_broadcast_declaration;
    else
        tmp_string = c_broadcast_declaration;
    tmp_string = tmp_string.replace(tmp_string.find("ERROR_NAME"), 10, error_name);
    tmp_string = tmp_string.replace(tmp_string.find("METHODNAME"), 10, getPrefix() + broadcast_name);
    tmp_string = tmp_string.replace(tmp_string.find("ARGS"), 4, out_args_string);
    broadcast_inteface += tmp_string;
    return broadcast_inteface;
}

} // namespace BstIdl