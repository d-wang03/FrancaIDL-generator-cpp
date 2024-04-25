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
#include "msgbox-tools/transformer/baremetal/deserialize.h"
#include "msgbox-tools/transformer/baremetal/serialize.h"
#include "msgbox-tools/transformer/baremetal/typedecl.h"
#include "msgbox-tools/transformer/baremetal/typename.h"
#include "utilities/string_utility.h"
#include <fstream>

namespace BstIdl
{
bool BareMetalProxyGenerator::generate()
{
    if (!validate())
    {
        std::cerr << "BareMetal Stub Generator validation failed." << std::endl;
        return false;
    }

    auto path = "/";
    m_folderPath = mk_dir(m_destDir, path);
    if (m_folderPath.empty())
        return false;

    bool ret = generateHeader();
    if (!ret)
    {
        std::cerr << "BareMetal Stub Generator header generation failed." << std::endl;
        return false;
    }

    ret = generateSource();
    if (!ret)
    {
        std::cerr << "BareMetal Stub Generator source generation failed." << std::endl;
        return false;
    }

    return true;
}

bool BareMetalProxyGenerator::validate()
{
    if (!m_instance || m_instance->getTag() != "instance" || !m_instance->getTarget())
        return false;

    m_infName.clear();
    m_fid.clear();
    m_sid = 255;
    m_interface = nullptr;

    // get and validate DstEndID
    auto value = m_instance->getSingleValue("DstEndID");
    if (!value)
    {
        std::cerr << "No DstEndID defined." << std::endl;
        return false;
    }
    auto type = value->getEnumerator();
    if (!type)
    {
        std::cerr << "Invalid DstEndID." << std::endl;
        return false;
    }
    m_dstEndID = type->getName();
    if (m_dstEndID != "CPU_0" && m_dstEndID != "CPU_1" && m_dstEndID != "CPU_2" && m_dstEndID != "CPU_3" &&
        m_dstEndID != "CPU_4" && m_dstEndID != "CPU_5" && m_dstEndID != "CPU_6" && m_dstEndID != "CPU_7" &&
        m_dstEndID != "CPUMP2_0" && m_dstEndID != "CPUMP2_1" && m_dstEndID != "ISPCV_0" && m_dstEndID != "ISPCV_1" &&
        m_dstEndID != "ISPCV_2" && m_dstEndID != "ISPCV_3" && m_dstEndID != "ISPCV_4" && m_dstEndID != "NET_0" &&
        m_dstEndID != "DMA_0" && m_dstEndID != "DMA_1" && m_dstEndID != "SWITCH_0" && m_dstEndID != "SWITCH_1" &&
        m_dstEndID != "SWITCH_2" && m_dstEndID != "SWITCH_3" && m_dstEndID != "SWITCH_4" && m_dstEndID != "SWITCH_5" &&
        m_dstEndID != "SECURE_0" && m_dstEndID != "SECURE_1" && m_dstEndID != "SAFETY_0" && m_dstEndID != "SAFETY_1" &&
        m_dstEndID != "REALTIME_0" && m_dstEndID != "REALTIME_1" && m_dstEndID != "REALTIME_2" &&
        m_dstEndID != "REALTIME_3" && m_dstEndID != "REALTIME_4" && m_dstEndID != "REALTIME_5" &&
        m_dstEndID != "MEDIA_0")
    {
        std::cerr << "Invalid DstEndID." << std::endl;
        return false;
    }

    // get and validate FID
    value = m_instance->getSingleValue("FID");
    if (!value)
    {
        std::cerr << "No FID defined." << std::endl;
        return false;
    }
    type = value->getEnumerator();
    if (!type)
    {
        std::cerr << "Invalid FID." << std::endl;
        return false;
    }
    m_fid = type->getName();
    if (m_srcEndID == "CPU_0" || m_srcEndID == "CPU_1" || m_srcEndID == "CPU_2" || m_srcEndID == "CPU_3" ||
        m_srcEndID == "CPU_4" || m_srcEndID == "CPU_5" || m_srcEndID == "CPU_6" || m_srcEndID == "CPU_7")
    {
        if (m_fid != "DEF" && m_fid != "F1" && m_fid != "F2" && m_fid != "F3" && m_fid != "F4" && m_fid != "F5" &&
            m_fid != "F6" && m_fid != "F7")
        {
            std::cerr << "Invalid FID." << std::endl;
            return false;
        }
    }
    else if (m_srcEndID == "CPUMP2_0" || m_srcEndID == "CPUMP2_1")
    {
        if (m_fid != "DEF" && m_fid != "F1")
        {
            std::cerr << "Invalid FID." << std::endl;
            return false;
        }
    }
    else
    {
        if (m_fid != "DEF" && m_fid != "F1" && m_fid != "F2" && m_fid != "F3")
        {
            std::cerr << "Invalid FID." << std::endl;
            return false;
        }
    }

    // get and validate SID.
    value = m_instance->getSingleValue("SID");
    if (!value)
    {
        std::cerr << "No SID defined." << std::endl;
        return false;
    }
    if (!value->isInteger())
    {
        std::cerr << "Invalid SID." << std::endl;
        return false;
    }
    m_sid = value->getInteger();
    if (m_sid < 0 || m_sid > 15)
    {
        std::cerr << "Invalid SID." << std::endl;
        return false;
    }

    // find FDInterface
    auto &fdmodels = BstIdl::FDModelManager::getInstance();
    if (fdmodels.size() == 0)
    {
        std::cerr << "No valid models!" << std::endl;
        return false;
    }
    for (const auto &model : fdmodels.getModelList())
    {
        for (const auto &d : model->getDeployments())
        {
            auto ptr = std::dynamic_pointer_cast<FDInterface>(d);
            if (!ptr)
                continue;
            auto spec = ptr->getSpec();
            if (!spec || spec->getName() != "com.bst.ipc.deployment")
                continue;
            auto target = ptr->getTarget();
            if (target.get() != m_instance->getTarget().get())
                continue;
            m_interface = ptr;
        }
    }
    if (!m_interface)
    {
        std::cerr << "No valid inteface!" << std::endl;
        return false;
    }

    // get stub name
    auto infName = m_interface->getName();
    m_infName = infName.substr(infName.find_last_of('.') + 1);

    // get MaxMethodNum
    bool ret = getIntegerProperty(m_interface, "MaxMethodNum", &m_maxMethodNum);
    if (!ret || m_maxMethodNum < 0 || m_maxMethodNum > 255)
    {
        std::cerr << "Invalid MaxMethodNum." << std::endl;
        return false;
    }

    // get MaxBroadcastNum
    ret = getIntegerProperty(m_interface, "MaxBroadcastNum", &m_maxBroadcastNum);
    if (!ret || m_maxBroadcastNum < 0 || m_maxBroadcastNum > 255)
    {
        std::cerr << "Invalid MaxBroadcastNum." << std::endl;
        return false;
    }

    return true;
}

bool BareMetalProxyGenerator::generateHeader()
{
    std::string content = R"($LICENSE

#ifndef $HEADER_MACRO
#define $HEADER_MACRO

#include "ipc_app_common.h"

#ifdef __cplusplus
extern "C"
{
#endif

    // user defined types
    $TYPES_DECL

    // constants
    $CONSTANT_DECL
    // method types
    $METHOD_TYPE
    // Broadcast types
    $BROADCAST_TYPE
    // Interface server
    typedef struct
    {
        // get version
        ipc_inf_version (*version)();

        // methods
        $METHODS
        // broadcasts
        $BROADCASTS
        // message router, call in main loop.
        // receive messages
        int32_t (*receive_message)();
        // dispatch message
        int32_t (*dispatch_message)();
    }$CLIENT_NAME;

    // init server
    $CLIENT_NAME *$CLIENT_NAME_init();
    // destroy client
    int32_t $CLIENT_NAME_destroy();

#ifdef __cplusplus
}
#endif

#endif // $HEADER_MACRO
)";
    std::string clientName = m_infName + "_client";
    std::string headerMacro = toUpper(clientName) + "_H";
    std::string typesDecl;
    std::string constantDecl;
    std::string methodType;
    std::string methodDefs;
    std::string broadcastType;
    std::string broadcasts;

