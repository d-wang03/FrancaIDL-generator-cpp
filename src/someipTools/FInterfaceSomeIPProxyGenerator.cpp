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
#include "someipTools/FInterfaceSomeIPProxyGenerator.h"
#include "FDModel/FDeployedProvider.h"
#include "FDModel/ProviderUtils.h"
#include "capicxx-core-tools/FTypeGenerator.h"
#include "fmt/core.h"
#include "model/FTypeCollection.h"
#include "someipTools/FrancaSomeIPGeneratorExtensions.h"
#include "someipTools/SomeipPropertyAccessor.h"
#include "utilities/string_utility.h"
#include <memory>
namespace BstCommonAPI
{
void FInterfaceSomeIPProxyGenerator::generateProxy(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                                   const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                   std::list<std::shared_ptr<BstIdl::FDExtensionRoot>> &providers,
                                                   bool generate_code, bool generateSyncCalls, std::string &dir)
{
    FrancaGeneratorExtensions &gen = FrancaGeneratorExtensions::getInstance();
    auto names = gen.getFullName(fInterface);
    names = names.substr(0, names.find_last_of("::") + 1);
    if (generate_code)
    {
        auto header_path = gen.someipProxyHeaderPath(fInterface);
        auto h_path = mk_dir(dir, header_path);
        if (h_path.empty())
            return;
        auto pos = header_path.find_last_of("/") + 1;
        auto file_name = header_path.substr(pos, header_path.size() - pos);
        std::ofstream OsWrite(h_path + "/" + file_name, std::ofstream::out);
        auto header = generateProxyHeader(fInterface, _accessor);
        remove_all(header, names);
        OsWrite << replace_all(header, "\t", "    ");
        OsWrite.close();

        auto source_path = gen.someipProxySourcePath(fInterface);
        auto s_path = mk_dir(dir, source_path);
        if (s_path.empty())
            return;
        pos = source_path.find_last_of("/") + 1;
        auto s_file_name = source_path.substr(pos, source_path.size() - pos);
        std::ofstream OsWriteS(s_path + "/" + s_file_name, std::ofstream::out);
        auto source = generateProxySource(fInterface, _accessor, providers);
        remove_all(source, names);
        OsWriteS << replace_all(source, "\t", "    ");
        OsWriteS.close();
    }
}

std::string FInterfaceSomeIPProxyGenerator::generateProxySource(
    const std::shared_ptr<BstIdl::FInterface> &fInterface, const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
    std::list<std::shared_ptr<BstIdl::FDExtensionRoot>> &providers)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    std::string header;
    header += someIPGen.generateCommonApiSomeIPLicenseHeader();
    header += "\n" + getLicense();
    header += "\n" + FTypeGenerator::generateComments(fInterface, false);
    header += "\n#include <" + genExtention.someipProxyHeaderPath(fInterface) + ">";
    header += "\n" + genExtention.startInternalCompilation();
    header += "\n#include <CommonAPI/SomeIP/AddressTranslator.hpp>";
    header += "\n" + genExtention.endInternalCompilation();
    header += genExtention.generateVersionNamespaceBegin(fInterface);
    auto model = std::dynamic_pointer_cast<BstIdl::FModel>(fInterface->getContainer());
    header += genExtention.generateNamespaceBeginDeclaration(model) + "\n";
    header += "\nstd::shared_ptr<CommonAPI::SomeIP::Proxy> create" + someIPGen.someipProxyClassName(fInterface) + "(";
    header += "\n\tconst CommonAPI::SomeIP::Address &_address,";
    header += "\n\tconst std::shared_ptr<CommonAPI::SomeIP::ProxyConnection> &_connection) {";
    header +=
        "\n\treturn std::make_shared<" + someIPGen.someipProxyClassName(fInterface) + ">(_address, _connection);\n}\n";
    header += "\nvoid initialize" + someIPGen.someipProxyClassName(fInterface) + "() {";
    std::string insert;
    for (auto p : providers)
    {
        std::shared_ptr<SomeipPropertyAccessor> providerAccessor =
            std::make_shared<SomeipPropertyAccessor>(std::make_shared<BstIdl::FDeployedProvider>(p));
        for (auto i : BstIdl::ProviderUtils::getInstances(p))
        {
            insert += "\n\tCommonAPI::SomeIP::AddressTranslator::get()->insert(";
            auto instanceID = providerAccessor->getInstanceId(std::dynamic_pointer_cast<BstIdl::FDExtensionElement>(i));
            insert +=
                "\n\t\t\"local:" + genExtention.getFullyQualifiedNameWithVersion(fInterface) + ":" + instanceID + "\",";
            insert += "\n\t\t" + someIPGen.getSomeIpServiceID(fInterface) +
                      fmt::format(", {0:#x}, ", providerAccessor->getSomeIpInstanceID(
                                                    std::dynamic_pointer_cast<BstIdl::FDExtensionElement>(i))) +
                      std::to_string(fInterface->getVersion()->getMajor()) + ", " +
                      std::to_string(fInterface->getVersion()->getMinor()) + ");";
        }
    }
    header += insert;
    header += "\n\tCommonAPI::SomeIP::Factory::get()->registerProxyCreateMethod(";
    header += "\n\t\t\"" + genExtention.getFullyQualifiedNameWithVersion(fInterface) + "\",";
    header += "\n\t\t&create" + someIPGen.someipProxyClassName(fInterface) + ");\n}\n";
    header += "\nINITIALIZER(register" + someIPGen.someipProxyClassName(fInterface) + ") {";
    header += "\n\tCommonAPI::SomeIP::Factory::get()->registerInterface(initialize" +
              someIPGen.someipProxyClassName(fInterface) + ");\n}\n";
    auto hasAttribute = fInterface->getAttributes().size() > 0;
    auto hasBroadcast = fInterface->getBroadcasts().size() > 0;
    auto hasManaged = fInterface->getManagedInterfaces().size() > 0;
    header +=
        "\n" + someIPGen.someipProxyClassName(fInterface) + "::" + someIPGen.someipProxyClassName(fInterface) + "(";
    header += "\n\tconst CommonAPI::SomeIP::Address &_address,";
    header += "\n\tconst std::shared_ptr<CommonAPI::SomeIP::ProxyConnection> &_connection)";
    header += "\n\t\t: CommonAPI::SomeIP::Proxy(_address, _connection)";
    if (fInterface->getBase() != nullptr || hasAttribute || hasBroadcast || hasManaged)
    {
        header += ",";
    }
    if (fInterface->getBase() != nullptr)
    {
        header += genExtention.generateSomeIPBaseInstantiations(fInterface);
        if (hasAttribute || hasBroadcast || hasManaged)
        {
            header += ",";
        }
    }
    for (auto attribute : fInterface->getAttributes())
    {
        header +=
            "\n\t\t " + replace_all(generateVariableInit(attribute, _accessor, fInterface), "\n", "\n\t\t  ") + ",";
    }
    for (auto broadcast : fInterface->getBroadcasts())
    {
        header += "\n\t\t  " + someipClassVariableName(broadcast) + "(*this, " +
                  someIPGen.getEventGroups(broadcast, _accessor).front() + ", " +
                  someIPGen.getEventIdentifier(broadcast, _accessor) + ", ";
        if (broadcast->isSelective())
        {
            header += "CommonAPI::SomeIP::event_type_e::ET_SELECTIVE_EVENT";
        }
        else
        {
            header += "CommonAPI::SomeIP::event_type_e::ET_EVENT";
        }
        header += ", " + someIPGen.getReliabilityType(broadcast, _accessor) + ", " +
                  someIPGen.getEndianess(broadcast, _accessor) + ", " +
                  someIPGen.getDeployments(broadcast, fInterface, _accessor) + "),";
    }
    for (auto managed : fInterface->getManagedInterfaces())
    {
        header += "\n" + genExtention.proxyManagerMemberName(managed) + "(*this, \"" +
                  genExtention.getFullyQualifiedNameWithVersion(managed) + "\", " +
                  getSomeIpServiceIDForInterface(providers, managed) + "),";
    }
    if (header.back() == ',')
        header.pop_back();
    header += "\n{\n}\n";
    header += "\n" + someIPGen.someipProxyClassName(fInterface) + "::~" + someIPGen.someipProxyClassName(fInterface) +
              "() {\n\tcompleted_.set_value();\n}\n";
    for (auto attribute : fInterface->getAttributes())
    {
        header +=
            "\n" +
            genExtention.generateGetMethodDefinitionWithin(attribute, someIPGen.someipProxyClassName(fInterface)) +
            " {";
        header += "\n\treturn " + someipClassVariableName(attribute);
        header += ";\n}";
    }
    for (auto broadcast : fInterface->getBroadcasts())
    {
        header +=
            "\n" +
            genExtention.generateGetMethodDefinitionWithin(broadcast, someIPGen.someipProxyClassName(fInterface)) +
            " {";
        header += "\n\treturn " + someipClassVariableName(broadcast) + ";\n}";
    }
    header += "\n";
    for (auto method : fInterface->getMethods())
    {
        auto timeout = _accessor->getTimeout(method);
        auto inParams = generateInParams(method, _accessor);
        auto outParams = generateOutParams(method, _accessor, false);
        auto comments = FTypeGenerator::generateComments(method, false);
        header += (comments.empty() ? "" : ("\n" + comments));
        if (m_generateSyncCalls || method->isFireAndForget())
        {
            header += "\n" +
                      genExtention.generateDefinitionWithin(method, someIPGen.someipProxyClassName(fInterface), false) +
                      " {";
            header += "\n\t" +
                      replace_all(generateProxyHelperDeployments(method, fInterface, false, _accessor), "\n", "\n\t");
            if (method->isFireAndForget())
            {
                header += "\n\t" + replace_all(generateProxyHelperClass(method, fInterface, _accessor), "\n", "\n\t") +
                          "::callMethod(";
            }
            else
            {
                if (timeout > 0)
                {
                    header += "\n\tstatic CommonAPI::CallInfo info(" + std::to_string(timeout) + ");";
                }
                header += "\n\t" + replace_all(generateProxyHelperClass(method, fInterface, _accessor), "\n", "\n\t") +
                          "::callMethodWithReply(";
            }
            header += "\n\t\t*this,";
            header += "\n\t\t" + someIPGen.getMethodIdentifier(method, _accessor) + ",";
            header += "\n\t\t" + someIPGen.isReliable(method, _accessor) + ",";
            header += "\n\t\t" + someIPGen.isLittleEndian(method, _accessor) + ",";
            if (!method->isFireAndForget())
            {
                header += "\n\t\t(_info ? _info : ";
                if (timeout > 0)
                    header += "&info";
                else
                {
                    header += "&CommonAPI::SomeIP::defaultCallInfo";
                }
                header += "),";
            }
            if (!inParams.empty())
            {
                header += "\n\t\t" + inParams + ",";
            }
            header += "\n\t\t_internalCallStatus";
            if (genExtention.hasError(method))
            {
                header += ",\n\t\tdeploy_error";
            }

            if (!outParams.empty())
            {
                header += ",\n\t\t" + outParams;
            }
            header += ");";
            auto tmp = replace_all(generateOutParamsValue(method, _accessor), "\n", "\n\t");
            header += (tmp.empty() ? "" : ("\n\t" + tmp)) + "\n}\n";
        }
        if (!method->isFireAndForget())
        {
            header +=
                "\n" +
                genExtention.generateAsyncDefinitionWithin(method, someIPGen.someipProxyClassName(fInterface), false) +
                " {";
            if (timeout > 0)
            {
                header += "\n\tstatic CommonAPI::CallInfo info(" + std::to_string(timeout) + ");";
            }

            header +=
                "\n\t" + replace_all(generateProxyHelperDeployments(method, fInterface, true, _accessor), "\n", "\n\t");
            header += "\n\treturn " +
                      replace_all(generateProxyHelperClass(method, fInterface, _accessor), "\n", "\n\t") +
                      "::callMethodAsync(";
            header += "\n\t\t*this,";
            header += "\n\t\t" + someIPGen.getMethodIdentifier(method, _accessor) + ",";
            header += "\n\t\t" + someIPGen.isReliable(method, _accessor) + ",";
            header += "\n\t\t" + someIPGen.isLittleEndian(method, _accessor) + ",";

            header += "\n\t\t(_info ? _info : ";
            if (timeout > 0)
                header += "&info";
            else
            {
                header += "&CommonAPI::SomeIP::defaultCallInfo";
            }
            header += "),";
            if (!inParams.empty())
            {
                header += "\n\t\t" + inParams + ",";
            }
            header += "\n\t\t" + replace_all(generateCallback(method, fInterface, _accessor), "\n", "\n\t") + ");\n}\n";
        }
    }
    for (auto managed : fInterface->getManagedInterfaces())
    {
        header += "\nCommonAPI::ProxyManager& " + someIPGen.someipProxyClassName(fInterface) +
                  "::" + genExtention.proxyManagerGetterName(managed) + "() {";
        header += "\n\treturn " + genExtention.proxyManagerMemberName(managed) + ";\n}";
    }
    header += "\nvoid " + someIPGen.someipProxyClassName(fInterface) +
              "::getOwnVersion(uint16_t& ownVersionMajor, uint16_t& ownVersionMinor) const {";
    auto itsVersion = fInterface->getVersion();
    if (itsVersion != nullptr)
    {
        header += "\n\townVersionMajor = " + std::to_string(fInterface->getVersion()->getMajor()) + ";";
        header += "\n\townVersionMinor = " + std::to_string(fInterface->getVersion()->getMinor()) + ";";
    }
    else
    {
        header += "\n\townVersionMajor = 0;";
        header += "\n\townVersionMinor = 0;";
    }
    header += "\n}\n";
    header += "\nstd::future<void> " + someIPGen.someipProxyClassName(fInterface) +
              "::getCompletionFuture() {\n\treturn completed_.get_future();\n}\n";
    header += "\n" + genExtention.generateNamespaceEndDeclaration(model);
    header += genExtention.generateVersionNamespaceEnd(fInterface);
    return header;
}

