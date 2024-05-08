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
#ifndef GENERAL_SERVICE_GENERATOR_EXTENSIONS_H
#define GENERAL_SERVICE_GENERATOR_EXTENSIONS_H
#include <list>
#include <map>
#include <memory>
#include <unordered_map>
/*
#include "asf-tools/ASFPropertyAccessor.h"*/
#include "asf-tools/ASFPropertyAccessor.h"

#include "asf-tools/MsgType.h"
//#include "capicxx-core-tools/CommonapiPropertyAccessor.h"

#define ROT32(x, y) (x << y) | (x >> (32 - y))
namespace BstASF
{

class GeneralServiceGeneratorExtensions
{
public:
    static GeneralServiceGeneratorExtensions &getInstance()
    {
        static GeneralServiceGeneratorExtensions ins;
        return ins;
    }

    void insertSomeipAccessor(const std::shared_ptr<BstIdl::FTypeCollection> &tc,
                              const std::shared_ptr<BstCommonAPI::SomeipPropertyAccessor> &accessor)
    {
        m_someipInterfaceAccessor[tc] = accessor;
    }
    std::shared_ptr<BstCommonAPI::SomeipPropertyAccessor> getSomeipAccessor(
        const std::shared_ptr<BstIdl::FTypeCollection> &tc) const;

    void initAccessor(const std::shared_ptr<BstIdl::FDExtensionRoot> &provider)
    {
        m_accessor = std::make_shared<ASFPropertyAccessor>(std::make_shared<BstIdl::FDeployedProvider>(provider));
    } // SomeipPropertyAccessor
    const std::shared_ptr<ASFPropertyAccessor> getAccessor() const
    {
        return m_accessor;
    }

    void initSomeipProviderAccessor(const std::shared_ptr<BstIdl::FDExtensionRoot> &provider)
    {
        m_someipProviderAccesssor = std::make_shared<BstCommonAPI::SomeipPropertyAccessor>(
            std::make_shared<BstIdl::FDeployedProvider>(provider));
    } // SomeipPropertyAccessor
    const std::shared_ptr<BstCommonAPI::SomeipPropertyAccessor> getSomeipProviderAccessor() const
    {
        return m_someipProviderAccesssor;
    }

    /*
        void insertMainInfs(const std::string &str,
                            const std::shared_ptr<BstIdl::FInterface> &inf)
        {
            m_MainInterfaces[str] = inf;
        }
        std::shared_ptr<BstIdl::FInterface> getMainInf(const std::string &str) const;

        std::shared_ptr<BstIdl::FInterface> getMainInf(const std::shared_ptr<BstIdl::FInterface> &inf) const;

        bool ifMainInf(const std::shared_ptr<BstIdl::FInterface> &inf) const;

        bool ifMainInf(const std::shared_ptr<BstIdl::FDExtensionElement> &instance) const;

    */
    void insertMethodRepliesMap(const std::shared_ptr<BstIdl::FMethod> &md, const std::map<std::string, bool> &replies)
    {
        m_allMethodRepliesMap[md] = replies;
    }

    std::unordered_map<std::shared_ptr<BstIdl::FMethod>, std::map<std::string, bool>> &getMethodRepliesMap()
    {
        return m_allMethodRepliesMap;
    }

    std::map<std::string, bool> getMethodReplies(const std::shared_ptr<BstIdl::FMethod> &md) const;

    std::string getElementName(const std::shared_ptr<BstIdl::FModelElement> &fModelElement) const;
    std::string getInstanceName(const std::shared_ptr<BstIdl::FDExtensionElement> &instance) const;

