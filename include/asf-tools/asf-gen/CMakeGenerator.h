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
#ifndef CMAKE_GENERATOR
#define CMAKE_GENERATOR
#include <memory>

#include "generator/AbstractGenerator.h"
#include "model/FInterface.h"

namespace BstASF
{

class CMakeGenerator : public BstIdl::AbstractGenerator
{
private:
    std::string generateCMakeLists(const bool &test_gen);

public:
    CMakeGenerator() = default;
    virtual ~CMakeGenerator() = default;
    bool generate() override
    {
        return true;
    }
    static CMakeGenerator &getInstance()
    {
        static CMakeGenerator instance;
        return instance;
    }

    CMakeGenerator(const CMakeGenerator &) = delete;
    CMakeGenerator(CMakeGenerator &&) noexcept = delete;
    CMakeGenerator &operator=(const CMakeGenerator &) = delete;
    CMakeGenerator &operator=(CMakeGenerator &&) = delete;

    void generateCMake(const std::string &dir, const bool &test_gen);
};

} // namespace BstASF
#endif // CMAKE_GENERATOR
