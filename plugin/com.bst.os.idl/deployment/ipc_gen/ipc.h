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

#include "ipc_common.h"
#include "user_head.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{

#endif

    // API definition
    /**
     * ipcRegister: register to ipc service
     * @return: session id
     */
    int IPC_Register(enum ipc_core_e dst, enum ipc_core_e src);
    /**
     * ipcUnregister: unregister to ipc_service
     * @return: 0 for success, negative values for errors.
     */
    int IPC_Unregister(int32_t session_id);

    /**
     * MethodCall: ipcSend.
     * @session_id: session id is used to send message. get from ipcRegister.
     * @msg: send message
     * @timeout: reserved
     * @return 0 for success, negative values for errors.
     */
    int IPC_send(int32_t session_id, struct msg_type *msg, ipc_msg_type type, uint16_t token, int32_t timeout);

    /**
     * MethodCall: syncCall
     * @session_id: session id is used to send message. get from ipcRegister.
     * @send_msg: send message
     * @recv_msg: reply message
     * @timeout: synchronous call timeout, max timeout is 5s
     *           call this function will send message and wait for server's reply,
     *           if server do not reply in timeout period, this call will return null reply and return -1
     * @return: 0 for success, negative values for errors.
     */
    int Sync_Call(int32_t session_id, struct msg_type *send_msg, struct msg_type *recv_msg, int32_t timeout);

    /**
     * MethodCall: asyncCall
     * @session_id: session id is used to send message. get from ipcRegister.
     * @async_msg:  send message
     * @return: 0 for success, negative values for errors.
     */
    int Async_Call(int32_t session_id, struct msg_type *send_msg, bool reply_flag);

    /**
     * MethodCall: subscribeSignal
     * @session_id: session id is used to send message. get from ipcRegister.
     * @cmd:  subscribe signal command
     * @return: 0 for success, negative values for errors.
     * @note: api for client
     */
    int Subscribe_Signal(int32_t session_id, uint32_t cmd);
    /**
     * MethodCall: registerMethod
     * @session_id: session id is used to send message. get from ipcRegister.
     * @cmd:  register command
     * @return: 0 for success, negative values for errors.
     * @note: api for server
     */
    int Register_Method(int32_t session_id, uint32_t cmd);

    typedef int (*recv_func_cb)(struct msg_type *recv_msg, ipc_msg_type type, uint16_t token);
    int Register_Callback_Process(int32_t session_id, recv_func_cb recv_process);
    /**
     * recv_func_package_cb: function type for async's receive callback
     *
     * @reply_msg: The message received.
     */
    typedef int (*recv_func_package_cb)(struct msg_package *recv_msg, ipc_msg_type type, uint16_t token);
    // package message API
    int Register_Callback_Package_Process(int32_t session_id, recv_func_package_cb recv_process);
    int Sync_Call_Package(int32_t session_id, struct msg_package *send_msg, struct msg_package *recv_msg,
                          int32_t timeout);                                                    // client
    int Async_Call_Package(int32_t session_id, struct msg_package *send_msg, bool reply_flag); // client
    int IPC_send_Package(int32_t session_id, struct msg_package *send_msg, ipc_msg_type type, uint16_t token,
                         int32_t timeout);

#ifdef __cplusplus
}
#endif

#endif