    bool isErrorType(const std::shared_ptr<BstIdl::FBroadcast> &fBroadcast,
                     const std::shared_ptr<BstIdl::FMethod> &fMethod,
                     const std::shared_ptr<BstCommonAPI::CommonapiPropertyAccessor> &deploymentAccessor);
    // comment
    std::string generateASFLicenseHeader();
    std::string getASFVersion();
    void mergeDeployments(const std::shared_ptr<BstIdl::FDInterface> &_source,
                          const std::shared_ptr<BstIdl::FDInterface> &_target);
    void mergeDeployments(const std::shared_ptr<BstIdl::FDTypes> &_source,
                          const std::shared_ptr<BstIdl::FDTypes> &_target);
    // ifdef
    std::list<std::string> splitCamelCase(const std::string &str);
    std::string getDefineName(const std::shared_ptr<BstIdl::FModelElement> &fModelElement);
    std::string getDefineName(const std::shared_ptr<BstIdl::FModel> &model);
    std::string getProjectDefineName(const std::shared_ptr<BstIdl::FModelElement> &fModelElement);
    std::string getProjectDefineName(const std::shared_ptr<BstIdl::FDExtensionElement> &fdExtensionRoot);
    // namespace & file & path
    std::string generateOverloadedStubSignatureWithNamespace(const std::shared_ptr<BstIdl::FMethod> &fMethod,
                                                             const std::map<std::string, bool> &replies) const;
    std::string generateOverloadedStubSignatureTypeWithNamespace(const std::shared_ptr<BstIdl::FMethod> &fMethod,
                                                                 const std::map<std::string, bool> &replies) const;
    std::string generateOverloadedStubSignature(const std::shared_ptr<BstIdl::FMethod> &fMethod,
                                                const std::map<std::string, bool> &replies) const;
    std::string generateOverloadedStubParaNames(const std::shared_ptr<BstIdl::FMethod> &fMethod,
                                                const std::map<std::string, bool> &replies) const;
    std::string getStubImplHeaderFile(const std::shared_ptr<BstIdl::FInterface> &fInterface);
    std::string getStubImplHeaderPath(const std::shared_ptr<BstIdl::FInterface> &fInterface);
    std::string getStubImplSourceFile(const std::shared_ptr<BstIdl::FInterface> &fInterface);
    std::string getStubImplSourcePath(const std::shared_ptr<BstIdl::FInterface> &fInterface);
    std::string getStubImplHeaderFile(const std::shared_ptr<BstIdl::FDExtensionElement> &instance);
    std::string getStubImplHeaderPath(const std::shared_ptr<BstIdl::FDExtensionElement> &instance);
    std::string getStubImplSourceFile(const std::shared_ptr<BstIdl::FDExtensionElement> &instance);
    std::string getStubImplSourcePath(const std::shared_ptr<BstIdl::FDExtensionElement> &instance);
    std::string generateStubMethodDefine(const std::shared_ptr<BstIdl::FMethod> &fMethod,
                                         const std::map<std::string, bool> &replies, const std::string &serverName,
                                         const std::shared_ptr<BstIdl::FDExtensionElement> &instance);
    // logic
    std::string getLogicHeaderFile(const std::shared_ptr<BstIdl::FInterface> &fInterface);
    std::string getLogicHeaderPath(const std::shared_ptr<BstIdl::FInterface> &fInterface);
    std::string getLogicSourceFile(const std::shared_ptr<BstIdl::FInterface> &fInterface);
    std::string getLogicSourcePath(const std::shared_ptr<BstIdl::FInterface> &fInterface);
    std::string getLogicHeaderFile(const std::shared_ptr<BstIdl::FDExtensionElement> &instance);
    std::string getLogicHeaderPath(const std::shared_ptr<BstIdl::FDExtensionElement> &instance);
    std::string getLogicSourceFile(const std::shared_ptr<BstIdl::FDExtensionElement> &instance);
    std::string getLogicSourcePath(const std::shared_ptr<BstIdl::FDExtensionElement> &instance);

