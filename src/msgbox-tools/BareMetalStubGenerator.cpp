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
#include "msgbox-tools/transformer/baremetal/deserialize.h"
#include "msgbox-tools/transformer/baremetal/serialize.h"
#include "msgbox-tools/transformer/baremetal/typedecl.h"
#include "msgbox-tools/transformer/baremetal/typename.h"
#include "utilities/string_utility.h"
#include <fstream>

namespace BstIdl
{
bool BareMetalStubGenerator::generate()
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

bool BareMetalStubGenerator::validate()
{
    if (!m_instance || m_instance->getTag() != "instance" || !m_instance->getTarget())
        return false;

    m_stubName.clear();
    m_fid.clear();
    m_sid = 255;
    m_interface = nullptr;

    // get and validate FID
    auto value = m_instance->getSingleValue("FID");
    if (!value)
    {
        std::cerr << "No FID defined." << std::endl;
        return false;
    }
    auto type = value->getEnumerator();
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
    m_stubName = infName.substr(infName.find_last_of('.') + 1);

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

bool BareMetalStubGenerator::generateHeader()
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
    // method type
    $METHOD_TYPE
    // broadcast type
    $BROADCAST_TYPE
    // Interface server
    typedef struct
    {
        // get version
        ipc_inf_version (*version)();

        // register methods
        $METHOD_REGS
        //register broadcasts
        $BROADCASTS
        // message router, call in main loop.
        // receive messages
        int32_t (*receive_message)();
        // dispatch message
        int32_t (*dispatch_message)();
    }$SERVER_NAME;

    // init server
    $SERVER_NAME *$SERVER_NAME_init();
    // destroy client
    int32_t $SERVER_NAME_destroy();

#ifdef __cplusplus
}
#endif

#endif // $HEADER_MACRO
)";
    std::string serverName = m_stubName + "_server";
    std::string headerMacro = toUpper(serverName) + "_H";
    std::string typesDecl;
    std::string constantDecl;
    std::string methodType;
    std::string methodRegs;
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
        methodType.append(getMethodSignature(m));
        methodRegs.append("int32_t (*register_" + m->getName() + ")(" + Transformer::BareMetal::getTypeName(m) +
                          " func);\n");
        if (auto ptr = m->getErrors())
            typesDecl.append(Transformer::BareMetal::getTypeDecl(ptr));
    }
    replace_all(methodType, "\n", "\n\t");
    replace_all(methodRegs, "\n", "\n\t\t");
    replace_all(typesDecl, "\n", "\n\t");

    std::string sub_type = m_stubName + "_broadcast_sub_t";
    broadcastType = "typedef void (*" + sub_type + ")(uint8_t pid, uint8_t fid, uint8_t sid);";
    for (const auto &b : inf->getBroadcasts())
    {
        broadcasts.append(getBroadcastSignature(b));
        broadcasts.append("int32_t (*register_" + b->getName() + "_subcribed)(" + sub_type + " func);\n");
        broadcasts.append("int32_t (*register_" + b->getName() + "_unsubcribed)(" + sub_type + " func);\n");
    }
    replace_all(broadcasts, "\n", "\n\t\t");

    replace_all(content, "$LICENSE", getLicense());
    replace_all(content, "$SERVER_NAME", serverName);
    replace_all(content, "$HEADER_MACRO", headerMacro);
    replace_all(content, "$TYPES_DECL", typesDecl);
    replace_all(content, "$CONSTANT_DECL", constantDecl);
    replace_all(content, "$METHOD_TYPE", methodType);
    replace_all(content, "$BROADCAST_TYPE", broadcastType);
    replace_all(content, "$METHOD_REGS", methodRegs);
    replace_all(content, "$BROADCASTS", broadcasts);

    // write file
    auto filename = m_folderPath + "/" + serverName + ".h";
    std::ofstream ofs(filename, std::ofstream::out);
    ofs << content;
    ofs.close();

    return true;
}

