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
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SUPPORT_CMD 5

void broadcast_callback_demo(double *a, bool *b)
{
    printf("broadcast demo double value:%lf\n", *a);
    if (*b == true)
    {
        printf("broadcast demo bool value:true\n");
    }

    if (*b == false)
    {
        printf("broadcast demo bool value:false\n");
    }
}

enum IDA_error method_callback_demo(double *a, bool *b)
{
    printf("method demo double value:%lf\n", *a);
    if (*b == true)
    {
        printf("method demo bool value:true\n");
    }

    if (*b == false)
    {
        printf("method demo bool value:false\n");
    }
    return IDA_NO_ERROR;
}

int main(int argc, char const *argv[])
{
    if (ipc_init() < 0)
    {
        return 0;
    }

    if (ID_Subscribe(broadcast_callback_demo) != NO_ERROR)
    {
        printf("broadcast ID subscribe false!\n");
        return 0;
    }

    if (IDA_Async_Register(100, 3.33, method_callback_demo) != NO_ERROR)
    {
        printf("method IDA subscribe false!\n");
        return 0;
    }
    if (subscribe_callback() != NO_ERROR)
    {
        printf("subscribe callbsck false!\n");
        return 0;
    }

    uint8_t idaIntValue = 111;
    double idaDoubleValue;
    float idaFloatValue = 112.2;
    bool idaBoolValue;

    IDA_SyncCall(idaIntValue, idaFloatValue, &idaDoubleValue, &idaBoolValue, 1000);

    sleep(2);

    IDA_ASyncCall();
    sleep(2);
    uint8_t idbIntValue = 222;
    float idbFloatValue = 113.3;
    IDB_Call(idbIntValue, idbFloatValue);

    sleep(100);
    ipc_deinit();
    return 0;
}
