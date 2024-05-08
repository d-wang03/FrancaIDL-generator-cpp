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
#include "asf-tools/asf-gen/CommonapiConfigGenerator.h"
#include "asf-tools/GeneralServiceGeneratorExtensions.h"
#include "capicxx-core-tools/FInterfaceStubGenerator.h"
#include "model/FModelManager.h"
#include "utilities/string_utility.h"
#include <fstream>
#include <iostream>
#include <map>

namespace BstASF
{

void CommonapiConfigGenerator::generateConfig(const std::string &dir)
{
    // write capiConfigHeader
    auto &gen = BstASF::GeneralServiceGeneratorExtensions::getInstance();
    auto configHeaderPath = gen.getCapiConfigHeaderPath();
    auto d_h_path = mk_dir(dir, configHeaderPath);
    if (d_h_path.empty())
        return;
    d_h_path += "/" + gen.getCapiConfigHeaderFile();
    std::ofstream OsWriteH(d_h_path, std::ofstream::out);
    auto configHeader = generateConfigHeader();
    OsWriteH << replace_all(configHeader, "\t", "    ");
    OsWriteH.close();

    // write capiConfigSource
    auto configSourcePath = gen.getCapiConfigSourcePath();
    auto d_s_path = mk_dir(dir, configSourcePath);
    if (d_s_path.empty())
        return;
    d_s_path += "/" + gen.getCapiConfigSourceFile();
    std::ofstream OsWriteS(d_s_path, std::ofstream::out);
    auto configSource = generateConfigSource();
    OsWriteS << replace_all(configSource, "\t", "    ");
    OsWriteS.close();
}

std::string CommonapiConfigGenerator::generateConfigHeader()
{
    std::string header;
    std::string tmp;
    auto &gen = BstASF::GeneralServiceGeneratorExtensions::getInstance();
    header += gen.generateASFLicenseHeader();
    header += "\n" + getLicense();
    auto class_name = gen.getConfigClassName();
    auto define_name = gen.getConfigClassName();
    transform(define_name.begin(), define_name.end(), define_name.begin(), ::toupper);
    header += "\n#ifndef " + define_name + "_H_";
    header += "\n#define " + define_name + "_H_\n";
    // include driver and servers' stubImpl.h
    header += "\n#include \"driver/tcapidriver.h\"\n";
    std::list<std::string> includes;
    for (const auto &instance : gen.getServerInstances())
    {
        auto str = "#include \"" + gen.getStubImplHeaderFile(instance) + "\"";
        addListItem(includes, str);
    }
    header += join(includes, "\n");
    // namespace
    header += "\nnamespace AdvancedServiceFramework\n{";
    // class
    header += "\nclass CAPIConfig : public TCAPIConfig";
    header += "\n{";
    header += "\npublic:";
    header += "\n\tCAPIConfig();";
    header += "\n\tvirtual ~CAPIConfig();";
    header += "\n\n\tvirtual void initialize();";
    header += "\n\tvirtual void terminate();";
    header += "\n\n\tstd::shared_ptr<CommonAPI::Runtime> getRuntime(){return m_runtime;}";
    // getService decl
    std::list<std::string> services;
    for (const auto &instance : gen.getServerInstances())
    {
        std::string service_name(getTargetName(instance));
        auto stubImplName = gen.getStubImplClassName(instance);
        auto mdName = "get" + service_name + "Service";
        auto serviceName = "m_" + toFirstLower(service_name) + "Service";
        header += "\n\tstd::shared_ptr<" + stubImplName + "> " + mdName + "(){return " + serviceName + ";}";
        // private server decl
        auto serverDecl = "\tstd::shared_ptr<" + stubImplName + "> " + serviceName + ";";
        addListItem(services, serverDecl);
    }
    // private:
    header += "\n\nprivate:";
    header += "\n\tstd::shared_ptr<CommonAPI::Runtime> m_runtime;\n";
    header += join(services, "\n");
    header += "\n\n};";
    header += "\n} // namespace AdvancedServiceFramework";
    header += "\n#endif //" + define_name + "_H_";
    return header;
}

std::string CommonapiConfigGenerator::generateConfigSource()
{
    std::string src;
    std::string tmp;
    auto &gen = BstASF::GeneralServiceGeneratorExtensions::getInstance();
    src += gen.generateASFLicenseHeader();
    src += "\n" + getLicense();
    auto class_name = gen.getConfigClassName();
    src += "\n#include \"" + class_name + ".h\"";
    src += "\n#include \"advancedservice.h\"";
    for (const auto &instance : gen.getServerInstances())
    {
        auto logic_file = gen.getLogicHeaderFile(instance);
        src += "\n#include \"" + logic_file + "\"";
    }
    src += "\n#include <unistd.h>\n#include <thread>\n#include <Bstlog/log.h>\n#include <CommonAPI/CommonAPI.hpp>\n";
    // def set_available
    src += "\n#define SET_AVAILABLE(service_name,flag)\tdo\\";
    src += "\n{\\";
    src += "\n\tauto msg = createAvailStatMsg(service_name,flag);\\";
    src += "\n\tdrv.emitSignal(&TDriver::dataReceived, *msg);\\";
    src += "\n}while(0);\n";

    // namespace
    src += "\nnamespace AdvancedServiceFramework\n{";
    // ctor dtor
    src += "\nCAPIConfig::CAPIConfig()\n{\n\t//ctor\n}\n";
    src += "\nCAPIConfig::~CAPIConfig()\n{\n\t//dtor\n}\n";
    // initialize
    src += generateInitialize();
    src += "\nvoid CAPIConfig::terminate()\n{\n\n}";
    src += "\n} // namespace AdvancedServiceFramework";
    return src;
}

std::string CommonapiConfigGenerator::generateInitialize()
{
    auto &gen = BstASF::GeneralServiceGeneratorExtensions::getInstance();
    std::string content("");
    content += "\nvoid CAPIConfig::initialize()\n{";
    content += "\n\tstd::string domain,instance,server;";
    content += "\n\t//Initialize Common API Runtime";
    content += "\n\tm_runtime = CommonAPI::Runtime::get();";
    content += "\n\t//Initialize Common API Services.";
    content += "\n\n\tauto ins = AdvancedService::getInstance();";
    std::list<std::string> initServices;
    for (const auto &instance : gen.getServerInstances())
    {
        auto inf = std::dynamic_pointer_cast<BstIdl::FInterface>(instance->getTarget());
        auto ins_name = getTargetName(instance);
        auto domainName = gen.getDomainName(instance);
        auto instanceName = gen.getInstanceID(instance);
        std::string init = "\n\tdomain = \"" + domainName + "\";";
        init += "\n\tinstance = \"" + instanceName + "\";";
        init += "\n\tserver = domain + \":\"";
        init += " + instance;\n\t";
        auto serverName = "m_" + toFirstLower(ins_name) + "Service";
        auto stubImplName = gen.getStubImplClassName(instance);
        init += serverName + " = std::make_shared<" + stubImplName + ">();";
        init += "\n\twhile (!m_runtime->registerService(domain, instance, " + serverName + "))";
        init += "\n\t{\n\t\tstd::this_thread::sleep_for(std::chrono::milliseconds(100));\n\t}";
        init += "\n\tLOG_INFO(\"Successfully Registered " + ins_name + "!\");";
        auto logicName = gen.getLogicClassName(instance);
        auto logicVarName = ins_name + "Logic";
        init += "\n\tauto " + logicVarName + " = std::dynamic_pointer_cast<" + logicName + ">(ins->getLogic(\"" +
                logicName + "\"));";
        init += "\n\t" + logicVarName + "->setService(" + serverName + ");";
        initServices.emplace_back(init);
    }
    content.append(join(initServices, "\n") + "\n}");
    return content;
}

std::string CommonapiConfigGenerator::getTargetName(std::shared_ptr<BstIdl::FDExtensionElement> instance)
{
    auto &gen = BstASF::GeneralServiceGeneratorExtensions::getInstance();
    auto inf = std::dynamic_pointer_cast<BstIdl::FInterface>(instance->getTarget());
    if (gen.getServer(inf).size() > 1)
        return gen.getInstanceName(instance);
    else
        return gen.getElementName(inf);
}
} // namespace BstASF