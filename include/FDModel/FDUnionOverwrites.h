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
#ifndef F_D_UNION_OVERWRITES_H
#define F_D_UNION_OVERWRITES_H
#include "FDCompoundOverwrites.h"
namespace BstIdl
{
class FDUnionOverwrites : public FDCompoundOverwrites
{
public:
    FDUnionOverwrites() : FDCompoundOverwrites()
    {
        setHost("unions");
    }
    virtual ~FDUnionOverwrites() = default;
    FDUnionOverwrites(const FDUnionOverwrites &) = default;
    FDUnionOverwrites(FDUnionOverwrites &&) noexcept = default;
    FDUnionOverwrites &operator=(const FDUnionOverwrites &) = default;
    FDUnionOverwrites &operator=(FDUnionOverwrites &&) = default;
};
} // namespace BstIdl
#endif