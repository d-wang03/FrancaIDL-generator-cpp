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
#include "capicxx-core-tools/FInterfaceProxyGenerator.h"
#include "capicxx-core-tools/FrancaGeneratorExtensions.h"
#include "utilities/list_utility.h"
#include "utilities/string_utility.h"
#include <fstream>
#include <iostream>
namespace BstCommonAPI
{
void FInterfaceProxyGenerator::generateProxy(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                             const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor,
                                             bool generate_code, bool generateSyncCalls, const std::string &dir)
{
    m_generateSyncCalls = generateSyncCalls;
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    if (generate_code)
    {
        auto base_header_path = gen.getProxyBaseHeaderPath(fInterface);
        auto path = mk_dir(dir, base_header_path);
        if (path.empty())
            return;
        auto pos = base_header_path.find_last_of("/") + 1;
        auto file_name = base_header_path.substr(pos, base_header_path.size() - pos);
        std::ofstream OsWrite(path + "/" + file_name, std::ofstream::out);
        auto base_header = generateProxyBaseHeader(fInterface, deploymentAccessor);
        OsWrite << replace_all(base_header, "\t", "    ");
        OsWrite.close();
        auto header_path = gen.getProxyHeaderPath(fInterface);
        auto h_path = mk_dir(dir, header_path);
        if (h_path.empty())
            return;
        pos = header_path.find_last_of("/") + 1;
        auto s_file_name = header_path.substr(pos, header_path.size() - pos);
        std::ofstream OsWriteS(h_path + "/" + s_file_name, std::ofstream::out);
        auto source = generateProxyHeader(fInterface, deploymentAccessor);
        OsWriteS << replace_all(source, "\t", "    ");
        OsWriteS.close();
    }
}

std::string FInterfaceProxyGenerator::generateProxyBaseHeader(
    const std::shared_ptr<BstIdl::FInterface> &fInterface,
    const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor)
{
    std::string header;
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    auto typeGenerator = FTypeGenerator::getInstance();
    header = getLicense();
    header += "\n" + genExtention.generateCommonApiLicenseHeader();
    header += "\n" + FTypeGenerator::generateComments(fInterface, false);
    auto name = genExtention.getDefineName(fInterface);
    transform(name.begin(), name.end(), name.begin(), ::toupper);
    header += "\n#ifndef " + name + "_PROXY_BASE_HPP_";
    header += "\n#define " + name + "_PROXY_BASE_HPP_";

    header += "\n\n#include <" + genExtention.getHeaderPath(fInterface) + ">";
    if (fInterface->getBase() != nullptr)
    {
        header += "\n#include <" + genExtention.getProxyBaseHeaderPath(fInterface->getBase()) + ">";
    }
    std::list<std::string> libraryHeaders;
    std::list<std::string> generatedHeaders;
    typeGenerator.generateRequiredTypeIncludes(fInterface, generatedHeaders, libraryHeaders, false);
    generatedHeaders.sort();
    libraryHeaders.sort();
    for (auto requiredHeaderFile : generatedHeaders)
    {
        header += "\n#include <" + requiredHeaderFile + ">";
    }
    header += "\n\n#if !defined (COMMONAPI_INTERNAL_COMPILATION)";
    header += "\n#define COMMONAPI_INTERNAL_COMPILATION\n#define HAS_DEFINED_COMMONAPI_INTERNAL_COMPILATION_HERE";
    header += "\n#endif\n";
    for (auto requiredHeaderFile : libraryHeaders)
    {
        header += "\n#include <" + requiredHeaderFile + ">";
    }

    header.append("\n");
    if (!fInterface->getManagedInterfaces().empty())
    {
        header += "\n#include <CommonAPI/ProxyManager.hpp>";
    }
    if (genExtention.hasAttributes(fInterface))
    {
        header += "\n#include <CommonAPI/Attribute.hpp>";
    }
    if (genExtention.hasBroadcasts(fInterface))
    {
        header += "\n#include <CommonAPI/Event.hpp>";
    }
    header += "\n#include <CommonAPI/Proxy.hpp>";
    if (!fInterface->getMethods().empty())
        header.append("\n#include <functional>\n#include <future>\n");
    header += "\n#if defined (HAS_DEFINED_COMMONAPI_INTERNAL_COMPILATION_HERE)\n#undef "
              "COMMONAPI_INTERNAL_COMPILATION\n#undef HAS_DEFINED_COMMONAPI_INTERNAL_COMPILATION_HERE\n#endif";
    header += "\n" + genExtention.generateVersionNamespaceBegin(fInterface);
    auto model = std::dynamic_pointer_cast<BstIdl::FModel>(fInterface->getContainer());
    header += genExtention.generateNamespaceBeginDeclaration(model);
    header += "\n\nclass " + genExtention.getProxyBaseClassName(fInterface);
    header += "\n\t: virtual public ";
    if (fInterface->getBase() != nullptr)
    {
        header += genExtention.getTypeCollectionName(fInterface->getBase(), fInterface) + "ProxyBase {";
    }
    else
    {
        header += "CommonAPI::Proxy {";
    }
    header += "\npublic:";
    for (auto attribute : fInterface->getAttributes())
    {
        header += "\n\ttypedef CommonAPI::" + getCommonApiBaseClassname(attribute) + "<" +
                  genExtention.getTypeName(attribute, fInterface, true) + "> " + genExtention.getClassName(attribute) +
                  ";";
    }
    for (auto broadcast : fInterface->getBroadcasts())
    {
        header += "\n\ttypedef CommonAPI::Event< \n\t\t";
        std::list<std::string> argList;
        for (auto it : broadcast->getOutArgs())
            argList.emplace_back(genExtention.getTypeName(it, fInterface, true));
        header += join(argList, ", ");
        header += "\n\t> " + genExtention.getClassName(broadcast) + ";\n";
    }
    header += replace_all(generateAsyncCallbackTypedefs(fInterface), "\n", "\n\t");
    header += "\n\t";
    for (auto attribute : fInterface->getAttributes())
    {
        header += replace_all(FTypeGenerator::generateComments(attribute, false), "\n", "\n\t");
        header += "virtual " + genExtention.generateGetMethodDefinition(attribute) + " = 0;\n\t";
    }
    for (auto broadcast : fInterface->getBroadcasts())
    {
        header += replace_all(FTypeGenerator::generateComments(broadcast, false), "\n", "\n\t");
        header += "virtual " + genExtention.generateGetMethodDefinition(broadcast) + " = 0;\n\t";
    }

    for (auto method : fInterface->getMethods())
    {
        header += replace_all(FTypeGenerator::generateComments(method, false), "\n", "\n\t");
        if (m_generateSyncCalls || method->isFireAndForget())
        {
            if (method->isFireAndForget())
            {
                header += R"(
    /**
     * @invariant Fire And Forget
     */
)";
            }
            header += "virtual " + genExtention.generateDefinition(method, true) + " = 0;\n\t";
        }
        if (!method->isFireAndForget())
            header += "virtual " + genExtention.generateAsyncDefinition(method, true) + " = 0;\n\t";
    }

