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
#ifndef FIRE_AND_FORGET_LIMITS_HPP
#define FIRE_AND_FORGET_LIMITS_HPP

#include "model/FInterface.h"
#include "utilities/list_utility.h"
#include <list>

namespace BstIdl
{
class FireAndForgetLimits
{
public:
    static FireAndForgetLimits &getInstance()
    {
        static FireAndForgetLimits instance;
        return instance;
    }
    bool isAsfInterface(const std::shared_ptr<BstIdl::FInterface> &target) const
    {
        return contains(m_asfInf, target);
    }
    void addAsfInterface(const std::shared_ptr<BstIdl::FInterface> &target)
    {
        addListItem(m_asfInf, target);
    }

private:
    FireAndForgetLimits() = default;
    virtual ~FireAndForgetLimits() = default;
    std::list<std::shared_ptr<BstIdl::FInterface>> m_asfInf;
};
} // namespace BstIdl
#endif