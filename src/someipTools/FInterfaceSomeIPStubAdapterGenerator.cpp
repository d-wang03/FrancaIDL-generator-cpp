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
#include "someipTools/FInterfaceSomeIPStubAdapterGenerator.h"
#include "FDModel/FDeployedProvider.h"
#include "FDModel/ProviderUtils.h"
#include "capicxx-core-tools/FTypeGenerator.h"
#include "fmt/core.h"
#include "model/FTypeCollection.h"
#include "someipTools/FrancaSomeIPGeneratorExtensions.h"
#include "someipTools/SomeipPropertyAccessor.h"
#include "utilities/string_utility.h"
#include <fstream>
#include <memory>

namespace BstCommonAPI
{
void FInterfaceSomeIPStubAdapterGenerator::generateStubAdapter(
    const std::shared_ptr<BstIdl::FInterface> &_interface, const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
    std::list<std::shared_ptr<BstIdl::FDExtensionRoot>> &providers, bool generate_someip_code, std::string &dir)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    auto names = gen.getFullName(_interface);
    names = names.substr(0, names.find_last_of("::") + 1);
    if (generate_someip_code)
    {
        auto header_path = someipStubAdapterHeaderPath(_interface);
        auto h_path = mk_dir(dir, header_path);
        if (h_path.empty())
            return;
        auto pos = header_path.find_last_of("/") + 1;
        auto file_name = header_path.substr(pos, header_path.size() - pos);
        std::ofstream OsWrite(h_path + "/" + file_name, std::ofstream::out);
        auto header = generateStubAdapterHeader(_interface, _accessor);
        remove_all(header, names);
        OsWrite << replace_all(header, "\t", "    ");
        OsWrite.close();

        auto source_path = someipStubAdapterSourcePath(_interface);
        auto s_path = mk_dir(dir, source_path);
        if (s_path.empty())
            return;
        pos = source_path.find_last_of("/") + 1;
        auto s_file_name = source_path.substr(pos, source_path.size() - pos);
        std::ofstream OsWriteS(s_path + "/" + s_file_name, std::ofstream::out);
        auto source = generateStubAdapterSource(_interface, _accessor, providers);
        remove_all(source, names);
        OsWriteS << replace_all(source, "\t", "    ");
        OsWriteS.close();
    }
}

std::string FInterfaceSomeIPStubAdapterGenerator::dispatcherTableEntry(
    const std::shared_ptr<BstIdl::FInterface> &fInterface, const std::string &identifierAsHexString,
    const std::string &memberFunctionName)
{
    auto gen = FrancaSomeIPGeneratorExtensions::getInstance();
    std::string ret = someipStubAdapterHelperClassName(fInterface) + "::addStubDispatcher( { " + identifierAsHexString +
                      " }, &" + memberFunctionName + " );";

    return ret;
}

