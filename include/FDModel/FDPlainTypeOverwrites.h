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
#ifndef F_D_PLAIN_TYPE_OVERWRITES_H
#define F_D_PLAIN_TYPE_OVERWRITES_H
#include "FDTypeOverwrites.h"
namespace BstIdl
{
class FDPlainTypeOverwrites : public FDTypeOverwrites
{

public:
    FDPlainTypeOverwrites() = default;
    virtual ~FDPlainTypeOverwrites() = default;
    FDPlainTypeOverwrites(const FDPlainTypeOverwrites &) = default;
    FDPlainTypeOverwrites(FDPlainTypeOverwrites &&) noexcept = default;
    FDPlainTypeOverwrites &operator=(const FDPlainTypeOverwrites &) = default;
    FDPlainTypeOverwrites &operator=(FDPlainTypeOverwrites &&) = default;
};
} // namespace BstIdl
#endif