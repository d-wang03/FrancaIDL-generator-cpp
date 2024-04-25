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
#include "libtool/FInterfaceStubGenerator.h"
#include "utilities/string_utility.h"

namespace BstIdl
{
std::string FInterfaceStubGenerator::generateStubHeader(const std::shared_ptr<FInterface> &fInterface)
{
    std::string header;
    auto typeGen = FTypeGenerator::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    header += genExtention.generateBSTLicenseHeader();
    header += "\n" + FTypeGenerator::generateComments(fInterface, false);
    auto name = genExtention.getDefineName(fInterface);
    transform(name.begin(), name.end(), name.begin(), ::toupper);
    header += "\n#ifndef " + name + "_STUB_HPP_";
    header += "\n#define " + name + "_STUB_HPP_";
    std::list<std::string> libraryHeaders;
    std::list<std::string> generatedHeaders;
    typeGen.generateInheritanceIncludes(fInterface, generatedHeaders, libraryHeaders);
    typeGen.generateRequiredTypeIncludes(fInterface, generatedHeaders, libraryHeaders, true);
    genExtention.generateSelectiveBroadcastStubIncludes(fInterface, generatedHeaders, libraryHeaders);
    for (auto requiredHeaderFile : generatedHeaders)
    {
        header += "\n#include \"" + requiredHeaderFile + "\"";
    }
    header += "\n#include \"" + genExtention.getHeaderPath(fInterface) + "\"\n";

    header += "\n" + genExtention.generateVersionNamespaceBegin(fInterface);
    auto model = std::dynamic_pointer_cast<FModel>(fInterface->getContainer());
    header += "\n" + genExtention.generateNamespaceBeginDeclaration(model);

    header += "class " + genExtention.getStubClassName(fInterface);
    header += " : public " + fInterface->getName();
    if (fInterface->getBase() != nullptr)
        header += ", public " + genExtention.getTypeCollectionName(fInterface->getBase(), fInterface) + "Stub";
    header += "\n{\npublic:";
    header += "\n\t" + genExtention.getStubClassName(fInterface) + "();";
    header += "\n\tvirtual ~" + genExtention.getStubClassName(fInterface) + "();";
    header += "\n\t" + genExtention.getStubClassName(fInterface) + "(const " +
              genExtention.getStubClassName(fInterface) + " &);";
    header += "\n\t" + genExtention.getStubClassName(fInterface) + "(" + genExtention.getStubClassName(fInterface) +
              " &&)noexcept;";
    header += "\n\t" + genExtention.getStubClassName(fInterface) + " &operator=(const " +
              genExtention.getStubClassName(fInterface) + " &);";
    header += "\n\t" + genExtention.getStubClassName(fInterface) + " &operator=(" +
              genExtention.getStubClassName(fInterface) + " &&);\n";
    for (auto attribute : fInterface->getAttributes())
    {
        header += "\n\t" + replace_all(FTypeGenerator::generateComments(attribute, false), "\n", "\n\t");
        header += "\n\t" + genExtention.generateGetMethodDefinition(attribute) + "\n\t{";
        header += "\n\t\treturn m_" + attribute->getName() + ";\n\t}";
        header += "\n\t" + genExtention.generateSetMethodDefinition(attribute) + "\n\t{";
        header += "\n\t\tm_" + attribute->getName() + " = " + attribute->getName() + "_;\n\t}\n";
    }

    for (auto broadcast : fInterface->getBroadcasts())
    {
        header += "\n\t" + replace_all(FTypeGenerator::generateComments(broadcast, false), "\n", "\n\t");
        header += "\n\t" + genExtention.generateSetCallbackDefinition(broadcast) + "\n\t{";
        std::string callbackName = toFirstLower(genExtention.getCallbackName(broadcast));
        header += "\n\t\tm_" + callbackName + " = callback_;\n\t}\n";
        header += "\n\t" + genExtention.generateFireBroadcastDefinition(broadcast) + "\n\t{";
        header += "\n\t\tif (m_" + callbackName + ")\n";
        header += "\t\t\tm_" + callbackName + "(";
        for (auto outArg : broadcast->getOutArgs())
        {
            if (outArg == broadcast->getOutArgs().front())
            {
                header += outArg->getName();
            }
            else
            {
                header += "," + outArg->getName();
            }
        }
        header += ");\n\t}\n";
    }

    for (auto method : fInterface->getMethods())
    {
        header += "\n\t" + replace_all(FTypeGenerator::generateComments(method, false), "\n", "\n\t");
        header += "\n\tvirtual " + genExtention.generateDefinition(method, true) + ";\n";
    }
    header += "\nprotected:\n\t";

    for (auto attribute : fInterface->getAttributes())
    {
        header += genExtention.getTypeName(attribute, attribute, false) + " m_" + attribute->getName() + ";\n\t";
    }

    for (auto broadcast : fInterface->getBroadcasts())
    {
        std::string callbackName = toFirstLower(genExtention.getCallbackName(broadcast));
        header += genExtention.getCallbackName(broadcast) + " m_" + callbackName + ";\n\t";
    }
    header += "\n};\n";

    header += "\n" + genExtention.generateNamespaceEndDeclaration(model);
    header += genExtention.generateVersionNamespaceEnd(fInterface);
    header += "\n\n" + genExtention.generateMajorVersionNamespace(fInterface);
    auto defineName = genExtention.getDefineName(fInterface);
    transform(defineName.begin(), defineName.end(), defineName.begin(), ::toupper);
    header += "\n#endif //" + defineName + "_STUB_HPP_\n";
    return header;
}
std::string FInterfaceStubGenerator::generateStubSource(const std::shared_ptr<FInterface> &fInterface)
{
    std::string header;
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    header += genExtention.generateBSTLicenseHeader();
    header += "\n#include \"" + genExtention.getStubHeaderPath(fInterface) + "\"\n";
    header += "\n" + genExtention.generateVersionNamespaceBegin(fInterface);
    auto &mgr = FModelManager::getInstance();
    auto model = std::dynamic_pointer_cast<FModel>(fInterface->getContainer());
    header += "\n" + genExtention.generateNamespaceBeginDeclaration(model);

    header += "\n" + genExtention.getStubClassName(fInterface) + "::~" + genExtention.getStubClassName(fInterface) +
              "() = default;";
    header += "\n" + genExtention.getStubClassName(fInterface) + "::" + genExtention.getStubClassName(fInterface) +
              "() = default;";
    header += "\n" + genExtention.getStubClassName(fInterface) + "::" + genExtention.getStubClassName(fInterface) +
              "(const " + genExtention.getStubClassName(fInterface) + " &) = default;";
    header += "\n" + genExtention.getStubClassName(fInterface) + "::" + genExtention.getStubClassName(fInterface) +
              "(" + genExtention.getStubClassName(fInterface) + " &&)noexcept = default;";
    header += "\n" + genExtention.getStubClassName(fInterface) + " &" + genExtention.getStubClassName(fInterface) +
              "::operator=(const " + genExtention.getStubClassName(fInterface) + " &) = default;";
    header += "\n" + genExtention.getStubClassName(fInterface) + " &" + genExtention.getStubClassName(fInterface) +
              "::operator=(" + genExtention.getStubClassName(fInterface) + " &&) = default;";

    for (auto method : fInterface->getMethods())
    {
        header += "\n\n" + FTypeGenerator::generateComments(method, false);
        header += "\n" + genExtention.generateMethodImpl(method, genExtention.getStubClassName(fInterface)) +
                  "\n{\n\t//TODO add implementation here\n}";
    }

    header += "\n" + genExtention.generateNamespaceEndDeclaration(model);
    header += "\n" + genExtention.generateVersionNamespaceEnd(fInterface);
    return header;
}

void FInterfaceStubGenerator::generateStub(const std::shared_ptr<FInterface> &fInterface, bool generate_stub,
                                           const std::string &dir)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    // generate header
    auto header_path = gen.getStubHeaderPath(fInterface);
    auto h_path = mk_dir(dir, header_path);
    if (h_path.empty())
        return;
    auto pos = header_path.find_last_of("/") + 1;
    auto file_name = header_path.substr(pos, header_path.size() - pos);
    std::ofstream OsWrite(h_path + "/" + file_name, std::ofstream::out);
    auto header = generateStubHeader(fInterface);
    OsWrite << replace_all(header, "\t", "    ");
    OsWrite.close();
    // generate source
    if (generate_stub)
    {
        auto source_path = gen.getStubSourcePath(fInterface);
        auto s_path = mk_dir(dir, source_path);
        if (s_path.empty())
            return;
        pos = source_path.find_last_of("/") + 1;
        auto s_file_name = source_path.substr(pos, source_path.size() - pos);
        std::ofstream OsWriteS(s_path + "/" + s_file_name, std::ofstream::out);
        auto source = generateStubSource(fInterface);
        OsWriteS << replace_all(source, "\t", "    ");
        OsWriteS.close();
    }
}
} // namespace BstIdl