bool BareMetalStubGenerator::generateSource()
{
    std::string content = R"($LICENSE

#include "$SERVER_NAME.h"
#include "ipc_app_common.h"
#include "ipc_app_serdes.h"
#include "ipc_app_svr_utils.h"
#include "ipc_trans_common.h"
#include "ipc_trans_layer.h"

// macro definitions
#define PID $PID_VALUE
#define FID $FID_VALUE
#define SID $SID_VALUE
#define MAX_METHOD_NUM $MAX_METHOD_NUM
#define MAX_BROADCAST_NUM $MAX_BROADCAST_NUM

$METHOD_ID_DEF
$BROADCAST_ID_DEF
// local variables
static ipc_inf_version s_version = $VERSION;
static uint32_t s_handle = 0U;
static $SERVER_NAME s_server = {0};
static uint8_t s_token = 0;
static int8_t s_recv_buffer[IPC_MAX_DATA_SIZE] = {0};

$METHOD_VARS
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
$METHOD_DEF

// broadcast
$BROADCAST_DEF

// receive messages
static int32_t receive_message()
{
    return ipc_trans_layer_get_msg(s_handle);
}

// dispatch messages
static int32_t dispatch_message()
{
    int32_t ret = 0;
    static serdes_t deserializer = {0};
    static serdes_t serializer = {0};

    while (ipc_trans_layer_stub_get_method_msg(s_handle, ipc_des_get_current_msg(&deserializer)) >= 0)
    {
        ret = ipc_des_validate_msg(&deserializer);
        if (ret < 0)
            continue;
        // init serializer.
        // it cannot fail, as &serializer won't be NULL.
        (void)ipc_ser_init(&serializer);
        // process message.
        switch (deserializer.header.cmd)
        {
        $METHOD_CASE
        $BROADCAST_CASE
        default:
            ret = -1;
            ret = ipc_ser_put(&serializer, (uint8_t *)&ret, sizeof(ret));
            break;
        }
        serializer.header = deserializer.header;
        serializer.header.cid = deserializer.header.pid;
        serializer.header.pid = PID;
        serializer.header.typ = IPC_MSG_TYPE_REPLY;
        if (ret >= 0)
            ret = ipc_ser_finish(&serializer);

        if (ret >= 0)
        {
            for (uint32_t i = 0; i <= serializer.index; ++i)
            {
                ret = ipc_trans_layer_stub_send_reply_msg(s_handle, serializer.msg_pool[i]);
                if (ret < 0)
                {
                    printf("send reply fail %d.\n", ret);
                    break;
                }
            }
        }

        deserializer.rcv_index = 0;
    }
    return ret;
}

// init server
$SERVER_NAME *$SERVER_NAME_init()
{
    // start trans layer.
    int32_t ret = ipc_trans_layer_start(0);
    if (ret < 0)
        return NULL;

    // create server handle.
    ret = ipc_trans_layer_stub_create_handle(FID, SID, &s_handle);
    if (ret < 0)
	{
		(void)ipc_trans_layer_stop();
		return NULL;
	}
    $METHOD_PTR_RESET
    // register CMDs.
    $CMD_REG
    
    s_server.version = get_ipc_inf_version;
    $METHOD_REG_INIT
    $BROADCAST_REG_INIT
    s_server.receive_message = receive_message;
    s_server.dispatch_message = dispatch_message;
    return &s_server;
}

// destory client
int32_t $SERVER_NAME_destroy()
{
    int32_t ret = ipc_trans_layer_unregister_method(s_handle);
	if (ret < 0)
		return ret;
    ret = ipc_trans_layer_destory_handle(s_handle);
    if (ret < 0)
        return ret;
    ret = ipc_trans_layer_stop();
    if (ret < 0)
        return ret;

    s_server.version = NULL;
    $METHOD_REG_DESTROY
    $BROADCAST_DESTROY
    s_server.receive_message = NULL;
    s_server.dispatch_message = NULL;
    $METHOD_PTR_RESET
    return ret;
}
)";

    replace_all(content, "$LICENSE", getLicense());

    std::string serverName = m_stubName + "_server";
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
    std::string methodPtrResets;
    std::string methodInits;
    std::string methodDtors;
    std::string cmdRegs;
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
        methodIDs.append("#define CMD_METHOD_")
            .append(toUpper(method->getName()))
            .append(" ")
            .append(std::to_string(id))
            .append("U\n");

        std::string var = "static $METHOD_TYPE s_$NAME_ptr = NULL;\n";
        replace_all(var, "$METHOD_TYPE", Transformer::BareMetal::getTypeName(method));
        replace_all(var, "$NAME", method->getName());
        methodVars.append(var);

        std::string regFunc = R"(
