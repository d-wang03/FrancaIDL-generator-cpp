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
#ifndef FRANCA_GENERATOR_EXTENSIONS
#define FRANCA_GENERATOR_EXTENSIONS
#include "model/FFactory.h"
#include "model/FModelManager.h"
#include <list>
#include <vector>

namespace BstIdl
{
class FrancaGeneratorExtensions
{
private:
    std::list<std::shared_ptr<FType>> directlyReferencedTypes;
    void addFTypeDirectlyReferencedTypes(std::list<std::shared_ptr<FType>> &list, std::shared_ptr<FType> fType);

    std::list<std::string> getNamespaceAsList(const std::shared_ptr<FModel> &fModel);

    std::list<std::string> getNamespaceAsList(const std::shared_ptr<FModelElement> &fModelElement);

    int64_t getMaximumEnumerationValue(const std::shared_ptr<FEnumerationType> &_enumeration);

    std::list<std::string> getSubnamespaceList(const std::shared_ptr<FModelElement> &destination,
                                               const std::shared_ptr<FObject> &source);

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
    std::list<std::shared_ptr<FMethod>> getMethodsWithError(const std::shared_ptr<FInterface> &_interface);

    std::string doCast(std::string _value, std::string _backingType);

    bool isSignedBackingType(std::string _backingType);

    std::string getConstantValue(const std::shared_ptr<FModelElement> &expression);

    std::string getConstantType(const std::shared_ptr<FInitializerExpression> &expression);

    void setEnumerationValues(const std::shared_ptr<FEnumerationType> &_enumeration);

    std::shared_ptr<FBasicTypeId> getBackingType(const std::shared_ptr<FEnumerationType> &fEnumerationType);

    std::string getFullyQualifiedName(const std::shared_ptr<FModelElement> &fModelElement);

    std::string getFullyQualifiedCppName(const std::shared_ptr<FModelElement> &fModelElement);

    std::string getInterfaceVersion(const std::shared_ptr<FInterface> &_interface);

    std::string getFullyQualifiedNameWithVersion(const std::shared_ptr<FInterface> &_interface);

    std::list<std::shared_ptr<FType>> getDirectlyReferencedTypes(const std::shared_ptr<FType> &type);

    std::string getDefineName(const std::shared_ptr<FModelElement> &fModelElement);

    std::string getDefineName(const std::shared_ptr<FModel> &model);

    std::string generateDummyValue(const std::shared_ptr<FTypeRef> &typeRef);

    std::list<std::string> splitCamelCase(const std::string &str);

    std::string getEnumPrefix();

    std::string getTypeName(const std::shared_ptr<FTypedElement> &_element,
                            const std::shared_ptr<FModelElement> &_source, bool _isOther);

    std::string generateBSTLicenseHeader();

    std::string getCommentedString(std::string str);

    std::string getLicenseHeader();

    std::string getCoreVersion();

    std::string getFrancaVersion();

    std::string getBundleVersion(std::string bundleName);

    std::string getVersionPathPrefix(const std::shared_ptr<FTypeCollection> &_tc);

    std::string getDirectoryPath(const std::shared_ptr<FModel> &fModel);

    std::string getHeaderFile(const std::shared_ptr<FTypeCollection> &fTypeCollection);

    std::string getInstanceHeaderFile(const std::shared_ptr<FTypeCollection> &fTypeCollection);

    std::string getHeaderPath(const std::shared_ptr<FTypeCollection> &fTypeCollection);

    std::string getSourceFile(const std::shared_ptr<FTypeCollection> &fTypeCollection);

    std::string getSourcePath(const std::shared_ptr<FTypeCollection> &fTypeCollection);

    std::string getProxyHeaderFile(const std::shared_ptr<FInterface> &fInterface);

    std::string getProxyHeaderPath(const std::shared_ptr<FInterface> &fInterface);

    std::string getProxySourceFile(const std::shared_ptr<FInterface> &fInterface);

    std::string getProxySourcePath(const std::shared_ptr<FInterface> &fInterface);

    std::string getStubHeaderFile(const std::shared_ptr<FInterface> &fInterface);

    std::string getStubSourcePath(const std::shared_ptr<FInterface> &fInterface);

    std::string getStubSourceFile(const std::shared_ptr<FInterface> &fInterface);

    bool hasPolymorphicBase(const std::shared_ptr<FStructType> &fStructType);

    bool hasDerivedTypes(const std::shared_ptr<FStructType> &fStructType);

    void getDerivedFStructTypes(const std::shared_ptr<FStructType> &fStructType,
                                std::list<std::shared_ptr<FStructType>> &struct_types);

