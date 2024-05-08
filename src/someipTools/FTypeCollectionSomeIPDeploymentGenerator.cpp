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
#include "someipTools/FTypeCollectionSomeIPDeploymentGenerator.h"
#include "capicxx-core-tools/FrancaGeneratorExtensions.h"
#include "model/FModelManager.h"
#include "someipTools/FrancaSomeIPGeneratorExtensions.h"
#include "utilities/string_utility.h"
#include <fstream>
namespace BstCommonAPI
{
void FTypeCollectionSomeIPDeploymentGenerator::generateTypeCollectionDeployment(
    const std::shared_ptr<BstIdl::FTypeCollection> &tc, const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
    bool generate_code_someip, std::string &dir)
{
    FrancaSomeIPGeneratorExtensions &gen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto names = gen.getFullName(tc);
    names = names.substr(0, names.find_last_of("::") + 1);
    if (generate_code_someip)
    {
        auto header_path = gen.someipDeploymentHeaderPath(tc);
        auto h_path = mk_dir(dir, header_path);
        if (h_path.empty())
            return;
        auto pos = header_path.find_last_of("/") + 1;
        auto file_name = header_path.substr(pos, header_path.size() - pos);
        std::ofstream OsWrite(h_path + "/" + file_name, std::ofstream::out);
        auto header = generateDeploymentHeader(tc, _accessor);
        remove_all(header, names);
        OsWrite << replace_all(header, "\t", "    ");
        OsWrite.close();

        auto source_path = gen.someipDeploymentSourcePath(tc);
        auto s_path = mk_dir(dir, source_path);
        if (s_path.empty())
            return;
        pos = source_path.find_last_of("/") + 1;
        auto s_file_name = source_path.substr(pos, source_path.size() - pos);
        std::ofstream OsWriteS(s_path + "/" + s_file_name, std::ofstream::out);
        auto source = generateDeploymentSource(tc, _accessor);
        remove_all(source, names);
        OsWriteS << replace_all(source, "\t", "    ");
        OsWriteS.close();
    }
    else
    {
        auto header_path = gen.someipDeploymentHeaderPath(tc);
        auto h_path = mk_dir(dir, header_path);
        if (h_path.empty())
            return;
        auto pos = header_path.find_last_of("/") + 1;
        auto file_name = header_path.substr(pos, header_path.size() - pos);
        std::ofstream OsWrite(h_path + "/" + file_name, std::ofstream::out);
        auto header = "";
        OsWrite << header;
        OsWrite.close();
        auto source_path = gen.someipDeploymentSourcePath(tc);
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

std::string FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentHeader(
    const std::shared_ptr<BstIdl::FTypeCollection> &_tc, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    std::string header;
    header += someIPGen.generateCommonApiSomeIPLicenseHeader() + "\n";
    header += "\n" + getLicense();
    auto name = genExtention.getDefineName(_tc);
    transform(name.begin(), name.end(), name.begin(), ::toupper);
    header += "\n#ifndef " + name + "_SOMEIP_DEPLOYMENT_HPP_";
    header += "\n#define " + name + "_SOMEIP_DEPLOYMENT_HPP_\n";
    auto DeploymentHeaders = someIPGen.getDeploymentInputIncludes(_tc, _accessor);
    for (auto deploymentHeader : DeploymentHeaders)
    {
        if (deploymentHeader != someIPGen.someipDeploymentHeaderPath(_tc))
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
    header += genExtention.generateVersionNamespaceBegin(_tc);
    auto &mgr = BstIdl::FModelManager::getInstance();
    auto model = std::dynamic_pointer_cast<BstIdl::FModel>(_tc->getContainer());
    header += genExtention.generateNamespaceBeginDeclaration(model);
    header += "\n" + genExtention.generateDeploymentNamespaceBegin(_tc) + "\n";
    header += "\n// typecollection-specific deployment types";
    for (auto t : _tc->getTypes())
    {
        auto deploymentType = generateDeploymentType(t, 0, _accessor);
        header += "\ntypedef " + deploymentType + " " + genExtention.getElementName(t) + "Deployment_t;\n";
    }

    header += "\n// typecollection-specific deployments\n";
    for (auto t : _tc->getTypes())
    {
        auto tmp = generateDeploymentDeclaration(t, _tc, _accessor);
        header += tmp.empty() ? "" : (tmp + "\n");
    }

    header += "\n" + genExtention.generateDeploymentNamespaceEnd(_tc);
    header += "\n" + genExtention.generateNamespaceEndDeclaration(model);
    header += "\n" + genExtention.generateVersionNamespaceEnd(_tc);
    header += "\n#endif // " + name + "_SOMEIP_DEPLOYMENT_HPP_";
    return header;
}
std::string FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentSource(
    const std::shared_ptr<BstIdl::FTypeCollection> &_tc, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    std::string header;
    header += someIPGen.generateCommonApiSomeIPLicenseHeader();
    header += "\n" + getLicense();
    auto DeploymentHeaders = someIPGen.getDeploymentInputIncludes(_tc, _accessor);
    for (auto it : DeploymentHeaders)
    {
        header += "\n#include <" + it + ">";
    }
    header += "\n" + genExtention.generateVersionNamespaceBegin(_tc);
    auto model = std::dynamic_pointer_cast<BstIdl::FModel>(_tc->getContainer());
    header += genExtention.generateNamespaceBeginDeclaration(model);
    header += "\n" + genExtention.generateDeploymentNamespaceBegin(_tc);
    header += "\n\n// typecollection-specific deployments";

    for (auto t : _tc->getTypes())
    {
        auto tmp = generateDeploymentDefinition(t, _tc, _accessor);
        if (!tmp.empty())
            header += "\n" + tmp;
    }

    header += "\n" + genExtention.generateDeploymentNamespaceEnd(_tc);
    header += "\n" + genExtention.generateNamespaceEndDeclaration(model);
    header += "\n" + genExtention.generateVersionNamespaceEnd(_tc);
    return header;
}

std::string FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentType(
    const std::shared_ptr<BstIdl::FTypeRef> &_typeRef, int _indent,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    if (!_typeRef)
        return "CommonAPI::EmptyDeployment";

    if (_typeRef->getDerived())
        return generateDeploymentType(_typeRef->getDerived(), _indent, _accessor);
    if (_typeRef->getInterval())
        return generateDeploymentType(_typeRef->getInterval(), _indent, _accessor);
    if (_typeRef->getPredefined())
        return generateDeploymentType(_typeRef->getPredefined(), _indent, _accessor);

    return "CommonAPI::EmptyDeployment";
}

std::string FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentType(
    const std::shared_ptr<BstIdl::FArrayType> &_array, int _indent,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    if (!_array)
        return "CommonAPI::EmptyDeployment";

    return generateArrayDeploymentType(_array->getElementType(), _indent, _accessor);
}

std::string FTypeCollectionSomeIPDeploymentGenerator::generateArrayDeploymentType(
    const std::shared_ptr<BstIdl::FTypeRef> &_typeRef, int _indent,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    if (!_typeRef)
        return "CommonAPI::EmptyDeployment";

    auto genExtention = FrancaSomeIPGeneratorExtensions::getInstance();
    std::string deployment = genExtention.generateIndent(_indent) + "CommonAPI::SomeIP::ArrayDeployment<\n";
    deployment +=
        generateDeploymentType(_typeRef, _indent + 1, _accessor) + "\n" + genExtention.generateIndent(_indent) + ">";
    return deployment;
}

std::string FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentType(
    const std::shared_ptr<BstIdl::FEnumerationType> &_enum, int _indent,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    if (!_enum)
        return "CommonAPI::EmptyDeployment";

    auto genExtention = FrancaSomeIPGeneratorExtensions::getInstance();
    if (_indent == 0)
    {
        auto tmp = genExtention.getBackingType(_enum, _accessor)->getName();
        transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
        return genExtention.generateIndent(_indent) + "CommonAPI::SomeIP::EnumerationDeployment<" + tmp + "_t>";
    }
    else
    {
        auto tc = genExtention.getContainingTypeCollection(_enum);
        return genExtention.generateIndent(_indent) + genExtention.getDeploymentType(_enum, tc, true);
    }
}
std::string FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentType(
    const std::shared_ptr<BstIdl::FMapType> &_map, int _indent,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    if (!_map)
        return "CommonAPI::EmptyDeployment";

    auto genExtention = FrancaSomeIPGeneratorExtensions::getInstance();
    std::string deployment = genExtention.generateIndent(_indent) + "CommonAPI::SomeIP::MapDeployment<\n";
    deployment += generateDeploymentType(_map->getKeyType(), _indent + 1, _accessor);
    deployment += ",\n" + generateDeploymentType(_map->getValueType(), _indent + 1, _accessor);
    deployment += "\n" + genExtention.generateIndent(_indent) + ">";
    return deployment;
}
std::string FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentType(
    const std::shared_ptr<BstIdl::FIntegerInterval> &_interval, int _indent,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    if (!_interval)
        return "CommonAPI::EmptyDeployment";

    return FrancaSomeIPGeneratorExtensions::getInstance().generateIndent(_indent) +
           "CommonAPI::SomeIP::IntegerDeployment<int32_t>";
}
std::string FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentType(
    const std::shared_ptr<BstIdl::FStructType> &_struct, int _indent,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    if (!_struct || _struct->getElements().empty())
        return FrancaSomeIPGeneratorExtensions::getInstance().generateIndent(_indent) + "CommonAPI::EmptyDeployment";

    std::string deployment = FrancaSomeIPGeneratorExtensions::getInstance().generateIndent(_indent) +
                             "CommonAPI::SomeIP::StructDeployment<\n";
    std::list<std::string> args;
    for (auto e : _struct->getElements())
    {
        auto overwriteAccessor = _accessor->getOverwriteAccessor(e);
        if (e->isArray())
            args.emplace_back(generateArrayDeploymentType(e->getType(), _indent + 1, overwriteAccessor));
        else
            args.emplace_back(generateDeploymentType(e->getType(), _indent + 1, overwriteAccessor));
    }
    deployment +=
        join(args, ",\n") + "\n" + FrancaSomeIPGeneratorExtensions::getInstance().generateIndent(_indent) + ">";
    return deployment;
}
std::string FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentType(
    const std::shared_ptr<BstIdl::FUnionType> &_union, int _indent,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    if (!_union || _union->getElements().empty())
        return "CommonAPI::EmptyDeployment";

    std::string deployment = FrancaSomeIPGeneratorExtensions::getInstance().generateIndent(_indent) +
                             "CommonAPI::SomeIP::VariantDeployment<\n";
    std::list<std::string> args;
    for (auto e : _union->getElements())
    {
        auto overwriteAccessor = _accessor->getOverwriteAccessor(e);
        if (e->isArray())
            args.emplace_back(generateArrayDeploymentType(e->getType(), _indent + 1, overwriteAccessor));
        else
            args.emplace_back(generateDeploymentType(e->getType(), _indent + 1, overwriteAccessor));
    }
    deployment +=
        join(args, ",\n") + "\n" + FrancaSomeIPGeneratorExtensions::getInstance().generateIndent(_indent) + ">";
    return deployment;
}

std::string FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentType(
    const std::shared_ptr<BstIdl::FTypeDef> &_typeDef, int _indent,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    if (!_typeDef)
        return "CommonAPI::EmptyDeployment";

    return generateDeploymentType(_typeDef->getActualType(), _indent, _accessor);
}

std::string FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentType(
    const std::shared_ptr<BstIdl::FBasicTypeId> &_type, int _indent,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    if (!_type)
        return "CommonAPI::EmptyDeployment";

    auto genExtention = FrancaSomeIPGeneratorExtensions::getInstance();
    std::string deployment = genExtention.generateIndent(_indent);
    if (_type->getValue() == BstIdl::FBasicTypeId::STRING)
        deployment = deployment + "CommonAPI::SomeIP::StringDeployment";
    else if (_type->getValue() == BstIdl::FBasicTypeId::BYTE_BUFFER)
        deployment = deployment + "CommonAPI::SomeIP::ByteBufferDeployment";
    else if (_type->getValue() == BstIdl::FBasicTypeId::INT8 || _type->getValue() == BstIdl::FBasicTypeId::INT16 ||
             _type->getValue() == BstIdl::FBasicTypeId::INT32 || _type->getValue() == BstIdl::FBasicTypeId::INT64 ||
             _type->getValue() == BstIdl::FBasicTypeId::UINT8 || _type->getValue() == BstIdl::FBasicTypeId::UINT16 ||
             _type->getValue() == BstIdl::FBasicTypeId::UINT32 || _type->getValue() == BstIdl::FBasicTypeId::UINT64)
        deployment = deployment + genExtention.getDeployment(_type);
    else
        deployment = deployment + "CommonAPI::EmptyDeployment";

    return deployment;
}

std::string FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentType(
    const std::shared_ptr<BstIdl::FType> &_type, int _indent, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    if (auto ptr = std::dynamic_pointer_cast<BstIdl::FArrayType>(_type))
        return generateDeploymentType(ptr, _indent, _accessor);
    else if (auto ptr = std::dynamic_pointer_cast<BstIdl::FEnumerationType>(_type))
        return generateDeploymentType(ptr, _indent, _accessor);
    else if (auto ptr = std::dynamic_pointer_cast<BstIdl::FMapType>(_type))
        return generateDeploymentType(ptr, _indent, _accessor);
    else if (auto ptr = std::dynamic_pointer_cast<BstIdl::FTypeDef>(_type))
        return generateDeploymentType(ptr, _indent, _accessor);
    else if (auto ptr = std::dynamic_pointer_cast<BstIdl::FIntegerInterval>(_type))
        return generateDeploymentType(ptr, _indent, _accessor);
    else if (auto ptr = std::dynamic_pointer_cast<BstIdl::FBasicTypeId>(_type))
        return generateDeploymentType(ptr, _indent, _accessor);
    else if (auto ptr = std::dynamic_pointer_cast<BstIdl::FStructType>(_type))
        return generateDeploymentType(ptr, _indent, _accessor);
    else if (auto ptr = std::dynamic_pointer_cast<BstIdl::FUnionType>(_type))
        return generateDeploymentType(ptr, _indent, _accessor);
    else
        return genExtention.generateIndent(_indent) + "CommonAPI::EmptyDeployment";
}

std::string FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentDeclaration(
    const std::shared_ptr<BstIdl::FType> &_type, const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto _array = std::dynamic_pointer_cast<BstIdl::FArrayType>(_type);
    auto _enum = std::dynamic_pointer_cast<BstIdl::FEnumerationType>(_type);
    auto _map = std::dynamic_pointer_cast<BstIdl::FMapType>(_type);
    auto _struct = std::dynamic_pointer_cast<BstIdl::FStructType>(_type);
    auto _union = std::dynamic_pointer_cast<BstIdl::FUnionType>(_type);
    auto _typeDef = std::dynamic_pointer_cast<BstIdl::FTypeDef>(_type);
    if (_array != nullptr)
    {
        return generateDeploymentDeclaration(_array, _tc, _accessor);
    }
    else if (_enum != nullptr)
    {
        return generateDeploymentDeclaration(_enum, _tc, _accessor);
    }
    else if (_map != nullptr)
    {
        return generateDeploymentDeclaration(_map, _tc, _accessor);
    }
    else if (_struct != nullptr)
    {
        return generateDeploymentDeclaration(_struct, _tc, _accessor);
    }
    else if (_union != nullptr)
    {
        return generateDeploymentDeclaration(_union, _tc, _accessor);
    }
    else if (_typeDef != nullptr)
    {
        return generateDeploymentDeclaration(_typeDef, _tc, _accessor);
    }
    return std::string();
}

std::string FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentDeclaration(
    const std::shared_ptr<BstIdl::FArrayType> &_array, const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    if (someIPGen.hasDeployment(_accessor, _array))
    {
        return generateDeploymentDeclaration(_array->getElementType(), _tc, _accessor) + "COMMONAPI_EXPORT extern " +
               someIPGen.getDeploymentType(_array, _tc, true) + " " + _array->getName() + "Deployment;";
    }
    return "";
}

std::string FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentDeclaration(
    const std::shared_ptr<BstIdl::FEnumerationType> &_enum, const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    if (someIPGen.hasDeployment(_accessor, _enum))
    {
        return "COMMONAPI_EXPORT extern " + genExtention.getElementName(_enum) + "Deployment_t " + _enum->getName() +
               "Deployment;";
    }
    return "";
}

std::string FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentDeclaration(
    const std::shared_ptr<BstIdl::FMapType> &_map, const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    if (someIPGen.hasDeployment(_accessor, _map))
    {
        return generateDeploymentDeclaration(_map->getKeyType(), _tc, _accessor) +
               generateDeploymentDeclaration(_map->getValueType(), _tc, _accessor) + "COMMONAPI_EXPORT extern " +
               someIPGen.getDeploymentType(_map, _tc, true) + " " + _map->getName() + "Deployment;";
    }
    return std::string();
}

std::string FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentDeclaration(
    const std::shared_ptr<BstIdl::FStructType> &_struct, const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    if (genExtention.isStructEmpty(_struct))
    {
        return "static_assert(false, \"struct " + _struct->getName() + " must not be empty !\");";
    }
    if (someIPGen.hasDeployment(_accessor, _struct))
    {
        std::string declaration = "";
        for (auto e : _struct->getElements())
        {
            declaration += generateDeploymentDeclaration(e, _tc, _accessor->getOverwriteAccessor(e));
        }
        declaration += "COMMONAPI_EXPORT extern " + someIPGen.getDeploymentType(_struct, _tc, true) + " " +
                       _struct->getName() + "Deployment;";
        return declaration + "\n";
    }
    return "";
}

std::string FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentDeclaration(
    const std::shared_ptr<BstIdl::FUnionType> &_union, const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    if (someIPGen.hasDeployment(_accessor, _union))
    {
        std::string declaration = "";
        for (auto e : _union->getElements())
        {
            declaration += generateDeploymentDeclaration(e, _tc, _accessor->getOverwriteAccessor(e));
        }
        declaration += "COMMONAPI_EXPORT extern " + someIPGen.getDeploymentType(_union, _tc, true) + " " +
                       _union->getName() + "Deployment;";
        return declaration + "\n";
    }
    return "";
}

std::string FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentDeclaration(
    const std::shared_ptr<BstIdl::FField> &_field, const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    if (someIPGen.hasSpecificDeployment(_accessor, _field) ||
        (_field->isArray() && someIPGen.hasDeployment(_accessor, _field)))
    {
        return "COMMONAPI_EXPORT extern " + someIPGen.getDeploymentType(_field, _tc, true) + " " +
               _accessor->getName() + "Deployment;\n";
    }
    return "";
}

std::string FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentDeclaration(
    const std::shared_ptr<BstIdl::FTypeDef> &_typeDef, const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    if (_typeDef && _typeDef->getActualType())
        return generateDeploymentDeclaration(_typeDef->getActualType(), _tc, _accessor);

    return "";
}

std::string FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentDeclaration(
    const std::shared_ptr<BstIdl::FTypeRef> &_typeRef, const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    if (!_typeRef)
        return "";

    if (auto ptr = _typeRef->getDerived())
        return generateDeploymentDeclaration(ptr, _tc, _accessor);

    return "";
}

std::string FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentDefinition(
    const std::shared_ptr<BstIdl::FType> &_type, const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto _array = std::dynamic_pointer_cast<BstIdl::FArrayType>(_type);
    auto _enum = std::dynamic_pointer_cast<BstIdl::FEnumerationType>(_type);
    auto _map = std::dynamic_pointer_cast<BstIdl::FMapType>(_type);
    auto _struct = std::dynamic_pointer_cast<BstIdl::FStructType>(_type);
    auto _union = std::dynamic_pointer_cast<BstIdl::FUnionType>(_type);
    auto _typeDef = std::dynamic_pointer_cast<BstIdl::FTypeDef>(_type);
    if (_array != nullptr)
    {
        return generateDeploymentDefinition(_array, _tc, _accessor);
    }
    else if (_enum != nullptr)
    {
        return generateDeploymentDefinition(_enum, _tc, _accessor);
    }
    else if (_map != nullptr)
    {
        return generateDeploymentDefinition(_map, _tc, _accessor);
    }
    else if (_struct != nullptr)
    {
        return generateDeploymentDefinition(_struct, _tc, _accessor);
    }
    else if (_union != nullptr)
    {
        return generateDeploymentDefinition(_union, _tc, _accessor);
    }
    else if (_typeDef != nullptr)
    {
        return generateDeploymentDefinition(_typeDef, _tc, _accessor);
    }
    return std::string();
}

std::string FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentDefinition(
    const std::shared_ptr<BstIdl::FArrayType> &_array, const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    if (someIPGen.hasDeployment(_accessor, _array))
    {
        auto definition =
            generateDeploymentDefinition(_array->getElementType(), _tc, _accessor->getOverwriteAccessor(_array));
        if (!_accessor->getParent())
        {
            definition += someIPGen.getDeploymentType(_array, _tc, true) + " " + _accessor->getName() +
                          _array->getName() + "Deployment(";
            definition += getDeploymentParameter(_array, _array, _tc, _accessor);
            definition += ");";
        }
        return definition;
    }
    return "";
}

std::string FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentDefinition(
    const std::shared_ptr<BstIdl::FEnumerationType> &_enum, const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    if (someIPGen.hasDeployment(_accessor, _enum) && !_accessor->getParent())
    {
        std::string definition = genExtention.getElementName(_enum) + "Deployment_t " + _accessor->getName() +
                                 _enum->getName() + "Deployment(";
        definition += getDeploymentParameter(_enum, _enum, _tc, _accessor);
        definition += ");";
        return definition;
    }
    return "";
}

std::string FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentDefinition(
    const std::shared_ptr<BstIdl::FMapType> &_map, const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    if (someIPGen.hasDeployment(_accessor, _map))
    {
        auto definition = generateDeploymentDefinition(_map->getKeyType(), _tc, _accessor) +
                          generateDeploymentDefinition(_map->getValueType(), _tc, _accessor);
        // Generate if top level element or has overriden key/value
        if (!_accessor->getParent() || !definition.empty())
        {
            definition += someIPGen.getDeploymentType(_map, _tc, true) + " " + _accessor->getName() + _map->getName() +
                          "Deployment(";
            definition += getDeploymentParameter(_map, _map, _tc, _accessor);
            definition += ");";
        }
        return definition;
    }
    return "";
}

std::string FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentDefinition(
    const std::shared_ptr<BstIdl::FStructType> &_struct, const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    std::string definition = "";
    if (someIPGen.hasDeployment(_accessor, _struct))
    {
        for (auto e : _struct->getElements())
        {
            definition += generateDeploymentDefinition(e, _tc, _accessor->getOverwriteAccessor(e));
        }
        // Generate if struct is top-level or has overridden field
        if (!_accessor->getParent() || !definition.empty())
        {
            definition += someIPGen.getDeploymentType(_struct, _tc, true) + " " + _accessor->getName() +
                          _struct->getName() + "Deployment(";
            definition += getDeploymentParameter(_struct, _struct, _tc, _accessor);
            definition += ");\n";
        }
        return definition;
    }
    return "";
}

std::string FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentDefinition(
    const std::shared_ptr<BstIdl::FUnionType> &_union, const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    std::string definition = "";
    if (someIPGen.hasDeployment(_accessor, _union))
    {
        for (auto e : _union->getElements())
        {
            definition += generateDeploymentDefinition(e, _tc, _accessor->getOverwriteAccessor(e));
        }
        // Generate if union is top-level or has overridden field
        if (!_accessor->getParent() || !definition.empty())
        {
            definition += someIPGen.getDeploymentType(_union, _tc, true) + " " + _accessor->getName() +
                          _union->getName() + "Deployment(";
            definition += getDeploymentParameter(_union, _union, _tc, _accessor);
            definition += ");\n";
        }
        return definition;
    }
    return "";
}

std::string FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentDefinition(
    const std::shared_ptr<BstIdl::FField> &_field, const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    if (someIPGen.hasSpecificDeployment(_accessor, _field) ||
        (_field->isArray() && someIPGen.hasDeployment(_accessor, _field)))
    {
        std::string definition = generateDeploymentDefinition(_field->getType(), _tc, _accessor);
        std::string accessorName = _accessor->getName();
        if (_field->isArray() && someIPGen.hasNonArrayDeployment(_accessor, _field))
        {
            definition +=
                someIPGen.getDeploymentType(_field->getType(), _tc, false) + " " + accessorName + "ElementDeployment(";
            definition += getDeploymentParameter(_field->getType(), _field, _tc, _accessor);
            definition += ");\n";
        }
        definition += someIPGen.getDeploymentType(_field, _tc, true) + " " + accessorName + "Deployment(";
        if (_field->isArray() && someIPGen.hasNonArrayDeployment(_accessor, _field))
        {
            definition += "&" + accessorName + "ElementDeployment, ";
            definition += getArrayDeploymentParameter(_field->getType(), _field, _tc, _accessor);
        }
        else
        {
            definition += getDeploymentParameter(_field, _field, _tc, _accessor);
        }
        definition += ");\n";
        return definition;
    }
    return "";
}

std::string FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentDefinition(
    const std::shared_ptr<BstIdl::FTypeDef> &_typeDef, const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    return generateDeploymentDefinition(_typeDef->getActualType(), _tc, _accessor);
}

std::string FTypeCollectionSomeIPDeploymentGenerator::generateDeploymentDefinition(
    const std::shared_ptr<BstIdl::FTypeRef> &_typeRef, const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    if (!_typeRef)
        return "";

    if (_accessor->isProperOverwrite())
        if (auto ptr = _typeRef->getDerived())
            return generateDeploymentDefinition(ptr, _tc, _accessor);

    return "";
}

std::string FTypeCollectionSomeIPDeploymentGenerator::getDeploymentParameter(
    const std::shared_ptr<BstIdl::FType> &_type, const std::shared_ptr<BstIdl::FObject> &_source,
    const std::shared_ptr<BstIdl::FTypeCollection> &_tc, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto _array = std::dynamic_pointer_cast<BstIdl::FArrayType>(_type);
    auto _enum = std::dynamic_pointer_cast<BstIdl::FEnumerationType>(_type);
    auto _map = std::dynamic_pointer_cast<BstIdl::FMapType>(_type);
    auto _struct = std::dynamic_pointer_cast<BstIdl::FStructType>(_type);
    auto _union = std::dynamic_pointer_cast<BstIdl::FUnionType>(_type);
    auto _typeDef = std::dynamic_pointer_cast<BstIdl::FTypeDef>(_type);
    if (_array != nullptr)
    {
        return getDeploymentParameter(_array, _source, _tc, _accessor);
    }
    else if (_enum != nullptr)
    {
        return getDeploymentParameter(_enum, _source, _tc, _accessor);
    }
    else if (_map != nullptr)
    {
        return getDeploymentParameter(_map, _source, _tc, _accessor);
    }
    else if (_struct != nullptr)
    {
        return getDeploymentParameter(_struct, _source, _tc, _accessor);
    }
    else if (_union != nullptr)
    {
        return getDeploymentParameter(_union, _source, _tc, _accessor);
    }
    else if (_typeDef != nullptr)
    {
        return getDeploymentParameter(_typeDef, _source, _tc, _accessor);
    }
    return std::string();
}

std::string FTypeCollectionSomeIPDeploymentGenerator::getDeploymentParameter(
    const std::shared_ptr<BstIdl::FArrayType> &_array, const std::shared_ptr<BstIdl::FObject> &_source,
    const std::shared_ptr<BstIdl::FTypeCollection> &_tc, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto parameter = getArrayElementTypeDeploymentParameter(_array->getElementType(), _source, _tc,
                                                            _accessor->getOverwriteAccessor(_array)) +
                     ", ";
    parameter += getArrayDeploymentParameter(_array, _source, _tc, _accessor);
    return parameter;
}

std::string FTypeCollectionSomeIPDeploymentGenerator::getDeploymentParameter(
    const std::shared_ptr<BstIdl::FEnumerationType> &_enum, const std::shared_ptr<BstIdl::FObject> &_source,
    const std::shared_ptr<BstIdl::FTypeCollection> &_tc, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::string parameter = "";
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    int baseType = someIPGen.getSomeIpEnumWidthHelper(_accessor, _source);
    if (baseType == -1)
        baseType = _accessor->getSomeIpEnumWidth(_enum);

    int bitWidth = someIPGen.getSomeIpEnumBitWidthHelper(_accessor, _source);
    if (bitWidth == -1)
        bitWidth = someIPGen.getSomeIpEnumBitWidthHelper(_accessor, _enum);

    if (bitWidth != -1)
    {
        parameter += std::to_string(bitWidth);
    }
    else
    {
        if (baseType != -1)
        {
            int bits = baseType * 8;
            parameter += std::to_string(bits);
        }
        else
        {
            parameter += "32"; // Default value: 4 Byte
        }
    }

    auto typeId = genExtention.getBackingType(_enum, _accessor);
    parameter += ", " + genExtention.isSigned(typeId->getLiteral());

    int invalidValue = someIPGen.getSomeIpEnumInvalidValueHelper(_accessor, _source);
    if (invalidValue == -1)
        invalidValue = _accessor->getSomeIpEnumInvalidValue(_enum);
    if (invalidValue != -1)
        parameter += ", " + std::to_string(invalidValue);

    return parameter;
}

std::string FTypeCollectionSomeIPDeploymentGenerator::getDeploymentParameter(
    const std::shared_ptr<BstIdl::FIntegerInterval> &_interval, const std::shared_ptr<BstIdl::FObject> &_source,
    const std::shared_ptr<BstIdl::FTypeCollection> &_tc, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::string parameter = "";
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    int bitWidth = someIPGen.getSomeIpIntegerBitWidthHelper(_accessor, _source);
    if (bitWidth != -1)
        parameter += std::to_string(bitWidth) + ", ";
    else
        parameter += "32";

    int invalidValue = someIPGen.getSomeIpIntegerInvalidValueHelper(_accessor, _source);
    if (invalidValue != -1)
        parameter += ", " + std::to_string(invalidValue);

    return parameter;
}

std::string FTypeCollectionSomeIPDeploymentGenerator::getDeploymentParameter(
    const std::shared_ptr<BstIdl::FMapType> &_map, const std::shared_ptr<BstIdl::FObject> &_source,
    const std::shared_ptr<BstIdl::FTypeCollection> &_tc, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    std::string parameter = someIPGen.getDeploymentRef(_map->getKeyType(), _accessor) + ", " +
                            someIPGen.getDeploymentRef(_map->getValueType(), _accessor) + ", ";
    parameter += getMapDeploymentParameter(_map, _source, _tc, _accessor);
    return parameter;
}

std::string FTypeCollectionSomeIPDeploymentGenerator::getDeploymentParameter(
    const std::shared_ptr<BstIdl::FStructType> &_struct, const std::shared_ptr<BstIdl::FObject> &_source,
    const std::shared_ptr<BstIdl::FTypeCollection> &_tc, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::string parameter = "";
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    int lengthWidth = someIPGen.getSomeIpStructLengthWidthHelper(_accessor, _struct);
    if (lengthWidth != -1)
        parameter += std::to_string(lengthWidth) + ", ";
    else
        parameter += "0, ";

    parameter += getDerivedDeploymentParameter(_struct, _tc, _accessor);
    // cut off the last comma
    return parameter.substr(0, parameter.length() - 2);
}

std::string FTypeCollectionSomeIPDeploymentGenerator::getDerivedDeploymentParameter(
    const std::shared_ptr<BstIdl::FStructType> &_struct, const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::string parameter = "";
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    if (_struct->getBase() != nullptr)
    { // need to use the accessor for the base struct !
        auto baseAccessor = someIPGen.getSomeIpAccessor(
            std::dynamic_pointer_cast<BstIdl::FTypeCollection>(_struct->getBase()->getContainer()));
        parameter += getDerivedDeploymentParameter(_struct->getBase(), _tc, baseAccessor);
    }
    for (auto s : _struct->getElements())
    {
        parameter += someIPGen.getDeploymentRef(s, _struct, _accessor->getOverwriteAccessor(s)) + ", ";
    }

    return parameter;
}

std::string FTypeCollectionSomeIPDeploymentGenerator::getDeploymentParameter(
    const std::shared_ptr<BstIdl::FUnionType> &_union, const std::shared_ptr<BstIdl::FObject> &_source,
    const std::shared_ptr<BstIdl::FTypeCollection> &_tc, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::string parameter = "";
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    int lengthWidth = someIPGen.getSomeIpUnionLengthWidthHelper(_accessor, _union);
    if (lengthWidth != -1)
        parameter += std::to_string(lengthWidth) + ", ";
    else
        parameter += "4, ";

    int typeWidth = someIPGen.getSomeIpUnionTypeWidthHelper(_accessor, _union);
    if (typeWidth != -1)
        parameter += std::to_string(typeWidth) + ", ";
    else
        parameter += "4, ";

    int defaultOrder = someIPGen.getSomeIpUnionDefaultOrderHelper(_accessor, _union);
    if (defaultOrder != -1)
        parameter += std::to_string(defaultOrder) + ", ";
    else
        parameter += "true, ";

    int maxLength = someIPGen.getSomeIpUnionMaxLengthHelper(_accessor, _union);
    if (maxLength != -1)
        parameter += std::to_string(maxLength) + ", ";
    else
        parameter += "0, ";

    for (auto s : _union->getElements())
    {
        parameter += someIPGen.getDeploymentRef(s, _union, _accessor->getOverwriteAccessor(s));
        if (s != _union->getElements().back())
            parameter += ", ";
    }

    return parameter;
}

std::string FTypeCollectionSomeIPDeploymentGenerator::getDeploymentParameter(
    const std::shared_ptr<BstIdl::FBasicTypeId> &_typeId, const std::shared_ptr<BstIdl::FObject> &_source,
    const std::shared_ptr<BstIdl::FTypeCollection> &_tc, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    std::string parameter = "";
    if (_typeId->getValue() == BstIdl::FBasicTypeId::STRING)
    {
        int length = _accessor->getSomeIpStringLength(_source);
        if (length != -1)
            parameter += std::to_string(length) + ", ";
        else
            parameter += "0, ";

        int lengthWidth = _accessor->getSomeIpStringLengthWidth(_source);
        if (lengthWidth != -1)
            parameter += std::to_string(lengthWidth) + ", ";
        else
            parameter += "4, ";

        auto encoding = _accessor->getSomeIpStringEncoding(_source);
        switch (encoding)
        {
        case SomeIpStringEncoding::utf16be:
            parameter += "CommonAPI::SomeIP::StringEncoding::UTF16BE";
            break;
        case SomeIpStringEncoding::utf16le:
            parameter += "CommonAPI::SomeIP::StringEncoding::UTF16LE";
            break;
        default:
            parameter += "CommonAPI::SomeIP::StringEncoding::UTF8";
            break;
        }
    }
    else if (_typeId->getValue() == BstIdl::FBasicTypeId::BYTE_BUFFER)
    {
        parameter += getByteBufferDeploymentParamter(_source, _source, _accessor);
    }
    else if (_typeId->getValue() == BstIdl::FBasicTypeId::INT8 || _typeId->getValue() == BstIdl::FBasicTypeId::INT16 ||
             _typeId->getValue() == BstIdl::FBasicTypeId::INT32 || _typeId->getValue() == BstIdl::FBasicTypeId::INT64 ||
             _typeId->getValue() == BstIdl::FBasicTypeId::UINT8 ||
             _typeId->getValue() == BstIdl::FBasicTypeId::UINT16 ||
             _typeId->getValue() == BstIdl::FBasicTypeId::UINT32 || _typeId->getValue() == BstIdl::FBasicTypeId::UINT64)
    {
        int bitWidth = someIPGen.getSomeIpIntegerBitWidthHelper(_accessor, _source);
        if (bitWidth != -1)
            parameter += std::to_string(bitWidth);
        else
            parameter += genExtention.getBitWidth(_typeId);
        int invalidValue = someIPGen.getSomeIpIntegerInvalidValueHelper(_accessor, _source);
        if (invalidValue != -1)
        {
            parameter += ", " + std::to_string(invalidValue);
        }
    }
    return parameter;
}

std::string FTypeCollectionSomeIPDeploymentGenerator::getDeploymentParameter(
    const std::shared_ptr<BstIdl::FTypeRef> &_typeRef, const std::shared_ptr<BstIdl::FObject> &_source,
    const std::shared_ptr<BstIdl::FTypeCollection> &_tc, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    if (_typeRef->getDerived() != nullptr)
    {
        return getDeploymentParameter(_typeRef->getDerived(), _source, _tc, _accessor);
    }

    if (_typeRef->getPredefined() != nullptr)
    {
        return getDeploymentParameter(_typeRef->getPredefined(), _source, _tc, _accessor);
    }

    if (_typeRef->getInterval() != nullptr)
        return getDeploymentParameter(_typeRef->getInterval(), _source, _tc, _accessor);

    return "";
}

std::string FTypeCollectionSomeIPDeploymentGenerator::getDeploymentParameter(
    const std::shared_ptr<BstIdl::FTypeDef> _typeDef, const std::shared_ptr<BstIdl::FObject> &_source,
    const std::shared_ptr<BstIdl::FTypeCollection> &_tc, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{

    return getDeploymentParameter(_typeDef->getActualType(), _source, _tc, _accessor);
}

std::string FTypeCollectionSomeIPDeploymentGenerator::getDeploymentParameter(
    const std::shared_ptr<BstIdl::FTypedElement> &_attribute, const std::shared_ptr<BstIdl::FObject> &_object,
    const std::shared_ptr<BstIdl::FTypeCollection> &_tc, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    if (_attribute->isArray())
    {
        std::string parameter =
            getArrayElementTypeDeploymentParameter(_attribute->getType(), _object, _tc, _accessor) + ", ";
        parameter += getArrayDeploymentParameter(_attribute, _attribute, _tc, _accessor);
        return parameter;
    }
    return getDeploymentParameter(_attribute->getType(), _attribute, _tc, _accessor);
}

std::string FTypeCollectionSomeIPDeploymentGenerator::getDeploymentParameter(
    const std::shared_ptr<BstIdl::FArgument> &_argument, const std::shared_ptr<BstIdl::FObject> &_object,
    const std::shared_ptr<BstIdl::FTypeCollection> &_tc, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    if (_argument->isArray())
    {
        std::string parameter =
            getArrayElementTypeDeploymentParameter(_argument->getType(), _object, _tc, _accessor) + ", ";
        parameter += getArrayDeploymentParameter(_argument, _argument, _tc, _accessor);
        return parameter;
    }
    return getDeploymentParameter(_argument->getType(), _argument, _tc, _accessor);
}

// Arrays may be either defined types or inline
std::string FTypeCollectionSomeIPDeploymentGenerator::getArrayElementTypeDeploymentParameter(
    const std::shared_ptr<BstIdl::FTypeRef> &_elementType, const std::shared_ptr<BstIdl::FObject> &_source,
    const std::shared_ptr<BstIdl::FTypeCollection> &_tc, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    return someIPGen.getDeploymentRef(_elementType, _accessor);
}

std::string FTypeCollectionSomeIPDeploymentGenerator::getByteBufferDeploymentParamter(
    const std::shared_ptr<BstIdl::FObject> &_buffer, const std::shared_ptr<BstIdl::FObject> &_source,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::string parameter = "";
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    int minLength = _accessor->getSomeIpByteBufferMinLength(_source);
    if (minLength == -1 && _buffer != _source)
        minLength = _accessor->getSomeIpByteBufferMinLength(_buffer);
    if (minLength != -1)
        parameter += std::to_string(minLength) + ", ";
    else
        parameter += std::to_string(someIPGen.SOMEIP_DEFAULT_MIN_LENGTH) + ", ";

    int maxLength = _accessor->getSomeIpByteBufferMaxLength(_source);
    if (maxLength == -1 && _buffer != _source)
        maxLength = _accessor->getSomeIpByteBufferMaxLength(_buffer);
    if (maxLength != -1)
        parameter += std::to_string(maxLength) + ", ";
    else
        parameter += std::to_string(someIPGen.SOMEIP_DEFAULT_MAX_LENGTH) + ", ";

    int lengthWidth = _accessor->getSomeIpByteBufferLengthWidth(_source);
    if (lengthWidth == -1 && _buffer != _source)
        lengthWidth = _accessor->getSomeIpByteBufferLengthWidth(_buffer);
    if (lengthWidth != -1)
        parameter += std::to_string(lengthWidth);
    else
        parameter += std::to_string(someIPGen.SOMEIP_DEFAULT_LENGTH_WIDTH);

    return parameter;
}

std::string FTypeCollectionSomeIPDeploymentGenerator::getArrayDeploymentParameter(
    const std::shared_ptr<BstIdl::FObject> &_array, const std::shared_ptr<BstIdl::FObject> &_source,
    const std::shared_ptr<BstIdl::FTypeCollection> &_tc, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::string parameter = "";
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    int minLength = someIPGen.getSomeIpArrayMinLengthHelper(_accessor, _source);
    if (minLength == -1 && _array != _source)
        minLength = someIPGen.getSomeIpArrayMinLengthHelper(_accessor, _array);
    if (minLength != -1)
        parameter += std::to_string(minLength) + ", ";
    else
        parameter += std::to_string(someIPGen.SOMEIP_DEFAULT_MIN_LENGTH) + ", ";

    int maxLength = someIPGen.getSomeIpArrayMaxLengthHelper(_accessor, _source);
    if (maxLength == -1 && _array != _source)
        maxLength = someIPGen.getSomeIpArrayMaxLengthHelper(_accessor, _array);
    if (maxLength != -1)
        parameter += std::to_string(maxLength) + ", ";
    else
        parameter += std::to_string(someIPGen.SOMEIP_DEFAULT_MAX_LENGTH) + ", ";

    int lengthWidth = someIPGen.getSomeIpArrayLengthWidthHelper(_accessor, _source);
    if (lengthWidth == -1 && _array != _source)
        lengthWidth = someIPGen.getSomeIpArrayLengthWidthHelper(_accessor, _array);
    if (lengthWidth != -1)
        parameter += std::to_string(lengthWidth);
    else
        parameter += std::to_string(someIPGen.SOMEIP_DEFAULT_LENGTH_WIDTH);

    return parameter;
}

std::string FTypeCollectionSomeIPDeploymentGenerator::getMapDeploymentParameter(
    const std::shared_ptr<BstIdl::FObject> &_map, const std::shared_ptr<BstIdl::FObject> &_source,
    const std::shared_ptr<BstIdl::FTypeCollection> &_tc, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::string parameter = "";
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    int minLength = someIPGen.getSomeIpMapMinLengthHelper(_accessor, _source);
    if (minLength == -1 && _map != _source)
        minLength = someIPGen.getSomeIpMapMinLengthHelper(_accessor, _map);
    if (minLength != -1)
        parameter += std::to_string(minLength) + ", ";
    else
        parameter += std::to_string(someIPGen.SOMEIP_DEFAULT_MIN_LENGTH);

    int maxLength = someIPGen.getSomeIpMapMaxLengthHelper(_accessor, _source);
    if (maxLength == -1 && _map != _source)
        maxLength = someIPGen.getSomeIpMapMaxLengthHelper(_accessor, _map);
    if (maxLength != -1)
        parameter += std::to_string(maxLength) + ", ";
    else
        parameter += std::to_string(someIPGen.SOMEIP_DEFAULT_MAX_LENGTH) + ", ";

    int lengthWidth = someIPGen.getSomeIpMapLengthWidthHelper(_accessor, _source);
    if (lengthWidth == -1 && _map != _source)
        lengthWidth = someIPGen.getSomeIpMapLengthWidthHelper(_accessor, _map);
    if (lengthWidth != -1)
        parameter += std::to_string(lengthWidth);
    else
        parameter += std::to_string(someIPGen.SOMEIP_DEFAULT_LENGTH_WIDTH) + ", ";

    return parameter;
}

} // namespace BstCommonAPI