std::string FInterfaceSomeIPProxyGenerator::generateProxyHeader(
    const std::shared_ptr<BstIdl::FInterface> &fInterface, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    std::string header;
    header += someIPGen.generateCommonApiSomeIPLicenseHeader();
    header += "\n" + getLicense();
    header += "\n" + FTypeGenerator::generateComments(fInterface, false);

    auto defineName = genExtention.getDefineName(fInterface);
    transform(defineName.begin(), defineName.end(), defineName.begin(), ::toupper);
    header += "\n#ifndef " + defineName + "_SOMEIP_PROXY_HPP_";
    header += "\n#define " + defineName + "_SOMEIP_PROXY_HPP_\n";
    header += "\n#include <" + genExtention.getProxyBaseHeaderPath(fInterface) + ">";
    if (fInterface->getBase() != nullptr)
    {
        header += "\n#include <" + genExtention.someipProxyHeaderPath(fInterface->getBase()) + ">";
    }
    auto DeploymentHeaders = someIPGen.getDeploymentInputIncludes(fInterface, _accessor);
    for (auto it : DeploymentHeaders)
    {
        header += "\n#include <" + it + ">";
    }
    header += "\n" + genExtention.startInternalCompilation();

    header += "\n#include <CommonAPI/SomeIP/Factory.hpp>";
    header += "\n#include <CommonAPI/SomeIP/Proxy.hpp>";
    header += "\n#include <CommonAPI/SomeIP/Types.hpp>";
    if (genExtention.hasAttributes(fInterface))
    {
        header += "\n#include <CommonAPI/SomeIP/Attribute.hpp>";
    }
    if (genExtention.hasBroadcasts(fInterface))
    {
        header += "\n#include <CommonAPI/SomeIP/Event.hpp>";
        if (genExtention.hasSelectiveBroadcasts(fInterface))
        {
            header += "\n#include <CommonAPI/Types.hpp>";
        }
    }
    if (!fInterface->getManagedInterfaces().empty())
    {
        header += "\n#include <CommonAPI/SomeIP/ProxyManager.hpp>";
    }
    header += "\n" + genExtention.endInternalCompilation() + "\n";
    header += R"(#include <string>

# if defined(_MSC_VER)
#  if _MSC_VER >= 1300
/*
 * Diamond inheritance is used for the CommonAPI::Proxy base class.
 * The Microsoft compiler put warning (C4250) using a desired c++ feature: "Delegating to a sister class"
 * A powerful technique that arises from using virtual inheritance is to delegate a method from a class in another class
 * by using a common abstract base class. This is also called cross delegation.
 */
#    pragma warning( disable : 4250 )
#  endif
# endif)";
    header += "\n" + genExtention.generateVersionNamespaceBegin(fInterface);
    auto model = std::dynamic_pointer_cast<BstIdl::FModel>(fInterface->getContainer());
    header += genExtention.generateNamespaceBeginDeclaration(model) + "\n";
    header += "\nclass " + someIPGen.someipProxyClassName(fInterface);
    header += "\n\t: virtual public " + genExtention.getProxyBaseClassName(fInterface) + ",";
    header += "\n\t  virtual public ";
    if (fInterface->getBase() != nullptr)
    {
        header += genExtention.getTypeCollectionName(fInterface->getBase(), fInterface) + "SomeIPProxy";
    }
    else
    {
        header += "CommonAPI::SomeIP::Proxy";
    }
    header += "{\npublic:";
    header += "\n\t" + someIPGen.someipProxyClassName(fInterface) + "(";
    header += "\n\t\tconst CommonAPI::SomeIP::Address &_address,";
    header += "\n\t\tconst std::shared_ptr<CommonAPI::SomeIP::ProxyConnection> &_connection);\n";
    header += "\n\tvirtual ~" + someIPGen.someipProxyClassName(fInterface) + "();\n";
    for (auto attribute : fInterface->getAttributes())
    {
        header += "\n\tvirtual " + genExtention.generateGetMethodDefinition(attribute) + ";\n";
    }
    for (auto broadcast : fInterface->getBroadcasts())
    {
        header += "\n\tvirtual " + genExtention.generateGetMethodDefinition(broadcast) + ";\n";
    }
    for (auto method : fInterface->getMethods())
    {
        header += "\n\t" + replace_all(FTypeGenerator::generateComments(method, false), "\n", "\n\t");
        if (m_generateSyncCalls || method->isFireAndForget())
        {
            header += "\n\tvirtual " + genExtention.generateDefinition(method, false) + ";\n";
        }
        if (!method->isFireAndForget())
        {
            header += "\n\tvirtual " + genExtention.generateAsyncDefinition(method, false) + ";\n";
        }
    }
    for (auto managed : fInterface->getManagedInterfaces())
    {
        header += "\n\tvirtual CommonAPI::ProxyManager &" + genExtention.proxyManagerGetterName(managed) + "();\n";
    }
    header += "\n\tvirtual void getOwnVersion(uint16_t &_major, uint16_t &_minor) const;\n";
    header += "\n\tvirtual std::future<void> getCompletionFuture();\n";
    header += "\nprivate:\n";
    for (auto attribute : fInterface->getAttributes())
    {
        if (genExtention.supportsTypeValidation(attribute))
        {
            header += "\n\tclass SomeIP" + someipClassVariableName(attribute) + "Attribute : public " +
                      someipClassName(attribute, fInterface, _accessor) + " {";
            header += "\n\tpublic:\n\t\ttemplate <typename... _A>";
            header += "\n\t\t\tSomeIP" + someipClassVariableName(attribute) + "Attribute(" +
                      someIPGen.someipProxyClassName(fInterface) + " &_proxy,";
            header += "\n\t\t\t\t_A ... arguments) : " + someipClassName(attribute, fInterface, _accessor) +
                      "(\n\t\t\t\t\t_proxy, arguments...) {}";
            if (!attribute->isReadonly())
            {
                header += "\n\t\tvoid setValue(const " + genExtention.getTypeName(attribute, fInterface, true) +
                          "& requestValue,";
                header += "\n\t\t\t\tCommonAPI::CallStatus& callStatus,";
                header += "\n\t\t\t\t" + genExtention.getTypeName(attribute, fInterface, true) + "& responseValue,";
                header += "\n\t\t\t\tconst CommonAPI::CallInfo *_info = nullptr) {";
                header +=
                    "\n\t\t\t// validate input parameters\n\t\t\tif (!requestValue.validate()) {\n\t\t\t\tcallStatus = "
                    "CommonAPI::CallStatus::INVALID_VALUE;\n\t\t\t\treturn;\n\t\t\t}";
                header += "\n\t\t\t// call base function if ok";
                header += "\n\t\t\t" + someipClassName(attribute, fInterface, _accessor) +
                          "::setValue(requestValue, callStatus, responseValue, _info);\n\t\t}";
                header += "\n\t\tstd::future<CommonAPI::CallStatus> setValueAsync(const " +
                          genExtention.getTypeName(attribute, fInterface, true) + "& requestValue,";
                header += "\n\t\t\t\tstd::function<void(const CommonAPI::CallStatus &, " +
                          genExtention.getTypeName(attribute, fInterface, true) + ")> _callback,";
                header += "\n\t\t\t\tconst CommonAPI::CallInfo *_info) {";
                header += "\n\t\t\t// validate input parameters\n\t\t\tif (!requestValue.validate()) {";
                header += "\n\t\t\t\t" + genExtention.getTypeName(attribute, fInterface, true) + " _returnvalue;";
                header += "\n\t\t\t\t_callback(CommonAPI::CallStatus::INVALID_VALUE, _returnvalue);";
                header += "\n\t\t\t\tstd::promise<CommonAPI::CallStatus> promise;";
                header += "\n\t\t\t\tpromise.set_value(CommonAPI::CallStatus::INVALID_VALUE);";
                header += "\n\t\t\t\treturn promise.get_future();\n\t\t\t}\n\t\t\t// call base function if ok";
                header += "\n\t\t\treturn " + someipClassName(attribute, fInterface, _accessor) +
                          "::setValueAsync(requestValue, _callback, _info);\n\t\t}";
            }
            header += "\n\t};\n\tSomeIP" + someipClassVariableName(attribute) + "Attribute " +
                      someipClassVariableName(attribute) + ";\n";
        }
        else
        {
            header += "\n\t" + someipClassName(attribute, fInterface, _accessor) + " " +
                      someipClassVariableName(attribute) + ";";
        }
    }
    for (auto broadcast : fInterface->getBroadcasts())
    {
        header +=
            "\n\t" + someipClassName(broadcast, fInterface, _accessor) + " " + someipClassVariableName(broadcast) + ";";
    }
    for (auto managed : fInterface->getManagedInterfaces())
    {
        header += "\n\tCommonAPI::SomeIP::ProxyManager " + genExtention.proxyManagerMemberName(managed) + ";";
    }
    header += "\n\tstd::promise<void> completed_;\n};\n";
    header += "\n" + genExtention.generateNamespaceEndDeclaration(model);
    header += genExtention.generateVersionNamespaceEnd(fInterface);
    header += "\n#endif // " + defineName + "_SOMEIP_PROXY_HPP_";
    return header;
}

