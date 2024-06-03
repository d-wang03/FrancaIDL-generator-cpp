#
# Copyright (c) 2024 Black Sesame Technologies
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
cmake_minimum_required(VERSION 2.10)
project(bst_idl)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules/)
include_directories(include)

add_compile_options("-D_SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING")

add_subdirectory(thirdparty/fmt)
include_directories(thirdparty/fmt/include)
include_directories(thirdparty/nlohmann-json)
include_directories(thirdparty/nlohmann-json/include)
cmake_policy(SET CMP0054 NEW)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    include_directories(thirdparty/getopt)
    include_directories(thirdparty/googletest/googletest/include)
    include_directories(thirdparty/googletest/googlemock/include)
    add_subdirectory(thirdparty/googletest)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
message(STATUS "Binary Output to : ${PROJECT_BINARY_DIR}")

execute_process(
    COMMAND git rev-parse --short HEAD
    OUTPUT_VARIABLE COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_subdirectory(src/model)
add_subdirectory(src/FDModel)
add_subdirectory(src/validator)
add_subdirectory(src/parser)
add_subdirectory(src/FDParser)
add_subdirectory(src/generator)
add_subdirectory(src/libtool)
add_subdirectory(src/capicxx-core-tools)
add_subdirectory(src/someipTools)
add_subdirectory(src/application)
add_subdirectory(src/asf-tools)
add_subdirectory(src/ipc-gen)
add_subdirectory(src/msgbox-tools)
add_subdirectory(test)

install(DIRECTORY plugin DESTINATION bin/idl-tools)

install(DIRECTORY idlFile DESTINATION bin/idl-tools)