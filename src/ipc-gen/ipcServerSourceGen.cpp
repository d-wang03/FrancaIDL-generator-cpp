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
#include "ipc-gen/ipcServerSourceGen.h"
#include "utilities/string_utility.h"

namespace BstIdl
{
void IPCServerSourceGenerator::generateServerSource(const std::shared_ptr<FDInterface> &fdInterface,
                                                    const std::string &dir)
{
    auto fInterface = fdInterface->getTarget();
    auto source_path = IPCCommonGenerator::getInstance().getServerSourcePath(fInterface);
    auto s_path = mk_dir(dir, source_path);
    if (s_path.empty())
        return;
    auto pos = source_path.find_last_of("/") + 1;
    auto file_name = source_path.substr(pos, source_path.size() - pos);
    std::ofstream OsWriteS(s_path + "/" + file_name, std::ofstream::out);
    setPrefix(IPCCommonGenerator::getInstance().getPrefix());
    auto source = generator(fdInterface);
    OsWriteS << source;
    OsWriteS.close();
}

std::string IPCServerSourceGenerator::generator(const std::shared_ptr<FDInterface> &fdInterface)
{
    auto &common_gen = IPCCommonGenerator::getInstance();
    std::string client_source = getLicense();
    client_source += generateIncludeHeader(fdInterface);

    if (common_gen.isR5Server())
        client_source += c_r5_ipc_init_define + c_r5_ipc_deinit_define + c_r5_callback_process_define +
                         c_r5_subscribe_callback_define + getR5MethodDefination(fdInterface);
    else
        client_source += common_gen.generateDefineFAttribute(fdInterface) + buildMethodCmdList(fdInterface) +
                         c_ipc_init_define + c_ipc_deinit_define + c_callback_process_define +
                         c_subscribe_callback_define;
    client_source += generateMethodsInterface(fdInterface) + generateBroadcastsInterface(fdInterface);
    return client_source;
}

std::string IPCServerSourceGenerator::buildMethodCmdList(const std::shared_ptr<FDInterface> &fdInterface)
{
    std::string cmd_list_string = "static int8_t method_cmd_list[]={CMD_LIST};\n";
    auto fdModel = std::dynamic_pointer_cast<FDModel>(fdInterface->getContainer());
    auto mapper = fdModel->getMapper(fdInterface);
    auto interface = fdInterface->getTarget();
    auto methods = interface->getMethods();
    if (methods.empty())
        return "";
    std::list<std::string> cmd_list;
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
                        return "";
                    }
                    auto integer = std::dynamic_pointer_cast<FDInteger>(fdvalue->getSingle());
                    if (integer == nullptr)
                    {
                        std::cerr << "method:" << it->getName() << " didn't have method number\n";
                        return cmd_list_string;
                    }
                    auto id_number = integer->getValue();
                    for (auto it : cmd_list)
                    {
                        if (it == std::to_string(id_number))
                        {
                            std::cerr << "method number:" << id_number << "already exists!\n";
                            return "";
                        }
                    }

                    cmd_list.emplace_back(std::to_string(id_number));
                    break;
                }
            }
        }
        else
        {
            std::cerr << "method:" << it->getName() << " didn't connect to fdmethod\n";
            return "";
        }
    }
    cmd_list_string = cmd_list_string.replace(cmd_list_string.find("CMD_LIST"), 8, join(cmd_list, ","));
    return cmd_list_string;
}

std::string IPCServerSourceGenerator::generateIncludeHeader(const std::shared_ptr<FDInterface> &fdInterface)
{
    auto &common_gen = IPCCommonGenerator::getInstance();
    std::string include_header("");
    if (common_gen.isR5Server())
        include_header = c_r5_include_header;
    else
        include_header = c_include_header;
    auto interface_name = fdInterface->getTarget()->getName();
    include_header = include_header.replace(include_header.find("NAME"), 4, interface_name);
    // get server & client ID
    auto id_nums = IPCCommonGenerator::getInstance().getCurrClientAndServerID();
    auto client_num = id_nums.first;
    auto server_num = id_nums.second;
    if (client_num == server_num)
    {
        std::cerr << "Interface:" << fdInterface->getName()
                  << "\'s server number and client number shouldn't be the same!\n";
        include_header = include_header.replace(include_header.find("CLIENT_NUMBER"), 13,
                                                "CLIENT_NUMBER AND SERVER NUMBER SHOULD NOT BE THE SAME!!!");
        include_header = include_header.replace(include_header.find("SERVER_NUMBER"), 13,
                                                "CLIENT_NUMBER AND SERVER NUMBER SHOULD NOT BE THE SAME!!!");
    }
    else
    {
        include_header = include_header.replace(include_header.find("CLIENT_NUMBER"), 13, std::to_string(client_num));
        include_header = include_header.replace(include_header.find("SERVER_NUMBER"), 13, std::to_string(server_num));
    }
    return include_header;
}