std::string FInterfaceSomeIPProxyGenerator::getSomeIpServiceIDForInterface(
    std::list<std::shared_ptr<BstIdl::FDExtensionRoot>> &_providers,
    const std::shared_ptr<BstIdl::FInterface> &_interface)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    for (auto p : _providers)
    {
        for (auto instance : BstIdl::ProviderUtils::getInstances(p))
        {
            if (instance->getTarget() == _interface)
            {
                auto id = someIPGen.getSomeIpAccessor(_interface)->getSomeIpServiceID(_interface);
                if (id != -1)
                {
                    return "0x" + std::to_string(id);
                }
            }
        }
    }
    // If no providers are available, try to get the service id directly
    auto serviceid = someIPGen.getSomeIpAccessor(_interface)->getSomeIpServiceID(_interface);
    if (serviceid != -1)
    {
        return "0x" + std::to_string(serviceid);
    }
    return "UNDEFINED_SERVICE_ID";
}

std::string FInterfaceSomeIPProxyGenerator::someipClassVariableName(
    const std::shared_ptr<BstIdl::FModelElement> &fModelElement)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    auto name = toFirstLower(genExtention.getElementName(fModelElement));
    if (genExtention.getElementName(fModelElement).empty())
        std::cout << "FModelElement has no name\n";
    return name + "_";
}

