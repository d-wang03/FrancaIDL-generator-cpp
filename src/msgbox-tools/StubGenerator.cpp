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
#include "msgbox-tools/BareMetalStubGenerator.h"
#include "FDModel/FDEnumerator.h"
#include "FDModel/FDModelManager.h"
#include "msgbox-tools/transformer/typedecl.h"
#include "msgbox-tools/transformer/typename.h"
#include "msgbox-tools/transformer/deserialize.h"
#include "msgbox-tools/transformer/serialize.h"
#include "utilities/string_utility.h"
#include <fstream>

namespace BstIdl
{

bool StubGenerator::generateHeader()
{
    std::string content = getStubHeaderTpl();
    replace_all(content, "$OTHERS", R"(/**
     * Receive a message from the server.
     *
     * @return 0 if success, negative if fail.
     */
    int32_t (*receive_message)(void);

    /**
     * Dispatch a message to the server.
     *
     * @return 0 if success, negative if fail.
     */
    int32_t (*dispatch_message)(void);)");
    replace_all(content, "$INTERNEL_DATA_STRUCT", R"(
/**
 *  The internal data used by the server.
 *  Users should define an instance and pass it to the server initialization function.
 */
struct _$SERVER_NAME_data_t {
    $SERVER_NAME_t server;
    ipc_inf_version_t version;
    uint8_t handle;
    bool initialized;
    des_buf_t des_buf;
    $METHOD_VAR_DECLS
    $BROADCAST_VAR_DECLS
    serdes_t serializer;
    serdes_t deserializer;
    ext_info_t info;
};
#define $SERVER_NAME_data_t struct _$SERVER_NAME_data_t
)");
    replace_all(content, "$SERVER_INIT", R"(/**
 * Initializes the server.
 *
 * @param data The data to be used by the server.
 * @return A pointer to the initialized server, NULL if fail.
 */
$SERVER_NAME_t *$SERVER_NAME_init($SERVER_NAME_data_t *data);)");

    std::string serverName = m_infName + "_server";
    std::string headerMacro = toUpper(serverName) + "_H";
    std::string methodType;
    std::string methodRegs;
    std::string methodVarDecls;
    std::string broadcasts;
    std::string broadcastVarDecls;

    auto inf = m_interface->getTarget();

    for (const auto &m : inf->getMethods())
    {
        methodType.append(getStubMethodSignature(m));
        methodRegs.append(getStubMethodRegisterSignature(m));
        if (m_bSeparateReply)
            methodRegs.append(getStubMethodReplySignature(m));
        methodVarDecls.append(getStubMethodInternalVarDecls(m));
    }
    replace_all(methodRegs, "\n", "\n\t");
    replace_all(methodVarDecls, "\n", "\n\t");
    trim(methodVarDecls);

    for (const auto &b : inf->getBroadcasts())
    {
        broadcasts.append(getStubBroadcastSignature(b));
        broadcastVarDecls.append(getStubBroadcastInternalVarDecls(b));
    }
    replace_all(broadcasts, "\n", "\n\t");
    replace_all(broadcastVarDecls, "\n", "\n\t");
    trim(broadcastVarDecls);

    replace_all(content, "$LICENSE", getLicense());
    replace_all(content, "$VERSION_COMMENT", getVersionComment());
    replace_all(content, "$RTE_DEFINE", m_rtestr);
    replace_all(content, "$INF_NAME", m_infName);
    replace_all(content, "$SERVER_NAME", serverName);
    replace_all(content, "$HEADER_MACRO", headerMacro);
    replace_all(content, "$METHOD_TYPE", methodType);
    replace_all(content, "$METHOD_REGS", methodRegs);
    replace_all(content, "$BROADCASTS", broadcasts);
    replace_all(content, "$METHOD_VAR_DECLS", methodVarDecls);
    replace_all(content, "$BROADCAST_VAR_DECLS", broadcastVarDecls);
    replace_all(content, "    ", "\t");
    replace_all(content, "\r\n", "\n");
    replace_all(content, "\t\n", "\n");

    // write file
    auto filename = m_folderPath + "/" + serverName + ".h";
    std::ofstream ofs(filename, std::ofstream::out | std::ios::binary);
    ofs << content;
    ofs.close();

    return true;
}

bool StubGenerator::generateSource()
{
    std::string content = getStubSourceTpl();
    replace_all(content, "$LICENSE", getLicense());
    // replace license comment style to pass checkpatch.pl
    replace_one(content, "/* SPDX-License-Identifier: GPL-2.0 OR Apache 2.0\n *", "// SPDX-License-Identifier: GPL-2.0 OR Apache 2.0\n/*");

    if (m_rtestr == "IPC_RTE_KERNEL")
    {
        replace_all(content, "static com_server_data_t *s_data = NULL;", "static com_server_data_t *s_data;");
        replace_all(content, "static $SERVER_NAME_ext_t *s_ext = NULL;", "static $SERVER_NAME_ext_t *s_ext;");
    }

    replace_all(content, "$VERSION_COMMENT", getVersionComment());
    replace_all(content, "$REG_MAP_LOAD_AND_EXPORT", getStubRegistryMapTpl());
    replace_all(content, "$DISPATCH_MESSAGE", getStubDispatchMessageTpl());

    std::string serverName = m_infName + "_server";
    replace_all(content, "$SERVER_NAME", serverName);

    replace_all(content, "$PID_VALUE", m_srcEndID);
    replace_all(content, "$FID_VALUE", m_fid);
    replace_all(content, "$SID_VALUE", std::to_string(m_sid).append("U"));
    replace_all(content, "$MAX_METHOD_NUM", std::to_string(m_maxMethodNum).append("U"));
    replace_all(content, "$MAX_BROADCAST_NUM", std::to_string(m_maxBroadcastNum).append("U"));

    std::string methodVars;
    std::string methodIDs;
    std::string methodDefs;
    std::string methodCases;
    std::string methodInits;
    // std::string methodDtors;
    std::string cmdRegs;
    for (const auto &m : m_interface->getMethods())
    {
        auto method = m->getTarget();
        auto name = method->getName();
        methodIDs.append(getMethodCmds(m));
        methodVars.append(getStubMethodVars(method));
        methodDefs.append(getStubMethodRegisterImpl(method));
        methodDefs.append(getStubMethodCallFunc(method));
        methodCases.append(getStubMethodCase(method));
        methodInits.append(getStubMethodPtrInit(method));
        cmdRegs.append(getStubCmdRegStr(name));
    }
    replace_one(methodCases, "else if", "if");
    trim(methodCases);
    replace_all(content, "$METHOD_ID_DEF", methodIDs);
    replace_all(content, "$METHOD_VARS", methodVars);
    replace_all(content, "$METHOD_DEF", methodDefs);
    replace_all(content, "$METHOD_CASE", methodCases);
    replace_all(content, "$METHOD_REG_INIT", methodInits);

    // $BROADCAST_ID_DEF
    std::string broadcastIDs;
    std::string broadcastVars;
    std::string broadcastDefs;
    std::string broadcastCases;
    std::string broadcastInits;
    std::string broadcastSubSize;
    std::string broadcastExpRegistry;
    std::string broadcastRegistryCases;
    std::string exportRegMapVarsDef;
    std::string exportRegMapRetVar;

    for (const auto &b : m_interface->getBroadcasts())
    {
        auto broadcast = b->getTarget();
        auto name = broadcast->getName();
        broadcastIDs.append(getBroadcastCmds(b));
        broadcastVars.append(getStubBroadcastVars(broadcast));
        broadcastDefs.append(getStubBroadcastCallFunc(broadcast));
        broadcastCases.append(getStubBroadcastCase(broadcast));
        broadcastInits.append(getStubBroadcastPtrInit(broadcast));
        broadcastSubSize.append(getStubBroadcastSubscribedSize(broadcast));
        broadcastExpRegistry.append(getStubBroadcastExportRegistry(broadcast));
        broadcastRegistryCases.append(getStubBroadcastRegistryCase(broadcast));
        cmdRegs.append(getStubCmdRegStr("sub_" + name));
        cmdRegs.append(getStubCmdRegStr("unsub_" + name));
    }
    trim(broadcastSubSize);
    trim(broadcastExpRegistry);
    trim(broadcastRegistryCases);
    if (m_interface->getBroadcasts().empty())
    {
        exportRegMapVarsDef.append(R"(int32_t size_cnt = 0;)");
        exportRegMapRetVar = "0";
    }
    else
    {
        exportRegMapVarsDef.append(R"(int32_t ret = 0;
	int32_t exp_size = 0;
	int32_t size_cnt = 0;)");
        exportRegMapRetVar = "ret";
    }
    replace_all(content, "$BROADCAST_ID_DEF", broadcastIDs);
    replace_all(content, "$BROADCAST_VARS", broadcastVars);
    replace_all(content, "$BROADCAST_DEF", broadcastDefs);
    replace_all(content, "$REG_VARS_DEF", exportRegMapVarsDef);
    replace_all(content, "$RET_VAR", exportRegMapRetVar);
    replace_all(content, "$BROADCAST_CASE", trim(broadcastCases));
    replace_all(content, "$BROADCAST_REG_INIT", broadcastInits);
    replace_all(content, "$GET_ALL_SUB_SIZE", broadcastSubSize);
    replace_all(content, "$EXP_ALL_BROADCASTS_REG", broadcastExpRegistry);
    replace_all(content, "$BROADCAST_REG_CASE", broadcastRegistryCases);
    replace_all(content, "$CMD_REG", cmdRegs);
    replace_all(content, "$VERSION", getVersionString());
    replace_all(content, "    ", "\t");
    replace_all(content, "\r\n", "\n");
    replace_all(content, "\t\n", "\n");
    replace_all(content, "\n\t\t\n", "\n\n");
    replace_all(content, "}\n\t\telse if ", "} else if ");

    // write file
    auto filename = m_folderPath + "/" + serverName + ".c";
    std::ofstream ofs(filename, std::ofstream::out | std::ios::binary);
    ofs << content;
    ofs.close();

    return true;
}



} // namespace BstIdl