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
#include "FidlParserPrivate.h"

namespace BstIdl
{

FidlParserPrivate::FidlParserPrivate() : trace_parsing(false), trace_scanning(false)
{
}

int FidlParserPrivate::parse(const std::string &f)
{
    filename = f;
    location.initialize(&filename);
    std::cout << "Parsing FIDL file:" << f;
    scan_begin();
    bst::parser parse(*this);
    parse.set_debug_level(trace_parsing);
    int res = parse();
    scan_end();
    std::cout << "..." << std::string(res ? "Failed!" : "Done") << std::endl;
    return res;
}

int FidlParserPrivate::parseString(const std::string &content)
{
    int res = 0;
    auto &modelMgr = FModelManager::getInstance();
    filename = "RawString";
    location.initialize(&filename);
    scan_string_begin(content);
    bst::parser parse(*this);
    parse.set_debug_level(trace_parsing);
    try
    {
        res = parse();
    }
    catch (const bst::parser::syntax_error &e)
    {
        std::cerr << e.what() << '\n';
    }

    scan_string_end();
    return res;
}

std::list<std::string> FidlParserPrivate::getPossibleFQN(const std::string &id)
{
    std::list<std::string> ret;
    if (currentModel == nullptr)
        return ret;

    // check if id is already a valid FQN for current model.
    auto modelName = currentModel->getName();
    auto cmp_ret = modelName.compare(0, modelName.size(), id, 0, modelName.size());
    if (cmp_ret == 0 && id.size() > modelName.size() && id.at(modelName.size()) == '.')
    {
        ret.emplace_back(id);
        return ret;
    }

    // if id is a valid FQN, pointing to another model.
    ret.emplace_back(id);

    // id is not a valid FQN.
    // checking imports
    for (const auto &item : currentModel->getImports())
    {
        auto prefix = item->getImportedNamespace();
        size_t length = prefix.size();
        if (length <= 2)
            continue;
        if (prefix.at(length - 2) == '.' && prefix.at(length - 1) == '*')
            prefix = prefix.substr(0, length - 1);
        ret.emplace_back(prefix + id);
    }
    // checking current model scope.
    ret.emplace_back(currentModel->getName() + "." + id);
    // checking under current type collection
    if (currentTypeCollection != nullptr)
    {
        auto prefix = modelName + "." + currentTypeCollection->getName() + ".";
        ret.emplace_back(prefix + id);
    }
    if (currentInterface != nullptr)
    {
        auto prefix = modelName + "." + currentInterface->getName() + ".";
        ret.emplace_back(prefix + id);
    }

    return ret;
}

} // namespace BstIdl