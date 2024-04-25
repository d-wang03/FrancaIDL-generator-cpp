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
#ifndef IPC_SERVER_SOURCE_GEN_H
#define IPC_SERVER_SOURCE_GEN_H
#include "ipcCommonItemGen.h"
#include "model/FFactory.h"
#include "model/FModelManager.h"
#include <fstream>
#include <iostream>
namespace BstIdl
{
class IPCServerSourceGenerator : public AbstractGenerator
{
private:
    IPCServerSourceGenerator() = default;
    ~IPCServerSourceGenerator() = default;
    IPCServerSourceGenerator(const IPCServerSourceGenerator &) = delete;
    IPCServerSourceGenerator(IPCServerSourceGenerator &&) noexcept = delete;
    IPCServerSourceGenerator &operator=(const IPCServerSourceGenerator &) = delete;
    IPCServerSourceGenerator &operator=(IPCServerSourceGenerator &&) = delete;
    void setPrefix(const std::string &common_prefix)
    {
        prefix = common_prefix;
    }
    std::string c_include_header = R"(
#include "NAMEServer.h"
int32_t session_id=0;
static const enum ipc_core_e client = (enum ipc_core_e)CLIENT_NUMBER;
static const enum ipc_core_e server = (enum ipc_core_e)SERVER_NUMBER;
)";
    std::string c_r5_include_header = R"(
#include "NAMEServer.h"
static const enum ipc_core_e client = (enum ipc_core_e)CLIENT_NUMBER;
static const enum ipc_core_e server = (enum ipc_core_e)SERVER_NUMBER;
)";
    std::string c_method_define = R"(
enum ERROR_NAME METHODID(ARGS)
{
}
    )";
    std::string c_ipc_init_define = R"(
int32_t ipc_init()
{
    session_id = IPC_Register(client, server);
    IPC_PRINT("session_id=%d",session_id);
    if (session_id == -REGISTER_IPC_OPEN_NODE_FAILED)
    {
        IPC_PRINT("IPC_register ipc node failed!\n");
        return -IPC_REGISTER_OPEN_NODE_ERROR;
    }
    if (session_id == -REGISTER_IPC_REQUEST_SESSION_FAIL)
    {
        IPC_PRINT("IPC_register ipc node failed!\n");
        return -IPC_REGISTER_REQUEST_SESSION_ERROR;
    }
    if (session_id == -REGISTER_IPC_DEPENDENCY_FAILED)
    {
        IPC_PRINT("IPC_register ipc node failed!\n");
        return -IPC_REGISTER_CREAT_RECV_PROCESS_ERROR;
    }
    return session_id;
}
    )";
    std::string c_ipc_deinit_define = R"(
int32_t ipc_deinit()
{
    IPC_PRINT("server ipc_deinit\n");
    int32_t ret = IPC_Unregister(session_id);
    if (ret == -UNREGISTER_IPC_FAILED)
    {
        IPC_PRINT("IPC_Unregister failed!\n");
        return -IPC_DEINIT_ERROR;
    }
    IPC_PRINT("server ipc_deinit success\n");
    return ret;
}
    )";
    std::string c_r5_ipc_init_define = R"(
sint32 ipc_init()
{
    session_id = IPC_Register(client, server);
    LOG(DBG_INFO,"IpcLibRecvApp", "session_id=%d",session_id);
    if (session_id == -REGISTER_IPC_OPEN_NODE_FAILED)
    {
        LOG(DBG_INFO,"IpcLibRecvApp", "IPC_register ipc node failed!");
        return -IPC_REGISTER_OPEN_NODE_ERROR;
    }
    if (session_id == -REGISTER_IPC_REQUEST_SESSION_FAIL)
    {
        LOG(DBG_INFO,"IpcLibRecvApp", "IPC_register ipc node failed!");
        return -IPC_REGISTER_REQUEST_SESSION_ERROR;
    }
    if (session_id == -REGISTER_IPC_DEPENDENCY_FAILED)
    {
        LOG(DBG_INFO,"IpcLibRecvApp", "IPC_register ipc node failed!");
        return -IPC_REGISTER_CREAT_RECV_PROCESS_ERROR;
    }
    return session_id;
}
    )";
    std::string c_r5_ipc_deinit_define = R"(