std::string IPCServerSourceGenerator::generateMethodsInterface(const std::shared_ptr<FDInterface> &fdInterface)
{
    bool is_R5Server = IPCCommonGenerator::getInstance().isR5Server();
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
                    auto integer = std::dynamic_pointer_cast<FDInteger>(fdvalue->getSingle());
                    if (integer == nullptr)
                    {
                        std::cerr << "method:" << it->getName() << " didn't have method number\n";
                        return method_string;
                    }
                    auto id_number = integer->getValue();
                    (void)generateMethodInterface(it, id_number);
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

    if (is_R5Server)
        method_string += c_r5_reply_method_process_define;
    else
        method_string += c_reply_method_process_define;
    method_string = method_string.replace(method_string.find("CASE_SEGMENT"), 12,
                                          reply_method_case_string + fire_and_forget_method_case_string);
    reply_method_case_string = "";
    fire_and_forget_method_case_string = "";

    if (!is_R5Server)
        method_string += c_message_process_define;

    return method_string;
}

std::string IPCServerSourceGenerator::copyR5OutArgsToPayload(std::list<std::shared_ptr<FArgument>> out_args,
                                                             std::string arg_prefix)
{
    std::list<std::string> args_name;
    std::string args_mem_copy_string = "";
    std::list<std::string> pull_list;
    std::list<bool> is_byte_buffer;
    if (!arg_prefix.empty())
        arg_prefix.append("_");

    for (auto it : out_args)
    {
        args_name.emplace_back(arg_prefix + it->getName());
        auto basic_type = it->getType()->getPredefined();
        if (basic_type != nullptr && basic_type->getName() == "ByteBuffer")
        {
            is_byte_buffer.push_back(true);
        }
        else
            is_byte_buffer.push_back(false);
    }
    while (!args_name.empty())
    {
        if (!pull_list.empty())
        {
            auto pos = join(pull_list, "+");
            if (is_byte_buffer.front())
            {
                args_mem_copy_string += "memcpy(send_msg.payload + " + pos + "," + args_name.front() + ", " +
                                        args_name.front() + "_size);\n";
            }
            else
                args_mem_copy_string += "memcpy(send_msg.payload + " + pos + ",&" + args_name.front() + ", sizeof(" +
                                        args_name.front() + "));\n";
        }
        else
        {
            if (is_byte_buffer.front())
            {
                args_mem_copy_string +=
                    "memcpy(send_msg.payload, " + args_name.front() + ", " + args_name.front() + "_size);\n";
            }
            else
                args_mem_copy_string +=
                    "memcpy(send_msg.payload, &" + args_name.front() + ", sizeof(" + args_name.front() + "));\n";
        }
        if (is_byte_buffer.front())
        {
            pull_list.push_back(args_name.front() + "_size");
        }
        else
            pull_list.push_back("sizeof(" + args_name.front() + ")");
        args_name.pop_front();
        is_byte_buffer.pop_front();
    }
    return args_mem_copy_string;
}

