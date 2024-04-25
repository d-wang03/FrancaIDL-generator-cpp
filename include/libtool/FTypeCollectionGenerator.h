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
#ifndef F_TYPE_COLLECTION_GENERATOR
#define F_TYPE_COLLECTION_GENERATOR
#include "FTypeGenerator.h"
#include "FrancaGeneratorExtensions.h"
#include "generator/AbstractGenerator.h"
#include "model/FTypeCollection.h"
#include <algorithm>
#include <dirent.h>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>

namespace BstIdl
{
class FTypeCollectionGenerator : public AbstractGenerator
{
private:
    std::string generateHeader(const std::shared_ptr<FTypeCollection> &fTypeCollection);

    std::string generateSource(const std::shared_ptr<FTypeCollection> &fTypeCollection);

    std::list<std::string> getAllDerivedFStructTypeHeaderPaths(const std::shared_ptr<FTypeCollection> &fTypeCollection);

    bool needsSourceComment(std::shared_ptr<FType> &_type);

    bool hasSourceFile(const std::shared_ptr<FTypeCollection> &fTypeCollection);

public:
    FTypeCollectionGenerator() = default;
    virtual ~FTypeCollectionGenerator() = default;
    FTypeCollectionGenerator(const FTypeCollectionGenerator &) = default;
    FTypeCollectionGenerator(FTypeCollectionGenerator &&) noexcept = default;
    FTypeCollectionGenerator &operator=(const FTypeCollectionGenerator &) = default;
    FTypeCollectionGenerator &operator=(FTypeCollectionGenerator &&) = default;

    static FTypeCollectionGenerator &getInstance()
    {
        static FTypeCollectionGenerator instance;
        return instance;
    }

    bool generate() override
    {
        return true;
    };

    void generate(const std::shared_ptr<FTypeCollection> &fTypeCollection, const std::string &dir);

    void getRequiredHeaderFiles(const std::shared_ptr<FTypeCollection> &fTypeCollection,
                                std::list<std::string> &generatedHeaders, std::list<std::string> &libraryHeaders);
};

} // namespace BstIdl
#endif