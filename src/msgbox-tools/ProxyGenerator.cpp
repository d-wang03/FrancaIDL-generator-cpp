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
#include "msgbox-tools/BareMetalProxyGenerator.h"
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
bool ProxyGenerator::generateHeader()
{
    std::string content = getProxyHeaderTpl();

    std::string clientName = m_infName + "_client";
    std::string headerMacro = toUpper(clientName) + "_H";
    std::string methodType;
    std::string methodDefs;
    std::string broadcastType;
    std::string broadcasts;
    std::string broadcastVarDecls;

    auto inf = m_interface->getTarget();

    for (const auto &m : inf->getMethods())
    {
        methodType.append(getProxyMethodAsyncCallbackSignature(m));
        methodDefs.append(getProxyMethodSyncSignature(m));
        methodDefs.append(getProxyMethodAsyncSignature(m));
    }
    replace_all(methodDefs, "\n", "\n\t");

    for (const auto &b : inf->getBroadcasts())
    {
        broadcastType.append(getProxyBroadcastCallbackSignature(b));
        broadcasts.append(getProxyBroadcastRegisterSignature(b));
        broadcastVarDecls.append(getProxyBroadcastInternalVarDecls(b));
    }
    replace_all(broadcasts, "\n", "\n\t");
    replace_all(broadcastVarDecls, "\n", "\n\t");
    trim(broadcastVarDecls);

    replace_all(content, "$LICENSE", getLicense());
    replace_all(content, "$VERSION_COMMENT", getVersionComment());
    replace_all(content, "$RTE_DEFINE", m_rtestr);
    replace_all(content, "$INF_NAME", m_infName);
    replace_all(content, "$CLIENT_NAME", clientName);
    replace_all(content, "$HEADER_MACRO", headerMacro);
    replace_all(content, "$METHOD_TYPE", methodType);
    replace_all(content, "$BROADCAST_TYPE", broadcastType);
    replace_all(content, "$METHODS", methodDefs);
    replace_all(content, "$BROADCASTS", broadcasts);
    replace_all(content, "$BROADCAST_VAR_DECLS", broadcastVarDecls);
    replace_all(content, "    ", "\t");
    replace_all(content, "\r\n", "\n");
    replace_all(content, "\t\n", "\n");

    // write file
    auto filename = m_folderPath + "/" + clientName + ".h";
    std::ofstream ofs(filename, std::ofstream::out | std::ios::binary);
    ofs << content;
    ofs.close();

    return true;
}

