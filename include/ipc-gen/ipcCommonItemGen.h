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
#ifndef IPC_COMMON_ITEM_GEN_H
#define IPC_COMMON_ITEM_GEN_H
#include "FDModel/FDModelManager.h"
#include "capicxx-core-tools/FrancaGeneratorExtensions.h"
#include "generator/AbstractGenerator.h"
#include "model/FFactory.h"
#include "model/FModelManager.h"
#include "utilities/string_utility.h"
#include <fstream>
#include <iostream>

namespace BstIdl
{
class FEnumerationType;
class IPCCommonGenerator
{
private:
    IPCCommonGenerator() = default;
    ~IPCCommonGenerator() = default;
    IPCCommonGenerator(const IPCCommonGenerator &) = delete;
    IPCCommonGenerator(IPCCommonGenerator &&) noexcept = delete;
    IPCCommonGenerator &operator=(const IPCCommonGenerator &) = delete;
    IPCCommonGenerator &operator=(IPCCommonGenerator &&) = delete;

public:
    std::string prefix;
    static const std::string c_include_file;
    static const std::string c_r5_include_file;
    static const std::string c_annotation_block;
    // basic data type:A55
    const std::string c_UInt8 = "uint8_t";
    const std::string c_Int8 = "int8_t";
    const std::string c_UInt16 = "uint16_t";
    const std::string c_Int16 = "int16_t";
    const std::string c_UInt32 = "uint32_t";
    const std::string c_Int32 = "int32_t";
    const std::string c_UInt64 = "uint64_t";
    const std::string c_Int64 = "int64_t";
    const std::string c_Boolean = "bool";
    const std::string c_Float = "float";
    const std::string c_Double = "double";
    const std::string c_String = "char *";
    const std::string c_ByteBuffer = "size_t SIZE,char * NAME";
    // basic data type:R5
    const std::string r5_c_UInt8 = "uint8";
    const std::string r5_c_Int8 = "sint8";
    const std::string r5_c_UInt16 = "uint16";
    const std::string r5_c_Int16 = "sint16";
    const std::string r5_c_UInt32 = "uint32";
    const std::string r5_c_Int32 = "sint32";
    const std::string r5_c_UInt64 = "uint64";
    const std::string r5_c_Int64 = "sint64";
    const std::string r5_c_Boolean = "boolean";
    const std::string r5_c_Float = "float32";
    const std::string r5_c_Double = "float64";
    const std::string r5_c_String = "char *";
    const std::string r5_c_ByteBuffer = "size_t SIZE,char * NAME";
    // Composite data type
    static const std::string c_arrayType;
    static const std::string c_enumType;
    static const std::string c_StructType;
    static const std::string c_UnionType;
    static const std::string c_typedef;
    static const std::string c_attribute;

    /***method***/
    // ipc_init ipc_deinit declaration
    static const std::string c_ipc_init_part;
    static const std::string c_r5_ipc_init_part;
    int client_number;
    int server_number;
    std::string type_transName = std::string("get_name");

    static IPCCommonGenerator &getInstance()
    {
        static IPCCommonGenerator instance;
        return instance;
    }
    std::string makeModel(const std::shared_ptr<FModel> &model);
    void generatePrefix(const std::shared_ptr<FDInterface> &fdInterface);
    const std::string &getPrefix()
    {
        return prefix;
    }
    std::string transformFBasicType(FBasicTypeId *type);
    std::string transformFBasicTypeForR5(FBasicTypeId *type);
    std::string transformFArrayType(FArrayType *type);
    std::string transformFStructType(FStructType *type);
    std::string transformFUnionType(FUnionType *type);
    std::string transformFEnumerationType(FEnumerationType *type);
    std::string transformFTypeDef(FTypeDef *type);
    std::string transformFField(FField *field);
    std::string transformDeclareFAttribute(FAttribute *attribute);
    std::string transformDefineFAttribute(FAttribute *attribute);
    std::string assignAttribute(const std::shared_ptr<FTypeRef> type);
    std::string getFTypeDefName(FTypeDef *type);
    std::string getFArrayTypeName(FArrayType *type);
    std::string getFStructTypeName(FStructType *type);
    std::string getFUnionTypeName(FUnionType *type);
    std::string getFEnumrationTypeName(FEnumerationType *type);
    std::list<std::string> findAllUnionElement(FUnionType *type);
    std::string generateFTypes(const std::shared_ptr<FDInterface> &interface);
    std::string generateDeclareFAttribute(const std::shared_ptr<FDInterface> &interface);
    std::string generateDefineFAttribute(const std::shared_ptr<FDInterface> &interface);
    std::list<std::string> findAllEnumerators(FEnumerationType *type, std::string method_name);
    std::list<std::string> findAllEnumerators(FEnumerationType *type);
    std::list<std::string> findAllStructElement(FStructType *type);
    std::string generateIncludeFile(FModel *model);
    std::string getElementName(const std::shared_ptr<BstIdl::FModelElement> &fModelElement);
    std::string getDirectoryPath(const std::shared_ptr<BstIdl::FModel> &fModel);
    std::string getVersionPathPrefix(const std::shared_ptr<BstIdl::FTypeCollection> &_tc);
    // client & server ID type_transName
    void setClientAndServerID(const int &client_id, const int &server_id);
    std::pair<int, int> getCurrClientAndServerID() const;
    std::string getTypeTransName();
    void setTypeTransName(bool is_server);
    bool isR5Server() const;
    // get client & server file Path
    std::string getClientIncludeFile(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection);
    std::string getClientIncludePath(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection);
    std::string getClientSourceFile(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection);
    std::string getClientSourcePath(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection);
    std::string getServerIncludeFile(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection);
    std::string getServerIncludePath(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection);
    std::string getServerSourceFile(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection);
    std::string getServerSourcePath(const std::shared_ptr<BstIdl::FTypeCollection> &fTypeCollection);
    // get client & server gen funcs
    void getServerBroadcastArgsStrList(const std::list<std::shared_ptr<BstIdl::FArgument>> &out_args,
                                       std::list<std::string> &out_args_list) const;
    void getClientBroadcastArgsStrList(const std::list<std::shared_ptr<BstIdl::FArgument>> &out_args,
                                       std::list<std::string> &out_args_list) const;
    void getMethodOutArgsStrList(const std::list<std::shared_ptr<BstIdl::FArgument>> &out_args,
                                 std::list<std::string> &out_args_list) const;
    std::string getArgsPayloadSize(const std::list<std::shared_ptr<BstIdl::FArgument>> &args,
                                   std::string buffer_prefix) const;
    std::list<std::string> getArgsList(const std::list<std::shared_ptr<BstIdl::FArgument>> &args) const;
    void getAllServerArgsLists(const std::list<std::shared_ptr<BstIdl::FArgument>> &args,
                               const std::string &method_name, const bool &is_inArgs, std::list<std::string> &args_list,
                               std::list<std::string> &args_define_list, std::list<std::string> &args_ref_list) const;
    void getAllClientOutArgsLists(const std::list<std::shared_ptr<BstIdl::FArgument>> &args,
                                  std::list<std::string> &args_list, std::list<std::string> &args_define_list,
                                  std::list<std::string> &args_ref_list) const;
};

} // namespace BstIdl
#endif