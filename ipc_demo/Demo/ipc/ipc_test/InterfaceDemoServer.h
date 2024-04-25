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
#include "ipc.h"
#include <stdio.h>
#include <string.h>

int32_t ipc_init();
int32_t ipc_deinit();
int32_t session_id;

struct Person
{
    char *firstname;
    char *surname;
};

struct TrackInfo
{
    char *title;
    char *album;
    struct Person composer;
    struct Person interpret;
};

enum father
{
    ONE,
    TWO,
    THREE
};
struct Person XiaoMing;
int8_t method_cmd_list[] = {0, 1};

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

enum ID_error
{
    ID_NO_ERROR,
    ID_IPC_REGISTER_OPEN_NODE_ERROR,
    ID_IPC_REGISTER_REQUEST_SESSION_ERROR,
    ID_IPC_REGISTER_CREAT_RECV_PROCESS_ERROR,
    ID_IPC_DEINIT_ERROR,
    ID_MSG_SIZE_OVERFLOW_ERROR,
    ID_SESSION_ID_OVERFLOW_ERROR,
    ID_SEND_MSG_TYPE_ERROR
};

enum IDA_error
{
    IDA_NO_ERROR,
    IDA_IPC_REGISTER_OPEN_NODE_ERROR,
    IDA_IPC_REGISTER_REQUEST_SESSION_ERROR,
    IDA_IPC_REGISTER_CREAT_RECV_PROCESS_ERROR,
    IDA_IPC_DEINIT_ERROR,
    IDA_MSG_SIZE_OVERFLOW_ERROR,
    IDA_REGISTER_CALLBACK_ERROR,
    IDA_SESSION_ID_OVERFLOW_ERROR,
    IDA_CMD_ID_OVERFLOW_ERROR,
    IDA_REGISTER_METHOD_ERROR,
    IDA_REPLY_MSG_TYPE_ERROR,
    IDA_METHOD_CMD_INVALID_ERROR,
    IDA_SEND_MSG_TYPE_INVALID_ERROR,
    IDA_ONE,
    IDA_TWO,
    IDA_THREE,
    IDA_FOUR,
    IDA_FIVE
};

enum IDB_error
{
    IDB_NO_ERROR,
    IDB_IPC_REGISTER_OPEN_NODE_ERROR,
    IDB_IPC_REGISTER_REQUEST_SESSION_ERROR,
    IDB_IPC_REGISTER_CREAT_RECV_PROCESS_ERROR,
    IDB_IPC_DEINIT_ERROR,
    IDB_MSG_SIZE_OVERFLOW_ERROR,
    IDB_REGISTER_CALLBACK_ERROR,
    IDB_SESSION_ID_OVERFLOW_ERROR,
    IDB_CMD_ID_OVERFLOW_ERROR,
    IDB_REGISTER_METHOD_ERROR,
    IDB_REPLY_MSG_TYPE_ERROR,
    IDB_METHOD_CMD_INVALID_ERROR,
    IDB_SEND_MSG_TYPE_INVALID_ERROR,
};
enum IDA_error IDA(const uint8_t argIDA, const float argIDB, double *argIDC, bool *argIDD);
enum IDB_error IDB(const uint8_t argIDA, const float argIDB);
int32_t message_process();
int32_t subscribe_callback();
int32_t callback_process(struct msg_package *re_msg, ipc_msg_type type, uint16_t token);
int32_t reply_method_process(struct msg_package *in_msg, uint16_t token);
enum ID_error ID(double argIDA, bool argIDB, int32_t timeout);
