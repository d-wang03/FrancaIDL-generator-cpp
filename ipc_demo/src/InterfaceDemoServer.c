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
#include "InterfaceDemoServer.h"
static const enum ipc_core_e client = (enum ipc_core_e)5;
static const enum ipc_core_e server = (enum ipc_core_e)6;

int32_t ipc_init()
{
    session_id = IPC_Register(client, server);
    printf("session_id=%d", session_id);
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

int32_t ipc_deinit()
{
    printf("server ipc_deinit\n");
    int32_t ret = IPC_Unregister(session_id);
    if (ret == -UNREGISTER_IPC_FAILED)
    {
        printf("IPC_Unregister failed!\n");
        return -IPC_DEINIT_ERROR;
    }
    printf("server ipc_deinit success\n");
    return ret;
}

int32_t subscribe_callback()
{
    if (Register_Callback_Package_Process(session_id, callback_process) != 0)
        return REGISTER_CALLBACK_ERROR;
    return NO_ERROR;
}

enum IDA_error IDA(const uint8_t argIDA, const float argIDB, double *argIDC, bool *argIDD)
{
}

enum IDB_error IDB(const uint8_t argIDA, const float argIDB)
{
}

int32_t reply_method_process(struct msg_package *in_msg, uint16_t token)
{
    struct msg_package send_msg;
    send_msg.cmd = in_msg->cmd;
    switch (in_msg->cmd)
    {

    case 0:
    {    uint8_t IDA_argIDA;
        float IDA_argIDB;
        double IDA_argIDC;
        bool IDA_argIDD;

        memcpy(&IDA_argIDA, in_msg->payload, sizeof(IDA_argIDA));
        memcpy(&IDA_argIDB, in_msg->payload + sizeof(IDA_argIDA), sizeof(IDA_argIDB));

        IDA(IDA_argIDA, IDA_argIDB, &IDA_argIDC, &IDA_argIDD);
        send_msg.payload_size = (sizeof(double) + sizeof(bool) - 1) / 8 + 1;
        if (send_msg.payload_size > 7)
        {
            printf("date overflow,date size should not be more than 56 byte!\n");
            return MSG_SIZE_OVERFLOW_ERROR;
        }
        memcpy(send_msg.payload, &IDA_argIDC, sizeof(IDA_argIDC));
        memcpy(send_msg.payload + sizeof(IDA_argIDC), &IDA_argIDD, sizeof(IDA_argIDD));

        break;
    }
    case 1:
    {
        uint8_t IDB_argIDA;
        float IDB_argIDB;

        memcpy(&IDB_argIDA, in_msg->payload, sizeof(IDB_argIDA));
        memcpy(&IDB_argIDB, in_msg->payload + sizeof(IDB_argIDA), sizeof(IDB_argIDB));

        IDB(IDB_argIDA, IDB_argIDB);
        break;
    }
    default:
        return METHOD_CMD_INVALID_ERROR;
    }
    if (in_msg->long_param == 1)
    {
        int32_t ret = IPC_send_Package(session_id, &send_msg, MSG_TYPE_REPLY, token, -1);
        if (ret == -SEND_MSG_SESSION_INVALID)
        {
            printf("reply method process:session id overflow!\n");
            return SESSION_ID_OVERFLOW_ERROR;
        }
        if (ret == -SEND_MSG_TYPE_INVALID)
        {
            printf("reply method process:send message type invalid!\n");
            return SEND_MSG_TYPE_INVALID_ERROR;
        }
    }
    return NO_ERROR;
}

int32_t message_process()
{
    if (session_id < 0)
    {
        printf("IPC_register  session id is lower than zero");
        return -session_id;
    }
    for (int8_t i = 0; i < sizeof(method_cmd_list); i++)
    {
        int32_t ret = Register_Method(session_id, method_cmd_list[i]);
        if (ret == -REGISTER_METHOD_SESSION_INVALID)
        {
            printf("Register method failed:session id overflow!\n");
            return SESSION_ID_OVERFLOW_ERROR;
        }
        if (ret == -REGISTER_METHOD_CMD_INVALID)
        {
            printf("Register method failed:command id is out of range!\n");
            return CMD_ID_OVERFLOW_ERROR;
        }
        if (ret == -REGISTER_METHOD_FAILED)
        {
            printf("Register method failed!\n");
            return REGISTER_METHOD_ERROR;
        }
    }
    int32_t ret = subscribe_callback();
    if (ret == REGISTER_CALLBACK_ERROR)
        return ret;
    return NO_ERROR;
}

int32_t callback_process(struct msg_package *re_msg, ipc_msg_type type, uint16_t token)
{
    if (type == MSG_TYPE_METHOD)
    {
        return reply_method_process(re_msg, token);
    }
    else
    {
        printf("Wrong async_func_calllback reply type!\n");
        return REPLY_MSG_TYPE_ERROR;
    }
}

enum ID_error ID(double argIDA, bool argIDB, int32_t timeout)
{
    if (session_id < 0)
    {
        printf("Broadcast_ID failed:session id is lower than zero");
        return -session_id;
    }
    struct msg_package send_msg;
    send_msg.cmd = 0;
    send_msg.payload_size = (sizeof(double) + sizeof(bool) - 1) / 8 + 1;
    if (send_msg.payload_size > 7)
    {
        printf("date overflow,date size should not be more than 7 byte!\n");
        return ID_MSG_SIZE_OVERFLOW_ERROR;
    }
    memcpy(send_msg.payload, &argIDA, sizeof(argIDA));
    memcpy(send_msg.payload + sizeof(argIDA), &argIDB, sizeof(argIDB));

    int32_t ret = IPC_send_Package(session_id, &send_msg, MSG_TYPE_SIGNAL, 0, timeout);
    if (ret == -SEND_MSG_SESSION_INVALID)
    {
        printf("Broadcast_ID failed:session id overflow!\n");
        return ID_SESSION_ID_OVERFLOW_ERROR;
    }
    if (ret == -SEND_MSG_TYPE_INVALID)
    {
        printf("Broadcast_ID failed:send message type invalid!\n");
        return ID_SEND_MSG_TYPE_ERROR;
    }
    return ID_NO_ERROR;
}