    auto inf = m_interface->getTarget();
    for (const auto &t : inf->getTypes())
    {
        typesDecl.append(Transformer::BareMetal::getTypeDecl(t));
    }

    for (const auto &c : inf->getConstants())
    {
        constantDecl.append(Transformer::BareMetal::getTypeDecl(c));
    }
    replace_all(constantDecl, "\n", "\n\t");

    for (const auto &m : inf->getMethods())
    {
        std::string callback = "typedef void (*$NAME_callback_t)($ARGS);\n";
        std::list<std::string> arg_list;
        for (const auto &a : m->getOutArgs())
            arg_list.emplace_back("const " + Transformer::BareMetal::getTypeName(a->getType()) + " " + a->getName());
        if (auto ptr = m->getErrorType())
            arg_list.emplace_back("const " + Transformer::BareMetal::getTypeName(ptr) + " err");
        if (auto ptr = m->getErrors())
            typesDecl.append(Transformer::BareMetal::getTypeDecl(ptr));
        arg_list.emplace_back("void *ext");
        replace_all(callback, "$NAME", m_infName + "_" + m->getName());
        replace_all(callback, "$ARGS", join(arg_list, ", "));
        methodType.append(callback);

        methodDefs.append(getMethodAsyncSignature(m));
    }
    replace_all(methodType, "\n", "\n\t");
    replace_all(methodDefs, "\n", "\n\t\t");
    replace_all(typesDecl, "\n", "\n\t");

