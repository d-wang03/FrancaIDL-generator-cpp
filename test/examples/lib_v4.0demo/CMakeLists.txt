CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(lib_demo LANGUAGES CXX)

include_directories(src-gen)

add_library(test src-gen/v4/com/bst/os/idl/libtest/libInfStub.cpp)

add_executable(${PROJECT_NAME} main.cpp
    src-gen/v4/com/bst/os/idl/libtest/libInfProxy.cpp)
target_link_libraries(${PROJECT_NAME} test)
