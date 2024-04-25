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
#include "capicxx-core-tools/FInterfaceGenerator.h"
#include "capicxx-core-tools/FrancaGeneratorExtensions.h"
#include "model/FModelManager.h"
#include "utilities/string_utility.h"
#include <fstream>
namespace BstCommonAPI
{
void FInterfaceGenerator::generateInterface(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                            const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor,
                                            bool generate_code, const std::string &dir)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    if (generate_code)
    {
        auto header_path = gen.getHeaderPath(fInterface);
        auto h_path = mk_dir(dir, header_path);
        if (h_path.empty())
            return;
        auto pos = header_path.find_last_of("/") + 1;
        auto file_name = header_path.substr(pos, header_path.size() - pos);
        std::ofstream OsWrite(h_path + "/" + file_name, std::ofstream::out);
        auto header = generateHeader(fInterface, deploymentAccessor);
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
            auto source = generateSource(fInterface, deploymentAccessor);
            OsWriteS << replace_all(source, "\t", "    ");
            OsWriteS.close();
        }
    }
    else
    {
        auto header_path = gen.getHeaderPath(fInterface);
        auto h_path = mk_dir(dir, header_path);
        if (h_path.empty())
            return;
        auto pos = header_path.find_last_of("/") + 1;
        auto file_name = header_path.substr(pos, header_path.size() - pos);
        std::ofstream OsWrite(h_path + "/" + file_name, std::ofstream::out);
        auto header = "";
        OsWrite << header;
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
            OsWriteS << "";
            OsWriteS.close();
        }
    }
}

bool FInterfaceGenerator::needsSourceComment(std::shared_ptr<BstIdl::FType> &_type)
{
    if (std::dynamic_pointer_cast<BstIdl::FStructType>(_type))
        return std::dynamic_pointer_cast<BstIdl::FStructType>(_type)->isPolymorphic();
    return false;
}

bool FInterfaceGenerator::hasSourceFile(const std::shared_ptr<BstIdl::FInterface> &fInterface)
{
    bool hasTypeWithImplementation = false;
    auto ins = FTypeGenerator::getInstance();
    for (auto type : fInterface->getTypes())
    {
        if (ins.hasImplementation(type))
        {
            hasTypeWithImplementation = true;
            break;
        }
    }
    return hasTypeWithImplementation;
}

std::string FInterfaceGenerator::generateHeader(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                                const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor)
{
    std::string header;
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    header = genExtention.generateCommonApiLicenseHeader();
    header += "\n" + getLicense();
    header += "\n" + FTypeGenerator::generateComments(fInterface, false);
    auto name = genExtention.getDefineName(fInterface);
    transform(name.begin(), name.end(), name.begin(), ::toupper);
    header += "\n#ifndef " + name + "_HPP_";
    header += "\n#define " + name + "_HPP_";
    std::list<std::string> libraryHeaders;
    std::list<std::string> generatedHeaders;
    getRequiredHeaderFiles(fInterface, generatedHeaders, libraryHeaders);
    generatedHeaders.sort();
    libraryHeaders.sort();
    for (auto requiredHeaderFile : generatedHeaders)
    {
        header += "\n#include <" + requiredHeaderFile + ">";
    }
    if (fInterface->getBase() != nullptr)
    {
        header += "\n#include <" + genExtention.getHeaderPath(fInterface->getBase()) + ">";
    }
    header += R"(
#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#define HAS_DEFINED_COMMONAPI_INTERNAL_COMPILATION_HERE
#endif
)";
    for (auto requiredHeaderFile : libraryHeaders)
    {
        header += "#include <" + requiredHeaderFile + ">\n";
    }

    header += R"(
