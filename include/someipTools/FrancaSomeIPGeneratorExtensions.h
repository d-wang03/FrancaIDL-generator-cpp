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
#ifndef FRANCA_SOME_I_P_GENERATOR_EXTENSIONS
#define FRANCA_SOME_I_P_GENERATOR_EXTENSIONS
#include "capicxx-core-tools/FrancaGeneratorExtensions.h"
#include "someipTools/SomeipPropertyAccessor.h"
#include <algorithm>
#include <dirent.h>
#include <fstream>
#include <list>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>
namespace BstCommonAPI
{
class FrancaSomeIPGeneratorExtensions : public FrancaGeneratorExtensions
{

public:
    const int SOMEIP_DEFAULT_MIN_LENGTH = 0;
    const int SOMEIP_DEFAULT_MAX_LENGTH = 0;
    const int SOMEIP_DEFAULT_LENGTH_WIDTH = 4;
    const int SOMEIP_DEFAULT_STRING_LENGTH = 0;
    const int SOMEIP_DEFAULT_ENUM_LENGTH_WIDTH = 1;
    const int SOMEIP_DEFAULT_STRUCT_LENGTH_WIDTH = 0;
    const int SOMEIP_DEFAULT_UNION_TYPE_WIDTH = 4;
    const bool SOMEIP_DEFAULT_UNION_DEFAULT_ORDER = true;
    const int SOMEIP_DEFAULT_ENUM_WIDTH = 1;
    const SomeIpStringEncoding SOMEIP_DEFAULT_STRING_ENCODING = SomeIpStringEncoding::utf8;

    FrancaSomeIPGeneratorExtensions() = default;
    virtual ~FrancaSomeIPGeneratorExtensions() = default;
    FrancaSomeIPGeneratorExtensions(const FrancaSomeIPGeneratorExtensions &) = default;
    FrancaSomeIPGeneratorExtensions(FrancaSomeIPGeneratorExtensions &&) noexcept = default;
    FrancaSomeIPGeneratorExtensions &operator=(const FrancaSomeIPGeneratorExtensions &) = default;
    FrancaSomeIPGeneratorExtensions &operator=(FrancaSomeIPGeneratorExtensions &&) = default;

    static FrancaSomeIPGeneratorExtensions &getInstance()
    {
        static FrancaSomeIPGeneratorExtensions instance;
        return instance;
    }

    std::shared_ptr<SomeipPropertyAccessor> getSomeIpAccessor(const std::shared_ptr<BstIdl::FTypeCollection> &_tc);

