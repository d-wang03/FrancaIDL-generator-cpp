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
#ifndef IPC_CLIENT_INCLUDE_GEN_H
#define IPC_CLIENT_INCLUDE_GEN_H
#include "ipcCommonItemGen.h"
#include "model/FFactory.h"
#include "model/FModelManager.h"
#include <fstream>
#include <iostream>
namespace BstIdl
{
class IPCClientIncludeGenerator : public AbstractGenerator
{
private:
    IPCClientIncludeGenerator() = default;
    ~IPCClientIncludeGenerator() = default;
    IPCClientIncludeGenerator(const IPCClientIncludeGenerator &) = delete;
    IPCClientIncludeGenerator(IPCClientIncludeGenerator &&) noexcept = delete;
    IPCClientIncludeGenerator &operator=(const IPCClientIncludeGenerator &) = delete;
    IPCClientIncludeGenerator &operator=(IPCClientIncludeGenerator &&) = delete;

    void setPrefix(const std::string &common_prefix)
    {
        prefix = common_prefix;
    }
    // common error enumration
    std::string c_common_error = R"(
enum common_error
{
    NO_ERROR,
    IPC_REGISTER_OPEN_NODE_ERROR,
    IPC_REGISTER_REQUEST_SESSION_ERROR,
    IPC_REGISTER_CREAT_RECV_PROCESS_ERROR,
    IPC_DEINIT_ERROR, 
    REPLY_MSG_TYPE_ERROR,
    CALLBACK_PTR_ERROR,
    SUBSCRIBE_CALLBACK_ERROR,
    BROADCAST_CMD_INVALID_ERROR,
    METHOD_CMD_INVALID_ERROR
};
  )";
    // Asynchronous method and Synchronous method error enumration
    std::string c_method_error = R"(
enum METHODID_error
{
    METHODID_NO_ERROR,
    METHODID_IPC_REGISTER_OPEN_NODE_ERROR,
    METHODID_IPC_REGISTER_REQUEST_SESSION_ERROR,
    METHODID_IPC_REGISTER_CREAT_RECV_PROCESS_ERROR,
    METHODID_IPC_DEINIT_ERROR,
    METHODID_REPLY_MSG_TYPE_ERROR,
    METHODID_CALLBACK_PTR_ERROR,
    METHODID_SUBSCRIBE_CALLBACK_ERROR,
    METHODID_MSG_SIZE_OVERFLOW_ERROR,
    METHODID_METHOD_CMD_INVALID_ERROR,
    METHODID_SESSION_ID_OVERFLOW_ERROR,
    METHODID_ASYNC_CALL_SEED_MSG_ERROR,
    METHODID_TIMEOUT_RANGE_OVERFLOW_ERROR,
    METHODID_PAYLOAD_SIZE_RANGE_OVERFLOW_ERROR,
    METHODID_SYNC_CALL_SEED_MSG_ERROR,
    METHODID_SYNC_CALL_TIMEOUT_ERROR,
    OTHERS
};
)";
    // Asynchronous method error enumration
    std::string c_async_method_error = R"(
enum METHODID_error
{
    ASYNC_METHODID_NO_ERROR,
    ASYNC_METHODID_IPC_REGISTER_OPEN_NODE_ERROR,
    ASYNC_METHODID_IPC_REGISTER_REQUEST_SESSION_ERROR,
    ASYNC_METHODID_IPC_REGISTER_CREAT_RECV_PROCESS_ERROR,
    ASYNC_METHODID_IPC_DEINIT_ERROR,
    ASYNC_METHODID_MSG_SIZE_OVERFLOW_ERROR,
    ASYNC_METHODID_SESSION_ID_OVERFLOW_ERROR,
    ASYNC_METHODID_ASYNC_CALL_SEED_MSG_ERROR,
    OTHERS
};
)";
    // Synchronous method error enumration
    std::string c_sync_method_error = R"(
enum METHODID_error
{
    SYNC_METHODID_NO_ERROR,
    SYNC_METHODID_IPC_REGISTER_OPEN_NODE_ERROR,
    SYNC_METHODID_IPC_REGISTER_REQUEST_SESSION_ERROR,
    SYNC_METHODID_IPC_REGISTER_CREAT_RECV_PROCESS_ERROR,
    SYNC_METHODID_IPC_DEINIT_ERROR,
    SYNC_METHODID_REPLY_MSG_TYPE_ERROR,
    SYNC_METHODID_CALLBACK_PTR_ERROR,
    SYNC_METHODID_SUBSCRIBE_CALLBACK_ERROR,
    SYNC_METHODID_MSG_SIZE_OVERFLOW_ERROR,
    SYNC_METHODID_METHOD_CMD_INVALID_ERROR,
    SYNC_METHODID_SESSION_ID_OVERFLOW_ERROR,
    SYNC_METHODID_ASYNC_CALL_SEED_MSG_ERROR,
    SYNC_METHODID_TIMEOUT_RANGE_OVERFLOW_ERROR,
    SYNC_METHODID_PAYLOAD_SIZE_RANGE_OVERFLOW_ERROR,
    SYNC_METHODID_SYNC_CALL_SEED_MSG_ERROR,
    SYNC_METHODID_SYNC_CALL_TIMEOUT_ERROR,
    OTHERS
};
)";
    // fire and forget error enumration
    std::string c_fire_and_forget_error = R"(