    // new logicDefault
    std::string getLogicDefaultHeaderFile(const std::shared_ptr<BstIdl::FInterface> &fInterface);
    std::string getLogicDefaultHeaderPath(const std::shared_ptr<BstIdl::FInterface> &fInterface);
    std::string getLogicDefaultSourceFile(const std::shared_ptr<BstIdl::FInterface> &fInterface);
    std::string getLogicDefaultSourcePath(const std::shared_ptr<BstIdl::FInterface> &fInterface);
    // ccapiconfig & main
    std::string getCapiConfigHeaderFile();
    std::string getCapiConfigHeaderPath();
    std::string getCapiConfigSourceFile();
    std::string getCapiConfigSourcePath();
    std::string getMainSourcePath();
    std::string getMainSourceFile();
    std::string getTypeConversionHeaderFile();
    std::string getTypeConversionPath();

    // className
    std::string getStubImplClassName(const std::shared_ptr<BstIdl::FInterface> &fInterface);
    std::string getLogicDefaultClassName(const std::shared_ptr<BstIdl::FInterface> &fInterface);
    std::string getStubDefaultClassName(const std::shared_ptr<BstIdl::FInterface> &fInterface);
    std::string getLogicClassName(const std::shared_ptr<BstIdl::FInterface> &fInterface);
    std::string getConfigClassName();
    std::string getLogicClassName(const std::shared_ptr<BstIdl::FDExtensionElement> &instance);
    std::string getStubImplClassName(const std::shared_ptr<BstIdl::FDExtensionElement> &instance);

    std::string getStubDefaultHeaderPath(const std::shared_ptr<BstIdl::FInterface> &fInterface);
    std::string getVersionPathPrefix(const std::shared_ptr<BstIdl::FTypeCollection> &_tc);
    // std::shared_ptr<BstIdl::FVersion> getRVersion(std::shared_ptr<BstIdl::FObject> element) const;
    std::string getDirectoryPath(const std::shared_ptr<BstIdl::FModel> &fModel);
    std::string getStubDefaultHeaderFile(const std::shared_ptr<BstIdl::FInterface> &fInterface);
    std::string getStubSourceFile(const std::shared_ptr<BstIdl::FInterface> &fInterface);
    std::string getStubHeaderFile(const std::shared_ptr<BstIdl::FInterface> &fInterface);
    std::string getHeaderPath(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection);
    std::string getHeaderFile(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection);
    void generateRequiredTypeIncludes(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                      std::list<std::string> &generatedHeaders, std::list<std::string> &libraryHeaders);
    // get config file & path
    std::string getStarUpConfigJsonFile(const std::shared_ptr<BstIdl::FDExtensionRoot> &provider);
    std::string getStarUpSystemdConfigFile(const std::shared_ptr<BstIdl::FDExtensionRoot> &provider);
    std::string getStarUpSLMConfigFile(const std::shared_ptr<BstIdl::FDExtensionRoot> &provider);
    // namespace
    std::string generateNamespaceBeginDeclaration(const std::shared_ptr<BstIdl::FModel> &model);

    // root
    void getSlotList(const std::shared_ptr<BstIdl::FInterface> &fInterface);
    std::string getSlots(const std::shared_ptr<BstIdl::FInterface> &fInterface);
    std::string getDeclSlots(const std::shared_ptr<BstIdl::FInterface> &fInterface);
    std::string getConnectSlots(const std::shared_ptr<BstIdl::FInterface> &fInterface);
    std::string getSlotFunctions(const std::shared_ptr<BstIdl::FInterface> &fInterface);
    std::list<std::string> getSlotInMap(const std::shared_ptr<BstIdl::FInterface> &fInterface);
    // servers
    void clearServer();
    const bool isEmptyServer() const;
    void addServer(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                   const std::shared_ptr<BstIdl::FDExtensionElement> &instance);
    bool isServer(const std::shared_ptr<BstIdl::FInterface> &inf);
    std::list<std::shared_ptr<BstIdl::FDExtensionElement>> getServer(
        const std::shared_ptr<BstIdl::FInterface> &fInterface);
    const std::multimap<std::shared_ptr<BstIdl::FInterface>, std::shared_ptr<BstIdl::FDExtensionElement>>
        &getAllServers() const
    {
        return m_servers;
    }
    // etIntefaces
    std::list<std::shared_ptr<BstIdl::FInterface>> getServerInterfaces() const;
    const std::list<std::shared_ptr<BstIdl::FInterface>> getServerIncludingBaseInterfaces() const;
    std::list<std::shared_ptr<BstIdl::FDExtensionElement>> getServerInstances() const;
    std::list<std::shared_ptr<BstIdl::FDExtensionElement>> getClientInstances() const;

