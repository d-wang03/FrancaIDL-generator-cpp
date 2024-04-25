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
#include "FDModel/FDElement.h"
#include "FDModel/FDRootElement.h"
#include "FDModel/FDSpecification.h"
namespace BstIdl
{
void FDElement::updatePropsFromSpec()
{
    std::shared_ptr<FDSpecification> spec = nullptr;
    if (auto this_ptr = dynamic_cast<FDRootElement *>(this))
    {
        spec = this_ptr->getSpec();
    }
    // find spec in container
    auto container = getContainer();
    while (container && !spec)
    {
        if (auto e = std::dynamic_pointer_cast<FDRootElement>(container))
        {
            spec = e->getSpec();
            break;
        }
        else
            container = container->getContainer();
    }

    if (spec)
    {
        for (auto decl : spec->findDeclarations(m_host))
        {
            for (auto p : decl->getProperties())
            {
                if (p->isDefault())
                    m_defaultProps.emplace_back(p);
                else if (p->isOptional())
                    m_optionalProps.emplace_back(p);
                else
                    m_mandatoryProps.emplace_back(p);
            }
        }
    }
}
} // namespace BstIdl