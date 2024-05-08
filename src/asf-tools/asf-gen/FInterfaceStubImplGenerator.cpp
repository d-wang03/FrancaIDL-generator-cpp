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
#include "asf-tools/asf-gen/FInterfaceStubImplGenerator.h"
#include "asf-tools/GeneralServiceGeneratorExtensions.h"
#include "FDModel/FDeployedInterface.h"
#include "capicxx-core-tools/FInterfaceStubGenerator.h"
#include "model/FModelManager.h"
#include "someipTools/SomeipPropertyAccessor.h"
#include "utilities/string_utility.h"
#include <fstream>
#include <iostream>
#include <map>

namespace BstASF
{

void FInterfaceStubImplGenerator::generateStubImpl(
    const std::shared_ptr<BstIdl::FInterface> &fInterface,
    const std::shared_ptr<BstCommonAPI::CommonapiPropertyAccessor> &deploymentAccessor, const std::string &dir)
{
    auto &gen = BstASF::GeneralServiceGeneratorExtensions::getInstance();
    auto instances = gen.getServer(fInterface);
    if (instances.empty())
        return;

    // methods replies
    clear();
    auto baseInterfaces = BstIdl::FModelManager::getInstance().getBaseInterfaces(fInterface);
    for (const auto &inf : baseInterfaces)
    {
        for (const auto &method : inf->getMethods())
        {
            if (!gen.isFireAndForgetMethod(method))
                generateMethodReplyDeclarations(deploymentAccessor, method, inf);
        }
    }
    // generate stubImpl for all instances targetting the fInterface.
    for (const auto &instance : instances)
    {
        // write stubImplHeader
        auto stubImplHeaderPath = gen.getStubImplHeaderPath(instance);
        auto d_h_path = mk_dir(dir, stubImplHeaderPath);
        if (d_h_path.empty())
            return;
        d_h_path += "/" + gen.getStubImplHeaderFile(instance);
        std::ofstream OsWriteH(d_h_path, std::ofstream::out);
        auto stubImplHeader = generateStubImplHeader(instance, deploymentAccessor);
        OsWriteH << replace_all(stubImplHeader, "\t", "    ");
        OsWriteH.close();

        // write stubImplSource
        auto stubImplSourcePath = gen.getStubImplSourcePath(instance);
        auto d_s_path = mk_dir(dir, stubImplSourcePath);
        if (d_s_path.empty())
            return;
        d_s_path += "/" + gen.getStubImplSourceFile(instance);
        std::ofstream OsWriteS(d_s_path, std::ofstream::out);
        auto stubImplSource = generateStubImplSource(instance, deploymentAccessor);
        OsWriteS << replace_all(stubImplSource, "\t", "    ");
        OsWriteS.close();
    }
}

std::string FInterfaceStubImplGenerator::generateStubImplHeader(
    const std::shared_ptr<BstIdl::FDExtensionElement> &instance,
    const std::shared_ptr<BstCommonAPI::CommonapiPropertyAccessor> &deploymentAccessor)
{
    auto &gen = BstASF::GeneralServiceGeneratorExtensions::getInstance();
    auto fInterface = std::dynamic_pointer_cast<BstIdl::FInterface>(instance->getTarget());
    if (!fInterface || !fInterface->getContainer())
        return "";

    std::string header;
    header += gen.generateASFLicenseHeader();
    header += "\n" + getLicense();
    auto class_name = gen.getStubImplClassName(instance);
    auto define_name = gen.getProjectDefineName(instance);
    transform(define_name.begin(), define_name.end(), define_name.begin(), ::toupper);
    header += "\n#ifndef " + define_name + "_STUB_IMPL_H_";
    header += "\n#define " + define_name + "_STUB_IMPL_H_\n";
    header += "\n#include \"typeconversion.h\"";
    header += "\n#include <CommonAPI/CommonAPI.hpp>";
    header += "\n#include <" + gen.getStubDefaultHeaderPath(fInterface) + ">";
    header += "\n#include <AdvancedServiceFramework>\n";
    header += "\nusing namespace " + fInterface->getNameSpace() + ";";
    header += "\nusing namespace AdvancedServiceFramework;\n";

    // class
    header += "\nclass " + class_name;
    auto tmp = replace_all(gen.getStubDefaultHeaderPath(fInterface), "/", "::");
    auto pos = tmp.find_last_of(".");
    tmp = tmp.substr(0, pos);
    header += " : public " + tmp;
    header += "\n{\n\npublic:";
    header += "\n\t" + class_name + "();";
    header += "\n\tvirtual ~" + class_name + "();";
    // method decl
    std::map<std::string, bool> replies;
    auto baseInterfaces = BstIdl::FModelManager::getInstance().getBaseInterfaces(fInterface);
    for (const auto &inf : baseInterfaces)
    {
        for (auto method : inf->getMethods())
        {
            if (gen.isRecordAndSimulateInterface(inf) && contains(gen.record_simulate_methods, method->getName()) &&
                inf != fInterface)
                continue;
            header += "\n" + BstCommonAPI::FTypeGenerator::generateComments(method, false);
            if (m_methodrepliesMap.find(method) != m_methodrepliesMap.end())
                replies = m_methodrepliesMap.find(method)->second;
            header += "\n\tvirtual void " + gen.getElementName(method) + "(" +
                      gen.generateOverloadedStubSignatureWithNamespace(method, replies) + ");";
        }
    }

    header.append("\n\nprivate:\n\tint m_record = 0;");
    header += "\n\n};";
    header += "\n#endif //" + define_name + "_STUB_IMPL_H_";
    return header;
}

std::string FInterfaceStubImplGenerator::generateStubImplSource(
    const std::shared_ptr<BstIdl::FDExtensionElement> &instance,
    const std::shared_ptr<BstCommonAPI::CommonapiPropertyAccessor> &deploymentAccessor)
{
    auto &gen = BstASF::GeneralServiceGeneratorExtensions::getInstance();
    auto fInterface = std::dynamic_pointer_cast<BstIdl::FInterface>(instance->getTarget());
    if (!fInterface || !fInterface->getContainer())
        return "";

    std::string header;
    header += gen.generateASFLicenseHeader();
    header += "\n" + getLicense();
    auto class_name = gen.getStubImplClassName(instance);
    auto define_name = gen.getStubImplClassName(instance);
    transform(define_name.begin(), define_name.end(), define_name.begin(), ::toupper);
    // include && namespace
    header += "\n#include <algorithm>\n#include <fstream>";
    header += "\n#include \"" + gen.getStubImplHeaderFile(instance) + "\"";
    header += "\nusing namespace " + fInterface->getNameSpace() + ";";
    header += "\nusing namespace AdvancedServiceFramework;\n";
    // ctr and dectr
    header += "\n" + class_name + "::" + class_name + "()\n{";
    header += "\n\n}";
    header += "\n" + class_name + "::~" + class_name + "()\n{";
    header += "\n\n}\n";
    // methods def
    std::map<std::string, bool> replies;
    auto serverName = gen.getServerName(instance);
    auto baseInterfaces = BstIdl::FModelManager::getInstance().getBaseInterfaces(fInterface);
    for (const auto &inf : baseInterfaces)
    {
        for (const auto &method : inf->getMethods())
        {
            if (gen.isRecordAndSimulateInterface(inf) && contains(gen.record_simulate_methods, method->getName()) &&
                inf != fInterface)
                continue;
            header += "\n" + BstCommonAPI::FTypeGenerator::generateComments(method, false);
            if (m_methodrepliesMap.find(method) != m_methodrepliesMap.end())
                replies = m_methodrepliesMap.find(method)->second;
            header += gen.generateStubMethodDefine(method, replies, serverName, instance);
        }
    }
    return header;
}

std::string FInterfaceStubImplGenerator::generateMethodReplyDeclarations(
    const std::shared_ptr<BstCommonAPI::CommonapiPropertyAccessor> &deploymentAccessor,
    const std::shared_ptr<BstIdl::FMethod> &fMethod, const std::shared_ptr<BstIdl::FInterface> &fInterface)
{
    std::string str;
    auto &gen = BstASF::GeneralServiceGeneratorExtensions::getInstance();
    std::map<std::string, bool> replies;
    if (m_methodrepliesMap.find(fMethod) != m_methodrepliesMap.end())
    {
        if (!m_methodrepliesMap[fMethod].empty())
            replies = m_methodrepliesMap[fMethod];
    }
    if (m_counterMap.find(gen.getElementName(fMethod)) == m_counterMap.end())
    {
        replies[gen.getElementName(fMethod)] = false;
        m_counterMap[gen.getElementName(fMethod)] = 0;
    }
    else
    {
        m_counterMap[gen.getElementName(fMethod)] = m_counterMap.find(gen.getElementName(fMethod))->second + 1;
        auto reply = gen.getElementName(fMethod) + std::to_string(m_counterMap[gen.getElementName(fMethod)]);
        replies[reply] = false;
    }
    for (auto broadcast : fInterface->getBroadcasts())
    {
        if (gen.isErrorType(broadcast, fMethod, deploymentAccessor))
        {
            auto errorReply = gen.getElementName(fMethod) + toFirstUpper(gen.getElementName(broadcast));
            replies[errorReply] = true;
        }
    }
    m_methodrepliesMap[fMethod] = replies;
    // if mainInterface, insert in generator.m_methodReplies
    auto modelName = fInterface->getContainer()->getName();
    auto &mrMap = gen.getMethodRepliesMap();
    mrMap.emplace(fMethod, replies);
    return str;
}
} // namespace BstASF
