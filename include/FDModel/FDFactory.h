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
#ifndef F_D_FACTORY_H
#define F_D_FACTORY_H

#include "FDExtensionElement.h"
#include "FDExtensionRoot.h"
#include "FDImport.h"
#include "FDInterface.h"
#include "FDModel.h"
#include "FDSpecification.h"
#include "FDTypes.h"

#include "FDAbstractExtensionElement.h"
#include "FDArgument.h"
#include "FDArray.h"
#include "FDAttribute.h"
#include "FDBoolean.h"
#include "FDBroadcast.h"
#include "FDBuiltInPropertyHost.h"
#include "FDCompound.h"
#include "FDElement.h"
#include "FDEnumType.h"
#include "FDEnumValue.h"
#include "FDEnumeration.h"
#include "FDEnumerationOverwrites.h"
#include "FDExtensionType.h"
#include "FDField.h"
#include "FDGeneric.h"
#include "FDInteger.h"
#include "FDInterfaceRef.h"
#include "FDMap.h"
#include "FDMethod.h"
#include "FDPlainTypeOverwrites.h"
#include "FDPropertyDecl.h"
#include "FDPropertyFlag.h"
#include "FDString.h"
#include "FDStruct.h"
#include "FDStructOverwrites.h"
#include "FDTypedef.h"
#include "FDUnion.h"
#include "FDUnionOverwrites.h"
#include <algorithm>
#include <memory>

namespace BstIdl
{
class FDFactory
{
public:
    static FDFactory &getInstance();

    std::shared_ptr<FDModel> createFDModel();
    std::shared_ptr<FDImport> createImport();
    std::shared_ptr<FDSpecification> createSpecification();
    std::shared_ptr<FDRootElement> createDeployment();
    std::shared_ptr<FDDeclaration> createFDDeclaration();
    std::shared_ptr<FDRootElement> createFDRootElement();
    std::shared_ptr<FDExtensionRoot> createFDExtensionRoot();
    std::shared_ptr<FDExtensionElement> createFDExtensionElement();
    std::shared_ptr<FDTypes> createFDTypes();
    std::shared_ptr<FDInterface> createFDInterface();

private:
    FDFactory() = default;
    ~FDFactory() = default;
    FDFactory(const FDFactory &) = delete;
    FDFactory(FDFactory &&) = delete;
};

} // namespace BstIdl

#endif
