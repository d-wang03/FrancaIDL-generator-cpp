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
#include "capicxx-core-tools/FInterfaceStubGenerator.h"
#include "capicxx-core-tools/FrancaGeneratorExtensions.h"
#include "model/FModelManager.h"
#include "utilities/string_utility.h"
#include <fstream>
#include <iostream>
#include <map>
namespace BstCommonAPI
{
void FInterfaceStubGenerator::generateStub(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                           const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor,
                                           bool generate_code, bool generate_skeleton, const std::string &dir)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    if (generate_code)
    {
        auto header_path = gen.getStubHeaderPath(fInterface);
        auto path = mk_dir(dir, header_path);
        if (path.empty())
            return;
        auto pos = header_path.find_last_of("/") + 1;
        auto file_name = header_path.substr(pos, header_path.size() - pos);
        std::ofstream OsWrite(path + "/" + file_name, std::ofstream::out);
        auto header = generateStubHeader(fInterface, deploymentAccessor);
        OsWrite << replace_all(header, "\t", "    ");
        OsWrite.close();
        if (generate_skeleton)
        {
            auto stubDefaultHeaderPath = gen.getStubDefaultHeaderPath(fInterface);
            auto d_h_path = mk_dir(dir, stubDefaultHeaderPath);
            if (d_h_path.empty())
                return;
            pos = stubDefaultHeaderPath.find_last_of("/") + 1;
            auto h_file_name = stubDefaultHeaderPath.substr(pos, stubDefaultHeaderPath.size() - pos);
            std::ofstream OsWriteH(d_h_path + "/" + h_file_name, std::ofstream::out);
            auto stubDefaultHeader = generateStubDefaultHeader(fInterface, deploymentAccessor);
            OsWriteH << replace_all(stubDefaultHeader, "\t", "    ");
            OsWriteH.close();
        }
    }
}
std::string FInterfaceStubGenerator::generateStubHeader(
    const std::shared_ptr<BstIdl::FInterface> &fInterface,
    const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor)
{
    std::string header;
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    auto typeGen = FTypeGenerator::getInstance();
    header = genExtention.generateCommonApiLicenseHeader();
    header += "\n" + getLicense();
    header += "\n" + FTypeGenerator::generateComments(fInterface, false);
    auto define_name = genExtention.getDefineName(fInterface);
    transform(define_name.begin(), define_name.end(), define_name.begin(), ::toupper);
    header += "\n\n#ifndef " + define_name + "_STUB_HPP_";
    header += "\n#define " + define_name + "_STUB_HPP_";
    header += "\n\n#include <functional>";
    header += "\n#include <sstream>\n";
    std::list<std::string> generatedHeaders;
    std::list<std::string> libraryHeaders;
    typeGen.generateInheritanceIncludes(fInterface, generatedHeaders, libraryHeaders);
    typeGen.generateRequiredTypeIncludes(fInterface, generatedHeaders, libraryHeaders, true);
    genExtention.generateSelectiveBroadcastStubIncludes(fInterface, generatedHeaders, libraryHeaders);
    generatedHeaders.sort();
    libraryHeaders.sort();
    for (auto requiredHeaderFile : generatedHeaders)
    {
        header += "\n#include <" + requiredHeaderFile + ">";
    }
    header += "\n#include <" + genExtention.getHeaderPath(fInterface) + ">\n";

    header += "\n#if !defined (COMMONAPI_INTERNAL_COMPILATION)\n#define COMMONAPI_INTERNAL_COMPILATION\n#define "
              "HAS_DEFINED_COMMONAPI_INTERNAL_COMPILATION_HERE\n#endif";
    for (auto requiredHeaderFile : libraryHeaders)
    {
        header += "\n#include <" + requiredHeaderFile + ">";
    }
    if (!fInterface->getAttributes().empty())
    {
        header += "\n#include <mutex>";
    }
    header += "\n#include <CommonAPI/Stub.hpp>\n";
    header += "\n#if defined (HAS_DEFINED_COMMONAPI_INTERNAL_COMPILATION_HERE)\n#undef COMMONAPI_INTERNAL_COMPILATION";
    header += "\n#undef HAS_DEFINED_COMMONAPI_INTERNAL_COMPILATION_HERE\n#endif\n";

    header += genExtention.generateVersionNamespaceBegin(fInterface);
    auto model = std::dynamic_pointer_cast<BstIdl::FModel>(fInterface->getContainer());
    header += genExtention.generateNamespaceBeginDeclaration(model);
    header += "\n\n/**\n * Receives messages from remote and handles all dispatching of deserialized calls\n * to a "
              "stub for the service ";
    header += genExtention.getElementName(fInterface) + ". Also provides means to send broadcasts\n";
    header += R"( * and attribute-changed-notifications of observable attributes as defined by this service.
 * An application developer should not need to bother with this class.
 */)";
    header += "\nclass " + genExtention.getStubAdapterClassName(fInterface);
    header += "\n\t: public virtual CommonAPI::StubAdapter,";
    header += "\n\t  public virtual " + genExtention.getElementName(fInterface);
    if (fInterface->getBase() != nullptr)
    {
        header += ",\n\t  public virtual " + genExtention.getTypeCollectionName(fInterface->getBase(), fInterface) +
                  "StubAdapter";
    }
    header += "{\npublic:";
    for (auto attribute : fInterface->getAttributes())
    {
        if (genExtention.isObservable(attribute))
        {
            header += "\n\t//Notifies all remote listeners about a change of value of the attribute " +
                      genExtention.getElementName(attribute) + ".";
            header += "\n\tvirtual void " + genExtention.getStubAdapterClassFireChangedMethodName(attribute) +
                      "(const " + genExtention.getTypeName(attribute, fInterface, true) + "& " +
                      genExtention.getElementName(attribute) + ") = 0;";
        }
    }
    for (auto broadcast : fInterface->getBroadcasts())
    {
        if (broadcast->isSelective())
        {
            header += "\n\t/**\n\t * Sends a selective broadcast event for " + genExtention.getElementName(broadcast) +
                      ". Should not be called directly.";
            header += "\n\t * Instead, the \"fire" + broadcast->getName() +
                      "Event\" methods of the stub should be used.\n\t*/";
            header += "\n\tvirtual void " + genExtention.getStubAdapterClassFireSelectiveMethodName(broadcast) + "(" +
                      genExtention.generateFireSelectiveSignatur(broadcast, fInterface) + ") = 0;";
            header += "\n\tvirtual void " + genExtention.getStubAdapterClassSendSelectiveMethodName(broadcast) + "(" +
                      genExtention.generateSendSelectiveSignatur(broadcast, fInterface, true) + ") = 0;";
            header += "\n\tvirtual void " + genExtention.getSubscribeSelectiveMethodName(broadcast) +
                      "(const std::shared_ptr<CommonAPI::ClientId> _client, bool& _success) = 0;";
            header += "\n\tvirtual void " + genExtention.getUnsubscribeSelectiveMethodName(broadcast) +
                      "(const std::shared_ptr<CommonAPI::ClientId> _client) = 0;";
            header += "\n\tvirtual std::shared_ptr<CommonAPI::ClientIdList> const " +
                      genExtention.getStubAdapterClassSubscribersMethodName(broadcast) + "() = 0;";
        }
        else
        {
            if (!genExtention.isErrorType(broadcast, deploymentAccessor))
                header += "\n\t/**\n\t* Sends a broadcast event for " + genExtention.getElementName(broadcast) +
                          ". Should not be called directly.";
            header += "\n\t* Instead, the \"fire" + broadcast->getName() +
                      "Event\" methods of the stub should be used.\n\t*/";
            header += "\n\tvirtual void " + genExtention.getStubAdapterClassFireEventMethodName(broadcast) + "(";
            for (auto it : broadcast->getOutArgs())
            {
                if (it == broadcast->getOutArgs().front())
                    header += "const " + genExtention.getTypeName(it, fInterface, true) + " &_" +
                              genExtention.getElementName(it);
                else
                {
                    header += ", const " + genExtention.getTypeName(it, fInterface, true) + " &_" +
                              genExtention.getElementName(it);
                }
            }
            header += ") = 0;";
        }
    }
    for (auto managed : fInterface->getManagedInterfaces())
    {
        header += "\n\tvirtual " + genExtention.stubRegisterManagedMethod(managed) + " = 0;";
        header += "\n\tvirtual bool " + genExtention.stubDeregisterManagedName(managed) + "(const std::string&) = 0;";
        header += "\n\tvirtual std::set<std::string>& " + genExtention.stubManagedSetGetterName(managed) + "() = 0;";
    }
    header += "\n\n\tvirtual void deactivateManagedInstances() = 0;";
    for (auto attribute : fInterface->getAttributes())
    {

        header += "\n\tvoid " + genExtention.getStubClassLockMethodName(attribute) + "(bool _lockAccess) {";
        header += "\n\t\tif (_lockAccess) {";
        header += "\n\t\t\t" + getStubAdapterAttributeMutexName(attribute) + ".lock();\n\t\t} else {";
        header += "\n\t\t\t" + getStubAdapterAttributeMutexName(attribute) + ".unlock();\n\t\t}\n\t}\n";
    }
    header += "\n";
    header += R"(