    std::string getDomainName(const std::shared_ptr<BstIdl::FDExtensionElement> &instance);
    std::string getInstanceID(const std::shared_ptr<BstIdl::FDExtensionElement> &instance);
    std::string getSomeIPDomain(const std::shared_ptr<BstIdl::FDExtensionElement> &instance);
    std::string getSomeIPInstanceId(const std::shared_ptr<BstIdl::FDExtensionElement> &instance);
    std::string getServerName(const std::shared_ptr<BstIdl::FDExtensionElement> &instance);
    // method
    std::string getMethodDecl(const std::shared_ptr<BstIdl::FMethod> &md);
    std::string getMethodSeletcorFuncPtrDecl(const std::shared_ptr<BstIdl::FMethod> &md);
    std::string getMethodDefine(const std::shared_ptr<BstIdl::FMethod> &md, const std::string &class_name);
    std::string getLogicMethodDefine(const std::shared_ptr<BstIdl::FMethod> &md, const std::string &class_name);
    std::string getBroadcastDecl(const std::shared_ptr<BstIdl::FBroadcast> &bc);
    std::string getBroadcastDefine(const std::shared_ptr<BstIdl::FBroadcast> &bc, const std::string &class_name);
    std::string getLogicClassFireBroadcastName(const std::shared_ptr<BstIdl::FBroadcast> &fBroadcast);
    std::string getSetMethodDeclStr(const std::shared_ptr<BstIdl::FAttribute> &attr);
    std::string getSetMethodDefineStr(const std::shared_ptr<BstIdl::FAttribute> &attr, const std::string &class_name);
    std::string getGetMethodDeclStr(const std::shared_ptr<BstIdl::FAttribute> &attr);
    std::string getGetMethodDefineStr(const std::shared_ptr<BstIdl::FAttribute> &attr, const std::string &class_name);

    std::string getTypeName(const std::shared_ptr<BstIdl::FTypedElement> &_element,
                            const std::shared_ptr<BstIdl::FModelElement> &_source, const bool _isOther) const;
    std::string getType(const std::string &slot);

    std::string getFullName(const std::shared_ptr<BstIdl::FObject> &_me);

    std::string getElementName(const std::shared_ptr<BstIdl::FModelElement> &fModelElement);

    std::string getSlotMethodName(const std::shared_ptr<BstIdl::FInterface> fInterface, const std::string &slot);

