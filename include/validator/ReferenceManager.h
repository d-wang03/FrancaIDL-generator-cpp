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
#ifndef REFERENCE_MANAGER_H
#define REFERENCE_MANAGER_H

#include "model/FEnumerationType.h"
#include "model/FInterface.h"
#include "model/FMethod.h"
#include "model/FQualifiedElementRef.h"
#include "model/FStructType.h"
#include "model/FTypeRef.h"
#include "model/FUnionType.h"
#include <algorithm>
#include <list>
#include <memory>
#include <tuple>

namespace BstIdl
{
class ReferenceManager
{
public:
    ReferenceManager(const ReferenceManager &) = delete;
    ReferenceManager(ReferenceManager &&) noexcept = delete;

    static ReferenceManager &getInstance();
    void clear();
    void addInterfaceExtendsRef(const std::shared_ptr<FInterface> &interface, const std::string &fqn);
    void addInterfaceManagesRef(const std::shared_ptr<FInterface> &interface, const std::string &fqn);
    void addMethodErrorRef(const std::shared_ptr<FMethod> &method, const std::string &fqn,
                           const std::shared_ptr<FInterface> &belonged);
    void addTypeRefDerivedRef(const std::shared_ptr<FTypeRef> &typeref, const std::string &fqn,
                              const std::shared_ptr<FInterface> &belonged);
    void addStructTypeExtendsRef(const std::shared_ptr<FStructType> &structType, const std::string &fqn,
                                 const std::shared_ptr<FInterface> &belonged);
    void addUnionTypeExtendsRef(const std::shared_ptr<FUnionType> &unionType, const std::string &fqn,
                                const std::shared_ptr<FInterface> &belonged);
    void addEnumerationTypeExtendsRef(const std::shared_ptr<FEnumerationType> &enumType, const std::string &fqn,
                                      const std::shared_ptr<FInterface> &belonged);
    void addQerElementRef(const std::shared_ptr<FQualifiedElementRef> &qre, const std::string &fqn,
                          const std::shared_ptr<FInterface> &belonged);

    std::list<std::string> resolveInterfaceExtendsRef();
    std::list<std::string> resolveInterfaceManagesRef();
    std::list<std::string> resolveMethodErrorRef();
    std::list<std::string> resolveTypeRefDerviedRef();
    std::list<std::string> resolveStructTypeExtendsRef();
    std::list<std::string> resolveUnionTypeExtendsRef();
    std::list<std::string> resolveEnumerationTypeExtendsRef();
    std::list<std::string> resolveQerElementRef();
    bool resolveAllRef();

private:
    ReferenceManager() = default;
    ~ReferenceManager() = default;

    std::list<std::tuple<std::shared_ptr<FInterface>, std::string>> m_interfaceExtendsRefs;
    std::list<std::tuple<std::shared_ptr<FInterface>, std::string>> m_interfaceManagesRefs;
    std::list<std::tuple<std::shared_ptr<FMethod>, std::string, std::shared_ptr<FInterface>>> m_methodErrorRefs;
    std::list<std::tuple<std::shared_ptr<FTypeRef>, std::string, std::shared_ptr<FInterface>>> m_typeRefDerivedRefs;
    std::list<std::tuple<std::shared_ptr<FStructType>, std::string, std::shared_ptr<FInterface>>>
        m_structTypeExtendsRefs;
    std::list<std::tuple<std::shared_ptr<FUnionType>, std::string, std::shared_ptr<FInterface>>> m_unionTypeExtendsRefs;
    std::list<std::tuple<std::shared_ptr<FEnumerationType>, std::string, std::shared_ptr<FInterface>>>
        m_enumerationTypeExtendsRefs;
    std::list<std::tuple<std::shared_ptr<FQualifiedElementRef>, std::string, std::shared_ptr<FInterface>>>
        m_qerElementRefs;
};
} // namespace BstIdl

#endif