protected:
    /**
     * Defines properties for storing the ClientIds of clients / proxies that have
     * subscribed to the selective broadcasts
     */)";
    for (auto broadcast : fInterface->getBroadcasts())
    {
        if (broadcast->isSelective())
        {
            header += "\n\tstd::shared_ptr<CommonAPI::ClientIdList> " +
                      genExtention.getStubAdapterClassSubscriberListPropertyName(broadcast) + ";";
        }
    }
    for (auto attribute : fInterface->getAttributes())
    {
        header += "\n\tstd::recursive_mutex " + getStubAdapterAttributeMutexName(attribute) + ";";
    }
    header += "\n};\n";
    header += R"(
/**
 * Defines the necessary callbacks to handle remote set events related to the attributes
 * defined in the IDL description for «fInterface.elementName».
 * For each attribute two callbacks are defined:
 * - a verification callback that allows to verify the requested value and to prevent setting
 *   e.g. an invalid value ("onRemoteSet<AttributeName>").
 * - an action callback to do local work after the attribute value has been changed
 *   ("onRemote<AttributeName>Changed").
 *
 * This class and the one below are the ones an application developer needs to have
 * a look at if he wants to implement a service.
 */)";
    replace_one(header, "«fInterface.elementName»", fInterface->getName());
    header += "\nclass " + genExtention.getStubRemoteEventClassName(fInterface);
    if (fInterface->getBase() != nullptr)
    {
        header += "\n\t: public virtual " + genExtention.getTypeCollectionName(fInterface->getBase(), fInterface) +
                  "StubRemoteEvent";
    }
    header += "\n{\npublic:\n\tvirtual ~" + genExtention.getStubRemoteEventClassName(fInterface) + "() { }";
    for (auto attribute : fInterface->getAttributes())
    {
        if (!attribute->isReadonly())
        {
            header += "\n\t/// Verification callback for remote set requests on the attribute " +
                      genExtention.getElementName(attribute);
            header += "\n\tvirtual bool " + genExtention.getStubRemoteEventClassSetMethodName(attribute) +
                      "(const std::shared_ptr<CommonAPI::ClientId> _client, " +
                      genExtention.getTypeName(attribute, fInterface, true) + " _value) = 0;";
            header += "\n\t/// Action callback for remote set requests on the attribute " +
                      genExtention.getElementName(attribute);
            header +=
                "\n\tvirtual void " + genExtention.getStubRemoteEventClassChangedMethodName(attribute) + "() = 0;";
        }
    }
    header += "\n};\n";
    std::string tmp = R"(