    for (const auto &b : inf->getBroadcasts())
    {
        std::string broadcast_type = R"(typedef void (*$FULL_NAME_callback_t)($ARGS);
typedef void (*$FULL_NAME_sub_callback_t)(int32_t err, void *ext);
typedef void (*$FULL_NAME_unsub_callback_t)(int32_t err, void *ext);
)";
        std::string fullname = m_infName + "_" + b->getName();
        std::list<std::string> arg_list;
        for (const auto &a : b->getOutArgs())
            arg_list.emplace_back("const " + Transformer::BareMetal::getTypeName(a->getType()) + " " + a->getName());
        arg_list.emplace_back("void *ext");
        replace_all(broadcast_type, "$ARGS", join(arg_list, ", "));
        replace_all(broadcast_type, "$FULL_NAME", fullname);
        broadcastType.append(broadcast_type);

        std::string broadcast_sub =
            R"(int32_t (*$NAME_sub)($FULL_NAME_callback_t cb, void *ext, $FULL_NAME_sub_callback_t cb2, void *ext2);
int32_t (*$NAME_unsub)($FULL_NAME_unsub_callback_t cb, void *ext);
)";
        replace_all(broadcast_sub, "$FULL_NAME", fullname);
        replace_all(broadcast_sub, "$NAME", b->getName());
        broadcasts.append(broadcast_sub);
    }
    replace_all(broadcastType, "\n", "\n\t");
    replace_all(broadcasts, "\n", "\n\t\t");

    replace_all(content, "$LICENSE", getLicense());
    replace_all(content, "$CLIENT_NAME", clientName);
    replace_all(content, "$HEADER_MACRO", headerMacro);
    replace_all(content, "$TYPES_DECL", typesDecl);
    replace_all(content, "$CONSTANT_DECL", constantDecl);
    replace_all(content, "$METHOD_TYPE", methodType);
    replace_all(content, "$BROADCAST_TYPE", broadcastType);
    replace_all(content, "$METHODS", methodDefs);
    replace_all(content, "$BROADCASTS", broadcasts);

    // write file
    auto filename = m_folderPath + "/" + clientName + ".h";
    std::ofstream ofs(filename, std::ofstream::out);
    ofs << content;
    ofs.close();

    return true;
}