std::string IPCServerSourceGenerator::copyOutArgsToPayload(std::list<std::shared_ptr<FArgument>> out_args,
                                                           std::string arg_prefix)
{
    std::list<std::string> args_name;
    std::string args_mem_copy_string = "";
    std::list<std::string> pull_list;
    std::list<bool> is_byte_buffer;
    if (!arg_prefix.empty())
        arg_prefix.append("_");

    for (auto it : out_args)
    {
        args_name.emplace_back(arg_prefix + it->getName());
        auto basic_type = it->getType()->getPredefined();
        if (basic_type != nullptr && basic_type->getName() == "ByteBuffer")
        {
            is_byte_buffer.push_back(true);
        }
        else
            is_byte_buffer.push_back(false);
    }
    while (!args_name.empty())
    {
        if (!pull_list.empty())
        {
            auto pos = join(pull_list, "+");
            if (is_byte_buffer.front())
            {
                args_mem_copy_string += "memcpy((char *)send_msg.payload + " + pos + "," + args_name.front() + ", " +
                                        args_name.front() + "_size);\n";
            }
            else
                args_mem_copy_string += "memcpy((char *)send_msg.payload + " + pos + ",&" + args_name.front() +
                                        ", sizeof(" + args_name.front() + "));\n";
        }
        else
        {
            if (is_byte_buffer.front())
            {
                args_mem_copy_string +=
                    "memcpy((char *)send_msg.payload, " + args_name.front() + ", " + args_name.front() + "_size);\n";
            }
            else
                args_mem_copy_string += "memcpy((char *)send_msg.payload, &" + args_name.front() + ", sizeof(" +
                                        args_name.front() + "));\n";
        }
        if (is_byte_buffer.front())
        {
            pull_list.push_back(args_name.front() + "_size");
        }
        else
            pull_list.push_back("sizeof(" + args_name.front() + ")");
        args_name.pop_front();
        is_byte_buffer.pop_front();
    }
    return args_mem_copy_string;
}

std::string IPCServerSourceGenerator::copyPayloadToInArgs(std::list<std::shared_ptr<FArgument>> in_args)
{
    std::list<std::string> args_name;
    std::string args_mem_copy_string = "";
    std::list<std::string> pull_list;
    std::list<bool> is_byte_buffer;
    for (auto it : in_args)
    {
        args_name.emplace_back(it->getName());
        auto basic_type = it->getType()->getPredefined();
        if (basic_type != nullptr && basic_type->getName() == "ByteBuffer")
        {
            is_byte_buffer.push_back(true);
        }
        else
            is_byte_buffer.push_back(false);
    }
    while (!args_name.empty())
    {
        if (!pull_list.empty())
        {
            auto pos = join(pull_list, "+");
            if (is_byte_buffer.front())
            {
                args_mem_copy_string += "memcpy(" + args_name.front() + ", (char *)in_msg->payload+" + pos + ", " +
                                        args_name.front() + "_size);\n";
            }
            else
                args_mem_copy_string += "memcpy(&" + args_name.front() + ", (char *)in_msg->payload+" + pos +
                                        ", sizeof(" + args_name.front() + "));\n";
        }
        else
        {
            if (is_byte_buffer.front())
            {
                args_mem_copy_string +=
                    "memcpy(" + args_name.front() + ", (char *)in_msg->payload, " + args_name.front() + "_size);\n";
            }
            else
                args_mem_copy_string +=
                    "memcpy(&" + args_name.front() + ", (char *)in_msg->payload, sizeof(" + args_name.front() + "));\n";
        }
        if (is_byte_buffer.front())
        {
            pull_list.push_back(args_name.front() + "_size");
        }
        else
            pull_list.push_back("sizeof(" + args_name.front() + ")");
        args_name.pop_front();
        is_byte_buffer.pop_front();
    }
    return args_mem_copy_string;
}
std::string IPCServerSourceGenerator::copyR5PayloadToInArgs(std::list<std::shared_ptr<FArgument>> in_args,
                                                            std::string method_name)
{
    std::string args_mem_copy_string = "";
    std::list<std::string> args_name;
    std::list<std::string> pull_list;
    std::list<bool> is_byte_buffer;
    for (auto it : in_args)
    {
        args_name.emplace_back(method_name + "_" + it->getName());
        auto basic_type = it->getType()->getPredefined();
        if (basic_type != nullptr && basic_type->getName() == "ByteBuffer")
        {
            is_byte_buffer.push_back(true);
        }
        else
            is_byte_buffer.push_back(false);
    }
    while (!args_name.empty())
    {
        if (!pull_list.empty())
        {
            auto pos = join(pull_list, "+");
            if (is_byte_buffer.front())
            {
                args_mem_copy_string +=
                    "memcpy(" + args_name.front() + ", in_msg->payload+" + pos + ", " + args_name.front() + "_size);\n";
            }
            else
                args_mem_copy_string += "memcpy(&" + args_name.front() + ", in_msg->payload+" + pos + ", sizeof(" +
                                        args_name.front() + "));\n";
        }
        else
        {
            if (is_byte_buffer.front())
            {
                args_mem_copy_string +=
                    "memcpy(" + args_name.front() + ", in_msg->payload, " + args_name.front() + "_size);\n";
            }
            else
                args_mem_copy_string +=
                    "memcpy(&" + args_name.front() + ", in_msg->payload, sizeof(" + args_name.front() + "));\n";
        }
        if (is_byte_buffer.front())
        {
            pull_list.push_back(args_name.front() + "_size");
        }
        else
            pull_list.push_back("sizeof(" + args_name.front() + ")");
        args_name.pop_front();
        is_byte_buffer.pop_front();
    }
    return args_mem_copy_string;
}
std::string IPCServerSourceGenerator::copyPayloadToInArgs(std::list<std::shared_ptr<FArgument>> in_args,
                                                          std::string method_name)
{
    std::string args_mem_copy_string = "";
    std::list<std::string> args_name;
    std::list<std::string> pull_list;
    std::list<bool> is_byte_buffer;
    for (auto it : in_args)
    {
        args_name.emplace_back(method_name + "_" + it->getName());
        auto basic_type = it->getType()->getPredefined();
        if (basic_type != nullptr && basic_type->getName() == "ByteBuffer")
        {
            is_byte_buffer.push_back(true);
        }
        else
            is_byte_buffer.push_back(false);
    }
    while (!args_name.empty())
    {
        if (!pull_list.empty())
        {
            auto pos = join(pull_list, "+");
            if (is_byte_buffer.front())
            {
                args_mem_copy_string += "memcpy(" + args_name.front() + ", (char *)in_msg->payload+" + pos + ", " +
                                        args_name.front() + "_size);\n";
            }
            else
                args_mem_copy_string += "memcpy(&" + args_name.front() + ", (char *)in_msg->payload+" + pos +
                                        ", sizeof(" + args_name.front() + "));\n";
        }
        else
        {
            if (is_byte_buffer.front())
            {
                args_mem_copy_string +=
                    "memcpy(" + args_name.front() + ", (char *)in_msg->payload, " + args_name.front() + "_size);\n";
            }
            else
                args_mem_copy_string +=
                    "memcpy(&" + args_name.front() + ", (char *)in_msg->payload, sizeof(" + args_name.front() + "));\n";
        }
        if (is_byte_buffer.front())
        {
            pull_list.push_back(args_name.front() + "_size");
        }
        else
            pull_list.push_back("sizeof(" + args_name.front() + ")");
        args_name.pop_front();
        is_byte_buffer.pop_front();
    }
    return args_mem_copy_string;
}