/**
 * Defines the interface that must be implemented by any class that should provide
 * the service «fInterface.elementName» to remote clients.
 * This class and the one above are the ones an application developer needs to have
 * a look at if he wants to implement a service.
 */)";
    replace_one(tmp, "«fInterface.elementName»", fInterface->getName());
    header += tmp + "\nclass " + genExtention.getStubClassName(fInterface);
    header += "\n\t: public virtual " + genExtention.getStubCommonAPIClassName(fInterface);
    if (fInterface->getBase() != nullptr)
    {
        header +=
            ",\n\t  public virtual " + genExtention.getTypeCollectionName(fInterface->getBase(), fInterface) + "Stub";
    }
    header += "\n{\npublic:";
    m_counterMap.clear();
    m_methodrepliesMap.clear();
    for (auto method : fInterface->getMethods())
    {
        if (!method->isFireAndForget())
        {
            header += "\t" + generateMethodReplyDeclarations(deploymentAccessor, method, fInterface, m_counterMap,
                                                             m_methodrepliesMap);
        }
    }
    header += "\n\n\tvirtual ~" + genExtention.getStubClassName(fInterface) + "() {}";
    header += R"(
    void lockInterfaceVersionAttribute(bool _lockAccess) { static_cast<void>(_lockAccess); }
    bool hasElement(const uint32_t _id) const {
        ID_IMP
    }
    virtual const CommonAPI::Version& getInterfaceVersion(std::shared_ptr<CommonAPI::ClientId> _client) = 0;
        )";
    auto itsSize = genExtention.getRsize(fInterface);
    std::string idImp = itsSize ? ("return (_id < " + std::to_string(itsSize) + ");")
                                : ("(void)_id; // (_id=" + std::to_string(itsSize) + ")\n\t\treturn false;");
    replace_one(header, "ID_IMP", idImp);
    for (auto attribute : fInterface->getAttributes())
    {
        header += "\n\t" + FTypeGenerator::generateComments(attribute, false);
        header += "\n\t// Provides getter access to the attribute " + genExtention.getElementName(attribute);
        std::string definition = "";
        if (FTypeGenerator::isdeprecated(attribute->getComment()))
        {
            definition = " COMMONAPI_DEPRECATED";
        }
        header += "\n\tvirtual" + definition + " const " + genExtention.getTypeName(attribute, fInterface, true) +
                  " &" + genExtention.getStubClassGetMethodName(attribute) +
                  "(const std::shared_ptr<CommonAPI::ClientId> _client) = 0;";
        if (genExtention.isObservable(attribute))
        {
            header += "\n\t// sets attribute with the given value and propagates it to the adapter";
            header += "\n\tvirtual void " + genExtention.getStubAdapterClassFireChangedMethodName(attribute) + "(" +
                      genExtention.getTypeName(attribute, fInterface, true) + " _value) {";
            header += "\n\t\tauto stubAdapter = " + genExtention.getStubCommonAPIClassName(fInterface) +
                      "::stubAdapter_.lock();";
            header += "\n\t\tif (stubAdapter)\n\t\t\tstubAdapter->" +
                      genExtention.getStubAdapterClassFireChangedMethodName(attribute) + "(_value);\n\t}";
        }
        header += "\n\tvoid " + genExtention.getStubClassLockMethodName(attribute) + "(bool _lockAccess) {";
        header +=
            "\n\t\tauto stubAdapter = " + genExtention.getStubCommonAPIClassName(fInterface) + "::stubAdapter_.lock();";
        header += "\n\t\tif (stubAdapter)\n\t\t\tstubAdapter->" + genExtention.getStubClassLockMethodName(attribute) +
                  "(_lockAccess);";
        header += "\n\t}";
    }
    for (auto method : fInterface->getMethods())
    {
        header += "\n\t" + replace_all(FTypeGenerator::generateComments(method, false), "\n", "\n\t");
        header += "// This is the method that will be called on remote calls on the method " +
                  genExtention.getElementName(method) + ".";
        std::string definition = "";
        if (FTypeGenerator::isdeprecated(method->getComment()))
        {
            definition = " COMMONAPI_DEPRECATED";
        }
        header += "\n\tvirtual" + definition + " void " + genExtention.getElementName(method) + "(" +
                  genExtention.generateOverloadedStubSignature(method, m_methodrepliesMap[method]) + ") = 0;";
    }
    for (auto broadcast : fInterface->getBroadcasts())
    {
        header += "\n\t" + replace_all(FTypeGenerator::generateComments(broadcast, false), "\n", "\n\t");
        std::string definition = "";
        if (FTypeGenerator::isdeprecated(broadcast->getComment()))
        {
            definition = " COMMONAPI_DEPRECATED";
        }
        if (broadcast->isSelective())
        {
            header += "/**\n\t * Sends a selective broadcast event for" + genExtention.getElementName(broadcast) +
                      " to the given ClientIds.";
            header += "\n\t * The ClientIds must all be out of the set of subscribed clients.";
            header +=
                "\n\t * If no ClientIds are given, the selective broadcast is sent to all subscribed clients.\n\t */";
            header += "\n\tvirtual" + definition + " void " +
                      genExtention.getStubAdapterClassFireSelectiveMethodName(broadcast) + "(" +
                      genExtention.generateSendSelectiveSignatur(broadcast, fInterface, true) + ") = 0;";
            header +=
                "\n\t// retrieves the list of all subscribed clients for " + genExtention.getElementName(broadcast);
            header += "\n\tvirtual std::shared_ptr<CommonAPI::ClientIdList> const " +
                      genExtention.getStubAdapterClassSubscribersMethodName(broadcast) + "() {";
            header += "\n\t\tauto stubAdapter = " + genExtention.getStubCommonAPIClassName(fInterface) +
                      "::stubAdapter_.lock();";
            header += "\n\t\tif (stubAdapter)\n\t\t\treturn(stubAdapter->" +
                      genExtention.getStubAdapterClassSubscribersMethodName(broadcast) + "());";
            header += "\n\t\telse\n\t\t\treturn NULL;\n\t}";
            header += "\n\t// Hook method for reacting on new subscriptions or removed subscriptions respectively for "
                      "selective broadcasts.";
            header += "\n\tvirtual void " + genExtention.getSubscriptionChangedMethodName(broadcast) +
                      "(const std::shared_ptr<CommonAPI::ClientId> _client, const "
                      "CommonAPI::SelectiveBroadcastSubscriptionEvent _event) = 0;";
            header += "\n\t// Hook method for reacting accepting or denying new subscriptions";
            header += "\n\tvirtual bool " + genExtention.getSubscriptionRequestedMethodName(broadcast) +
                      "(const std::shared_ptr<CommonAPI::ClientId> _client) = 0;";
            header += "\n\tvirtual void " + genExtention.getStubAdapterClassSendSelectiveMethodName(broadcast) + "(" +
                      genExtention.generateSendSelectiveSignatur(broadcast, fInterface, true) + ") {";
            header += "\n\t\tauto stubAdapter = " + genExtention.getStubCommonAPIClassName(fInterface) +
                      "::stubAdapter_.lock();";
            header += "\n\t\tif (stubAdapter)\n\t\t\tstubAdapter->" +
                      genExtention.getStubAdapterClassSendSelectiveMethodName(broadcast) + "(";
            for (auto it : broadcast->getOutArgs())
            {
                header += "_" + genExtention.getElementName(it) + ", ";
            }
            header += "_receivers);\n\t}";
        }
        else
        {
            if (!genExtention.isErrorType(broadcast, deploymentAccessor))
            {
                header += "\n\t// Sends a broadcast event for " + genExtention.getElementName(broadcast) + ".";
                header += "\n\tvirtual" + definition + " void " +
                          genExtention.getStubAdapterClassFireEventMethodName(broadcast) + "(";
                for (auto it : broadcast->getOutArgs())
                {
                    if (it == broadcast->getOutArgs().front())
                        header += "const " + genExtention.getTypeName(it, fInterface, true) + " &_" +
                                  genExtention.getElementName(it);
                    else
                        header += ", const " + genExtention.getTypeName(it, fInterface, true) + " &_" +
                                  genExtention.getElementName(it);
                }
                header += ") {";
                header += "\n\t\tauto stubAdapter = " + genExtention.getStubCommonAPIClassName(fInterface) +
                          "::stubAdapter_.lock();";
                header += "\n\t\tif (stubAdapter)\n\t\t\tstubAdapter->" +
                          genExtention.getStubAdapterClassFireEventMethodName(broadcast) + "(";
                for (auto it : broadcast->getOutArgs())
                {
                    if (it == broadcast->getOutArgs().front())
                        header += "_" + genExtention.getElementName(it);
                    else
                        header += ", _" + genExtention.getElementName(it);
                }
                header += ");\n\t}";
            }
        }
    }
    for (auto managed : fInterface->getManagedInterfaces())
    {
        header += "\n\tvirtual bool " + genExtention.stubRegisterManagedMethodWithInstanceNumberImpl(managed) + " {";
        header += "\n\t\tstd::stringstream ss;";
        header +=
            "\n\t\tauto stubAdapter = " + genExtention.getStubCommonAPIClassName(fInterface) + "::stubAdapter_.lock();";
        header += "\n\t\tif (stubAdapter) {";
        header += "\n\t\t\tss << stubAdapter->getAddress().getInstance() << \".i\" << _instanceNumber;";
        header += "\n\t\t\tstd::string instance = ss.str();";
        header += "\n\t\t\treturn stubAdapter->" + genExtention.stubRegisterManagedName(managed) + "(_stub, instance);";
        header += "\n\t\t} else {\n\t\t\treturn false;\n\t\t}\n\t}";

        header += "\n\tvirtual bool " + genExtention.stubRegisterManagedMethodImpl(managed) + " {";
        header +=
            "\n\t\tauto stubAdapter = " + genExtention.getStubCommonAPIClassName(fInterface) + "::stubAdapter_.lock();";
        header += "\n\t\tif (stubAdapter) {";
        header += "\n\t\t\treturn stubAdapter->" + genExtention.stubRegisterManagedName(managed) + "(_stub, instance);";
        header += "\n\t\t} else {\n\t\t\treturn false;\n\t\t}\n\t}";

        header +=
            "\n\tvirtual bool " + genExtention.stubDeregisterManagedName(managed) + "(const std::string& _instance) {";
        header +=
            "\n\t\tauto stubAdapter = " + genExtention.getStubCommonAPIClassName(fInterface) + "::stubAdapter_.lock();";
        header += "\n\t\tif (stubAdapter) {";
        header += "\n\t\t\treturn stubAdapter->" + genExtention.stubDeregisterManagedName(managed) + "(_instance);";
        header += "\n\t\t} else {\n\t\t\treturn false;\n\t\t}\n\t}";

        header += "\n\tvirtual std::set<std::string>& " + genExtention.stubManagedSetGetterName(managed) + "() {";
        header +=
            "\n\t\tauto stubAdapter = " + genExtention.getStubCommonAPIClassName(fInterface) + "::stubAdapter_.lock();";
        header += "\n\t\tif (stubAdapter) {";
        header += "\n\t\t\treturn stubAdapter->" + genExtention.stubManagedSetGetterName(managed) + "();";
        header += "\n\t\t} else {\n\t\t\tstatic std::set<std::string> emptySet = "
                  "std::set<std::string>();\n\t\t\treturn emptySet;\n\t\t}\n\t}";
    }
    header += "\n\n\tusing " + genExtention.getStubCommonAPIClassName(fInterface) + "::initStubAdapter;";
    header +=
        "\n\ttypedef " + genExtention.getStubCommonAPIClassName(fInterface) + "::StubAdapterType StubAdapterType;";
    header += "\n\ttypedef " + genExtention.getStubCommonAPIClassName(fInterface) +
              "::RemoteEventHandlerType RemoteEventHandlerType;";
    header += "\n\ttypedef " + genExtention.getStubRemoteEventClassName(fInterface) + " RemoteEventType;";
    header += "\n\ttypedef " + genExtention.getElementName(fInterface) + " StubInterface;\n};\n";
    header += "\n" + genExtention.generateNamespaceEndDeclaration(model);
    header += genExtention.generateVersionNamespaceEnd(fInterface);
    header += "\n" + genExtention.generateMajorVersionNamespace(fInterface);
    header += "\n#endif // " + define_name + "_STUB_HPP_";
    return header;
}
std::string FInterfaceStubGenerator::generateStubDefaultHeader(
    const std::shared_ptr<BstIdl::FInterface> &fInterface,
    const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor)
{
    std::string header;
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    header = genExtention.generateCommonApiLicenseHeader();
    header += "\n" + getLicense();
    auto define_name = genExtention.getDefineName(fInterface);
    transform(define_name.begin(), define_name.end(), define_name.begin(), ::toupper);
    header += "\n#ifndef " + define_name + "_STUB_DEFAULT_HPP_";
    header += "\n#define " + define_name + "_STUB_DEFAULT_HPP_\n";
    if (fInterface->getBase() != nullptr)
    {
        header += "\n#include <" + genExtention.getStubDefaultHeaderPath(fInterface->getBase()) + ">";
    }
    header += "\n#include <CommonAPI/Export.hpp>";
    header += "\n#include <" + genExtention.getStubHeaderPath(fInterface) + ">";
    header += "\n#include <cassert>\n#include <sstream>\n\n# if defined(_MSC_VER)\n#  if _MSC_VER >= 1300";
    header += R"(
/*
 * Diamond inheritance is used for the CommonAPI::Proxy base class.
 * The Microsoft compiler put warning (C4250) using a desired c++ feature: "Delegating to a sister class"
 * A powerful technique that arises from using virtual inheritance is to delegate a method from a class in another class
 * by using a common abstract base class. This is also called cross delegation.
 */
#    pragma warning( disable : 4250 )
#  endif
# endif
)";
    header += genExtention.generateVersionNamespaceBegin(fInterface);
    auto model = std::dynamic_pointer_cast<BstIdl::FModel>(fInterface->getContainer());
    header += genExtention.generateNamespaceBeginDeclaration(model) + "\n";
    header += R"(