std::string FInterfaceSomeIPStubAdapterGenerator::generateStubAdapterHeader(
    const std::shared_ptr<BstIdl::FInterface> &_interface, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    std::string header;
    header += someIPGen.generateCommonApiSomeIPLicenseHeader();
    header += "\n" + getLicense();
    header += FTypeGenerator::generateComments(_interface, false);
    auto defineName = genExtention.getDefineName(_interface);
    transform(defineName.begin(), defineName.end(), defineName.begin(), ::toupper);
    header += "\n#ifndef " + defineName + "_SOMEIP_STUB_ADAPTER_HPP_";
    header += "\n#define " + defineName + "_SOMEIP_STUB_ADAPTER_HPP_\n";
    header += "\n#include <" + genExtention.getStubHeaderPath(_interface) + ">";
    if (_interface->getBase() != nullptr)
    {
        header += "\n#include <" + someipStubAdapterHeaderPath(_interface->getBase()) + ">";
    }
    auto DeploymentHeaders = someIPGen.getDeploymentInputIncludes(_interface, _accessor);
    DeploymentHeaders.sort();
    for (auto deploymentHeader : DeploymentHeaders)
    {
        header += "\n#include <" + deploymentHeader + ">";
    }
    header += "\n" + genExtention.startInternalCompilation();
    header += "\n#include <CommonAPI/SomeIP/AddressTranslator.hpp>";
    header += "\n#include <CommonAPI/SomeIP/StubAdapterHelper.hpp>";
    header += "\n#include <CommonAPI/SomeIP/StubAdapter.hpp>";
    header += "\n#include <CommonAPI/SomeIP/Factory.hpp>";
    header += "\n#include <CommonAPI/SomeIP/Types.hpp>";
    header += "\n#include <CommonAPI/SomeIP/Constants.hpp>";
    header += "\n" + genExtention.endInternalCompilation();
    header += genExtention.generateVersionNamespaceBegin(_interface);
    auto model = std::dynamic_pointer_cast<BstIdl::FModel>(_interface->getContainer());
    header += genExtention.generateNamespaceBeginDeclaration(model);
    header +=
        "\n\ntemplate <typename _Stub = " + genExtention.getStubFullClassName(_interface) + ", typename... _Stubs>";
    header += "\nclass " + someipStubAdapterClassNameInternal(_interface);
    header += "\n\t: public virtual " + genExtention.getStubAdapterClassName(_interface) + ",";
    if (_interface->getBase() == nullptr)
    {
        header += "\n\t  public virtual CommonAPI::SomeIP::StubAdapterHelper< _Stub, "
                  "_Stubs...>,";
        header += "\n\t  public std::enable_shared_from_this< " + someipStubAdapterClassNameInternal(_interface) +
                  "<_Stub, _Stubs...>>";
    }
    else
    {
        header += "\n\t  public " + genExtention.getTypeCollectionName(_interface->getBase(), _interface) +
                  "SomeIPStubAdapterInternal<_Stub, _Stubs...>";
    }
    header += "\n{\npublic:";
    header += "\n\ttypedef CommonAPI::SomeIP::StubAdapterHelper< _Stub, _Stubs...> " +
              someipStubAdapterHelperClassName(_interface) + ";\n";
    header += "\n\t~" + someipStubAdapterClassNameInternal(_interface) + "() {";
    header += "\n\t\tdeactivateManagedInstances();";
    header += "\n\t\t" + someipStubAdapterHelperClassName(_interface) + "::deinit();\n\t}\n";
    for (auto attribute : _interface->getAttributes())
    {
        if (genExtention.isObservable(attribute))
        {
            header += "\n\t" + replace_all(FTypeGenerator::generateComments(attribute, false), "\n", "\n\t");
            header += "void " + genExtention.getStubAdapterClassFireChangedMethodName(attribute) + "(const " +
                      genExtention.getTypeName(attribute, _interface, true) + " &_value);\n\t";
        }
    }
    for (auto broadcast : _interface->getBroadcasts())
    {
        header += "\n\t" + replace_all(FTypeGenerator::generateComments(broadcast, false), "\n", "\n\t");
        if (broadcast->isSelective())
        {
            header += "void " + genExtention.getStubAdapterClassFireSelectiveMethodName(broadcast) + "(" +
                      genExtention.generateFireSelectiveSignatur(broadcast, _interface) + ");\n\t";
            header += "void " + genExtention.getStubAdapterClassSendSelectiveMethodName(broadcast) + "(" +
                      genExtention.generateSendSelectiveSignatur(broadcast, _interface, true) + ");\n\t";
            header += "void " + genExtention.getSubscribeSelectiveMethodName(broadcast) +
                      "(const std::shared_ptr<CommonAPI::ClientId> _client, "
                      "bool "
                      "&_success);\n\t";
            header += "void " + genExtention.getUnsubscribeSelectiveMethodName(broadcast) +
                      "(const std::shared_ptr<CommonAPI::ClientId> _client);\n\t";
            header += "std::shared_ptr<CommonAPI::ClientIdList> const " +
                      genExtention.getStubAdapterClassSubscribersMethodName(broadcast) + "();\n\t";
        }
        else
        {
            if (!genExtention.isErrorType(broadcast, _accessor))
            {
                header += "\n\tvoid " + genExtention.getStubAdapterClassFireEventMethodName(broadcast) + "(";
                for (auto it : broadcast->getOutArgs())
                {
                    if (it == broadcast->getOutArgs().front())
                        header += "const " + genExtention.getTypeName(it, _interface, true) + " &_" +
                                  genExtention.getElementName(it);
                    else
                    {
                        header += ", const " + genExtention.getTypeName(it, _interface, true) + " &_" +
                                  genExtention.getElementName(it);
                    }
                }
                header += ");";
            }
        }
    }
    for (auto managed : _interface->getManagedInterfaces())
    {
        header += "\n\t" + genExtention.stubRegisterManagedMethod(managed) + ";";
        header += "\n\tbool " + genExtention.stubDeregisterManagedName(managed) + "(const std::string&);";
        header += "\n\tstd::set<std::string>& " + genExtention.stubManagedSetGetterName(managed) + "();";
    }
    if (_interface->getManagedInterfaces().empty())
    {
        header += "\n\tvoid deactivateManagedInstances() {}";
    }
    else
    {
        header += "\n\tvoid deactivateManagedInstances() {";
        header += "\n\t\tstd::set<std::string>::iterator iter;";
        header += "\n\t\tstd::set<std::string>::iterator iterNext;";
        for (auto managed : _interface->getManagedInterfaces())
        {
            header += "\n\t\titer = " + genExtention.stubManagedSetName(managed) + ".begin();";
            header += "\n\t\twhile (iter != " + genExtention.stubManagedSetName(managed) + ".end()) {";
            header += "\n\t\t\titerNext = std::next(iter);";
            header += "\n\t\t\tif (" + genExtention.stubDeregisterManagedName(managed) +
                      "(*iter)) {\n\t\t\t\titer = iterNext;\n\t\t\t}";
            header += "\n\t\t\telse {\n\t\t\t\titer++;\n\t\t\t}\n\t\t}";
        }
        header += "\n\t}";
    }
    header += "\n";
    if (_interface->getBase() != nullptr)
    {
        header += "\n\tvirtual void "
                  "init(std::shared_ptr<CommonAPI::SomeIP::StubAdapter> "
                  "instance) {";
        header += "\n\t\treturn " + someipStubAdapterHelperClassName(_interface) + "::init(instance);\n\t}";

        header += "\n\tvirtual void deinit() {";
        header += "\n\t\treturn " + someipStubAdapterHelperClassName(_interface) + "::deinit();\n\t}";

        header += "\n\tvirtual bool onInterfaceMessage(const "
                  "CommonAPI::SomeIP::Message "
                  "&_message) {";
        header +=
            "\n\t\treturn " + someipStubAdapterHelperClassName(_interface) + "::onInterfaceMessage(_message);\n\t}";
    }

    header += "\n\tCommonAPI::SomeIP::GetAttributeStubDispatcher<";
    header += "\n\t\t" + genExtention.getStubFullClassName(_interface) + ",";
    header += "\n\t\tCommonAPI::Version";
    header += "\n\t> get" + genExtention.getElementName(_interface) + "InterfaceVersionStubDispatcher;";
    header += "\n\t" + generateAttributeDispatcherDeclarations(_interface, _accessor);
    std::list<std::string> dispatcherDefinitionsList;
    for (auto attribute : _interface->getAttributes())
    {
        auto tmp =
            replace_all(generateAttributeDispatcherDefinitions(attribute, _interface, _accessor), "\n", "\n\t\t");
        dispatcherDefinitionsList.push_back(tmp);
    }
    std::map<std::string, int> counterMap;
    std::map<std::shared_ptr<BstIdl::FMethod>, int> methodNumberMap;
    header +=
        "\n\t" + generateMethodDispatcherDeclarations(_interface, _interface, counterMap, methodNumberMap, _accessor);
    counterMap.clear();
    for (auto method : _interface->getMethods())
    {
        auto tmp = replace_all(
            generateMethodDispatcherDefinitions(method, _interface, _interface, _accessor, counterMap, methodNumberMap),
            "\n", "\n\t\t");
        dispatcherDefinitionsList.emplace_back(tmp);
    }
    header += "\n\t" + someipStubAdapterClassNameInternal(_interface) + "(";
    header += "\n\t\tconst CommonAPI::SomeIP::Address &_address,";
    header += "\n\t\tconst std::shared_ptr<CommonAPI::SomeIP::ProxyConnection> "
              "&_connection,";
    header += "\n\t\tconst std::shared_ptr<CommonAPI::StubBase> &_stub):";
    header += "\n\t\tCommonAPI::SomeIP::StubAdapter(_address, _connection),";
    header += "\n\t\t" + someipStubAdapterHelperClassName(_interface) +
              "(\n\t\t\t_address,\n\t\t\t_connection, \n\t\t\t_stub),";

    if (_interface->getBase() != nullptr)
    {
        header += "\n\t\t" + genExtention.getTypeCollectionName(_interface->getBase(), _interface) +
                  "SomeIPStubAdapterInternal<_Stub, _Stubs...>(_address, "
                  "_connection, "
                  "_stub),";
    }
    header += "\n\t\tget" + genExtention.getElementName(_interface) + "InterfaceVersionStubDispatcher(&" +
              genExtention.getStubClassName(_interface) + "::lockInterfaceVersionAttribute, &" +
              genExtention.getStubClassName(_interface) + "::getInterfaceVersion, false, true)";
    if (!dispatcherDefinitionsList.empty())
        header += ",\n\t\t" + join(dispatcherDefinitionsList, ",\n\t\t");
    header += "\n\t{\n\t\t" + replace_all(generateAttributeDispatcherTableContent(_interface), "\n", "\n\t\t");
    header += "\n\t\t" + replace_all(generateMethodDispatcherTableContent(_interface, counterMap, methodNumberMap),
                                     "\n", "\n\t\t");
    header += generateStubAttributeTableInitializer(_interface, _accessor);
    for (auto it : _interface->getAttributes())
    {
        if (genExtention.isObservable(it))
        {
            header += "\n\t\tstd::shared_ptr<CommonAPI::SomeIP::ClientId> "
                      "itsClient = "
                      "std::make_shared<CommonAPI::SomeIP::ClientId>(0xFFFF, "
                      "0xFFFFFFFF, "
                      "0xFFFFFFFF);\n";
            break;
        }
    }
    header += "\n\t\t// Provided events/fields";
    for (auto broadcast : _interface->getBroadcasts())
    {
        if (!genExtention.isErrorType(broadcast, _accessor))
        {
            header += "\n\t\t{\n\t\t\tstd::set<CommonAPI::SomeIP::eventgroup_id_t> "
                      "itsEventGroups;";
            for (auto eventgroup : someIPGen.getEventGroups(broadcast, _accessor))
            {
                header += "\n\t\t\titsEventGroups.insert(CommonAPI::SomeIP::"
                          "eventgroup_id_"
                          "t(" +
                          eventgroup + "));";
            }
            if (broadcast->isSelective())
            {
                header += "\n\t\t\tCommonAPI::SomeIP::StubAdapter::registerEvent(" +
                          someIPGen.getEventIdentifier(broadcast, _accessor) +
                          ", itsEventGroups, "
                          "CommonAPI::SomeIP::event_type_e::ET_SELECTIVE_EVENT, " +
                          someIPGen.getReliabilityType(broadcast, _accessor) + ");";
            }
            else
            {
                header += "\n\t\t\tCommonAPI::SomeIP::StubAdapter::registerEvent(" +
                          someIPGen.getEventIdentifier(broadcast, _accessor) +
                          ", itsEventGroups, "
                          "CommonAPI::SomeIP::event_type_e::ET_EVENT, " +
                          someIPGen.getReliabilityType(broadcast, _accessor) + ");";
            }
            header += "\n\t\t}";
        }
    }
    for (auto attribute : _interface->getAttributes())
    {
        if (genExtention.isObservable(attribute))
        {
            header += "\n\t\tif (_stub->hasElement(" +
                      std::to_string(genExtention.getElementPosition(_interface, attribute)) + ")) {";
            header += "\n\t\t\tstd::set<CommonAPI::SomeIP::eventgroup_id_t> "
                      "itsEventGroups;";
            for (auto eventgroup : someIPGen.getNotifierEventGroups(attribute, _accessor))
            {
                header += "\n\t\t\titsEventGroups.insert(CommonAPI::SomeIP::"
                          "eventgroup_id_"
                          "t(" +
                          eventgroup + "));";
            }
            header += "\n\t\t\tCommonAPI::SomeIP::StubAdapter::registerEvent(" +
                      someIPGen.getNotifierIdentifier(attribute, _accessor) +
                      ", itsEventGroups, "
                      "CommonAPI::SomeIP::event_type_e::ET_FIELD, " +
                      someIPGen.getNotifierReliabilityType(attribute, _accessor) + ");";
            header += "\n\t\t\t" + genExtention.getStubAdapterClassFireChangedMethodName(attribute) +
                      "(std::dynamic_pointer_cast< " + genExtention.getStubFullClassName(_interface) + ">(_stub)->" +
                      someIPGen.getMethodName(attribute) + "(itsClient));\n\t\t}\n";
        }
    }
    header += "\n\t}\n";
    header += "\n\t// Register/Unregister event handlers for selective broadcasts";
    header += "\n\tvoid registerSelectiveEventHandlers();";
    header += "\n\tvoid unregisterSelectiveEventHandlers();";
    if (genExtention.hasSelectiveBroadcasts(_interface) || _interface->getManagedInterfaces().size() > 0)
    {
        header += "\nprivate:";
        for (auto broadcast : _interface->getBroadcasts())
        {
            if (broadcast->isSelective())
            {
                header += "\n\tstd::mutex " + genExtention.getClassName(broadcast) + "Mutex_;";
                header += "\n\tvoid " + genExtention.getClassName(broadcast) +
                          "Handler(CommonAPI::SomeIP::client_id_t _client, "
                          "CommonAPI::SomeIP::uid_t _uid, "
                          "CommonAPI::SomeIP::gid_t "
                          "_gid, bool _subscribe, const "
                          "CommonAPI::SomeIP::SubscriptionAcceptedHandler_t& "
                          "_acceptedHandler);";
            }
        }
        for (auto managed : _interface->getManagedInterfaces())
        {
            header += "\n\tstd::set<std::string> " + genExtention.stubManagedSetName(managed) + ";";
        }
    }
    header += "\n\n};";
    for (auto attribute : _interface->getAttributes())
    {
        if (genExtention.isObservable(attribute))
        {
            header += "\n" + FTypeGenerator::generateComments(attribute, false);
            header += "template <typename _Stub, typename... _Stubs>";
            header += "\nvoid " + someipStubAdapterClassNameInternal(_interface) +
                      "<_Stub, _Stubs...>::" + genExtention.getStubAdapterClassFireChangedMethodName(attribute) +
                      "(const " + genExtention.getTypeName(attribute, _interface, true) + " &_value) {";
            header += "\n\t" +
                      replace_all(generateFireChangedMethodBody(attribute, _interface, _accessor), "\n", "\n\t") +
                      "\n}\n";
        }
    }
    for (auto broadcast : _interface->getBroadcasts())
    {
        header += "\n" + FTypeGenerator::generateComments(broadcast, false);
        if (broadcast->isSelective())
        {
            header += "\ntemplate <typename _Stub, typename... _Stubs>";
            header += "\nvoid " + someipStubAdapterClassNameInternal(_interface) +
                      "<_Stub, _Stubs...>::" + genExtention.getStubAdapterClassFireSelectiveMethodName(broadcast) +
                      "(" + genExtention.generateFireSelectiveSignatur(broadcast, _interface) + ") {";
            header += "\n\tstd::shared_ptr<CommonAPI::SomeIP::ClientId> client = "
                      "std::dynamic_pointer_cast<CommonAPI::SomeIP::ClientId, "
                      "CommonAPI::ClientId>(_client);";
            for (auto arg : broadcast->getOutArgs())
            {
                auto deploymentType = someIPGen.getDeploymentType(arg, _interface, true);
                auto deployment = someIPGen.getDeploymentRef(arg, arg->isArray(), broadcast, _interface,
                                                             _accessor->getOverwriteAccessor(arg));
                if (deploymentType != "CommonAPI::EmptyDeployment" && !deploymentType.empty())
                {
                    header += "\n\tCommonAPI::Deployable< " + genExtention.getTypeName(arg, arg, true) + ", " +
                              deploymentType + "> deployed_" + arg->getName() + "(_" + arg->getName() + ", ";
                    if (!deployment.empty())
                    {
                        header += deployment;
                    }
                    else
                    {
                        header += "nullptr";
                    }
                    header += ");";
                }
            }
            header += "\n\tif (client) {";
            header += "\n\t\tCommonAPI::SomeIP::StubEventHelper<CommonAPI::SomeIP::"
                      "SerializableArguments< ";
            std::list<std::string> args;
            for (auto it : broadcast->getOutArgs())
            {
                args.emplace_back(getDeployedTypeName(it, _interface, _accessor->getOverwriteAccessor(it)));
            }
            header += join(args, "\n\t\t, ") + "\n\t\t>>";
            header += "\n\t\t "
                      "::sendEvent(\n\t\t\t";
            args.clear();
            args.emplace_back("client->getClientId()");
            args.emplace_back("*this");
            args.emplace_back(someIPGen.getEventIdentifier(broadcast, _accessor));
            args.emplace_back(someIPGen.getEndianess(broadcast, _accessor));
            for (auto it : broadcast->getOutArgs())
            {
                args.emplace_back(getDeployedElementName(it, _interface, _accessor->getOverwriteAccessor(it)));
            }
            header += join(args, ",\n\t\t\t") + "\n\t\t );\n\t}\n}\n";
            header += "\ntemplate <typename _Stub, typename... _Stubs>";
            header += "\nvoid " + someipStubAdapterClassNameInternal(_interface) +
                      "<_Stub, _Stubs...>::" + genExtention.getStubAdapterClassSendSelectiveMethodName(broadcast) +
                      "(" + genExtention.generateSendSelectiveSignatur(broadcast, _interface, false) + ") {";
            header += "\n\tstd::shared_ptr<CommonAPI::ClientIdList> "
                      "actualReceiverList;";
            header += "\n\tactualReceiverList = _receivers;\n";
            header += "\n\tif(_receivers == NULL) {";
            header +=
                "\n\t\tstd::lock_guard < std::mutex > itsLock(" + genExtention.getClassName(broadcast) + "Mutex_);";
            header +=
                "\n\t\tif (" + genExtention.getStubAdapterClassSubscriberListPropertyName(broadcast) + " != NULL)";
            header += "\n\t\t\tactualReceiverList = "
                      "std::make_shared<CommonAPI::ClientIdList>(*" +
                      genExtention.getStubAdapterClassSubscriberListPropertyName(broadcast) + ");";
            header += "\n\t}\n\n\t"
                      "if(actualReceiverList == NULL)\n\t\treturn;\n";
            header += "\n\tfor (auto clientIdIterator = "
                      "actualReceiverList->cbegin();";
            header += "\n\t\t\tclientIdIterator != actualReceiverList->cend();";
            header += "\n\t\t\tclientIdIterator++) {";
            header += "\n\t\tbool found(false);\n\t\t{";
            header +=
                "\n\t\t\tstd::lock_guard < std::mutex > itsLock(" + genExtention.getClassName(broadcast) + "Mutex_);";
            header += "\n\t\t\tfound = (" + genExtention.getStubAdapterClassSubscriberListPropertyName(broadcast) +
                      "->find(*clientIdIterator) != " +
                      genExtention.getStubAdapterClassSubscriberListPropertyName(broadcast) + "->end());\n\t\t}";
            header += "\n\t\tif(_receivers == NULL || found) {";
            header +=
                "\n\t\t\t" + genExtention.getStubAdapterClassFireSelectiveMethodName(broadcast) + "(*clientIdIterator";
            for (auto it : broadcast->getOutArgs())
            {
                header += ", _" + genExtention.getElementName(it);
            }
            header += ");\n\t\t}\n\t}\n}\n";
            header += "\ntemplate <typename _Stub, typename... _Stubs>";
            header += "\nvoid " + someipStubAdapterClassNameInternal(_interface) +
                      "<_Stub, _Stubs...>::" + genExtention.getSubscribeSelectiveMethodName(broadcast) +
                      "(const std::shared_ptr<CommonAPI::ClientId> _client, bool "
                      "&_success) {";
            header += "\n\tbool ok = " + someipStubAdapterHelperClassName(_interface) + "::stub_->" +
                      genExtention.getSubscriptionRequestedMethodName(broadcast) + "(_client);";
            header += "\n\tif (ok) {";
            header += "\n\t\t{\n\t\t\tstd::lock_guard<std::mutex> itsLock(" + genExtention.getClassName(broadcast) +
                      "Mutex_);";
            header += "\n\t\t\t" + genExtention.getStubAdapterClassSubscriberListPropertyName(broadcast) +
                      "->insert(_client);\n\t\t}\n\t\t_success = true;";
            header += "\n\t} else {\n\t\t_success = false;\n\t}\n}\n";

            header += "\ntemplate <typename _Stub, typename... _Stubs>";
            header += "\nvoid " + someipStubAdapterClassNameInternal(_interface) +
                      "<_Stub, _Stubs...>::" + genExtention.getUnsubscribeSelectiveMethodName(broadcast) +
                      "(const std::shared_ptr<CommonAPI::ClientId> _client) {";
            header +=
                "\n\t{\n\t\tstd::lock_guard<std::mutex> itsLock(" + genExtention.getClassName(broadcast) + "Mutex_);";
            header += "\n\t\t" + genExtention.getStubAdapterClassSubscriberListPropertyName(broadcast) +
                      "->erase(_client);\n\t}\n}\n";

            header += "\ntemplate <typename _Stub, typename... _Stubs>";
            header +=
                "\nstd::shared_ptr<CommonAPI::ClientIdList> const " + someipStubAdapterClassNameInternal(_interface) +
                "<_Stub, _Stubs...>::" + genExtention.getStubAdapterClassSubscribersMethodName(broadcast) + "() {";
            header += "\n\tstd::lock_guard<std::mutex> itsLock(" + genExtention.getClassName(broadcast) + "Mutex_);";
            header += "\n\treturn std::make_shared<CommonAPI::ClientIdList>(*" +
                      genExtention.getStubAdapterClassSubscriberListPropertyName(broadcast) + ");\n}\n";

            header += "\ntemplate <typename _Stub, typename... _Stubs>";
            header += "\nvoid " + someipStubAdapterClassNameInternal(_interface) +
                      "<_Stub, _Stubs...>::" + genExtention.getClassName(broadcast) +
                      "Handler(CommonAPI::SomeIP::client_id_t _client, "
                      "CommonAPI::SomeIP::uid_t _uid, "
                      "CommonAPI::SomeIP::gid_t _gid, bool "
                      "_subscribe, const "
                      "CommonAPI::SomeIP::SubscriptionAcceptedHandler_t& "
                      "_acceptedHandler) {";
            header += "\n\tstd::shared_ptr<CommonAPI::SomeIP::ClientId> clientId = "
                      "std::make_shared<CommonAPI::SomeIP::ClientId>(CommonAPI::"
                      "SomeIP::"
                      "ClientId(_client, _uid, _gid));";
            header += "\n\tbool result = true;\n\tif (_subscribe) {";
            header += "\n\t\t" + genExtention.getSubscribeSelectiveMethodName(broadcast) + "(clientId, result);";
            header += "\n\t\tif (result) {\n\t\t\t_acceptedHandler(true);";
            header += "\n\t\t\t" + someipStubAdapterHelperClassName(_interface) + "::stub_->" +
                      genExtention.getSubscriptionChangedMethodName(broadcast) +
                      "(clientId, "
                      "CommonAPI::SelectiveBroadcastSubscriptionEvent::"
                      "SUBSCRIBED);";
            header += "\n\t\t} else "
                      "{\n\t\t\t_acceptedHandler(false);\n\t\t}\n\t} else {";
            header += "\n\t\t" + genExtention.getUnsubscribeSelectiveMethodName(broadcast) + "(clientId);";
            header += "\n\t\t" + someipStubAdapterHelperClassName(_interface) + "::stub_->" +
                      genExtention.getSubscriptionChangedMethodName(broadcast) +
                      "(clientId, "
                      "CommonAPI::SelectiveBroadcastSubscriptionEvent::"
                      "UNSUBSCRIBED);";
            header += "\n\t\t_acceptedHandler(true);\n\t}\n}";
        }
        else
        {
            if (!genExtention.isErrorType(broadcast, _accessor))
            {
                header += "\ntemplate <typename _Stub, typename... _Stubs>";
                header += "\nvoid " + someipStubAdapterClassNameInternal(_interface) +
                          "<_Stub, _Stubs...>::" + genExtention.getStubAdapterClassFireEventMethodName(broadcast) + "(";
                for (auto it : broadcast->getOutArgs())
                {
                    if (it == broadcast->getOutArgs().front())
                        header += "const " + genExtention.getTypeName(it, _interface, true) + " &_" +
                                  genExtention.getElementName(it);
                    else
                    {
                        header += ", const " + genExtention.getTypeName(it, _interface, true) + " &_" +
                                  genExtention.getElementName(it);
                    }
                }
                header += ") {";
                for (auto arg : broadcast->getOutArgs())
                {
                    auto deploymentType = someIPGen.getDeploymentType(arg, _interface, true);
                    auto deployment = someIPGen.getDeploymentRef(arg, arg->isArray(), broadcast, _interface,
                                                                 _accessor->getOverwriteAccessor(arg));
                    if (deploymentType != "CommonAPI::EmptyDeployment" && !deploymentType.empty())
                    {
                        header += "\n\tCommonAPI::Deployable< " + genExtention.getTypeName(arg, arg, true) + ", " +
                                  deploymentType + "> deployed_" + arg->getName() + "(_" + arg->getName() + ", ";
                        if (!deployment.empty())
                        {
                            header += deployment;
                        }
                        else
                        {
                            header += "nullptr";
                        }
                        header += ");";
                    }
                }
                header += "\n\tCommonAPI::SomeIP::StubEventHelper<CommonAPI::"
                          "SomeIP::"
                          "SerializableArguments< ";
                for (auto it : broadcast->getOutArgs())
                {
                    if (it == broadcast->getOutArgs().front())
                    {
                        header += getDeployedTypeName(it, _interface, _accessor->getOverwriteAccessor(it));
                    }
                    else
                        header += ", " + getDeployedTypeName(it, _interface, _accessor->getOverwriteAccessor(it));
                }
                header += "\n\t>>";
                header += "\n\t\t::sendEvent("
                          "\n\t\t*this,";
                header += "\n\t\t" + someIPGen.getEventIdentifier(broadcast, _accessor) + ",";
                header += "\n\t\t" + someIPGen.getEndianess(broadcast, _accessor);
                if (broadcast->getOutArgs().size() > 0)
                {
                    header += ",";
                }
                for (auto it : broadcast->getOutArgs())
                {
                    if (it == broadcast->getOutArgs().front())
                    {
                        header +=
                            "\n\t\t" + getDeployedElementName(it, _interface, _accessor->getOverwriteAccessor(it));
                    }
                    else
                        header += "," + getDeployedElementName(it, _interface, _accessor->getOverwriteAccessor(it));
                }
                header += "\n\t);\n}";
            }
        }
    }
    header += "\ntemplate <typename _Stub, typename... _Stubs>";
    header += "\nvoid " + someipStubAdapterClassNameInternal(_interface) +
              "<_Stub, _Stubs...>::registerSelectiveEventHandlers() {";
    for (auto broadcast : _interface->getBroadcasts())
    {
        if (broadcast->isSelective())
        {
            header += "\n\t" + genExtention.getStubAdapterClassSubscriberListPropertyName(broadcast) +
                      " = std::make_shared<CommonAPI::ClientIdList>();";
            header += "\n\tCommonAPI::SomeIP::AsyncSubscriptionHandler_t " + genExtention.getClassName(broadcast) +
                      "SubscribeHandler =";
            header += "\n\t\tstd::bind(&" + someipStubAdapterClassNameInternal(_interface) +
                      "::" + genExtention.getClassName(broadcast) + "Handler,";
            header += "\n\t\tstd::dynamic_pointer_cast<" + someipStubAdapterClassNameInternal(_interface) +
                      ">(this->shared_from_this()),";
            header += "\n\t\tstd::placeholders::_1, std::placeholders::_2, "
                      "std::placeholders::_3, std::placeholders::_4, "
                      "std::placeholders::_5);";
            header += "\n\tCommonAPI::SomeIP::StubAdapter::connection_->"
                      "registerSubscriptionHandler(CommonAPI::SomeIP::StubAdapter::"
                      "getSomeIpAddress(), " +
                      someIPGen.getEventGroups(broadcast, _accessor).front() + ", " +
                      genExtention.getClassName(broadcast) + "SubscribeHandler);\n";
        }
    }
    header += "\n}\n";

    header += "\ntemplate <typename _Stub, typename... _Stubs>";
    header += "\nvoid " + someipStubAdapterClassNameInternal(_interface) +
              "<_Stub, _Stubs...>::unregisterSelectiveEventHandlers() {";
    for (auto broadcast : _interface->getBroadcasts())
    {
        if (broadcast->isSelective())
        {
            header += "\n\tCommonAPI::SomeIP::StubAdapter::connection_->"
                      "unregisterSubscriptionHandler(CommonAPI::SomeIP::"
                      "StubAdapter::"
                      "getSomeIpAddress(), " +
                      someIPGen.getEventGroups(broadcast, _accessor).front() + ");";
        }
    }
    header += "\n}\n";
    for (auto managed : _interface->getManagedInterfaces())
    {
        header += "\ntemplate <typename _Stub, typename... _Stubs>";
        header += "\nbool " + someipStubAdapterClassNameInternal(_interface) +
                  "<_Stub, _Stubs...>::" + genExtention.stubRegisterManagedMethodImpl(managed) + " {";
        header += "\n\tif (" + genExtention.stubManagedSetName(managed) +
                  ".find(_instance) == " + genExtention.stubManagedSetName(managed) + ".end()) {";
        header +=
            "\n\t\tstd::string commonApiAddress = \"local:" + genExtention.getFullyQualifiedNameWithVersion(managed) +
            ":\" + _instance;";
        header += "\n\t\tCommonAPI::SomeIP::Address itsSomeIpAddress;";
        header += "\n\t\tCommonAPI::SomeIP::AddressTranslator::get()->translate("
                  "commonApiAddress, itsSomeIpAddress);";
        header += "\n\t\tstd::shared_ptr<CommonAPI::SomeIP::Factory> itsFactory = "
                  "CommonAPI::SomeIP::Factory::get();";
        header += "\n\t\tauto stubAdapter = itsFactory->createStubAdapter(_stub, \"" +
                  genExtention.getFullyQualifiedNameWithVersion(managed) +
                  "\", itsSomeIpAddress, "
                  "CommonAPI::SomeIP::StubAdapter::connection_);";
        header += "\n\t\tif(itsFactory->registerManagedService(stubAdapter)) {";
        header += "\n\t\t\t" + genExtention.stubManagedSetName(managed) + ".insert(_instance);";
        header += "\n\t\t\treturn true;\n\t\t}\n\t}\n\treturn false;\n}";

        header += "\ntemplate <typename _Stub, typename... _Stubs>";
        header += "\nbool " + someipStubAdapterClassNameInternal(_interface) +
                  "<_Stub, _Stubs...>::" + genExtention.stubDeregisterManagedName(managed) +
                  "(const std::string &_instance) {";
        header += "\n\tstd::string itsAddress = \"local:" + genExtention.getFullyQualifiedNameWithVersion(managed) +
                  ":\" + _instance;";
        header += "\n\tif (" + genExtention.stubManagedSetName(managed) +
                  ".find(_instance) != " + genExtention.stubManagedSetName(managed) + ".end()) {";
        header += "\n\t\tstd::shared_ptr<CommonAPI::SomeIP::Factory> itsFactory = "
                  "CommonAPI::SomeIP::Factory::get();";
        header += "\n\t\tif (itsFactory->isRegisteredService(itsAddress)) {";
        header += "\n\t\t\titsFactory->unregisterManagedService(itsAddress);";
        header += "\n\t\t\t" + genExtention.stubManagedSetName(managed) + ".erase(_instance);";
        header += "\n\t\t\treturn true;\n\t\t}\n\t}\n\treturn false;\n}";

        header += "\ntemplate <typename _Stub, typename... _Stubs>";
        header += "\nstd::set<std::string> &" + someipStubAdapterClassNameInternal(_interface) +
                  "<_Stub, _Stubs...>::" + genExtention.stubManagedSetGetterName(managed) + "() {";
        header += "\n\treturn " + genExtention.stubManagedSetName(managed) + ";\n}";
    }
    header += "\ntemplate <typename _Stub = " + genExtention.getStubFullClassName(_interface) + ", typename... _Stubs>";
    header += "\nclass " + someipStubAdapterClassName(_interface);
    header += "\n\t: public " + someipStubAdapterClassNameInternal(_interface) + "<_Stub, _Stubs...> {";
    header += "\npublic:\n\t" + someipStubAdapterClassName(_interface) + "(const CommonAPI::SomeIP::Address &_address,";
    header += "\n\t\t\t\t\t\t\t\t\t\t\tconst "
              "std::shared_ptr<CommonAPI::SomeIP::ProxyConnection> &_connection,";
    header += "\n\t\t\t\t\t\t\t\t\t\t\tconst std::shared_ptr<CommonAPI::StubBase> "
              "&_stub)";
    header += "\n\t\t: CommonAPI::SomeIP::StubAdapter(_address, _connection),";
    header += "\n\t\t  CommonAPI::SomeIP::StubAdapterHelper< _Stub, _Stubs...>(_address, _connection, _stub),";
    header += "\n\t\t  " + someipStubAdapterClassNameInternal(_interface) +
              "<_Stub, _Stubs...>(_address, _connection, _stub) {\n\t}\n};";
    header += "\n" + genExtention.generateNamespaceEndDeclaration(model);
    header += genExtention.generateVersionNamespaceEnd(_interface);
    header += "\n#endif // " + defineName + "_SOMEIP_STUB_ADAPTER_HPP_";
    return header;
}

