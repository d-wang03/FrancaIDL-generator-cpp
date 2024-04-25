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
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <poll.h>
#include <semaphore.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/mman.h>
#include <stdint.h>

#include "ipc.h"
#include "bstipc.h"

/********************* local variables ***************************/
static int fd = -1;
static int memfd = -1;
static int g_session_id = -1;
int32_t wait_token = -1;
static recv_func_package_cb recv_msg_package_cb;

struct msg_type g_recv_msg;
struct msg_package g_recv_pack_msg;

void* send_payload_addr = NULL;
void* recv_payload_addr = NULL;

sem_t sem;
pthread_t tid;

/********************* local function ***************************/
int sem_timedwait_millsecs(long msecs)
{
	struct timespec ts;
	
	clock_gettime(CLOCK_REALTIME, &ts);
	long secs = msecs/1000;
	msecs = msecs%1000;
	
	long add = 0;
	msecs = msecs*1000*1000 + ts.tv_nsec;
	add = msecs / (1000*1000*1000);
	ts.tv_sec += (add + secs);
	ts.tv_nsec = msecs%(1000*1000*1000);

	return sem_timedwait(&sem, &ts);
}

void *recv_thread(void *arg)
{
	//start recv ipc message, first we need check all dependency
	//sync call need release completion
	//async call, signal, method need call callback function
	int ret;
	ipc_msg_type type;
	bool need_reply;
	bool payload_flag;
	uint16_t token;
	
	while (1) 
	{
		ret = IPC_recv(g_session_id, &g_recv_msg, -1, &type, &need_reply, &payload_flag, &token);

		if (ret < 0) {
			printf("session %d recv fail, ret = %d\n", g_session_id, ret);
			pthread_exit(NULL);
		}

		//sync call need release semaphore
		if (type == MSG_TYPE_REPLY && need_reply) {
			sem_post(&sem);
			continue;
		}

		//other case need trigger callback function
		/*if (recv_msg_cb != NULL) {
			((recv_func_cb)recv_msg_cb)(&g_recv_msg, type);
			continue;
		}*/

		if (recv_msg_package_cb != NULL) {
			struct msg_package recv_pack_msg;
			recv_pack_msg.cmd = g_recv_msg.cmd;
			recv_pack_msg.long_param = g_recv_msg.long_param;
			//get payload
			memcpy(recv_pack_msg.payload, recv_payload_addr, sizeof(recv_pack_msg.payload));
			//clear receive payload
			//memset(recv_payload_addr, 0, sizeof(recv_pack_msg.payload));
			((recv_func_package_cb)recv_msg_package_cb)(&recv_pack_msg, type, token);
			continue;
		}
	}
}

int IPC_recv(int32_t session_id, struct msg_type *recv_msg, int32_t timeout, ipc_msg_type *type, bool *need_reply, bool *payload_flag, uint16_t *token)
{
	int ret;

	//session id check 
	if (session_id < 0 || session_id > MAX_SESSION_COUNTS) {
		printf("session id is over range %d\n", session_id);
		return -1;
	}

	struct send_or_recv_msg msg;
	msg.session_id = session_id;
	msg.timeout = timeout;
	ret = ioctl(fd, IPC_IO_MSG_RECV, &msg);
	if (ret < 0) {
		*need_reply = false;
		printf("IPC_recv_msg failed! ret = %d\n", ret);
		return ret;
	}
	
	recv_msg->cmd = msg.msg.cmd;
	recv_msg->long_param = msg.msg.data;
#ifdef MSG_SIZE_EXTENSION
	recv_msg->long_data = msg.msg.long_data;
#endif 
	*type = msg.msg.type;

	if (wait_token == msg.msg.token) {
		*need_reply = true;
		wait_token = -1;
	}
	else
		*need_reply = false;

	*payload_flag = msg.payload_flag;
	*token = msg.msg.token;
	return 0;
}

void* get_msg_buffer(uint8_t core)
{
	if (memfd == -1)
    	if((memfd = open("/dev/mem", O_RDWR | O_SYNC)) == -1) return NULL;
	void *map_base_buf = mmap(0,MAP_SIZE, PROT_READ | PROT_WRITE , MAP_SHARED, memfd, MSG_BUF_ADDR(core) & ~MAP_MASK);
    if(map_base_buf == (void *) -1) return NULL;
    void *virt_buf_addr = map_base_buf + (MSG_BUF_ADDR(core) & MAP_MASK);
	return virt_buf_addr;
}