/**
 * Provides a default implementation for «fInterface.stubRemoteEventClassName» and
 * «fInterface.stubClassName». Method callbacks have an empty implementation,
 * remote set calls on attributes will always change the value of the attribute
 * to the one received.
 *
 * Override this stub if you only want to provide a subset of the functionality
 * that would be defined for this service, and/or if you do not need any non-default
 * behaviour.
 */
)";
    header = header.replace(header.find("«fInterface.stubClassName»"), 28, genExtention.getStubClassName(fInterface));
    header = header.replace(header.find("«fInterface.stubRemoteEventClassName»"), 39,
                            genExtention.getStubRemoteEventClassName(fInterface));
    header += "class COMMONAPI_EXPORT_CLASS_EXPLICIT " + genExtention.getStubDefaultClassName(fInterface);
    header += "\n\t: public virtual " + genExtention.getStubClassName(fInterface);
    if (fInterface->getBase() != nullptr)
    {
        header += ",\n\t  public virtual " + genExtention.getTypeCollectionName(fInterface->getBase(), fInterface) +
                  "StubDefault";
    }
    header += "{\npublic:";
    // ctor
    header += "\n\tCOMMONAPI_EXPORT " + genExtention.getStubDefaultClassName(fInterface) + "()";
    header += "\n\t\t: remoteEventHandler_(this),";
    if (!fInterface->getManagedInterfaces().empty())
    {
        header += "\n\t\t  autoInstanceCounter_(0),";
    }
    header +=
        "\n\t\t  interfaceVersion_(" + genExtention.getElementName(fInterface) + "::getInterfaceVersion()) {\n\t}\n";
    // getInterface
    header += "\n\tCOMMONAPI_EXPORT const CommonAPI::Version& getInterfaceVersion(std::shared_ptr<CommonAPI::ClientId> "
              "_client) {";
    header += "\n\t\t(void)_client;\n\t\treturn interfaceVersion_;\n\t}\n";
    // initStubAdapter
    header += "\n\tCOMMONAPI_EXPORT " + genExtention.getStubRemoteEventClassName(fInterface) +
              "* initStubAdapter(const std::shared_ptr< " + genExtention.getStubAdapterClassName(fInterface) +
              "> &_adapter) {";
    if (fInterface->getBase() != nullptr)
    {
        header +=
            "\n\t\t" + genExtention.getStubDefaultClassName(fInterface->getBase()) + "::initStubAdapter(_adapter);";
    }
    header += "\n\t\t" + genExtention.getStubCommonAPIClassName(fInterface) + "::stubAdapter_ = _adapter;";
    header += "\n\t\treturn &remoteEventHandler_;\n\t}\n";
    // attributes
    for (auto attribute : fInterface->getAttributes())
    {
        auto typeName = genExtention.getTypeName(attribute, fInterface, true);
        header += "\n\tCOMMONAPI_EXPORT virtual const " + typeName + "& " +
                  genExtention.getStubClassGetMethodName(attribute) + "() {";
        header += "\n\t\treturn " + getStubDefaultClassVariableName(attribute) + ";\n\t}\n";
        header += "\n\tCOMMONAPI_EXPORT virtual const " + typeName + "& " +
                  genExtention.getStubClassGetMethodName(attribute) +
                  "(const std::shared_ptr<CommonAPI::ClientId> _client) {";
        header +=
            "\n\t\t(void)_client;\n\t\treturn " + genExtention.getStubClassGetMethodName(attribute) + "();\n\t}\n";
        header += "\n\tCOMMONAPI_EXPORT virtual void " + getStubDefaultClassSetMethodName(attribute) + "(" + typeName +
                  " _value) {";
        if (genExtention.isObservable(attribute))
        {
            header += "\n\t\tconst bool valueChanged = " + getStubDefaultClassTrySetMethodName(attribute) +
                      "(std::move(_value));";
            header += "\n\t\tif (valueChanged) {";
            header += "\n\t\t\t" + genExtention.getStubAdapterClassFireChangedMethodName(attribute) + "(" +
                      getStubDefaultClassVariableName(attribute) + ");\n\t\t}";
        }
        else
        {
            header += "\n\t\t(void)" + getStubDefaultClassTrySetMethodName(attribute) + "(_value);";
        }
        header += "\n\t}";
        if (!attribute->isReadonly())
        {
            header += "\n\tCOMMONAPI_EXPORT virtual void " + getStubDefaultClassSetMethodName(attribute) +
                      "(const std::shared_ptr<CommonAPI::ClientId> _client, " + typeName + " _value) {";
            header += "\n\t\t(void)_client;\n\t\t" + getStubDefaultClassSetMethodName(attribute) + "(_value);\n\t}";
        }
    }
    for (auto method : fInterface->getMethods())
    {
        auto replies = m_methodrepliesMap[method];
        header += "\n\t" + replace_all(FTypeGenerator::generateComments(method, false), "\n", "\n\t");
        header += "COMMONAPI_EXPORT virtual void " + genExtention.getElementName(method) + "(" +
                  genExtention.generateOverloadedStubSignature(method, replies) + ") {";
        header += "\n\t\t(void)_client;";
        if (!method->getInArgs().empty())
        {
            for (auto it : method->getInArgs())
            {
                header += "\n\t\t(void)_" + it->getName() + ";";
            }
        }
        if (!method->isFireAndForget())
        {
            if (!replies.empty())
            {
                bool containValue = false;
                std::for_each(replies.cbegin(), replies.cend(), [&containValue](const auto &item) {
                    if (item.second)
                        containValue = true;
                });
                if (containValue)
                {
                    header += "\n\t\t(void)_call;";
                    for (auto reply : replies)
                    {
                        auto methodName = reply.first;
                        auto isErrorReply = reply.second;
                        if (isErrorReply)
                        {
                            header += "\n\t\t(void)_" + methodName + "Reply;";
                        }
                    }
                }
            }
            auto tmp = replace_all(genExtention.generateDummyArgumentDefinitions(method), "\n", "\n\t\t");
            header += tmp.empty() ? "" : "\n\t\t" + tmp;
            for (auto arg : method->getOutArgs())
            {
                if ((!arg->isArray()) && genExtention.supportsValidation(arg->getType()))
                {
                    header += "\n\t\tif (!" + genExtention.getElementName(arg) + ".validate()) {\n\t\t\treturn;\n\t\t}";
                }
            }
            header += "\n\t\t_reply(" + genExtention.generateDummyArgumentList(method) + ");";
        }
        header += "\n\t}";
    }
    for (auto broadcast : fInterface->getBroadcasts())
    {
        header += "\n\t" + replace_all(FTypeGenerator::generateComments(broadcast, false), "\n", "\n\t");
        if (broadcast->isSelective())
        {
            header += "COMMONAPI_EXPORT virtual void " +
                      genExtention.getStubAdapterClassFireSelectiveMethodName(broadcast) + "(" +
                      genExtention.generateSendSelectiveSignatur(broadcast, fInterface, true) + ") {";
            std::list<std::string> argList;
            for (auto arg : broadcast->getOutArgs())
            {
                if (!arg->isArray() && genExtention.supportsValidation(arg->getType()))
                {
                    header +=
                        "\n\t\tif (!_" + genExtention.getElementName(arg) + ".validate()) {\n\t\t\treturn;\n\t\t}";
                }
                argList.emplace_back("_" + arg->getName());
            }
            header += "\n\t\t" + genExtention.getStubAdapterClassSendSelectiveMethodName(broadcast) + "(" +
                      join(argList, ", ") + ", _receivers);\n\t}";

            header += "\n\t// Hook method for reacting on new subscriptions or removed subscriptions respectively for "
                      "selective broadcasts.";
            header += "\n\tCOMMONAPI_EXPORT virtual void " + genExtention.getSubscriptionChangedMethodName(broadcast) +
                      "(const std::shared_ptr<CommonAPI::ClientId> _client, const "
                      "CommonAPI::SelectiveBroadcastSubscriptionEvent _event) {";
            header += "\n\t\t(void)_client;\n\t\t(void)_event;\n\t\t// No operation in default\n\t}";

            header += "\n\t// Hook method for reacting accepting or denying new subscriptions";
            header += "\n\tCOMMONAPI_EXPORT virtual bool " +
                      genExtention.getSubscriptionRequestedMethodName(broadcast) +
                      "(const std::shared_ptr<CommonAPI::ClientId> _client) {";
            header += "\n\t\t(void)_client;\n\t\t// Accept in default\n\t\treturn true;\n\t}";
        }
        else
        {
            if (!genExtention.isErrorType(broadcast, deploymentAccessor))
            {
                std::list<std::string> argList;
                for (auto it : broadcast->getOutArgs())
                {
                    argList.emplace_back("const " + genExtention.getTypeName(it, fInterface, true) + " &_" +
                                         it->getName());
                }
                header += "COMMONAPI_EXPORT virtual void " +
                          genExtention.getStubAdapterClassFireEventMethodName(broadcast) + "(" + join(argList, ", ") +
                          ") {";
                argList.clear();
                for (auto arg : broadcast->getOutArgs())
                {
                    if (!arg->isArray() && genExtention.supportsValidation(arg->getType()))
                    {
                        header += "\n\t\tif (!_" + arg->getName() + ".validate()) {\n\t\t\treturn;\n\t\t}";
                    }
                    argList.emplace_back("_" + arg->getName());
                }
                header += "\n\t\t" + genExtention.getStubClassName(fInterface) +
                          "::" + genExtention.getStubAdapterClassFireEventMethodName(broadcast) + "(" +
                          join(argList, ", ") + ");\n\t}";
            }
        }
    }
    for (auto managed : fInterface->getManagedInterfaces())
    {
        header += "\n\tCOMMONAPI_EXPORT bool " + genExtention.stubRegisterManagedAutoName(managed) +
                  "(std::shared_ptr< " + genExtention.getStubFullClassName(managed) + "> _stub) {";
        header += "\n\t\tautoInstanceCounter_++;";
        header += "\n\t\treturn " + genExtention.getStubClassName(fInterface) +
                  "::" + genExtention.stubRegisterManagedName(managed) + "(_stub, autoInstanceCounter_);\n\t}";

        header += "\n\tCOMMONAPI_EXPORT bool" + genExtention.stubRegisterManagedName(managed) + "(std::shared_ptr< " +
                  genExtention.getStubFullClassName(managed) + "> _stub, const std::string &_instance) {";
        header += "\n\t\treturn " + genExtention.getStubClassName(fInterface) +
                  "::" + genExtention.stubRegisterManagedName(managed) + "(_stub, _instance);\n\t}";

        header += "\n\tCOMMONAPI_EXPORT bool " + genExtention.stubDeregisterManagedName(managed) +
                  "(const std::string &_instance) {";
        header += "\n\t\treturn " + genExtention.getStubClassName(fInterface) +
                  "::" + genExtention.stubDeregisterManagedName(managed) + "(_instance);\n\t}";

        header +=
            "\n\tCOMMONAPI_EXPORT std::set<std::string>& " + genExtention.stubManagedSetGetterName(managed) + "() {";
        header += "\n\t\treturn " + genExtention.getStubClassName(fInterface) +
                  "::" + genExtention.stubManagedSetGetterName(managed) + "();\n\t}";
    }
    header += "\nprotected:";
    for (auto attribute : fInterface->getAttributes())
    {
        auto typeName = genExtention.getTypeName(attribute, fInterface, true);
        header += "\n\t" + replace_all(FTypeGenerator::generateComments(attribute, false), "\n", "\n\t");
        header += "COMMONAPI_EXPORT virtual bool " + getStubDefaultClassTrySetMethodName(attribute) + "(" + typeName +
                  " _value) {";
        header += "\n\t\tif (!" + getStubDefaultClassValidateMethodName(attribute) + "(_value))\n\t\t\treturn false;\n";
        header += "\n\t\tbool valueChanged;";
        header += "\n\t\tstd::shared_ptr<" + genExtention.getStubAdapterClassName(fInterface) +
                  "> stubAdapter = CommonAPI::Stub<" + genExtention.getStubAdapterClassName(fInterface) + ", " +
                  genExtention.getStubRemoteEventClassName(fInterface) + ">::stubAdapter_.lock();";
        header += "\n\t\tif(stubAdapter) {";
        header += "\n\t\t\tstubAdapter->" + genExtention.getStubClassLockMethodName(attribute) + "(true);";
        header += "\n\t\t\tvalueChanged = (" + getStubDefaultClassVariableName(attribute) + " != _value);";
        header += "\n\t\t\t" + getStubDefaultClassVariableName(attribute) + " = std::move(_value);";
        header += "\n\t\t\tstubAdapter->" + genExtention.getStubClassLockMethodName(attribute) + "(false);";
        header += "\n\t\t} else {";
        header += "\n\t\t\tvalueChanged = (" + getStubDefaultClassVariableName(attribute) + " != _value);";
        header += "\n\t\t\t" + getStubDefaultClassVariableName(attribute) +
                  " = std::move(_value);\n\t\t}\n\n\t\treturn valueChanged;\n\t}";

        header += "\n\tCOMMONAPI_EXPORT virtual bool " + getStubDefaultClassValidateMethodName(attribute) + "(const " +
                  typeName + " &_value) {";
        header += "\n\t\t(void)_value;";
        if (genExtention.supportsTypeValidation(attribute))
        {
            header += "\n\t\treturn " + genExtention.validateType(attribute, fInterface) + ";";
        }
        else
        {
            header += "\n\t\treturn true;";
        }
        header += "\n\t}";

        if (!attribute->isReadonly())
        {
            header += "\n\tCOMMONAPI_EXPORT virtual void " +
                      genExtention.getStubRemoteEventClassChangedMethodName(attribute) + "() {";
            header += "\n\t\t// No operation in default\n\t}";
        }
    }

    header += "\n\tclass COMMONAPI_EXPORT_CLASS_EXPLICIT RemoteEventHandler: public virtual " +
              genExtention.getStubRemoteEventClassName(fInterface);
    if (fInterface->getBase())
        header +=
            ", public virtual " + genExtention.getStubDefaultClassName(fInterface->getBase()) + "::RemoteEventHandler";
    header += " {";
    header += "\n\tpublic:";
    header += "\n\t\tCOMMONAPI_EXPORT RemoteEventHandler(" + genExtention.getStubDefaultClassName(fInterface) +
              " *_defaultStub)";
    header += "\n\t\t\t: " + genExtention.generateBaseRemoteHandlerConstructorsCalls(fInterface);
    header += "\n\t\t\t  defaultStub_(_defaultStub) {\n\t\t}\n";

    for (auto attribute : fInterface->getAttributes())
    {
        auto typeName = genExtention.getTypeName(attribute, fInterface, true);
        header += "\n\t\t" + replace_all(FTypeGenerator::generateComments(attribute, false), "\n", "\n\t\t");
        if (!attribute->isReadonly())
        {
            header += "COMMONAPI_EXPORT virtual void " +
                      genExtention.getStubRemoteEventClassChangedMethodName(attribute) + "() {";
            header += "\n\t\t\tassert(defaultStub_ !=NULL);\n\t\t\tdefaultStub_->" +
                      genExtention.getStubRemoteEventClassChangedMethodName(attribute) + "();\n\t\t}";

            header += "\n\t\tCOMMONAPI_EXPORT virtual bool " +
                      genExtention.getStubRemoteEventClassSetMethodName(attribute) + "(" + typeName + " _value) {";
            header += "\n\t\t\tassert(defaultStub_ !=NULL);\n\t\t\treturn defaultStub_->" +
                      getStubDefaultClassTrySetMethodName(attribute) + "(std::move(_value));\n\t\t}";

            header += "\n\t\tCOMMONAPI_EXPORT virtual bool " +
                      genExtention.getStubRemoteEventClassSetMethodName(attribute) +
                      "(const std::shared_ptr<CommonAPI::ClientId> _client, " + typeName + " _value) {";
            header += "\n\t\t\t(void)_client;\n\t\t\treturn " +
                      genExtention.getStubRemoteEventClassSetMethodName(attribute) + "(_value);\n\t\t}";
        }
    }
    header += "\n\tprivate:";
    header += "\n\t\t" + genExtention.getStubDefaultClassName(fInterface) + " *defaultStub_;";
    header += "\n\t};";
    header += "\nprotected:";
    header += "\n\t" + genExtention.getStubDefaultClassName(fInterface) + "::RemoteEventHandler remoteEventHandler_;\n";
    header += "\nprivate:";
    if (!fInterface->getManagedInterfaces().empty())
    {
        header += "\n\tuint32_t autoInstanceCounter_;";
    }
    for (auto attribute : fInterface->getAttributes())
    {
        header += "\n\t" + replace_all(FTypeGenerator::generateComments(attribute, false), "\n", "\n\t");
        header += genExtention.getTypeName(attribute, fInterface, true) + " " +
                  getStubDefaultClassVariableName(attribute) + " {};";
    }
    header += "\n\tCommonAPI::Version interfaceVersion_;\n};";

    header += "\n" + genExtention.generateNamespaceEndDeclaration(model);
    header += genExtention.generateVersionNamespaceEnd(fInterface);
    header += "\n";
    header += genExtention.generateMajorVersionNamespace(fInterface);
    header += "\n#endif //" + define_name;
    return header;
}