bool ProxyGenerator::generateSource()
{
    std::string content = getProxySourceTpl();

    replace_all(content, "$LICENSE", getLicense());
    // replace license comment style to pass checkpatch.pl
    replace_one(content, "/* SPDX-License-Identifier: GPL-2.0 OR Apache 2.0\n *", "// SPDX-License-Identifier: GPL-2.0 OR Apache 2.0\n/*");
    if (m_rtestr == "IPC_RTE_KERNEL")
    {
        replace_all(content, "static com_client_data_t *s_data = NULL;", "static com_client_data_t *s_data;");
        replace_all(content, "static $CLIENT_NAME_ext_t *s_ext = NULL;", "static $CLIENT_NAME_ext_t *s_ext;");
    }

    replace_all(content, "$VERSION_COMMENT", getVersionComment());
    replace_all(content, "$DISPATCH_MESSAGE", getProxyDispatchMessageTpl());

    std::string clientName = m_infName + "_client";
    replace_all(content, "$CLIENT_NAME", clientName);

    replace_all(content, "$PID_VALUE", m_srcEndID);
    replace_all(content, "$CID_VALUE", m_dstEndID);
    replace_all(content, "$FID_VALUE", m_fid);
    replace_all(content, "$SID_VALUE", std::to_string(m_sid).append("U"));

    std::string methodIDs;
    std::string methodOutStruct;
    std::string methodDefs;
    std::string methodCases;
    std::string methodInits;
    std::string methodDtors;
    for (const auto &m : m_interface->getMethods())
    {
        auto method = m->getTarget();
        methodIDs.append(getMethodCmds(m));
        methodDefs.append(getProxyMethodSerializeImpl(method));
        if (method->isFireAndForget())
        {
            methodDefs.append(getProxyMethodFireAndForgetfImpl(method));
        }
        else
        {
            methodDefs.append("#ifndef IPC_RTE_BAREMETAL\n");
            methodDefs.append(getProxyMethodSyncCallbackImpl(method));
            methodDefs.append(getProxyMethodSyncImpl(method));
            methodDefs.append("#endif\n");
            methodDefs.append(getProxyMethodAsyncImpl(method));
        }
        methodDefs.append(getProxyMethodInvokeCallbackImpl(method));

        methodOutStruct.append(getProxyMethodOutStruct(method));
        methodCases.append(getProxyMethodCase(method));
        methodInits.append(getProxyMethodSyncPtrInit(method));
        methodInits.append(getProxyMethodAsyncPtrInit(method));
        methodDtors.append(getProxyMethodAsyncPtrDestroy(method));
    }
    replace_all(content, "$METHOD_ID_DEF", methodIDs);
    replace_all(content, "$METHOD_DEFS", methodDefs);
    replace_one(methodCases, "else if", "if");
    trim(methodCases);
    replace_all(content, "$METHOD_REPLY_CASES", methodCases);
    replace_all(content, "$METHOD_REG_INIT", methodInits);
    replace_all(content, "$METHOD_REG_DESTROY", methodDtors);
    replace_all(content, "$METHOD_SYNC_OUT", methodOutStruct);

    // $BROADCAST_ID_DEF
    std::string broadcastIDs;
    std::string broadcastVars;
    std::string broadcastDefs;
    std::string broadcastCases;
    std::string broadcastReplyCases;
    std::string broadcastInits;
    std::string broadcastDtors;
    for (const auto &b : m_interface->getBroadcasts())
    {
        auto broadcast = b->getTarget();
        auto name = b->getTarget()->getName();
        auto fullname = m_infName + "_" + name;
        auto upperName = toUpper(name);
        broadcastIDs.append(getBroadcastCmds(b));

        broadcastVars.append(getProxyBroadcastVars(broadcast));
        broadcastDefs.append(getProxyBroadcastSubImpl(broadcast));
        broadcastDefs.append(getProxyBroadcastUnsubImpl(broadcast));
        broadcastDefs.append(getProxyBroadcastInvokeCallbackImpl(broadcast));
        broadcastCases.append(getProxyBroadcastCase(broadcast));
        broadcastReplyCases.append(getProxyBroadcastReplyCase(broadcast));
        broadcastInits.append(getProxyBroadcastPtrInit(broadcast));
        broadcastDtors.append(getProxyBroadcastPtrDestroy(broadcast));
    }
    replace_all(content, "$BROADCAST_ID_DEF", broadcastIDs);
    replace_all(content, "$BROADCAST_VARS", broadcastVars);
    replace_all(content, "$BROADCAST_DEFS", broadcastDefs);
    replace_one(broadcastCases, "else if", "if");
    trim(broadcastCases);
    trim(broadcastReplyCases);
    replace_all(content, "$BROADCAST_CASES", broadcastCases);
    replace_all(content, "$BROADCAST_REPLY_CASES", broadcastReplyCases);
    replace_all(content, "$BROADCAST_REG_INIT", broadcastInits);
    replace_all(content, "$BROADCAST_DESTROY", broadcastDtors);
    replace_all(content, "$VERSION", getVersionString());
    replace_all(content, "$INF_UPPER_NAME", toUpper(m_infName));
    replace_all(content, "    ", "\t");
    replace_all(content, "\r\n", "\n");
    replace_all(content, "\t\n", "\n");
    replace_all(content, "\n\t\t\n", "\n\n");

    // write file
    auto filename = m_folderPath + "/" + clientName + ".c";
    std::ofstream ofs(filename, std::ofstream::out | std::ios::binary);
    ofs << content;
    ofs.close();

    return true;
}

} // namespace BstIdl