std::string FInterfaceSomeIPProxyGenerator::someipClassVariableName(
    const std::shared_ptr<BstIdl::FBroadcast> &fBroadcast)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    auto name = toFirstLower(genExtention.getElementName(fBroadcast));
    if (genExtention.getElementName(fBroadcast).empty())
        std::cout << "FModelElement has no name\n";

    if (fBroadcast->isSelective())
        name = name + "Selective";

    name = name + "_";

    return name;
}

std::string FInterfaceSomeIPProxyGenerator::generateProxyHelperDeployments(
    const std::shared_ptr<BstIdl::FMethod> &method, const std::shared_ptr<BstIdl::FInterface> &fInterface,
    bool _isAsync, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    std::list<std::string> args;
    if (someIPGen.hasError(method))
    {
        args.emplace_back("CommonAPI::Deployable< " + someIPGen.getErrorType(method) + ", " +
                          someIPGen.getErrorDeploymentType(method, false) + "> deploy_error(" +
                          someIPGen.getErrorDeploymentRef(method, fInterface, _accessor) + ");");
    }

    for (auto a : method->getInArgs())
    {
        args.emplace_back(
            "CommonAPI::Deployable< " + someIPGen.getTypeName(a, method, true) + ", " +
            someIPGen.getDeploymentType(a, fInterface, true) + "> deploy_" + a->getName() + "(_" + a->getName() + ", " +
            someIPGen.getDeploymentRef(a, a->isArray(), method, fInterface, _accessor->getOverwriteAccessor(a)) + ");");
    }

    for (auto a : method->getOutArgs())
    {
        args.emplace_back(
            "CommonAPI::Deployable< " + someIPGen.getTypeName(a, method, true) + ", " +
            someIPGen.getDeploymentType(a, fInterface, true) + "> deploy_" + a->getName() + "(" +
            someIPGen.getDeploymentRef(a, a->isArray(), method, fInterface, _accessor->getOverwriteAccessor(a)) + ");");
    }
    return join(args, "\n");
}

