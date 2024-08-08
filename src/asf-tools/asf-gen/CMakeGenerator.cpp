/*
 * Copyright (c) 2024 Black Sesame Technologies
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "asf-tools/asf-gen/CMakeGenerator.h"
#include "asf-tools/GeneralServiceGeneratorExtensions.h"
#include "model/FModelManager.h"
#include "utilities/list_utility.h"
#include "utilities/string_utility.h"
#include <fstream>
#include <iostream>
#include <map>

namespace BstASF
{

void CMakeGenerator::generateCMake(const std::string &dir, const bool &test_gen)
{
    auto d_path = dir;
    if (d_path.empty())
        return;
    d_path += "/CMakeLists.txt";
    std::ofstream OsWrite(d_path, std::ofstream::out);
    auto cmakeLists = generateCMakeLists(test_gen);
    OsWrite << replace_all(cmakeLists, "\t", "    ");
    OsWrite.close();
}

std::string CMakeGenerator::generateCMakeLists(const bool &test_gen)
{
    std::string prj_name("");
    auto &gen = BstASF::GeneralServiceGeneratorExtensions::getInstance();
    if (gen.getAllServers().empty() || gen.getAllServers().begin()->first == nullptr ||
        gen.getAllServers().begin()->first->getContainer() == nullptr)
        return std::string("");

    std::string content = R"({License}
{ASF_LICENSE_HEADER}
cmake_minimum_required(VERSION 2.10)
)";
    replace_one(content, "{License}", getLicense());
    replace_one(content, "{ASF_LICENSE_HEADER}", gen.generateASFLicenseHeader());
    replace_all(content, "/*", "#");
    replace_all(content, " */", "#");
    replace_all(content, " *", "#");
    content += "\nproject(" + prj_name + ")\n";
    const bool isProjectMode = gen.isProjectMode();
    auto tmp = gen.getAllServers().begin()->first->getContainer()->getName();
    auto container_name = toFirstUpper(tmp.substr(tmp.find_last_of(".") + 1));
    if (BstIdl::FModelManager::getInstance().size() > 1)
        prj_name = "AllServices";
    else if (tmp.find_last_of(".") != tmp.npos)
    {
        prj_name = container_name + "Service";
    }
    content += "\nproject(" + prj_name + ")\n";
    content += R"(
#find CommonAPI CommonAPI-SomeIP vsomeip3 Log
find_package (CommonAPI REQUIRED CONFIG)
find_package (CommonAPI-SomeIP REQUIRED)
find_package (vsomeip3 REQUIRED)
find_package (Log REQUIRED))";
    content += "\nLINK_DIRECTORIES(${Log_LIBRARY_DIRS})";
    content += "\nadd_compile_options(-fno-visibility-inlines-hidden)\n";
    // include_dir
    content += R"(
