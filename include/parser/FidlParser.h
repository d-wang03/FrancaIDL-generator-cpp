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
#ifndef FIDL_PARSER_H
#define FIDL_PARSER_H

#include "model/FModel.h"
#include "model/FParser.h"
#include <memory>
#include <string>

namespace BstIdl
{

// Conducting the whole scanning and parsing of Calc++.
class FidlParser : public FParser
{
public:
    // Run the parser on file F.  Return 0 on success.
    const std::list<std::shared_ptr<FModel>> &parse(const std::string &filename);
    // Run the parser on the content string.
    static std::shared_ptr<FModel> parseString(const std::string &content);

    int parseOne(const std::string &f);
};
} // namespace BstIdl

#endif