std::string FInterfaceSomeIPProxyGenerator::generateProxyHelperClass(
    const std::shared_ptr<BstIdl::FMethod> &_method, const std::shared_ptr<BstIdl::FInterface> &fInterface,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::string str;
    std::list<std::string> args;
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    str += "CommonAPI::SomeIP::ProxyHelper<\n";
    for (auto a : _method->getInArgs())
    {
        args.emplace_back("\n\t\tCommonAPI::Deployable<\n\t\t\t" + someIPGen.getTypeName(a, _method, true) +
                          ",\n\t\t\t" + someIPGen.getDeploymentType(a, fInterface, true) + "\n\t\t>");
    }
    str += "\tCommonAPI::SomeIP::SerializableArguments<" + join(args, ",") + "\n\t>,";
    args.clear();
    if (someIPGen.hasError(_method))
    {
        args.emplace_back("\n\t\tCommonAPI::Deployable<\n\t\t\t" + someIPGen.getErrorType(_method) + ",\n\t\t\t" +
                          someIPGen.getErrorDeploymentType(_method, false) + "\n\t\t>");
    }
    for (auto a : _method->getOutArgs())
    {
        args.emplace_back("\n\t\tCommonAPI::Deployable<\n\t\t\t" + someIPGen.getTypeName(a, _method, true) +
                          ",\n\t\t\t" + someIPGen.getDeploymentType(a, fInterface, true) + "\n\t\t>");
    }
    str += "\n\tCommonAPI::SomeIP::SerializableArguments<" + join(args, ",") + "\n\t>\n>";
    return str;
}

