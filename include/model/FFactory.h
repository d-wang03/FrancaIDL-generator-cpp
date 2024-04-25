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
#ifndef F_FACTORY_H
#define F_FACTORY_H

#include "FAnnotation.h"
#include "FAnnotationBlock.h"
#include "FArgument.h"
#include "FArrayType.h"
#include "FAssignment.h"
#include "FAttribute.h"
#include "FBinaryOperation.h"
#include "FBlock.h"
#include "FBooleanConstant.h"
#include "FBracketInitializer.h"
#include "FBroadcast.h"
#include "FCompoundInitializer.h"
#include "FConstantDef.h"
#include "FCurrentError.h"
#include "FDeclaration.h"
#include "FDoubleConstant.h"
#include "FElementInitializer.h"
#include "FEnumerationType.h"
#include "FEnumerator.h"
#include "FEvaluableElement.h"
#include "FField.h"
#include "FFieldInitializer.h"
#include "FFloatConstant.h"
#include "FImport.h"
#include "FInitializer.h"
#include "FIntegerConstant.h"
#include "FIntegerInterval.h"
#include "FInterface.h"
#include "FMapType.h"
#include "FMethod.h"
#include "FMethodErrorEnumRef.h"
#include "FModel.h"
#include "FQualifiedElementRef.h"
#include "FStringConstant.h"
#include "FStructType.h"
#include "FTypeCollection.h"
#include "FTypeDef.h"
#include "FTypeRef.h"
#include "FUnaryOperation.h"
#include "FUnionType.h"
#include "FVariable.h"
#include "FVersion.h"
#include "utilities/list_utility.h"
#include <memory>

namespace BstIdl
{
class FFactory
{
public:
    static FFactory &getInstance();

    std::shared_ptr<FModel> createFModel()
    {
        return std::make_shared<FModel>();
    }
    std::shared_ptr<FImport> createImport()
    {
        return std::make_shared<FImport>();
    }
    std::shared_ptr<FTypeCollection> createFTypeCollection()
    {
        return std::make_shared<FTypeCollection>();
    }
    std::shared_ptr<FInterface> createFInterface()
    {
        return std::make_shared<FInterface>();
    }
    std::shared_ptr<FMethod> createFMethod()
    {
        return std::make_shared<FMethod>();
    }
    std::shared_ptr<FBroadcast> createFBroadcast()
    {
        return std::make_shared<FBroadcast>();
    }
    std::shared_ptr<FAttribute> createFAttribute()
    {
        return std::make_shared<FAttribute>();
    }
    std::shared_ptr<FArgument> createFArgument()
    {
        return std::make_shared<FArgument>();
    }
    std::shared_ptr<FVersion> createFVersion()
    {
        return std::make_shared<FVersion>();
    }
    std::shared_ptr<FArrayType> createFArrayType()
    {
        return std::make_shared<FArrayType>();
    }
    std::shared_ptr<FTypeDef> createFTypeDef()
    {
        return std::make_shared<FTypeDef>();
    }
    std::shared_ptr<FVariable> createFVariable()
    {
        return std::make_shared<FVariable>();
    }
    std::shared_ptr<FStructType> createFStructType()
    {
        return std::make_shared<FStructType>();
    }
    std::shared_ptr<FEnumerator> createFEnumerator()
    {
        return std::make_shared<FEnumerator>();
    }
    std::shared_ptr<FEnumerationType> createFEnumerationType()
    {
        return std::make_shared<FEnumerationType>();
    }
    std::shared_ptr<FMapType> createFMapType()
    {
        return std::make_shared<FMapType>();
    }
    std::shared_ptr<FTypeRef> createFTypeRef()
    {
        return std::make_shared<FTypeRef>();
    }
    std::shared_ptr<FConstantDef> createFConstantDef()
    {
        return std::make_shared<FConstantDef>();
    }
    std::shared_ptr<FInitializer> createFInitializer()
    {
        return std::make_shared<FInitializer>();
    }
    std::shared_ptr<FCompoundInitializer> createFCompoundInitializer()
    {
        return std::make_shared<FCompoundInitializer>();
    }
    std::shared_ptr<FFieldInitializer> createFFieldInitializer()
    {
        return std::make_shared<FFieldInitializer>();
    }
    std::shared_ptr<FBracketInitializer> createFBracketInitializer()
    {
        return std::make_shared<FBracketInitializer>();
    }
    std::shared_ptr<FElementInitializer> createFElementInitializer()
    {
        return std::make_shared<FElementInitializer>();
    }
    std::shared_ptr<FAnnotation> createFAnnotation()
    {
        return std::make_shared<FAnnotation>();
    }
    std::shared_ptr<FAnnotationBlock> createFAnnotationBlock()
    {
        return std::make_shared<FAnnotationBlock>();
    }
    std::shared_ptr<FUnionType> createFUnionType()
    {
        return std::make_shared<FUnionType>();
    }
    std::shared_ptr<FField> createFField()
    {
        return std::make_shared<FField>();
    }
    std::shared_ptr<FEvaluableElement> createFEvaluableElement()
    {
        return std::make_shared<FEvaluableElement>();
    }
    std::shared_ptr<FBlock> createFBlock()
    {
        return std::make_shared<FBlock>();
    }
    std::shared_ptr<FAssignment> createFAssignment()
    {
        return std::make_shared<FAssignment>();
    }
    std::shared_ptr<FQualifiedElementRef> createFQualifiedElementRef()
    {
        return std::make_shared<FQualifiedElementRef>();
    }
    std::shared_ptr<FMethodErrorEnumRef> createFMethodErrorEnumRef()
    {
        return std::make_shared<FMethodErrorEnumRef>();
    }
    std::shared_ptr<FCurrentError> createFCurrentError()
    {
        return std::make_shared<FCurrentError>();
    }
    std::shared_ptr<FIntegerConstant> createFIntegerConstant()
    {
        return std::make_shared<FIntegerConstant>();
    }
    std::shared_ptr<FDoubleConstant> createFDoubleConstant()
    {
        return std::make_shared<FDoubleConstant>();
    }
    std::shared_ptr<FFloatConstant> createFFloatConstant()
    {
        return std::make_shared<FFloatConstant>();
    }
    std::shared_ptr<FBooleanConstant> createFBooleanConstant()
    {
        return std::make_shared<FBooleanConstant>();
    }
    std::shared_ptr<FStringConstant> createFStringConstant()
    {
        return std::make_shared<FStringConstant>();
    }
    std::shared_ptr<FBinaryOperation> createFBinaryOperation()
    {
        return std::make_shared<FBinaryOperation>();
    }
    std::shared_ptr<FUnaryOperation> createFUnaryOperation()
    {
        return std::make_shared<FUnaryOperation>();
    }
    std::shared_ptr<FIntegerInterval> createFIntegerInterval()
    {
        return std::make_shared<FIntegerInterval>();
    }

private:
    FFactory() = default;
    ~FFactory() = default;
    FFactory(const FFactory &) = delete;
    FFactory(FFactory &&) = delete;
};
} // namespace BstIdl

#endif