    std::string someipDeploymentHeaderFile(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string someipDeploymentHeaderPath(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string someipDeploymentSourceFile(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string someipDeploymentSourcePath(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string someipDeploymentHeaderFile(const std::shared_ptr<BstIdl::FTypeCollection> &_tc);

    std::string someipDeploymentHeaderPath(const std::shared_ptr<BstIdl::FTypeCollection> &_tc);

    std::string someipDeploymentSourceFile(const std::shared_ptr<BstIdl::FTypeCollection> &_tc);

    std::string someipDeploymentSourcePath(const std::shared_ptr<BstIdl::FTypeCollection> &_tc);

    std::string someipProxyHeaderFile(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string someipProxyHeaderPath(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string someipProxySourceFile(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string someipProxySourcePath(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string getSomeIPVersion();

    std::string generateCommonApiSomeIPLicenseHeader();

    std::shared_ptr<SomeipPropertyAccessor> getSpecificAccessor(const std::shared_ptr<BstIdl::FObject> &_obj);

    int getSomeIpArrayMaxLengthHelper(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                      const std::shared_ptr<BstIdl::FObject> &_obj);

    int getSomeIpArrayMinLengthHelper(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                      const std::shared_ptr<BstIdl::FObject> &_object);

    int getSomeIpArrayLengthWidthHelper(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                        const std::shared_ptr<BstIdl::FObject> &_obj);

    int getSomeIpMapMinLengthHelper(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                    const std::shared_ptr<BstIdl::FObject> &_obj);

    int getSomeIpMapMaxLengthHelper(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                    const std::shared_ptr<BstIdl::FObject> &_obj);

    int getSomeIpUnionLengthWidthHelper(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                        const std::shared_ptr<BstIdl::FObject> &_obj);

    int getSomeIpMapLengthWidthHelper(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                      const std::shared_ptr<BstIdl::FObject> &_obj);

    int getSomeIpUnionTypeWidthHelper(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                      const std::shared_ptr<BstIdl::FObject> &_obj);

    int getSomeIpEnumBitWidthHelper(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                    const std::shared_ptr<BstIdl::FObject> &_obj);

    bool getSomeIpUnionDefaultOrderHelper(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                          const std::shared_ptr<BstIdl::FObject> &_obj);

    int getSomeIpUnionMaxLengthHelper(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                      const std::shared_ptr<BstIdl::FObject> &_obj);

    int getSomeIpStructLengthWidthHelper(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                         const std::shared_ptr<BstIdl::FObject> &_obj);

    int getSomeIpEnumWidthHelper(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                 const std::shared_ptr<BstIdl::FObject> &_obj);

    int getSomeIpEnumInvalidValueHelper(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                        const std::shared_ptr<BstIdl::FObject> &_obj);

    int getSomeIpIntegerBitWidthHelper(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                       const std::shared_ptr<BstIdl::FObject> &_obj);

    int getSomeIpIntegerInvalidValueHelper(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                           const std::shared_ptr<BstIdl::FObject> &_obj);

    bool hasSomeIpArrayMinLength(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                 const std::shared_ptr<BstIdl::FObject> &_object);

    bool hasSomeIpArrayMaxLength(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                 const std::shared_ptr<BstIdl::FObject> &_object);

    bool hasSomeIpArrayLengthWidth(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                   const std::shared_ptr<BstIdl::FObject> &_object);

    bool hasSomeIpByteBufferMinLength(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                      const std::shared_ptr<BstIdl::FObject> &_object);

    bool hasSomeIpByteBufferMaxLength(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                      const std::shared_ptr<BstIdl::FObject> &_object);

    bool hasSomeIpByteBufferLengthWidth(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                        const std::shared_ptr<BstIdl::FObject> &_object);

    bool isDefaultWidth(int _width);

    bool hasSomeIpIntegerBitWidth(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                  const std::shared_ptr<BstIdl::FObject> &_object);

    bool hasSomeIpIntegerInvalidValue(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                      const std::shared_ptr<BstIdl::FObject> &_object);

    bool hasSomeIpStringLength(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                               const std::shared_ptr<BstIdl::FObject> &_object);

    bool hasSomeIpStringLengthWidth(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                    const std::shared_ptr<BstIdl::FObject> &_object);

    bool hasSomeIpStructLengthWidth(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                    const std::shared_ptr<BstIdl::FObject> &_object);

    bool hasSomeIpStringEncoding(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                 const std::shared_ptr<BstIdl::FObject> &_object);

    bool hasSomeIpMapMinLength(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                               const std::shared_ptr<BstIdl::FObject> &_object);

    bool hasSomeIpMapMaxLength(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                               const std::shared_ptr<BstIdl::FObject> &_object);

    bool hasSomeIpMapLengthWidth(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                 const std::shared_ptr<BstIdl::FObject> &_object);

    bool hasSomeIpUnionLengthWidth(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                   const std::shared_ptr<BstIdl::FObject> &_object);

    bool hasSomeIpUnionTypeWidth(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                 const std::shared_ptr<BstIdl::FObject> &_object);

    bool hasSomeIpUnionDefaultOrder(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                    const std::shared_ptr<BstIdl::FObject> &_object);

    bool hasSomeIpUnionMaxLength(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                 const std::shared_ptr<BstIdl::FObject> &_object);

    bool hasSomeIpEnumWidth(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                            const std::shared_ptr<BstIdl::FObject> &_object);

    bool hasSomeIpEnumBitWidth(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                               const std::shared_ptr<BstIdl::FObject> &_object);

    bool hasSomeIpEnumInvalidValue(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                                   const std::shared_ptr<BstIdl::FObject> &_object);

    bool hasDeployment(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                       const std::shared_ptr<BstIdl::FTypedElement> &_element);

    bool hasDeployment(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                       const std::shared_ptr<BstIdl::FArrayType> &_array);

    bool hasDeployment(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                       const std::shared_ptr<BstIdl::FMapType> &_map);

    bool hasDeployment(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                       const std::shared_ptr<BstIdl::FEnumerationType> &_enum);

    bool hasDeployment(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                       const std::shared_ptr<BstIdl::FStructType> &_struct);

    bool hasDeployment(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                       const std::shared_ptr<BstIdl::FUnionType> &_union);

    bool hasDeployment(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                       const std::shared_ptr<BstIdl::FTypeDef> &_typeDef);

    bool hasDeployment(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                       const std::shared_ptr<BstIdl::FIntegerInterval> &_type);

    bool hasDeployment(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                       const std::shared_ptr<BstIdl::FBasicTypeId> &_type);

    bool hasDeployment(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                       const std::shared_ptr<BstIdl::FType> &_type);

    bool hasDeployment(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                       const std::shared_ptr<BstIdl::FTypeRef> &_type);

    bool isFull(const std::shared_ptr<BstIdl::FBasicTypeId> &_type, int _width);

    bool hasSpecificDeployment(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                               const std::shared_ptr<BstIdl::FTypedElement> &_attribute);

    bool hasDeployedArgument(const std::shared_ptr<BstIdl::FBroadcast> &_broadcast,
                             const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    bool hasDeployedArgument(const std::shared_ptr<BstIdl::FMethod> &_method,
                             const std::shared_ptr<SomeipPropertyAccessor> &_accessor, bool _in, bool _out);

    std::list<std::string> getDeploymentInputIncludes(const std::shared_ptr<BstIdl::FType> &_type,
                                                      const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::list<std::string> getDeploymentInputIncludes(const std::shared_ptr<BstIdl::FTypeDef> &_typeDef,
                                                      const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::list<std::string> getDeploymentInputIncludes(const std::shared_ptr<BstIdl::FTypeRef> &_typeRef,
                                                      const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::list<std::string> getDeploymentInputIncludes(const std::shared_ptr<BstIdl::FStructType> &_struct,
                                                      const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::list<std::string> getDeploymentInputIncludes(const std::shared_ptr<BstIdl::FUnionType> &_union,
                                                      const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::list<std::string> getDeploymentInputIncludes(const std::shared_ptr<BstIdl::FMapType> &_map,
                                                      const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::list<std::string> getDeploymentInputIncludes(const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                                      const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::list<std::string> getDeploymentInputIncludes(const std::shared_ptr<BstIdl::FInterface> &_interface,
                                                      const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getDeployment(const std::shared_ptr<BstIdl::FBasicTypeId> &_type);

    std::string getDeploymentType(const std::shared_ptr<BstIdl::FTypeDef> &_typeDef,
                                  const std::shared_ptr<BstIdl::FTypeCollection> &_interface, bool _useTc);

    std::string getDeploymentType(const std::shared_ptr<BstIdl::FTypedElement> &_typedElement,
                                  const std::shared_ptr<BstIdl::FTypeCollection> &_interface, bool _useTc);

    std::string getDeploymentType(const std::shared_ptr<BstIdl::FTypeRef> &_typeRef,
                                  const std::shared_ptr<BstIdl::FTypeCollection> &_interface, bool _useTc);

    std::string getDeploymentType(const std::shared_ptr<BstIdl::FIntegerInterval> &_type,
                                  const std::shared_ptr<BstIdl::FTypeCollection> &_interface, bool _useTc);

    std::string getDeploymentType(const std::shared_ptr<BstIdl::FBasicTypeId> &_type,
                                  const std::shared_ptr<BstIdl::FTypeCollection> &_interface, bool _useTc);

    std::string getDeploymentType(const std::shared_ptr<BstIdl::FType> &_type,
                                  const std::shared_ptr<BstIdl::FTypeCollection> &_interface, bool _useTc);

    bool hasNonArrayDeployment(const std::shared_ptr<SomeipPropertyAccessor> &_accessor,
                               const std::shared_ptr<BstIdl::FTypedElement> &_attribute);

    std::string getDeploymentRef(const std::shared_ptr<BstIdl::FTypedElement> &_typedElement, bool _isArray,
                                 const std::shared_ptr<BstIdl::FModelElement> &_element,
                                 const std::shared_ptr<BstIdl::FInterface> &_interface,
                                 const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getDeploymentRef(const std::shared_ptr<BstIdl::FTypedElement> &_typedElement,
                                 const std::shared_ptr<BstIdl::FModelElement> &_element,
                                 const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getDeploymentRef(const std::shared_ptr<BstIdl::FTypeRef> &_typeRef,
                                 const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getDeploymentRef(const std::shared_ptr<BstIdl::FType> &_type,
                                 const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getDeploymentRef(const std::shared_ptr<BstIdl::FBasicTypeId> &_typeId,
                                 const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getDeploymentName(const std::shared_ptr<BstIdl::FTypedElement> &_typedElement,
                                  const std::shared_ptr<BstIdl::FModelElement> &_element,
                                  const std::shared_ptr<BstIdl::FInterface> &_interface,
                                  const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getDeploymentName(const std::shared_ptr<BstIdl::FTypeDef> &_typeDef,
                                  const std::shared_ptr<BstIdl::FTypeCollection> &_interface,
                                  const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getDeploymentName(const std::shared_ptr<BstIdl::FTypeRef> &_typeRef,
                                  const std::shared_ptr<BstIdl::FTypeCollection> &_interface,
                                  const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getDeploymentName(const std::shared_ptr<BstIdl::FType> &_type,
                                  const std::shared_ptr<BstIdl::FTypeCollection> &_interface,
                                  const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getDeploymentName(const std::shared_ptr<BstIdl::FBasicTypeId> &_typeId,
                                  const std::shared_ptr<BstIdl::FTypeCollection> &_interface,
                                  const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string someipProxyClassName(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::string getSomeIpServiceID(const std::shared_ptr<BstIdl::FInterface> &fInterface);

    std::list<std::string> getNotifierEventGroups(const std::shared_ptr<BstIdl::FAttribute> &_attribute,
                                                  const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getNotifierIdentifier(const std::shared_ptr<BstIdl::FAttribute> &_attribute,
                                      const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getGetterIdentifier(const std::shared_ptr<BstIdl::FAttribute> &_attribute,
                                    const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string isGetterReliable(const std::shared_ptr<BstIdl::FAttribute> &_attribute,
                                 const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string isNotifierReliable(const std::shared_ptr<BstIdl::FAttribute> &_attribute,
                                   const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getNotifierReliabilityType(const std::shared_ptr<BstIdl::FAttribute> &_attribute,
                                           const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getEndianess(const std::shared_ptr<BstIdl::FAttribute> &_attribute,
                             const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getEndianess(const std::shared_ptr<BstIdl::FBroadcast> &_broadcast,
                             const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getSetterIdentifier(const std::shared_ptr<BstIdl::FAttribute> &_attribute,
                                    const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string isSetterReliable(const std::shared_ptr<BstIdl::FAttribute> &_attribute,
                                 const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::list<std::string> getEventGroups(const std::shared_ptr<BstIdl::FBroadcast> &_broadcast,
                                          const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getEventIdentifier(const std::shared_ptr<BstIdl::FBroadcast> &_broadcast,
                                   const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getReliabilityType(const std::shared_ptr<BstIdl::FBroadcast> &_broadcast,
                                   const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string isReliable(const std::shared_ptr<BstIdl::FBroadcast> &_broadcast,
                           const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string isReliable(const std::shared_ptr<BstIdl::FMethod> &_method,
                           const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string isLittleEndian(const std::shared_ptr<BstIdl::FMethod> &_method,
                               const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getDeployments(const std::shared_ptr<BstIdl::FBroadcast> &_broadcast,
                               const std::shared_ptr<BstIdl::FInterface> &fInterface,
                               const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getErrorDeploymentType(const std::shared_ptr<BstIdl::FMethod> &_method, bool _isArgument);

    std::string getErrorDeploymentRef(const std::shared_ptr<BstIdl::FMethod> &_method,
                                      const std::shared_ptr<BstIdl::FInterface> &_interface,
                                      const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getDeployments(const std::shared_ptr<BstIdl::FMethod> &_method,
                               const std::shared_ptr<BstIdl::FInterface> &_interface,
                               const std::shared_ptr<SomeipPropertyAccessor> &_accessor, bool _withInArgs,
                               bool _withOutArgs);

    std::string getMethodIdentifier(const std::shared_ptr<BstIdl::FMethod> &_method,
                                    const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getDeployable(const std::shared_ptr<BstIdl::FArgument> &_argument,
                              const std::shared_ptr<BstIdl::FInterface> &_interface,
                              const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getDeployables(std::list<std::shared_ptr<BstIdl::FArgument>> &_arguments,
                               const std::shared_ptr<BstIdl::FInterface> &_interface,
                               const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getMethodName(const std::shared_ptr<BstIdl::FAttribute> &_attribute);

    std::string setMethodName(const std::shared_ptr<BstIdl::FAttribute> &_attribute);

    std::string getDeploymentTypes(std::list<std::shared_ptr<BstIdl::FArgument>> &_arguments,
                                   const std::shared_ptr<BstIdl::FInterface> &_interface,
                                   const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    // vsomeip extensions
    std::string getServerHostMessage(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                     const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getClientHostMessage(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                     const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getLogging(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                           const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getServiceDiscovery(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                    const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getApplications(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                const std::shared_ptr<SomeipPropertyAccessor> &_accessor);
    std::list<std::string> getApplication(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                          const std::shared_ptr<SomeipPropertyAccessor> &_accessor);
    // vsomeip services
    std::string getServices(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                            const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::list<std::string> getService(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                      const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getEvents(const std::shared_ptr<BstIdl::FDExtensionElement> &_instance,
                          const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::list<std::string> getEvent(const std::shared_ptr<BstIdl::FDExtensionElement> &_instance,
                                    const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getEventGroups(const std::shared_ptr<BstIdl::FDExtensionElement> &_instance,
                               const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::list<std::string> getEventGroup(const std::shared_ptr<BstIdl::FDExtensionElement> &_instance,
                                         const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getEventID(const std::shared_ptr<BstIdl::FBroadcast> &_broadcast,
                           const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::list<std::string> getEventGroupsID(const std::shared_ptr<BstIdl::FBroadcast> &_broadcast,
                                            const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getNotifierID(const std::shared_ptr<BstIdl::FAttribute> &_attribute,
                              const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::list<std::string> getNotifierEventGroupsID(const std::shared_ptr<BstIdl::FAttribute> &_attribute,
                                                    const std::shared_ptr<SomeipPropertyAccessor> &_accessor);
    // vsomeip clients
    std::string getClients(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                           const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getClient(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                          const std::shared_ptr<SomeipPropertyAccessor> &_accessor);
};
} // namespace BstCommonAPI
#endif