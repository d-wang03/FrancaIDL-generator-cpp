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
#ifndef IPC_CLIENT_SOURCE_GEN_H
#define IPC_CLIENT_SOURCE_GEN_H
#include "ipcCommonItemGen.h"
#include "model/FFactory.h"
#include "model/FModelManager.h"
#include <fstream>
#include <iostream>
namespace BstIdl
{
class IPCClientSourceGenerator : public AbstractGenerator
{
private:
    IPCClientSourceGenerator() = default;
    ~IPCClientSourceGenerator() = default;
    IPCClientSourceGenerator(const IPCClientSourceGenerator &) = delete;
    IPCClientSourceGenerator(IPCClientSourceGenerator &&) noexcept = delete;
    IPCClientSourceGenerator &operator=(const IPCClientSourceGenerator &) = delete;
    IPCClientSourceGenerator &operator=(IPCClientSourceGenerator &&) = delete;
    void setPrefix(const std::string &common_prefix)
    {
        prefix = common_prefix;
    }
    std::string c_include_header = R"(
#include "NAMEClient.h"
int32_t session_id=0;
static const enum ipc_core_e client = (enum ipc_core_e)CLIENT_NUMBER;
static const enum ipc_core_e server = (enum ipc_core_e)SERVER_NUMBER;

)";
    // ipc init
    std::string c_ipc_init_define = R"(
int32_t ipc_init()
{
    session_id = IPC_Register(server, client);
    if (session_id == -REGISTER_IPC_OPEN_NODE_FAILED)
    {
        printf("IPC_register ipc node failed!\n");
        return -IPC_REGISTER_OPEN_NODE_ERROR;
    }
    if (session_id == -REGISTER_IPC_REQUEST_SESSION_FAIL)
    {
        printf("IPC_register ipc node failed!\n");
        return -IPC_REGISTER_REQUEST_SESSION_ERROR;
    }
    if (session_id == -REGISTER_IPC_DEPENDENCY_FAILED)
    {
        printf("IPC_register ipc node failed!\n");
        return -IPC_REGISTER_CREAT_RECV_PROCESS_ERROR;
    }
    return session_id;
}
)";
    std::string c_ipc_deinit_define = R"(
int32_t ipc_deinit()
{
    int32_t ret = IPC_Unregister(session_id);
    if (ret == -UNREGISTER_IPC_FAILED)
    {
        printf("IPC_Unregister failed!\n");
        return -IPC_DEINIT_ERROR;
    }
    return ret;
}
)";
    // Synchronous call method define
    std::string c_sync_method_define = R"(
enum ERROR_NAME METHODID_SyncCall(ARGS , int32_t timeout)
{
    if (session_id < 0) 
    {
        printf("IPC_register  session id is lower than zero");
        return -session_id;
    }
    struct msg_package send_msg;   
    struct msg_package re_msg;
    send_msg.cmd= METHOD_CMD;
    send_msg.long_param=1;
    send_msg.payload_size=(SIZE_OF_ARGS-1)/8+1;
    if(send_msg.payload_size > 7)
    {
        printf("date overflow,date size should not be more than 7 byte!\n");
        return METHODID_MSG_SIZE_OVERFLOW_ERROR;
    }
    COPY_INARGS_TO_PAYLOAD
    int32_t ret =Sync_Call_Package(session_id,&send_msg,&re_msg,timeout);
    if(ret==-SYNC_CALL_SESSION_INVALID)
    {
        printf("call Sync_Call_Package failed:session id overflow!\n");
        return METHODID_SESSION_ID_OVERFLOW_ERROR;
    }
    if(ret==-SYNC_CALL_TIMEOUT_OUTRANGED)
    {
        printf("call Sync_Call_Package failed:timeout range overflow!\n");
        return METHODID_TIMEOUT_RANGE_OVERFLOW_ERROR;
    }
    if(ret==-SYNC_CALL_MSG_OVERSIZED)
    {
        printf("call Sync_Call_Package failed:payload size range overflow!\n");
        return METHODID_PAYLOAD_SIZE_RANGE_OVERFLOW_ERROR;
    }
    if(ret==-SYNC_CALL_SEND_FAILED)
    {
        printf("call Sync_Call_Package failed:timeout range overflow!\n");
        return METHODID_SYNC_CALL_SEED_MSG_ERROR;
    }
    if(ret==-SYNC_CALL_TIMEOUT)
    {
        printf("call Sync_Call_Package failed:receive message timeout !\n");
        return METHODID_SYNC_CALL_TIMEOUT_ERROR;
    }
    COPY_PAYLOAD_TO_OUTARGS
    return METHODID_NO_ERROR;
}
)";
    // callback pointer deFINE
    std::string c_callback_pointer = "ID_ASyncCallback_t ID_async_callback=NULL;\n";
    // Asynchronous call method register
    std::string c_async_method_register_define = R"(
