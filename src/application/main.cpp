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
#include "FDModel/FDInterface.h"
#include "FDParser/FDParser.h"
#include "parser/FidlParser.h"

#include "asf-tools/GeneralServiceGenerator.h"
#include "asf-tools/GeneralServiceGeneratorExtensions.h"
#include "capicxx-core-tools/CapiGenerator.h"
#include "generator/CapiTestcaseGenerator.h"
#include "generator/HTMLGenerator.h"
#include "ipc-gen/ipcGen.h"
#include "nlohmann/json.hpp"
#include "libtool/FLibraryGenerator.h"
#include "msgbox-tools/MsgBoxGenerator.h"
#include "someipTools/SomeipGenerator.h"
#include "utilities/list_utility.h"
#include "utilities/path_utility.h"
#include "utilities/string_utility.h"
#include <experimental/filesystem>
#include <fstream>
#include <getopt.h>
#include <sstream>
#include <unistd.h>
#include "model/FIntegerInterval.h"

using json = nlohmann::json;
namespace fs = std::experimental::filesystem;

void usage(void)
{
    std::cout << "usage: bst_idl_code_gen [[-t TYPE] [-d DIR] [-l LIC] [-s] [-P] xxx.fidl/xxx.fdepl] [-v] [-h]"
              << std::endl;
    std::cout << "BST IDL Code Generator" << std::endl;
    std::cout << "-t TYPE, --type=TYPE\tSet code type to be generated. Values can be: doc, capi, someip, lib, ipc, "
                 "asf, test, msgbx, auto."
                 "When not set, the default type is auto. The type will be deducted from fdepl files."
              << std::endl;
    std::cout << "-d DIR, --dest=DIR\tSet output directory, Optional, default is current directory." << std::endl;
    std::cout << "-l LIC, --license=LIC\tSet license file, Optional, default is BST license." << std::endl;
    std::cout << "-s, --stub\tGenerate stub code. It only used in capi and lib."
                 "By default, the capi/lib generates proxy code for client usage."
                 "This option will generate stub code for service, additionally."
              << std::endl;
    std::cout << "-P, --project\tGenerate as project mode. By default, the asf_tool generates as make mode."
                 "This option will generate asf code to src, include and src-gen directories."
                 "\n\tOnly this is on, can asf gen-code add recording and simulating features. "
              << std::endl;
    std::cout << "-v, --version\tShow version info." << std::endl;
    std::cout << "-h, --help\tShow help info." << std::endl;
    std::cout << "-T\tTemporary features." << std::endl;
}

static struct option long_options[] = {
    {"type", optional_argument, NULL, 't'},    {"dest", optional_argument, NULL, 'd'},
    {"license", optional_argument, NULL, 'l'}, {"stub", no_argument, NULL, 's'},
    {"project", no_argument, NULL, 'P'},       {"version", no_argument, NULL, 'v'},
    {"help", no_argument, NULL, 'h'},          {0, 0, 0, 0}};

void version(void)
{
    std::cout << "BST IDL Code Generator, compatible to :" << std::endl;
    std::cout << "*    org.franca.core 0.12.0.1\n"
                 "*    org.genivi.commonapi.core 3.2.0\n"
                 "*    org.genivi.commonapi.someip 3.2.0.1\n"
                 "*    com.bst.ipc.deployment (Message Box) "
              << BstIdl::MsgBoxGenerator::version() << "\n*    commit hash " << BstIdl::MsgBoxGenerator::commit_hash()
              << std::endl;
}

void temporary(void)
{
    std::string packageName;
    std::string infName;
    std::string methodName;
    std::string broadcastName;

    std::cout << "Let's generate fidl/fdepl files.\n"
                 "Please input the package name: "
              << std::endl;
    std::cin >> packageName;
    std::cout << "Please input the interface name: " << std::endl;
    std::cin >> infName;
    std::cout << "Now the full qualified name (FQN) is " << packageName << "." << infName
              << "\nPlease input a method name that contained in " << infName << std::endl;
    std::cin >> methodName;
    std::cout << "Please input a broadcast name that contained in " << infName << std::endl;
    std::cin >> broadcastName;

    std::cout << "Here is the content for generated fidl:" << std::endl;
    std::cout << "\npackage " << packageName << "\ninterface " << infName << "\n{\n\tversion {major 0 minor 1}"
              << "\n\tmethod " << methodName << "\n\t{\n\t\tin\n\t\t{\n\t\t}\n\t\tout\n\t\t{\n\t\t}\n\t}\n"
              << "\n\tbroadcast " << broadcastName << "\n\t{\n\t\tin\n\t\t{\n\t\t}\n\t}\n"
              << "\n}\n"
              << std::endl;
}

