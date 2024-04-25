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
#include "model/FStructType.h"

namespace BstIdl
{
std::shared_ptr<FEvaluableElement> FStructType::findElement(std::list<std::string> &fqn)
{
    if (fqn.empty())
        return nullptr;

    std::shared_ptr<BstIdl::FField> element;
    for (const auto &e : m_elements)
    {
        if (e->getName().compare(fqn.front()) == 0)
        {
            element = e;
            fqn.pop_front();
            break;
        }
    }

    if (!element && m_base)
    {
        element = std::static_pointer_cast<FField>(m_base->findElement(fqn));
    }

    if (!element)
        return nullptr;

    if (fqn.empty())
        return element;
    else
    {
        auto type = element->getType();
        if (!type)
            return nullptr;
        auto derived = type->getDerived();
        if (!derived)
            return nullptr;
        return derived->findElement(fqn);
    }
}
} // namespace BstIdl
