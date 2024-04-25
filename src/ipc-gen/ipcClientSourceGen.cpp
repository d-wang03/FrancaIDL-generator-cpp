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
#include "ipc-gen/ipcClientSourceGen.h"
#include "utilities/string_utility.h"

namespace BstIdl
{
void IPCClientSourceGenerator::generateClientSource(const std::shared_ptr<FDInterface> &fdInterface,
                                                    const std::string &dir)
{
    auto fInterface = fdInterface->getTarget();
    auto source_path = IPCCommonGenerator::getInstance().getClientSourcePath(fInterface);
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

std::string IPCClientSourceGenerator::generator(const std::shared_ptr<FDInterface> &fdInterface)
{
    auto &common_gen = IPCCommonGenerator::getInstance();
    std::string client_source = getLicense();
    client_source += generateIncludeHeader(fdInterface) + common_gen.generateDefineFAttribute(fdInterface) +
                     c_ipc_init_define + c_ipc_deinit_define + c_subscribe_callback_define;
    client_source += generateMethodsInterface(fdInterface) + generateBroadcastsInterface(fdInterface);
    return client_source;
}
std::string IPCClientSourceGenerator::generateIncludeHeader(const std::shared_ptr<FDInterface> &fdInterface)
{
    std::string include_header = c_include_header;
    auto interface_name = fdInterface->getTarget()->getName();
    include_header = include_header.replace(include_header.find("NAME"), 4, interface_name);

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
        return include_header;
    }
    include_header = include_header.replace(include_header.find("CLIENT_NUMBER"), 13, std::to_string(client_num));
    include_header = include_header.replace(include_header.find("SERVER_NUMBER"), 13, std::to_string(server_num));
    return include_header;
}

std::string IPCClientSourceGenerator::generateMethodsInterface(const std::shared_ptr<FDInterface> &fdInterface)
{
    std::string method_string = "";
    auto fdModel = std::dynamic_pointer_cast<FDModel>(fdInterface->getContainer());
    auto mapper = fdModel->getMapper(fdInterface);
    auto interface = fdInterface->getTarget();
    auto methods = interface->getMethods();
    std::list<int> method_cmd_list;
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
                    if (contains(method_cmd_list, id_number))
                    {
                        std::cerr << "method " << it->getName() << "'s method number:" << id_number
                                  << " already exist!\n";
                        return method_string;
                    }
                    method_cmd_list.push_back(id_number);
                    method_string += generateMethodInterface(it, id_number);
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
    method_string += c_reply_method_process_define;
    method_string = method_string.replace(method_string.find("CASE_SEGMENT"), 12, method_case_segment_string);
    method_case_segment_string = "";
    method_string += c_client_callback_process_define;
    return method_string;
}

std::string IPCClientSourceGenerator::copyInArgs(std::list<std::shared_ptr<FArgument>> in_args)
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
                args_mem_copy_string += "\tmemcpy((char *)send_msg.payload+" + pos + "," + args_name.front() + ", " +
                                        args_name.front() + "_size);\n";
            }
            else
                args_mem_copy_string += "\tmemcpy((char *)send_msg.payload+" + pos + ",&" + args_name.front() +
                                        ", sizeof(" + args_name.front() + "));\n";
        }
        else
        {
            if (is_byte_buffer.front())
            {
                args_mem_copy_string +=
                    "memcpy((char *)send_msg.payload," + args_name.front() + ", " + args_name.front() + "_size);\n";
            }
            else
                args_mem_copy_string +=
                    "memcpy((char *)send_msg.payload,&" + args_name.front() + ", sizeof(" + args_name.front() + "));\n";
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

std::string IPCClientSourceGenerator::copyOutArgs(std::list<std::shared_ptr<FArgument>> out_args)
{
    std::list<std::string> args_name;
    std::string args_mem_copy_string = "";
    std::list<std::string> pull_list;
    std::list<bool> is_byte_buffer;
    for (auto it : out_args)
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
                args_mem_copy_string += "\tmemcpy(" + args_name.front() + ", (char *)re_msg.payload+" + pos + ", " +
                                        args_name.front() + "_size);\n";
            }
            else
                args_mem_copy_string += "\tmemcpy(" + args_name.front() + ", (char *)re_msg.payload+" + pos +
                                        ", sizeof(*" + args_name.front() + "));\n";
        }
        else
        {
            if (is_byte_buffer.front())
            {
                args_mem_copy_string +=
                    "memcpy(" + args_name.front() + ", (char *)re_msg.payload, " + args_name.front() + "_size);\n";
            }
            else
                args_mem_copy_string +=
                    "memcpy(" + args_name.front() + ", (char *)re_msg.payload, sizeof(*" + args_name.front() + "));\n";
        }
        if (is_byte_buffer.front())
        {
            pull_list.push_back(args_name.front() + "_size");
        }
        else
            pull_list.push_back("sizeof(*" + args_name.front() + ")");
        args_name.pop_front();
        is_byte_buffer.pop_front();
    }
    return args_mem_copy_string;
}

