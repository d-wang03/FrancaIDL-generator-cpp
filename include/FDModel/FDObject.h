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
#ifndef F_D_OBJECT
#define F_D_OBJECT
#include "fmt/core.h"
#include <memory>
namespace BstIdl
{
class FDObject
{

public:
    FDObject() = default;
    virtual ~FDObject() = default;
    FDObject(const FDObject &) = default;
    FDObject(FDObject &&) noexcept = default;
    FDObject &operator=(const FDObject &) = default;
    FDObject &operator=(FDObject &&) = default;
    std::string getName() const
    {
        return m_name;
    }
    std::string getInstanceName() const
    {
        // only use the lower 32 bits of this pointer as instance name's postfix.
        return m_name + "_" + fmt::format("{0:#x}", ((uint64_t)this & 0xFFFFFFU));
    }
    void setName(const std::string &value)
    {
        m_name = value;
    }
    std::shared_ptr<FDObject> getContainer() const
    {
        return m_container.lock();
    }
    void setContainer(const std::shared_ptr<FDObject> &value)
    {
        m_container = value;
    }
    virtual std::string toGraph()
    {
        return "object " + getInstanceName() + "\n";
    }
    std::string getFQN() const
    {
        return getContainer() ? (getContainer()->getFQN() + "." + m_name) : m_name;
    }

private:
    std::weak_ptr<FDObject> m_container;
    std::string m_name;
};
} // namespace BstIdl
#endif