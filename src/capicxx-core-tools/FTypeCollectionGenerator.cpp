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
#include "capicxx-core-tools/FTypeCollectionGenerator.h"
#include "capicxx-core-tools/FTypeGenerator.h"
#include "capicxx-core-tools/FrancaGeneratorExtensions.h"
#include "model/FModelManager.h"
#include "utilities/string_utility.h"
#include <fstream>

namespace BstCommonAPI
{
void FTypeCollectionGenerator::generate(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection,
                                        const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor,
                                        bool generate_code, const std::string &dir)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    if (generate_code)
    {
        auto header_path = gen.getHeaderPath(fTypeCollection);
        auto h_path = mk_dir(dir, header_path);
        if (h_path.empty())
            return;
        auto pos = header_path.find_last_of("/") + 1;
        auto file_name = header_path.substr(pos, header_path.size() - pos);
        std::ofstream OsWrite(h_path + "/" + file_name, std::ofstream::out);
        auto header = generateHeader(fTypeCollection, deploymentAccessor);
        OsWrite << replace_all(header, "\t", "    ");
        OsWrite.close();
        if (hasSourceFile(fTypeCollection))
        {
            auto source_path = gen.getSourcePath(fTypeCollection);
            auto s_path = mk_dir(dir, source_path);
            if (s_path.empty())
                return;
            auto pos = source_path.find_last_of("/") + 1;
            auto s_file_name = source_path.substr(pos, source_path.size() - pos);
            std::ofstream OsWriteS(s_path + "/" + s_file_name, std::ofstream::out);
            auto source = generateSource(fTypeCollection, deploymentAccessor);
            OsWriteS << replace_all(source, "\t", "    ");
            OsWriteS.close();
        }
    }
    else
    {
        auto header_path = gen.getHeaderPath(fTypeCollection);
        auto h_path = mk_dir(dir, header_path);
        if (h_path.empty())
            return;
        auto pos = header_path.find_last_of("/") + 1;
        auto file_name = header_path.substr(pos, header_path.size() - pos);
        std::ofstream OsWrite(h_path + "/" + file_name, std::ofstream::out);
        auto header = "";
        OsWrite << header;
        OsWrite.close();
        if (hasSourceFile(fTypeCollection))
        {
            auto source_path = gen.getSourcePath(fTypeCollection);
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
void FTypeCollectionGenerator::getRequiredHeaderFiles(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection,
                                                      std::list<std::string> &generatedHeaders,
                                                      std::list<std::string> &libraryHeaders)
{

    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    addListItem(libraryHeaders, "CommonAPI/Types.hpp");
    FTypeGenerator &type_gen = FTypeGenerator::getInstance();
    for (auto type : fTypeCollection->getTypes())
    {
        type_gen.addRequiredHeaders(type, generatedHeaders, libraryHeaders);
    }
    generatedHeaders.remove(gen.getHeaderPath(fTypeCollection));
}

std::string FTypeCollectionGenerator::generateHeader(
    const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection,
    const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor)
{
    std::string header;
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    header = genExtention.generateCommonApiLicenseHeader();
    header += "\n" + getLicense();
    header += "\n" + FTypeGenerator::generateComments(fTypeCollection, false);
    auto name = genExtention.getDefineName(fTypeCollection);
    transform(name.begin(), name.end(), name.begin(), ::toupper);
    header += "\n#ifndef " + name + "_HPP_";
    header += "\n#define " + name + "_HPP_\n\n";
    std::list<std::string> libraryHeaders;
    std::list<std::string> generatedHeaders;
    getRequiredHeaderFiles(fTypeCollection, generatedHeaders, libraryHeaders);
    generatedHeaders.sort();
    for (auto requiredHeaderFile : generatedHeaders)
    {
        header += "#include <" + requiredHeaderFile + ">\n";
    }
    header += R"(
#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#define HAS_DEFINED_COMMONAPI_INTERNAL_COMPILATION_HERE
#endif

)";
    libraryHeaders.sort();
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
    header += genExtention.generateVersionNamespaceBegin(fTypeCollection);
    auto &mgr = BstIdl::FModelManager::getInstance();
    auto model = std::dynamic_pointer_cast<BstIdl::FModel>(fTypeCollection->getContainer());
    header += genExtention.generateNamespaceBeginDeclaration(model) + "\n";

    header += "\nstruct " + genExtention.getElementName(fTypeCollection) + " {\n";
    auto typeGenerator = FTypeGenerator::getInstance();
    header +=
        replace_all(typeGenerator.generateFTypeDeclarations(fTypeCollection, deploymentAccessor), "\n", "\n\t") + "\n";
    header +=
        replace_all(typeGenerator.generateFConstDeclarations(fTypeCollection, deploymentAccessor), "\n", "\n\t") + "\n";
    header += "static inline const char* getTypeCollectionName() {\n\tstatic const char* typeCollectionName =\"";
    header += genExtention.getFullyQualifiedName(fTypeCollection) + "\";\n\treturn typeCollectionName;\n}\n";
    auto itsVersion = fTypeCollection->getVersion();
    if (itsVersion != nullptr)
    {
        header += "\ninline CommonAPI::Version getTypeCollectionVersion() {\n";
        header += "\treturn CommonAPI::Version(" + std::to_string(itsVersion->getMajor()) + "," +
                  std::to_string(itsVersion->getMinor()) + ");\n}\n";
    }

    header += "\n};// struct " + genExtention.getElementName(fTypeCollection) + "\n";

    header += "\n" + genExtention.generateNamespaceEndDeclaration(model);
    header += "\n" + genExtention.generateVersionNamespaceEnd(fTypeCollection);

    header += "\nnamespace CommonAPI {\n\t" + genExtention.generateVariantComparators(fTypeCollection) + "\n}";
    header += "\nnamespace std {\n\t" +
              replace_all(genExtention.generateHashers(fTypeCollection, deploymentAccessor), "\n", "\n\t") + "\n}\n";

    header += "\n" + genExtention.generateMajorVersionNamespace(fTypeCollection);
    header += "\n#endif //" + name + "_HPP_\n";
    return header;
}

std::string FTypeCollectionGenerator::generateSource(
    const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection,
    const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor)
{
    std::string header;
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    header = genExtention.generateCommonApiLicenseHeader();
    header += "\n" + getLicense();
    header += "\n" + FTypeGenerator::generateComments(fTypeCollection, false);
    header += "#include \"" + genExtention.getHeaderFile(fTypeCollection) + "\"\n";
    for (auto fStructTypeHeaderPath : getAllDerivedFStructTypeHeaderPaths(fTypeCollection))
        header += "#include <" + fStructTypeHeaderPath + ">\n";
    header += "\n" + genExtention.generateVersionNamespaceBegin(fTypeCollection);
    auto &mgr = BstIdl::FModelManager::getInstance();
    auto model = std::dynamic_pointer_cast<BstIdl::FModel>(fTypeCollection->getContainer());
    header += "\n" + genExtention.generateNamespaceBeginDeclaration(model);

    header += "\n" + genExtention.generateNamespaceEndDeclaration(model);
    header += "\n" + genExtention.generateVersionNamespaceEnd(fTypeCollection);
    return header;
}

bool FTypeCollectionGenerator::hasSourceFile(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection)
{
    bool hasTypeWithImplementation = false;
    auto ins = FTypeGenerator::getInstance();
    for (auto type : fTypeCollection->getTypes())
    {
        if (ins.hasImplementation(type))
        {
            hasTypeWithImplementation = true;
            break;
        }
    }
    return hasTypeWithImplementation;
}

std::list<std::string> FTypeCollectionGenerator::getAllDerivedFStructTypeHeaderPaths(
    const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    std::list<std::string> paths;
    auto types = fTypeCollection->getTypes();
    for (auto type : types)
    {
        if (std::dynamic_pointer_cast<BstIdl::FStructType>(type))
        {
            auto struct_type = std::dynamic_pointer_cast<BstIdl::FStructType>(type);
            if (genExtention.hasPolymorphicBase(struct_type))
            {
                std::list<std::shared_ptr<BstIdl::FStructType>> derived_struct_types;
                genExtention.getDerivedFStructTypes(struct_type, derived_struct_types);
                for (auto it : derived_struct_types)
                {
                    if (std::dynamic_pointer_cast<BstIdl::FInterface>(it->getContainer()) == nullptr)
                    {
                        auto container = std::dynamic_pointer_cast<BstIdl::FInterface>(it->getContainer());
                        if (genExtention.getHeaderPath(container) != genExtention.getHeaderPath(fTypeCollection))
                            paths.emplace_back(genExtention.getHeaderPath(container));
                    }
                }
            }
        }
    }
    return paths;
}
} // namespace BstCommonAPI