    for (auto managed : fInterface->getManagedInterfaces())
    {
        header += "virtual CommonAPI::ProxyManager& " + genExtention.proxyManagerGetterName(managed) + "() = 0;\n\t";
    }

    header += "\n\tvirtual std::future<void> getCompletionFuture() = 0;\n};\n";

    header += genExtention.generateNamespaceEndDeclaration(model);
    header += genExtention.generateVersionNamespaceEnd(fInterface);
    header += "\n" + genExtention.generateMajorVersionNamespace(fInterface);
    header += "\n#endif //" + name + "_PROXY_BASE_HPP_\n";
    return header;
}

std::string FInterfaceProxyGenerator::generateAsyncCallbackTypedefs(
    const std::shared_ptr<BstIdl::FInterface> &fInterface)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    std::list<std::string> callbackDefinitions;
    std::string str = "\n";
    for (auto fMethod : fInterface->getMethods())
    {
        if (!fMethod->isFireAndForget())
        {
            auto definitionSignature =
                genExtention.generateASyncTypedefSignature(fMethod) + genExtention.getAsyncCallbackClassName(fMethod);
            if (!contains(callbackDefinitions, definitionSignature))
            {
                if (genExtention.needsMangling(fMethod))
                    str += R"(
/*
 * This method has overloaded output parameters!
 *
 * A hash value was generated out of the names and types of these output
 * parameters to distinguish the callback typedefs. Be careful in changing the
 * type definitions in Franca if you actively use these typedefs in your code.
 */
)";
                str += "typedef std::function<void(" + genExtention.generateASyncTypedefSignature(fMethod) + ")> " +
                       genExtention.getAsyncCallbackClassName(fMethod) + ";\n";
                callbackDefinitions.emplace_back(definitionSignature);
            }
        }
    }
    return str;
}