std::string IPCServerSourceGenerator::generateMethodInterface(const std::shared_ptr<FMethod> &method, int id_number)
{
    auto &common_gen = IPCCommonGenerator::getInstance();
    const bool is_R5Server = common_gen.isR5Server();
    std::string method_inteface = "";
    auto in_args = method->getInArgs();
    auto out_args = method->getOutArgs();
    auto method_name = method->getName();
    if (in_args.empty())
    {
        std::cerr << "method " << method_name << " didn't have input argument!\n";
        return method_inteface;
    }
    // get method defination
    std::string in_args_string = "";
    std::list<std::string> in_args_list;
    std::list<std::string> in_args_define_list;
    std::list<std::string> in_args_use_list;
    std::list<std::string> out_args_use_list;
    common_gen.getAllServerArgsLists(in_args, method_name, true, in_args_list, in_args_define_list, in_args_use_list);
    in_args_string = join(in_args_list, ",");
    std::string error_name = getPrefix() + method_name + "_error";

    if (!out_args.empty())
    {
        std::string out_args_string = "";
        std::string out_arg_define_string = "";
        std::list<std::string> out_args_list;
        std::list<std::string> out_args_define_list;

        common_gen.getAllServerArgsLists(out_args, method_name, false, out_args_list, out_args_define_list,
                                         out_args_use_list);
        out_args_string = join(out_args_list, ",");
        // payload_size with method_name
        std::string out_args_size(common_gen.getArgsPayloadSize(out_args, method_name));

        // method define
        std::string tmp_string;
        tmp_string = c_method_define;
        tmp_string = tmp_string.replace(tmp_string.find("ERROR_NAME"), 10, error_name);
        tmp_string = tmp_string.replace(tmp_string.find("METHODID"), 8, getPrefix() + method_name);
        tmp_string = tmp_string.replace(tmp_string.find("ARGS"), 4, in_args_string + "," + out_args_string);
        method_inteface += tmp_string;

        // c_reply_async_method_process_case_segment_define
        if (is_R5Server)
            reply_method_case_string += c_r5_reply_async_method_process_case_segment_define;
        else
            reply_method_case_string += c_reply_async_method_process_case_segment_define;

        if (id_number >= 0 && id_number < 256)
            replace_all(reply_method_case_string, "MSG_CMD", std::to_string(id_number));
        else
            std::cerr << "method " << method_name
                      << " have overflowed id number which is signed char type in fdepl: " << id_number << std::endl;
        std::string out_args_define = join(out_args_define_list, ";\n\t\t\t") + ";\n\t\t\t";
        std::string in_args_define = join(in_args_define_list, ";\n\t\t\t") + ";\n\t\t\t";
        replace_all(reply_method_case_string, "ARGS_DEFINE_SEGMENT", in_args_define + out_args_define);
        std::string call_method_string = getPrefix() + method->getName() + "(" + join(in_args_use_list, ",") + "," +
                                         join(out_args_use_list, ",") + ");";
        replace_all(reply_method_case_string, "CALL_METHOD", call_method_string);
        replace_all(reply_method_case_string, "ARGS_SIZE", out_args_size);
        std::string copyPayload_OutArgs("");
        std::string copyPayload_InArgs("");
        if (is_R5Server)
        {
            copyPayload_InArgs = replace_all(copyR5PayloadToInArgs(in_args, method_name), "\n", "\n\t\t\t");
            copyPayload_OutArgs = replace_all(copyR5OutArgsToPayload(out_args, method_name), "\n", "\n\t\t\t");
        }
        else
        {
            copyPayload_InArgs = replace_all(copyPayloadToInArgs(in_args, method_name), "\n", "\n\t\t\t");
            copyPayload_OutArgs = replace_all(copyOutArgsToPayload(out_args, method_name), "\n", "\n\t\t\t");
        }
        replace_all(reply_method_case_string, "COPY_PAYLOAD_TO_IN_ARGS", copyPayload_InArgs);
        replace_all(reply_method_case_string, "COPY_OUT_ARGS_TO_PAYLOAD", copyPayload_OutArgs);
    }
    else
    {
        // method define
        std::string tmp_string;
        tmp_string = c_method_define;
        tmp_string = tmp_string.replace(tmp_string.find("ERROR_NAME"), 10, error_name);
        tmp_string = tmp_string.replace(tmp_string.find("METHODID"), 8, getPrefix() + method_name);
        tmp_string = tmp_string.replace(tmp_string.find("ARGS"), 4, in_args_string);
        method_inteface += tmp_string;
        // c_fire_and_forget_method_process_case_segment_define
        fire_and_forget_method_case_string += c_fire_and_forget_method_process_case_segment_define;
        if (id_number >= 0 && id_number < 256)
            replace_all(fire_and_forget_method_case_string, "MSG_CMD", std::to_string(id_number));
        else
            std::cerr << "method " << method_name
                      << " have overflowed id number which is signed char type in fdepl: " << id_number << std::endl;
        std::string in_args_define = join(in_args_define_list, ";\n\t\t\t") + ";\n\t\t\t";
        replace_all(fire_and_forget_method_case_string, "ARGS_DEFINE_SEGMENT", in_args_define);
        std::string copyPayload_InArgs("");
        if (is_R5Server)
            copyPayload_InArgs = replace_all(copyR5PayloadToInArgs(in_args, method_name), "\n", "\n\t\t\t");
        else
            copyPayload_InArgs = replace_all(copyPayloadToInArgs(in_args, method_name), "\n", "\n\t\t\t");
        replace_all(fire_and_forget_method_case_string, "COPY_PAYLOAD_TO_IN_ARGS", copyPayload_InArgs);
        std::string call_method_string = getPrefix() + method->getName() + "(" + join(in_args_use_list, ",") + ");";
        replace_all(fire_and_forget_method_case_string, "CALL_METHOD", call_method_string);
    }
    return method_inteface;
}

