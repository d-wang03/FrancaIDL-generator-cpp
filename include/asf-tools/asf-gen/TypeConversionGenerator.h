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
#ifndef TYPE_CONVERSION_GENERATOR_H
#define TYPE_CONVERSION_GENERATOR_H
#include "capicxx-core-tools/FTypeGenerator.h"
#include "generator/AbstractGenerator.h"
#include "model/FInterface.h"
#include "model/FMethod.h"
#include <map>
#include <memory>

namespace BstASF
{

class TypeConversionGenerator : public BstIdl::AbstractGenerator
{
private:
    TypeConversionGenerator() = default;
    virtual ~TypeConversionGenerator() = default;

    std::list<std::string> generateStructConversion(const std::shared_ptr<BstIdl::FTypeCollection> &object);
    void getHeaders(const std::shared_ptr<BstIdl::FInterface> &fInterface, std::list<std::string> &headers);
    std::string generateConversionHeader();

public:
    bool generate() override
    {
        return true;
    }
    static TypeConversionGenerator &getInstance()
    {
        static TypeConversionGenerator instance;
        return instance;
    }

    TypeConversionGenerator(const TypeConversionGenerator &) = delete;
    TypeConversionGenerator(TypeConversionGenerator &&) noexcept = delete;
    TypeConversionGenerator &operator=(const TypeConversionGenerator &) = delete;
    TypeConversionGenerator &operator=(TypeConversionGenerator &&) = delete;
    void generateConversionFile(const std::string &dir);
};

} // namespace BstASF
#endif