std::string FInterfaceSomeIPStubAdapterGenerator::generateAttributeDispatcherDeclarations(
    const std::shared_ptr<BstIdl::FInterface> &_interface, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::string str;
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    for (auto a : _interface->getAttributes())
    {
        auto typeName = genExtention.getTypeName(a, _interface, true);
        str += "\n\t" + replace_all(FTypeGenerator::generateComments(a, false), "\n", "\n\t");
        auto deploymentType = someIPGen.getDeploymentType(a, _interface, true);
        auto getIdentifier = someIPGen.getGetterIdentifier(a, _accessor);
        std::list<std::string> args;
        args.emplace_back(genExtention.getStubFullClassName(_interface));
        args.emplace_back(typeName);
        if (deploymentType != "CommonAPI::EmptyDeployment" && !deploymentType.empty())
        {
            args.emplace_back(deploymentType);
        }
        if (getIdentifier != "0x0")
        {
            str += "CommonAPI::SomeIP::GetAttributeStubDispatcher<\n\t\t";
            str += join(args, ",\n\t\t") + "\n\t> " + someipGetStubDispatcherVariable(a) + ";\n\t";
        }
        if (!a->isReadonly())
        {
            str += "\n\tCommonAPI::SomeIP::Set";
            if (genExtention.isObservable(a))
            {
                str += "Observable";
            }
            str += "AttributeStubDispatcher<\n\t\t";
            str += join(args, ",\n\t\t") + "\n\t> " + someipSetStubDispatcherVariable(a) + ";\n\t";
        }
    }
    return str;
}

