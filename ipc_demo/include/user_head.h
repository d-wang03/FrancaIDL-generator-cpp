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
#ifndef __USER_HEAD_H_
#define __USER_HEAD_H_

// this is header file between driver and libipc

// macro definition
#define MAX_SESSION_COUNTS 64
#define MAX_CMD_COUNTS 256
#define SHARED_BUF_ADDR 0x8ff00000
#define PAYLOAD_OFFSET 0x8
#define MSG_BUF_ADDR(core) SHARED_BUF_ADDR + 0x40 * core + PAYLOAD_OFFSET
#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

typedef enum
{
    IPC_MSG_TYPE_METHOD = 1,
    IPC_MSG_TYPE_REPLY = 2,
    IPC_MSG_TYPE_SIGNAL = 3,
    IPC_MSG_TYPE_MAX,
} _msg_type;

// err number definition
#define IPC_INIT_ERR -1
#define IPC_INIT_ERR_INVALID_PARAM -2
#define IPC_INIT_ERR_LINK -3
#define IPC_INIT_ERR_SESSION -4

#define IPC_SEND_ERR -1
#define IPC_SEND_ERR_INVALID_PARAM -2
#define IPC_SEND_ERR_LINK -3
#define IPC_SEND_ERR_TIMEOUT -4

#define IPC_RECV_ERR -1
#define IPC_RECV_ERR_INVALID_PARAM -2
#define IPC_RECV_ERR_LINK -3
#define IPC_RECV_ERR_TIMEOUT -4
#define IPC_RECV_ERR_GET_MSG_FAIL -5

#define IPC_CLOSE_ERR -1

#define IPC_SUBSCRIBE_ERR -1
#define IPC_SUBSCRIBE_INVALID_PARAM -2

#define IPC_RECV_PROCESS_REGISTER_ERR -1
#define IPC_RECV_PROCESS_REGISTER_FAIL -2

#define IPC_METHOD_REGISTER_ERR -1
#define IPC_METHOD_REGISTER_INVALID_PARAM -2
#define IPC_METHOD_REGISTER_REPETITION -3

// IO definition
#define IPC_IO_MEM 'M'
#define IPC_IO_MEM_ALLOC _IO(IPC_IO_MEM, 1) // current is deprecated
#define IPC_IO_MEM_FREE _IO(IPC_IO_MEM, 2)  // current is deprecated
#define IPC_IO_REGISTER_INFO _IO(IPC_IO_MEM, 3)
#define IPC_IO_SES_CREATE _IO(IPC_IO_MEM, 4)
#define IPC_IO_SES_DESTROY _IO(IPC_IO_MEM, 5)
#define IPC_IO_DRI_READY _IO(IPC_IO_MEM, 6)      // current is deprecated
#define IPC_IO_MSG_SEND_ASYNC _IO(IPC_IO_MEM, 7) // current is deprecated
#define IPC_IO_MSG_SEND _IO(IPC_IO_MEM, 8)
#define IPC_IO_MSG_RECV _IO(IPC_IO_MEM, 9)
#define IPC_IO_GET_INFO _IO(IPC_IO_MEM, 10)
#define IPC_IO_CFG_INFO_ENABLE _IO(IPC_IO_MEM, 11)
#define IPC_IO_CFG_INFO_DISABLE _IO(IPC_IO_MEM, 12)
#define IPC_IO_GET_SYS_INFO _IO(IPC_IO_MEM, 13)
#define IPC_IO_GET_PAYLOAD_ADDR _IO(IPC_IO_MEM, 14)

/**
 * this struct is used for user for to create session
 *
 * @src:    the source core id to communicate with,
 *          that's the source core id of session.
 * @dst:    the target core id of session
 */
struct _session_init
{
    uint8_t src;
    uint8_t dst;
    int32_t session_id;
};

// definition of IPC message.
struct ipc_msg
{
    _msg_type type;
    uint8_t cmd;
    uint16_t token;
    uint32_t data;
    uint64_t userdata;
#ifdef MSG_SIZE_EXTENSION
    uint64_t long_data;
#endif
};

// send or recv msg struct
struct send_or_recv_msg
{
    struct ipc_msg msg;
    uint8_t session_id;
    int32_t timeout;
    bool reply_flag;
    bool payload_flag;
    uint8_t payload_size;
};

// subscribe signal struct
struct msg_subscribe
{
    _msg_type type;
    uint8_t session_id;
    uint32_t cmd;
};

// get payload info
struct session_payload_info
{
    uint8_t session_id;
    uint64_t send_addr;
    uint64_t recv_addr;
};

#endif