bool BareMetalProxyGenerator::generateSource()
{
    std::string content = R"($LICENSE

#include "$CLIENT_NAME.h"
#include "ipc_app_common.h"
#include "ipc_app_serdes.h"
#include "ipc_app_client_utils.h"
#include "ipc_trans_common.h"
#include "ipc_trans_layer.h"

// macro definitions
#define PID $PID_VALUE
#define CID $CID_VALUE
#define FID $FID_VALUE
#define SID $SID_VALUE

$METHOD_ID_DEF
$BROADCAST_ID_DEF
// local variables
static ipc_inf_version s_version = $VERSION;
static uint32_t s_handle = 0U;
static $CLIENT_NAME s_client = {0};
static uint8_t s_token = 0;
static int8_t s_recv_buffer[IPC_MAX_DATA_SIZE] = {0};
static callback_registration_t s_method_registry[IPC_TOKEN_NUM] = {0};
$BROADCAST_VARS
static inline void increase_token()
{
    if (++s_token >= IPC_TOKEN_NUM)
        s_token = 0;
}

// interface implementation
// get interface version
static ipc_inf_version get_ipc_inf_version()
{
    return s_version;
}

// method
$METHOD_DEFS
// broadcast
$BROADCAST_DEFS
// receive messages
static int32_t receive_message()
{
    return ipc_trans_layer_get_msg(s_handle);
}

// dispatch messages
static int32_t dispatch_message()
{
    int32_t ret = 0;
    static serdes_t broadcast_des = {0};
    static serdes_t reply_des = {0};

    while (ipc_trans_layer_proxy_get_broadcast_msg(s_handle, ipc_des_get_current_msg(&broadcast_des)) >= 0)
    {
        ret = ipc_des_validate_msg(&broadcast_des);
        if (ret < 0)
            continue;
        switch (broadcast_des.header.cmd)
        {
        $BROADCAST_CASES
        default:
            break;
        }
        (void)ipc_des_init(&broadcast_des);
    }
    while (ipc_trans_layer_proxy_get_reply_msg(s_handle, ipc_des_get_current_msg(&reply_des)) >= 0)
    {
        ret = ipc_des_validate_msg(&reply_des);
        if (ret < 0)
            continue;
        switch (reply_des.header.cmd)
        {
        $METHOD_REPLY_CASES
        $BROADCAST_REPLY_CASES
        default:
            break;
        }
        (void)ipc_des_init(&reply_des);
        if (ret < 0)
            printf("deserialization failed.\n");
    }
    return ret;
}

// init server
$CLIENT_NAME *$CLIENT_NAME_init()
{
    // start trans layer.
    int32_t ret = ipc_trans_layer_start(1);
    if (ret < 0)
        return NULL;
    // create client handle.
    ret = ipc_trans_layer_proxy_create_handle(FID, SID, &s_handle);
    if (ret < 0)
        return NULL;

    s_client.version = get_ipc_inf_version;
    $METHOD_REG_INIT
    $BROADCAST_REG_INIT
    s_client.receive_message = receive_message;
    s_client.dispatch_message = dispatch_message;
    return &s_client;
}

// destory client
int32_t $CLIENT_NAME_destroy()
{
    int32_t ret = ipc_trans_layer_destory_handle(s_handle);
    if (ret < 0)
        return ret;
    ret = ipc_trans_layer_stop();
    if (ret < 0)
        return ret;

    s_client.version = NULL;
    $METHOD_REG_DESTROY
    $BROADCAST_DESTROY
    s_client.receive_message = NULL;
    s_client.dispatch_message = NULL;
    return ret;
}
)";

    replace_all(content, "$LICENSE", getLicense());

    std::string clientName = m_infName + "_client";
    replace_all(content, "$CLIENT_NAME", clientName);

    replace_all(content, "$PID_VALUE", m_srcEndID);
    replace_all(content, "$CID_VALUE", m_dstEndID);
    replace_all(content, "$FID_VALUE", m_fid);
    replace_all(content, "$SID_VALUE", std::to_string(m_sid).append("U"));

    std::string methodIDs;
    std::string methodDefs;
    std::string methodCases;
    std::string methodInits;
    std::string methodDtors;
    for (const auto &m : m_interface->getMethods())
    {
        auto method = m->getTarget();
        int32_t id = -1;
        auto ret = getIntegerProperty(m, "MethodID", &id);
        if (!ret || id < 0 || id >= m_maxMethodNum)
        {
            std::cerr << "Invalid MethodID." << std::endl;
            return false;
        }
        std::string name = method->getName();
        std::string upperName = toUpper(name);
        methodIDs.append("#define CMD_METHOD_").append(upperName).append(" ").append(std::to_string(id)).append("U\n");

        std::string asyncFunc = R"(
static int32_t call_$NAME_async($ARGS)
{
    int32_t ret = 0;
    serdes_t serdes = {0};
    serdes_t *ser = &serdes;

    // prepare message
    serdes.header.pid = PID;
    serdes.header.cid = CID;
    serdes.header.fid = FID;
    serdes.header.sid = SID;
    serdes.header.tok = s_token;
    serdes.header.cmd = CMD_METHOD_$UPPER_NAME;
    serdes.header.typ = IPC_MSG_TYPE_METHOD;

    // serialize
    (void)ipc_ser_init(ser);
    $SERIALIZE
    (void)ipc_ser_finish(ser);

	// check and set registry
	if (add_registry(&s_method_registry[s_token], (void *)cb, ext) != 0)
		return -2;

    // send message
    uint32_t i = 0;
    for (; i <= serdes.index; ++i)
    {
        ret = ipc_trans_layer_proxy_send_method(s_handle, serdes.msg_pool[i]);
        if (ret < 0)
            break;
    }
    if (i > serdes.index)
    {
        increase_token();
        return 0;
    }
    else
    {
		clear_registry(&s_method_registry[s_token]);
        printf("send method $NAME fail %d.\n", ret);
        return ret;
    }
}
)";
        std::string fullname = m_infName + "_" + name;
        replace_all(asyncFunc, "$UPPER_NAME", upperName);
        replace_all(asyncFunc, "$NAME", name);
        replace_all(asyncFunc, "$ARGS", getMethodAsyncArgs(method));
        std::string serialize;
        for (const auto &a : method->getInArgs())
        {
            serialize.append(BstIdl::Transformer::BareMetal::serialize(a));
        }
        replace_all(serialize, "\n", "\n\t");
        replace_all(asyncFunc, "$SERIALIZE", serialize);
        methodDefs.append(asyncFunc);

        std::string callbackFunc = R"(