std::string FInterfaceSomeIPStubAdapterGenerator::generateMethodDispatcherDeclarations(
    const std::shared_ptr<BstIdl::FInterface> &_interface, const std::shared_ptr<BstIdl::FInterface> &_container,
    std::map<std::string, int> &_counters, std::map<std::shared_ptr<BstIdl::FMethod>, int> &_methods,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::string str;
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    auto accessor = someIPGen.getSomeIpAccessor(_interface);
    for (auto method : _interface->getMethods())
    {
        str += replace_all(FTypeGenerator::generateComments(method, false), "\n", "\n\t");
        if (!method->isFireAndForget())
        {
            std::list<std::string> errorReplyTypes;
            for (auto broadcast : _interface->getBroadcasts())
            {
                if (genExtention.isErrorType(broadcast, _accessor))
                {
                    errorReplyTypes.push_back(genExtention.errorReplyTypes(broadcast, method, _accessor));
                    str += generateErrorReplyCallback(broadcast, _interface, method, _accessor);
                }
            }
            str += "CommonAPI::SomeIP::MethodWithReplyStubDispatcher<";
            str += "\n\t\t" + genExtention.getStubFullClassName(_interface) + ",";
            str += "\n\t\tstd::tuple< " + getAllInTypes(method) + ">,";
            str += "\n\t\tstd::tuple< " + getAllOutTypes(method) + ">,";
            str +=
                "\n\t\tstd::tuple< " + someIPGen.getDeploymentTypes(method->getInArgs(), _interface, accessor) + ">,";
            str += "\n\t\tstd::tuple< " + someIPGen.getErrorDeploymentType(method, true);
            str += someIPGen.getDeploymentTypes(method->getOutArgs(), _interface, accessor) + ">";

            for (auto it : errorReplyTypes)
            {
                str += "\n, std::function< void (" + it + ")>";
            }

            if (!(_counters.count(someipStubDispatcherVariable(method))))
            {
                _counters[someipStubDispatcherVariable(method)] = 0;
                _methods[method] = 0;
                str += "\n\t> " + someipStubDispatcherVariable(method) + ";";
            }
            else
            {
                _counters[someipStubDispatcherVariable(method)] = _counters[someipStubDispatcherVariable(method)] + 1;
                _methods[method] = _counters[someipStubDispatcherVariable(method)];
                str += "\n\t> " + someipStubDispatcherVariable(method) +
                       std::to_string(_counters[someipStubDispatcherVariable(method)]) + ";";
            }
        }
        else
        {
            // delete static for the class MethodStubDispatcher's Assignment operator has been implicit deleted (It has
            // two non-static const member.).
            str += "CommonAPI::SomeIP::MethodStubDispatcher<";
            str += "\n\t\t" + genExtention.getStubFullClassName(_interface) + ",";
            str += "\n\t\tstd::tuple< " + getAllInTypes(method) + ">,";
            str += "\n\t\tstd::tuple< " + someIPGen.getDeploymentTypes(method->getInArgs(), _interface, accessor) + ">";
            if (!_counters.count(someipStubDispatcherVariable(method)))
            {
                _counters[someipStubDispatcherVariable(method)] = 0;
                _methods[method] = 0;
                str += "\n\t> " + someipStubDispatcherVariable(method) + ";";
            }
            else
            {
                _counters[someipStubDispatcherVariable(method)] = _counters[someipStubDispatcherVariable(method)] + 1;
                _methods[method] = _counters[someipStubDispatcherVariable(method)];
                str += "\n\t> " + someipStubDispatcherVariable(method) +
                       std::to_string(_counters[someipStubDispatcherVariable(method)]) + ";";
            }
        }
        str += "\n\t\n\t";
    }
    return str;
}

