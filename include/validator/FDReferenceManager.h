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
#ifndef F_D_REFERENCE_MANAGER_H
#define F_D_REFERENCE_MANAGER_H

#include "FDModel/FDArray.h"
#include "FDModel/FDEnumeration.h"
#include "FDModel/FDEnumerationOverwrites.h"
#include "FDModel/FDExtensionElement.h"
#include "FDModel/FDExtensionRoot.h"
#include "FDModel/FDField.h"
#include "FDModel/FDInterface.h"
#include "FDModel/FDInterfaceRef.h"
#include "FDModel/FDMap.h"
#include "FDModel/FDModelManager.h"
#include "FDModel/FDObject.h"
#include "FDModel/FDSpecification.h"
#include "FDModel/FDStruct.h"
#include "FDModel/FDTypedef.h"
#include "FDModel/FDTypes.h"
#include "FDModel/FDUnion.h"
#include "model/FInterface.h"
#include "model/FTypeCollection.h"
#include <algorithm>
#include <list>
#include <memory>
#include <tuple>

namespace BstIdl
{
class FDReferenceManager
{
public:
    FDReferenceManager(const FDReferenceManager &) = delete;
    FDReferenceManager(FDReferenceManager &&) noexcept = delete;

    static FDReferenceManager &getInstance();
    void clear();
    void addFDPropertyDeclRef(const std::shared_ptr<FDProperty> &property, const std::string &fqn);
    void addSpecificationExtendsRef(const std::shared_ptr<FDSpecification> &spec, const std::string &fqn);
    void addExtensionRootSpecificationRef(const std::shared_ptr<FDExtensionRoot> &extensionRoot,
                                          const std::string &fqn);
    void addExtensionRootTargetRef(const std::shared_ptr<FDExtensionRoot> &obj, const std::string &fqn); // FQN:EObject
    void addExtensionRootUseRef(const std::shared_ptr<FDExtensionRoot> &obj,
                                const std::string &fqn); // FQN:FDRootElement
    void addExtensionElementTargetRef(const std::shared_ptr<FDExtensionElement> &obj, const std::string &fqn);
    void addTypesSpecificationRef(const std::shared_ptr<FDTypes> &types, const std::string &fqn);
    void addTypesTypeCollectionRef(const std::shared_ptr<FDTypes> &types, const std::string &fqn);
    void addTypesUseRef(const std::shared_ptr<FDTypes> &types, const std::string &fqn);
    void addInterfaceSpecificationRef(const std::shared_ptr<FDInterface> &interface, const std::string &fqn);
    void addInterfaceInterfaceRef(const std::shared_ptr<FDInterface> &interface, const std::string &fqn);
    void addInterfaceUseRef(const std::shared_ptr<FDInterface> &interface, const std::string &fqn);
    void addAttributeAttributeRef(const std::shared_ptr<FDAttribute> &attribute, const std::string &fqn);
    void addMethodTargetRef(const std::shared_ptr<FDMethod> &method, const std::string &fqn);
    void addArgumentTargetRef(const std::shared_ptr<FDArgument> &argument, const std::string &fqn);
    void addArrayTargetRef(const std::shared_ptr<FDArray> &array, const std::string &fqn);
    void addFDStructTargetRef(const std::shared_ptr<FDStruct> &new_struct, const std::string &fqn);
    void addFDUnionTargetRef(const std::shared_ptr<FDUnion> &new_union, const std::string &fqn);
    void addFDTypedefTargetRef(const std::shared_ptr<FDTypedef> &new_typedef, const std::string &fqn);
    void addFDFieldTargetRef(const std::shared_ptr<FDField> &field, const std::string &fqn);
    void addFDEnumerationTargetRef(const std::shared_ptr<FDEnumeration> &enumeration, const std::string &fqn);
    void addFDEnumValueTargetRef(const std::shared_ptr<FDEnumValue> &enumValue, const std::string &fqn);
    void addFDMapTargetRef(const std::shared_ptr<FDMap> &map, const std::string &fqn);
    void addFDInterfaceValueRef(const std::shared_ptr<FDInterfaceRef> &interfaceRef, const std::string &fqn);
    void addBroadcastTargetRef(const std::shared_ptr<FDBroadcast> &broadcast, const std::string &fqn);
    // find overwrite field and enumValue to resolve targets
    std::shared_ptr<BstIdl::FObject> findOverwritedElement(std::string &fqn, std::list<std::string> &errorList);

