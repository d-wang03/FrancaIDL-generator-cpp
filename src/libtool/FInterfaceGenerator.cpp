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
#include "libtool/FInterfaceGenerator.h"
#include "utilities/string_utility.h"

namespace BstIdl
{
std::string FInterfaceGenerator::generateHeader(const std::shared_ptr<FInterface> fInterface)
{
    std::string header;
    auto fraExt = FrancaGeneratorExtensions::getInstance();
    header += fraExt.generateBSTLicenseHeader();
    header += "\n" + FTypeGenerator::generateComments(fInterface, false);
    auto name = fraExt.getDefineName(fInterface);
    transform(name.begin(), name.end(), name.begin(), ::toupper);
    header += "\n#ifndef " + name + "_HPP_";
    header += "\n#define " + name + "_HPP_";
    std::list<std::string> libraryHeaders;
    std::list<std::string> generatedHeaders;
    getRequiredHeaderFiles(fInterface, generatedHeaders, libraryHeaders);
    for (auto requiredHeaderFile : generatedHeaders)
    {
        header += "\n#include <" + requiredHeaderFile + ">";
    }
    for (auto libraryHeaderFile : libraryHeaders)
    {
        header += "\n#include <" + libraryHeaderFile + ">";
    }
    if (fInterface->getBase() != nullptr)
    {
        header += "\n#include <" + fraExt.getHeaderPath(fInterface->getBase()) + ">";
    }
    header += "\n" + fraExt.generateVersionNamespaceBegin(fInterface);
    auto model = std::dynamic_pointer_cast<FModel>(fInterface->getContainer());
    header += "\n" + fraExt.generateNamespaceBeginDeclaration(model);
    header += "\nclass " + fInterface->getName();
    if (fInterface->getBase() != nullptr)
        header += " : virtual public " + fraExt.getTypeCollectionName(fInterface->getBase(), fInterface);
    header += "\n{\npublic:";
    header += "\n\t" + fInterface->getName() + "() = default;";
    header += "\n\tvirtual ~" + fInterface->getName() + "() = default;";
    header += "\n\t" + fInterface->getName() + "(const " + fInterface->getName() + " &) = default;";
    header += "\n\t// " + fInterface->getName() + "(" + fInterface->getName() + " &&)noexcept = default;";
    header += "\n\t" + fInterface->getName() + " &operator=(const " + fInterface->getName() + " &) = default;";
    header += "\n\t// " + fInterface->getName() + " &operator=(" + fInterface->getName() + " &&) = default;\n";
    header += "\n\tstatic inline const char* getInterface();";
    header += "\n\tstatic inline std::tuple<int,int> getInterfaceVersion();\n";
    auto typeGen = FTypeGenerator::getInstance();
    header += replace_all(typeGen.generateFTypeDeclarations(fInterface), "\n", "\n\t");
    header += replace_all(typeGen.generateFConstDeclarations(fInterface), "\n", "\n\t") + "\n};\n";
    header += "\nconst char* " + fInterface->getName() + "::getInterface() \n{\n\treturn (\"" +
              fraExt.getFullyQualifiedNameWithVersion(fInterface) + "\");\n}\n";
    header += "\nstd::tuple<int, int> " + fInterface->getName() + "::getInterfaceVersion() \n{\n";
    auto itsVersion = fInterface->getVersion();
    if (itsVersion != nullptr)
    {
        header += "\n\treturn std::make_tuple<int, int>(";
        header += std::to_string(itsVersion->getMajor()) + ", " + std::to_string(itsVersion->getMinor()) + ");\n";
    }
    else
    {
        header += "\n\treturn std::make_tuple<int, int>(0, 0);\n";
    }
    header += "}\n";

    header += "\n" + fraExt.generateNamespaceEndDeclaration(model);
    header += fraExt.generateVersionNamespaceEnd(fInterface);
    header += "\n\n" + fraExt.generateMajorVersionNamespace(fInterface);
    auto defineName = fraExt.getDefineName(fInterface);
    transform(defineName.begin(), defineName.end(), defineName.begin(), ::toupper);
    header += "\n#endif //" + defineName + "_HPP_\n";
    return header;
}

std::string FInterfaceGenerator::generateSource(const std::shared_ptr<FInterface> fInterface)
{
    std::string header;
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    header += genExtention.generateBSTLicenseHeader();
    header += "\n" + FTypeGenerator::generateComments(fInterface, false);
    header += "#include \"" + genExtention.getHeaderFile(fInterface) + "\"\n";
    header += "\n" + genExtention.generateVersionNamespaceBegin(fInterface);
    auto &mgr = FModelManager::getInstance();
    auto model = std::dynamic_pointer_cast<FModel>(fInterface->getContainer());
    header += "\n" + genExtention.generateNamespaceBeginDeclaration(model);
    for (auto type : fInterface->getTypes())
    {
        if (needsSourceComment(type))
            header += FTypeGenerator::generateComments(type, false);
    }
    header += FTypeGenerator::getInstance().generateFConstDefinitions(fInterface) + "\n";
    header += "\n" + genExtention.generateNamespaceEndDeclaration(model);
    header += "\n" + genExtention.generateVersionNamespaceEnd(fInterface);
    return header;
}

bool FInterfaceGenerator::needsSourceComment(std::shared_ptr<FType> &_type)
{
    if (std::dynamic_pointer_cast<FStructType>(_type))
        return std::dynamic_pointer_cast<FStructType>(_type)->isPolymorphic();
    return false;
}

bool FInterfaceGenerator::hasSourceFile(const std::shared_ptr<BstIdl::FInterface> &fInterface)
{
    bool hasTypeWithImplementation = false;
    bool hasConstantWithImplementation = false;
    auto ins = FTypeGenerator::getInstance();
    for (auto type : fInterface->getTypes())
    {
        if (ins.hasImplementation(type))
        {
            hasTypeWithImplementation = true;
            return true;
        }
    }
    for (auto constdef : fInterface->getConstants())
    {
        if (ins.hasImplementation(constdef))
        {
            hasConstantWithImplementation = true;
            return true;
        }
    }
    std::cout << "lib interface has no source.cpp\n";
    return false;
}

void FInterfaceGenerator::generateInterface(const std::shared_ptr<FInterface> &fInterface, const std::string &dir)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    auto header_path = gen.getHeaderPath(fInterface);
    auto h_path = mk_dir(dir, header_path);
    if (h_path.empty())
        return;
    auto pos = header_path.find_last_of("/") + 1;
    auto file_name = header_path.substr(pos, header_path.size() - pos);
    std::ofstream OsWrite(h_path + "/" + file_name, std::ofstream::out);
    auto header = generateHeader(fInterface);
    OsWrite << replace_all(header, "\t", "    ");
    OsWrite.close();
    if (hasSourceFile(fInterface))
    {
        auto source_path = gen.getSourcePath(fInterface);
        auto s_path = mk_dir(dir, source_path);
        if (s_path.empty())
            return;
        auto pos = source_path.find_last_of("/") + 1;
        auto s_file_name = source_path.substr(pos, source_path.size() - pos);
        std::ofstream OsWriteS(s_path + "/" + s_file_name, std::ofstream::out);
        auto source = generateSource(fInterface);
        OsWriteS << replace_all(source, "\t", "    ");
        OsWriteS.close();
    }
}