std::string FInterfaceSomeIPProxyGenerator::generateInParams(const std::shared_ptr<BstIdl::FMethod> &_method,
                                                             const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    std::string inParams = "";
    for (auto a : _method->getInArgs())
    {
        if (!inParams.empty())
            inParams += ", ";

        inParams += "deploy_" + a->getName();
    }
    return inParams;
}

std::string FInterfaceSomeIPProxyGenerator::generateOutParams(const std::shared_ptr<BstIdl::FMethod> &_method,
                                                              const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                                              bool _instantiate)
{
    std::string outParams = "";
    for (auto a : _method->getOutArgs())
    {
        if (!outParams.empty())
            outParams += ", ";

        outParams += "deploy_" + a->getName();
    }
    return outParams;
}

std::string FInterfaceSomeIPProxyGenerator::generateOutParamsValue(
    const std::shared_ptr<BstIdl::FMethod> &_method, const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    std::string outParamsValue = "";
    if (genExtention.hasError(_method))
    {
        outParamsValue += "_error = deploy_error.getValue();";
    }
    for (auto a : _method->getOutArgs())
    {
        outParamsValue += "_" + a->getName() + " = deploy_" + a->getName() + ".getValue();\n";
    }
    return outParamsValue;
}

std::string FInterfaceSomeIPProxyGenerator::generateCallback(const std::shared_ptr<BstIdl::FMethod> &_method,
                                                             const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                                             const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    std::string error = "";
    if (genExtention.hasError(_method))
    {
        error = "deploy_error";
    }
    std::string callback = "[_callback] (" + generateCallbackParameter(_method, fInterface, _accessor) + ") {\n";
    callback += "\t\tif (_callback)\n";
    callback += "\t\t\t_callback(_internalCallStatus";
    if (genExtention.hasError(_method))
    {
        callback += ", _deploy_error.getValue()";
    }
    for (auto a : _method->getOutArgs())
    {
        callback += ", _" + a->getName();
        callback += ".getValue()";
    }
    callback += ");\n\t";
    callback += "},\n\t";
    auto out = generateOutParams(_method, _accessor, true);
    if (!error.empty() && !out.empty())
        error += ", ";
    callback += "std::make_tuple(" + error + out + ")";
    return callback;
}

