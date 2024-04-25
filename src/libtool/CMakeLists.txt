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
CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(FLibGenerator LANGUAGES CXX)

# find_package(Threads REQUIRED)

# include_directories(../include)

file(GLOB LIB_TOOL_SRCS "*.cpp")
list(SORT LIB_TOOL_SRCS)

# add_executable(${PROJECT_NAME} ${LIB_TOOL_SRCS})
# add_test(${PROJECT_NAME} COMMAND ${PROJECT_NAME})
# target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads FDIDLParser IDLParser FValidator FModel FDModel FGenerator fmt stdc++fs)
add_library(${PROJECT_NAME} ${LIB_TOOL_SRCS})
# target_link_libraries(${PROJECT_NAME} PRIVATE FDIDLParser IDLParser FValidator FModel FDModel FGenerator fmt stdc++fs)
