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
#ifndef F_TYPE_COLLECTION_SOME_I_P_DEPLOYMENT_GENERATOR
#define F_TYPE_COLLECTION_SOME_I_P_DEPLOYMENT_GENERATOR
#include "FrancaSomeIPGeneratorExtensions.h"
#include "SomeipPropertyAccessor.h"
#include "generator/AbstractGenerator.h"
#include "model/FTypeCollection.h"
#include <memory>
namespace BstCommonAPI
{
class FTypeCollectionSomeIPDeploymentGenerator : public BstIdl::AbstractGenerator
{
public:
    FTypeCollectionSomeIPDeploymentGenerator() = default;
    virtual ~FTypeCollectionSomeIPDeploymentGenerator() = default;
    FTypeCollectionSomeIPDeploymentGenerator(const FTypeCollectionSomeIPDeploymentGenerator &) = default;
    FTypeCollectionSomeIPDeploymentGenerator(FTypeCollectionSomeIPDeploymentGenerator &&) noexcept = default;
    FTypeCollectionSomeIPDeploymentGenerator &operator=(const FTypeCollectionSomeIPDeploymentGenerator &) = default;
    FTypeCollectionSomeIPDeploymentGenerator &operator=(FTypeCollectionSomeIPDeploymentGenerator &&) = default;