std::string FInterfaceSomeIPProxyGenerator::generateCallbackParameter(
    const std::shared_ptr<BstIdl::FMethod> &_method, const std::shared_ptr<BstIdl::FInterface> &fInterface,
    const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    std::string declaration = "CommonAPI::CallStatus _internalCallStatus";
    if (genExtention.hasError(_method))
        declaration += ", CommonAPI::Deployable< " + genExtention.getErrorType(_method) + ", " +
                       someIPGen.getErrorDeploymentType(_method, false) + " > _deploy_error";
    for (auto a : _method->getOutArgs())
    {
        declaration += ", ";
        declaration += "CommonAPI::Deployable< " + genExtention.getTypeName(a, _method, true) + ", " +
                       someIPGen.getDeploymentType(a, fInterface, true) + " > _" + a->getName();
    }
    return declaration;
}

std::string FInterfaceSomeIPProxyGenerator::someipClassName(const std::shared_ptr<BstIdl::FAttribute> &_attribute,
                                                            const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                                            const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    std::string type = "CommonAPI::SomeIP::";

    if (_attribute->isReadonly())
        type = type + "Readonly";

    type = type + "Attribute<" + genExtention.getClassName(_attribute);
    auto deployment = someIPGen.getDeploymentType(_attribute, fInterface, true);
    if (deployment != "CommonAPI::EmptyDeployment")
        type += ", " + deployment;
    type += ">";

    if (genExtention.isObservable(_attribute))
        type = "CommonAPI::SomeIP::ObservableAttribute<" + type + ">";

    return type;
}

