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
#include "someipTools/FInterfaceSomeIPDeploymentGenerator.h"
#include "model/FModelManager.h"
#include "someipTools/FTypeCollectionSomeIPDeploymentGenerator.h"
#include "utilities/string_utility.h"
#include <fstream>
namespace BstCommonAPI
{
void FInterfaceSomeIPDeploymentGenerator::generateDeployment(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                                             const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                             bool generate_code_someip, std::string &dir)
{
    FrancaSomeIPGeneratorExtensions &gen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto names = gen.getFullName(fInterface);
    names = names.substr(0, names.find_last_of("::") + 1);
    if (generate_code_someip)
    {
        auto header_path = gen.someipDeploymentHeaderPath(fInterface);
        auto h_path = mk_dir(dir, header_path);
        if (h_path.empty())
            return;
        auto pos = header_path.find_last_of("/") + 1;
        auto file_name = header_path.substr(pos, header_path.size() - pos);
        std::ofstream OsWrite(h_path + "/" + file_name, std::ofstream::out);
        auto header = generateDeploymentHeader(fInterface, _accessor);
        remove_all(header, names);
        OsWrite << replace_all(header, "\t", "    ");
        OsWrite.close();

        auto source_path = gen.someipDeploymentSourcePath(fInterface);
        auto s_path = mk_dir(dir, source_path);
        if (s_path.empty())
            return;
        pos = source_path.find_last_of("/") + 1;
        auto s_file_name = source_path.substr(pos, source_path.size() - pos);
        std::ofstream OsWriteS(s_path + "/" + s_file_name, std::ofstream::out);
        auto source = generateDeploymentSource(fInterface, _accessor);
        remove_all(source, names);
        OsWriteS << replace_all(source, "\t", "    ");
        OsWriteS.close();
    }
    else
    {
        auto header_path = gen.someipDeploymentHeaderPath(fInterface);
        auto h_path = mk_dir(dir, header_path);
        if (h_path.empty())
            return;
        auto pos = header_path.find_last_of("/") + 1;
        auto file_name = header_path.substr(pos, header_path.size() - pos);
        std::ofstream OsWrite(h_path + "/" + file_name, std::ofstream::out);
        auto header = "";
        OsWrite << header;
        OsWrite.close();
        auto source_path = gen.someipDeploymentSourcePath(fInterface);
        auto s_path = mk_dir(dir, source_path);
        if (s_path.empty())
            return;
        pos = source_path.find_last_of("/") + 1;
        auto s_file_name = source_path.substr(pos, source_path.size() - pos);
        std::ofstream OsWriteS(s_path + "/" + s_file_name, std::ofstream::out);
        OsWriteS << "";
        OsWriteS.close();
    }
}
std::string FInterfaceSomeIPDeploymentGenerator::generateDeploymentHeader(
    const std::shared_ptr<BstIdl::FInterface> &fInterface, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    std::string header;
    header += someIPGen.generateCommonApiSomeIPLicenseHeader();
    header += "\n" + getLicense();
    auto name = genExtention.getDefineName(fInterface);
    transform(name.begin(), name.end(), name.begin(), ::toupper);
    header += "\n\n#ifndef " + name + "_SOMEIP_DEPLOYMENT_HPP_";
    header += "\n#define " + name + "_SOMEIP_DEPLOYMENT_HPP_\n";
    auto DeploymentHeaders = someIPGen.getDeploymentInputIncludes(fInterface, _accessor);
    for (auto deploymentHeader : DeploymentHeaders)
    {
        if (deploymentHeader != someIPGen.someipDeploymentHeaderPath(fInterface))
        {
            header += "\n#include <" + deploymentHeader + ">\n";
        }
    }
    header += R"(

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#define HAS_DEFINED_COMMONAPI_INTERNAL_COMPILATION_HERE
#endif
#include <CommonAPI/SomeIP/Deployment.hpp>
#if defined (HAS_DEFINED_COMMONAPI_INTERNAL_COMPILATION_HERE)
#undef COMMONAPI_INTERNAL_COMPILATION
#undef HAS_DEFINED_COMMONAPI_INTERNAL_COMPILATION_HERE
#endif
)";
    header += genExtention.generateVersionNamespaceBegin(fInterface);
    auto model = std::dynamic_pointer_cast<BstIdl::FModel>(fInterface->getContainer());
    header += genExtention.generateNamespaceBeginDeclaration(model);
    header += "\n" + genExtention.generateDeploymentNamespaceBegin(fInterface);

