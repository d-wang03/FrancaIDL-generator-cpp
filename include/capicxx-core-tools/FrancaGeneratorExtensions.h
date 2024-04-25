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
#ifndef FRANCA_GENERATOR_EXTENSIONS_C
#define FRANCA_GENERATOR_EXTENSIONS_C
#include <list>
#include <algorithm>
#include <sys/types.h>
#include <map>
#include "CommonapiPropertyAccessor.h"
#define ROT32(x, y) (x << y) | (x >> (32 - y))
namespace BstCommonAPI
{
class FrancaGeneratorExtensions
{
public:
    FrancaGeneratorExtensions() = default;
    virtual ~FrancaGeneratorExtensions() = default;
    FrancaGeneratorExtensions(const FrancaGeneratorExtensions &) = default;
    FrancaGeneratorExtensions(FrancaGeneratorExtensions &&) noexcept = default;
    FrancaGeneratorExtensions &operator=(const FrancaGeneratorExtensions &) = default;
    FrancaGeneratorExtensions &operator=(FrancaGeneratorExtensions &&) = default;

    static FrancaGeneratorExtensions &getInstance()
    {
        static FrancaGeneratorExtensions ins;
        return ins;
    }

    void insertAccessor(const std::shared_ptr<BstIdl::FTypeCollection> &tc,
                        const std::shared_ptr<CommonapiPropertyAccessor> &accessor)
    {
        m_accessors[tc] = accessor;
    }

    std::shared_ptr<CommonapiPropertyAccessor> getAccessor(const std::shared_ptr<BstIdl::FTypeCollection> &tc);

    std::string getBundleVersion(std::string bundleName);

    std::string getFrancaVersion();

    std::string getCoreVersion();

    std::string getLicenseHeader();

    std::string getCommentedString(std::string str);

    std::string generateCommonApiLicenseHeader();

    std::string getElementName(const std::shared_ptr<BstIdl::FModelElement> &fModelElement);

    std::string getVersionPathPrefix(const std::shared_ptr<BstIdl::FTypeCollection> &_tc);

    std::string getDirectoryPath(const std::shared_ptr<BstIdl::FModel> &fModel);

    std::string getHeaderFile(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection);

    std::string getHeaderPath(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection);

    std::string getSourceFile(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection);

    std::string getSourcePath(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection);