enum ERROR_NAME METHODID_Async_Register(ARGS, MethodID_ASyncCallback_t callback)
{ 
    if (session_id < 0)
    {
        printf("IPC_register session id is lower than zero");
        return -session_id;
    }

    if (!callback)
    {
        printf("callback is null");
        return METHODID_CALLBACK_PTR_ERROR;
    }
    METHODID_async_callback = callback;
    
    METHODID_send_msg.cmd = METHOD_CMD;
    METHODID_send_msg.payload_size = (SIZE_OF_ARGS-1)/8+1;
    METHODID_send_msg.long_param = 1;
    if (METHODID_send_msg.payload_size > 7)
    {
        printf("date overflow,date size should not be more than 56 byte!\n");
        return METHODID_MSG_SIZE_OVERFLOW_ERROR;
    }
    COPY_INARGS_TO_PAYLOAD
    return METHODID_NO_ERROR;
}
)";
    std::string c_async_method_send_msg_declaration = "static struct msg_package METHODID_send_msg;\n";
    // Asynchronous call method define
    std::string c_async_method_define = R"(
enum ERROR_NAME METHODID_ASyncCall()
{
    int32_t ret = Async_Call_Package(session_id, &METHODID_send_msg, true);
    if (ret == -ASYNC_CALL_SESSION_INVALID)
    {
        printf("call Async_Call_Package failed: session id overflow !\n");
        return METHODID_SESSION_ID_OVERFLOW_ERROR;
    }
    if (ret == -ASYNC_CALL_MSG_OVERSIZED)
    {
        printf("call Async_Call_Package failed: message size overflow !\n");
        return METHODID_MSG_SIZE_OVERFLOW_ERROR;
    }
    if (ret == -ASYNC_CALL_SEND_FAILED)
    { 
        printf("call Async_Call_Package failed:send message failed!\n");
        return METHODID_ASYNC_CALL_SEED_MSG_ERROR;
    }
    return METHODID_NO_ERROR;
}
)";
    // client callback process define
    std::string c_client_callback_process_define = R"(
int32_t callback_process(struct msg_package *re_msg, ipc_msg_type type, uint16_t token)
{
    if (type == MSG_TYPE_REPLY)
    {
        return reply_method_process(re_msg);
    }
    else if (type == MSG_TYPE_SIGNAL)
    {
        return reply_broadcast_process(re_msg);
    }
    else
    {
        printf("Wrong async_func_calllback reply type!\n");
        return REPLY_MSG_TYPE_ERROR;
    }
}
)";
    // reply method process define
    std::string c_reply_method_process_define = R"(
int32_t reply_method_process(struct msg_package *re_msg)
{ 
    switch(re_msg->cmd)
    {
        CASE_SEGMENT
    default:
        printf("method command id is invalid!");
        return METHOD_CMD_INVALID_ERROR;
    }
    return NO_ERROR;
}
)";
    // reply method process define
    std::string c_reply_broadcast_process_define = R"(
int32_t reply_broadcast_process(struct msg_package *re_msg)
{ 
    switch(re_msg->cmd)
    {
    CASE_SEGMENT
    default:
        printf("broadcast command id is invalid!");
        return BROADCAST_CMD_INVALID_ERROR;
    }
    return NO_ERROR;
}
)";
    // case segment
    std::string c_case_segment = R"(
    case CMD_NUMBER:
        if(METHODID_async_callback==NULL)
        {
            printf("METHODID_async_callback is nullptr!\n");
            return CALLBACK_PTR_ERROR;
        }
        OUT_ARGS_DEFINE_SEGMENT
        COPY_PAYLOAD_TO_OUTARGS
        METHODID_async_callback(OUTARGS);
        break;
)";
    std::string method_case_segment_string = "";
    std::string broadcast_case_segment_string = "";
    // subscribe callback define
    std::string c_subscribe_callback_define = R"(