    header += "\n\n// Interface-specific deployment types\n";
    for (auto t : fInterface->getTypes())
    {
        auto deploymentType = generateDeploymentType(t, 0, _accessor);
        header += "typedef " + deploymentType + " " + genExtention.getElementName(t) + "Deployment_t;\n";
    }
    header += "\n// Type-specific deployments\n";
    for (const auto &t : fInterface->getTypes())
    {
        header += FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentDeclaration(
                      t, std::static_pointer_cast<BstIdl::FTypeCollection>(fInterface), _accessor) +
                  "\n";
    }
    header += "\n// Attribute-specific deployments\n";
    for (auto a : fInterface->getAttributes())
    {
        auto overwriteAccessor = _accessor->getOverwriteAccessor(a);
        auto tmp = generateDeploymentDeclaration(a, fInterface, overwriteAccessor);
        header += tmp.empty() ? "" : (tmp + "\n");
    }
    header += "\n// Argument-specific deployment\n";
    for (auto m : fInterface->getMethods())
    {
        for (auto a : m->getInArgs())
        {
            auto overwriteAccessor = _accessor->getOverwriteAccessor(a);
            auto tmp = generateDeploymentDeclaration(a, m, fInterface, overwriteAccessor);
            header += tmp.empty() ? "" : (tmp + "\n");
        }
        for (auto a : m->getOutArgs())
        {
            auto overwriteAccessor = _accessor->getOverwriteAccessor(a);
            auto tmp = generateDeploymentDeclaration(a, m, fInterface, overwriteAccessor);
            header += tmp.empty() ? "" : (tmp + "\n");
        }
    }

    header += "\n// Broadcast-specific deployments\n";
    for (auto broadcast : fInterface->getBroadcasts())
    {
        for (auto a : broadcast->getOutArgs())
        {
            auto overwriteAccessor = _accessor->getOverwriteAccessor(a);
            auto tmp = generateDeploymentDeclaration(a, broadcast, fInterface, overwriteAccessor);
            header += tmp.empty() ? "" : (tmp + "\n");
        }
    }

    header += "\n" + genExtention.generateDeploymentNamespaceEnd(fInterface);
    header += "\n" + genExtention.generateNamespaceEndDeclaration(model);
    header += genExtention.generateVersionNamespaceEnd(fInterface);
    header += "\n#endif // " + name + "_SOMEIP_DEPLOYMENT_HPP_";
    return header;
}
std::string FInterfaceSomeIPDeploymentGenerator::generateDeploymentSource(
    const std::shared_ptr<BstIdl::FInterface> &fInterface, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    std::string header;
    header += someIPGen.generateCommonApiSomeIPLicenseHeader();
    header += "\n" + getLicense();
    header += "\n#include <" + someIPGen.someipDeploymentHeaderPath(fInterface) + ">";
    header += "\n" + genExtention.generateVersionNamespaceBegin(fInterface);
    auto model = std::dynamic_pointer_cast<BstIdl::FModel>(fInterface->getContainer());
    header += genExtention.generateNamespaceBeginDeclaration(model) + "\n";
    header += genExtention.generateDeploymentNamespaceBegin(fInterface) + "\n";

    header += "\n// Type-specific deployments\n";
    for (auto t : fInterface->getTypes())
    {
        header +=
            FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentDefinition(t, fInterface, _accessor) + "\n";
    }

    header += "\n// Attribute-specific deployments\n";
    for (auto a : fInterface->getAttributes())
    {
        auto overwriteAccessor = _accessor->getOverwriteAccessor(a);
        auto tmp = generateDeploymentDefinition(a, fInterface, overwriteAccessor);
        header += tmp.empty() ? "" : (tmp + "\n");
    }
    header += "\n// Argument-specific deployment\n";
    for (auto m : fInterface->getMethods())
    {
        for (auto a : m->getInArgs())
        {
            auto overwriteAccessor = _accessor->getOverwriteAccessor(a);
            auto tmp = generateDeploymentDefinition(a, m, fInterface, overwriteAccessor);
            header += tmp.empty() ? "" : (tmp + "\n");
        }
        for (auto a : m->getOutArgs())
        {
            auto overwriteAccessor = _accessor->getOverwriteAccessor(a);
            auto tmp = generateDeploymentDefinition(a, m, fInterface, overwriteAccessor);
            header += tmp.empty() ? "" : (tmp + "\n");
        }
    }

    header += "\n// Broadcast-specific deployments\n";
    for (auto broadcast : fInterface->getBroadcasts())
    {
        for (auto a : broadcast->getOutArgs())
        {
            auto overwriteAccessor = _accessor->getOverwriteAccessor(a);
            auto tmp = generateDeploymentDefinition(a, broadcast, fInterface, overwriteAccessor);
            header += tmp.empty() ? "" : (tmp + "\n");
        }
    }
    header += "\n" + genExtention.generateDeploymentNamespaceEnd(fInterface);
    header += "\n" + genExtention.generateNamespaceEndDeclaration(model);
    header += genExtention.generateVersionNamespaceEnd(fInterface);
    return header;
}

