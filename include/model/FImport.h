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
#ifndef F_IMPORT_H
#define F_IMPORT_H
#include <string>
namespace BstIdl
{
class FImport
{
public:
    FImport(const std::string &namesapce, const std::string &uri) : m_namespace(namesapce), m_uri(uri)
    {
    }
    FImport() = default;
    ~FImport() = default;
    FImport(const FImport &) = default;
    FImport(FImport &&) noexcept = default;
    FImport &operator=(const FImport &) = default;
    FImport &operator=(FImport &&) = default;

    std::string getImportedNamespace() const
    {
        return m_namespace;
    }
    void setImportedNamespace(const std::string &value)
    {
        m_namespace = value;
    }
    std::string getImportURI() const
    {
        return m_uri;
    }
    void setImportURI(const std::string &value)
    {
        m_uri = value;
    }

private:
    std::string m_namespace;
    std::string m_uri;
};

} // namespace BstIdl

#endif