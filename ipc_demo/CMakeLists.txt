cmake_minimum_required(VERSION 2.6)

project(ipc-test)

#add_definitions(-std=c++11)
#set(CMAKE_C_FLAGS "-lpthread")

message(${PROJECT_SOURCE_DIR})
link_directories(${PROJECT_SOURCE_DIR}/lib)

include_directories(./include)  
aux_source_directory(./src/ DIR_SRC)

#SET(IPC_SRC ./src/ipc.c ./src/InterfaceDemoClient.c ./src/InterfaceDemoServer.c)
SET(IPC_CLIENT_SRC ./src/ipc.c ./src/InterfaceDemoClient.c)
SET(IPC_SERVER_SRC ./src/ipc.c ./src/InterfaceDemoServer.c) 
#ADD_LIBRARY(ipc SHARED ${IPC_SRC})
ADD_LIBRARY(ipc_client SHARED ${IPC_CLIENT_SRC})
ADD_LIBRARY(ipc_server SHARED ${IPC_SERVER_SRC})
#target_link_libraries(ipc pthread)
target_link_libraries(ipc_client pthread)
target_link_libraries(ipc_server pthread)
# install(TARGETS ipc LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

add_executable(ipc_pack_send ./send.c)
add_executable(ipc_pack_recv ./recv.c)

target_link_libraries(ipc_pack_recv ipc_server)
target_link_libraries(ipc_pack_send ipc_client)

#install(TARGETS ipc_demo RUNTIME DESTINATION bin)
#install(TARGETS getIPfrom397 RUNTIME DESTINATION bin)
#install(TARGETS ipc RUNTIME DESTINATION bin)
#install(TARGETS ipc LIBRARY DESTINATION lib)
#install(TARGETS ipc_pack_send LIBRARY DESTINATION lib)
#install(TARGETS ipc_pack_recv LIBRARY DESTINATION lib)