static inline int32_t call_$NAME_callback(serdes_t *des)
{
    if (!des)
        return -1;
    
    int32_t ret = 0;
    uint32_t length = 0;

    $DESERIALIZE
    callback_registration_t *reg = &s_method_registry[des->header.tok];
    $FULLNAME_callback_t cb = ($FULLNAME_callback_t)(reg->cb);
    if (cb)
        cb($ARGS);
    clear_registry(reg);

    return 0;
}
)";
        std::string des;
        std::string initvars;
        std::list<std::string> arg_list;
        auto err = method->getErrorType();
        if (err)
        {
            auto errDes = BstIdl::Transformer::BareMetal::initVar(err);
            errDes.append(BstIdl::Transformer::BareMetal::deserialize(err));
            replace_all(errDes, "$NAME", "err");
            des.append(errDes);

            auto fastpath = R"(
if (err != NO_ERROR)
{
    $INIT_VARS
    callback_registration_t *reg = &s_method_registry[des->header.tok];
    $FULLNAME_callback_t cb = ($FULLNAME_callback_t)(reg->cb);
    if (cb)
        cb($ARGS);
    return 0;
}

)";
            des.append(fastpath);
        }
        for (const auto &a : method->getOutArgs())
        {
            arg_list.emplace_back(a->getName());
            des.append(BstIdl::Transformer::BareMetal::deserialize(a));
            initvars.append(BstIdl::Transformer::BareMetal::initVar(a));
        }
        if (err)
            arg_list.emplace_back("err");
        arg_list.emplace_back("reg->ext");
        replace_all(des, "\n", "\n\t");
        replace_all(initvars, "\n", "\n\t\t");

        replace_all(callbackFunc, "$DESERIALIZE", des);
        replace_all(callbackFunc, "$NAME", name);
        replace_all(callbackFunc, "$FULLNAME", fullname);
        replace_all(callbackFunc, "$INIT_VARS", initvars);
        replace_all(callbackFunc, "$ARGS", join(arg_list, ", "));
        methodDefs.append(callbackFunc);

        std::string method_case = R"(case CMD_METHOD_$UPPER_NAME: 
    ret = call_$NAME_callback(&reply_des);
    break;
)";
        replace_all(method_case, "$NAME", name);
        replace_all(method_case, "$UPPER_NAME", upperName);
        replace_all(method_case, "\n", "\n\t\t");
        methodCases.append(method_case);

        std::string reg_init = "s_client.$NAME = call_$NAME_async;\n\t";
        replace_all(reg_init, "$NAME", name);
        methodInits.append(reg_init);

        std::string reg_reset = "s_client.$NAME = NULL;\n\t";
        replace_all(reg_reset, "$NAME", name);
        methodDtors.append(reg_reset);
    }
    replace_all(content, "$METHOD_ID_DEF", methodIDs);
    replace_all(content, "$METHOD_DEFS", methodDefs);
    replace_all(content, "$METHOD_REPLY_CASES", methodCases);
    replace_all(content, "$METHOD_REG_INIT", methodInits);
    replace_all(content, "$METHOD_REG_DESTROY", methodDtors);

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
        auto name = b->getTarget()->getName();
        auto fullname = m_infName + "_" + name;
        auto upperName = toUpper(name);
        int32_t id = -1;
        auto ret = getIntegerProperty(b, "SubscribeMethodID", &id);
        if (!ret || id < m_maxMethodNum || id > 255)
        {
            std::cerr << "Invalid SubscribeMethodID." << std::endl;
            return false;
        }
        broadcastIDs.append("#define CMD_METHOD_SUB_")
            .append(upperName)
            .append(" ")
            .append(std::to_string(id))
            .append("U\n");

        ret = getIntegerProperty(b, "UnsubscribeMethodID", &id);
        if (!ret || id < m_maxMethodNum || id > 255)
        {
            std::cerr << "Invalid UnsubscribeMethodID." << std::endl;
            return false;
        }
        broadcastIDs.append("#define CMD_METHOD_UNSUB_")
            .append(upperName)
            .append(" ")
            .append(std::to_string(id))
            .append("U\n");

        ret = getIntegerProperty(b, "BroadcastID", &id);
        if (!ret || id < 0 || id >= m_maxBroadcastNum)
        {
            std::cerr << "Invalid BroadcastID." << std::endl;
            return false;
        }
        broadcastIDs.append("#define CMD_BROADCAST_")
            .append(upperName)
            .append(" ")
            .append(std::to_string(id))
            .append("U\n");

        broadcastVars.append("static callback_registration_t s_").append(name).append("_registry = {0};\n");

        std::string broadcast_func = R"(