bool generateByType(const std::string &type, const fs::path &dest_dir, bool generate_stub, const std::string &license)
{
    bool ret = false;
    if (type == "doc")
    {
        std::cout << "Generating API document...";
        auto generator = std::make_shared<BstIdl::HTMLGenerator>(dest_dir.string());
        generator->setLicense(license);
        ret = generator->generate();
        std::cout << "Done!" << std::endl;
    }
    else if (type == "asf")
    {
        auto src_gen_dir = dest_dir;
        auto generator = std::make_shared<BstASF::GeneralServiceGenerator>(src_gen_dir.string(), generate_stub);
        generator->setLicense(license);
        ret = generator->generate();
    }
    else if (type == "capi")
    {
        auto src_gen_dir = dest_dir;
        auto generator =
            std::make_shared<BstCommonAPI::CommonapiGenerator>(src_gen_dir.append("src-gen").string(), generate_stub);
        generator->setLicense(license);
        ret = generator->generate();
    }
    else if (type == "someip")
    {
        auto src_gen_dir = dest_dir;
        auto generator =
            std::make_shared<BstCommonAPI::SomeipGenerator>(src_gen_dir.append("src-gen").string(), generate_stub);
        generator->setLicense(license);
        ret = generator->generate();
    }
    else if (type == "lib")
    {
        auto src_gen_dir = dest_dir;
        auto generator =
            std::make_shared<BstIdl::FLibraryGenerator>(src_gen_dir.append("src-gen").string(), generate_stub);
        generator->setLicense(license);
        ret = generator->generate();
    }
    else if (type == "ipc")
    {
        auto src_gen_dir = dest_dir;
        auto generator = std::make_shared<BstIdl::IPCGenerator>(src_gen_dir.append("src-gen").string(), generate_stub);
        generator->setLicense(license);
        ret = generator->generate();
    }
    else if (type == "msgbx")
    {
        auto src_gen_dir = dest_dir;
        auto generator = std::make_shared<BstIdl::MsgBoxGenerator>(src_gen_dir.append("src-gen").string());
        generator->setLicense(license);
        ret = generator->generate();
    }
    else if (type == "graph")
    {
        // generate graph files.
        std::error_code ec;
        auto graph_dir = dest_dir;
        if (!fs::create_directory(graph_dir.append("graph").string(), ec) && ec)
        {
            std::cerr << "Creating directory failed! " << ec.message() << std::endl;
            ret = false;
        }
        else
        {
            for (const auto &item : BstIdl::FDModelManager::getInstance().getModelList())
            {
                std::ofstream fdmodelWritter(graph_dir.string() + "/" + item->getName() + ".wsd", std::ofstream::out);
                fdmodelWritter << item->toGraph();
                fdmodelWritter.close();
            }
            ret = true;
        }
    }
    else if (type == "test")
    {
        auto src_gen_dir = dest_dir;
        auto generator = std::make_shared<BstIdl::CapiTestcaseGenerator>(src_gen_dir.append("test").string());
        generator->setLicense(license);
        ret = generator->generate();
    }
    else
    {
        std::cerr << "Unknown type " << type << std::endl;
    }
    return ret;
}