#if defined (HAS_DEFINED_COMMONAPI_INTERNAL_COMPILATION_HERE)
#undef COMMONAPI_INTERNAL_COMPILATION
#undef HAS_DEFINED_COMMONAPI_INTERNAL_COMPILATION_HERE
#endif
)";
    header += genExtention.generateVersionNamespaceBegin(fInterface);
    auto &mgr = BstIdl::FModelManager::getInstance();
    auto model = std::dynamic_pointer_cast<BstIdl::FModel>(fInterface->getContainer());
    header += genExtention.generateNamespaceBeginDeclaration(model);

    header += "\nclass " + genExtention.getElementName(fInterface);
    if (fInterface->getBase() != nullptr)
        header += "\t: virtual public " + genExtention.getTypeCollectionName(fInterface->getBase(), fInterface);
    header += "{\npublic:";
    header += "\n\tvirtual ~" + genExtention.getElementName(fInterface) + "() { }\n";
    header += "\n\tstatic inline const char* getInterface();";
    header += "\n\tstatic inline CommonAPI::Version getInterfaceVersion();\n";
    auto typeGenerator = FTypeGenerator::getInstance();
    header += replace_all(typeGenerator.generateFTypeDeclarations(fInterface, deploymentAccessor), "\n", "\n\t");
    header += replace_all(typeGenerator.generateFConstDeclarations(fInterface, deploymentAccessor), "\n", "\n\t");
    header += "\n};\n";
    header += "\nconst char* " + genExtention.getElementName(fInterface) + "::getInterface() {\n\treturn (\"" +
              genExtention.getFullyQualifiedNameWithVersion(fInterface) + "\");\n}\n";
    header += "\nCommonAPI::Version " + genExtention.getElementName(fInterface) + "::getInterfaceVersion() {";

    auto itsVersion = fInterface->getVersion();
    if (itsVersion != nullptr)
    {
        header += "\n\treturn CommonAPI::Version(";
        header += std::to_string(itsVersion->getMajor()) + "," + std::to_string(itsVersion->getMinor()) + ");\n";
    }
    else
    {
        header += "\n\treturn CommonAPI::Version(0, 0);\n";
    }
    header += "}\n";

    for (auto type : fInterface->getTypes())
    {
        header += FTypeGenerator::generateComments(type, false);
    }

    header += "\n" + genExtention.generateNamespaceEndDeclaration(model);
    header += genExtention.generateVersionNamespaceEnd(fInterface);

    header += "\nnamespace CommonAPI {";
    header += genExtention.generateVariantComparators(fInterface) + "\n}\n\n";
    header += genExtention.generateMajorVersionNamespace(fInterface);
    auto defineName = genExtention.getDefineName(fInterface);
    transform(defineName.begin(), defineName.end(), defineName.begin(), ::toupper);
    header += "\n#endif //" + defineName + "_HPP_\n";
    return header;
}
std::string FInterfaceGenerator::generateSource(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                                const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor)
{
    std::string header;
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    header = genExtention.generateCommonApiLicenseHeader();
    header += "\n" + getLicense();
    header += "\n" + FTypeGenerator::generateComments(fInterface, false);
    header += "#include \"" + genExtention.getHeaderFile(fInterface) + "\"\n";
    header += "\n" + genExtention.generateVersionNamespaceBegin(fInterface);
    auto &mgr = BstIdl::FModelManager::getInstance();
    auto model = std::dynamic_pointer_cast<BstIdl::FModel>(fInterface->getContainer());
    header += "\n" + genExtention.generateNamespaceBeginDeclaration(model);

    for (auto type : fInterface->getTypes())
    {
        if (needsSourceComment(type))
            header += FTypeGenerator::generateComments(type, false);
    }
    header += "\n" + genExtention.generateNamespaceEndDeclaration(model);
    header += "\n" + genExtention.generateVersionNamespaceEnd(fInterface);
    return header;
}

void FInterfaceGenerator::getRequiredHeaderFiles(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                                 std::list<std::string> &generatedHeaders,
                                                 std::list<std::string> &libraryHeaders)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    std::string str = "CommonAPI/Types.hpp";
    addListItem(libraryHeaders, str);

    for (auto method : fInterface->getMethods())
    {
        if (method->getErrors() != nullptr)
        {
            str = "CommonAPI/InputStream.hpp";
            addListItem(libraryHeaders, str);
            str = "CommonAPI/OutputStream.hpp";
            addListItem(libraryHeaders, str);
            break;
        }
    }

    if (!fInterface->getManagedInterfaces().empty())
    {
        str = "set";
        addListItem(generatedHeaders, str);
    }
    for (auto type : fInterface->getTypes())
    {
        FTypeGenerator &type_gen = FTypeGenerator::getInstance();
        type_gen.addRequiredHeaders(type, generatedHeaders, libraryHeaders);
    }

    std::list<std::shared_ptr<BstIdl::FMethod>> errorMethods;
    for (auto method : fInterface->getMethods())
    {
        if (method->getErrors() != nullptr)
        {
            errorMethods.emplace_back(method);
        }
    }
    if (!errorMethods.empty())
    {
        str = "CommonAPI/InputStream.hpp";
        str = "CommonAPI/OutputStream.hpp";
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
} // namespace BstCommonAPI