// subscribe $NAME
static int32_t subscribe_$NAME($FULLNAME_callback_t cb, void *ext, $FULLNAME_sub_callback_t cb2,
                                        void *ext2)
{
    int32_t ret = 0;
    rw_msg msg = {0};

    // prepare message
    msg.header.pid = PID;
    msg.header.cid = CID;
    msg.header.fid = FID;
    msg.header.sid = SID;
    msg.header.tok = s_token;
    msg.header.cmd = CMD_METHOD_SUB_$UPPER_NAME;
    msg.header.typ = IPC_MSG_TYPE_METHOD;
    msg.header.len = 0;
    msg.header.is_eof = 1;

	// check and set registry
	if (add_registry(&s_$NAME_registry, (void *)cb, ext) != 0
		|| add_registry(&s_method_registry[s_token], (void *)cb2, ext2) != 0)
		return -2;

    // send message
    ret = ipc_trans_layer_proxy_send_method(s_handle, msg);
    if (ret < 0)
    {
		clear_registry(&s_$NAME_registry);
		clear_registry(&s_method_registry[s_token]);
        printf("send method subscribe_$NAME fail %d.\n", ret);
        return ret;
    }
    else
    {
        increase_token();
        return 0;
    }
}

// unsubscribe $NAME
static int32_t unsubscribe_$NAME($FULLNAME_unsub_callback_t cb, void *ext)
{
    int32_t ret = 0;
    rw_msg msg = {0};

    // prepare message
    msg.header.pid = PID;
    msg.header.cid = CID;
    msg.header.fid = FID;
    msg.header.sid = SID;
    msg.header.tok = s_token;
    msg.header.cmd = CMD_METHOD_UNSUB_$UPPER_NAME;
    msg.header.typ = IPC_MSG_TYPE_METHOD;
    msg.header.len = 0;
    msg.header.is_eof = 1;

	// check and set registry
	if (add_registry(&s_method_registry[s_token], (void *)cb, ext) != 0)
		return -2;

    // send message
    ret = ipc_trans_layer_proxy_send_method(s_handle, msg);
    if (ret < 0)
    {
		clear_registry(&s_method_registry[s_token]);
        printf("send method unsubscribe_$NAME fail %d.\n", ret);
        return ret;
    }
    else
    {
        increase_token();
        return 0;
    }
}

