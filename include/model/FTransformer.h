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
#ifndef F_TRANSFORMER_H
#define F_TRANSFORMER_H

#include <functional>
#include <string>

namespace BstIdl
{
class FArrayType;
class FAttribute;
class FArgument;
class FBroadcast;
class FConstantDef;
class FDeclaration;
class FEnumerationType;
class FEnumerator;
class FField;
class FIntegerInterval;
class FInterface;
class FMapType;
class FMethod;
class FModel;
class FStructType;
class FTypeCollection;
class FTypeDef;
class FTypeRef;
class FUnionType;
class FVariable;
class FTypeElement;
class FBasicTypeId;

class FTransformer
{
public:
    std::function<std::string(FArrayType *)> forFArrayType;
    std::function<std::string(FAttribute *)> forFAttribute;
    std::function<std::string(FArgument *)> forFArgument;
    std::function<std::string(FBroadcast *)> forFBroadcast;
    std::function<std::string(FConstantDef *)> forFConstantDef;
    std::function<std::string(FDeclaration *)> forFDeclaration;
    std::function<std::string(FEnumerationType *)> forFEnumerationType;
    std::function<std::string(FEnumerator *)> forFEnumerator;
    std::function<std::string(FField *)> forFField;
    std::function<std::string(FIntegerInterval *)> forFIntegerInterval;
    std::function<std::string(FInterface *)> forFInterface;
    std::function<std::string(FMapType *)> forFMapType;
    std::function<std::string(FMethod *)> forFMethod;
    std::function<std::string(FModel *)> forFModel;
    std::function<std::string(FBasicTypeId *)> forFBasicTypeId;
    std::function<std::string(FStructType *)> forFStructType;
    std::function<std::string(FTypeCollection *)> forFTypeCollection;
    std::function<std::string(FTypeDef *)> forFTypeDef;
    std::function<std::string(FTypeRef *)> forFTypeRef;
    std::function<std::string(FUnionType *)> forFUnionType;
    std::function<std::string(FVariable *)> forFVariable;
    std::function<std::string(FTypeElement *)> forFTypeElement;
};

} // namespace BstIdl

#endif