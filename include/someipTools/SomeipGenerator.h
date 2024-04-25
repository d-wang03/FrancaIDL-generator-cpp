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
#ifndef SOMEIP_COMMONAPI_GENERATOR_H
#define SOMEIP_COMMONAPI_GENERATOR_H

#include "capicxx-core-tools/CapiGenerator.h"
#include "someipTools/SomeipPropertyAccessor.h"
#include <map>

namespace BstCommonAPI
{
class SomeipGenerator : public CommonapiGenerator
{
public:
    SomeipGenerator(const std::string &dest_dir, bool generate_stub) : CommonapiGenerator(dest_dir, generate_stub)
    {
    }

    virtual ~SomeipGenerator() = default;

    bool generate() override;
};

} // namespace BstCommonAPI

#endif