/********************* API definition ***************************/
/**
 * IPC_Register: register to ipc_service
 * 
 * step:
 * 1. open ipc driver node
 * 2. create session 
 * 3. other config send to ipc service
 * 
 * note: src_id is not used currently
 * RETURN: session_id
*/
int IPC_Register(enum ipc_core_e dst_id, enum ipc_core_e src_id)
{
	int ret;
	fd = open("/dev/bstipc0d", O_RDWR);
	if (fd < 0)
    {
        printf("can't open node! fd = %d\n", fd);
		return -REGISTER_IPC_OPEN_NODE_FAILED;
    }

	struct _session_init init;
	init.dst = dst_id;
	init.src = src_id;

	int session_id = ioctl(fd, IPC_IO_SES_CREATE, &init);
	if (session_id < 0) {
		printf("IPC_register failed, return id is %d\n", session_id);
		return -REGISTER_IPC_REQUEST_SESSION_FAIL;
	}
	g_session_id = session_id;

	//init sem
	if (sem_init(&sem, 0, 0) == -1)
	{
		printf("sem_init error");
		return -REGISTER_IPC_DEPENDENCY_FAILED;
	}
#if 0
	//get shared buffer payload address
	struct session_payload_info info;
	info.session_id = session_id;
	int ret = ioctl(fd, IPC_IO_GET_PAYLOAD_ADDR, &info);
	if (ret < 0)
	{
		printf("can get payload buffer addr/n");
		return -REGISTER_IPC_DEPENDENCY_FAILED;
	}
	send_payload_addr = (void*)info.send_addr;
	recv_payload_addr = (void*)info.recv_addr;
#endif
	send_payload_addr = get_msg_buffer(src_id);
	recv_payload_addr = get_msg_buffer(dst_id);

	//printf("get send addr 0x%lx, recv addr 0x%lx\n", send_payload_addr, recv_payload_addr);

	//start recv thread
	ret = pthread_create(&tid, NULL, &recv_thread, NULL);
	if (ret != 0) {
		printf("create recv thread fail, ret = %d\n", ret);
		return -REGISTER_IPC_DEPENDENCY_FAILED;
	}
	pthread_detach(tid);

	return session_id;
}

/**
 * Register_Callback_Process: Subscribe_Callback_Process
 * 
 * @session_id:     session id
 * @recv_process:    callback processing when async message receive 
 * @callback: 
 * RETURN: 0 for success, negative value for errors.
*/
/*int Register_Callback_Process(int32_t session_id, recv_func_cb recv_process)
{	
	//session id check 
	if (session_id < 0 || session_id > MAX_SESSION_COUNTS) {
		printf("session id is over range %d\n", session_id);
		return -1;
	}

	//callback register
	g_session_id = session_id;
	recv_msg_cb = recv_process;
	return 0;
}*/

int Register_Callback_Package_Process(int32_t session_id, recv_func_package_cb recv_process)
{	
	//session id check 
	if (session_id < 0 || session_id > MAX_SESSION_COUNTS) {
		printf("session id is over range %d\n", session_id);
		return -REGISTER_CALLBACK_SESSION_INVALID;
	}

	//callback register
	g_session_id = session_id;
	recv_msg_package_cb = recv_process;
	return 0;
}
/**
 * MethodCall: Subscribe_Signal
 * 
 * @session_id:     session id
 * @send_msg:    callback processing when async message receive 
 * RETURN: 0 for success, negative value for errors.
*/
int Subscribe_Signal(int32_t session_id, uint32_t cmd)
{
	int ret;

	//session id check 
	if (session_id < 0 || session_id > MAX_SESSION_COUNTS) {
		printf("session id is over range %d\n", session_id);
		return -SUBSCRIBE_SIGNAL_SESSION_INVALID;
	}

	//cmd check
	if (cmd < 0 || cmd > MAX_CMD_COUNTS) {
		printf("cmd id is over range %d\n", cmd);
		return -SUBSCRIBE_SIGNAL_CMD_INVALID;
	}

	struct msg_subscribe info;
	info.type = IPC_MSG_TYPE_SIGNAL;
	info.session_id = session_id;
	info.cmd = cmd;
	ret = ioctl(fd, IPC_IO_REGISTER_INFO, &info);
	if (ret < 0) {
		printf("Subscribe_Signal call failed!, ret = %d\n", ret);
		return -SUBSCRIBE_SIGNAL_FAILED;
	}

	return 0;
}