#set include directories
include_directories(
    ${COMMONAPI_INCLUDE_DIRS}
    ${COMMONAPI_SOMEIP_INCLUDE_DIRS}
    ${VSOMEIP_INCLUDE_DIRS}
    ${Log_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    )";
    std::list<std::string> proxy_gen_srcs;
    std::list<std::string> stub_gen_srcs;
    std::list<std::string> deploy_gen_srcs;
    std::list<std::string> asf_gen_srcs;
    std::list<std::string> svc_srcs;
    std::list<std::string> files;
    std::list<std::string> directories;
    // record & simulate
    std::list<std::string> set_and_file;
    std::list<std::string> add_executable;
    std::list<std::string> target_link;
    std::list<std::string> install_name;

    for (const auto &instance : gen.getServerInstances())
    {
        auto inf = std::dynamic_pointer_cast<BstIdl::FInterface>(instance->getTarget());
        auto name_space = "src-gen/" + gen.getVersionPathPrefix(inf) +
                          gen.getDirectoryPath(std::dynamic_pointer_cast<BstIdl::FModel>(inf->getContainer()));
        addListItem(directories, name_space);
        // srcs
        addListItem(stub_gen_srcs, name_space + "/*StubAdapter.cpp");
        addListItem(deploy_gen_srcs, name_space + "/*Deployment.cpp");
        if (!isProjectMode)
        {
            addListItem(asf_gen_srcs, name_space + "/*Logic*.cpp");
            addListItem(svc_srcs, gen.getLogicSourceFile(instance));
            addListItem(svc_srcs, gen.getStubImplSourceFile(instance));
        }
        else
        {
            addListItem(svc_srcs, gen.getLogicSourcePath(instance));
            addListItem(svc_srcs, gen.getStubImplSourcePath(instance));
        }

        // files
        files.emplace_back(gen.getLogicSourceFile(instance));
        files.emplace_back(gen.getStubImplSourceFile(instance));
        files.emplace_back("src-gen/" + gen.getLogicDefaultSourcePath(inf));

        // record & simulate
        auto inf_name = toFirstUpper(inf->getName());
        auto upper_name = toUpper(inf_name);
        auto record_exename = inf_name + "RecordTest";
        auto simulate_exename = inf_name + "SimulateTest";
        auto record_srcs = "PRJ_" + upper_name + "_RECORD_SRCS";
        auto simulate_srcs = "PRJ_" + upper_name + "_SIMULATE_SRCS";
        // set & file
        set_and_file.emplace_back("\nset(" + record_srcs + " ./" + inf_name + "Record.cpp)");
        set_and_file.emplace_back("set(" + simulate_srcs + " ./" + inf_name + "Simulate.cpp)\n");
        set_and_file.emplace_back("file( GLOB " + record_srcs + "${" + record_srcs + "} ${PRJ_PROXY_GEN_SRCS})");
        set_and_file.emplace_back("file( GLOB " + simulate_srcs + "${" + simulate_srcs + "} ${PRJ_PROXY_GEN_SRCS})\n");
        // add_executable
        add_executable.emplace_back("\nadd_executable(" + record_exename + " ${" + record_srcs + "} )");
        add_executable.emplace_back("add_executable(" + simulate_exename + " ${" + simulate_srcs + "} )\n");
        // link
        target_link.emplace_back("\n\ntarget_link_libraries(" + record_exename +
                                 " CommonAPI vsomeip3 CommonAPI-SomeIP pthread rt)");
        target_link.emplace_back("target_link_libraries(" + simulate_exename +
                                 " CommonAPI vsomeip3 CommonAPI-SomeIP pthread rt)");
    }
    for (const auto &instance : gen.getClientInstances())
    {
        auto inf = std::dynamic_pointer_cast<BstIdl::FInterface>(instance->getTarget());
        auto name_space = "src-gen/" + gen.getVersionPathPrefix(inf) +
                          gen.getDirectoryPath(std::dynamic_pointer_cast<BstIdl::FModel>(inf->getContainer()));
        addListItem(directories, name_space);
        addListItem(proxy_gen_srcs, name_space + "/*Proxy.cpp");
        addListItem(deploy_gen_srcs, name_space + "/*Deployment.cpp");
    }
    content += join(directories, "\n\t");
    content += "\n\tsrc-gen";
    if (isProjectMode)
        content.append("\n\tinclude\n)\n");
    else
        content.append("\n\t)\n");

    // FILE
    content += "\n#glob generated srcs";
    content += "\nfile(GLOB PRJ_PROXY_GEN_SRCS " + join(proxy_gen_srcs, " ") + " )";
    content += "\nfile(GLOB PRJ_STUB_GEN_SRCS " + join(stub_gen_srcs, " ") + " )";
    content += "\nfile(GLOB PRJ_DEPLOY_GEN_SRCS " + join(deploy_gen_srcs, " ") + " )";

    if (isProjectMode)
        content += "\nfile(GLOB PRJ_ASF_GEN_SRCS src-gen/*config.cpp src-gen/*Logic*.cpp )\n";
    else
        content += "\nfile(GLOB PRJ_ASF_GEN_SRCS *config.cpp " + join(asf_gen_srcs, " ") + ")\n";

    // service
    content += "\n#service";
    if (!isProjectMode)
        content += "\nfile(GLOB PRJ_SVC_SRCS main.cpp " + join(svc_srcs, " ") + ")";
    else
        content += "\nfile(GLOB PRJ_SVC_SRCS src/main.cpp " + join(svc_srcs, " ") + ")";
    content.append("\nset(converted_list)\nconvertCode(PRJ_SVC_SRCS converted_list c++)");
    // add_executable record & simulate
    content += "\nadd_executable(" + prj_name +
               " ${converted_list} ${PRJ_ASF_GEN_SRCS} ${PRJ_STUB_GEN_SRCS} ${PRJ_DEPLOY_GEN_SRCS})";

    // link record & simulate
    content += "\ntarget_link_libraries(" + prj_name +
               " advancedservice CommonAPI vsomeip3 CommonAPI-SomeIP ${Log_LIBRARIES} pthread)";
    if (test_gen)
        content.append("\n\nadd_subdirectory(test)");

    content += "\n\ninstall(TARGETS " + prj_name + " " + join(install_name, " ") + ")";
    return content;
}

} // namespace BstASF
