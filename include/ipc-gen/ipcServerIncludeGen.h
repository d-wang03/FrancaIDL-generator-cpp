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
#ifndef IPC_SERVER_INCLUDE_GEN_H
#define IPC_SERVER_INCLUDE_GEN_H
#include "ipcCommonItemGen.h"
#include "model/FFactory.h"
#include "model/FModelManager.h"
#include <fstream>
#include <iostream>
namespace BstIdl
{
class IPCServerIncludeGenerator : public AbstractGenerator
{
private:
    IPCServerIncludeGenerator() = default;
    ~IPCServerIncludeGenerator() = default;
    IPCServerIncludeGenerator(const IPCServerIncludeGenerator &) = delete;
    IPCServerIncludeGenerator(IPCServerIncludeGenerator &&) noexcept = delete;
    IPCServerIncludeGenerator &operator=(const IPCServerIncludeGenerator &) = delete;
    IPCServerIncludeGenerator &operator=(IPCServerIncludeGenerator &&) = delete;
    void setPrefix(const std::string &common_prefix)
    {
        prefix = common_prefix;
    }
    // method common error enumration
    std::string c_method_common_error = R"(
enum method_common_error
{
    NO_ERROR,
    IPC_REGISTER_OPEN_NODE_ERROR,
    IPC_REGISTER_REQUEST_SESSION_ERROR,
    IPC_REGISTER_CREAT_RECV_PROCESS_ERROR,
    IPC_DEINIT_ERROR, 
    MSG_SIZE_OVERFLOW_ERROR,
    REGISTER_CALLBACK_ERROR,
    SESSION_ID_OVERFLOW_ERROR,
    CMD_ID_OVERFLOW_ERROR,
    REGISTER_METHOD_ERROR,
    REPLY_MSG_TYPE_ERROR,
    METHOD_CMD_INVALID_ERROR,
    SEND_MSG_TYPE_INVALID_ERROR
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
    BROADCASTID_MSG_SIZE_OVERFLOW_ERROR,
    BROADCASTID_SESSION_ID_OVERFLOW_ERROR,
    BROADCASTID_SEND_MSG_TYPE_ERROR
};
  )";
    // method common error enumration
    std::string c_method_error = R"(
enum METHODID_error
{
    METHODID_NO_ERROR,
    METHODID_IPC_REGISTER_OPEN_NODE_ERROR,
    METHODID_IPC_REGISTER_REQUEST_SESSION_ERROR,
    METHODID_IPC_REGISTER_CREAT_RECV_PROCESS_ERROR,
    METHODID_IPC_DEINIT_ERROR, 
    METHODID_MSG_SIZE_OVERFLOW_ERROR,
    METHODID_REGISTER_CALLBACK_ERROR,
    METHODID_SESSION_ID_OVERFLOW_ERROR,
    METHODID_CMD_ID_OVERFLOW_ERROR,
    METHODID_REGISTER_METHOD_ERROR,
    METHODID_REPLY_MSG_TYPE_ERROR,
    METHODID_METHOD_CMD_INVALID_ERROR,
    METHODID_SEND_MSG_TYPE_INVALID_ERROR,
    OTHERS
};
  )";
    // method declaration
    std::string c_method_declaration = "enum ERROR_NAME METHODNAME(ARGS);\n";
    // broadcast declaration
    std::string c_broadcast_declaration = "enum ERROR_NAME METHODNAME(ARGS,int32_t timeout);\n";
    // message process declaration
    std::string c_message_process_declaration = "int32_t message_process();\n";
    // subscribe callback declaration
    std::string c_subscribe_callback_declaration = "int32_t subscribe_callback();\n";
    // callback process declaration
    std::string c_callback_process_declaration =
        "int32_t callback_process(struct msg_package *re_msg, ipc_msg_type type, uint16_t token);\n";
    // reply method process declaration
    std::string c_reply_method_process_declaration =
        "int32_t reply_method_process(struct msg_package *in_msg, uint16_t token);\n";

    // set server  r5_
    std::string c_r5_subscribe_callback_declaration = "int subscribe_callback();\n";
    std::string c_r5_reply_method_process_declaration =
        "sint32 reply_method_process(struct msg_package *in_msg, uint16 token);\n";
    std::string c_r5_broadcast_declaration = "enum ERROR_NAME METHODNAME(ARGS,sint32 timeout);\n";

    // std::string server_type;

    std::string m_destDir;
    std::string prefix;

public:
    std::string generateMethodErrorEnum(const std::shared_ptr<FMethod> &method);
    std::string generateBroadcastErrorEnum(const std::shared_ptr<FBroadcast> &broadcast);
    std::string generateMethodsInterface(const std::shared_ptr<FDInterface> &fdInterface);
    std::string generateMethodInterface(const std::shared_ptr<FMethod> &method);
    std::string generateBroadcastsInterface(const std::shared_ptr<FDInterface> &fdInterface);
    std::string generateBroadcastInterface(const std::shared_ptr<FBroadcast> &broadcast);
    std::string generateErrorEnum(const std::shared_ptr<FDInterface> &fdInterface);
    std::string generator(const std::shared_ptr<FDInterface> &fdInterface);
    static IPCServerIncludeGenerator &getInstance()
    {
        static IPCServerIncludeGenerator instance;
        return instance;
    }
    void generateServerInclude(const std::shared_ptr<FDInterface> &fdInterface, const std::string &dir);
    bool generate() override
    {
        return true;
    }
    std::string getIncludeFile(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection);
    std::string getIncludePath(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection);
    std::string getPrefix() const
    {
        return prefix;
    }
};

} // namespace BstIdl
#endif