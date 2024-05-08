cmake_minimum_required(VERSION 2.10)

project(ipc-test)

message(${PROJECT_SOURCE_DIR})
link_directories(${PROJECT_SOURCE_DIR}/lib)

set(SRC_GEN_PATH ipc_demo/src-gen/v1/com/bst/os/idl/ipc/test)
include_directories(ipc_demo/src-gen)
include_directories(${SRC_GEN_PATH})  

FILE(GLOB IPC_CLIENT_SRC ipc_demo/src-gen/ipc.c ${SRC_GEN_PATH}/*Client.c)
FILE(GLOB IPC_SERVER_SRC ipc_demo/src-gen/ipc.c ${SRC_GEN_PATH}/*Server.c) 

add_executable(ipc_server ./server.c ${IPC_SERVER_SRC}) 
add_executable(ipc_client ./client.c ${IPC_CLIENT_SRC})

target_link_libraries(ipc_server pthread)
target_link_libraries(ipc_client pthread)