std::string FInterfaceProxyGenerator::generateProxyHeader(
    const std::shared_ptr<BstIdl::FInterface> &fInterface,
    const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor)
{
    std::string header;
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    header = getLicense();
    header += "\n" + genExtention.generateCommonApiLicenseHeader();
    auto name = genExtention.getDefineName(fInterface);
    transform(name.begin(), name.end(), name.begin(), ::toupper);
    header += "\n\n#ifndef " + name + "_PROXY_HPP_";
    header += "\n#define " + name + "_PROXY_HPP_";
    header += "\n\n#include <" + genExtention.getProxyBaseHeaderPath(fInterface) + ">";
    if (fInterface->getBase() != nullptr)
    {
        header += "\n#include \"" + genExtention.getProxyHeaderPath(fInterface->getBase()) + "\"";
    }
    header += "\n\n#if !defined (COMMONAPI_INTERNAL_COMPILATION)";
    header += "\n#define COMMONAPI_INTERNAL_COMPILATION";
    header += "\n#define HAS_DEFINED_COMMONAPI_INTERNAL_COMPILATION_HERE";
    header += "\n#endif";

    if (genExtention.hasAttributes(fInterface))
    {
        header += "\n\n#include <CommonAPI/AttributeExtension.hpp>";
        header += "\n#include <CommonAPI/Factory.hpp>";
    }
    header += "\n\n#if defined (HAS_DEFINED_COMMONAPI_INTERNAL_COMPILATION_HERE)";
    header += "\n#undef COMMONAPI_INTERNAL_COMPILATION";
    header += "\n#undef HAS_DEFINED_COMMONAPI_INTERNAL_COMPILATION_HERE\n#endif\n";

    header += "\n" + genExtention.generateVersionNamespaceBegin(fInterface);
    auto model = std::dynamic_pointer_cast<BstIdl::FModel>(fInterface->getContainer());
    header += "\n" + genExtention.generateNamespaceBeginDeclaration(model);
    header += "\ntemplate <typename ... _AttributeExtensions>";
    header += "\nclass " + genExtention.getProxyClassName(fInterface);
    header += "\n\t: virtual public " + genExtention.getElementName(fInterface) + ",";
    header += "\n\t  virtual public " + genExtention.getProxyBaseClassName(fInterface) + ",";
    if (fInterface->getBase() != nullptr)
    {
        header +=
            genExtention.getTypeCollectionName(fInterface->getBase(), fInterface) + "Proxy<_AttributeExtensions...>,";
    }
    header += "\n\t  virtual public _AttributeExtensions... {\npublic:";

    header += "\n\t" + genExtention.getProxyClassName(fInterface) + "(std::shared_ptr<CommonAPI::Proxy> delegate);";
    header += "\n\t~" + genExtention.getProxyClassName(fInterface) + "();\n";

    header += "\n\ttypedef " + genExtention.getRelativeNameReference(fInterface, fInterface) + " InterfaceType;\n";
    header += R"(
    /**
     * Returns the CommonAPI address of the remote partner this proxy communicates with.
     */
    virtual const CommonAPI::Address &getAddress() const;

    /**
     * Returns true if the remote partner for this proxy is currently known to be available.
     */
    virtual bool isAvailable() const;

    /**
     * Returns true if the remote partner for this proxy is available.
     */
    virtual bool isAvailableBlocking() const;

    /**
     * Returns the wrapper class that is used to (de-)register for notifications about
     * the availability of the remote partner of this proxy.
     */
    virtual CommonAPI::ProxyStatusEvent& getProxyStatusEvent();

    /**
     * Returns the wrapper class that is used to access version information of the remote
     * partner of this proxy.
     */
    virtual CommonAPI::InterfaceVersionAttribute& getInterfaceVersionAttribute();
    
    virtual std::future<void> getCompletionFuture();
    )";

    for (auto attribute : fInterface->getAttributes())
    {
        header += replace_all(FTypeGenerator::generateComments(attribute, false), "\n", "\n\t");
        header += "\n\t /**\n\t * Returns the wrapper class that provides access to the attribute " +
                  genExtention.getElementName(attribute) + ".\n\t */\n\t";
        header += "\n\tvirtual " + genExtention.generateGetMethodDefinition(attribute);
        header += " {\n\t\treturn delegate_->get" + genExtention.getClassName(attribute) + "();\n\t}";
    }

    for (auto method : fInterface->getMethods())
    {
        if (m_generateSyncCalls || method->isFireAndForget())
        {
            header += "\n\t/**\n\t" + replace_all(FTypeGenerator::generateComments(method, true), "\n", "\n\t");
            header += " * Calls " + genExtention.getElementName(method) + " with ";
            if (method->isFireAndForget())
            {
                header += "Fire&Forget";
            }
            else
            {
                header += "synchronous semantics.";
            }
            header += "\n\t *";
            if (!method->getInArgs().empty())
            {
                header += "\n\t * All const parameters are input parameters to this method.";
            }
            if (!method->getOutArgs().empty())
            {
                header += "\n\t * All non-const parameters will be filled with the returned values.";
            }
            header += "\n\t * The CallStatus will be filled when the method returns and indicate either";
            header +=
                "\n\t * \"SUCCESS\" or which type of error has occurred. In case of an error, ONLY the CallStatus";
            header += "\n\t * will be set.\n\t */"; // 444
            header += "\n\tvirtual " + genExtention.generateDefinition(method, true) + ";";
        }
        if (!method->isFireAndForget())
        {
            header +=
                "\n\t/**\n\t * Calls " + genExtention.getElementName(method) + " with asynchronous semantics.\n\t *";
            header += "\n\t * The provided callback will be called when the reply to this call arrives or";
            header += "\n\t * an error occurs during the call. The CallStatus will indicate either \"SUCCESS\"";
            header += "\n\t * or which type of error has occurred. In case of any error, ONLY the CallStatus";
            header += "\n\t * will have a defined value.";
            header += "\n\t * The std::future returned by this method will be fulfilled at arrival of the reply.";
            header += "\n\t * It will provide the same value for CallStatus as will be handed to the callback.\n\t */";
            header += "\n\tvirtual " + genExtention.generateAsyncDefinition(method, true) + ";";
        }
    }
    header.append("\n\t");
    for (auto broadcast : fInterface->getBroadcasts())
    {
        header += replace_all(FTypeGenerator::generateComments(broadcast, false), "\n", "\n\t");
        header += "/**\n\t * Returns the wrapper class that provides access to the broadcast " +
                  genExtention.getElementName(broadcast) + "\n\t */\n\t";
        header += "virtual " + genExtention.generateGetMethodDefinition(broadcast) + " {\n\t";
        header += "\treturn delegate_->get" + genExtention.getClassName(broadcast) + "();\n\t}\n\t";
    }
    header.append("\n");

    for (auto managed : fInterface->getManagedInterfaces())
    {
        header += "\n\tvirtual CommonAPI::ProxyManager& " + genExtention.proxyManagerGetterName(managed) + "();";
    }
    header += "\nprivate:\n\tstd::shared_ptr< " + genExtention.getProxyBaseClassName(fInterface) + "> delegate_;\n\t};";
    header +=
        "\ntypedef " + genExtention.getProxyClassName(fInterface) + "<> " + getProxyDefaultClassName(fInterface) + ";";

    if (genExtention.hasAttributes(fInterface))
    {
        header += "\n\tnamespace " + getExtensionsSubnamespace(fInterface) + " {";
        for (auto attribute : fInterface->getAttributes())
        {
            header += "\n\t\t" + generateExtension(attribute, fInterface);
        }
        header += "\n\t} // namespace " + getExtensionsSubnamespace(fInterface);
    }
    header += "\n//\n// " + genExtention.getProxyClassName(fInterface) + " Implementation\n//";
    header += "\ntemplate <typename ... _AttributeExtensions>";
    header += "\n" + genExtention.getProxyClassName(fInterface) +
              "<_AttributeExtensions...>::" + genExtention.getProxyClassName(fInterface) +
              "(std::shared_ptr<CommonAPI::Proxy> delegate):";
    if (fInterface->getBase() != nullptr)
    {
        header +=
            "\n\t" + genExtention.getFullName(fInterface->getBase()) + "Proxy<_AttributeExtensions...>(delegate),";
    }
    header += "\n\t_AttributeExtensions(*(std::dynamic_pointer_cast< " +
              genExtention.getProxyBaseClassName(fInterface) + ">(delegate)))...,";
    header += "\n\tdelegate_(std::dynamic_pointer_cast< " + genExtention.getProxyBaseClassName(fInterface) +
              ">(delegate)) {\n}\n";
    header += "\ntemplate <typename ... _AttributeExtensions>\n";
    header += genExtention.getProxyClassName(fInterface) + "<_AttributeExtensions...>::~" +
              genExtention.getProxyClassName(fInterface) + "() {\n}\n";
    for (auto method : fInterface->getMethods())
    {
        header += "\n" + FTypeGenerator::generateComments(method, false);
        if (m_generateSyncCalls || method->isFireAndForget())
        {
            header += "template <typename ... _AttributeExtensions>";
            header += "\n" +
                      genExtention.generateDefinitionWithin(
                          method, genExtention.getProxyClassName(fInterface) + "<_AttributeExtensions...>", false) +
                      " {";
            for (auto arg : method->getInArgs())
            {
                if (!arg->isArray() && genExtention.supportsValidation(arg->getType()))
                {
                    header += "\n\tif (!_" + genExtention.getElementName(arg) +
                              ".validate()) {\n\t\t_internalCallStatus = "
                              "CommonAPI::CallStatus::INVALID_VALUE;\n\t\treturn;\n\t}";
                }
            }
            header += "\n\tdelegate_->" + genExtention.getElementName(method) + "(" + generateSyncVariableList(method) +
                      ");\n}\n";
        }
        if (!method->isFireAndForget())
        {
            header += "\ntemplate <typename ... _AttributeExtensions>";
            header += "\n" +
                      genExtention.generateAsyncDefinitionWithin(
                          method, genExtention.getProxyClassName(fInterface) + "<_AttributeExtensions...>", false) +
                      " {";
            for (auto arg : method->getInArgs())
            {
                if (!arg->isArray() && genExtention.supportsValidation(arg->getType()))
                {
                    header += "\n\tif (!_" + genExtention.getElementName(arg) + ".validate()) {";
                    auto tmp = replace_all(genExtention.generateDummyArgumentDefinitions(method), "\n", "\n\t\t");
                    header += tmp.empty() ? "" : ("\n\t\t" + tmp);
                    auto callbackArguments = genExtention.generateDummyArgumentList(method);
                    header += "\n\t\t_callback(CommonAPI::CallStatus::INVALID_VALUE";
                    if (callbackArguments != "")
                    {
                        header += ", " + callbackArguments;
                    }
                    header += ");\n\t\tstd::promise<CommonAPI::CallStatus> promise;";
                    header += "\n\t\tpromise.set_value(CommonAPI::CallStatus::INVALID_VALUE);";
                    header += "\n\t\treturn promise.get_future();\n\t}";
                }
            }
            header += "\n\treturn delegate_->" + genExtention.getElementName(method) + "Async(" +
                      generateASyncVariableList(method) + ");\n}\n";
        }
    }
    header += "\ntemplate <typename ... _AttributeExtensions>";
    header += "\nconst CommonAPI::Address &" + genExtention.getProxyClassName(fInterface) +
              "<_AttributeExtensions...>::getAddress() const {\n\treturn delegate_->getAddress();\n}\n";

    header += "\ntemplate <typename ... _AttributeExtensions>";
    header += "\nbool " + genExtention.getProxyClassName(fInterface) +
              "<_AttributeExtensions...>::isAvailable() const {\n\treturn delegate_->isAvailable();\n}\n";

    header += "\ntemplate <typename ... _AttributeExtensions>";
    header +=
        "\nbool " + genExtention.getProxyClassName(fInterface) +
        "<_AttributeExtensions...>::isAvailableBlocking() const {\n\treturn delegate_->isAvailableBlocking();\n}\n";

    header += "\ntemplate <typename ... _AttributeExtensions>";
    header += "\nCommonAPI::ProxyStatusEvent& " + genExtention.getProxyClassName(fInterface) +
              "<_AttributeExtensions...>::getProxyStatusEvent() {\n\treturn delegate_->getProxyStatusEvent();\n}\n";

    header += "\ntemplate <typename ... _AttributeExtensions>";
    header += "\nCommonAPI::InterfaceVersionAttribute& " + genExtention.getProxyClassName(fInterface) +
              "<_AttributeExtensions...>::getInterfaceVersionAttribute() {\n\treturn "
              "delegate_->getInterfaceVersionAttribute();\n}\n";

    header += "\ntemplate <typename ... _AttributeExtensions>";
    header += "\nstd::future<void> " + genExtention.getProxyClassName(fInterface) +
              "<_AttributeExtensions...>::getCompletionFuture() {\n\treturn delegate_->getCompletionFuture();\n}\n";

    for (auto managed : fInterface->getManagedInterfaces())
    {
        header += "\ntemplate <typename ... _AttributeExtensions>";
        header += "\nCommonAPI::ProxyManager& " + genExtention.getProxyClassName(fInterface) +
                  "<_AttributeExtensions...>" + genExtention.proxyManagerGetterName(managed) +
                  "() {\n\treturn delegate_->" + genExtention.proxyManagerGetterName(managed) + "();\n}";
    }

    header += "\n" + genExtention.generateNamespaceEndDeclaration(model);
    header += genExtention.generateVersionNamespaceEnd(fInterface);
    if (genExtention.hasAttributes(fInterface))
    {
        std::list<std::string> args;
        header += "\nnamespace CommonAPI {\ntemplate<template<typename > class _AttributeExtension>";
        header += "\nstruct DefaultAttributeProxyHelper< " + genExtention.getVersionPrefix(fInterface) +
                  genExtention.generateCppNamespace(model) + genExtention.getProxyClassName(fInterface) + ",";
        header += "\n\t_AttributeExtension> {\n\ttypedef typename " + genExtention.getVersionPrefix(fInterface) +
                  genExtention.generateCppNamespace(model) + genExtention.getProxyClassName(fInterface) + "<\n";
        for (auto it : fInterface->getAttributes())
        {
            args.emplace_back(genExtention.getVersionPrefix(fInterface) + genExtention.generateCppNamespace(model) +
                              getExtensionsSubnamespace(fInterface) + "::" + getExtensionClassName(it) +
                              "<_AttributeExtension>");
        }
        header += join(args, ",\n\t\t") + "\n\t> class_t;\n};\n}";
    }
    header += "\n" + genExtention.generateMajorVersionNamespace(fInterface);
    header += "\n#endif // " + name + "_PROXY_HPP_";
    return header;
}
std::string FInterfaceProxyGenerator::getCommonApiBaseClassname(const std::shared_ptr<BstIdl::FAttribute> &fAttribute)
{
    std::string baseClassname = "Attribute";
    if (fAttribute->isReadonly())
    {
        baseClassname = "Readonly" + baseClassname;
    }
    if (!fAttribute->isNoSubscriptions())
    { // isObservable
        baseClassname = "Observable" + baseClassname;
    }
    return baseClassname;
}