static int32_t register_$NAME($METHOD_TYPE func)
{
    s_$NAME_ptr = func;
    return 0;
}
)";
        replace_all(regFunc, "$METHOD_TYPE", Transformer::BareMetal::getTypeName(method));
        replace_all(regFunc, "$NAME", method->getName());
        methodDefs.append(regFunc);

        std::string serdesFunc = R"(
static int32_t call_$NAME(serdes_t *des, serdes_t *ser)
{
    if (!des || !ser || !s_$NAME_ptr)
        return -1;

    int32_t ret = 0;
    uint32_t length = 0;

    $DESERIALIZE
    $INIT
    (*s_$NAME_ptr)($ARGS);

    $SERIALIZE
    return 0;
}
)";
        std::string des;
        std::string init;
        std::string ser;
        std::list<std::string> arg_list;
        for (const auto &a : method->getInArgs())
        {
            des.append(BstIdl::Transformer::BareMetal::deserialize(a));
            arg_list.emplace_back(a->getName());
        }
        replace_all(des, "\n", "\n\t");
        auto err = method->getErrorType();
        if (err)
        {
            auto errVar = BstIdl::Transformer::BareMetal::initVar(err);
            replace_all(errVar, "$NAME", "err");
            init.append(errVar);
            auto errSer = BstIdl::Transformer::BareMetal::serialize(err);
            replace_all(errSer, "$NAME", "err");
            ser.append(errSer);
        }
        for (const auto &a : method->getOutArgs())
        {
            init.append(BstIdl::Transformer::BareMetal::initVar(a));
            arg_list.emplace_back("&" + a->getName());
            ser.append(BstIdl::Transformer::BareMetal::serialize(a));
        }
        if (err)
            arg_list.emplace_back("&err");
        replace_all(init, "\n", "\n\t");
        replace_all(ser, "\n", "\n\t");

        replace_all(serdesFunc, "$DESERIALIZE", des);
        replace_all(serdesFunc, "$INIT", init);
        replace_all(serdesFunc, "$NAME", method->getName());
        replace_all(serdesFunc, "$ARGS", join(arg_list, ", "));
        replace_all(serdesFunc, "$SERIALIZE", ser);
        methodDefs.append(serdesFunc);

        std::string method_case = R"(case CMD_METHOD_$UPPER_NAME:
    ret = call_$NAME(&deserializer, &serializer);
    if (ret < 0)
    {
        printf("call_$NAME failed.\n");
        (void)ipc_des_init(&deserializer);
        (void)ipc_ser_init(&serializer);
        ret = -1;
        ret = ipc_ser_put(&serializer, (uint8_t *)&ret, sizeof(ret));
    }
    break;
)";
        replace_all(method_case, "$NAME", method->getName());
        replace_all(method_case, "$UPPER_NAME", toUpper(method->getName()));
        replace_all(method_case, "\n", "\n\t\t");
        methodCases.append(method_case);

        std::string ptr_reset = R"(
    // reset $NAME pointer.
    s_$NAME_ptr = NULL;
)";
        replace_all(ptr_reset, "$NAME", method->getName());
        methodPtrResets.append(ptr_reset);

        std::string reg_init = "s_server.register_$NAME = register_$NAME;\n\t";
        replace_all(reg_init, "$NAME", method->getName());
        methodInits.append(reg_init);

        std::string reg_reset = "s_server.register_$NAME = NULL;\n\t";
        replace_all(reg_reset, "$NAME", method->getName());
        methodDtors.append(reg_reset);

        std::string cmd_reg = R"(ret = ipc_trans_layer_register_method(s_handle, CMD_METHOD_$UPPER_NAME);
    if (ret < 0)
	{
        (void)ipc_trans_layer_unregister_method(s_handle);
        (void)ipc_trans_layer_destory_handle(s_handle);
		(void)ipc_trans_layer_stop();
		return NULL;
	}
    )";
        replace_all(cmd_reg, "$UPPER_NAME", toUpper(method->getName()));
        cmdRegs.append(cmd_reg);
    }
    replace_all(content, "$METHOD_ID_DEF", methodIDs);
    replace_all(content, "$METHOD_VARS", methodVars);
    replace_all(content, "$METHOD_DEF", methodDefs);
    replace_all(content, "$METHOD_CASE", methodCases);
    replace_all(content, "$METHOD_PTR_RESET", methodPtrResets);
    replace_all(content, "$METHOD_REG_INIT", methodInits);
    replace_all(content, "$METHOD_REG_DESTROY", methodDtors);

    // $BROADCAST_ID_DEF
    std::string broadcastIDs;
    std::string broadcastVars;
    std::string broadcastDefs;
    std::string broadcastCases;
    std::string broadcastInits;
    std::string broadcastDtors;
    std::string sub_type = m_stubName + "_broadcast_sub_t";
    for (const auto &b : m_interface->getBroadcasts())
    {
        auto name = b->getTarget()->getName();
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

        std::string sub_ptr = "static " + sub_type + " s_" + name + "_sub_ptr = NULL;\n";
        std::string unsub_ptr = "static " + sub_type + " s_" + name + "_unsub_ptr = NULL;\n";
        broadcastVars.append(sub_ptr);
        broadcastVars.append(unsub_ptr);
        broadcastVars.append("static broadcast_registry s_").append(name).append("_registry = {0};\n");

        std::string broadcast_func = R"(
static int32_t register_$NAME_subcribed($SUB_TYPE func)
{
    s_$NAME_sub_ptr = func;
    return 0;
}

static int32_t register_$NAME_unsubcribed($SUB_TYPE func)
{
    s_$NAME_unsub_ptr = func;
    return 0;
}

static int32_t $NAME($ARGS)
{
    int32_t ret = 0;
    int32_t index = 0;
    rw_msg_header header = {0};
    header.pid = PID;
    header.cmd = CMD_BROADCAST_$UPPER_NAME;
    header.typ = IPC_MSG_TYPE_BROADCAST;

    serdes_t serdes = {0};
    (void)ipc_ser_init(&serdes);
    serdes_t *ser = &serdes;
    $SERIALIZE
    broadcast_reg_entry *entry = s_$NAME_registry.entries;
    for (index = s_$NAME_registry.start, entry += s_$NAME_registry.start; index < s_$NAME_registry.end;
         ++index, ++entry)
    {
        if (entry->pid != 0)
        {
            uint32_t i = 0;
            header.cid = entry->pid;
            header.fid = entry->fid;
            header.sid = entry->sid;
            header.tok = s_token;
            ipc_ser_set_header(&serdes, header);
            ipc_ser_finish(&serdes);
            for (; i <= serdes.index; ++i)
            {
                int32_t send_ret = ipc_trans_layer_stub_send_broadcast(s_handle, serdes.msg_pool[i]);
                if (send_ret < 0)
                {
                    printf("send broadcast fail %d.\n", send_ret);
                    break;
                }
            }
            if (i > serdes.index)
                ++ret;
        }
    }
    increase_token();
    return ret;
})";
        replace_all(broadcast_func, "$NAME", name);
        replace_all(broadcast_func, "$UPPER_NAME", toUpper(name));
        replace_all(broadcast_func, "$SUB_TYPE", sub_type);
        std::string ser;
        std::list<std::string> arg_list;
        for (const auto &a : b->getTarget()->getOutArgs())
        {
            ser.append(BstIdl::Transformer::BareMetal::serialize(a));
            arg_list.emplace_back(Transformer::BareMetal::getTypeName(a->getType()) + " " + a->getName());
        }
        replace_all(ser, "\n", "\n\t");
        replace_all(broadcast_func, "$SERIALIZE", ser);
        replace_all(broadcast_func, "$ARGS", join(arg_list, ", "));
        broadcastDefs.append(broadcast_func);

        std::string broadcast_case = R"(case CMD_METHOD_SUB_$UPPER_NAME:
    if (s_$NAME_sub_ptr)
        (*s_$NAME_sub_ptr)((uint8_t)deserializer.header.pid, (uint8_t)deserializer.header.fid, 
                                (uint8_t)deserializer.header.sid);
    ret = add_registration(&s_$NAME_registry, (uint8_t)deserializer.header.pid,
                            (uint8_t)deserializer.header.fid, (uint8_t)deserializer.header.sid);
    ret = ipc_ser_put(&serializer, (uint8_t *)&ret, sizeof(ret));
    break;