std::string FInterfaceStubGenerator::generateStubDefaultSource(
    const std::shared_ptr<BstIdl::FInterface> &fInterface,
    const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor)
{
    std::string header;
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    header = genExtention.generateCommonApiLicenseHeader();
    header += "\n" + getLicense();
    header += "\n#include <" + genExtention.getStubDefaultHeaderPath(fInterface) + ">";
    header += "\n#include <assert.h>";
    header += "\n" + genExtention.generateVersionNamespaceBegin(fInterface);
    auto &mgr = BstIdl::FModelManager::getInstance();
    auto model = std::dynamic_pointer_cast<BstIdl::FModel>(fInterface->getContainer());
    header += "\n" + genExtention.generateNamespaceBeginDeclaration(model);
    header += "\n" + genExtention.getStubDefaultClassName(fInterface) +
              "::" + genExtention.getStubDefaultClassName(fInterface) + "():";
    header += "\n\tremoteEventHandler_(this),";
    if (!fInterface->getManagedInterfaces().empty())
    {
        header += "\n\tautoInstanceCounter_(0),";
    }
    header += "\n\tinterfaceVersion_(" + genExtention.getElementName(fInterface) + "::getInterfaceVersion()) {\n}";
    header += "\nconst CommonAPI::Version& " + genExtention.getStubDefaultClassName(fInterface) +
              "::getInterfaceVersion(std::shared_ptr<CommonAPI::ClientId> _client) {";
    header += "\n\t(void)_client;\n\treturn interfaceVersion_;\n}";
    header += "\n" + genExtention.getStubRemoteEventClassName(fInterface) + "* " +
              genExtention.getStubDefaultClassName(fInterface) + "::initStubAdapter(const std::shared_ptr< " +
              genExtention.getStubAdapterClassName(fInterface) + "> &_adapter) {";
    if (fInterface->getBase() != nullptr)
    {
        header += "\n\t" + genExtention.getStubDefaultClassName(fInterface->getBase()) + "::initStubAdapter(_adapter);";
    }
    header += "\n\t" + genExtention.getStubCommonAPIClassName(fInterface) + "::stubAdapter_ = _adapter;";
    header += "\n\treturn &remoteEventHandler_;\n}";
    for (auto attribute : fInterface->getAttributes())
    {
        auto typeName = genExtention.getTypeName(attribute, fInterface, true);
        header += "\nconst " + typeName + "& " + genExtention.getStubDefaultClassName(fInterface) +
                  "::" + genExtention.getStubClassGetMethodName(attribute) + "() {";
        header += "\n\treturn " + getStubDefaultClassVariableName(attribute) + ";\n}";

        header += "\nconst " + typeName + "& " + genExtention.getStubDefaultClassName(fInterface) +
                  "::" + genExtention.getStubClassGetMethodName(attribute) +
                  "(const std::shared_ptr<CommonAPI::ClientId> _client) {";
        header += "\n\t(void)_client;\n\treturn " + genExtention.getStubClassGetMethodName(attribute) + "();\n}";
        header += "\nvoid " + genExtention.getStubDefaultClassName(fInterface) +
                  "::" + getStubDefaultClassSetMethodName(attribute) + "(" + typeName + " _value) {";
        if (genExtention.isObservable(attribute))
        {
            header += "\n\tstd::shared_ptr<" + genExtention.getStubAdapterClassName(fInterface) +
                      "> stubAdapter = CommonAPI::Stub<" + genExtention.getStubAdapterClassName(fInterface) + ",";
            header += "\n\t" + genExtention.getStubRemoteEventClassName(fInterface) + ">::stubAdapter_.lock();";
            header += "\n\tif(stubAdapter) {";
            header += "\n\t\tstubAdapter->" + genExtention.getStubClassLockMethodName(attribute) + "(true);";
            header += "\n\t\tconst bool valueChanged = (" + getStubDefaultClassVariableName(attribute) + " != _value);";
            header += "\n\t\tif (valueChanged) {";
            header += "\n\t\t\t" + getStubDefaultClassVariableName(attribute) + " = std::move(_value);";
            header += "\n\t\t\t" + genExtention.getStubAdapterClassFireChangedMethodName(attribute) + "(" +
                      getStubDefaultClassVariableName(attribute) + ");\n\t\t}";
            header +=
                "\n\t\tstubAdapter->" + genExtention.getStubClassLockMethodName(attribute) + "(false);\n\t} else {";
            header += "\n\t\tconst bool valueChanged = " + getStubDefaultClassTrySetMethodName(attribute) +
                      "(std::move(_value));";
            header += "\n\t\tif (valueChanged) {";
            header += "\n\t\t\t" + genExtention.getStubAdapterClassFireChangedMethodName(attribute) + "(" +
                      getStubDefaultClassVariableName(attribute) + ");\n\t\t}\n\t}";
        }
        else
        {
            header += "\n\t" + getStubDefaultClassTrySetMethodName(attribute) + "(std::move(_value));";
        }
        header += "\n";
        header += "\nbool " + genExtention.getStubDefaultClassName(fInterface) +
                  "::" + getStubDefaultClassTrySetMethodName(attribute) + "(" + typeName + " _value) {";
        header += "\n\tif (!" + getStubDefaultClassValidateMethodName(attribute) + "(_value))\n\t\treturn false;";
        header += "\n\tbool valueChanged;";
        header += "\n\tstd::shared_ptr<" + genExtention.getStubAdapterClassName(fInterface) +
                  "> stubAdapter = CommonAPI::Stub<" + genExtention.getStubAdapterClassName(fInterface) + ", " +
                  genExtention.getStubRemoteEventClassName(fInterface) + ">::stubAdapter_.lock();";
        header += "\n\tif(stubAdapter) {";
        header += "\n\t\tstubAdapter->" + genExtention.getStubClassLockMethodName(attribute) + "(true);";
        header += "\n\t\tvalueChanged = (" + getStubDefaultClassVariableName(attribute) + " != _value);";
        header += "\n\t\t" + getStubDefaultClassVariableName(attribute) + " = std::move(_value);";
        header += "\n\t\tstubAdapter->" + genExtention.getStubClassLockMethodName(attribute) + "(false);";

        header += "\n\t} else {";
        header += "\n\t\tvalueChanged = (" + getStubDefaultClassVariableName(attribute) + " != _value);";
        header += "\n\t\t" + getStubDefaultClassVariableName(attribute) +
                  " = std::move(_value);\n\t}\n\treturn valueChanged;\n}";

        header += "\nbool " + genExtention.getStubDefaultClassName(fInterface) +
                  "::" + getStubDefaultClassValidateMethodName(attribute) + "(const " + typeName + " &_value) {";
        header += "\n\t(void)_value;";
        if (genExtention.supportsTypeValidation(attribute))
        {
            header += "\n\treturn " + genExtention.validateType(attribute, fInterface);
        }
        else
        {
            header += "\n\treturn true;";
        }
        header += "\n}";
        if (attribute->isReadonly())
        {
            header += "\nvoid " + genExtention.getStubDefaultClassName(fInterface) +
                      "::" + getStubDefaultClassSetMethodName(attribute) +
                      "(const std::shared_ptr<CommonAPI::ClientId> _client, " + typeName + " _value) {";
            header += "\n\t" + getStubDefaultClassSetMethodName(attribute) + "(_value);\n}";

            header += "\n\nvoid " + genExtention.getStubDefaultClassName(fInterface) +
                      "::" + genExtention.getStubRemoteEventClassChangedMethodName(attribute) + "() {";
            header += "\n\t// No operation in default\n}";

            header += "\n\nvoid " + genExtention.getStubDefaultClassName(fInterface) +
                      "::RemoteEventHandler::" + genExtention.getStubRemoteEventClassChangedMethodName(attribute) +
                      "() {";
            header += "\n\tassert(defaultStub_ !=NULL);\n\tdefaultStub_->" +
                      genExtention.getStubRemoteEventClassChangedMethodName(attribute) + "();\n}";

            header += "\n\nbool " + genExtention.getStubDefaultClassName(fInterface) +
                      "::RemoteEventHandler::" + genExtention.getStubRemoteEventClassSetMethodName(attribute) + "(" +
                      typeName + " _value) {";
            header += "\n\tassert(defaultStub_ !=NULL);\n\tdefaultStub_->" +
                      genExtention.getStubRemoteEventClassSetMethodName(attribute) + "(std::move(_value));\n}";

            header += "\n\nbool " + genExtention.getStubDefaultClassName(fInterface) +
                      "::RemoteEventHandler::" + genExtention.getStubRemoteEventClassSetMethodName(attribute) +
                      "(const std::shared_ptr<CommonAPI::ClientId> _client, " + typeName + " _value) {";
            header += "\n\t(void)_client;\n\treturn " + genExtention.getStubRemoteEventClassSetMethodName(attribute) +
                      "(_value);\n}";
        }
    }
    for (auto method : fInterface->getMethods())
    {
        header += "\n" + FTypeGenerator::generateComments(method, false);
        std::map<std::string, bool> replies;
        if (m_methodrepliesMap.find(method) != m_methodrepliesMap.end())
        {
            replies = m_methodrepliesMap.find(method)->second;
        }
        header += "\nvoid " + genExtention.getStubDefaultClassName(fInterface) +
                  "::" + genExtention.getElementName(method) + "(" +
                  genExtention.generateOverloadedStubSignature(method, replies) + ") {";
        header += "\n\t(void)_client;";
        if (!method->getInArgs().empty())
        {
            for (auto it : method->getInArgs())
            {
                header += "\n\t(void)_" + it->getName() + ";";
            }
        }
        if (!method->isFireAndForget())
        {
            if (!replies.empty())
            {
                bool containValue = false;
                for (auto it : replies)
                {
                    if (!it.second)
                    {
                        header += "\n\t(void)_callId;";
                        containValue = true;
                        break;
                    }
                }
                if (containValue)
                    for (auto reply : replies)
                    {
                        auto methodName = reply.first;
                        auto isErrorReply = reply.second;
                        if (isErrorReply)
                        {
                            header += "\n\t(void)_" + methodName + "Reply;";
                        }
                    }
            }
            header += "\n\t" + genExtention.generateDummyArgumentDefinitions(method);
            for (auto arg : method->getOutArgs())
            {
                if (!arg->isArray() && genExtention.supportsValidation(arg->getType()))
                {
                    header += "\n\tif (!" + genExtention.getElementName(method) + ".validate()) {\n\t\treturn;\n\t}";
                }
            }
            header += "\n\t_reply(" + genExtention.generateDummyArgumentList(method) + ");";
        }
        header += "\n}";
    }
    for (auto broadcast : fInterface->getBroadcasts())
    {
        header += "\n" + FTypeGenerator::generateComments(broadcast, false);
        if (broadcast->isSelective())
        {
            header += "\nvoid " + genExtention.getStubDefaultClassName(fInterface) +
                      "::" + genExtention.getStubAdapterClassFireSelectiveMethodName(broadcast) + "(" +
                      genExtention.generateSendSelectiveSignatur(broadcast, fInterface, false) + ") {";
            for (auto arg : broadcast->getOutArgs())
            {
                if (!arg->isArray() && genExtention.supportsValidation(arg->getType()))
                {
                    header += "\n\tif (!_" + genExtention.getElementName(arg) + ".validate()) {\n\t\treturn;\n\t}";
                }
            }
            header += "\n\t" + genExtention.getStubAdapterClassSendSelectiveMethodName(broadcast) + "(";
            for (auto it : broadcast->getOutArgs())
            {
                header += "_" + genExtention.getElementName(it) + ",";
            }
            header += "_receivers);";

            header += "\nvoid " + genExtention.getStubDefaultClassName(fInterface) +
                      "::" + genExtention.getSubscriptionChangedMethodName(broadcast) +
                      "(const std::shared_ptr<CommonAPI::ClientId> _client, const "
                      "CommonAPI::SelectiveBroadcastSubscriptionEvent _event) {";
            header += "\n\t(void)_client;\n\t(void)_event;\n\t// No operation in default\n}";

            header += "\n\tbool " + genExtention.getStubDefaultClassName(fInterface) +
                      "::" + genExtention.getSubscriptionRequestedMethodName(broadcast) +
                      "(const std::shared_ptr<CommonAPI::ClientId> _client) {";
            header += "\n\t(void)_client;\n\t// Accept in default\n\treturn true;\n}";
        }
        else
        {
            if (!genExtention.isErrorType(broadcast, deploymentAccessor))
            {
                header += "\nvoid " + genExtention.getStubDefaultClassName(fInterface) +
                          "::" + genExtention.getStubAdapterClassFireEventMethodName(broadcast) + "(";
                for (auto it : broadcast->getOutArgs())
                {
                    if (it == broadcast->getOutArgs().front())
                        header += "const " + genExtention.getTypeName(it, fInterface, true) + " &_" +
                                  genExtention.getElementName(it);
                    else
                    {
                        header += ", const " + genExtention.getTypeName(it, fInterface, true) + " &_" +
                                  genExtention.getElementName(it);
                    }
                }
                header += ") {";
                for (auto arg : broadcast->getOutArgs())
                {
                    if (!arg->isArray() && genExtention.supportsValidation(arg->getType()))
                    {
                        header += "\n\tif (!_" + genExtention.getElementName(arg) + ".validate()) {\n\t\treturn;\n\t}";
                    }
                }
                header += "\n\t" + genExtention.getStubClassName(fInterface) +
                          "::" + genExtention.getStubAdapterClassFireEventMethodName(broadcast) + "(";
                for (auto arg : broadcast->getOutArgs())
                {
                    if (arg == broadcast->getOutArgs().front())
                    {
                        header += "_" + genExtention.getElementName(arg);
                    }
                    else
                    {
                        header += ", _" + genExtention.getElementName(arg);
                    }
                }
                header += ");\n}";
            }
        }
    }
    for (auto managed : fInterface->getManagedInterfaces())
    {
        header += "\nbool " + genExtention.getStubDefaultClassName(fInterface) +
                  "::" + genExtention.stubRegisterManagedAutoName(managed) + "(std::shared_ptr< " +
                  genExtention.getStubFullClassName(managed) + "> _stub) {";
        header += "\n\tautoInstanceCounter_++;";
        header += "\n\treturn " + genExtention.getStubClassName(fInterface) +
                  "::" + genExtention.stubRegisterManagedName(managed) + "(_stub, autoInstanceCounter_);\n}";

        header += "\nbool " + genExtention.getStubDefaultClassName(fInterface) +
                  "::" + genExtention.stubRegisterManagedMethodImpl(managed) + "{";
        header += "\n\treturn " + genExtention.getStubClassName(fInterface) +
                  "::" + genExtention.stubRegisterManagedName(managed) + "(_stub, _instance);\n}";

        header += "\nbool " + genExtention.getStubDefaultClassName(fInterface) +
                  "::" + genExtention.stubDeregisterManagedName(managed) + "(const std::string &_instance) {";
        header += "\n\treturn " + genExtention.getStubClassName(fInterface) +
                  "::" + genExtention.stubDeregisterManagedName(managed) + "(_instance);\n}";

        header += "\nstd::set<std::string>& " + genExtention.getStubDefaultClassName(fInterface) +
                  "::" + genExtention.stubManagedSetGetterName(managed) + "() {";
        header += "\n\treturn " + genExtention.getStubClassName(fInterface) +
                  "::" + genExtention.stubManagedSetGetterName(managed) + "();\n}";
    }
    header += "\n" + genExtention.getStubDefaultClassName(fInterface) + "::RemoteEventHandler::RemoteEventHandler(" +
              genExtention.getStubDefaultClassName(fInterface) + " *_defaultStub)";
    header += "\n\t: " + genExtention.generateBaseRemoteHandlerConstructorsCalls(fInterface);
    header += "  defaultStub_(_defaultStub) {\n}";
    header += "\n" + genExtention.generateNamespaceEndDeclaration(model);
    header += genExtention.generateVersionNamespaceEnd(fInterface);
    return header;
}