std::string FInterfaceProxyGenerator::getProxyDefaultClassName(const std::shared_ptr<BstIdl::FInterface> &fInterface)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    return genExtention.getProxyClassName(fInterface) + "Default";
}

std::string FInterfaceProxyGenerator::getExtensionsSubnamespace(const std::shared_ptr<BstIdl::FInterface> &fInterface)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    return genExtention.getElementName(fInterface) + "Extensions";
}

std::string FInterfaceProxyGenerator::getExtensionClassName(const std::shared_ptr<BstIdl::FAttribute> &fAttribute)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    return genExtention.getClassName(fAttribute) + "Extension";
}

std::string FInterfaceProxyGenerator::generateExtension(const std::shared_ptr<BstIdl::FAttribute> &fAttribute,
                                                        const std::shared_ptr<BstIdl::FInterface> &fInterface)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    auto str = FTypeGenerator::generateComments(fAttribute, false);
    str += "\ntemplate <template <typename > class _ExtensionType>";
    str += "\nclass " + getExtensionClassName(fAttribute) + " {";
    str += "\npublic:\n\ttypedef _ExtensionType< " + genExtention.getProxyBaseClassName(fInterface) +
           "::" + genExtention.getClassName(fAttribute) + "> extension_type;";
    str += "\n\tstatic_assert(std::is_base_of<typename CommonAPI::AttributeExtension< " +
           genExtention.getProxyBaseClassName(fInterface) + "::" + genExtention.getClassName(fAttribute) +
           ">, extension_type>::value,";
    str += "\n\t\t\"Not CommonAPI Attribute Extension!\");";
    str += "\n\t" + getExtensionClassName(fAttribute) + "(" + genExtention.getProxyBaseClassName(fInterface) +
           "& proxy): attributeExtension_(proxy.get" + genExtention.getClassName(fAttribute) + "()) {\n\t}";

    str += "\n\tinline extension_type& get" + getExtensionClassName(fAttribute) +
           "() {\n\t\treturn attributeExtension_;\n\t}";
    str += "\nprivate:\n\textension_type attributeExtension_;\n};";
    return str;
}

std::string FInterfaceProxyGenerator::generateSyncVariableList(const std::shared_ptr<BstIdl::FMethod> &fMethod)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    std::list<std::string> syncVariableList;
    for (auto it : fMethod->getInArgs())
    {
        syncVariableList.emplace_back("_" + genExtention.getElementName(it));
    }
    syncVariableList.emplace_back("_internalCallStatus");
    if (genExtention.hasError(fMethod))
    {
        syncVariableList.emplace_back("_error");
    }
    for (auto it : fMethod->getOutArgs())
    {
        syncVariableList.emplace_back("_" + genExtention.getElementName(it));
    }
    if (!fMethod->isFireAndForget())
    {
        syncVariableList.emplace_back("_info");
    }

    return join(syncVariableList, ", ");
}

std::string FInterfaceProxyGenerator::generateASyncVariableList(const std::shared_ptr<BstIdl::FMethod> &fMethod)
{
    std::list<std::string> argList;
    for (const auto &item : fMethod->getInArgs())
        argList.emplace_back("_" + item->getName());
    argList.emplace_back("_callback");
    if (!fMethod->isFireAndForget())
        argList.emplace_back("_info");
    return join(argList, ", ");
}
} // namespace BstCommonAPI