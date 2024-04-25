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
#include "libtool/FTypeCollectionGenerator.h"
namespace BstIdl
{
std::string FTypeCollectionGenerator::generateHeader(const std::shared_ptr<FTypeCollection> &fTypeCollection)
{
    std::string header;
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    header += genExtention.generateBSTLicenseHeader();
    header += "\n" + FTypeGenerator::generateComments(fTypeCollection, false);
    auto name = genExtention.getDefineName(fTypeCollection);
    transform(name.begin(), name.end(), name.begin(), ::toupper);
    header += "\n#ifndef " + name + "_HPP_";
    header += "\n#define " + name + "_HPP_\n\n";
    std::list<std::string> libraryHeaders;
    std::list<std::string> generatedHeaders;
    libraryHeaders.emplace_back("tuple");
    getRequiredHeaderFiles(fTypeCollection, generatedHeaders, libraryHeaders);
    for (auto requiredHeaderFile : generatedHeaders)
    {
        header += "#include <" + requiredHeaderFile + ">\n";
    }
    for (auto requiredHeaderFile : libraryHeaders)
    {
        header += "#include <" + requiredHeaderFile + ">\n";
    }
    header += "\n" + genExtention.generateVersionNamespaceBegin(fTypeCollection);
    auto &mgr = FModelManager::getInstance();
    auto model = std::dynamic_pointer_cast<FModel>(fTypeCollection->getContainer());
    header += "\n" + genExtention.generateNamespaceBeginDeclaration(model);
    header += "struct " + fTypeCollection->getName() + "\n{\n";
    auto typeGenerator = FTypeGenerator::getInstance();
    header += typeGenerator.generateFTypeDeclarations(fTypeCollection) + "\n";
    header += typeGenerator.generateFConstDeclarations(fTypeCollection) + "\n";
    header += "\nstatic inline const char* getTypeCollectionName() \n{\n\tstatic const char* typeCollectionName =\"";
    header += genExtention.getFullyQualifiedName(fTypeCollection) + "\";\n\treturn typeCollectionName;\n}\n";
    auto itsVersion = fTypeCollection->getVersion();
    if (itsVersion != nullptr)
    {
        header += "static inline std::tuple<int, int> getTypeCollectionVersion() {\n";
        header += "\treturn std::make_tuple<int, int>(" + std::to_string(itsVersion->getMajor()) + ", " +
                  std::to_string(itsVersion->getMinor()) + ");\n}\n";
    }

    header += "};// struct " + fTypeCollection->getName() + "\n";

    header += "\n" + genExtention.generateNamespaceEndDeclaration(model);
    header += "\n" + genExtention.generateVersionNamespaceEnd(fTypeCollection);
    header += "\n" + genExtention.generateMajorVersionNamespace(fTypeCollection);
    auto defineName = genExtention.getDefineName(fTypeCollection);
    transform(defineName.begin(), defineName.end(), defineName.begin(), ::toupper);
    header += "\n#endif //" + defineName + "_HPP_\n";
    return header;
}

std::string FTypeCollectionGenerator::generateSource(const std::shared_ptr<FTypeCollection> &fTypeCollection)
{
    std::string header;
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    header += genExtention.generateBSTLicenseHeader();
    header += "\n" + FTypeGenerator::generateComments(fTypeCollection, false);
    header += "#include \"" + genExtention.getHeaderFile(fTypeCollection) + "\"\n";
    for (auto fStructTypeHeaderPath : getAllDerivedFStructTypeHeaderPaths(fTypeCollection))
        header += "#include <" + fStructTypeHeaderPath + ">\n";
    header += "\n" + genExtention.generateVersionNamespaceBegin(fTypeCollection);
    auto &mgr = FModelManager::getInstance();
    auto model = std::dynamic_pointer_cast<FModel>(fTypeCollection->getContainer());
    header += "\n" + genExtention.generateNamespaceBeginDeclaration(model);

    header += "\n" + genExtention.generateNamespaceEndDeclaration(model);
    header += "\n" + genExtention.generateVersionNamespaceEnd(fTypeCollection);
    return header;
}

bool FTypeCollectionGenerator::hasSourceFile(const std::shared_ptr<FTypeCollection> &fTypeCollection)
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
    const std::shared_ptr<FTypeCollection> &fTypeCollection)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    std::list<std::string> paths;
    auto types = fTypeCollection->getTypes();
    for (auto type : types)
    {
        if (std::dynamic_pointer_cast<FStructType>(type))
        {
            auto struct_type = std::dynamic_pointer_cast<FStructType>(type);
            if (genExtention.hasPolymorphicBase(struct_type))
            {
                std::list<std::shared_ptr<FStructType>> derived_struct_types;
                genExtention.getDerivedFStructTypes(struct_type, derived_struct_types);
                for (auto it : derived_struct_types)
                {
                    if (std::dynamic_pointer_cast<FInterface>(it->getContainer()) == nullptr)
                    {
                        auto container = std::dynamic_pointer_cast<FInterface>(it->getContainer());
                        if (genExtention.getHeaderPath(container) != genExtention.getHeaderPath(fTypeCollection))
                            paths.emplace_back(genExtention.getHeaderPath(container));
                    }
                }
            }
        }
    }
    return paths;
}

bool FTypeCollectionGenerator::needsSourceComment(std::shared_ptr<FType> &_type)
{
    if (std::dynamic_pointer_cast<FStructType>(_type))
        return std::dynamic_pointer_cast<FStructType>(_type)->isPolymorphic();
    return false;
}

void FTypeCollectionGenerator::generate(const std::shared_ptr<FTypeCollection> &fTypeCollection, const std::string &dir)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    auto header_path = gen.getHeaderPath(fTypeCollection);
    auto h_path = mk_dir(dir, header_path);
    if (h_path.empty())
        return;
    auto pos = header_path.find_last_of("/") + 1;
    auto file_name = header_path.substr(pos, header_path.size() - pos);
    std::ofstream OsWrite(h_path + "/" + file_name, std::ofstream::out);
    auto header = generateHeader(fTypeCollection);
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
        std::ofstream OsWrite(s_path + "/" + s_file_name, std::ofstream::out);
        auto source = generateSource(fTypeCollection);
        OsWrite << replace_all(source, "\t", "    ");
        OsWrite.close();
    }
}
void FTypeCollectionGenerator::getRequiredHeaderFiles(const std::shared_ptr<FTypeCollection> &fTypeCollection,
                                                      std::list<std::string> &generatedHeaders,
                                                      std::list<std::string> &libraryHeaders)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    auto types = fTypeCollection->getTypes();
    for (auto type : types)
    {
        FTypeGenerator &type_gen = FTypeGenerator::getInstance();
        type_gen.addRequiredHeaders(type, generatedHeaders, libraryHeaders);
    }
    for (auto it = generatedHeaders.begin(); it != generatedHeaders.end();)
    {
        if (*it == gen.getHeaderPath(fTypeCollection))
            it = generatedHeaders.erase(it);
        else
            ++it;
    }
}
} // namespace BstIdl