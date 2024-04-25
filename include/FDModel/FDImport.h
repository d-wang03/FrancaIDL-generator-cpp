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
#ifndef F_D_IMPORT_H
#define F_D_IMPORT_H
#include <iostream>
namespace BstIdl
{
class FDImport
{
protected:
    std::string m_importURI;
    std::string m_importedSpec;

public:
    FDImport() = default;
    virtual ~FDImport() = default;
    FDImport(const FDImport &) = default;
    FDImport(FDImport &&) noexcept = default;
    FDImport &operator=(const FDImport &) = default;
    FDImport &operator=(FDImport &&) = default;
    std::string const getImportURI()
    {
        return m_importURI;
    }
    void setImportURI(const std::string &importURI)
    {
        m_importURI = importURI;
    }
    std::string const getImportedSpec()
    {
        return m_importedSpec;
    }
    void setImportedSpec(const std::string &importedSpec)
    {
        m_importedSpec = importedSpec;
    }
};
} // namespace BstIdl
#endif