std::string FInterfaceStubGenerator::getStubAdapterAttributeMutexName(
    const std::shared_ptr<BstIdl::FAttribute> &fAttribute)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    return toFirstLower(gen.getElementName(fAttribute)) + "Mutex_";
}

std::string FInterfaceStubGenerator::getStubDefaultClassSetMethodName(
    const std::shared_ptr<BstIdl::FAttribute> &fAttribute)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    return "set" + toFirstUpper(gen.getElementName(fAttribute)) + "Attribute";
}

std::string FInterfaceStubGenerator::generateMethodReplyDeclarations(
    const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor,
    const std::shared_ptr<BstIdl::FMethod> &fMethod, const std::shared_ptr<BstIdl::FInterface> &fInterface,
    std::map<std::string, int> &counterMap,
    std::map<std::shared_ptr<BstIdl::FMethod>, std::map<std::string, bool>> &methodrepliesMap)
{
    std::string str;
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    std::map<std::string, bool> replies;
    if (methodrepliesMap.find(fMethod) != methodrepliesMap.end())
    {
        if (!methodrepliesMap[fMethod].empty())
            replies = methodrepliesMap[fMethod];
    }
    if (counterMap.find(gen.getElementName(fMethod)) == counterMap.end())
    {
        replies[gen.getElementName(fMethod)] = false;
        counterMap[gen.getElementName(fMethod)] = 0;
        str = "\n\ttypedef std::function<void (" + gen.generateStubReplySignature(fMethod) + ")>" +
              gen.getElementName(fMethod) + "Reply_t;";
    }
    else
    {
        counterMap[gen.getElementName(fMethod)] = counterMap.find(gen.getElementName(fMethod))->second + 1;
        auto reply = gen.getElementName(fMethod) + std::to_string(counterMap[gen.getElementName(fMethod)]);
        replies[reply] = false;
        str = "\n\ttypedef std::function<void (" + gen.generateStubReplySignature(fMethod) + ")>" + reply + "Reply_t;";
    }
    for (auto broadcast : fInterface->getBroadcasts())
    {
        if (gen.isErrorType(broadcast, fMethod, deploymentAccessor))
        {
            auto errorReply = gen.getElementName(fMethod) + toFirstUpper(gen.getElementName(broadcast));
            replies[errorReply] = true;
            str = "\n\ttypedef std::function<void (" +
                  gen.generateStubErrorReplySignature(broadcast, deploymentAccessor) + ")>" + errorReply + "Reply_t;";
        }
    }
    methodrepliesMap[fMethod] = replies;
    return str;
}

std::string FInterfaceStubGenerator::getStubDefaultClassTrySetMethodName(
    const std::shared_ptr<BstIdl::FAttribute> &fAttribute)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    return "trySet" + toFirstUpper(gen.getElementName(fAttribute)) + "Attribute";
}

std::string FInterfaceStubGenerator::getStubDefaultClassValidateMethodName(
    const std::shared_ptr<BstIdl::FAttribute> &fAttribute)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    return "validate" + toFirstUpper(gen.getElementName(fAttribute)) + "AttributeRequestedValue";
}

std::string FInterfaceStubGenerator::getStubDefaultClassVariableName(
    const std::shared_ptr<BstIdl::FAttribute> &fAttribute)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    return toFirstLower(gen.getElementName(fAttribute)) + "AttributeValue_";
}
} // namespace BstCommonAPI