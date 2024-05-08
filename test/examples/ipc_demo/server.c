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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <poll.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include "InterfaceDemoServer.h"

//TODO: This method need to be implemented in external user code.
enum IDA_error IDA(const uint8_t argIDA,const float argIDB,double* argIDC,bool* argIDD)
{

}

//TODO: This method need to be implemented in external user code.
enum IDB_error IDB(const uint8_t argIDA,const float argIDB)
{

}

int main(int argc, char const *argv[])
{
   if (ipc_init() < 0) 
    {
        return 0;
    }
    if(message_process()!=NO_ERROR)
    {
        printf("message process failed!\n");
        return 0;
    }
    sleep(15);
    ID(65.88, true,500);
    sleep(100);
    ipc_deinit();
    
	return 0;
}