std::string IPCServerSourceGenerator::generateBroadcastsInterface(const std::shared_ptr<FDInterface> &fdInterface)
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
                    auto integer = std::dynamic_pointer_cast<FDInteger>(fdvalue->getSingle());
                    if (integer == nullptr)
                    {
                        std::cerr << "broadcast:" << it->getName() << " didn't have broadcast number\n";
                        return broadcast_string;
                    }
                    auto id_number = integer->getValue();
                    broadcast_string += generateBroadcastInterface(it, id_number);
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

std::string IPCServerSourceGenerator::generateBroadcastInterface(const std::shared_ptr<FBroadcast> &broadcast,
                                                                 int id_number)
{
    auto &common_gen = IPCCommonGenerator::getInstance();
    std::string broadcast_inteface = "";
    auto out_args = broadcast->getOutArgs();
    auto broadcast_name = broadcast->getName();
    if (out_args.empty())
    {
        std::cerr << "broadcast " << broadcast_name << " didn't have output argument!\n";
        return broadcast_inteface;
    }

    std::list<std::string> out_args_list;
    common_gen.getServerBroadcastArgsStrList(out_args, out_args_list);
    auto out_args_string = join(out_args_list, ",");
    std::string out_args_size(common_gen.getArgsPayloadSize(out_args, ""));
    // broadcast defination gen
    std::string tmp_string;
    if (common_gen.isR5Server())
        tmp_string = c_r5_broadcast_define;
    else
        tmp_string = c_broadcast_define;
    auto error_name = getPrefix() + broadcast_name + "_error";
    tmp_string = tmp_string.replace(tmp_string.find("ERROR_NAME"), 10, error_name);
    tmp_string = tmp_string.replace(tmp_string.find("BroadcastID"), 11, getPrefix() + broadcast_name);
    if (id_number >= 0 && id_number < 256)
        tmp_string = tmp_string.replace(tmp_string.find("CMD_NUMBER"), 10, std::to_string(id_number));
    else
        std::cerr << "broadcast: " << broadcast_name
                  << " have overflowed id number which is signed char type in fdepl: " << id_number << std::endl;
    tmp_string = tmp_string.replace(tmp_string.find("ARGS_SIZE"), 9, out_args_size);
    tmp_string = tmp_string.replace(tmp_string.find("ARGS"), 4, out_args_string);
    std::string copyOutArgs("");
    if (common_gen.isR5Server())
        copyOutArgs = replace_all(copyR5OutArgsToPayload(out_args, std::string("")), "\n", "\n\t");
    else
        copyOutArgs = replace_all(copyOutArgsToPayload(out_args, std::string("")), "\n", "\n\t");
    replace_all(tmp_string, "COPY_OUT_ARGS_TO_PAYLOAD", copyOutArgs);
    tmp_string = replace_all(tmp_string, "Broadcast_ID", broadcast_name);
    tmp_string = replace_all(tmp_string, "BROADCASTNAME", toUpper(broadcast->getName()));
    broadcast_inteface += tmp_string;
    return broadcast_inteface;
}

