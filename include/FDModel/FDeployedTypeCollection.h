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
#ifndef F_DEPLOYED_TYPE_COLLECTION_H
#define F_DEPLOYED_TYPE_COLLECTION_H
#include "MappingGenericPropertyAccessor.h"
namespace BstIdl
{
class FDeployedTypeCollection : public MappingGenericPropertyAccessor
{
protected:
    std::shared_ptr<FDTypes> m_ftypes;

public:
    FDeployedTypeCollection(std::shared_ptr<FDTypes> ftypes)
        : MappingGenericPropertyAccessor(ftypes->getSpec(), std::make_shared<FDMapper>(ftypes)), m_ftypes(ftypes)
    {
    }

    std::shared_ptr<BstIdl::FTypeCollection> getTypeCollection()
    {
        return m_ftypes->getTarget();
    }
};
} // namespace BstIdl
#endif