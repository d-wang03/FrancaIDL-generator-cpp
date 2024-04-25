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
#include "parser/FidlParser.h"
#include "FidlParserPrivate.h"
#include "model/FModelManager.h"
#include "validator/ModelValidator.h"

namespace BstIdl
{
const std::list<std::shared_ptr<FModel>> &FidlParser::parse(const std::string &filename)
{
    FModelManager &mgr = FModelManager::getInstance();
    mgr.clear();
    auto &refMgr = ReferenceManager::getInstance();
    refMgr.clear();
    FidlParserPrivate parser;
    auto ret = parser.parse(filename);
    bool failed = false;
    if (ret != 0)
        failed = true;
    while (!mgr.getPendingFidls().empty())
    {
        auto pFidl = mgr.getPendingFidls().front();
        mgr.getPendingFidls().pop_front();
        if (!pFidl.empty())
        {
            ret = parser.parse(pFidl);
            if (ret != 0)
                failed = true;
            else
            {
                mgr.addProcessedFidl(pFidl);
            }
        }
    }

    if (failed)
    {
        mgr.clear();
        return mgr.getModelList();
    }

    try
    {
        refMgr.resolveAllRef();
        refMgr.clear();
        ModelValidator::getInstance().validate();
    }
    catch (const std::runtime_error &e)
    {
        mgr.clear();
        std::string msg = simplify(std::string(e.what()));
        std::cerr << "Invalid model: " << msg << std::endl;
    }

    return mgr.getModelList();
}

std::shared_ptr<FModel> FidlParser::parseString(const std::string &content)
{
    auto &mgr = FModelManager::getInstance();
    mgr.clear();
    auto &refMgr = ReferenceManager::getInstance();
    refMgr.clear();
    FidlParserPrivate parser;
    auto ret = parser.parseString(content);
    if (ret != 0)
        return nullptr;

    try
    {
        refMgr.resolveAllRef();
        refMgr.clear();
        ModelValidator::getInstance().validateFModels();
    }
    catch (const std::runtime_error &e)
    {
        std::string msg = simplify(std::string(e.what()));
        std::cerr << "Invalid model: " << msg << std::endl;
        return nullptr;
    }

    return parser.currentModel;
}

int FidlParser::parseOne(const std::string &f)
{
    FidlParserPrivate parser;
    auto ret = parser.parse(f);
    return ret;
}
} // namespace BstIdl