int main(int argc, char *argv[])
{
    CurrentAppPath::getInstance().currentAppPath(argv[0]);
    auto dest_dir = fs::current_path();
    std::string license;
    bool generate_stub = false;
    bool project_mode = false;
    std::string type;
    int32_t opt = 0;
    int32_t option_index = 0;
    opterr = 0;
    bool use_dest_dir = false;
    while ((opt = getopt_long(argc, argv, "sd:t:l:vhTP", long_options, &option_index)) != -1)
    {
        switch (opt)
        {
        case 's':
            generate_stub = true;
            BstASF::GeneralServiceGeneratorExtensions::getInstance().setGenStub();
            break;
        case 'd': {
            fs::path outpath(optarg);
            if (fs::exists(outpath) && fs::is_directory(outpath))
            {
                dest_dir = fs::canonical(std::move(outpath));
                use_dest_dir = true;
            }
            else
            {
                std::cerr << optarg << " not exists or not a directory." << std::endl;
                return 0;
            }
            break;
        }
        case 'P': {
            project_mode = true;
            BstASF::GeneralServiceGeneratorExtensions::getInstance().setProjectMode();
            break;
        }
        case 'l': {
            fs::path licFile(optarg);
            if (fs::exists(licFile) && fs::is_regular_file(licFile))
            {
                std::ifstream ifs(licFile.string());
                if (!ifs.is_open())
                {
                    std::cerr << "Failed to open " << licFile << std::endl;
                    return 0;
                }
                else
                {
                    std::stringstream ss;
                    ss << ifs.rdbuf();
                    ifs.close();
                    license = ss.str();
                }
            }
            else
            {
                std::cerr << optarg << " not exists or not a regular file." << std::endl;
                return 0;
            }
            break;
        }
        case 't':
            type = optarg;
            break;
        case 'v':
            version();
            return 0;
        case 'T':
            temporary();
            return 0;
        case 'h':
        default:
            usage();
            return 0;
        }
    }

    if (argc != optind + 1)
    {
        usage();
        return 0;
    }

    // get full path of source .fidl/.fdepl file.
    auto fullpath = fs::path(argv[optind]);
    if (!fs::exists(fullpath))
    {
        std::cerr << fullpath << " does NOT exits!" << std::endl;
        return 0;
    }
    else if (fs::is_directory(fullpath))
    {
        std::cerr << fullpath << " is a directory!" << std::endl;
        return 0;
    }

    // calculate abspath.
    auto abspath = fs::canonical(std::move(fullpath));
    if (!use_dest_dir)
        dest_dir.append(abspath.stem().string());

    // change current path to the source file path.
    fs::current_path(abspath.parent_path());

    if (type.empty())
        type = "auto";

    // print start information.
    std::cout << "===========================================================" << std::endl;
    version();
    std::cout << "Source file        : " << abspath << std::endl;
    if (project_mode)
        std::cout << "Generation mode    : project" << std::endl;
    else
        std::cout << "Generation mode    : make" << std::endl;
    std::cout << "Generation type    : " << type << std::endl;
    // std::cout << "Generate stub code : " << (generate_stub ? "true" : "false") << std::endl;
    std::cout << "Output dir         : " << dest_dir << std::endl;

    // parse file
    std::cout << "Parsing model files..." << std::endl;
    auto ext = abspath.extension().string();
    if (ext == ".fidl")
    {
        BstIdl::FidlParser parser;
        auto ret = parser.parse(abspath.filename().string());
        if (ret.empty())
        {
            std::cerr << "Parse Failed!" << std::endl;
            return 1;
        }
    }
    else if (ext == ".fdepl")
    {
        BstIdl::FDParser parser;
        auto ret = parser.parse(abspath.filename().string());
        if (ret.empty())
        {
            std::cerr << "Parse Failed!" << std::endl;
            return 2;
        }
    }
    else
    {
        std::cout << "Failed! Unsupported file." << std::endl;
        return 0;
    }
    std::cout << "Parsing Complete!" << std::endl;

    // start code generation.
    bool ret = true;
    if (type.empty())
        type = "auto";

    if (type == "auto")
    {
        std::list<std::string> spec_names;
        for (const auto &fdmodel : BstIdl::FDModelManager::getInstance().getModelList())
        {
            for (const auto &spec : fdmodel->getSpecifications())
            {
                if (spec)
                    spec_names.emplace_back(spec->getName());
            }
        }
        if (ret && contains(spec_names, "com.bst.os.idl.asf.deployment"))
        {
            std::cout << "Found Deployment Type: ASF" << std::endl;
            ret = generateByType("asf", dest_dir, generate_stub, license);
        }
        if (ret && contains(spec_names, "org.genivi.commonapi.core.deployment"))
        {
            std::cout << "Found Deployment Type: Common API" << std::endl;
            ret = generateByType("capi", dest_dir, generate_stub, license);
        }
        if (ret && contains(spec_names, "org.genivi.commonapi.someip.deployment"))
        {
            std::cout << "Found Deployment Type: SOME/IP" << std::endl;
            ret = generateByType("someip", dest_dir, generate_stub, license);
        }
        if (ret && contains(spec_names, "com.bst.os.idl.test.deployment"))
        {
            std::cout << "Found Deployment Type: Interface Test" << std::endl;
            ret = generateByType("test", dest_dir, generate_stub, license);
        }
        if (ret && contains(spec_names, "com.bst.os.idl.ipc.deployment"))
        {
            std::cout << "Found Deployment Type: Inter Processor Communication" << std::endl;
            ret = generateByType("ipc", dest_dir, generate_stub, license);
        }
        if (ret && contains(spec_names, "com.bst.os.idl.library.deployment"))
        {
            std::cout << "Found Deployment Type: Library" << std::endl;
            ret = generateByType("lib", dest_dir, generate_stub, license);
        }
        if (ret && contains(spec_names, "com.bst.ipc.deployment"))
        {
            std::cout << "Found Deployment Type: Message Box" << std::endl;
            ret = generateByType("msgbx", dest_dir, generate_stub, license);
        }
    }
    else
    {
        ret = generateByType(type, dest_dir, generate_stub, license);
    }

    if (ret)
        std::cout << "Code generation complete." << std::endl;
    else
        std::cerr << "Code generation failed." << std::endl;
}
