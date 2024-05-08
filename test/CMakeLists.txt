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

PROJECT(idl_test LANGUAGES CXX)

find_package(Threads REQUIRED)

include_directories(
    ../include
    idlFile)

file(GLOB TEST_SRCS "*.cpp")
list(SORT TEST_SRCS)

add_executable(${PROJECT_NAME} ${TEST_SRCS})
add_test(${PROJECT_NAME} COMMAND ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads GeneralServiceGenerator FDIDLParser IDLParser FValidator FDModel SomeipGenerator CapiGenerator FGenerator FModel fmt gtest stdc++fs)
install(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin/idl-tools/test
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

add_subdirectory(gen-test)

install(DIRECTORY examples DESTINATION bin/idl-tools)