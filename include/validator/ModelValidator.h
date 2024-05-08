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
#ifndef TYPE_VALIDATOR_H
#define TYPE_VALIDATOR_H

#include "FDModel/FDExtensionRoot.h"
#include "FDModel/FDInterface.h"
#include "FDModel/FDTypes.h"
#include "model/FTypeDef.h"
#include "model/FTypeRef.h"
#include "utilities/string_utility.h"
#include <map>
#include <memory>
#include <string>

namespace BstIdl
{
class FStructType;
class FEnumerationType;
class FUnionType;

class ModelValidator
{
public:
    static ModelValidator &getInstance();
    bool validate();
    bool validateFModels();
    bool validateFDModels();

private:
    ModelValidator() = default;
    ~ModelValidator() = default;

    std::list<std::string> validate(const std::shared_ptr<FType> &type);
    std::list<std::string> validate(const std::shared_ptr<FConstantDef> &constant);
    std::list<std::string> validate(const std::shared_ptr<FEnumerationType> &type,
                                    const std::shared_ptr<BstIdl::FObject> &object);
    std::list<std::string> validate(const std::shared_ptr<FArrayType> &fArray);
    std::list<std::string> validate(const std::shared_ptr<FArgument> &fArg);
    std::list<std::string> validate(const std::shared_ptr<FField> &field);
    std::list<std::string> validate(const std::shared_ptr<FStructType> &type);
    std::list<std::string> validate(const std::shared_ptr<FUnionType> &type);
    std::list<std::string> validate(const std::shared_ptr<FTypeDef> &type);
    std::list<std::string> validate(const std::list<std::shared_ptr<BstIdl::FMethod>> &elements,
                                    const std::string &location);
    std::list<std::string> validate(const std::list<std::shared_ptr<BstIdl::FBroadcast>> &elements,
                                    const std::string &location);
    std::list<std::string> validate(const std::shared_ptr<FDInterface> &fdInf);
    std::list<std::string> validate(const std::shared_ptr<FDTypes> &fdTc);
    std::list<std::string> validate(const std::shared_ptr<FDExtensionRoot> &fdEr);
    std::list<std::string> validateTypeDefinition(const std::shared_ptr<FDRootElement> &parent,
                                                  const std::list<std::shared_ptr<FDTypeDefinition>> &elements);
};
} // namespace BstIdl

#endif