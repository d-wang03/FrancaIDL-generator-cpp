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
#ifndef F_D_MODEL_H
#define F_D_MODEL_H
#include "FDArgumentList.h"
#include "FDMapper.h"
#include "FDObject.h"
#include "FDOperation.h"
#include "FDRootElement.h"
#include "FDSpecification.h"

namespace BstIdl
{
class FDImport;
class FDModel : public FDObject
{
protected:
    std::list<std::shared_ptr<FDImport>> m_imports;
    std::list<std::shared_ptr<FDSpecification>> m_specifications;
    std::list<std::shared_ptr<FDRootElement>> m_deployments;
    std::map<std::shared_ptr<FDRootElement>, std::shared_ptr<FDMapper>> m_mappers;

public:
    FDModel() = default;
    virtual ~FDModel() = default;
    FDModel(const FDModel &) = default;
    FDModel(FDModel &&) noexcept = default;
    FDModel &operator=(const FDModel &) = default;
    FDModel &operator=(FDModel &&) = default;

    std::list<std::shared_ptr<FDImport>> &getImports()
    {
        return m_imports;
    }
    std::list<std::shared_ptr<FDSpecification>> &getSpecifications()
    {
        return m_specifications;
    }
    std::list<std::shared_ptr<FDRootElement>> &getDeployments()
    {
        return m_deployments;
    }

    std::string toGraph() override;

    void buildMappers();

    std::shared_ptr<FDMapper> getMapper(const std::shared_ptr<FDRootElement> &deploy) const;

    std::shared_ptr<FDInterface> getInterfaceDeployment(const std::string &spec, const std::string &name) const;

    std::shared_ptr<FDTypes> getTypeCollectionDeployment(const std::string &spec, const std::string &name) const;

    std::shared_ptr<FDExtensionRoot> getProviderDeployment(const std::string &spec) const;

    std::list<std::shared_ptr<FDInterface>> getInterfaceDeployments(const std::string &spec) const;
};
} // namespace BstIdl
#endif