/**
 * MethodCall: Subscribe_Method
 * 
 * @session_id:     session id
 * @send_msg:    callback processing when async message receive 
 * RETURN: 0 for success, negative value for errors.
*/
int Register_Method(int32_t session_id, uint32_t cmd)
{
	int ret;

	//session id check 
	if (session_id < 0 || session_id > MAX_SESSION_COUNTS) {
		printf("session id is over range %d\n", session_id);
		return -REGISTER_METHOD_SESSION_INVALID;
	}

	//cmd check
	if (cmd < 0 || cmd > MAX_CMD_COUNTS) {
		printf("cmd id is over range %d\n", cmd);
		return -REGISTER_METHOD_CMD_INVALID;
	}

	struct msg_subscribe info;
	info.type = IPC_MSG_TYPE_METHOD;
	info.session_id = session_id;
	info.cmd = cmd;
	ret = ioctl(fd, IPC_IO_REGISTER_INFO, &info);
	if (ret < 0) {
		printf("Subscribe_Method call failed!, ret = %d\n", ret);
		return -REGISTER_METHOD_FAILED;
	}

	return 0;
}

/**
 * IPC_Unregister: unregister to ipc_service
 * RETURN: 0 for success, negative value for errors.
*/
int IPC_Unregister(int32_t session_id)
{
	munmap(send_payload_addr, MAP_SIZE);
	munmap(recv_payload_addr, MAP_SIZE);
	//recv thread 
	int ret = ioctl(fd, IPC_IO_SES_DESTROY, &session_id);
	if (ret < 0) {
		printf("IPC_unregister failed!\n");
		return -UNREGISTER_IPC_FAILED;
	}

	close(memfd);
	close(fd);

	return 0;
}

/**
 * Send IPC message.
 * @session_id: the session used to send message. get from IPC_Register.
 * @msg: the message to be sent.
 * @timeout: no important     
 * @return 0 for success, negative value for errors.
 */
int IPC_send(int32_t session_id, struct msg_type *msg, ipc_msg_type type,  int32_t timeout)
{
	int ret;

	//session id check 
	if (session_id < 0 || session_id > MAX_SESSION_COUNTS) {
		printf("session id is over range %d\n", session_id);
		return -SEND_MSG_SESSION_INVALID;
	}
	
	struct send_or_recv_msg send_msg;
	if (type < MSG_TYPE_METHOD || type > MSG_TYPE_MAX) {
		printf("session id %d  unsupported type  %d\n", session_id, type);
		return -SEND_MSG_TYPE_INVALID;
	}
	send_msg.msg.type = type;
	send_msg.msg.cmd = msg->cmd;
	send_msg.msg.data = msg->long_param;
#ifdef MSG_SIZE_EXTENSION
	send_msg.msg.long_data = msg->long_data;
#endif 
	send_msg.session_id = session_id;
	send_msg.timeout = -1;
	send_msg.reply_flag = false;
	send_msg.payload_flag = false;
	send_msg.payload_size = 0;

	ret = ioctl(fd, IPC_IO_MSG_SEND, &send_msg);
	if (ret < 0) {
		printf("IPC_send_msg failed! ret = %d\n", ret);
		return -SEND_MSG_FAILED;
	}

	return 0;
}

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
int Sync_Call(int32_t session_id, struct msg_type *send_msg, struct msg_type *recv_msg, int32_t timeout)
{
	int ret;

	//session id check 
	if (session_id < 0 || session_id > MAX_SESSION_COUNTS) {
		printf("session id is over range %d\n", session_id);
		return -SYNC_CALL_SESSION_INVALID;
	}
	if (timeout > 5000 || timeout < 0) {
		printf("timeout is over range %d\n", timeout);
		return -SYNC_CALL_TIMEOUT_OUTRANGED;
	}

	struct send_or_recv_msg msg;
	msg.msg.cmd = send_msg->cmd;
	msg.msg.data = send_msg->long_param;
	msg.msg.type = IPC_MSG_TYPE_METHOD;
#ifdef MSG_SIZE_EXTENSION
	msg.msg.long_data = msg->long_data;
#endif 	
	msg.session_id = session_id;
	msg.timeout = 0;
	msg.reply_flag = true;
	msg.payload_flag = false;
	msg.payload_size = 0;

	ret = ioctl(fd, IPC_IO_MSG_SEND, &msg);
	if (ret < 0) {
		printf("ipc sync call failed!, ret = %d\n", ret);
		return -SYNC_CALL_SEND_FAILED;
	}
	wait_token = msg.msg.token;
	//printf("need wait reply token = %d\n", wait_token);

	//max timeout is 5s
	ret = sem_timedwait_millsecs(timeout);
	if (ret < 0) {
		if (errno == ETIMEDOUT)
		{
			//not receive msg
			printf("session id %d Sync_Call() timed out\n", session_id);
		}
		else
		{
			//other error
			printf("sem_timedwait other error\n");
		}
		return -SYNC_CALL_TIMEOUT;
	} 

	recv_msg->cmd = g_recv_msg.cmd;
	recv_msg->long_param = g_recv_msg.long_param;
#ifdef MSG_SIZE_EXTENSION
	recv_msg->long_data = g_recv_msg.long_data;
#endif 	
	return 0;
}

