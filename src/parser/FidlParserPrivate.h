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
#ifndef FIDL_PARSER_PRIVATE_H
#define FIDL_PARSER_PRIVATE_H

#include "model/FModel.h"
#include "model/FModelManager.h"
#include "parser.hh"
#include "validator/ReferenceManager.h"
#include <string>

namespace BstIdl
{

// Conducting the whole scanning and parsing of Calc++.
class FidlParserPrivate
{
public:
    FidlParserPrivate();

    // Run the parser on file F.  Return 0 on success.
    int parse(const std::string &f);
    // Run the parser on the content string.
    int parseString(const std::string &content);
    // The name of the file being parsed.
    std::string filename;
    // Whether to generate parser debug traces.
    bool trace_parsing;

    // Handling the scanner.
    void scan_begin();
    void scan_end();
    void scan_string_begin(const std::string &raw);
    void scan_string_end();
    // Whether to generate scanner debug traces.
    bool trace_scanning;
    // The token's location used by the scanner.
    bst::location location;

    // aux operations
    std::list<std::string> getPossibleFQN(const std::string &id);

    std::shared_ptr<FModel> currentModel;
    std::shared_ptr<FInterface> currentInterface;
    std::shared_ptr<FTypeCollection> currentTypeCollection;

    bool hasExtends;
    bool hasManages;
    bool hasErrorSection;
};
} // namespace BstIdl

// Give Flex the prototype of yylex we want ...
#define YY_DECL bst::parser::symbol_type yylex(BstIdl::FidlParserPrivate &drv)
// ... and declare it for the parser's sake.
YY_DECL;

#endif