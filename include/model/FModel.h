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
#ifndef F_MODEL_H
#define F_MODEL_H

#include "FObject.h"
#include "FTransformer.h"
#include "utilities/list_utility.h"
#include <list>
#include <memory>
#include <string>
namespace BstIdl
{
class FImport;
class FInterface;
class FTypeCollection;
class FModel : public FObject
{
public:
    FModel() = default;
    ~FModel() = default;
    FModel(const FModel &) = default;
    FModel(FModel &&) noexcept = default;
    FModel &operator=(const FModel &) = default;
    FModel &operator=(FModel &&) = default;

    std::list<std::shared_ptr<FImport>> &getImports()
    {
        return m_imports;
    }
    std::list<std::shared_ptr<FInterface>> &getInterfaces()
    {
        return m_interfaces;
    }
    std::list<std::shared_ptr<FTypeCollection>> &getTypeCollections()
    {
        return m_typeCollections;
    }

    void setFilenames(const std::string &filename)
    {
        addListItem(m_filenames, filename);
    }
    std::list<std::string> getFilenames() const
    {
        return m_filenames;
    }

    std::string transform(std::string func) override
    {
        std::string ret = getName();
        auto trans = FObject::s_transMap[func];
        if (trans && trans->forFModel)
            ret = trans->forFModel(this);
        return ret;
    }

private:
    std::list<std::shared_ptr<FImport>> m_imports;
    std::list<std::shared_ptr<FInterface>> m_interfaces;
    std::list<std::shared_ptr<FTypeCollection>> m_typeCollections;
    std::list<std::string> m_filenames;
};

} // namespace BstIdl

#endif