    std::string getProxyBaseHeaderFile(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string getProxyBaseHeaderPath(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string getProxyHeaderFile(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string getProxyHeaderPath(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string getStubSourceFile(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string getStubSourcePath(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string getStubHeaderFile(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string getStubHeaderPath(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string getStubDefaultHeaderFile(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string getSkeletonNamePostfix();

    std::string getStubDefaultHeaderPath(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string getStubDefaultSourceFile(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string getStubDefaultSourcePath(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string someipProxyHeaderFile(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string someipProxyHeaderPath(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string someipProxySourceFile(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string someipProxySourcePath(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    void generateSelectiveBroadcastStubIncludes(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                                std::list<std::string> &generatedHeaders,
                                                std::list<std::string> &libraryHeaders);

    std::list<std::string> splitCamelCase(const std::string &str);

    std::string getDefineName(const std::shared_ptr<BstIdl::FModelElement> &fModelElement);

    std::string getDefineName(const std::shared_ptr<BstIdl::FModel> &model);

    std::string generateVersionNamespaceBegin(const std::shared_ptr<BstIdl::FTypeCollection> &_tc);

    std::string generateNamespaceBeginDeclaration(const std::shared_ptr<BstIdl::FModel> &model);

    bool isTheSameVersion(const std::shared_ptr<BstIdl::FVersion> _mine,
                          const std::shared_ptr<BstIdl::FVersion> _other);

    std::string getVersionPrefix(const std::shared_ptr<BstIdl::FTypeCollection> &_tc);

    std::string getContainerName(const std::shared_ptr<BstIdl::FObject> &_container);

    std::string getFullName(const std::shared_ptr<BstIdl::FObject> &_me);

    std::string getTypeCollectionName(const std::shared_ptr<BstIdl::FTypeCollection> &_me,
                                      const std::shared_ptr<BstIdl::FTypeCollection> &_other);

    std::shared_ptr<BstIdl::FObject> getCommonContainer(const std::shared_ptr<BstIdl::FObject> &_me,
                                                        const std::shared_ptr<BstIdl::FObject> &_other);

    std::string getPartialName(const std::shared_ptr<BstIdl::FModelElement> &_me,
                               const std::shared_ptr<BstIdl::FObject> &_until);

    std::string getElementName(const std::shared_ptr<BstIdl::FModelElement> &_me,
                               const std::shared_ptr<BstIdl::FModelElement> &_other, bool _isOther);

    std::string getFullyQualifiedName(const std::shared_ptr<BstIdl::FModelElement> &fModelElement);

    std::string getPrimitiveTypeName(const std::shared_ptr<BstIdl::FBasicTypeId> &fBasicTypeId);

    bool hasDerivedTypes(const std::shared_ptr<BstIdl::FStructType> &fStructType);

    void getDerivedFStructTypes(const std::shared_ptr<BstIdl::FStructType> &fStructType,
                                std::list<std::shared_ptr<BstIdl::FStructType>> &struct_types);

    std::string getElementType(const std::shared_ptr<BstIdl::FTypeRef> &_typeRef,
                               const std::shared_ptr<BstIdl::FModelElement> &_container, bool _isOther);

    uint64_t getSerialId(const std::shared_ptr<BstIdl::FStructType> &fStructType);

    std::string createSerials(const std::shared_ptr<BstIdl::FStructType> &fStructType);

    std::string generateDummyValue(const std::shared_ptr<BstIdl::FTypeRef> &typeRef);

    bool hasPolymorphicBase(const std::shared_ptr<BstIdl::FStructType> &fStructType);

    std::string getTypeName(const std::shared_ptr<BstIdl::FTypedElement> &_element,
                            const std::shared_ptr<BstIdl::FModelElement> &_source, bool _isOther);

    std::list<std::shared_ptr<BstIdl::FField>> getAllElements(const std::shared_ptr<BstIdl::FUnionType> &_union);

    std::list<std::shared_ptr<BstIdl::FField>> getAllElements(const std::shared_ptr<BstIdl::FStructType> &_struct);

    std::shared_ptr<BstIdl::FInterface> getContainingInterface(
        const std::shared_ptr<BstIdl::FModelElement> &fModelElement);

    std::shared_ptr<BstIdl::FTypeCollection> getContainingTypeCollection(
        const std::shared_ptr<BstIdl::FModelElement> &fModelElement);

    std::string proxyManagerMemberName(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string generateCases(const std::shared_ptr<BstIdl::FStructType> &fStructType,
                              const std::shared_ptr<BstIdl::FModelElement> &parent, bool qualified);

    bool isComplex(std::string _typeName);

    std::string getConstantType(const std::shared_ptr<BstIdl::FInitializerExpression> &expression);

    std::string getConstantValue(const std::shared_ptr<BstIdl::FModelElement> &expression);

    std::string getEnumeratorValue(const std::shared_ptr<BstIdl::FExpression> &expression);

    int64_t getMaximumEnumerationValue(const std::shared_ptr<BstIdl::FEnumerationType> &_enumeration);

    void setEnumerationValues(const std::shared_ptr<BstIdl::FEnumerationType> &_enumeration);

    std::shared_ptr<BstIdl::FBasicTypeId> getBackingType(
        const std::shared_ptr<BstIdl::FEnumerationType> &fEnumerationType,
        const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor);

    std::string getEnumPrefix();

    std::string getBaseType(const std::shared_ptr<BstIdl::FEnumerationType> &_enumeration,
                            const std::shared_ptr<BstIdl::FEnumerationType> &_other, std::string _backingType);

    std::list<std::shared_ptr<BstIdl::FMethod>> getMethodsWithError(
        const std::shared_ptr<BstIdl::FInterface> &_interface);

    std::string getInterfaceVersion(const std::shared_ptr<BstIdl::FInterface> &_interface);

    std::string getFullyQualifiedNameWithVersion(const std::shared_ptr<BstIdl::FInterface> &_interface);

    std::string getFullyQualifiedCppName(const std::shared_ptr<BstIdl::FModelElement> &fModelElement);

    std::string generateNamespaceEndDeclaration(const std::shared_ptr<BstIdl::FModel> &model);

    std::string generateVersionNamespaceEnd(const std::shared_ptr<BstIdl::FTypeCollection> &_tc);

    std::string generateMajorVersionNamespace(const std::shared_ptr<BstIdl::FTypeCollection> &_tc);

    std::string generateDeploymentNamespaceEnd(const std::shared_ptr<BstIdl::FTypeCollection> &_tc);

    std::string generateVariantComparators(const std::shared_ptr<BstIdl::FTypeCollection> &fTypes);

    bool hasAttributes(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    bool hasBroadcasts(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    bool hasSelectiveBroadcasts(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string getProxyBaseClassName(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string getClassName(const std::shared_ptr<BstIdl::FAttribute> &fAttribute);

    std::string getClassName(const std::shared_ptr<BstIdl::FBroadcast> &fBroadcast);

    bool isSignedBackingType(std::string _backingType);

    std::string errorReplyTypes(const std::shared_ptr<BstIdl::FBroadcast> &fBroadcast,
                                const std::shared_ptr<BstIdl::FMethod> &fMethod,
                                const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor);

    std::string doCast(std::string _value, std::string _backingType);

    bool hasError(const std::shared_ptr<BstIdl::FMethod> &fMethod);

    std::string getErrorName(const std::shared_ptr<BstIdl::FEnumerationType> &fMethodErrors);

    std::string getRelativeNameReference(const std::shared_ptr<BstIdl::FModelElement> &destination,
                                         const std::shared_ptr<BstIdl::FObject> &source);

    std::string getErrorNameReference(const std::shared_ptr<BstIdl::FMethod> &fMethod,
                                      const std::shared_ptr<BstIdl::FObject> &source);

    std::string generateASyncTypedefSignature(const std::shared_ptr<BstIdl::FMethod> &fMethod);

    std::string getAsyncCallbackClassName(const std::shared_ptr<BstIdl::FMethod> &fMethod);

    bool needsMangling(const std::shared_ptr<BstIdl::FMethod> &fMethod);

    std::string generateGetMethodDefinition(const std::shared_ptr<BstIdl::FAttribute> &fAttribute);

    std::string generateGetMethodDefinitionWithin(const std::shared_ptr<BstIdl::FAttribute> &fAttribute,
                                                  std::string parentClassName);

    std::string generateGetMethodDefinition(const std::shared_ptr<BstIdl::FBroadcast> &fBroadcast);

    std::string generateGetMethodDefinitionWithin(const std::shared_ptr<BstIdl::FBroadcast> &fBroadcast,
                                                  std::string parentClassName);

    std::string generateAsyncDefinition(const std::shared_ptr<BstIdl::FMethod> &fMethod, bool _isDefault);

    std::string generateAsyncDefinitionWithin(const std::shared_ptr<BstIdl::FMethod> &fMethod,
                                              std::string parentClassName, bool _isDefault);

    std::string generateAsyncDefinitionSignature(const std::shared_ptr<BstIdl::FMethod> &fMethod, bool _isDefault);

    std::string generateDefinitionSignature(const std::shared_ptr<BstIdl::FMethod> &fMethod, bool _isDefault);

    std::string generateDefinition(const std::shared_ptr<BstIdl::FMethod> &fMethod, bool _isDefault);

    std::string generateDefinitionWithin(const std::shared_ptr<BstIdl::FMethod> &fMethod, std::string parentClassName,
                                         bool _isDefault);

    std::string proxyManagerGetterName(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string getProxyClassName(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    bool supportsValidation(const std::shared_ptr<BstIdl::FTypeRef> &fTtypeRef);

    std::string generateDummyArgumentInitializations(const std::shared_ptr<BstIdl::FMethod> &fMethod);

    std::string generateDummyArgumentDefinitions(const std::shared_ptr<BstIdl::FMethod> &fMethod);

    std::string generateDummyArgumentList(const std::shared_ptr<BstIdl::FMethod> &fMethod);

    std::string generateCppNamespace(const std::shared_ptr<BstIdl::FModel> &fModel);

    std::string getStubAdapterClassName(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    bool isObservable(const std::shared_ptr<BstIdl::FAttribute> &fAttribute);

    std::string getStubAdapterClassFireChangedMethodName(const std::shared_ptr<BstIdl::FAttribute> &fAttribute);

    std::string getStubAdapterClassFireSelectiveMethodName(const std::shared_ptr<BstIdl::FBroadcast> &fBroadcast);

    std::string generateFireSelectiveSignatur(const std::shared_ptr<BstIdl::FBroadcast> &fBroadcast,
                                              const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string getStubAdapterClassSendSelectiveMethodName(const std::shared_ptr<BstIdl::FBroadcast> &fBroadcast);

    std::string getSubscribeSelectiveMethodName(const std::shared_ptr<BstIdl::FBroadcast> &fBroadcast);

    std::string getUnsubscribeSelectiveMethodName(const std::shared_ptr<BstIdl::FBroadcast> &fBroadcast);

    bool isErrorType(const std::shared_ptr<BstIdl::FBroadcast> &fBroadcast,
                     const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor);

    bool isErrorType(const std::shared_ptr<BstIdl::FBroadcast> &fBroadcast,
                     const std::shared_ptr<BstIdl::FMethod> &fMethod,
                     const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor);

    std::string getStubAdapterClassFireEventMethodName(const std::shared_ptr<BstIdl::FBroadcast> &fBroadcast);

    std::string stubRegisterManagedMethod(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string stubRegisterManagedName(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string getStubFullClassName(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string getStubClassName(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string stubDeregisterManagedName(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string stubManagedSetGetterName(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string getStubClassLockMethodName(const std::shared_ptr<BstIdl::FAttribute> &fAttribute);

    std::string getStubAdapterClassSubscriberListPropertyName(const std::shared_ptr<BstIdl::FBroadcast> &fBroadcast);

    std::string getStubRemoteEventClassName(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string getStubRemoteEventClassSetMethodName(const std::shared_ptr<BstIdl::FAttribute> &fAttribute);

    std::string getStubRemoteEventClassChangedMethodName(const std::shared_ptr<BstIdl::FAttribute> &fAttribute);

    std::string getStubCommonAPIClassName(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string generateStubReplySignature(const std::shared_ptr<BstIdl::FMethod> &fMethod);

    std::string generateStubErrorReplySignature(const std::shared_ptr<BstIdl::FBroadcast> &fBroadcast,
                                                const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor);

    std::list<std::shared_ptr<BstIdl::FArgument>> errorArgs(
        const std::shared_ptr<BstIdl::FBroadcast> &fBroadcast,
        const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor);

    std::string getStubClassGetMethodName(const std::shared_ptr<BstIdl::FAttribute> &fAttribute);

    std::string generateOverloadedStubSignature(const std::shared_ptr<BstIdl::FMethod> &fMethod,
                                                std::map<std::string, bool> &replies);

    std::string getSubscriptionRequestedMethodName(const std::shared_ptr<BstIdl::FBroadcast> &fBroadcast);

    std::string stubRegisterManagedMethodWithInstanceNumberImpl(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string stubRegisterManagedMethodImpl(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string getHeaderDefineName(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string getStubDefaultClassName(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string stubManagedSetName(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string errorReplyCallbackName(const std::shared_ptr<BstIdl::FBroadcast> &fBroadcast,
                                       const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor);

    std::string generateSendSelectiveSignatur(const std::shared_ptr<BstIdl::FBroadcast> &fBroadcast,
                                              const std::shared_ptr<BstIdl::FInterface> &fInterface, bool withDefault);

    std::string getStubAdapterClassSubscribersMethodName(const std::shared_ptr<BstIdl::FBroadcast> &fBroadcast);

    std::string getSubscriptionChangedMethodName(const std::shared_ptr<BstIdl::FBroadcast> &fBroadcast);

    std::string stubRegisterManagedAutoName(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    bool supportsTypeValidation(const std::shared_ptr<BstIdl::FAttribute> &fAttribute);

    std::string validateType(const std::shared_ptr<BstIdl::FAttribute> &fAttribute,
                             const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string generateBaseRemoteHandlerConstructorsCalls(const std::shared_ptr<BstIdl::FInterface> &_interface);

    std::list<std::shared_ptr<BstIdl::FInterface>> getBaseInterfaces(
        const std::shared_ptr<BstIdl::FInterface> &_interface);

    std::string generateHashers(const std::shared_ptr<BstIdl::FTypeCollection> &fTypes,
                                const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor);

    std::string getFQN(const std::shared_ptr<BstIdl::FType> &type,
                       const std::shared_ptr<BstIdl::FTypeCollection> &fTypes);

    std::string generateHash(const std::shared_ptr<BstIdl::FType> &type,
                             const std::shared_ptr<BstIdl::FTypeCollection> &fTypes,
                             const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor);

    std::string generateIndent(int _indent);

    bool isStructEmpty(const std::shared_ptr<BstIdl::FStructType> &fStructType);

    std::string getRelativeName(const std::shared_ptr<BstIdl::FModelElement> &_element);

    std::string getBitWidth(const std::shared_ptr<BstIdl::FBasicTypeId> &_typeId);

    std::string generateDeploymentNamespaceBegin(const std::shared_ptr<BstIdl::FTypeCollection> &_tc);

    std::string isSigned(std::string _typeName);

    std::string startInternalCompilation();

    std::string endInternalCompilation();

    std::string generateSomeIPBaseInstantiations(const std::shared_ptr<BstIdl::FInterface> &_interface);

    std::string getErrorType(const std::shared_ptr<BstIdl::FMethod> &_method);

    std::list<std::shared_ptr<BstIdl::FModelElement>> getElements(const std::shared_ptr<BstIdl::FInterface> &_iface);

    int getElementPosition(const std::shared_ptr<BstIdl::FInterface> &_iface,
                           const std::shared_ptr<BstIdl::FModelElement> &_elem);

    std::string generateErrorReplyCallbackSignature(
        const std::shared_ptr<BstIdl::FBroadcast> &fBroadcast, const std::shared_ptr<BstIdl::FMethod> &fMethod,
        const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor);

    std::string errorReplyCallbackBindArgs(const std::shared_ptr<BstIdl::FBroadcast> &fBroadcast,
                                           const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor);

    std::string errorName(const std::shared_ptr<BstIdl::FBroadcast> &fBroadcast,
                          const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor);

    std::size_t getRsize(const std::shared_ptr<BstIdl::FInterface> &_iface);

    std::list<std::shared_ptr<BstIdl::FType>> getDirectlyReferencedTypes(const std::shared_ptr<BstIdl::FType> &type);

    void mergeDeployments(const std::shared_ptr<BstIdl::FDInterface> &_source,
                          const std::shared_ptr<BstIdl::FDInterface> &_target);
    void mergeDeployments(const std::shared_ptr<BstIdl::FDTypes> &_source,
                          const std::shared_ptr<BstIdl::FDInterface> &_target);
    void mergeDeployments(const std::shared_ptr<BstIdl::FDTypes> &_source,
                          const std::shared_ptr<BstIdl::FDTypes> &_target);
    void mergeDeploymentsExt(const std::shared_ptr<BstIdl::FDTypes> &_source,
                             const std::shared_ptr<BstIdl::FDTypes> &_target);
    void mergeDeployments(const std::list<std::shared_ptr<BstIdl::FDTypeDefinition>> &_source,
                          std::list<std::shared_ptr<BstIdl::FDTypeDefinition>> &_target);

protected:
    std::list<std::shared_ptr<BstIdl::FUnionType>> getBaseList(const std::shared_ptr<BstIdl::FUnionType> &fUnionType);

    std::list<std::string> getElementTypeNames(const std::shared_ptr<BstIdl::FUnionType> &fUnion);

    std::string generateVariantComnparatorIf(std::list<std::string> list);

    std::string generateComparatorImplementation(const std::shared_ptr<BstIdl::FUnionType> &_derived,
                                                 const std::shared_ptr<BstIdl::FUnionType> &_base);

    std::list<std::string> getNamespaceAsList(const std::shared_ptr<BstIdl::FModel> &fModel);

    std::list<std::string> getNamespaceAsList(const std::shared_ptr<BstIdl::FModelElement> &fModelElement);

    uint32_t hash(const std::string &word);

    uint32_t murmur3_32(const char *key, uint32_t len, uint32_t seed);

    std::string putFTypeObject(const std::shared_ptr<BstIdl::FStructType> &fStructType);

    std::list<std::string> getSubnamespaceList(const std::shared_ptr<BstIdl::FModelElement> &destination,
                                               const std::shared_ptr<BstIdl::FObject> &source);

    std::string getBasicAsyncCallbackClassName(const std::shared_ptr<BstIdl::FMethod> &fMethod);

    std::string getMangledAsyncCallbackClassName(const std::shared_ptr<BstIdl::FMethod> &fMethod);

    std::string getBasicMangledName(const std::shared_ptr<BstIdl::FBasicTypeId> &basicType);

    std::string getDerivedMangledName(const std::shared_ptr<BstIdl::FEnumerationType> &fTypeRef);

    std::string getDerivedMangledName(const std::shared_ptr<BstIdl::FMapType> &fTypeRef);

    std::string getDerivedMangledName(const std::shared_ptr<BstIdl::FStructType> &fTypeRef);

    std::string getDerivedMangledName(const std::shared_ptr<BstIdl::FUnionType> &fTypeRef);

    std::string getDerivedMangledName(const std::shared_ptr<BstIdl::FArrayType> &fTypeRef);

    std::string getDerivedMangledName(const std::shared_ptr<BstIdl::FTypeDef> &fTypeRef);

    std::string getMangledName(const std::shared_ptr<BstIdl::FTypeRef> &fTypeRef);

    std::string generateDummyArgumentInitialization(const std::shared_ptr<BstIdl::FTypeRef> &typeRef,
                                                    const std::shared_ptr<BstIdl::FArgument> &list_element,
                                                    const std::shared_ptr<BstIdl::FMethod> &fMethod);

    std::string generateDummyArgumentInitialization(const std::shared_ptr<BstIdl::FType> &typeRef,
                                                    const std::shared_ptr<BstIdl::FArgument> &list_element,
                                                    const std::shared_ptr<BstIdl::FMethod> &fMethod);

    std::string generateDummyArgumentInitialization(const std::shared_ptr<BstIdl::FBasicTypeId> &basicType);

    std::list<std::shared_ptr<BstIdl::FType>> directlyReferencedTypes;
    void addFTypeDirectlyReferencedTypes(std::list<std::shared_ptr<BstIdl::FType>> &list,
                                         std::shared_ptr<BstIdl::FType> fType);

    std::string getClassNamespaceWithName(const std::shared_ptr<BstIdl::FModelElement> &child, const std::string &name,
                                          const std::shared_ptr<BstIdl::FModelElement> &parent,
                                          const std::string &parentName);

    std::map<std::shared_ptr<BstIdl::FTypeCollection>, std::shared_ptr<CommonapiPropertyAccessor>> m_accessors;
};
} // namespace BstCommonAPI
#endif