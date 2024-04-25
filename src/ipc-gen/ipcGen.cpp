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
#include "ipc-gen/ipcGen.h"
#include "FDParser/FDParser.h"
#include "parser/FidlParser.h"
#include "utilities/path_utility.h"
namespace fs = std::experimental::filesystem;

namespace BstIdl
{

bool IPCGenerator::generate()
{
    if (m_destDir.empty())
        return false;

    // clear existing files
    fs::remove_all(m_destDir);

    auto &fmodels = BstIdl::FModelManager::getInstance();
    auto &fdmodels = BstIdl::FDModelManager::getInstance();
    if (fmodels.size() == 0 && fdmodels.size() == 0)
    {
        std::cerr << "No valid models!" << std::endl;
        return false;
    }
    auto &common_gen = IPCCommonGenerator::getInstance();
    transformerDefine();
    for (auto model : fdmodels.getModelList())
    {
        auto elems = model->getDeployments();
        for (auto elem : elems)
        {
            auto interface = std::dynamic_pointer_cast<FDInterface>(elem);
            if (interface != nullptr)
            {
                common_gen.generatePrefix(interface);
                std::cout << "Getting Client_ID and Server_ID for IPC Generator ..." << std::endl;
                int client_number, server_number;
                getClientAndServerID(interface, client_number, server_number);
                common_gen.setClientAndServerID(client_number, server_number);
                std::cout << "Generating include files for client ..." << std::endl;
                auto &clientIncGen = IPCClientIncludeGenerator::getInstance();
                clientIncGen.setLicense(getLicense());
                common_gen.setTypeTransName(false);
                clientIncGen.generateClientInclude(interface, m_destDir);
                std::cout << "Done" << std::endl;
                std::cout << "Generating source files for client ..." << std::endl;
                auto &clientSrcGen = IPCClientSourceGenerator::getInstance();
                clientSrcGen.setLicense(getLicense());
                common_gen.setTypeTransName(false);
                clientSrcGen.generateClientSource(interface, m_destDir);
                std::cout << "Done" << std::endl;
                if (m_generateStub)
                {
                    std::cout << "Generating include files for server ..." << std::endl;
                    auto &serverIncGen = IPCServerIncludeGenerator::getInstance();
                    serverIncGen.setLicense(getLicense());
                    common_gen.setTypeTransName(true);
                    serverIncGen.generateServerInclude(interface, m_destDir);
                    std::cout << "Done" << std::endl;
                    std::cout << "Generating source files for server ..." << std::endl;
                    auto &serverSrcGen = IPCServerSourceGenerator::getInstance();
                    serverSrcGen.setLicense(getLicense());
                    common_gen.setTypeTransName(true);
                    serverSrcGen.generateServerSource(interface, m_destDir);
                    std::cout << "Done" << std::endl;
                }
            }
        }
    }
    std::cout << "Copy IPC library files ..." << std::endl;
    auto path =
        CurrentAppPath::getInstance().parent_path().parent_path().append("plugin/com.bst.os.idl/deployment/ipc_gen");
    fs::copy(path, m_destDir);
    std::cout << "Done" << std::endl;

    return true;
}

void IPCGenerator::transformerDefine()
{
    IPCCommonGenerator *common_ins = &IPCCommonGenerator::getInstance();
    BstIdl::FTransformer *basic_trans = new BstIdl::FTransformer();
    basic_trans->forFBasicTypeId =
        std::bind(&IPCCommonGenerator::transformFBasicType, common_ins, std::placeholders::_1);
    basic_trans->forFArrayType = std::bind(&IPCCommonGenerator::getFArrayTypeName, common_ins, std::placeholders::_1);
    basic_trans->forFStructType = std::bind(&IPCCommonGenerator::getFStructTypeName, common_ins, std::placeholders::_1);
    basic_trans->forFUnionType = std::bind(&IPCCommonGenerator::getFUnionTypeName, common_ins, std::placeholders::_1);
    basic_trans->forFEnumerationType =
        std::bind(&IPCCommonGenerator::getFEnumrationTypeName, common_ins, std::placeholders::_1);
    basic_trans->forFTypeDef = std::bind(&IPCCommonGenerator::getFTypeDefName, common_ins, std::placeholders::_1);
    BstIdl::FObject::addTransformer("get_name", basic_trans);

    BstIdl::FTransformer *r5_basic_trans = new BstIdl::FTransformer();
    r5_basic_trans->forFBasicTypeId =
        std::bind(&IPCCommonGenerator::transformFBasicTypeForR5, common_ins, std::placeholders::_1);
    r5_basic_trans->forFArrayType =
        std::bind(&IPCCommonGenerator::getFArrayTypeName, common_ins, std::placeholders::_1);
    r5_basic_trans->forFStructType =
        std::bind(&IPCCommonGenerator::getFStructTypeName, common_ins, std::placeholders::_1);
    r5_basic_trans->forFUnionType =
        std::bind(&IPCCommonGenerator::getFUnionTypeName, common_ins, std::placeholders::_1);
    r5_basic_trans->forFEnumerationType =
        std::bind(&IPCCommonGenerator::getFEnumrationTypeName, common_ins, std::placeholders::_1);
    r5_basic_trans->forFTypeDef = std::bind(&IPCCommonGenerator::getFTypeDefName, common_ins, std::placeholders::_1);
    BstIdl::FObject::addTransformer("r5_get_name", r5_basic_trans);

    BstIdl::FTransformer *derived_type_trans = new BstIdl::FTransformer();
    derived_type_trans->forFArrayType =
        std::bind(&IPCCommonGenerator::transformFArrayType, common_ins, std::placeholders::_1);
    derived_type_trans->forFStructType =
        std::bind(&IPCCommonGenerator::transformFStructType, common_ins, std::placeholders::_1);
    derived_type_trans->forFUnionType =
        std::bind(&IPCCommonGenerator::transformFUnionType, common_ins, std::placeholders::_1);
    derived_type_trans->forFEnumerationType =
        std::bind(&IPCCommonGenerator::transformFEnumerationType, common_ins, std::placeholders::_1);
    derived_type_trans->forFTypeDef =
        std::bind(&IPCCommonGenerator::transformFTypeDef, common_ins, std::placeholders::_1);
    derived_type_trans->forFField = std::bind(&IPCCommonGenerator::transformFField, common_ins, std::placeholders::_1);
    BstIdl::FObject::addTransformer("transform_type", derived_type_trans);

    BstIdl::FTransformer *declare_attribute_trans = new BstIdl::FTransformer();
    declare_attribute_trans->forFAttribute =
        std::bind(&IPCCommonGenerator::transformDeclareFAttribute, common_ins, std::placeholders::_1);
    BstIdl::FObject::addTransformer("transform_declare_attribute", declare_attribute_trans);

    BstIdl::FTransformer *define_attribute_trans = new BstIdl::FTransformer();
    define_attribute_trans->forFAttribute =
        std::bind(&IPCCommonGenerator::transformDefineFAttribute, common_ins, std::placeholders::_1);
    BstIdl::FObject::addTransformer("transform_define_attribute", define_attribute_trans);
}

void IPCGenerator::getClientAndServerID(const std::shared_ptr<FDInterface> &fdInterface, int &client_number,
                                        int &server_number)
{
    for (auto property : fdInterface->getProperties())
    {
        if (property->getName() == "client")
        {
            auto fdvalue = property->getValue();
            if (!fdvalue->isSingle())
            {
                std::cerr << "Interface:" << fdInterface->getName() << " didn't have client number\n";
                return;
            }
            auto integer = std::dynamic_pointer_cast<FDInteger>(fdvalue->getSingle());
            if (integer == nullptr)
            {
                std::cerr << "Interface:" << fdInterface->getName() << " didn't have client number\n";
                return;
            }
            client_number = integer->getValue();
        }
        if (property->getName() == "server")
        {
            auto fdvalue = property->getValue();
            if (!fdvalue->isSingle())
            {
                std::cerr << "Interface:" << fdInterface->getName() << " didn't have server number\n";
                return;
            }
            auto integer = std::dynamic_pointer_cast<FDInteger>(fdvalue->getSingle());
            if (integer == nullptr)
            {
                std::cerr << "Interface:" << fdInterface->getName() << " didn't have server number\n";
                return;
            }
            server_number = integer->getValue();
        }
    }
}

} // namespace BstIdl