std::string FInterfaceSomeIPDeploymentGenerator::generateDeploymentDeclaration(
    const std::shared_ptr<BstIdl::FAttribute> &_attribute, const std::shared_ptr<BstIdl::FInterface> &_interface,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    if (someIPGen.hasSpecificDeployment(_accessor, _attribute) ||
        (_attribute->isArray() && someIPGen.hasDeployment(_accessor, _attribute)))
    {
        return "COMMONAPI_EXPORT extern " + someIPGen.getDeploymentType(_attribute, _interface, true) + " " +
               _attribute->getName() + "Deployment;";
    }
    return "";
}

std::string FInterfaceSomeIPDeploymentGenerator::generateDeploymentDeclaration(
    const std::shared_ptr<BstIdl::FArgument> &_argument, const std::shared_ptr<BstIdl::FMethod> &_method,
    const std::shared_ptr<BstIdl::FInterface> &_interface, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    if (someIPGen.hasSpecificDeployment(_accessor, _argument) ||
        (_argument->isArray() && someIPGen.hasDeployment(_accessor, _argument)))
    {
        return "COMMONAPI_EXPORT extern " + someIPGen.getDeploymentType(_argument, _interface, true) + " " +
               _method->getName() + "_" + _argument->getName() + "Deployment;";
    }
    return std::string();
}

std::string FInterfaceSomeIPDeploymentGenerator::generateDeploymentDeclaration(
    const std::shared_ptr<BstIdl::FArgument> &_argument, const std::shared_ptr<BstIdl::FBroadcast> &_broadcast,
    const std::shared_ptr<BstIdl::FInterface> &_interface, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    if (someIPGen.hasSpecificDeployment(_accessor, _argument) ||
        (_argument->isArray() && someIPGen.hasDeployment(_accessor, _argument)))
    {
        return "COMMONAPI_EXPORT extern " + someIPGen.getDeploymentType(_argument, _interface, true) + " " +
               _broadcast->getName() + "_" + _argument->getName() + "Deployment;";
    }
    return std::string();
}

