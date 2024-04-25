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
project(FDIDLParser)

find_package(BISON)
find_package(FLEX)

BISON_TARGET(FDParser FDParser.y ${CMAKE_CURRENT_BINARY_DIR}/FDParser.cc)
FLEX_TARGET(FDScanner FDScanner.l  ${CMAKE_CURRENT_BINARY_DIR}/FDScanner.cc)
ADD_FLEX_BISON_DEPENDENCY(FDScanner FDParser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(../include)

file(GLOB PARSER_SRCS "*.cpp")

add_library(${PROJECT_NAME} 
  ${BISON_FDParser_OUTPUTS}
  ${FLEX_FDScanner_OUTPUTS}
  ${PARSER_SRCS}
)
# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/*.y ${CMAKE_CURRENT_BINARY_DIR}
#   COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/*.l ${CMAKE_CURRENT_BINARY_DIR}
# )