std::string IPCServerSourceGenerator::getR5MethodDefination(const std::shared_ptr<FDInterface> &fdInterface)
{
    auto &common_gen = IPCCommonGenerator::getInstance();
    auto interface = fdInterface->getTarget();
    std::string method_define = "";
    for (const auto &method : interface->getMethods())
    {
        auto in_args = method->getInArgs();
        auto out_args = method->getOutArgs();
        auto method_name = method->getName();
        if (in_args.empty())
        {
            std::cerr << "method " << method_name << " didn't have input argument!\n";
            return method_define;
        }
        // interface define
        auto in_args_string = join(common_gen.getArgsList(in_args), ",");
        std::string error_name = getPrefix() + method_name + "_error";
        if (!out_args.empty())
        {
            std::string tmp_string;
            tmp_string = c_r5_method_defination;
            tmp_string = tmp_string.replace(tmp_string.find("ERROR_NAME"), 10, error_name);
            tmp_string = tmp_string.replace(tmp_string.find("METHODNAME"), 10, getPrefix() + method_name);
            std::list<std::string> out_args_list;
            common_gen.getMethodOutArgsStrList(out_args, out_args_list);
            auto out_args_string = join(out_args_list, ",");
            tmp_string = tmp_string.replace(tmp_string.find("ARGS"), 4, in_args_string + "," + out_args_string);
            method_define += tmp_string;
        }
        else
        {
            std::string tmp_string = c_r5_method_defination;
            tmp_string = tmp_string.replace(tmp_string.find("ERROR_NAME"), 10, error_name);
            tmp_string = tmp_string.replace(tmp_string.find("METHODNAME"), 10, getPrefix() + method_name);
            tmp_string = tmp_string.replace(tmp_string.find("ARGS"), 4, in_args_string);
            method_define += tmp_string;
        }
    }
    return method_define;
}

} // namespace BstIdl
