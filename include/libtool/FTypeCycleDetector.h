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
#ifndef F_TYPE_CYCLE_DETECTOR_H
#define F_TYPE_CYCLE_DETECTOR_H
#include "FrancaGeneratorExtensions.h"
#include "model/FType.h"
#include <algorithm>
#include <map>
#include <vector>
namespace BstIdl
{
class FTypeCycleDetector
{
private:
    std::shared_ptr<FrancaGeneratorExtensions> francaGeneratorExtensions;
    std::map<std::shared_ptr<FType>, int> indices;
    std::map<std::shared_ptr<FType>, int> lowlink;
    std::vector<std::shared_ptr<FType>> stack;
    int index;
    bool tarjan(const std::shared_ptr<FType> &type);

public:
    FTypeCycleDetector() = default;
    virtual ~FTypeCycleDetector() = default;
    FTypeCycleDetector(const FTypeCycleDetector &) = default;
    FTypeCycleDetector(FTypeCycleDetector &&) noexcept = default;
    FTypeCycleDetector &operator=(const FTypeCycleDetector &) = default;
    FTypeCycleDetector &operator=(FTypeCycleDetector &&) = default;
    FTypeCycleDetector(std::shared_ptr<FrancaGeneratorExtensions> &francaGeneratorExtensions)
    {
        this->francaGeneratorExtensions = francaGeneratorExtensions;
    }
    std::string outErrorString;
    bool hasCycle(const std::shared_ptr<FType> &type);
    bool hasCycle(const std::list<std::shared_ptr<FType>> types);
};
} // namespace BstIdl

#endif