std::string FInterfaceGenerator::generateInstanceHeader(std::shared_ptr<FInterface> fInterface,
                                                        std::list<std::string> &deployedInstances)
{
    std::string header;
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    header += "\n" + genExtention.generateBSTLicenseHeader();
    header += "\n" + FTypeGenerator::generateComments(fInterface, false);
    auto name = genExtention.getDefineName(fInterface);
    transform(name.begin(), name.end(), name.begin(), ::toupper);
    header += "\n#ifndef" + name + "_INSTANCE_HPP_";
    header += "\n#define" + name + "_INSTANCE_HPP_";
    header += "\n#include <string>";
    header += "\n" + genExtention.generateVersionNamespaceBegin(fInterface);
    auto &mgr = FModelManager::getInstance();
    auto model = std::dynamic_pointer_cast<FModel>(fInterface->getContainer());
    header += "\n" + genExtention.generateNamespaceBeginDeclaration(model);
    for (auto instanceId : deployedInstances)
    {
        std::string element_name = fInterface->getName();
        while (element_name.find(".") != element_name.npos)
        {
            element_name = element_name.replace(element_name.find("."), 1, "_");
        }
        std::string new_instanceId;
        new_instanceId = instanceId;
        while (new_instanceId.find(".") != new_instanceId.npos)
        {
            new_instanceId = new_instanceId.replace(new_instanceId.find("."), 1, "_");
        }
        header += "\nconst char * const" + element_name + "_" + new_instanceId + "=" + "\"" + instanceId + "\"";
    }
    header += "\nconst std::string " + fInterface->getName() + "_INSTANCES[] = {";
    for (auto instanceId : deployedInstances)
    {
        std::string element_name = fInterface->getName();
        while (element_name.find(".") != element_name.npos)
        {
            element_name = element_name.replace(element_name.find("."), 1, "_");
        }
        std::string new_instanceId;
        new_instanceId = instanceId;
        while (new_instanceId.find(".") != new_instanceId.npos)
        {
            new_instanceId = new_instanceId.replace(new_instanceId.find("."), 1, "_");
        }
        header += element_name + "_" + new_instanceId;
        if (instanceId != deployedInstances.back())
            header += ",";
    }
    header += "};\n";
    header += "\n" + genExtention.generateNamespaceEndDeclaration(model);
    header += "\n" + genExtention.generateVersionNamespaceEnd(fInterface);
    header += "\n" + genExtention.generateMajorVersionNamespace(fInterface);
    auto defineName = genExtention.getDefineName(fInterface);
    transform(defineName.begin(), defineName.end(), defineName.begin(), ::toupper);
    header += "\n#endif //" + defineName + "_INSTANCE_HPP_\n";
    return header;
}