/**
 * MethodCall: Async_Call
 * 
 * @session_id:     session id
 * @send_msg:     fill out struct msg_type, it will describe send message cmd and parameter
 * @callback: 
 * RETURN: 0 for success, negative value for errors.
*/
int Async_Call(int32_t session_id, struct msg_type *send_msg)
{
	int ret;

	//session id check 
	if (session_id < 0 || session_id > MAX_SESSION_COUNTS) {
		printf("session id is over range %d\n", session_id);
		return -ASYNC_CALL_SESSION_INVALID;
	}

	struct send_or_recv_msg msg;
	msg.msg.cmd = send_msg->cmd;
	msg.msg.data = send_msg->long_param;
	msg.msg.type = IPC_MSG_TYPE_METHOD;
#ifdef MSG_SIZE_EXTENSION
	msg.msg.long_data = msg->long_data;
#endif 	
	msg.session_id = session_id;
	msg.timeout = 0;
	msg.reply_flag = false;
	msg.payload_flag = false;
	msg.payload_size = 0;

	ret = ioctl(fd, IPC_IO_MSG_SEND, &msg);
	if (ret < 0) {
		printf("ipc async call failed!, ret = %d\n", ret);
		return -ASYNC_CALL_SEND_FAILED;
	}
	return 0;
}

/**
 * MethodCall: IPC_send_Package
 * 
 * @session_id:     session id
 * @send_msg:    callback processing when async message receive 
 * RETURN: 0 for success, negative value for errors.
*/
int IPC_send_Package(int32_t session_id, struct msg_package *send_msg, ipc_msg_type type, uint16_t token, int32_t timeout)
{
	int ret, cnt;

	//session id check 
	if (session_id < 0 || session_id > MAX_SESSION_COUNTS) {
		printf("session id is over range %d\n", session_id);
		return -SEND_MSG_SESSION_INVALID;
	}

	if (type <= MSG_TYPE_METHOD || type > MSG_TYPE_MAX) {
		printf("IPC_send_Package type is invalid %d\n", type);
		return -SEND_MSG_TYPE_INVALID;
	}

	if (send_msg->payload_size > MAX_PACKAGE_PAYLOAD_SIZE) {
		printf("IPC_send_Package payload size is out of range, size = %d\n",send_msg->payload_size);
		return -SEND_MSG_OVERSIZED;
	}
	//payload memcpy
	if (send_payload_addr == NULL) {
		printf("send payload buffer addr is invalid\n");
		return -SEND_MSG_FAILED;
	}

	struct send_or_recv_msg msg;
	msg.msg.type = type;
	msg.msg.cmd = send_msg->cmd;
	msg.msg.data = send_msg->long_param;
	msg.msg.token = token;
	msg.session_id = session_id;
	msg.timeout = 0;
	msg.reply_flag = false;
	msg.payload_flag = true;
	msg.payload_size = send_msg->payload_size;

	memcpy(send_payload_addr, send_msg->payload, sizeof(send_msg->payload));
	ret = ioctl(fd, IPC_IO_MSG_SEND, &msg);
	if (ret < 0) {
		printf("IPC_send_msg failed! ret = %d\n", ret);
		return -SEND_MSG_FAILED;
	}

	return 0;
}

