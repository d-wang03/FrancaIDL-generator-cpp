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
#include "FDParser/FDParser.h"
#include "FDModel/FDModelManager.h"
#include "FDParserPrivate.h"
#include "model/FModelManager.h"
#include "parser/FidlParser.h"
#include "utilities/path_utility.h"
#include "utilities/string_utility.h"
#include "validator/ModelValidator.h"

namespace BstIdl
{
const std::list<std::shared_ptr<FDModel>> &FDParser::parse(const std::string &filename)
{
    FDModelManager &depl_mgr = FDModelManager::getInstance();
    BstIdl::FModelManager &idl_mgr = BstIdl::FModelManager::getInstance();
    depl_mgr.clear();
    idl_mgr.clear();
    auto &depl_refMgr = FDReferenceManager::getInstance();
    depl_refMgr.clear();
    auto &idl_refMgr = BstIdl::ReferenceManager::getInstance();
    idl_refMgr.clear();
    FDParserPrivate depl_parser;
    BstIdl::FidlParser fidl_parser;
    bool failed = false;
    auto ret = depl_parser.parse(filename);
    if (!ret)
        failed = true;
    while (!depl_mgr.getPendingModels().empty())
    {
        auto filename = depl_mgr.getPendingModels().front();
        depl_mgr.getPendingModels().pop_front();
        if (!filename.empty())
        {
            ret = depl_parser.parse(filename);
            if (ret)
                depl_mgr.addProcessedModel(filename);
            else
                failed = true;
        }
    }

    while (!idl_mgr.getPendingFidls().empty())
    {
        auto filename = idl_mgr.getPendingFidls().front();
        idl_mgr.getPendingFidls().pop_front();
        if (!filename.empty())
        {
            auto ret = fidl_parser.parseOne(filename);
            if (ret == 0)
                depl_mgr.addProcessedModel(filename);
            else
                failed = true;
        }
    }

    idl_refMgr.resolveAllRef();
    // validate fmodels
    failed = (!ModelValidator::getInstance().validateFModels()) || failed;
    failed = (!depl_refMgr.resolveAllRef()) || failed;
    // validate fdmodels
    failed = (!ModelValidator::getInstance().validateFDModels()) || failed;
    // clear reference managers
    idl_refMgr.clear();
    depl_refMgr.clear();

    // build mappers between deploy model and model.
    depl_mgr.createMapper();

    if (failed)
    {
        idl_mgr.clear();
        depl_mgr.clear();
    }

    return depl_mgr.getModelList();
}

std::shared_ptr<FDModel> FDParser::parseString(const std::string &content)
{
    auto &mgr = FDModelManager::getInstance();
    mgr.clear();
    auto &depl_refMgr = FDReferenceManager::getInstance();
    depl_refMgr.clear();
    auto &idl_refMgr = BstIdl::ReferenceManager::getInstance();
    FDParserPrivate parser;
    auto ret = parser.parseString(content);
    if (ret != 0)
        return nullptr;
    try
    {
        idl_refMgr.resolveAllRef();
        depl_refMgr.resolveAllRef();
        std::cout << "resolveAllRef" << std::endl;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
        ;
    }

    return parser.currentModel;
}
} // namespace BstIdl