    std::list<std::string> resolveFDPropertyDeclRef();
    std::list<std::string> resolveSpecificationExtendsRef();
    std::list<std::string> resolveExtensionRootSpecificationRef();
    std::list<std::string> resolveExtensionRootTargetRef();
    std::list<std::string> resolveExtensionRootUseRef();
    std::list<std::string> resolveExtensionElementTargetRef();
    std::list<std::string> resolveTypesSpecificationRef();
    std::list<std::string> resolveTypesTypeCollectionRef();
    std::list<std::string> resolveTypesUseRef();
    std::list<std::string> resolveInterfaceSpecificationRef();
    std::list<std::string> resolveInterfaceTargetRef();
    std::list<std::string> resolveInterfaceUseRef();
    std::list<std::string> resolveAttributeTargetRef();
    std::list<std::string> resolveMethodTargetRef();
    std::list<std::string> resolveArgumentTargetRef();
    std::list<std::string> resolveArrayTargetRef();
    std::list<std::string> resolveFDStructTargetRef();
    std::list<std::string> resolveFDUnionTargetRef();
    std::list<std::string> resolveFDTypedefTargetRef();
    std::list<std::string> resolveFDFieldTargetRef();
    std::list<std::string> resolveFDEnumerationTargetRef();
    std::list<std::string> resolveFDEnumValueTargetRef();
    std::list<std::string> resolveFDMapTargetRef();
    std::list<std::string> resolveFDInterfaceValueRef();
    std::list<std::string> resolveBroadcastTargetRef();
    bool resolveAllRef();

private:
    FDReferenceManager() = default;
    ~FDReferenceManager() = default;

    std::list<std::tuple<std::shared_ptr<FDProperty>, std::string>> m_propertyDeclRefs;
    std::list<std::tuple<std::shared_ptr<FDSpecification>, std::string>> m_specificationExtendRefs;
    std::list<std::tuple<std::shared_ptr<FDExtensionRoot>, std::string>> m_extensionRootSpecificationRefs;
    std::list<std::tuple<std::shared_ptr<FDExtensionRoot>, std::string>> m_extensionRootTargetRefs; // FQN:FDObject
    std::list<std::tuple<std::shared_ptr<FDExtensionRoot>, std::string>> m_extensionRootUseRefs;    // FQN:FDRootElement
    std::list<std::tuple<std::shared_ptr<FDExtensionElement>, std::string>> m_extensionElementTargetRefs;
    std::list<std::tuple<std::shared_ptr<FDTypes>, std::string>> m_typeSpecificationsRefs;
    std::list<std::tuple<std::shared_ptr<FDTypes>, std::string>> m_typesTypeCollectionRefs; // FTypeCollection
    std::list<std::tuple<std::shared_ptr<FDTypes>, std::string>> m_typesUseRefs;
    std::list<std::tuple<std::shared_ptr<FDInterface>, std::string>> m_interfaceSpecificationRefs;
    std::list<std::tuple<std::shared_ptr<FDInterface>, std::string>> m_interfaceInterfaceRefs;
    std::list<std::tuple<std::shared_ptr<FDInterface>, std::string>> m_interfaceUseRefs;        // FDRootElement
    std::list<std::tuple<std::shared_ptr<FDAttribute>, std::string>> m_attributeAttributeRefs;  // FAttribute
    std::list<std::tuple<std::shared_ptr<FDMethod>, std::string>> m_methodTargetRefs;           // FMethod
    std::list<std::tuple<std::shared_ptr<FDArgument>, std::string>> m_argumentTargetRefs;       // FArgument
    std::list<std::tuple<std::shared_ptr<FDArray>, std::string>> m_arrayTargetRefs;             // FArrayType
    std::list<std::tuple<std::shared_ptr<FDStruct>, std::string>> m_structTargetRefs;           // FStructType
    std::list<std::tuple<std::shared_ptr<FDUnion>, std::string>> m_unionTargetRefs;             // FStructType
    std::list<std::tuple<std::shared_ptr<FDTypedef>, std::string>> m_typeDefTargetRefs;         // FTypeDef
    std::list<std::tuple<std::shared_ptr<FDField>, std::string>> m_fieldTargetRefs;             // FField
    std::list<std::tuple<std::shared_ptr<FDEnumeration>, std::string>> m_enumerationTargetRefs; // FEnumerationType
    std::list<std::tuple<std::shared_ptr<FDEnumValue>, std::string>> m_enumValueTargetRefs;     // FEnumerator
    std::list<std::tuple<std::shared_ptr<FDMap>, std::string>> m_mapTargetRefs;                 // FMapType
    std::list<std::tuple<std::shared_ptr<FDInterfaceRef>, std::string>>
        m_interfaceValueRefs; // FDInterfaceRef value=[fidl::FInterface|FQN]
    std::list<std::tuple<std::shared_ptr<FDBroadcast>, std::string>> m_broadcastTargetRefs; // FBroadcast
};
} // namespace BstIdl

#endif