/**
 * Sync_Call_Package: Sync_Call_Package
 * 
 * @session_id:     session id
 * @send_msg:    callback processing when async message receive 
 * @recv_msg: 
 * @timeout:
 * RETURN: 0 for success, negative value for errors.
*/
int Sync_Call_Package(int32_t session_id,struct msg_package *send_msg, struct msg_package *recv_msg, int32_t timeout)
{
	int ret;

	//session id check 
	if (session_id < 0 || session_id > MAX_SESSION_COUNTS) {
		printf("session id is over range %d\n", session_id);
		return -SYNC_CALL_SESSION_INVALID;
	}
	if (timeout > 5000 || timeout < 0)
	{
		printf("timeout is over range %d\n", timeout);
		return -SYNC_CALL_TIMEOUT_OUTRANGED;
	}
	if (send_msg->payload_size > MAX_PACKAGE_PAYLOAD_SIZE) {
		printf("IPC_send_Package payload size is out of range, size = %d\n",send_msg->payload_size);
		return -SYNC_CALL_MSG_OVERSIZED;
	}
	//payload memcpy
	if (send_payload_addr == NULL) {
		printf("send payload buffer addr is invalid\n");
		return -SEND_MSG_FAILED;
	}

	struct send_or_recv_msg msg;
	msg.msg.type = IPC_MSG_TYPE_METHOD;
	msg.msg.cmd = send_msg->cmd;
	msg.msg.data = send_msg->long_param;
	msg.session_id = session_id;
	msg.timeout = 0;
	msg.reply_flag = true;
	msg.payload_flag = true;
	msg.payload_size = 0;

	memcpy(send_payload_addr, send_msg->payload, sizeof(uint64_t) * send_msg->payload_size);

	ret = ioctl(fd, IPC_IO_MSG_SEND, &msg);
	if (ret < 0) {
		printf("ipc sync package call failed!, ret = %d\n", ret);
		return -SYNC_CALL_SEND_FAILED;
	}
	wait_token = msg.msg.token;
	//printf("need wait reply token = %d\n", wait_token);

	//max timeout is 5s
	ret = sem_timedwait_millsecs(timeout);
	if (ret < 0) {
		if (errno == ETIMEDOUT)
		{
			//not receive msg
			printf("session id %d Sync_Call_Package() timed out\n", session_id);
		}
		else
		{
			//other error
			printf("sem_timedwait other error\n");
		}
		return -SYNC_CALL_TIMEOUT;
	} 

	//message header update
	recv_msg->cmd = g_recv_msg.cmd;
	recv_msg->long_param = g_recv_msg.long_param;
	//get payload
	memcpy(recv_msg->payload, recv_payload_addr ,sizeof(recv_msg->payload));
	//clear receive payload
	//memset(recv_payload_addr, 0, sizeof(recv_msg->payload));
	return 0;
}

/**
 * MethodCall: Async_Call_Package
 * 
 * @session_id:     session id
 * @send_msg:    callback processing when async message receive 
 * RETURN: 0 for success, negative value for errors.
*/
int Async_Call_Package(int32_t session_id,struct msg_package *send_msg, bool reply_flag)
{
	int ret;

	//session id check 
	if (session_id < 0 || session_id > MAX_SESSION_COUNTS) {
		printf("session id is over range %d\n", session_id);
		return -ASYNC_CALL_SESSION_INVALID;
	}
	
	if (send_msg->payload_size > MAX_PACKAGE_PAYLOAD_SIZE) {
		printf("Async_Call_Package payload size is out of range, size = %d\n",send_msg->payload_size);
		return -ASYNC_CALL_MSG_OVERSIZED;
	}
	//payload memcpy
	if (send_payload_addr == NULL) {
		printf("send payload buffer addr is invalid\n");
		return -SEND_MSG_FAILED;
	}

	struct send_or_recv_msg msg;
	msg.msg.type = IPC_MSG_TYPE_METHOD;
	msg.msg.cmd = send_msg->cmd;
	msg.msg.data = send_msg->long_param;
	msg.session_id = session_id;
	msg.timeout = 0;
	msg.reply_flag = reply_flag;
	msg.payload_flag = true;
	msg.payload_size = 0;

	memcpy(send_payload_addr, send_msg->payload, sizeof(uint64_t) * send_msg->payload_size);

	ret = ioctl(fd, IPC_IO_MSG_SEND, &msg);
	if (ret < 0) {
		printf("ipc sync package call failed!, ret = %d\n", ret);
		return -ASYNC_CALL_SEND_FAILED;
	}
	return 0;
}