static inline int32_t call_$NAME_callback(serdes_t *des)
{
    if (!des)
        return -1;
    
    int32_t ret = 0;
    uint32_t length = 0;

    $DESERIALIZE
    callback_registration_t *reg = &s_$NAME_registry;
    $FULLNAME_callback_t cb = ($FULLNAME_callback_t)(reg->cb);
    if (cb)
        cb($ARGS);

    return 0;
}
)";
        std::string des;
        std::list<std::string> arg_list;
        for (const auto &a : b->getTarget()->getOutArgs())
        {
            arg_list.emplace_back(a->getName());
            des.append(BstIdl::Transformer::BareMetal::deserialize(a));
        }
        arg_list.emplace_back("reg->ext");
        replace_all(des, "\n", "\n\t");

        replace_all(broadcast_func, "$DESERIALIZE", des);
        replace_all(broadcast_func, "$ARGS", join(arg_list, ", "));
        replace_all(broadcast_func, "$NAME", name);
        replace_all(broadcast_func, "$UPPER_NAME", upperName);
        replace_all(broadcast_func, "$FULLNAME", fullname);

        broadcastDefs.append(broadcast_func);

        std::string broadcast_case = R"(case CMD_BROADCAST_$UPPER_NAME:
    ret = call_$NAME_callback(&broadcast_des);
    break;
)";
        replace_all(broadcast_case, "\n", "\n\t\t");
        replace_all(broadcast_case, "$NAME", name);
        replace_all(broadcast_case, "$UPPER_NAME", upperName);
        broadcastCases.append(broadcast_case);

        std::string reply_case = R"(case CMD_METHOD_SUB_$UPPER_NAME: 
{
    int32_t err = 0;
    ret = ipc_des_get(&reply_des, (uint8_t *)&err, sizeof(err));
    if (ret >= 0)
    {
        callback_registration_t *reg = &s_method_registry[reply_des.header.tok];
        $FULLNAME_sub_callback_t cb = ($FULLNAME_sub_callback_t)(reg->cb);
        if (cb)
            cb(err, reg->ext);
        clear_registry(reg);
    }
    break;
}
case CMD_METHOD_UNSUB_$UPPER_NAME: 
{
    int32_t err = 0;
    ret = ipc_des_get(&reply_des, (uint8_t *)&err, sizeof(err));
    if (ret >= 0)
    {
        callback_registration_t *reg = &s_method_registry[reply_des.header.tok];
        $FULLNAME_unsub_callback_t cb = ($FULLNAME_unsub_callback_t)(reg->cb);
        if (cb)
            cb(err, reg->ext);
        clear_registry(reg);
    }
    break;
})";
        replace_all(reply_case, "$NAME", name);
        replace_all(reply_case, "$FULLNAME", fullname);
        replace_all(reply_case, "$UPPER_NAME", upperName);
        replace_all(reply_case, "\n", "\n\t\t");
        broadcastReplyCases.append(reply_case);

        std::string reg_init = R"(s_client.$NAME_sub = subscribe_$NAME;
