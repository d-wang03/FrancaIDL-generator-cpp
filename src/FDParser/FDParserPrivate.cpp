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
#include "FDParserPrivate.h"

namespace BstIdl
{

FDParserPrivate::FDParserPrivate() : trace_parsing(false), trace_scanning(false), has_error(false)
{
}

bool FDParserPrivate::parse(const std::string &f)
{
    file = f;
    location.initialize(&file);
    has_error = false;
    std::cout << "Parsing FDEPL file:" << f << "..." << std::endl;
    modelName = file.substr(file.find_last_of('/') + 1);
    scan_begin();
    yy::parser parse(*this);
    parse.set_debug_level(trace_parsing);
    int res = parse.parse();
    scan_end();
    bool failed = (res != 0) || has_error;
    std::cout << "Parse " << std::string(failed ? "Failed!" : "Done!") << std::endl;
    return !failed;
}

int FDParserPrivate::parseString(const std::string &content)
{
    int res = 0;
    std::string filename = "RawString";
    location.initialize(&filename);
    scan_string_begin(content);
    yy::parser parse(*this);
    parse.set_debug_level(trace_parsing);
    try
    {
        res = parse();
    }
    catch (const yy::parser::syntax_error &e)
    {
        std::cerr << e.what() << '\n';
    }
    scan_string_end();
    return res;
}
} // namespace BstIdl