sint32 ipc_deinit()
{
    LOG(DBG_INFO,"IpcLibRecvApp", "server ipc_deinit");
    sint32 ret = IPC_Unregister(session_id);
    if (ret == -UNREGISTER_IPC_FAILED)
    {
        LOG(DBG_INFO,"IpcLibRecvApp", "IPC_Unregister failed!");
        return -IPC_DEINIT_ERROR;
    }
    LOG(DBG_INFO,"IpcLibRecvApp", "server ipc_deinit success");
    return ret;
}
    )";
    // message process define
    std::string c_message_process_define = R"(
int32_t message_process()
{
    if (session_id < 0)
    {
        IPC_PRINT("IPC_register  session id is lower than zero");
        return -session_id;
    }
    for (int8_t i = 0; i < sizeof(method_cmd_list); i++)
    {
        int32_t ret = Register_Method(session_id, method_cmd_list[i]);
        if (ret == -REGISTER_METHOD_SESSION_INVALID)
        {
            IPC_PRINT("Register method failed:session id overflow!\n");
            return SESSION_ID_OVERFLOW_ERROR;
        }
        if (ret == -REGISTER_METHOD_CMD_INVALID)
        {
            IPC_PRINT("Register method failed:command id is out of range!\n");
            return CMD_ID_OVERFLOW_ERROR;
        }
        if (ret == -REGISTER_METHOD_FAILED)
        {
            IPC_PRINT("Register method failed!\n");
            return REGISTER_METHOD_ERROR;
        }
    }
    int32_t ret = subscribe_callback();
    if (ret == REGISTER_CALLBACK_ERROR)
        return ret;
    return NO_ERROR;
}
    )";
    // subscribe callback define
    // r5 int subscribe_callback
    std::string c_subscribe_callback_define = R"(
int32_t subscribe_callback()
{
    if (Register_Callback_Package_Process(session_id, callback_process) != 0)
        return REGISTER_CALLBACK_ERROR;
    return NO_ERROR;
}
    )";
    std::string c_r5_subscribe_callback_define = R"(
int subscribe_callback()
{
    if (Register_Callback_Package_Process(session_id, callback_process) != 0)
        return REGISTER_CALLBACK_ERROR;
    return NO_ERROR;
} 
    )";
    // callback process define
    std::string c_callback_process_define = R"(
int32_t callback_process(struct msg_package *re_msg, ipc_msg_type type, uint16_t token)
{
    if(type==MSG_TYPE_METHOD)
    {
        return reply_method_process(re_msg,token);
    }
    else
    {
        IPC_PRINT("Wrong async_func_calllback reply type!\n");
        return REPLY_MSG_TYPE_ERROR;
    } 
}
)";
    std::string c_r5_callback_process_define = R"(
sint32 callback_process(struct msg_package *re_msg, ipc_msg_type type, uint16 token)
{
    if (type == MSG_TYPE_METHOD)
    {
        sint32 ret = reply_method_process(re_msg, token);
        return ret;
    }
    else
    {
        LOG(DBG_INFO,"IpcLibRecvApp", "Wrong async_func_calllback reply type!");
        return REPLY_MSG_TYPE_ERROR;
    }
}
)";
    // reply method process define
    std::string c_reply_method_process_define = R"(
int32_t reply_method_process(struct msg_package *in_msg, uint16_t token)
{ 
    struct msg_package send_msg;
    send_msg.cmd=in_msg->cmd;
    switch(in_msg->cmd)
    {
        CASE_SEGMENT
        default:
            return METHOD_CMD_INVALID_ERROR;
    }
    if(in_msg->long_param==1)
    {
        int32_t ret = IPC_send_Package(session_id, &send_msg, MSG_TYPE_REPLY,token, -1);
        if (ret == -SEND_MSG_SESSION_INVALID)
        {
            IPC_PRINT("reply method process:session id overflow!\n");
            return SESSION_ID_OVERFLOW_ERROR;
        }
        if (ret == -SEND_MSG_TYPE_INVALID)
        {
            IPC_PRINT("reply method process:send message type invalid!\n");
            return SEND_MSG_TYPE_INVALID_ERROR;
        }
    }
    return NO_ERROR;
}
    )";
    // reply method process define
    std::string c_r5_reply_method_process_define = R"(
