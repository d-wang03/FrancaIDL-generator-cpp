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
#ifndef __IPC_H_
#define __IPC_H_

#include "user_head.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{

#endif
    // error number definition
    enum ipc_error_num_e
    {
        REGISTER_IPC_OPEN_NODE_FAILED = 1,
        REGISTER_IPC_REQUEST_SESSION_FAIL,
        REGISTER_IPC_DEPENDENCY_FAILED,
        REGISTER_CALLBACK_SESSION_INVALID,
        SUBSCRIBE_SIGNAL_SESSION_INVALID,
        SUBSCRIBE_SIGNAL_CMD_INVALID,
        SUBSCRIBE_SIGNAL_FAILED,
        REGISTER_METHOD_SESSION_INVALID,
        REGISTER_METHOD_CMD_INVALID,
        REGISTER_METHOD_FAILED,
        UNREGISTER_IPC_FAILED,
        SEND_MSG_SESSION_INVALID,
        SEND_MSG_TYPE_INVALID,
        SEND_MSG_OVERSIZED,
        SEND_MSG_FAILED,
        SYNC_CALL_SESSION_INVALID,
        SYNC_CALL_TIMEOUT_OUTRANGED,
        SYNC_CALL_MSG_OVERSIZED,
        SYNC_CALL_SEND_FAILED,
        SYNC_CALL_TIMEOUT,
        ASYNC_CALL_SESSION_INVALID,
        ASYNC_CALL_SEND_FAILED,
        ASYNC_CALL_MSG_OVERSIZED,
    };

    // structure definition
    enum ipc_core_e
    { /* 5 bits */
        IPC_CORE_ARM0,
        IPC_CORE_ARM1,
        IPC_CORE_ARM2,
        IPC_CORE_ARM3,
        IPC_CORE_ARM4,
        IPC_CORE_ARM5,
        IPC_CORE_ARM6,
        IPC_CORE_ARM7,
        IPC_CORE_R5_0 = 8,
        IPC_CORE_R5_1 = 9,
        IPC_CORE_DSP_0 = 10,
        IPC_CORE_DSP_1 = 11,
        IPC_CORE_DSP_2 = 12,
        IPC_CORE_DSP_3 = 13,
        IPC_CORE_ISP = 14,
        IPC_CORE_VSP = 15,
        IPC_CORE_DSP = 16,
        IPC_CORE_SECURE = 17,
        IPC_CORE_MAX,
    };

#define MAX_PACKAGE_PAYLOAD_SIZE 7

    /**
     * struct msg_type - struct description of send message type
     *
     * this struct is used for user to fill up send message
     *
     * @cmd:    cmd type, determined by commucicating parties, max count is 256
     * @long_param:		message content, determined by communicating parties, message max size is 4 bytes
     */
    struct msg_type
    {

        uint8_t cmd;         // cmd type param:
        uint32_t long_param; // valid address list number
#ifdef MSG_SIZE_EXTENSION
        uint64_t long_data;
#endif
    };

    // second version of message type
    /**
     * struct msg_package - struct description of package ipc message type
     *
     * this struct is used for user to fill up ipc package message
     *
     * @cmd:    cmd type, determined by communication parties, max count is 256
     * @payload_size:	payload size, max count is 7
     * @long_param: compatible with previous ipc message type
     * @payload: package message payload, this message volume is 64bit * 7 = 56 bytes, you should pay attention to align
     * this payload.
     */
    struct msg_package
    {
        uint8_t cmd;
        uint8_t payload_size;
        uint32_t long_param;
        uint64_t payload[7];
    };

    typedef enum
    {
        MSG_TYPE_METHOD = 1,
        MSG_TYPE_REPLY = 2,
        MSG_TYPE_SIGNAL = 3,
        MSG_TYPE_MAX,
    } ipc_msg_type;

    /**
     * recv_func_package_cb: function type for async's receive callback
     *
     * @replay_msg: The message received.
     */
    typedef int (*recv_func_package_cb)(struct msg_package *recv_msg, ipc_msg_type type, uint16_t token);

    // API definition
    /**
     * IPC_Register: unregister to ipc_service
     * RETURN: session id
     */
    int IPC_Register(enum ipc_core_e dst, enum ipc_core_e src);

    /**
     * IPC_Unregister: unregister to ipc_service
     * RETURN: 0 for success, negative value for errors.
     */
    int IPC_Unregister(int32_t session_id);

    /**
     * Send IPC message.
     * @session_id: the session used to send message. get from IPC_Register.
     * @msg: the message to be sent.
     * @timeout: no important
     * @return 0 for success, negative value for errors.
     */
    int IPC_send(int32_t session_id, struct msg_type *msg, ipc_msg_type type, int32_t timeout);

    /**
     * MethodCall: Sync_Call
     *
     * @session_id:     session id
     * @send_msg:     fill out struct msg_type, it will describe send message cmd and parameter
     * @recv_msg:  return message struct
     * @timeout: synchronous call timeout, max timeout is 5s
     *           call this function will send message and wait for server's reply,
     *           if server do not reply in timeout peroid, this call will return null reply and return -1
     *
     * RETURN: 0 for success, negative value for errors.
     */
    int Sync_Call(int32_t session_id, struct msg_type *send_msg, struct msg_type *recv_msg, int32_t timeout);

    /**
     * MethodCall: Async_Call
     *
     * @session_id:     session id
     * @async_msg:     fill out struct msg_type, it will describe send message cmd and parameter
     * @callback:
     * RETURN: 0 for success, negative value for errors.
     */
    int Async_Call(int32_t session_id, struct msg_type *send_msg);

    // package message API
    // int Register_Callback_Process(int32_t session_id, recv_func_cb recv_process); //recv msg from remote entrance
    int Register_Callback_Package_Process(int32_t session_id, recv_func_package_cb recv_process);

    int Sync_Call_Package(int32_t session_id, struct msg_package *send_msg, struct msg_package *recv_msg,
                          int32_t timeout);                                                    // client
    int Async_Call_Package(int32_t session_id, struct msg_package *send_msg, bool reply_flag); // client
    int IPC_send_Package(int32_t session_id, struct msg_package *send_msg, ipc_msg_type type, uint16_t token,
                         int32_t timeout);

    // api for client
    int Subscribe_Signal(int32_t session_id, uint32_t cmd); // client
    // api for server
    int Register_Method(int32_t session_id, uint32_t cmd); // server

#ifdef __cplusplus
}
#endif

#endif
