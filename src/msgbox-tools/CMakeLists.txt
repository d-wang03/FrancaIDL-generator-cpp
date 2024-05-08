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

PROJECT(msgbox_gen LANGUAGES CXX)

add_definitions(-DCOMMIT_HASH="${COMMIT_HASH}")
message(STATUS "COMMIT_HASH: ${COMMIT_HASH}")

execute_process(
    COMMAND git -C ../../msgbx_ipc rev-parse  --short HEAD
    OUTPUT_VARIABLE COMMIT_HASH2
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_definitions(-DCOMMIT_HASH2="${COMMIT_HASH2}")
message(STATUS "COMMIT_HASH2: ${COMMIT_HASH2}")

file(GLOB MSGBOX_SRCS "*.cpp")
list(SORT MSGBOX_SRCS)

add_library(${PROJECT_NAME} ${MSGBOX_SRCS})