sint32 reply_method_process(struct msg_package *in_msg, uint16 token)
{ 
    struct msg_package send_msg;
    send_msg.cmd=in_msg->cmd;
    switch(in_msg->cmd)
    {
        CASE_SEGMENT
        default:
            return METHOD_CMD_INVALID_ERROR;
    }
    if(in_msg->long_param==1)
    {
        sint32 ret = IPC_send_Package(session_id, &send_msg, MSG_TYPE_REPLY,token, -1);
        if (ret == -SEND_MSG_SESSION_INVALID)
        {
            LOG(DBG_INFO,"IpcLibRecvApp", "reply method process:session id overflow!");
            return SESSION_ID_OVERFLOW_ERROR;
        }
        if (ret == -SEND_MSG_TYPE_INVALID)
        {
            LOG(DBG_INFO,"IpcLibRecvApp", "reply method process:send message type invalid!");
            return SEND_MSG_TYPE_INVALID_ERROR;
        }
    }
    return NO_ERROR;
}
    )";
    // reply method process case segment define
    std::string c_reply_async_method_process_case_segment_define = R"(
        case MSG_CMD:
        {
            ARGS_DEFINE_SEGMENT
            COPY_PAYLOAD_TO_IN_ARGS
            CALL_METHOD
            send_msg.payload_size = (ARGS_SIZE-1)/8+1;
            if(send_msg.payload_size>7)
            {
                IPC_PRINT("date overflow,date size should not be more than 56 byte!\n");
                return MSG_SIZE_OVERFLOW_ERROR;
            }
            COPY_OUT_ARGS_TO_PAYLOAD
            break;
        }
    )";
    std::string c_r5_reply_async_method_process_case_segment_define = R"(
        case MSG_CMD:
        {
            ARGS_DEFINE_SEGMENT
            COPY_PAYLOAD_TO_IN_ARGS
            CALL_METHOD
            send_msg.payload_size = (ARGS_SIZE-1)/8+1;
            if(send_msg.payload_size>7)
            {
                LOG(DBG_INFO,"IpcLibRecvApp", "date overflow,date size should not be more than 56 byte!");
                return MSG_SIZE_OVERFLOW_ERROR;
            }
            COPY_OUT_ARGS_TO_PAYLOAD
            break;
        }
    )";
    // fire and forget method process case segment define
    std::string c_fire_and_forget_method_process_case_segment_define = R"(
        case MSG_CMD:
        {
            ARGS_DEFINE_SEGMENT
            COPY_PAYLOAD_TO_IN_ARGS
            CALL_METHOD
            break;
        }
    )";
    std::string reply_method_case_string = "";
    std::string fire_and_forget_method_case_string = "";
    // subscribe callback define
    std::string c_broadcast_define = R"(
enum ERROR_NAME BroadcastID(ARGS,int32_t timeout)
{
    if (session_id < 0) 
    {
        IPC_PRINT("Broadcast_ID failed:session id is lower than zero");
        return -session_id;
    }
    struct msg_package send_msg;
    send_msg.cmd=CMD_NUMBER;
    send_msg.payload_size = (ARGS_SIZE-1)/8+1;
    if(send_msg.payload_size>7)
    {
        IPC_PRINT("date overflow,date size should not be more than 7 byte!\n");
        return BROADCASTNAME_MSG_SIZE_OVERFLOW_ERROR;
    }
    COPY_OUT_ARGS_TO_PAYLOAD