std::string FInterfaceSomeIPStubAdapterGenerator::recursivelyGenerateMethodDispatcherDeclarations(
    const std::shared_ptr<BstIdl::FInterface> &_interface, const std::shared_ptr<BstIdl::FInterface> &_container,
    std::map<std::string, int> &_counters, std::map<std::shared_ptr<BstIdl::FMethod>, int> &_methods,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto str = generateMethodDispatcherDeclarations(_interface, _container, _counters, _methods, _accessor);
    if (_interface->getBase() != nullptr)
        str += recursivelyGenerateMethodDispatcherDeclarations(_interface->getBase(), _container, _counters, _methods,
                                                               _accessor);
    return str;
}

std::string FInterfaceSomeIPStubAdapterGenerator::generateAttributeDispatcherDefinitions(
    const std::shared_ptr<BstIdl::FAttribute> &_attribute, const std::shared_ptr<BstIdl::FInterface> &_interface,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::string str;
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    auto typeName = genExtention.getTypeName(_attribute, _interface, true);
    auto deploymentType = someIPGen.getDeploymentType(_attribute, _interface, true);
    auto getIdentifier = someIPGen.getGetterIdentifier(_attribute, _accessor);
    if (getIdentifier != "0x0")
    {
        str += someipGetStubDispatcherVariable(_attribute) + "(\n\t";
        str += "&" + genExtention.getStubFullClassName(_interface) +
               "::" + genExtention.getStubClassLockMethodName(_attribute) + ",\n\t";
        str += "&" + genExtention.getStubFullClassName(_interface) +
               "::" + genExtention.getStubClassGetMethodName(_attribute) + ",\n\t" +
               someIPGen.getEndianess(_attribute, _accessor) + ",\n\t" + "_stub->hasElement(" +
               std::to_string(genExtention.getElementPosition(_interface, _attribute)) + ")";
        if (someIPGen.hasDeployment(_accessor->getOverwriteAccessor(_attribute), _attribute))
        {
            str += ",\n\t" + someIPGen.getDeploymentRef(_attribute, _attribute->isArray(), nullptr, _interface,
                                                        _accessor->getOverwriteAccessor(_attribute));
        }
        str += ")";
        if (!_attribute->isReadonly())
            str += ",";
        str += "\n";
    }
    if (!_attribute->isReadonly())
    {
        str += someipSetStubDispatcherVariable(_attribute) + "(";
        str += "\n\t&" + genExtention.getStubFullClassName(_interface) +
               "::" + genExtention.getStubClassLockMethodName(_attribute) + ",";
        str += "\n\t&" + genExtention.getStubClassName(_interface) +
               "::" + genExtention.getStubClassGetMethodName(_attribute) + ",";
        str += "\n\t&" + genExtention.getStubRemoteEventClassName(_interface) +
               "::" + genExtention.getStubRemoteEventClassSetMethodName(_attribute) + ",";
        str += "\n\t&" + genExtention.getStubRemoteEventClassName(_interface) +
               "::" + genExtention.getStubRemoteEventClassChangedMethodName(_attribute) + ",";
        if (genExtention.isObservable(_attribute))
        {
            str += "\n\t&" + genExtention.getStubAdapterClassName(_interface) +
                   "::" + genExtention.getStubAdapterClassFireChangedMethodName(_attribute) + ",";
        }
        str += "\n\t" + someIPGen.getEndianess(_attribute, _accessor) + "," + "\n\t_stub->hasElement(" +
               std::to_string(genExtention.getElementPosition(_interface, _attribute)) + ")";
        if (someIPGen.hasDeployment(_accessor->getOverwriteAccessor(_attribute), _attribute))
        {
            str += ",\n\t" + someIPGen.getDeploymentRef(_attribute, _attribute->isArray(), nullptr, _interface,
                                                        _accessor->getOverwriteAccessor(_attribute));
        }
        str += ")";
    }
    return str;
}