    std::string generateVersionNamespaceBegin(const std::shared_ptr<FTypeCollection> &_tc);

    std::string generateVersionNamespaceEnd(const std::shared_ptr<FTypeCollection> &_tc);

    std::string generateNamespaceBeginDeclaration(const std::shared_ptr<FModel> &model);

    std::string generateNamespaceEndDeclaration(const std::shared_ptr<FModel> &model);

    std::string generateMajorVersionNamespace(const std::shared_ptr<FTypeCollection> &_tc);

    bool isTheSameVersion(const std::shared_ptr<FVersion> _mine, std::shared_ptr<FVersion> _other);

    std::string getTypeCollectionName(const std::shared_ptr<FTypeCollection> &_me,
                                      const std::shared_ptr<FTypeCollection> &_other);

    std::string getContainerName(const std::shared_ptr<FObject> &_container);

    std::string getPrimitiveTypeName(const std::shared_ptr<FBasicTypeId> &fBasicTypeId);

    std::shared_ptr<FObject> getCommonContainer(const std::shared_ptr<FObject> &_me,
                                                const std::shared_ptr<FObject> &_other);

    std::string getElementType(const std::shared_ptr<FTypeRef> &_typeRef,
                               const std::shared_ptr<FModelElement> &_container, bool _isOther);

    std::string getElementName(const std::shared_ptr<BstIdl::FModelElement> &fModelElement);

    std::string getElementName(const std::shared_ptr<FModelElement> &_me, const std::shared_ptr<FModelElement> &_other,
                               bool _isOther);

    std::string getPartialName(const std::shared_ptr<FModelElement> &_me, const std::shared_ptr<FObject> &_until);

    std::string getFullName(const std::shared_ptr<FObject> &_me);

    std::string getVersionPrefix(const std::shared_ptr<FTypeCollection> &_tc);

    std::string getClassName(const std::shared_ptr<FAttribute> &fAttribute);

    std::string getClassName(const std::shared_ptr<FBroadcast> &fBroadcast);

    std::string getStubClassName(const std::shared_ptr<FInterface> &fInterface);

    std::string generateBroadcastCallback(const std::shared_ptr<FBroadcast> &_broadcast);

    std::string getStubHeaderPath(const std::shared_ptr<FInterface> &fInterface);

    void generateSelectiveBroadcastStubIncludes(const std::shared_ptr<FInterface> &fInterface,
                                                std::list<std::string> &generatedHeaders,
                                                std::list<std::string> &libraryHeaders);

    std::string generateGetMethodDefinition(const std::shared_ptr<FAttribute> &fAttribute);

    std::string generateSetMethodDefinition(const std::shared_ptr<FAttribute> &fAttribute);

    std::string generateSetCallbackDefinition(const std::shared_ptr<FBroadcast> &fBroadcast);

    std::string generateDefinition(const std::shared_ptr<FMethod> &fMethod, bool _isDefault);

    std::string generateDefinitionWithin(const std::shared_ptr<FMethod> &fMethod, std::string parentClassName,
                                         bool _isDefault);

    std::string generateDefinitionSignature(const std::shared_ptr<FMethod> &fMethod);

    bool hasError(const std::shared_ptr<FMethod> &fMethod);

    std::string getErrorNameReference(const std::shared_ptr<FMethod> &fMethod, const std::shared_ptr<FObject> &source);

    std::string getErrorName(const std::shared_ptr<FEnumerationType> &fMethodErrors);

    std::string getCallbackName(const std::shared_ptr<FBroadcast> &fBroadcast);

    std::string getRelativeNameReference(const std::shared_ptr<FModelElement> &destination,
                                         const std::shared_ptr<FObject> &source);

    std::string generateGetMethodImpl(const std::shared_ptr<FAttribute> &fAttribute, std::string className);

    std::string generateSetMethodImpl(const std::shared_ptr<FAttribute> &fAttribute, std::string className);

    std::string generateSetCallbackImpl(const std::shared_ptr<FBroadcast> &fBroadcast, std::string className);

    std::string generateMethodImpl(const std::shared_ptr<FMethod> &fMethod, std::string parentClassName);

    std::string generateArgList(const std::shared_ptr<FMethod> &fMethod);

    std::string generateFireBroadcastDefinition(const std::shared_ptr<FBroadcast> &fBroadcast);

    std::string getStubAdapterClassFireEventMethodName(const std::shared_ptr<FBroadcast> &fBroadcast);

    std::list<std::shared_ptr<FField>> getAllElements(const std::shared_ptr<FUnionType> &_union);

    std::list<std::shared_ptr<FField>> getAllElements(const std::shared_ptr<FStructType> &_struct);
};
} // namespace BstIdl
#endif