    int32_t ret = IPC_send_Package(session_id, &send_msg, MSG_TYPE_SIGNAL, 0 , timeout);
    if(ret==-SEND_MSG_SESSION_INVALID)
    {
        IPC_PRINT("Broadcast_ID failed:session id overflow!\n");
        return BROADCASTNAME_SESSION_ID_OVERFLOW_ERROR;
    }
    if(ret==-SEND_MSG_TYPE_INVALID)
    {
        IPC_PRINT("Broadcast_ID failed:send message type invalid!\n");
        return BROADCASTNAME_SEND_MSG_TYPE_ERROR;
    }
    return BROADCASTNAME_NO_ERROR; 
}
)";
    // subscribe callback define
    std::string c_r5_broadcast_define = R"(
enum ERROR_NAME BroadcastID(ARGS,sint32 timeout)
{
    if (session_id < 0) 
    {
        LOG(DBG_INFO,"IpcLibRecvApp", "Broadcast_ID failed:session id is lower than zero");
        return -session_id;
    }
    struct msg_package send_msg;
    send_msg.cmd=CMD_NUMBER;
    send_msg.payload_size = (ARGS_SIZE-1)/8+1;
    if(send_msg.payload_size>7)
    {
        LOG(DBG_INFO,"IpcLibRecvApp",  "date overflow,date size should not be more than 7 byte!");
        return BROADCASTNAME_MSG_SIZE_OVERFLOW_ERROR;
    }
    COPY_OUT_ARGS_TO_PAYLOAD

    sint32 ret = IPC_send_Package(session_id, &send_msg, MSG_TYPE_SIGNAL, 0 , timeout);
    if(ret==-SEND_MSG_SESSION_INVALID)
    {
        LOG(DBG_INFO,"IpcLibRecvApp", "Broadcast_ID failed:session id overflow!");
        return BROADCASTNAME_SESSION_ID_OVERFLOW_ERROR;
    }
    if(ret==-SEND_MSG_TYPE_INVALID)
    {
        LOG(DBG_INFO,"IpcLibRecvApp", "Broadcast_ID failed:send message type invalid!");
        return BROADCASTNAME_SEND_MSG_TYPE_ERROR;
    }
    return BROADCASTNAME_NO_ERROR; 
}
)";
    // method declaration
    std::string c_r5_method_defination = "\nenum ERROR_NAME METHODNAME(ARGS)\n{\n}";

    std::string m_destDir;
    std::string prefix;

public:
    std::string generator(const std::shared_ptr<FDInterface> &fdInterface);
    static IPCServerSourceGenerator &getInstance()
    {
        static IPCServerSourceGenerator instance;
        return instance;
    }
    void generateServerSource(const std::shared_ptr<FDInterface> &fdInterface, const std::string &dir);
    bool generate() override
    {
        return true;
    }
    std::string buildMethodCmdList(const std::shared_ptr<FDInterface> &fdInterface);
    std::string getSourceFile(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection);
    std::string getSourcePath(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection);
    std::string generateIncludeHeader(const std::shared_ptr<FDInterface> &fdInterface);
    std::string generateMethodsInterface(const std::shared_ptr<FDInterface> &fdInterface);
    // A55
    std::string copyOutArgsToPayload(std::list<std::shared_ptr<FArgument>> out_args, std::string arg_prefix);
    std::string copyPayloadToInArgs(std::list<std::shared_ptr<FArgument>> in_args, std::string method_name);
    std::string copyPayloadToInArgs(std::list<std::shared_ptr<FArgument>> in_args);
    // R5
    std::string copyR5OutArgsToPayload(std::list<std::shared_ptr<FArgument>> out_args, std::string arg_prefix);
    std::string copyR5PayloadToInArgs(std::list<std::shared_ptr<FArgument>> in_args, std::string method_name);

    std::string generateMethodInterface(const std::shared_ptr<FMethod> &method, int id_number);
    std::string generateBroadcastsInterface(const std::shared_ptr<FDInterface> &fdInterface);
    std::string generateBroadcastInterface(const std::shared_ptr<FBroadcast> &broadcast, int id_number);
    std::string getR5MethodDefination(const std::shared_ptr<FDInterface> &fdInterface);

    std::string getPrefix() const
    {
        return prefix;
    }
};

} // namespace BstIdl
#endif