    // record & simulate
    bool isRecordAndSimulateInterface(const std::shared_ptr<BstIdl::FInterface> &fInterface) const;
    void addRecordAndSimulateInterface(const std::shared_ptr<BstIdl::FDExtensionElement> &instance);
    const std::list<std::shared_ptr<BstIdl::FInterface>> &getRSInterfaces() const
    {
        return m_rsInterfaces;
    }
    // void setMethodName(const std::shared_ptr<BstIdl::FInterface> &fInterface, std::list<std::string> &method_names);
    // bool addRecordAndSimulateMethod(std::shared_ptr<BstIdl::FInterface> &fInterface);
    // void createRecordMethods(std::shared_ptr<BstIdl::FInterface> &fInterface);
    // record in logic
    std::string getRecordDefineInLogic(const std::shared_ptr<BstIdl::FMethod> &fMethod);
    // record in stubImpl
    std::string getRecordDefineInStubImpl(const std::shared_ptr<BstIdl::FMethod> &fMethod);
    std::string getStartRecordDefineInStubImpl(const std::shared_ptr<BstIdl::FMethod> &fMethod);
    std::string getStopRecordDefineInStubImpl(const std::shared_ptr<BstIdl::FMethod> &fMethod);
    std::string getReadRecordDefineInStubImpl(const std::shared_ptr<BstIdl::FMethod> &fMethod);
    // client server instance
    bool isServerTypeInstance(const std::shared_ptr<BstIdl::FDExtensionElement> &instance) const;
    std::list<std::shared_ptr<BstIdl::FInterface>> getServerTypeInterfacesIncludingBase() const;
    std::list<std::shared_ptr<BstIdl::FInterface>> getServerTypeInterfaces() const;
    bool isClientTypeInstance(const std::shared_ptr<BstIdl::FDExtensionElement> &instance) const;
    std::list<std::shared_ptr<BstIdl::FInterface>> getClientTypeInterfacesIncludingBase() const;
    bool hasUserLogic(const std::shared_ptr<BstIdl::FDExtensionElement> &instance) const;
    std::shared_ptr<BstIdl::FDInterface> getSomeIpFDInterface(
        const std::shared_ptr<BstIdl::FDExtensionElement> &instance) const;
    bool isFireAndForgetMethod(const std::shared_ptr<BstIdl::FMethod> &method) const;
    // protect module
    const bool isProjectMode() const
    {
        return project_mode;
    }
    void setProjectMode()
    {
        project_mode = true;
    }
    // gen stub
    const bool isGenStub() const
    {
        return geb_stub;
    }
    void setGenStub()
    {
        geb_stub = true;
    }
    const std::list<std::string> record_simulate_methods = {"startRecord", "stopRecord", "readRecord"};

    // vsomeip extensions
    std::string isReliable(const std::shared_ptr<BstIdl::FBroadcast> &_broadcast);
    std::string isReliable(const std::shared_ptr<BstIdl::FMethod> &_method);
    std::string isNotifierReliable(const std::shared_ptr<BstIdl::FAttribute> &_attribute);
    std::string getServerHostMessage(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                     const std::shared_ptr<ASFPropertyAccessor> &_accessor);

    std::string getClientHostMessage(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                     const std::shared_ptr<ASFPropertyAccessor> &_accessor);

    std::string getLogging(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                           const std::shared_ptr<ASFPropertyAccessor> &_accessor);

    std::string getServiceDiscovery(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                    const std::shared_ptr<ASFPropertyAccessor> &_accessor);

    std::string getApplications(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                const std::shared_ptr<ASFPropertyAccessor> &_accessor);
    std::list<std::string> getApplication(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                          const std::shared_ptr<ASFPropertyAccessor> &_accessor);
    // vsomeip services
    std::string getServices(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                            const std::shared_ptr<BstIdl::FDExtensionRoot> &_someipProvider,
                            const std::shared_ptr<ASFPropertyAccessor> &_accessor);

    std::list<std::string> getService(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                      const std::shared_ptr<BstIdl::FDExtensionRoot> &_someipProvider,
                                      const std::shared_ptr<ASFPropertyAccessor> &_accessor);

    std::string getEvents(const std::shared_ptr<BstIdl::FDExtensionElement> &_instance,
                          const std::shared_ptr<BstCommonAPI::SomeipPropertyAccessor> &_accessor);

    std::list<std::string> getEvent(const std::shared_ptr<BstIdl::FDExtensionElement> &_instance,
                                    const std::shared_ptr<BstCommonAPI::SomeipPropertyAccessor> &_accessor);

    std::string getEventGroups(const std::shared_ptr<BstIdl::FDExtensionElement> &_instance,
                               const std::shared_ptr<BstCommonAPI::SomeipPropertyAccessor> &_accessor);

