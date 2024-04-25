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
#include "InterfaceDemoClient.h"
static const enum ipc_core_e client = (enum ipc_core_e)5;
static const enum ipc_core_e server = (enum ipc_core_e)6;

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

int32_t subscribe_callback()
{
    if(Register_Callback_Package_Process(session_id,callback_process)!=0)
        return SUBSCRIBE_CALLBACK_ERROR;
    return NO_ERROR;
}

enum IDA_error IDA_SyncCall(const uint8_t argIDA,const float argIDB,double* argIDC,bool* argIDD , int32_t timeout)
{
    if (session_id < 0) 
    {
        printf("IPC_register  session id is lower than zero");
        return -session_id;
    }
    struct msg_package send_msg;   
    struct msg_package re_msg;
    send_msg.cmd= 0;
    send_msg.long_param=1;
    send_msg.payload_size=(sizeof(uint8_t)+sizeof(float)-1)/8+1;
    if(send_msg.payload_size > 7)
    {
        printf("date overflow,date size should not be more than 7 byte!\n");
        return IDA_MSG_SIZE_OVERFLOW_ERROR;
    }
    memcpy(send_msg.payload,&argIDA, sizeof(argIDA));
    memcpy(send_msg.payload+sizeof(argIDA),&argIDB, sizeof(argIDB));

    int32_t ret =Sync_Call_Package(session_id,&send_msg,&re_msg,timeout);
    if(ret==-SYNC_CALL_SESSION_INVALID)
    {
        printf("call Sync_Call_Package failed:session id overflow!\n");
        return IDA_SESSION_ID_OVERFLOW_ERROR;
    }
    if(ret==-SYNC_CALL_TIMEOUT_OUTRANGED)
    {
        printf("call Sync_Call_Package failed:timeout range overflow!\n");
        return IDA_TIMEOUT_RANGE_OVERFLOW_ERROR;
    }
    if(ret==-SYNC_CALL_MSG_OVERSIZED)
    {
        printf("call Sync_Call_Package failed:payload size range overflow!\n");
        return IDA_PAYLOAD_SIZE_RANGE_OVERFLOW_ERROR;
    }
    if(ret==-SYNC_CALL_SEND_FAILED)
    {
        printf("call Sync_Call_Package failed:timeout range overflow!\n");
        return IDA_SYNC_CALL_SEED_MSG_ERROR;
    }
    if(ret==-SYNC_CALL_TIMEOUT)
    {
        printf("call Sync_Call_Package failed:receive message timeout !\n");
        return IDA_SYNC_CALL_TIMEOUT_ERROR;
    }
    memcpy(argIDC, re_msg.payload, sizeof(*argIDC));
    memcpy(argIDD, re_msg.payload+sizeof(*argIDC), sizeof(*argIDD));

    return IDA_NO_ERROR;
}

static struct msg_package IDA_send_msg;
enum IDA_error IDA_Async_Register(const uint8_t argIDA,const float argIDB, IDA_ASyncCallback_t callback)
{
    if (session_id < 0)
    {
        printf("IPC_register session id is lower than zero");
        return -session_id;
    }

    if (!callback)
    {
        printf("callback is null");
        return IDA_CALLBACK_PTR_ERROR;
    }
    IDA_async_callback = callback;

    IDA_send_msg.cmd = 0;
    IDA_send_msg.payload_size = (sizeof(uint8_t)+sizeof(float)-1)/8+1;
    IDA_send_msg.long_param = 1;
    if (IDA_send_msg.payload_size > 7)
    {
        printf("date overflow,date size should not be more than 56 byte!\n");
        return IDA_MSG_SIZE_OVERFLOW_ERROR;
    }
    memcpy(IDA_send_msg.payload,&argIDA, sizeof(argIDA));
    memcpy(IDA_send_msg.payload+sizeof(argIDA),&argIDB, sizeof(argIDB));

    return IDA_NO_ERROR;
}

enum IDA_error IDA_ASyncCall()
{
    int32_t ret = Async_Call_Package(session_id, &IDA_send_msg, true);
    if (ret == -ASYNC_CALL_SESSION_INVALID)
    {
        printf("call Async_Call_Package failed: session id overflow !\n");
        return IDA_SESSION_ID_OVERFLOW_ERROR;
    }
    if (ret == -ASYNC_CALL_MSG_OVERSIZED)
    {
        printf("call Async_Call_Package failed: message size overflow !\n");
        return IDA_MSG_SIZE_OVERFLOW_ERROR;
    }
    if (ret == -ASYNC_CALL_SEND_FAILED)
    { 
        printf("call Async_Call_Package failed:send message failed!\n");
        return IDA_ASYNC_CALL_SEED_MSG_ERROR;
    }
    return IDA_NO_ERROR;
}

