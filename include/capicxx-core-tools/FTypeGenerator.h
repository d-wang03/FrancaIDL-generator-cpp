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
#ifndef F_TYPE_GENERATOR_H
#define F_TYPE_GENERATOR_H
#include "CommonapiPropertyAccessor.h"
#include "model/FFactory.h"
#include <vector>

namespace BstCommonAPI
{
class FTypeGenerator
{
public:
    FTypeGenerator() = default;
    virtual ~FTypeGenerator() = default;
    FTypeGenerator(const FTypeGenerator &) = default;
    FTypeGenerator(FTypeGenerator &&) noexcept = default;
    FTypeGenerator &operator=(const FTypeGenerator &) = default;
    FTypeGenerator &operator=(FTypeGenerator &&) = default;
    enum ModelTyp
    {
        INTERFACE = 0,
        METHOD,
        ENUM,
        UNKNOWN
    };
    static FTypeGenerator &getInstance()
    {
        static FTypeGenerator ins;
        return ins;
    }

    static std::string breaktext(std::string text, const BstIdl::FAnnotationType &annotation);

    static std::string generateComments(const std::shared_ptr<BstIdl::FModelElement> &model, bool inline_);

    void addRequiredHeaders(const std::shared_ptr<BstIdl::FType> &fType, std::list<std::string> &generatedHeaders,
                            std::list<std::string> &libraryHeaders);

    std::string generateHasher(const std::shared_ptr<BstIdl::FMapType> &fMap);

    std::string getConstReferenceVariable(const std::shared_ptr<BstIdl::FField> &destination,
                                          const std::shared_ptr<BstIdl::FModelElement> &source);

    std::string generateLiterals(const std::shared_ptr<BstIdl::FEnumerationType> &_enumeration,
                                 std::string _backingType);

    std::string getInitialValue(const std::shared_ptr<BstIdl::FEnumerationType> &_enumeration);

    std::string generateBaseTypeAssignmentOperator(const std::shared_ptr<BstIdl::FEnumerationType> _enumeration,
                                                   const std::shared_ptr<BstIdl::FEnumerationType> _other,
                                                   const std::shared_ptr<CommonapiPropertyAccessor> &_accessor);

    std::string generateLiteralValidation(const std::shared_ptr<BstIdl::FEnumerationType> &_enumeration,
                                          const std::string backingType, std::list<std::string> &_values);

    std::string generateLiteralString(const std::shared_ptr<BstIdl::FEnumerationType> &_enumeration,
                                      const std::string &backingType, std::list<std::string> &_values);

    std::string generateDeclaration(const std::shared_ptr<BstIdl::FEnumerationType> &_enumeration,
                                    const std::shared_ptr<BstIdl::FModelElement> &_parent,
                                    const std::shared_ptr<CommonapiPropertyAccessor> &_accessor);

    std::string generateFTypeDeclaration(const std::shared_ptr<BstIdl::FTypeDef> &fTypeDef,
                                         const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor);

    std::string generateFTypeDeclaration(const std::shared_ptr<BstIdl::FArrayType> &fArrayType,
                                         const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor);

    std::string generateFTypeDeclaration(const std::shared_ptr<BstIdl::FMapType> &fMap,
                                         const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor);

    std::string generateFTypeDeclaration(const std::shared_ptr<BstIdl::FStructType> &fStructType,
                                         const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor);

    std::string generateFTypeDeclaration(const std::shared_ptr<BstIdl::FEnumerationType> &fEnumerationType,
                                         const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor);

    std::string generateFTypeDeclaration(const std::shared_ptr<BstIdl::FUnionType> &fUnionType,
                                         const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor);

    std::string generateFTypeDeclarations(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection,
                                          const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor);

    std::string generateFConstDeclarations(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection,
                                           const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor);

    std::string generateFConstDefinitions(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection,
                                          const std::shared_ptr<CommonapiPropertyAccessor> &deploymentAccessor);

    bool hasImplementation(const std::shared_ptr<BstIdl::FType> &fType);

    bool hasImplementation(const std::shared_ptr<BstIdl::FConstantDef> &fConst);

    void generateRequiredTypeIncludes(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                      std::list<std::string> &generatedHeaders, std::list<std::string> &libraryHeaders,
                                      bool isStub);

    static bool isdeprecated(const std::shared_ptr<BstIdl::FAnnotationBlock> &annotations);

    void generateInheritanceIncludes(const std::shared_ptr<BstIdl::FInterface> &fInterface,
                                     std::list<std::string> &generatedHeaders, std::list<std::string> &libraryHeaders);

private:
    void getRequiredHeaderPath(const std::shared_ptr<BstIdl::FTypeRef> &fTypeRef,
                               std::list<std::string> &generatedHeaders, std::list<std::string> &libraryHeaders);

    void getRequiredHeaderPath(const std::shared_ptr<BstIdl::FBasicTypeId> &basic_id,
                               std::list<std::string> &generatedHeaders, std::list<std::string> &libraryHeaders);

    void addFTypeRequiredHeaders(const std::shared_ptr<BstIdl::FType> &type, std::list<std::string> &generatedHeaders,
                                 std::list<std::string> &libraryHeaders);

    std::string generateKeyType(const std::shared_ptr<BstIdl::FMapType> &fMap);

    std::string generateValueType(const std::shared_ptr<BstIdl::FMapType> &fMap);

    std::string printInitializerExpression(const std::shared_ptr<BstIdl::FInitializerExpression> &rhs);

    std::string printInitializerExpression(const std::shared_ptr<BstIdl::FCurrentError> &rhs);

    std::string printInitializerExpression(const std::shared_ptr<BstIdl::FBracketInitializer> &rhs);

    std::string printInitializerExpression(const std::shared_ptr<BstIdl::FCompoundInitializer> &rhs);

    std::string printInitializerExpression(const std::shared_ptr<BstIdl::FBooleanConstant> &rhs);

    std::string printInitializerExpression(const std::shared_ptr<BstIdl::FDoubleConstant> &rhs);

    std::string printInitializerExpression(const std::shared_ptr<BstIdl::FFloatConstant> &rhs);

    std::string printInitializerExpression(const std::shared_ptr<BstIdl::FIntegerConstant> &rhs);

    std::string printInitializerExpression(const std::shared_ptr<BstIdl::FStringConstant> &rhs);

    std::string printInitializerExpression(const std::shared_ptr<BstIdl::FMethodErrorEnumRef> &rhs);

    std::string printInitializerExpression(const std::shared_ptr<BstIdl::FBinaryOperation> &rhs);

    std::string printInitializerExpression(const std::shared_ptr<BstIdl::FUnaryOperation> &rhs);

    std::string printInitializerExpression(const std::shared_ptr<BstIdl::FQualifiedElementRef> &rhs);

    std::vector<std::shared_ptr<BstIdl::FType>> sortTypes(const std::list<std::shared_ptr<BstIdl::FType>> &typeList);

    std::string getElementNames(const std::shared_ptr<BstIdl::FUnionType> &fUnion);
};
} // namespace BstCommonAPI
#endif