std::string FInterfaceSomeIPStubAdapterGenerator::generateMethodDispatcherDefinitions(
    const std::shared_ptr<BstIdl::FMethod> &_method, const std::shared_ptr<BstIdl::FInterface> &_interface,
    const std::shared_ptr<BstIdl::FInterface> &_thisInterface, const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
    std::map<std::string, int> &counterMap, std::map<std::shared_ptr<BstIdl::FMethod>, int> &methodnumberMap)
{
    std::string str;
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    if (!_method->isFireAndForget())
    {
        std::list<std::string> errorReplyTypes;
        std::list<std::string> errorReplyCallbacks;
        for (auto broadcast : _interface->getBroadcasts())
        {
            if (genExtention.isErrorType(broadcast, _method, _accessor))
            {
                errorReplyTypes.push_back(genExtention.errorReplyTypes(broadcast, _method, _accessor));
                errorReplyCallbacks.push_back(
                    "std::bind(&" + someipStubAdapterClassNameInternal(_interface) +
                    "<_Stub, _Stubs...>::" + genExtention.errorReplyCallbackName(broadcast, _accessor) + ", this, " +
                    genExtention.errorReplyCallbackBindArgs(broadcast, _accessor) + ")");
            }
        }
        if (!(counterMap.count(someipStubDispatcherVariable(_method))))
        {
            counterMap[someipStubDispatcherVariable(_method)] = 0;
            methodnumberMap[_method] = 0;
            str += someipStubDispatcherVariable(_method) + "(";
            str +=
                "\n\t&" + genExtention.getStubClassName(_interface) + "::" + genExtention.getElementName(_method) + ",";
            str += "\n\t" + someIPGen.isLittleEndian(_method, _accessor) + ",";
            str +=
                "\n\t_stub->hasElement(" + std::to_string(genExtention.getElementPosition(_interface, _method)) + "),";
            str += "\n\t" + someIPGen.getDeployments(_method, _interface, _accessor, true, false) + ",";
            str += "\n\t" + someIPGen.getDeployments(_method, _interface, _accessor, false, true);
            for (auto it : errorReplyCallbacks)
            {
                str += ",\n" + it;
            }
            str += ")";
        }
        else
        {
            counterMap[someipStubDispatcherVariable(_method)] = counterMap[someipStubDispatcherVariable(_method)] + 1;
            methodnumberMap[_method] = counterMap[someipStubDispatcherVariable(_method)];
            str += someipStubDispatcherVariable(_method) +
                   std::to_string(counterMap[someipStubDispatcherVariable(_method)]) + "(";
            str +=
                "\n\t&" + genExtention.getStubClassName(_interface) + "::" + genExtention.getElementName(_method) + ",";
            str += "\n\t" + someIPGen.isLittleEndian(_method, _accessor) + ",";
            str +=
                "\n\t_stub->hasElement(" + std::to_string(genExtention.getElementPosition(_interface, _method)) + "),";
            str += "\n\t" + someIPGen.getDeployments(_method, _interface, _accessor, true, false) + ",";
            str += "\n\t" + someIPGen.getDeployments(_method, _interface, _accessor, false, true);
            for (auto it : errorReplyCallbacks)
            {
                str += ",\n" + it;
            }
            str += ")";
        }
    }
    else
    {
        if (!(counterMap.count(someipStubDispatcherVariable(_method))))
        {
            counterMap[someipStubDispatcherVariable(_method)] = 0;
            methodnumberMap[_method] = 0;
            str += someipStubDispatcherVariable(_method) + "(";
            str +=
                "\n\t&" + genExtention.getStubClassName(_interface) + "::" + genExtention.getElementName(_method) + ",";
            str += "\n\t" + someIPGen.isLittleEndian(_method, _accessor) + ",";
            str +=
                "\n\t_stub->hasElement(" + std::to_string(genExtention.getElementPosition(_interface, _method)) + "),";
            str += "\n\t" + someIPGen.getDeployments(_method, _interface, _accessor, true, false) + ")";
        }
        else
        {
            counterMap[someipStubDispatcherVariable(_method)] = counterMap[someipStubDispatcherVariable(_method)] + 1;
            methodnumberMap[_method] = counterMap[someipStubDispatcherVariable(_method)];
            str += someipStubDispatcherVariable(_method) +
                   std::to_string(counterMap[someipStubDispatcherVariable(_method)]) + "(";
            str +=
                "\n\t&" + genExtention.getStubClassName(_interface) + "::" + genExtention.getElementName(_method) + ",";
            str += "\n\t" + someIPGen.isLittleEndian(_method, _accessor) + ",";
            str +=
                "\n\t_stub->hasElement(" + std::to_string(genExtention.getElementPosition(_interface, _method)) + "),";
            str += "\n\t" + someIPGen.getDeployments(_method, _interface, _accessor, true, false) + ")";
        }
    }
    return str;
}