std::string FInterfaceSomeIPProxyGenerator::generateVariableInit(
    const std::shared_ptr<BstIdl::FAttribute> &_attribute, const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
    const std::shared_ptr<BstIdl::FInterface> &fInterface)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    auto init = someipClassVariableName(_attribute) + "(*this";

    if (genExtention.isObservable(_attribute))
    {
        init += ", " + someIPGen.getNotifierEventGroups(_attribute, _accessor).front() + ", " +
                someIPGen.getNotifierIdentifier(_attribute, _accessor);
    }

    init += ", " + someIPGen.getGetterIdentifier(_attribute, _accessor) + ", " +
            someIPGen.isGetterReliable(_attribute, _accessor);
    if (!_attribute->isNoSubscriptions())
    {
        init += ", " + someIPGen.getNotifierReliabilityType(_attribute, _accessor);
    }
    init += ", " + someIPGen.getEndianess(_attribute, _accessor);

    if (!_attribute->isReadonly())
    {
        init += ", " + someIPGen.getSetterIdentifier(_attribute, _accessor) + ", " +
                someIPGen.isSetterReliable(_attribute, _accessor);
    }

    auto deployment = someIPGen.getDeploymentRef(_attribute, _attribute->isArray(), nullptr, fInterface,
                                                 _accessor->getOverwriteAccessor(_attribute));
    if (!deployment.empty())
        init += ", " + deployment;

    init += ")";

    return init;
}

std::string FInterfaceSomeIPProxyGenerator::someipClassName(const std::shared_ptr<BstIdl::FBroadcast> &_broadcast,
                                                            const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                                            const std::shared_ptr<SomeipPropertyAccessor> &_accessor)
{
    auto someIPGen = FrancaSomeIPGeneratorExtensions::getInstance();
    auto genExtention = FrancaGeneratorExtensions::getInstance();
    std::string eventDeclaration = "CommonAPI::SomeIP::";

    eventDeclaration += "Event<" + genExtention.getClassName(_broadcast);
    for (auto a : _broadcast->getOutArgs())
    {
        eventDeclaration += ", ";
        eventDeclaration += someIPGen.getDeployable(a, fInterface, _accessor);
    }
    eventDeclaration += '>';

    return eventDeclaration;
}
} // namespace BstCommonAPI