int32_t subscribe_callback()
{
    if(Register_Callback_Package_Process(session_id,callback_process)!=0)
        return SUBSCRIBE_CALLBACK_ERROR;
    return NO_ERROR;
}
)";
    // fire and forget method define
    std::string c_fire_and_forget_define = R"(
enum ERROR_NAME METHODID_Call(ARGS)
{
    if (session_id < 0) 
    {
        printf("IPC_register  session id is lower than zero");
        return -session_id;
    }
    struct msg_package send_msg;
    send_msg.cmd = METHOD_CMD;
    send_msg.payload_size=(SIZE_OF_ARGS-1)/8+1;
    send_msg.long_param=0;
    if(send_msg.payload_size>7)
    {
        printf("date overflow,date size should not be more than 7 byte!\n");
        return METHODID_MSG_SIZE_OVERFLOW_ERROR;
    }
    COPY_INARGS_TO_PAYLOAD
    int32_t ret=Async_Call_Package(session_id,&send_msg,true);
    if(ret==-ASYNC_CALL_SESSION_INVALID)
    {
        printf("call Async_Call_Package failed:session id overflow!\n");
        return METHODID_SESSION_ID_OVERFLOW_ERROR;
    }
    if(ret==-ASYNC_CALL_MSG_OVERSIZED)
    {
        printf("call Async_Call_Package failed:message size overflow!\n");
        return METHODID_MSG_SIZE_OVERFLOW_ERROR;
    }
    if(ret==-ASYNC_CALL_SEND_FAILED)
    {
        printf("call Async_Call_Package failed:send message failed!\n");
        return METHODID_ASYNC_CALL_SEED_MSG_ERROR;
    }
    return METHODID_NO_ERROR;
}
)";
    // broadcast subscribe method define
    std::string c_broadcast_subscribe_define = R"(
enum ERROR_NAME BROADCASTID_Subscribe(BroadcastID_ASyncCallback_t callback)
{
    if (session_id < 0) 
    {
        printf("IPC_register  session id is lower than zero");
        return -session_id;
    }
    if(!callback)
    {
        printf("callback is null");
        return BROADCASTID_CALLBACK_PTR_ERROR;
    }
    BROADCASTID_async_callback=callback;
    int32_t ret=Subscribe_Signal(session_id,CMD_NUMBER);
    if(ret==-SUBSCRIBE_SIGNAL_SESSION_INVALID)
    {
        printf("Subscribe Signal failed:session id overflow!\n");
        return BROADCASTID_SESSION_ID_OVERFLOW_ERROR;
    }
    if(ret==-SUBSCRIBE_SIGNAL_CMD_INVALID)
    {
        printf("Subscribe Signal failed:command id is out of range!\n");
        return BROADCASTID_CMD_ID_OVERFLOW_ERROR;
    }
    if(ret==-SUBSCRIBE_SIGNAL_FAILED)
    {
        printf("Subscribe Signal failed!\n");
        return BROADCASTID_SUBSCRIBE_SIGNAL_ERROR;
    }
    return BROADCASTID_NO_ERROR;
}
)";

    std::string m_destDir;
    std::string prefix;

public:
    std::string generator(const std::shared_ptr<FDInterface> &fdInterface);
    std::string generateIncludeHeader(const std::shared_ptr<FDInterface> &fdInterface);
    std::string generateMethodsInterface(const std::shared_ptr<FDInterface> &fdInterface);
    std::string copyInArgs(std::list<std::shared_ptr<FArgument>> in_args);
    std::string copyOutArgs(std::list<std::shared_ptr<FArgument>> out_args);
    std::string copyOutArgs_1(std::list<std::shared_ptr<FArgument>> out_args);
    std::string generateMethodInterface(const std::shared_ptr<FMethod> &method, int id_number);
    std::string generateBroadcastsInterface(const std::shared_ptr<FDInterface> &fdInterface);
    std::string generateBroadcastInterface(const std::shared_ptr<FBroadcast> &broadcast, int id_number);
    static IPCClientSourceGenerator &getInstance()
    {
        static IPCClientSourceGenerator instance;
        return instance;
    }
    void generateClientSource(const std::shared_ptr<FDInterface> &fdInterface, const std::string &dir);
    bool generate() override
    {
        return true;
    }
    std::string getSourceFile(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection);
    std::string getSourcePath(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection);
    std::string getPrefix() const
    {
        return prefix;
    }
};

} // namespace BstIdl
#endif