std::string FInterfaceSomeIPStubAdapterGenerator::getDeployedTypeName(
    const std::shared_ptr<BstIdl::FArgument> &_arg, const std::shared_ptr<BstIdl::FInterface> &_interface,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    std::string deploymentType = someIPGen.getDeploymentType(_arg, _interface, true);
    if (deploymentType != "CommonAPI::EmptyDeployment" && !deploymentType.empty())
    {
        return "CommonAPI::Deployable< " + genExtention.getTypeName(_arg, _interface, true) + ", " + deploymentType +
               " > ";
    }
    else
    {
        return genExtention.getTypeName(_arg, _interface, true);
    }
}

std::string FInterfaceSomeIPStubAdapterGenerator::getDeployedElementName(
    const std::shared_ptr<BstIdl::FArgument> &_arg, const std::shared_ptr<BstIdl::FInterface> &_interface,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    std::string deploymentType = someIPGen.getDeploymentType(_arg, _interface, true);
    if (deploymentType != "CommonAPI::EmptyDeployment" && !deploymentType.empty())
    {
        return "deployed_" + _arg->getName();
    }
    else
    {
        return "_" + _arg->getName();
    }
}

std::string FInterfaceSomeIPStubAdapterGenerator::getInterfaceHierarchy(
    const std::shared_ptr<BstIdl::FInterface> &_interface)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    if (_interface->getBase() == nullptr)
    {
        return genExtention.getStubFullClassName(_interface);
    }
    else
    {
        return genExtention.getStubFullClassName(_interface) + ", " + getInterfaceHierarchy(_interface->getBase());
    }
}

std::string FInterfaceSomeIPStubAdapterGenerator::generateStubAdapterSource(
    const std::shared_ptr<BstIdl::FInterface> &_interface, const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
    std::list<std::shared_ptr<BstIdl::FDExtensionRoot>> &providers)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    std::string header;
    header += someIPGen.generateCommonApiSomeIPLicenseHeader();
    header += "\n" + getLicense();
    header += "\n#include <" + someipStubAdapterHeaderPath(_interface) + ">";
    header += "\n#include <" + genExtention.getHeaderPath(_interface) + ">";
    header += "\n" + genExtention.startInternalCompilation();
    header += "\n#include <CommonAPI/SomeIP/AddressTranslator.hpp>";
    header += "\n" + genExtention.endInternalCompilation();
    header += genExtention.generateVersionNamespaceBegin(_interface);
    auto model = std::dynamic_pointer_cast<BstIdl::FModel>(_interface->getContainer());
    header += genExtention.generateNamespaceBeginDeclaration(model);
    header += "\n\nstd::shared_ptr<CommonAPI::SomeIP::StubAdapter> create" + someipStubAdapterClassName(_interface) +
              R"((
                   const CommonAPI::SomeIP::Address &_address,
                   const std::shared_ptr<CommonAPI::SomeIP::ProxyConnection> &_connection,
                   const std::shared_ptr<CommonAPI::StubBase> &_stub) {
    return std::make_shared< )" +
              someipStubAdapterClassName(_interface);
    header += "<" + getInterfaceHierarchy(_interface) + ">>(_address, _connection, _stub);\n}\n";
    header += "\nvoid initialize" + someipStubAdapterClassName(_interface) + "() {";
    for (auto p : providers)
    {
        auto providerAccessor =
            std::make_shared<SomeipPropertyAccessor>(std::make_shared<BstIdl::FDeployedProvider>(p));
        for (auto i : BstIdl::ProviderUtils::getInstances(p))
        {
            if (i->getTarget() == _interface)
            {
                header += "\n\tCommonAPI::SomeIP::AddressTranslator::get()->"
                          "insert(";
                header += "\n\t\t\"local:" + genExtention.getFullyQualifiedNameWithVersion(_interface) + ":" +
                          providerAccessor->getInstanceId(std::dynamic_pointer_cast<BstIdl::FDExtensionElement>(i)) +
                          "\",";
                header += "\n\t\t" + someIPGen.getSomeIpServiceID(_interface) + ", ";
                header += fmt::format("{0:#x}", providerAccessor->getSomeIpInstanceID(
                                                    std::dynamic_pointer_cast<BstIdl::FDExtensionElement>(i)));
                header += ", " + std::to_string(_interface->getVersion()->getMajor()) + ", " +
                          std::to_string(_interface->getVersion()->getMinor()) + ");";
            }
        }
    }
    header += "\n\tCommonAPI::SomeIP::Factory::get()->"
              "registerStubAdapterCreateMethod(";
    header += "\n\t\t\"" + genExtention.getFullyQualifiedNameWithVersion(_interface) + "\",";
    header += "\n\t\t&create" + someipStubAdapterClassName(_interface) + ");\n}\n";
    header += "\nINITIALIZER(register" + someipStubAdapterClassName(_interface) + ") {";
    header += "\n\tCommonAPI::SomeIP::Factory::get()->registerInterface(initialize" +
              someipStubAdapterClassName(_interface) + ");\n}\n";
    header += "\n" + genExtention.generateNamespaceEndDeclaration(model);
    header += genExtention.generateVersionNamespaceEnd(_interface);
    return header;
}

std::string FInterfaceSomeIPStubAdapterGenerator::generateAttributeDispatcherTableContent(
    const std::shared_ptr<BstIdl::FInterface> &_interface)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    std::string str;
    auto accessor = someIPGen.getSomeIpAccessor(_interface);
    for (auto attribute : _interface->getAttributes())
    {
        str += FTypeGenerator::generateComments(attribute, false);
        auto getIdentifier = someIPGen.getGetterIdentifier(attribute, accessor);
        if (getIdentifier != "0x0")
        {
            str += dispatcherTableEntry(_interface, getIdentifier, someipGetStubDispatcherVariable(attribute)) + "\n";
        }
        if (!attribute->isReadonly())
        {
            str += dispatcherTableEntry(_interface, someIPGen.getSetterIdentifier(attribute, accessor),
                                        someipSetStubDispatcherVariable(attribute)) +
                   "\n";
        }
    }
    return str;
}