s_client.$NAME_unsub = unsubscribe_$NAME;)";
        replace_all(reg_init, "$NAME", name);
        replace_all(reg_init, "\n", "\n\t");
        broadcastInits.append(reg_init);

        std::string reg_reset = R"(s_client.$NAME_sub = NULL;
s_client.$NAME_unsub = NULL;)";
        replace_all(reg_reset, "$NAME", name);
        replace_all(reg_reset, "\n", "\n\t");
        broadcastDtors.append(reg_reset);
    }
    replace_all(content, "$BROADCAST_ID_DEF", broadcastIDs);
    replace_all(content, "$BROADCAST_VARS", broadcastVars);
    replace_all(content, "$BROADCAST_DEFS", broadcastDefs);
    replace_all(content, "$BROADCAST_CASES", broadcastCases);
    replace_all(content, "$BROADCAST_REPLY_CASES", broadcastReplyCases);
    replace_all(content, "$BROADCAST_REG_INIT", broadcastInits);
    replace_all(content, "$BROADCAST_DESTROY", broadcastDtors);

    // version
    auto ver = m_interface->getTarget()->getVersion();
    auto version =
        "{.major = " + std::to_string(ver->getMajor()) + ", .minor = " + std::to_string(ver->getMinor()) + "}";
    replace_all(content, "$VERSION", version);

    // write file
    auto filename = m_folderPath + "/" + clientName + ".c";
    std::ofstream ofs(filename, std::ofstream::out);
    ofs << content;
    ofs.close();

    return true;
}

std::string BareMetalProxyGenerator::getMethodAsyncArgs(const std::shared_ptr<FMethod> &method)
{
    if (!method)
        return "";

    std::list<std::string> arg_list;
    for (const auto &a : method->getInArgs())
    {
        arg_list.emplace_back("const " + Transformer::BareMetal::getTypeName(a->getType()) + " " + a->getName());
    }
    arg_list.emplace_back(m_infName + "_" + method->getName() + "_callback_t cb");
    arg_list.emplace_back("void *ext");
    return join(arg_list, ", ");
}

std::string BareMetalProxyGenerator::getMethodAsyncSignature(const std::shared_ptr<FMethod> &method)
{
    if (!method)
        return "";

    std::string ret = "int32_t (*$NAME)($ARGS);\n";
    replace_all(ret, "$ARGS", getMethodAsyncArgs(method));
    replace_all(ret, "$NAME", method->getName());
    return ret;
}
std::string BareMetalProxyGenerator::getBroadcastSignature(const std::shared_ptr<FBroadcast> &broadcast)
{
    if (!broadcast)
        return "";

    std::string ret = "int32_t (*$NAME)($ARGS);\n";
    replace_all(ret, "$NAME", broadcast->getName());
    std::list<std::string> arg_list;
    for (const auto &a : broadcast->getOutArgs())
    {
        arg_list.emplace_back(Transformer::BareMetal::getTypeName(a->getType()) + " " + a->getName());
    }
    replace_all(ret, "$ARGS", join(arg_list, ", "));
    return ret;
}

bool BareMetalProxyGenerator::getIntegerProperty(const std::shared_ptr<FDElement> &element, const std::string &property,
                                                 int32_t *value)
{
    if (!element || !value)
    {
        std::cerr << "Invalid element." << std::endl;
        return false;
    }
    auto val = element->getSingleValue(property);
    if (!val)
    {
        std::cerr << "No " << property << " defined." << std::endl;
        return false;
    }
    if (!val->isInteger())
    {
        std::cerr << "Invalid " << property << std::endl;
        return false;
    }
    *value = val->getInteger();
    return true;
}

} // namespace BstIdl