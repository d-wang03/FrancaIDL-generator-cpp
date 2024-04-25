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
#include "FDModel/FDFactory.h"
#include "FDModel/FDObject.h"
namespace BstIdl
{
FDFactory &FDFactory::getInstance()
{
    static FDFactory ins;
    return ins;
}
std::shared_ptr<FDModel> FDFactory::createFDModel()
{
    return std::make_shared<FDModel>();
}
std::shared_ptr<FDImport> FDFactory::createImport()
{
    return std::make_shared<FDImport>();
}
std::shared_ptr<FDSpecification> FDFactory::createSpecification()
{
    return std::make_shared<FDSpecification>();
}
std::shared_ptr<FDRootElement> FDFactory::createDeployment()
{
    return std::make_shared<FDRootElement>();
}
std::shared_ptr<FDDeclaration> FDFactory::createFDDeclaration()
{
    return std::make_shared<FDDeclaration>();
}
std::shared_ptr<FDRootElement> FDFactory::createFDRootElement()
{
    return std::make_shared<FDRootElement>();
}
std::shared_ptr<FDExtensionRoot> FDFactory::createFDExtensionRoot()
{
    return std::make_shared<FDExtensionRoot>();
}
std::shared_ptr<FDExtensionElement> FDFactory::createFDExtensionElement()
{
    return std::make_shared<FDExtensionElement>();
}
std::shared_ptr<FDTypes> FDFactory::createFDTypes()
{
    return std::make_shared<FDTypes>();
}
std::shared_ptr<FDInterface> FDFactory::createFDInterface()
{
    return std::make_shared<FDInterface>();
}
} // namespace BstIdl