case CMD_METHOD_UNSUB_$UPPER_NAME:
    if (s_$NAME_unsub_ptr)
        (*s_$NAME_unsub_ptr)((uint8_t)deserializer.header.pid,
                                (uint8_t)deserializer.header.fid, (uint8_t)deserializer.header.sid);
    ret = remove_registration(&s_$NAME_registry, (uint8_t)deserializer.header.pid,
                                (uint8_t)deserializer.header.fid, (uint8_t)deserializer.header.sid);
    ret = ipc_ser_put(&serializer, (uint8_t *)&ret, sizeof(ret));
    break;
)";
        replace_all(broadcast_case, "\n", "\n\t\t");
        replace_all(broadcast_case, "$NAME", name);
        replace_all(broadcast_case, "$UPPER_NAME", toUpper(name));
        broadcastCases.append(broadcast_case);

        std::string reg_init = R"(s_server.$NAME = $NAME;
    s_server.register_$NAME_subcribed = register_$NAME_subcribed;
    s_server.register_$NAME_unsubcribed = register_$NAME_unsubcribed;
    )";
        replace_all(reg_init, "$NAME", name);
        broadcastInits.append(reg_init);

        std::string reg_reset = R"(s_server.$NAME = NULL;
    s_server.register_$NAME_subcribed = NULL;
    s_server.register_$NAME_unsubcribed = NULL;
    )";
        replace_all(reg_reset, "$NAME", name);
        broadcastDtors.append(reg_reset);

        std::string cmd_reg = R"(ret = ipc_trans_layer_register_method(s_handle, CMD_METHOD_SUB_$UPPER_NAME);
    if (ret < 0)
	{
        (void)ipc_trans_layer_unregister_method(s_handle);
        (void)ipc_trans_layer_destory_handle(s_handle);
		(void)ipc_trans_layer_stop();
		return NULL;
	}

    ret = ipc_trans_layer_register_method(s_handle, CMD_METHOD_UNSUB_$UPPER_NAME);
    if (ret < 0)
	{
        (void)ipc_trans_layer_unregister_method(s_handle);
        (void)ipc_trans_layer_destory_handle(s_handle);
		(void)ipc_trans_layer_stop();
		return NULL;
	}
)";
        replace_all(cmd_reg, "$UPPER_NAME", upperName);
        cmdRegs.append(cmd_reg);
    }
    replace_all(content, "$BROADCAST_ID_DEF", broadcastIDs);
    replace_all(content, "$BROADCAST_VARS", broadcastVars);
    replace_all(content, "$BROADCAST_DEF", broadcastDefs);
    replace_all(content, "$BROADCAST_CASE", broadcastCases);
    replace_all(content, "$BROADCAST_REG_INIT", broadcastInits);
    replace_all(content, "$BROADCAST_DESTROY", broadcastDtors);
    replace_all(content, "$CMD_REG", cmdRegs);

    // version
    auto ver = m_interface->getTarget()->getVersion();
    auto version =
        "{.major = " + std::to_string(ver->getMajor()) + ", .minor = " + std::to_string(ver->getMinor()) + "}";
    replace_all(content, "$VERSION", version);

    // write file
    auto filename = m_folderPath + "/" + serverName + ".c";
    std::ofstream ofs(filename, std::ofstream::out);
    ofs << content;
    ofs.close();

    return true;
}

std::string BareMetalStubGenerator::getMethodSignature(const std::shared_ptr<FMethod> &method)
{
    if (!method)
        return "";

    std::string ret = "typedef void (*$METHOD_TYPE)($ARGS);\n";
    replace_all(ret, "$METHOD_TYPE", Transformer::BareMetal::getTypeName(method));
    std::list<std::string> arg_list;
    for (const auto &a : method->getInArgs())
    {
        arg_list.emplace_back("const " + Transformer::BareMetal::getTypeName(a->getType()) + " " + a->getName());
    }
    for (const auto &a : method->getOutArgs())
    {
        arg_list.emplace_back(Transformer::BareMetal::getTypeName(a->getType()) + "* " + a->getName());
    }
    if (auto ptr = method->getErrorType())
    {
        arg_list.emplace_back(Transformer::BareMetal::getTypeName(ptr) + "* err");
    }
    replace_all(ret, "$ARGS", join(arg_list, ", "));
    return ret;
}
std::string BareMetalStubGenerator::getBroadcastSignature(const std::shared_ptr<FBroadcast> &broadcast)
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

bool BareMetalStubGenerator::getIntegerProperty(const std::shared_ptr<FDElement> &element, const std::string &property,
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