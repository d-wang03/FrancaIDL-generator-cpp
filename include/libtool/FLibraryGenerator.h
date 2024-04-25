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
#ifndef F_LIBRARY_GENERATOR_H
#define F_LIBRARY_GENERATOR_H
#include "FInterfaceGenerator.h"
#include "FInterfaceProxyGenerator.h"
#include "FInterfaceStubGenerator.h"
#include "FTypeCollectionGenerator.h"
#include "generator/AbstractGenerator.h"
namespace BstIdl
{
class FLibraryGenerator : public AbstractGenerator
{
public:
    FLibraryGenerator(const std::string &dest_dir, bool generateStub)
        : AbstractGenerator(dest_dir), m_generateStub(generateStub)
    {
    }
    virtual ~FLibraryGenerator() = default;

    FLibraryGenerator(const FLibraryGenerator &) = delete;
    FLibraryGenerator(FLibraryGenerator &&) noexcept = delete;
    FLibraryGenerator &operator=(const FLibraryGenerator &) = delete;
    FLibraryGenerator &operator=(FLibraryGenerator &&) = delete;

    bool generate() override;

private:
    bool m_generateStub;
};

} // namespace BstIdl
#endif