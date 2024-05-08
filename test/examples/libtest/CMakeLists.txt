CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(lib_test LANGUAGES CXX)

include_directories(../src-gen)

add_library(test ../src-gen/v0/commonapi/examples/E01HelloWorldStub.cpp)

add_executable(${PROJECT_NAME} main.cpp
    ../src-gen/v0/commonapi/examples/E01HelloWorldProxy.cpp)
target_link_libraries(${PROJECT_NAME} test)