enum IDB_error IDB_Call(const uint8_t argIDA,const float argIDB)
{
    if (session_id < 0) 
    {
        printf("IPC_register  session id is lower than zero");
        return -session_id;
    }
    struct msg_package send_msg;
    send_msg.cmd = 1;
    send_msg.payload_size=(sizeof(uint8_t)+sizeof(float)-1)/8+1;
    send_msg.long_param=0;
    if(send_msg.payload_size>7)
    {
        printf("date overflow,date size should not be more than 7 byte!\n");
        return IDB_MSG_SIZE_OVERFLOW_ERROR;
    }
    memcpy(send_msg.payload,&argIDA, sizeof(argIDA));
    memcpy(send_msg.payload+sizeof(argIDA),&argIDB, sizeof(argIDB));

    int32_t ret=Async_Call_Package(session_id,&send_msg,true);
    if(ret==-ASYNC_CALL_SESSION_INVALID)
    {
        printf("call Async_Call_Package failed:session id overflow!\n");
        return IDB_SESSION_ID_OVERFLOW_ERROR;
    }
    if(ret==-ASYNC_CALL_MSG_OVERSIZED)
    {
        printf("call Async_Call_Package failed:message size overflow!\n");
        return IDB_MSG_SIZE_OVERFLOW_ERROR;
    }
    if(ret==-ASYNC_CALL_SEND_FAILED)
    {
        printf("call Async_Call_Package failed:send message failed!\n");
        return IDB_ASYNC_CALL_SEED_MSG_ERROR;
    }
    return IDB_NO_ERROR;
}

int32_t reply_method_process(struct msg_package *re_msg)
{
    switch(re_msg->cmd)
    {
    case 0:
        if(IDA_async_callback==NULL)
        {
            printf("IDA_async_callback is nullptr!\n");
            return CALLBACK_PTR_ERROR;
        }
        double argIDC;
        bool argIDD;

        memcpy(&argIDC, re_msg->payload, sizeof(argIDC));
        memcpy(&argIDD, re_msg->payload+sizeof(argIDC), sizeof(argIDD));

        IDA_async_callback(&argIDC,&argIDD);
        break;
    
    default:
        printf("method command id is invalid!");
        return METHOD_CMD_INVALID_ERROR;
    }
    return NO_ERROR;
}

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

enum ID_error ID_Subscribe(ID_ASyncCallback_t callback)
{
    if (session_id < 0) 
    {
        printf("IPC_register  session id is lower than zero");
        return -session_id;
    }
    if(!callback)
    {
        printf("callback is null");
        return ID_CALLBACK_PTR_ERROR;
    }
    ID_async_callback=callback;
    int32_t ret=Subscribe_Signal(session_id,0);
    if(ret==-SUBSCRIBE_SIGNAL_SESSION_INVALID)
    {
        printf("Subscribe Signal failed:session id overflow!\n");
        return ID_SESSION_ID_OVERFLOW_ERROR;
    }
    if(ret==-SUBSCRIBE_SIGNAL_CMD_INVALID)
    {
        printf("Subscribe Signal failed:command id is out of range!\n");
        return ID_CMD_ID_OVERFLOW_ERROR;
    }
    if(ret==-SUBSCRIBE_SIGNAL_FAILED)
    {
        printf("Subscribe Signal failed!\n");
        return ID_SUBSCRIBE_SIGNAL_ERROR;
    }
    return ID_NO_ERROR;
}

int32_t reply_broadcast_process(struct msg_package *re_msg)
{ 
    switch(re_msg->cmd)
    {
    case 0:
        if(ID_async_callback==NULL)
        {
            printf("ID_async_callback is nullptr!\n");
            return CALLBACK_PTR_ERROR;
        }
        double argIDA;
        bool argIDB;

        memcpy(&argIDA, re_msg->payload, sizeof(argIDA));
        memcpy(&argIDB, re_msg->payload+sizeof(argIDA), sizeof(argIDB));

        ID_async_callback(&argIDA,&argIDB);
        break;
    
    default:
        printf("broadcast command id is invalid!");
        return BROADCAST_CMD_INVALID_ERROR;
    }
    return NO_ERROR;
}
