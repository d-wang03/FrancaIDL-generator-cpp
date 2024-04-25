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
#ifndef F_OBJECT_H
#define F_OBJECT_H
#include <iostream>
#include <map>
#include <memory>
namespace BstIdl
{
class FTransformer;
class FObject
{
private:
    std::weak_ptr<FObject> m_container;

protected:
    std::string m_name;

public:
    FObject(const std::string &name) : m_name(name)
    {
    }
    FObject() = default;
    virtual ~FObject() = default;
    FObject(const FObject &) = default;
    FObject(FObject &&) noexcept = default;
    FObject &operator=(const FObject &) = default;
    FObject &operator=(FObject &&) = default;

    virtual std::string getName() const
    {
        return m_name;
    }
    void setName(const std::string &value)
    {
        m_name = value;
    }

    std::shared_ptr<FObject> getContainer() const
    {
        return m_container.lock();
    }
    void setContainer(const std::shared_ptr<FObject> &value)
    {
        m_container = value;
    }

    virtual std::string getFQN() const
    {
        return getContainer() ? (getContainer()->getFQN() + "." + getName()) : getName();
    }

    virtual std::string transform(std::string func)
    {
        return getName();
    }

    static bool addTransformer(const std::string &name, FTransformer *transformer)
    {
        if (hasTransformer(name))
            return false;
        else
            s_transMap[name] = transformer;
        return true;
    }

    static bool hasTransformer(const std::string &name)
    {
        if (s_transMap.count(name) == 0)
            return false;
        if (s_transMap.at(name) == nullptr)
            return false;
        return true;
    }

protected:
    static std::map<std::string, FTransformer *> s_transMap;
};
} // namespace BstIdl
#endif