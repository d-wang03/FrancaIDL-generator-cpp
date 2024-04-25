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
#include "FTypeCycleDetector.h"
#include "model/FFactory.h"
#include <iostream>
namespace BstIdl
{
class FTypeGenerator
{
private:
    std::shared_ptr<FrancaGeneratorExtensions> francaGeneratorExtensions;
    void addFTypeRequiredHeaders(const std::shared_ptr<FType> &type, std::list<std::string> &generatedHeaders,
                                 std::list<std::string> &libraryHeaders);

    std::string getConstReferenceVariable(const std::shared_ptr<FField> &destination,
                                          const std::shared_ptr<FModelElement> &source);

    void getRequiredHeaderPath(const std::shared_ptr<FTypeRef> &fTypeRef, std::list<std::string> &generatedHeaders,
                               std::list<std::string> &libraryHeaders);

    void getRequiredHeaderPath(const std::shared_ptr<FBasicTypeId> &basic_id, std::list<std::string> &generatedHeaders,
                               std::list<std::string> &libraryHeaders);

    std::list<std::shared_ptr<FType>> sortTypes(std::list<std::shared_ptr<FType>> &typeList,
                                                std::shared_ptr<FTypeCollection> containingTypeCollection);

    bool hasNoCircularDependencies(std::list<std::shared_ptr<FType>> types);

    std::string generateKeyType(const std::shared_ptr<FMapType> &fMap);

    std::string generateValueType(const std::shared_ptr<FMapType> &fMap);

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

    bool hasImplementation(const std::shared_ptr<FType> &fType);

    static bool isdeprecated(const std::shared_ptr<FAnnotationBlock> &annotations);

    static std::string breaktext(std::string text, const FAnnotationType &annotation);

    static std::string generateComments(const std::shared_ptr<FModelElement> &model, bool inline_);

    void addRequiredHeaders(const std::shared_ptr<FType> &fType, std::list<std::string> &generatedHeaders,
                            std::list<std::string> &libraryHeaders);

    std::string getInitialValue(const std::shared_ptr<FEnumerationType> &_enumeration);

    std::string generateLiterals(const std::shared_ptr<FEnumerationType> &_enumeration, std::string _backingType);

    std::string generateFTypeDeclaration(const std::shared_ptr<FTypeDef> &fTypeDef);

    std::string generateFTypeDeclaration(const std::shared_ptr<FArrayType> &fArrayType);

    std::string generateFTypeDeclaration(const std::shared_ptr<FMapType> &fMap);

    std::string generateFTypeDeclaration(const std::shared_ptr<FStructType> &fStructType);

    std::string generateFTypeDeclaration(const std::shared_ptr<FEnumerationType> &fEnumerationType);

    std::string generateFTypeDeclaration(const std::shared_ptr<FUnionType> &fUnionType);
    std::string getElementNames(const std::shared_ptr<FUnionType> &fUnionType);

    std::string generateDeclaration(const std::shared_ptr<FEnumerationType> &_enumeration,
                                    const std::shared_ptr<FModelElement> &_parent);

    // focus
    std::string generateFTypeDeclarations(const std::shared_ptr<FTypeCollection> &fTypeCollection);

    std::string generateFConstDeclarations(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection);

    void generateInheritanceIncludes(const std::shared_ptr<FInterface> &fInterface,
                                     std::list<std::string> &generatedHeaders, std::list<std::string> &libraryHeaders);

    void generateRequiredTypeIncludes(const std::shared_ptr<FInterface> &fInterface,
                                      std::list<std::string> &generatedHeaders, std::list<std::string> &libraryHeaders,
                                      bool isStub);

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
};
} // namespace BstIdl
#endif