void FInterfaceGenerator::getRequiredHeaderFiles(const std::shared_ptr<FInterface> &fInterface,
                                                 std::list<std::string> &generatedHeaders,
                                                 std::list<std::string> &libraryHeaders)
{
    libraryHeaders.push_back("cstdint");
    libraryHeaders.push_back("functional");
    libraryHeaders.push_back("memory");
    libraryHeaders.push_back("tuple");
    if (!fInterface->getManagedInterfaces().empty())
    {
        generatedHeaders.push_back("set");
    }
    for (auto type : fInterface->getTypes())
    {
        FTypeGenerator &type_gen = FTypeGenerator::getInstance();
        type_gen.addRequiredHeaders(type, generatedHeaders, libraryHeaders);
    }
    std::list<std::shared_ptr<FMethod>> errorMethods;
    for (auto method : fInterface->getMethods())
    {
        if (method->getErrors() != nullptr)
        {
            errorMethods.emplace_back(method);
        }
    }
    if (!errorMethods.empty())
    {
        for (auto errorMethod : errorMethods)
        {
            auto error_base = errorMethod->getErrors()->getBase();
            if (error_base != nullptr)
            {
                FTypeGenerator &type_gen = FTypeGenerator::getInstance();
                type_gen.addRequiredHeaders(error_base, generatedHeaders, libraryHeaders);
            }
        }
    }
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    while (std::find(generatedHeaders.begin(), generatedHeaders.end(), gen.getHeaderPath(fInterface)) !=
           generatedHeaders.end())
    {
        auto it = std::find(generatedHeaders.begin(), generatedHeaders.end(), gen.getHeaderPath(fInterface));
        generatedHeaders.erase(it);
    }
}
} // namespace BstIdl