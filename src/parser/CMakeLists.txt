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
project(IDLParser)

find_package(BISON)
find_package(FLEX)

BISON_TARGET(FidlParser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cc)
FLEX_TARGET(FidlScanner scanner.l  ${CMAKE_CURRENT_BINARY_DIR}/scanner.cc)
ADD_FLEX_BISON_DEPENDENCY(FidlScanner FidlParser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(../include)

file(GLOB PARSER_SRCS "*.cpp")

add_library(${PROJECT_NAME} 
  ${BISON_FidlParser_OUTPUTS}
  ${FLEX_FidlScanner_OUTPUTS}
  ${PARSER_SRCS}
)