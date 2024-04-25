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
#ifndef F_D_PARSER_PRIVATE_H
#define F_D_PARSER_PRIVATE_H
#include "FDModel/FDModel.h"
#include "FDParser.hh"
#include "validator/FDReferenceManager.h"
#include <iostream>
#include <memory>
namespace BstIdl
{
class FDParserPrivate
{
public:
    FDParserPrivate();

    // Run the parser on file F.  Return 0 on success.
    bool parse(const std::string &f);
    // Run the parser on the content string.
    int parseString(const std::string &content);
    // The name of the file being parsed.
    std::string file;
    // Whether to generate parser debug traces.
    bool trace_parsing;
    // Whether to generate scanner debug traces.
    bool trace_scanning;
    // The token's location used by the scanner.
    yy::location location;

    void scan_begin();
    void scan_end();
    void scan_string_begin(const std::string &raw);
    void scan_string_end();
    std::shared_ptr<FDModel> currentModel;
    std::string modelName;

    bool has_error;
};
} // namespace BstIdl
// Give Flex the prototype of yylex we want ...
#define YY_DECL yy::parser::symbol_type yylex(BstIdl::FDParserPrivate &drv)
// ... and declare it for the parser's sake.
YY_DECL;
#endif