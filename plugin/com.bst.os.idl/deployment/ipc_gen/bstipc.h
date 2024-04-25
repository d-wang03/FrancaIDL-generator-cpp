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
#ifndef __BSTIPC_H_
#define __BSTIPC_H_

#include "user_head.h"

#ifdef __cplusplus
extern "C"
{
#endif
    /**
     * IPC_recv
     * @session_id: the session used to receive message. get from ipc_init.
     * @recv_msg: the buffer to receive message.
     * @timeout: -1 means wait forever, 0 means no wait, positive value is the millisenconds
     *          to wait for. other value is meaningless
     * @return 0 for success, negative value for errors.
     * @note if msg->type is IPC_MSG_TYPE_REPLY, then the msg->cmd and msg->token must the same
     *         with the corresponding method call.
     */
    int IPC_recv(int32_t session_id, struct msg_type *recv_msg, int32_t timeout, ipc_msg_type *type, bool *need_reply,
                 bool *payload_flag, uint16_t *token);
    /**
     * IPC_recv_Package
     * @session_id: the session used to receive message. get from ipc_init.
     * @recv_msg: the buffer to receive message.
     * @timeout: -1 means wait forever, 0 means no wait, positive value is the millisenconds
     *          to wait for. other value is meaningless
     * @return 0 for success, negative value for errors.
     * @note if msg->type is IPC_MSG_TYPE_REPLY, then the msg->cmd and msg->token must the same
     *         with the corresponding method call.
     */
    int IPC_recv_Package(int32_t session_id, struct msg_package *recv_msg, int32_t timeout, ipc_msg_type *type,
                         bool *need_reply);

    int func_get_info(enum ipc_core_e coid);
    int func_configure_info_enable(enum ipc_core_e coid);
    int func_configure_info_disable(enum ipc_core_e coid);
    int func_get_sys_info(void);
#ifdef __cplusplus
}
#endif

#endif