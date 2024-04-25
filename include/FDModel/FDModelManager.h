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
#ifndef F_D_MODEL_MANAGER_H
#define F_D_MODEL_MANAGER_H

#include "FDArray.h"
#include "FDEnumValue.h"
#include "FDEnumeration.h"
#include "FDInterface.h"
#include "FDInterfaceRef.h"
#include "FDMap.h"
#include "FDMapper.h"
#include "FDModel.h"
#include "FDStruct.h"
#include "FDTypedef.h"
#include "FDTypes.h"
#include "FDUnion.h"
#include "model/FMapType.h"
#include "model/FModelManager.h"
#include "model/FTypeDef.h"
#include <list>
#include <memory>

namespace BstIdl
{
class FDModelManager
{
public:
    FDModelManager(const FDModelManager &) = delete;
    FDModelManager(FDModelManager &&) noexcept = delete;

    static FDModelManager &getInstance();

    std::shared_ptr<FDModel> addModel(const std::string &fqn);
    std::shared_ptr<FDModel> findModel(const std::string &fqn);
    std::shared_ptr<FDSpecification> findFDSpecification(const std::string &fqn);
    std::shared_ptr<FDPropertyDecl> findFDPropertyDecl(const std::string &fqn);
    std::shared_ptr<FDRootElement> findFDRootElement(const std::string &fqn);
    std::shared_ptr<FDInterface> findInterfaceDeployment(const std::string &spec,
                                                         const std::shared_ptr<BstIdl::FInterface> &target) const;
    std::shared_ptr<FDExtensionRoot> findProviderDeployment(const std::string &spec) const;
    std::shared_ptr<FDExtensionElement> findInstance(const std::string &spec, const std::string &targetName) const;
    std::string getFQN(const std::shared_ptr<FDInterfaceRef> &object) const;
    const std::list<std::shared_ptr<FDModel>> &getModelList() const;
    size_t size() const;
    bool empty() const;
    void clear();

    bool addPendingModel(const std::string &file);
    std::list<std::string> &getPendingModels();
    bool addProcessedModel(const std::string &file);
    bool findProcessedModel(const std::string &file);
    std::string makeModelName(std::string name);

    void createMapper();

private:
    FDModelManager() = default;
    ~FDModelManager() = default;

    std::list<std::shared_ptr<FDModel>> m_models;
    // list the import file
    std::list<std::string> pendingModels;
    std::list<std::string> processedModels;
    int anonymousModel;
};
} // namespace BstIdl

#endif