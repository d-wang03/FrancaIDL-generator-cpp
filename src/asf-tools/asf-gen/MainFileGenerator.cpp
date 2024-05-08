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
#include "asf-tools/asf-gen/MainFileGenerator.h"
#include "asf-tools/GeneralServiceGeneratorExtensions.h"
#include "asf-tools/MsgType.h"
#include "capicxx-core-tools/FInterfaceStubGenerator.h"
#include "model/FModelManager.h"
#include "utilities/string_utility.h"
#include <fstream>
#include <iostream>
#include <map>

namespace BstASF
{

void MainFileGenerator::generateMain(const std::string &dir)
{
    auto &gen = BstASF::GeneralServiceGeneratorExtensions::getInstance();
    // write main.cpp
    auto mainSourcePath = gen.getMainSourcePath();
    auto d_s_path = mk_dir(dir, mainSourcePath);
    if (d_s_path.empty())
        return;
    d_s_path += "/" + gen.getMainSourceFile();
    std::ofstream OsWriteS(d_s_path, std::ofstream::out);
    auto configSource = generateMainSource();
    OsWriteS << replace_all(configSource, "\t", "    ");
    OsWriteS.close();
}

std::string MainFileGenerator::generateMainSource()
{
    std::string src;
    std::string tmp;
    auto &gen = BstASF::GeneralServiceGeneratorExtensions::getInstance();
    src += gen.generateASFLicenseHeader();
    src += "\n" + getLicense();
    auto class_name = gen.getConfigClassName();

    src += "\n#include <signal.h>\n#include <Bstlog/log.h>";
    // include serverlogic
    std::list<std::string> include_files;
    for (const auto &instance : gen.getServerInstances())
    {
        auto logic_file = gen.getLogicHeaderFile(instance);
        addListItem(include_files, "#include \"" + logic_file + "\"");
    }
    src.append("\n" + join(include_files, "\n"));
    // sigint /sigterm
    src += "\nusing namespace AdvancedServiceFramework;";
    src += "\n\nstatic __sighandler_t s_int_handler, s_ter_handler;";
    src += "\nstatic void sigint_handler(int arg)\n{";
    src += "\n\tauto framework = AdvancedService::getInstance();";
    src += "\n\tframework->stop();";
    src += "\n\ts_int_handler(arg);\n}";
    src += "\n\nstatic void sigterm_handler(int arg)\n{";
    src += "\n\tauto framework = AdvancedService::getInstance();";
    src += "\n\tframework->stop();";
    src += "\n\ts_ter_handler(arg);\n}";

    // main()
    src += "\n\nint main()\n{\n";
    src += "\n\t//get framwork instance.";
    src += "\n\tauto framework = AdvancedService::getInstance();";
    std::string name("");
    std::string log_path("");

    if (gen.getServerInstances().size() > 1)
    {
        name = "AllServices";
        log_path = "asfservicenanolog";
    }
    else if (gen.getServerInstances().size() == 1)
    {
        auto inf_name = gen.getServerInstances().front()->getTarget()->getName();
        name = inf_name;
        log_path = toLower(inf_name) + "nanolog";
    }
    src += "\n\t//set BstLog config";
    src += "\n\tbst::Log::getInstance().setConfig(\"" + name + "\", \"" + log_path + "\", DEBUG);";

    src += generateConfigDriverStr();
    std::string configLogic("\n\t//config logic objects");
    std::string connectObjects(
        "\n\t//connect all "
        "objects\n\tconnect(tcapidriver,&TCAPIDriver::dataReceived,tdispatcher,&TDispatcher::input);");
    std::string addObjects(
        "\n\t//add all objects to "
        "framework\n\tframework->add(std::move(tcapidriver));\n\tframework->add(std::move(tdispatcher));");
    src += "\n" + generateConfigDispatcherStr(configLogic, connectObjects, addObjects);
    src += configLogic;
    src += "\n" + connectObjects;
    src += "\n" + addObjects;

    src += "\n\n\t//start framework";
    src += "\n\tframework->start();";
    src += "\n\n\t//set ctrl + C handler.";
    src += "\n\ts_int_handler = signal(SIGINT,sigint_handler);";
    src += "\n\t//set kill handler.";
    src += "\n\ts_ter_handler = signal(SIGTERM, sigterm_handler);";
    src += "\n\n\t//wait for exit.";
    src += "\n\tframework->waitForExec();";

    src += "\n\treturn 0;\n}";
    return src;
}

// configure driver objects
std::string MainFileGenerator::generateConfigDriverStr()
{
    std::string config("");
    config += "\n\n\t//configure driver objects";
    config += "\n\t//Add & Config Common API Driver.";
    config += "\n\tauto pconfig = std::make_shared<CAPIConfig>();";
    config += "\n\tauto tcapidriver = makeObject<TCAPIDriver>(pconfig);";
    return config;
}

std::string MainFileGenerator::generateConfigDispatcherStr(std::string &configLogic, std::string &connectObjects,
                                                           std::string &addObjects)
{
    auto &gen = BstASF::GeneralServiceGeneratorExtensions::getInstance();
    std::string config("");
    config += "\n\t//config dispatcher object";
    config += "\n\tauto tdispatcher = makeObject<TDispatcher>();";
    int index = 1;
    for (const auto &instance : gen.getServerInstances())
    {
        auto logicClass = gen.getLogicClassName(instance);
        auto InstanceName = toFirstLower(logicClass);
        auto serverName = gen.getServerName(instance);
        auto fInterface = std::dynamic_pointer_cast<BstIdl::FInterface>(instance->getTarget());
        auto slots = gen.getSlotInMap(fInterface);
        auto inf_name = gen.getElementName(fInterface);

        for (const auto &slot : slots)
        {
            auto slotType = MsgType::getByName(gen.getType(slot));
            auto typeName = slot.substr(slotType->getLiteral().size() + inf_name.size() + 2);
            auto output = "&TDispatcher::output<" + std::to_string(index) + ">";
            config += "\n\ttdispatcher->addRoutePath(new TCAPIMsg(TCAPIMsg::Method, \"" + serverName + "\", \"" +
                      typeName + "\"), " + output + ");\n";
            // connect
            connectObjects +=
                "\n\tconnect(tdispatcher, " + output + ", " + InstanceName + ", &" + logicClass + "::" + slot + ");";
            ++index;
        }
        // logic
        configLogic += "\n\tauto " + InstanceName + "= makeObject<" + logicClass + ">();";
        configLogic += "\n\t" + InstanceName + "->setServer(\"" + serverName + "\");";
        // CONNECT
        connectObjects +=
            "\n\tconnect(" + InstanceName + ", &" + logicClass + "::output, tcapidriver, &TCAPIDriver::sendData);";
        if (gen.isRecordAndSimulateInterface(std::dynamic_pointer_cast<BstIdl::FInterface>(instance->getTarget())))
            connectObjects += "\n\tconnect(" + InstanceName + ", &TCAPILogic::record, recorder, &TRecorder::input);";
        // addObjects
        addObjects += "\n\tframework->add(std::move(" + InstanceName + "));";
    }
    // add config tools
    configLogic += "\n\n\t//config tools";
    configLogic +=
        "\n\t//make empty recorder at the beginning, and set the record filename at \"startRecord\" interface.";
    configLogic += "\n\tauto recorder = makeObject<TRecorder>(\"\");";
    configLogic +=
        "\n\t//make empty simulator at the beginning, and set the record filename at \"readRecord\" interface.";
    configLogic += "\n\tauto simulator = makeObject<TSimulator>();";

    if (!gen.getRSInterfaces().empty())
        connectObjects += "\n\tconnect(simulator, &TSimulator::output, tdispatcher, &TDispatcher::input);";

    addObjects += "\n\tframework->add(std::move(recorder));";
    addObjects += "\n\tframework->add(std::move(simulator));";

    return config;
}

std::string MainFileGenerator::generateInitializeServersStr(
    const std::list<std::shared_ptr<BstIdl::FDExtensionElement>> &instances)
{
    auto &gen = BstASF::GeneralServiceGeneratorExtensions::getInstance();
    std::string init("");
    init += "\n\t//Initialize Common API Services.";
    int index = 1;
    for (const auto &instance : instances)
    {
        auto domainName = gen.getDomainName(instance);
        auto instanceName = gen.getInstanceID(instance);
        init += "\n\tdomain = \"" + domainName + "\";";
        init += "\n\tinstance = \"" + instanceName + "\";";
        init += "\n\tserver = domain + \":\" + instance;";
        init += "\n\tTCAPIMsgAvailStatImp msgFor" +
                gen.getElementName(std::dynamic_pointer_cast<BstIdl::FModelElement>(instance->getTarget())) +
                "(server,true);\n\n";
        ++index;
    }

    return init;
}
} // namespace BstASF