std::string FInterfaceSomeIPDeploymentGenerator::generateDeploymentDefinition(
    const std::shared_ptr<BstIdl::FAttribute> &_attribute, const std::shared_ptr<BstIdl::FInterface> &_interface,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    if (someIPGen.hasSpecificDeployment(_accessor, _attribute) ||
        (_attribute->isArray() && someIPGen.hasDeployment(_accessor, _attribute)))
    {
        std::string definition = "";
        if (_attribute->isArray() && someIPGen.hasNonArrayDeployment(_accessor, _attribute))
        {
            definition += someIPGen.getDeploymentType(_attribute->getType(), _interface, true) + " " +
                          _attribute->getName() + "ElementDeployment(";
            definition += getDeploymentParameter(_attribute->getType(), _attribute, _interface, _accessor);
            definition += ");\n";
        }
        definition +=
            someIPGen.getDeploymentType(_attribute, _interface, true) + " " + _attribute->getName() + "Deployment(";
        if (_attribute->isArray() && someIPGen.hasNonArrayDeployment(_accessor, _attribute))
        {
            definition += "&" + _attribute->getName() + "ElementDeployment, ";
            definition += getArrayDeploymentParameter(_attribute->getType(), _attribute, _interface, _accessor);
        }
        else
        {
            definition += getDeploymentParameter(_attribute, _attribute, _interface, _accessor);
        }
        definition += ");";
        return definition;
    }
    return "";
}

std::string FInterfaceSomeIPDeploymentGenerator::generateDeploymentDefinition(
    const std::shared_ptr<BstIdl::FArgument> &_argument, const std::shared_ptr<BstIdl::FMethod> &_method,
    const std::shared_ptr<BstIdl::FInterface> &_interface, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    if (someIPGen.hasSpecificDeployment(_accessor, _argument) ||
        (_argument->isArray() && someIPGen.hasDeployment(_accessor, _argument)))
    {
        std::string definition = "";
        if (_argument->isArray() && someIPGen.hasNonArrayDeployment(_accessor, _argument))
        {
            definition += someIPGen.getDeploymentType(_argument->getType(), _interface, true) + " " +
                          _method->getName() + "_" + _argument->getName() + "ElementDeployment(";
            definition += getDeploymentParameter(_argument->getType(), _argument, _interface, _accessor);
            definition += ");\n";
        }
        definition += someIPGen.getDeploymentType(_argument, _interface, true) + " " + _method->getName() + "_" +
                      _argument->getName() + "Deployment(";
        if (_argument->isArray() && someIPGen.hasNonArrayDeployment(_accessor, _argument))
        {
            definition += "&" + _method->getName() + "_" + _argument->getName() + "ElementDeployment, ";
            definition += getArrayDeploymentParameter(_argument->getType(), _argument, _interface, _accessor);
        }
        else
        {
            definition += getDeploymentParameter(_argument, _argument, _interface, _accessor);
        }
        definition += ");";
        return definition;
    }
    return std::string();
}

std::string FInterfaceSomeIPDeploymentGenerator::generateDeploymentDefinition(
    const std::shared_ptr<BstIdl::FArgument> &_argument, const std::shared_ptr<BstIdl::FBroadcast> &_broadcast,
    const std::shared_ptr<BstIdl::FInterface> &_interface, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    if (someIPGen.hasSpecificDeployment(_accessor, _argument) ||
        (_argument->isArray() && someIPGen.hasDeployment(_accessor, _argument)))
    {
        std::string definition = "";
        if (_argument->isArray() && someIPGen.hasNonArrayDeployment(_accessor, _argument))
        {
            definition += someIPGen.getDeploymentType(_argument->getType(), _interface, true) + " " +
                          _broadcast->getName() + "_" + _argument->getName() + "ElementDeployment(";
            definition += getDeploymentParameter(_argument->getType(), _argument, _interface, _accessor);
            definition += ");\n";
        }
        definition += someIPGen.getDeploymentType(_argument, _interface, true) + " " + _broadcast->getName() + "_" +
                      _argument->getName() + "Deployment(";
        if (_argument->isArray() && someIPGen.hasNonArrayDeployment(_accessor, _argument))
        {
            definition += "&" + _broadcast->getName() + "_" + _argument->getName() + "ElementDeployment, ";
            definition += getArrayDeploymentParameter(_argument->getType(), _argument, _interface, _accessor);
        }
        else
        {
            definition += getDeploymentParameter(_argument, _argument, _interface, _accessor);
        }
        definition += ");";
        return definition;
    }
    return std::string();
}
} // namespace BstCommonAPI