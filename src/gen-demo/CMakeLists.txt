CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(gen_test LANGUAGES CXX)

find_package(Threads REQUIRED)

include_directories(
    ../include)

file(GLOB TEST_SRCS "*.cpp")
list(SORT TEST_SRCS)

add_executable(${PROJECT_NAME} ${TEST_SRCS})
add_test(${PROJECT_NAME} COMMAND ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads GeneralserviceGenerator FDIDLParser IDLParser FValidator FDModel SomeipGenerator CapiGenerator FGenerator FModel fmt gtest stdc++fs)

install(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin/idl-tools/test
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

install(DIRECTORY ../../newidlFile DESTINATION bin/idl-tools
)

install(DIRECTORY ../../idlFile DESTINATION bin/idl-tools
)

install(DIRECTORY ../../plugin DESTINATION bin/idl-tools
)

install(DIRECTORY auto-startup_demo DESTINATION bin/idl-tools/examples
)

install(DIRECTORY test_demo DESTINATION bin/idl-tools/examples
)

install(DIRECTORY proxytest_demo DESTINATION bin/idl-tools/examples
)

install(DIRECTORY doc_demo DESTINATION bin/idl-tools/examples
)

install(DIRECTORY proxytest_demo DESTINATION bin/idl-tools/examples
)

install(DIRECTORY lib_v4.0demo DESTINATION bin/idl-tools/examples/libtest
)