std::string FInterfaceSomeIPStubAdapterGenerator::generateMethodDispatcherTableContent(
    const std::shared_ptr<BstIdl::FInterface> &_interface, std::map<std::string, int> &_counters,
    std::map<std::shared_ptr<BstIdl::FMethod>, int> &_methods)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    std::string str;
    auto accessor = someIPGen.getSomeIpAccessor(_interface);
    for (auto method : _interface->getMethods())
    {
        str += FTypeGenerator::generateComments(method, false);
        if (_methods[method] == 0)
        {
            str += dispatcherTableEntry(_interface, someIPGen.getMethodIdentifier(method, accessor),
                                        someipStubDispatcherVariable(method)) +
                   "\n";
        }
        else
        {
            str += dispatcherTableEntry(_interface, someIPGen.getMethodIdentifier(method, accessor),
                                        someipStubDispatcherVariable(method) + std::to_string(_methods[method])) +
                   "\n";
        }
    }
    return str;
}

std::string FInterfaceSomeIPStubAdapterGenerator::someipStubAdapterHeaderFile(
    const std::shared_ptr<BstIdl::FInterface> &fInterface)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    return genExtention.getElementName(fInterface) + "SomeIPStubAdapter.hpp";
}

std::string FInterfaceSomeIPStubAdapterGenerator::someipStubAdapterHeaderPath(
    const std::shared_ptr<BstIdl::FInterface> &fInterface)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    return genExtention.getVersionPathPrefix(fInterface) +
           genExtention.getDirectoryPath(std::dynamic_pointer_cast<BstIdl::FModel>(fInterface->getContainer())) + "/" +
           someipStubAdapterHeaderFile(fInterface);
}

std::string FInterfaceSomeIPStubAdapterGenerator::someipStubAdapterSourceFile(
    const std::shared_ptr<BstIdl::FInterface> &fInterface)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    return genExtention.getElementName(fInterface) + "SomeIPStubAdapter.cpp";
}

std::string FInterfaceSomeIPStubAdapterGenerator::someipStubAdapterSourcePath(
    const std::shared_ptr<BstIdl::FInterface> &fInterface)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    return genExtention.getVersionPathPrefix(fInterface) +
           genExtention.getDirectoryPath(std::dynamic_pointer_cast<BstIdl::FModel>(fInterface->getContainer())) + "/" +
           someipStubAdapterSourceFile(fInterface);
}

std::string FInterfaceSomeIPStubAdapterGenerator::someipStubAdapterClassName(
    const std::shared_ptr<BstIdl::FInterface> &fInterface)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    return genExtention.getElementName(fInterface) + "SomeIPStubAdapter";
}

std::string FInterfaceSomeIPStubAdapterGenerator::someipStubAdapterClassNameInternal(
    const std::shared_ptr<BstIdl::FInterface> &fInterface)
{
    return someipStubAdapterClassName(fInterface) + "Internal";
}

std::string FInterfaceSomeIPStubAdapterGenerator::someipStubAdapterHelperClassName(
    const std::shared_ptr<BstIdl::FInterface> &fInterface)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    return genExtention.getElementName(fInterface) + "SomeIPStubAdapterHelper";
}

std::string FInterfaceSomeIPStubAdapterGenerator::getAllInTypes(const std::shared_ptr<BstIdl::FMethod> &fMethod)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    std::string str;
    for (auto it : fMethod->getInArgs())
    {
        if (it == fMethod->getInArgs().front())
        {
            str += genExtention.getTypeName(it, fMethod, true);
        }
        else
            str += ", " + genExtention.getTypeName(it, fMethod, true);
    }
    return str;
}

std::string FInterfaceSomeIPStubAdapterGenerator::getAllOutTypes(const std::shared_ptr<BstIdl::FMethod> &fMethod)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    std::string types;
    std::list<std::string> args;
    for (auto it : fMethod->getOutArgs())
    {
        args.emplace_back(genExtention.getTypeName(it, fMethod, true));
    }

    if (genExtention.hasError(fMethod))
    {
        args.emplace_front(genExtention.getErrorNameReference(fMethod, fMethod->getContainer()));
    }
    return join(args, ", ");
}

std::string FInterfaceSomeIPStubAdapterGenerator::someipStubDispatcherVariable(
    const std::shared_ptr<BstIdl::FMethod> &fMethod)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    auto name = toFirstLower(genExtention.getElementName(fMethod));

    return name + "StubDispatcher";
}

std::string FInterfaceSomeIPStubAdapterGenerator::someipGetStubDispatcherVariable(
    const std::shared_ptr<BstIdl::FAttribute> &fAttribute)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    return someIPGen.getMethodName(fAttribute) + "StubDispatcher";
}

std::string FInterfaceSomeIPStubAdapterGenerator::someipSetStubDispatcherVariable(
    const std::shared_ptr<BstIdl::FAttribute> &fAttribute)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    return someIPGen.setMethodName(fAttribute) + "StubDispatcher";
}

std::string FInterfaceSomeIPStubAdapterGenerator::generateFireChangedMethodBody(
    const std::shared_ptr<BstIdl::FAttribute> &_attribute, const std::shared_ptr<BstIdl::FInterface> &_interface,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    std::string str;
    std::string deploymentType = someIPGen.getDeploymentType(_attribute, _interface, true);
    auto deployment = someIPGen.getDeploymentRef(_attribute, _attribute->isArray(), nullptr, _interface,
                                                 _accessor->getOverwriteAccessor(_attribute));
    if (deploymentType != "CommonAPI::EmptyDeployment" && !deploymentType.empty())
    {
        str += "CommonAPI::Deployable< " + genExtention.getTypeName(_attribute, _interface, true) + ", " +
               deploymentType + "> deployedValue(_value, ";
        if (!deployment.empty())
        {
            str += deployment;
        }
        else
        {
            str += "nullptr";
        }
        str += ");\n";
    }
    str += "CommonAPI::SomeIP::StubEventHelper<\n\tCommonAPI::SomeIP::"
           "SerializableArguments<";
    if (deploymentType != "CommonAPI::EmptyDeployment" && !deploymentType.empty())
    {
        str += "\n\t\tCommonAPI::Deployable<";
        str += "\n\t\t\t" + genExtention.getTypeName(_attribute, _interface, true) + ",";
        str += "\n\t\t\t" + deploymentType + "\n\t\t>";
    }
    else
    {
        str += "\n\t\t\t" + genExtention.getTypeName(_attribute, _interface, true);
    }
    str += "\n\t\t>\n>::sendEvent(\n\t*this,";
    str += "\n\t" + someIPGen.getNotifierIdentifier(_attribute, _accessor) + ",";
    str += "\n\t" + someIPGen.getEndianess(_attribute, _accessor) + ",";
    if (deploymentType != "CommonAPI::EmptyDeployment" && !deploymentType.empty())
    {
        str += "\n\tdeployedValue";
    }
    else
    {
        str += "\n\t_value";
    }
    str += "\n);";
    return str;
}

std::string FInterfaceSomeIPStubAdapterGenerator::generateStubAttributeTableInitializer(
    const std::shared_ptr<BstIdl::FInterface> &fInterface, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    return "";
}

std::string FInterfaceSomeIPStubAdapterGenerator::generateErrorReplyCallback(
    const std::shared_ptr<BstIdl::FBroadcast> &_broadcast, const std::shared_ptr<BstIdl::FInterface> &_interface,
    const std::shared_ptr<BstIdl::FMethod> &_method, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    std::string str;
    str += "void " + genExtention.errorReplyCallbackName(_broadcast, _accessor) + "(" +
           genExtention.generateErrorReplyCallbackSignature(_broadcast, _method, _accessor) + ") {";
    if (genExtention.errorArgs(_broadcast, _accessor).size() > 1)
    {
        str += "\n\tauto args = std::make_tuple(\n\t\t";
        for (auto it : genExtention.errorArgs(_broadcast, _accessor))
        {
            if (it == genExtention.errorArgs(_broadcast, _accessor).front())
            {
                str += "\n\t\t" + someIPGen.getDeployable(it, _interface, _accessor) + "(_" +
                       genExtention.getElementName(it) + ", " +
                       someIPGen.getDeploymentRef(it, it->isArray(), _broadcast, _interface, _accessor) + ")";
            }

            str += ",\n\t\t" + someIPGen.getDeployable(it, _interface, _accessor) + "(_" +
                   genExtention.getElementName(it) + ", " +
                   someIPGen.getDeploymentRef(it, it->isArray(), _broadcast, _interface, _accessor) + ")";
        }
        str += ");";
    }
    else
    {
        str += "\n\tauto args = std::make_tuple();";
    }
    str += "\n\t(void)args;";
    str += "\n\t//sayHelloStubDispatcher.sendErrorReplyMessage(_call, " +
           genExtention.errorName(_broadcast, _accessor) + ", args);\n}";
    return str;
}
} // namespace BstCommonAPI