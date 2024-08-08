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
#include "libtool/FInterfaceProxyGenerator.h"
#include "FDModel/FDModel.h"
#include "FDModel/FDModelManager.h"
#include "utilities/string_utility.h"
#include <fstream>

namespace BstIdl
{
std::string FInterfaceProxyGenerator::generateProxyHeader(const std::shared_ptr<FInterface> &fInterface)
{
    std::string content;
    auto typeGen = FTypeGenerator::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    content += getLicense();
    content += "\n" + genExtention.generateBSTLicenseHeader();
    content += "\n" + FTypeGenerator::generateComments(fInterface, false);
    auto name = genExtention.getDefineName(fInterface);
    transform(name.begin(), name.end(), name.begin(), ::toupper);
    content += "\n#ifndef " + name + "_PROXY_HPP_";
    content += "\n#define " + name + "_PROXY_HPP_";
    std::list<std::string> libraryHeaders;
    std::list<std::string> generatedHeaders;
    typeGen.generateRequiredTypeIncludes(fInterface, generatedHeaders, libraryHeaders, true);
    genExtention.generateSelectiveBroadcastStubIncludes(fInterface, generatedHeaders, libraryHeaders);
    for (auto requiredHeaderFile : generatedHeaders)
    {
        content += "\n#include \"" + requiredHeaderFile + "\"";
    }
    content += "\n#include \"" + genExtention.getHeaderPath(fInterface) + "\"\n";

    content += "\n" + genExtention.generateVersionNamespaceBegin(fInterface);
    auto &mgr = FModelManager::getInstance();
    auto model = std::dynamic_pointer_cast<FModel>(fInterface->getContainer());
    content += "\n" + genExtention.generateNamespaceBeginDeclaration(model);
    content += "\nclass " + genExtention.getStubClassName(fInterface) + ";\n";
    content += "class " + getProxyClassName(fInterface);
    content += " : public " + fInterface->getName();
    content += "\n{\npublic:";
    content += "\n\t" + getProxyClassName(fInterface) + "();";
    content += "\n\tvirtual ~" + getProxyClassName(fInterface) + "();";
    content += "\n\t" + getProxyClassName(fInterface) + "(const " + getProxyClassName(fInterface) + " &);";
    content += "\n\t" + getProxyClassName(fInterface) + "(" + getProxyClassName(fInterface) + " &&)noexcept;";
    content += "\n\t" + getProxyClassName(fInterface) + " &operator=(const " + getProxyClassName(fInterface) + " &);";
    content += "\n\t" + getProxyClassName(fInterface) + " &operator=(" + getProxyClassName(fInterface) + " &&);";
    content += "\n";

    auto allInterfaces = mgr.getBaseInterfaces(fInterface);
    for (auto inf : allInterfaces)
    {
        for (auto attribute : inf->getAttributes())
        {
            content += "\n\t" + replace_all(FTypeGenerator::generateComments(attribute, false), "\n", "\n\t");
            content += "\n\t" + genExtention.generateGetMethodDefinition(attribute) + ";";
            content += "\n\t" + genExtention.generateSetMethodDefinition(attribute) + ";";
        }
    }

    for (auto inf : allInterfaces)
    {
        for (auto broadcast : inf->getBroadcasts())
        {
            content += "\n\t" + replace_all(FTypeGenerator::generateComments(broadcast, false), "\n", "\n\t");
            content += "\n\t" + genExtention.generateSetCallbackDefinition(broadcast) + ";\n";
        }
    }

    for (auto inf : allInterfaces)
    {
        for (auto method : inf->getMethods())
        {
            content += "\n\t" + replace_all(FTypeGenerator::generateComments(method, false), "\n", "\n\t");
            content += "\n\t" + genExtention.generateDefinition(method, true) + ";\n";
        }
    }
    content += "\nprivate:\n\tstd::shared_ptr<" + genExtention.getStubClassName(fInterface) + "> m_ptr;\n};";

    content += "\n" + genExtention.generateNamespaceEndDeclaration(model);
    content += genExtention.generateVersionNamespaceEnd(fInterface);
    auto defineName = genExtention.getDefineName(fInterface);
    transform(defineName.begin(), defineName.end(), defineName.begin(), ::toupper);
    content += "\n#endif //" + defineName + "_PROXY_HPP_\n";
    return content;
}

std::string FInterfaceProxyGenerator::generateProxySource(const std::shared_ptr<FInterface> &fInterface)
{
    std::string content;
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    if (!fInterface)
        return "";
    auto stubName = genExtention.getStubClassName(fInterface);
    auto stubHeaderPath = genExtention.getStubHeaderPath(fInterface);
    // checking if exists deployment
    auto &fdmodels = FDModelManager::getInstance();
    for (auto fdmodel : fdmodels.getModelList())
    {
        if (fdmodel)
        {
            auto fdInterface =
                fdmodel->getInterfaceDeployment("com.bst.os.idl.library.deployment", fInterface->getFQN());
            if (fdInterface)
            {
                auto val1 = fdInterface->getSingleValue("UserStubName");
                if (val1 && val1->isString())
                    stubName = val1->getString();
                auto val2 = fdInterface->getSingleValue("UserStubHeaderPath");
                if (val2 && val2->isString())
                    stubHeaderPath = val2->getString();
            }
        }
    }

    content += getLicense();
    content += "\n" + genExtention.generateBSTLicenseHeader();
    content += "\n#include \"" + genExtention.getProxyHeaderPath(fInterface) + "\"\n";
    content += "#include \"" + stubHeaderPath + "\"\n";
    content += "\n" + genExtention.generateVersionNamespaceBegin(fInterface);
    auto &mgr = FModelManager::getInstance();
    auto model = std::dynamic_pointer_cast<FModel>(fInterface->getContainer());
    content += "\n" + genExtention.generateNamespaceBeginDeclaration(model) + "\n";
    content +=
        getProxyClassName(fInterface) + "::" + getProxyClassName(fInterface) + "() : " + fInterface->getName() + "(),";

    content += " m_ptr(std::make_shared<" + stubName + ">()) \n{\n}";

    content += "\n" + getProxyClassName(fInterface) + "::~" + getProxyClassName(fInterface) + "() = default;";
    content += "\n" + getProxyClassName(fInterface) + "::" + getProxyClassName(fInterface) + "(const " +
               getProxyClassName(fInterface) + "&) = default;";
    content += "\n" + getProxyClassName(fInterface) + "::" + getProxyClassName(fInterface) + "(" +
               getProxyClassName(fInterface) + "&&)noexcept = default;";
    content += "\n" + getProxyClassName(fInterface) + " &" + getProxyClassName(fInterface) + "::operator=(const " +
               getProxyClassName(fInterface) + "&) = default;";
    content += "\n" + getProxyClassName(fInterface) + " &" + getProxyClassName(fInterface) + "::operator=(" +
               getProxyClassName(fInterface) + "&&) = default;";

    auto allInterfaces = mgr.getBaseInterfaces(fInterface);
    for (auto inf : allInterfaces)
    {
        for (auto attribute : inf->getAttributes())
        {
            content += "\n" + FTypeGenerator::generateComments(attribute, false);
            content += "\n" + genExtention.generateGetMethodImpl(attribute, getProxyClassName(fInterface)) + "\n{";
            content += "\n\treturn m_ptr->get" + genExtention.getClassName(attribute) + "();\n}";
            content += "\n" + genExtention.generateSetMethodImpl(attribute, getProxyClassName(fInterface)) +
                       "\n{\n\tm_ptr->set" + genExtention.getClassName(attribute) + "(" + attribute->getName() +
                       "_);\n}";
        }
    }

    for (auto inf : allInterfaces)
    {
        for (auto broadcast : inf->getBroadcasts())
        {
            content += "\n" + FTypeGenerator::generateComments(broadcast, false);
            content += "\n" + genExtention.generateSetCallbackImpl(broadcast, getProxyClassName(fInterface)) + "\n{";
            content += "\n\tm_ptr->set" + genExtention.getCallbackName(broadcast) + "(callback_);\n}\n";
        }
    }

    for (auto inf : allInterfaces)
    {
        for (auto method : inf->getMethods())
        {
            content += "\n" + FTypeGenerator::generateComments(method, false);
            content += "\n" + genExtention.generateMethodImpl(method, getProxyClassName(fInterface)) + "\n{";
            content += "\n\tm_ptr->" + method->getName() + "(" + genExtention.generateArgList(method) + ");\n}\n";
        }
    }
    content += "\n" + genExtention.generateNamespaceEndDeclaration(model);
    content += "\n" + genExtention.generateVersionNamespaceEnd(fInterface);
    return content;
}

std::string FInterfaceProxyGenerator::getProxyClassName(const std::shared_ptr<FInterface> &fInterface)
{
    return fInterface->getName() + "Proxy";
}

void FInterfaceProxyGenerator::generateProxy(const std::shared_ptr<FInterface> &fInterface, const std::string &dir)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    auto header_path = gen.getProxyHeaderPath(fInterface);
    auto h_path = mk_dir(dir, header_path);
    if (h_path.empty())
        return;
    auto pos = header_path.find_last_of("/") + 1;
    auto file_name = header_path.substr(pos, header_path.size() - pos);
    std::ofstream OsWrite(h_path + "/" + file_name, std::ofstream::out);
    auto header = generateProxyHeader(fInterface);
    OsWrite << replace_all(header, "\t", "    ");
    OsWrite.close();
    auto source_path = gen.getProxySourcePath(fInterface);
    auto s_path = mk_dir(dir, source_path);
    if (s_path.empty())
        return;
    pos = source_path.find_last_of("/") + 1;
    auto s_file_name = source_path.substr(pos, source_path.size() - pos);
    std::ofstream OsWriteS(s_path + "/" + s_file_name, std::ofstream::out);
    auto source = generateProxySource(fInterface);
    OsWriteS << replace_all(source, "\t", "    ");
    OsWriteS.close();
}
} // namespace BstIdl