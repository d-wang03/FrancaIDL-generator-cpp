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

enum IDA_error
{
    IDA_NO_ERROR,
    IDA_IPC_REGISTER_OPEN_NODE_ERROR,
    IDA_IPC_REGISTER_REQUEST_SESSION_ERROR,
    IDA_IPC_REGISTER_CREAT_RECV_PROCESS_ERROR,
    IDA_IPC_DEINIT_ERROR,
    IDA_REPLY_MSG_TYPE_ERROR,
    IDA_CALLBACK_PTR_ERROR,
    IDA_SUBSCRIBE_CALLBACK_ERROR,
    IDA_MSG_SIZE_OVERFLOW_ERROR,
    IDA_METHOD_CMD_INVALID_ERROR,
    IDA_SESSION_ID_OVERFLOW_ERROR,
    IDA_ASYNC_CALL_SEED_MSG_ERROR,
    IDA_TIMEOUT_RANGE_OVERFLOW_ERROR,
    IDA_PAYLOAD_SIZE_RANGE_OVERFLOW_ERROR,
    IDA_SYNC_CALL_SEED_MSG_ERROR,
    IDA_SYNC_CALL_TIMEOUT_ERROR,
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
    IDB_SESSION_ID_OVERFLOW_ERROR,
    IDB_ASYNC_CALL_SEED_MSG_ERROR,
};

enum ID_error
{
    ID_NO_ERROR,
    ID_IPC_REGISTER_OPEN_NODE_ERROR,
    ID_IPC_REGISTER_REQUEST_SESSION_ERROR,
    ID_IPC_REGISTER_CREAT_RECV_PROCESS_ERROR,
    ID_IPC_DEINIT_ERROR,
    ID_REPLY_MSG_TYPE_ERROR,
    ID_CALLBACK_PTR_ERROR,
    ID_SUBSCRIBE_CALLBACK_ERROR,
    ID_BROADCAST_CMD_INVALID_ERROR,
    ID_SESSION_ID_OVERFLOW_ERROR,
    ID_CMD_ID_OVERFLOW_ERROR,
    ID_SUBSCRIBE_SIGNAL_ERROR,
};
typedef enum IDA_error (*IDA_ASyncCallback_t)(double *argIDC, bool *argIDD);
IDA_ASyncCallback_t IDA_async_callback;
enum IDA_error IDA_ASyncCall();
enum IDA_error IDA_Async_Register(const uint8_t argIDA, const float argIDB, IDA_ASyncCallback_t callback);
enum IDA_error IDA_SyncCall(const uint8_t argIDA, const float argIDB, double *argIDC, bool *argIDD, int32_t timeout);
enum IDB_error IDB_Call(const uint8_t argIDA, const float argIDB);

int32_t subscribe_callback();
int32_t reply_method_process(struct msg_package *re_msg);
int32_t callback_process(struct msg_package *re_msg, ipc_msg_type type, uint16_t token);
typedef void (*ID_ASyncCallback_t)(double *argIDA, bool *argIDB);
ID_ASyncCallback_t ID_async_callback;
enum ID_error ID_Subscribe(ID_ASyncCallback_t callback);
int32_t reply_broadcast_process(struct msg_package *re_msg);
