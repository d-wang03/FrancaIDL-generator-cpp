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
#ifndef PROVIDER_UTILS
#define PROVIDER_UTILS
#include "FDExtensionElement.h"
#include "FDExtensionRoot.h"
namespace BstIdl
{
class ProviderUtils
{
public:
    ProviderUtils() = default;
    virtual ~ProviderUtils() = default;
    ProviderUtils(const ProviderUtils &) = default;
    ProviderUtils(ProviderUtils &&) noexcept = default;
    ProviderUtils &operator=(const ProviderUtils &) = default;
    ProviderUtils &operator=(ProviderUtils &&) = default;

    static std::list<std::shared_ptr<FDExtensionElement>> getInstances(const std::shared_ptr<FDExtensionRoot> &provider)
    {
        std::list<std::shared_ptr<FDExtensionElement>> list;
        auto size = provider->getElements().size();
        for (auto it : provider->getElements())
        {
            if (it->getTag() == "instance")
            {
                list.emplace_back(it);
            }
        }
        return list;
    }
};
} // namespace BstIdl
#endif