    static FTypeCollectionSomeIPDeploymentGenerator &getInstance()
    {
        static FTypeCollectionSomeIPDeploymentGenerator instance;
        return instance;
    }
    bool generate() override
    {
        return true;
    }
    void generateTypeCollectionDeployment(const std::shared_ptr<BstIdl::FTypeCollection> &tc,
                                          const std::shared_ptr<SomeipPropertyAccessor> &deploymentAccessor,
                                          bool generate_code_someip, std::string &dir);

private:
    std::string generateDeploymentHeader(const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                         const std::shared_ptr<SomeipPropertyAccessor> &deploymentAccessor);
    std::string generateDeploymentSource(const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                         const std::shared_ptr<SomeipPropertyAccessor> &deploymentAccessor);

protected:
    std::string generateDeploymentType(const std::shared_ptr<BstIdl::FTypeRef> &_typeRef, int _indent,
                                       const std::shared_ptr<SomeipPropertyAccessor> &_accessor);
    std::string generateDeploymentType(const std::shared_ptr<BstIdl::FArrayType> &_array, int _indent,
                                       const std::shared_ptr<SomeipPropertyAccessor> &_accessor);
    std::string generateArrayDeploymentType(const std::shared_ptr<BstIdl::FTypeRef> &_typeRef, int _indent,
                                            const std::shared_ptr<SomeipPropertyAccessor> &_accessor);
    std::string generateDeploymentType(const std::shared_ptr<BstIdl::FEnumerationType> &_enum, int _indent,
                                       const std::shared_ptr<SomeipPropertyAccessor> &_accessor);
    std::string generateDeploymentType(const std::shared_ptr<BstIdl::FMapType> &_map, int _indent,
                                       const std::shared_ptr<SomeipPropertyAccessor> &_accessor);
    std::string generateDeploymentType(const std::shared_ptr<BstIdl::FIntegerInterval> &_interval, int _indent,
                                       const std::shared_ptr<SomeipPropertyAccessor> &_accessor);
    std::string generateDeploymentType(const std::shared_ptr<BstIdl::FStructType> &_struct, int _indent,
                                       const std::shared_ptr<SomeipPropertyAccessor> &_accessor);
    std::string generateDeploymentType(const std::shared_ptr<BstIdl::FUnionType> &_union, int _indent,
                                       const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateDeploymentType(const std::shared_ptr<BstIdl::FTypeDef> &_typeDef, int _indent,
                                       const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateDeploymentType(const std::shared_ptr<BstIdl::FBasicTypeId> &_type, int _indent,
                                       const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateDeploymentType(const std::shared_ptr<BstIdl::FType> &_type, int _indent,
                                       const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateDeploymentDeclaration(const std::shared_ptr<BstIdl::FType> &_type,
                                              const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                              const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateDeploymentDeclaration(const std::shared_ptr<BstIdl::FArrayType> &_array,
                                              const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                              const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateDeploymentDeclaration(const std::shared_ptr<BstIdl::FEnumerationType> &_enum,
                                              const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                              const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateDeploymentDeclaration(const std::shared_ptr<BstIdl::FMapType> &_map,
                                              const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                              const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateDeploymentDeclaration(const std::shared_ptr<BstIdl::FStructType> &_struct,
                                              const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                              const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateDeploymentDeclaration(const std::shared_ptr<BstIdl::FUnionType> &_union,
                                              const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                              const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateDeploymentDeclaration(const std::shared_ptr<BstIdl::FField> &_field,
                                              const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                              const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateDeploymentDeclaration(const std::shared_ptr<BstIdl::FTypeDef> &_typeDef,
                                              const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                              const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateDeploymentDeclaration(const std::shared_ptr<BstIdl::FTypeRef> &_typeRef,
                                              const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                              const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateDeploymentDefinition(const std::shared_ptr<BstIdl::FType> &_type,
                                             const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                             const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateDeploymentDefinition(const std::shared_ptr<BstIdl::FArrayType> &_array,
                                             const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                             const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateDeploymentDefinition(const std::shared_ptr<BstIdl::FEnumerationType> &_enum,
                                             const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                             const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateDeploymentDefinition(const std::shared_ptr<BstIdl::FMapType> &_map,
                                             const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                             const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateDeploymentDefinition(const std::shared_ptr<BstIdl::FStructType> &_struct,
                                             const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                             const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateDeploymentDefinition(const std::shared_ptr<BstIdl::FUnionType> &_union,
                                             const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                             const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateDeploymentDefinition(const std::shared_ptr<BstIdl::FField> &_field,
                                             const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                             const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateDeploymentDefinition(const std::shared_ptr<BstIdl::FTypeDef> &_typeDef,
                                             const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                             const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string generateDeploymentDefinition(const std::shared_ptr<BstIdl::FTypeRef> &_typeRef,
                                             const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                             const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getDeploymentParameter(const std::shared_ptr<BstIdl::FType> &_type,
                                       const std::shared_ptr<BstIdl::FObject> &_source,
                                       const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                       const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getDeploymentParameter(const std::shared_ptr<BstIdl::FArrayType> &_array,
                                       const std::shared_ptr<BstIdl::FObject> &_source,
                                       const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                       const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getDeploymentParameter(const std::shared_ptr<BstIdl::FEnumerationType> &_enum,
                                       const std::shared_ptr<BstIdl::FObject> &_source,
                                       const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                       const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getDeploymentParameter(const std::shared_ptr<BstIdl::FMapType> &_map,
                                       const std::shared_ptr<BstIdl::FObject> &_source,
                                       const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                       const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getDeploymentParameter(const std::shared_ptr<BstIdl::FIntegerInterval> &_interval,
                                       const std::shared_ptr<BstIdl::FObject> &_source,
                                       const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                       const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getDeploymentParameter(const std::shared_ptr<BstIdl::FStructType> &_struct,
                                       const std::shared_ptr<BstIdl::FObject> &_source,
                                       const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                       const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getDerivedDeploymentParameter(const std::shared_ptr<BstIdl::FStructType> &_struct,
                                              const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                              const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getDeploymentParameter(const std::shared_ptr<BstIdl::FUnionType> &_union,
                                       const std::shared_ptr<BstIdl::FObject> &_source,
                                       const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                       const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getDeploymentParameter(const std::shared_ptr<BstIdl::FBasicTypeId> &_typeId,
                                       const std::shared_ptr<BstIdl::FObject> &_source,
                                       const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                       const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getDeploymentParameter(const std::shared_ptr<BstIdl::FTypeRef> &_typeRef,
                                       const std::shared_ptr<BstIdl::FObject> &_source,
                                       const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                       const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getDeploymentParameter(const std::shared_ptr<BstIdl::FTypeDef> _typeDef,
                                       const std::shared_ptr<BstIdl::FObject> &_source,
                                       const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                       const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getDeploymentParameter(const std::shared_ptr<BstIdl::FTypedElement> &_attribute,
                                       const std::shared_ptr<BstIdl::FObject> &_object,
                                       const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                       const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getDeploymentParameter(const std::shared_ptr<BstIdl::FArgument> &_argument,
                                       const std::shared_ptr<BstIdl::FObject> &_object,
                                       const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                       const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getArrayElementTypeDeploymentParameter(const std::shared_ptr<BstIdl::FTypeRef> &_elementType,
                                                       const std::shared_ptr<BstIdl::FObject> &_source,
                                                       const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                                       const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getByteBufferDeploymentParamter(const std::shared_ptr<BstIdl::FObject> &_buffer,
                                                const std::shared_ptr<BstIdl::FObject> &_source,
                                                const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getArrayDeploymentParameter(const std::shared_ptr<BstIdl::FObject> &_array,
                                            const std::shared_ptr<BstIdl::FObject> &_source,
                                            const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                            const std::shared_ptr<SomeipPropertyAccessor> &_accessor);

    std::string getMapDeploymentParameter(const std::shared_ptr<BstIdl::FObject> &_map,
                                          const std::shared_ptr<BstIdl::FObject> &_source,
                                          const std::shared_ptr<BstIdl::FTypeCollection> &_tc,
                                          const std::shared_ptr<SomeipPropertyAccessor> &_accessor);
};
} // namespace BstCommonAPI
#endif