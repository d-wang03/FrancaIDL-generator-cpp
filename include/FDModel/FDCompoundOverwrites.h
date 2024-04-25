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
#ifndef F_D_COMPOUND_OVERWRITES_H
#define F_D_COMPOUND_OVERWRITES_H
#include "FDCompound.h"
#include "FDTypeOverwrites.h"
namespace BstIdl
{
class FDCompoundOverwrites : public FDCompound, public FDTypeOverwrites
{
public:
    FDCompoundOverwrites() = default;
    virtual ~FDCompoundOverwrites() = default;
    FDCompoundOverwrites(const FDCompoundOverwrites &) = default;
    FDCompoundOverwrites(FDCompoundOverwrites &&) noexcept = default;
    FDCompoundOverwrites &operator=(const FDCompoundOverwrites &) = default;
    FDCompoundOverwrites &operator=(FDCompoundOverwrites &&) = default;
};
} // namespace BstIdl
#endif