enum METHODID_error
{
    METHODID_NO_ERROR,
    METHODID_IPC_REGISTER_OPEN_NODE_ERROR,
    METHODID_IPC_REGISTER_REQUEST_SESSION_ERROR,
    METHODID_IPC_REGISTER_CREAT_RECV_PROCESS_ERROR,
    METHODID_IPC_DEINIT_ERROR,
    METHODID_MSG_SIZE_OVERFLOW_ERROR,
    METHODID_SESSION_ID_OVERFLOW_ERROR,
    METHODID_ASYNC_CALL_SEED_MSG_ERROR,
    OTHERS
};
)";
    // broadcast error enumration
    std::string c_broadcast_error = R"(
enum BROADCASTID_error
{
    BROADCASTID_NO_ERROR,
    BROADCASTID_IPC_REGISTER_OPEN_NODE_ERROR,
    BROADCASTID_IPC_REGISTER_REQUEST_SESSION_ERROR,
    BROADCASTID_IPC_REGISTER_CREAT_RECV_PROCESS_ERROR,
    BROADCASTID_IPC_DEINIT_ERROR,
    BROADCASTID_REPLY_MSG_TYPE_ERROR,
    BROADCASTID_CALLBACK_PTR_ERROR,
    BROADCASTID_SUBSCRIBE_CALLBACK_ERROR,
    BROADCASTID_BROADCAST_CMD_INVALID_ERROR,
    BROADCASTID_SESSION_ID_OVERFLOW_ERROR,
    BROADCASTID_CMD_ID_OVERFLOW_ERROR,
    BROADCASTID_SUBSCRIBE_SIGNAL_ERROR,
};
)";
    // define callback:to define method or broadcast callback function pointer
    std::string c_callback_pointer_type_define = "typedef void (*METHODID_ASyncCallback_t)(ARGS);\n";
    // callback pointer declaration
    std::string c_callback_pointer = "extern ID_ASyncCallback_t ID_async_callback;\n";
    // Synchronous call method declaration
    std::string c_sync_method_declaration = "enum ERROR_NAME METHODID_SyncCall(ARGS,int32_t timeout);\n";
    // ASynchronous call method declaration
    std::string c_async_method_declaration = "enum ERROR_NAME METHODID_ASyncCall();\n";
    // fire and forget method declaration
    std::string c_fire_and_forget_declaration = "enum ERROR_NAME METHODID_Call(ARGS);\n";
    // broadcast subscribe method declaration
    std::string c_broadcast_subscribe_declaration = "enum ERROR_NAME %ID_Subscribe(%ID_ASyncCallback_t callback);\n";
    // subscribe callback declaration : to subscribe all of the asynchronous callback methods
    // reply method process declaration : when server returns message and message type equals MSG_TYPE_REPLY,this
    // function will be called by callback_process
    std::string c_async_process_declaration_block = R"(
int32_t subscribe_callback();
int32_t reply_method_process(struct msg_package *re_msg);
)";
    // Asynchronous call method register
    std::string c_async_method_register_declaration =
        "enum ERROR_NAME METHODID_Async_Register(ARGS, MethodID_ASyncCallback_t callback);\n";
    // reply method process declaration : when server returns message ,this function will be triggered
    std::string c_callback_process_declaration =
        "int32_t callback_process(struct msg_package *re_msg, ipc_msg_type type, uint16_t token);\n";
    // reply broadcast process declaration : when server returns message and message type equals MSG_TYPE_SIGNAL,this
    // function will be called by callback_process
    std::string c_reply_broadcast_process_declaration =
        "int32_t reply_broadcast_process(struct msg_package *re_msg);\n";
    std::string m_destDir;
    // prefix
    std::string prefix;

public:
    std::string generateBroadcastErrorEnum(const std::shared_ptr<FBroadcast> &broadcast);
    std::string generateMethodErrorEnum(const std::shared_ptr<FMethod> &method);
    std::string generateSyncMethodErrorEnum(const std::shared_ptr<FMethod> &method);
    std::string generateFireAndForgetMethodErrorEnum(const std::shared_ptr<FMethod> &method);
    std::string generateAsyncMethodErrorEnum(const std::shared_ptr<FMethod> &method);
    std::string generateMethodsInterface(const std::shared_ptr<FDInterface> &fdInterface);
    std::string generateMethodInterface(const std::shared_ptr<FMethod> &method);
    std::string generateBroadcastsInterface(const std::shared_ptr<FDInterface> &fdInterface);
    std::string generateBroadcastInterface(const std::shared_ptr<FBroadcast> &broadcast);
    std::string generateErrorEnum(const std::shared_ptr<FDInterface> &fdInterface);
    std::string generator(const std::shared_ptr<FDInterface> &fdInterface);
    void generateClientInclude(const std::shared_ptr<FDInterface> &fdInterface, const std::string &dir);
    std::string getIncludeFile(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection);
    std::string getIncludePath(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection);

    static IPCClientIncludeGenerator &getInstance()
    {
        static IPCClientIncludeGenerator instance;
        return instance;
    }
    bool generate() override
    {
        return true;
    }
    std::string getPrefix() const
    {
        return prefix;
    }
};

} // namespace BstIdl
#endif