std::string IPCClientSourceGenerator::copyOutArgs_1(std::list<std::shared_ptr<FArgument>> out_args)
{
    std::list<std::string> args_name;
    std::string args_mem_copy_string = "";
    std::list<std::string> pull_list;
    std::list<bool> is_byte_buffer;
    for (auto it : out_args)
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
                args_mem_copy_string += "\t\tmemcpy(" + args_name.front() + ", (char *)re_msg->payload+" + pos + ", " +
                                        args_name.front() + "_size);\n";
            }
            else
                args_mem_copy_string += "\t\tmemcpy(&" + args_name.front() + ", (char *)re_msg->payload+" + pos +
                                        ", sizeof(" + args_name.front() + "));\n";
        }
        else
        {
            if (is_byte_buffer.front())
            {
                args_mem_copy_string +=
                    "memcpy(" + args_name.front() + ", (char *)re_msg->payload, " + args_name.front() + "_size);\n";
            }
            else
                args_mem_copy_string +=
                    "memcpy(&" + args_name.front() + ", (char *)re_msg->payload, sizeof(" + args_name.front() + "));\n";
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

std::string IPCClientSourceGenerator::generateMethodInterface(const std::shared_ptr<FMethod> &method, int id_number)
{
    auto &common_gen = IPCCommonGenerator::getInstance();
    std::string method_inteface = "";
    auto in_args = method->getInArgs();
    auto out_args = method->getOutArgs();
    auto method_name = method->getName();
    bool isOverflow = false;
    if (in_args.empty())
    {
        std::cerr << "method " << method_name << " didn't have input argument!\n";
        return method_inteface;
    }
    if (id_number < 0 || id_number > 255)
    {
        isOverflow = true;
        std::cerr << "method " << method_name
                  << " have overflowed id number which is signed char type in fdepl: " << id_number << std::endl;
    }

    // in_args_list str & payload_sizeof
    auto in_args_string = join(common_gen.getArgsList(in_args), ",");
    std::string in_args_size(common_gen.getArgsPayloadSize(in_args, ""));

    if (!out_args.empty())
    {
        std::string error_name = getPrefix() + method_name + "_error";
        std::string out_args_string = "";
        std::list<std::string> out_args_list;
        std::list<std::string> out_args_define_list;
        std::list<std::string> out_args_reference;

        common_gen.getAllClientOutArgsLists(out_args, out_args_list, out_args_define_list, out_args_reference);
        out_args_string = join(out_args_list, ",");
        std::string tmp_string;
        tmp_string = c_callback_pointer;
        method_inteface += replace_all(tmp_string, "ID", getPrefix() + method_name);
        tmp_string = c_sync_method_define;
        tmp_string = tmp_string.replace(tmp_string.find("ERROR_NAME"), 10, error_name);
        tmp_string = tmp_string.replace(tmp_string.find("METHODID_SyncCall"), 8, getPrefix() + method_name);
        tmp_string = tmp_string.replace(tmp_string.find("ARGS"), 4, in_args_string + "," + out_args_string);
        tmp_string = tmp_string.replace(tmp_string.find("METHOD_CMD"), 10,
                                        isOverflow ? "ERROR_METHOD_CMD_OVERFLOW_" + std::to_string(id_number)
                                                   : std::to_string(id_number));
        tmp_string = tmp_string.replace(tmp_string.find("SIZE_OF_ARGS"), 12, in_args_size);
        tmp_string = replace_all(tmp_string, "METHODID", toUpper(method_name));
        tmp_string = tmp_string.replace(tmp_string.find("COPY_INARGS_TO_PAYLOAD"), 22, copyInArgs(in_args));
        tmp_string = tmp_string.replace(tmp_string.find("COPY_PAYLOAD_TO_OUTARGS"), 23, copyOutArgs(out_args));
        method_inteface += tmp_string;

        tmp_string = c_async_method_send_msg_declaration;
        tmp_string = replace_all(tmp_string, "METHODID", toUpper(method_name));
        method_inteface += tmp_string;

        tmp_string = c_async_method_register_define;
        tmp_string = tmp_string.replace(tmp_string.find("ERROR_NAME"), 10, error_name);
        tmp_string = tmp_string.replace(tmp_string.find("METHODID_Async_Register"), 8, getPrefix() + method_name);
        tmp_string = tmp_string.replace(tmp_string.find("MethodID_ASyncCallback_t"), 8, getPrefix() + method_name);
        tmp_string = tmp_string.replace(tmp_string.find("METHODID_async_callback"), 8, getPrefix() + method_name);
        tmp_string = tmp_string.replace(tmp_string.find("ARGS"), 4, in_args_string);
        tmp_string = tmp_string.replace(tmp_string.find("METHOD_CMD"), 10,
                                        isOverflow ? "ERROR_METHOD_CMD_OVERFLOW_" + std::to_string(id_number)
                                                   : std::to_string(id_number));
        tmp_string = tmp_string.replace(tmp_string.find("SIZE_OF_ARGS"), 12, in_args_size);
        tmp_string = replace_all(tmp_string, "METHODID", toUpper(method_name));
        tmp_string = tmp_string.replace(tmp_string.find("COPY_INARGS_TO_PAYLOAD"), 22, copyInArgs(in_args));
        tmp_string = replace_all(tmp_string, ")send_msg", ")" + toUpper(method_name) + "_send_msg");
        method_inteface += tmp_string;

        tmp_string = c_async_method_define;
        tmp_string = tmp_string.replace(tmp_string.find("ERROR_NAME"), 10, error_name);
        tmp_string = tmp_string.replace(tmp_string.find("METHODID_ASyncCall"), 8, getPrefix() + method_name);
        tmp_string = replace_all(tmp_string, "METHODID", toUpper(method_name));
        method_inteface += tmp_string;
        method_case_segment_string += c_case_segment;
        method_case_segment_string = method_case_segment_string.replace(
            method_case_segment_string.find("CMD_NUMBER"), 10,
            isOverflow ? "ERROR_CMD_NUMBER_OVERFLOW_" + std::to_string(id_number) : std::to_string(id_number));
        std::string out_args_define = join(out_args_define_list, ";\n\t\t") + ";\n";
        method_case_segment_string = method_case_segment_string.replace(
            method_case_segment_string.find("OUT_ARGS_DEFINE_SEGMENT"), 23, out_args_define);
        method_case_segment_string = method_case_segment_string.replace(
            method_case_segment_string.find("COPY_PAYLOAD_TO_OUTARGS"), 23, copyOutArgs_1(out_args));
        auto out_args_reference_string = join(out_args_reference, ",");
        method_case_segment_string = method_case_segment_string.replace(method_case_segment_string.find("OUTARGS"), 7,
                                                                        out_args_reference_string);
        method_case_segment_string = replace_all(method_case_segment_string, "METHODID", getPrefix() + method_name);
    }
    else
    {
        std::string tmp_string = c_fire_and_forget_define;
        std::string error_name = getPrefix() + method_name + "_error";
        tmp_string = tmp_string.replace(tmp_string.find("ERROR_NAME"), 10, error_name);
        tmp_string = tmp_string.replace(tmp_string.find("METHODID"), 8, getPrefix() + method_name);
        tmp_string = tmp_string.replace(tmp_string.find("ARGS"), 4, in_args_string);
        tmp_string = tmp_string.replace(tmp_string.find("METHOD_CMD"), 10,
                                        isOverflow ? "ERROR_METHOD_CMD_OVERFLOW_" + std::to_string(id_number)
                                                   : std::to_string(id_number));
        tmp_string = tmp_string.replace(tmp_string.find("SIZE_OF_ARGS"), 12, in_args_size);
        tmp_string = tmp_string.replace(tmp_string.find("COPY_INARGS_TO_PAYLOAD"), 22, copyInArgs(in_args));
        tmp_string = replace_all(tmp_string, "METHODID", toUpper(method_name));
        method_inteface += tmp_string;
    }
    return method_inteface;
}

std::string IPCClientSourceGenerator::generateBroadcastsInterface(const std::shared_ptr<FDInterface> &fdInterface)
{
    std::string broadcast_string = "";
    auto fdModel = std::dynamic_pointer_cast<FDModel>(fdInterface->getContainer());
    auto mapper = fdModel->getMapper(fdInterface);
    auto interface = fdInterface->getTarget();
    auto broadcasts = interface->getBroadcasts();
    std::list<int> broadcast_cmd_list;
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
                    if (contains(broadcast_cmd_list, id_number))
                    {
                        std::cerr << "broadcast " << it->getName() << "'s broadcast number:" << id_number
                                  << " already exist!\n";
                        return broadcast_string;
                    }
                    broadcast_cmd_list.push_back(id_number);
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
    broadcast_string += c_reply_broadcast_process_define;
    broadcast_string =
        broadcast_string.replace(broadcast_string.find("CASE_SEGMENT"), 12, broadcast_case_segment_string);
    broadcast_case_segment_string = "";
    return broadcast_string;
}

std::string IPCClientSourceGenerator::generateBroadcastInterface(const std::shared_ptr<FBroadcast> &broadcast,
                                                                 int id_number)
{
    std::string broadcast_inteface = "";
    auto out_args = broadcast->getOutArgs();
    auto broadcast_name = broadcast->getName();
    bool isOverflow = false;
    if (out_args.empty())
    {
        std::cerr << "broadcast:" << broadcast_name << " didn't connect to fdbroadcast\n";
        return broadcast_inteface;
    }
    if (id_number < 0 || id_number > 255)
    {
        isOverflow = true;
        std::cerr << "broadcast " << broadcast_name
                  << " have overflowed id number which is signed char type in fdepl: " << id_number << std::endl;
    }
    std::string error_name = getPrefix() + broadcast_name + "_error";
    std::string out_args_string = "";
    std::list<std::string> out_args_list;
    std::list<std::string> out_args_define_list;
    std::list<std::string> out_args_reference;

    IPCCommonGenerator::getInstance().getAllClientOutArgsLists(out_args, out_args_list, out_args_define_list,
                                                               out_args_reference);
    // bc_inf gen
    out_args_string = join(out_args_list, ",");
    std::string tmp_string;
    tmp_string = c_callback_pointer;
    broadcast_inteface += replace_all(tmp_string, "ID", getPrefix() + broadcast_name);
    tmp_string = c_broadcast_subscribe_define;
    tmp_string = tmp_string.replace(tmp_string.find("ERROR_NAME"), 10, error_name);
    tmp_string = tmp_string.replace(tmp_string.find("CMD_NUMBER"), 10,
                                    isOverflow ? "ERROR_CMD_NUMBER_OVERFLOW_" + std::to_string(id_number)
                                               : std::to_string(id_number));
    tmp_string = tmp_string.replace(tmp_string.find("BROADCASTID_Subscribe"), 11, getPrefix() + broadcast_name);
    tmp_string = tmp_string.replace(tmp_string.find("BroadcastID_ASyncCallback_t"), 11, getPrefix() + broadcast_name);
    tmp_string = tmp_string.replace(tmp_string.find("BROADCASTID_async_callback"), 11, getPrefix() + broadcast_name);
    tmp_string = replace_all(tmp_string, "BROADCASTID", toUpper(broadcast_name));
    broadcast_inteface += tmp_string;

    broadcast_case_segment_string += c_case_segment;
    broadcast_case_segment_string = broadcast_case_segment_string.replace(
        broadcast_case_segment_string.find("CMD_NUMBER"), 10,
        isOverflow ? "ERROR_CMD_NUMBER_OVERFLOW_" + std::to_string(id_number) : std::to_string(id_number));
    std::string out_args_define = join(out_args_define_list, ";\n\t\t") + ";\n";
    broadcast_case_segment_string = broadcast_case_segment_string.replace(
        broadcast_case_segment_string.find("OUT_ARGS_DEFINE_SEGMENT"), 23, out_args_define);
    broadcast_case_segment_string = broadcast_case_segment_string.replace(
        broadcast_case_segment_string.find("COPY_PAYLOAD_TO_OUTARGS"), 23, copyOutArgs_1(out_args));
    auto out_args_reference_string = join(out_args_reference, ",");
    broadcast_case_segment_string = broadcast_case_segment_string.replace(broadcast_case_segment_string.find("OUTARGS"),
                                                                          7, out_args_reference_string);
    broadcast_case_segment_string =
        replace_all(broadcast_case_segment_string, "METHODID", getPrefix() + broadcast_name);

    return broadcast_inteface;
}
} // namespace BstIdl