    std::list<std::string> getEventGroup(const std::shared_ptr<BstIdl::FDExtensionElement> &_instance,
                                         const std::shared_ptr<BstCommonAPI::SomeipPropertyAccessor> &_accessor);

    std::string getEventID(const std::shared_ptr<BstIdl::FBroadcast> &_broadcast,
                           const std::shared_ptr<BstCommonAPI::SomeipPropertyAccessor> &_accessor);

    std::list<std::string> getEventGroupsID(const std::shared_ptr<BstIdl::FBroadcast> &_broadcast,
                                            const std::shared_ptr<BstCommonAPI::SomeipPropertyAccessor> &_accessor);

    std::string getNotifierID(const std::shared_ptr<BstIdl::FAttribute> &_attribute,
                              const std::shared_ptr<BstCommonAPI::SomeipPropertyAccessor> &_accessor);

    std::list<std::string> getNotifierEventGroupsID(
        const std::shared_ptr<BstIdl::FAttribute> &_attribute,
        const std::shared_ptr<BstCommonAPI::SomeipPropertyAccessor> &_accessor);

protected:
    // std::unordered_map<std::string, std::shared_ptr<BstIdl::FInterface>> m_MainInterfaces;
    std::unordered_map<std::shared_ptr<BstIdl::FTypeCollection>, std::shared_ptr<BstCommonAPI::SomeipPropertyAccessor>>
        m_someipInterfaceAccessor;
    std::shared_ptr<BstCommonAPI::SomeipPropertyAccessor> m_someipProviderAccesssor;
    std::shared_ptr<BstASF::ASFPropertyAccessor> m_accessor;
    std::unordered_map<std::shared_ptr<BstIdl::FMethod>, std::map<std::string, bool>> m_allMethodRepliesMap;
    std::multimap<std::shared_ptr<BstIdl::FInterface>, std::shared_ptr<BstIdl::FDExtensionElement>> m_servers;
    std::unordered_map<std::shared_ptr<BstIdl::FInterface>, std::list<std::string>> m_slotsMap;
    // record &simulate = rs
    std::list<std::shared_ptr<BstIdl::FInterface>> m_rsInterfaces;

private:
    GeneralServiceGeneratorExtensions() = default;
    virtual ~GeneralServiceGeneratorExtensions() = default;
    GeneralServiceGeneratorExtensions(const GeneralServiceGeneratorExtensions &) = default;
    GeneralServiceGeneratorExtensions(GeneralServiceGeneratorExtensions &&) noexcept = default;
    GeneralServiceGeneratorExtensions &operator=(const GeneralServiceGeneratorExtensions &) = delete;
    GeneralServiceGeneratorExtensions &operator=(GeneralServiceGeneratorExtensions &&) = delete;

    std::string getSlotName(const std::string &msgType, const std::shared_ptr<BstIdl::FModelElement> &obj,
                            const std::shared_ptr<BstIdl::FInterface> fInterface);
    bool hasRecordMethod(const std::shared_ptr<BstIdl::FInterface> &fInterface);
    bool hasRecordFDMethod(const std::shared_ptr<BstIdl::FDExtensionElement> &instance);
    const bool isCorrectRecordMethod(const std::shared_ptr<BstIdl::FMethod> &fMethod);
    const bool isCorrectRecordFDMethod(const std::shared_ptr<BstIdl::FDMethod> &fdMethod);
    const std::string getFDMethodName(const std::shared_ptr<BstIdl::FDMethod> &fdMethod);
    std::string getUserLogicName(const std::shared_ptr<BstIdl::FDExtensionElement> &instance) const;
    std::string getUserLogicHeaderPath(const std::shared_ptr<BstIdl::FDExtensionElement> &instance) const;

    bool project_mode = false;
    bool geb_stub = false;
    // const std::list<std::string> record_simulate_methods = {"startRecord", "stopRecord", "readRecord"};
};

} // namespace BstASF
#endif
