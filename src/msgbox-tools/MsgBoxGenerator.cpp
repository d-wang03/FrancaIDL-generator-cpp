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
#include "msgbox-tools/MsgBoxGenerator.h"
#include "FDModel/FDModelManager.h"
#include "FDModel/FDEnumerator.h"
#include "msgbox-tools/BareMetalStubGenerator.h"
#include "msgbox-tools/BareMetalProxyGenerator.h"

#include <fstream>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

namespace BstIdl
{

bool MsgBoxGenerator::generate()
{
    if (m_destDir.empty())
        return false;

    if(fs::exists(m_destDir))
        fs::remove_all(m_destDir);

    auto path = "/";
    m_folderPath = mk_dir(m_destDir, path);
    if (m_folderPath.empty())
        return false;

    auto &fdmodels = BstIdl::FDModelManager::getInstance();
    if (fdmodels.size() == 0)
    {
        CERR << "No valid models!" << ENDL;
        return false;
    }

    for (const auto &model : fdmodels.getModelList())
    {
        for (const auto &element : model->getDeployments())
        {
            auto spec = element->getSpec();
            if (!spec || spec->getName() != "com.bst.ipc.deployment")
                continue;
            if (auto ptr = std::dynamic_pointer_cast<FDExtensionRoot>(element))
            {
                if (ptr->getTag() != "provider")
                    continue;
                if (!validateProvider(ptr))
                {
                    CERR << "Error provider deployment." << ENDL;
                    return false;
                }
                std::string rtestr;
                if (m_rte == "BareMetal")
                    rtestr = "IPC_RTE_BAREMETAL";
                else if (m_rte == "Posix")
                    rtestr = "IPC_RTE_POSIX";
                else if (m_rte == "LinuxKernel")
                    rtestr = "IPC_RTE_KERNEL";
                else if (m_rte == "RTOS")
                    rtestr = "IPC_RTE_RTOS";
                else
                {
                    CERR << "Unknown RTE : " << m_rte << ENDL;
                    return false;
                }
                bool ret = false;
                if (m_providerType == "Server")
                {
                    std::cout << "Generating " << m_rte << " server code ..." << ENDL;
                    for (const auto &ins : ptr->getElements())
                    {
                        if (!ins || ins->getTag() != "instance")
                            continue;
                        auto gen = std::make_shared<StubGenerator>(m_destDir, m_srcEndID, ins);
                        gen->setRteStr(rtestr);
                        ret = gen->generate();
                        m_infs.emplace_back(gen->getInfName());
                        if (ret)
                            std::cout <<"Generating server code for instance " << gen->getInfName() << " Success!" << ENDL;
                        else
                            CERR <<"Generating server code for instance " << gen->getInfName() << "Failed!" << ENDL;
                    }
                    std::cout << "Generating header code for provider " << ptr->getName() << ENDL;
                    ret = generateStubHeader(ptr);
                    if (ret)
                        std::cout << "Success!" << ENDL;
                    else
                        CERR << "Failed!" << ENDL;
                    std::cout << "Generating source code for provider " << ptr->getName() << ENDL;
                    ret = generateStubSource(ptr);
                    if (ret)
                        std::cout << "Success!" << ENDL;
                    else
                        CERR << "Failed!" << ENDL;
                }
                else if (m_providerType == "Client")
                {
                    std::cout << "Generating " << m_rte << " client code ..." << ENDL;
                    for (const auto &ins : ptr->getElements())
                    {
                        if (!ins || ins->getTag() != "instance")
                            continue;
                        auto gen = std::make_shared<ProxyGenerator>(m_destDir, m_srcEndID, ins);
                        gen->setRteStr(rtestr);
                        ret = gen->generate();
                        m_infs.emplace_back(gen->getInfName());
                        if (ret)
                            std::cout <<"Generating client code for instance " << gen->getInfName() << " Success!" << ENDL;
                        else
                            CERR <<"Generating client code for instance " << gen->getInfName() << "Failed!" << ENDL;
                    }
                    std::cout << "Generating header code for provider " << ptr->getName() << ENDL;
                    ret = generateProxyHeader(ptr);
                    if (ret)
                        std::cout << "Success!" << ENDL;
                    else
                        CERR << "Failed!" << ENDL;
                    std::cout << "Generating source code for provider " << ptr->getName() << ENDL;
                    ret = generateProxySource(ptr);
                    if (ret)
                        std::cout << "Success!" << ENDL;
                    else
                        CERR << "Failed!" << ENDL;
                }

                if (ret)
                    std::cout << "Generation for " << ptr->getName() << " success." << ENDL;
                else
                {
                    CERR << "Generation for " << ptr->getName() << " fail." << ENDL;
                }
            }
        }
    }

    return true;
}

bool MsgBoxGenerator::validateProvider(const std::shared_ptr<FDExtensionRoot> &provider)
{
    if (!provider)
        return false;

    m_providerType.clear();
    m_rte.clear();
    m_srcEndID.clear();

    // get and validate ProviderType
    auto value = provider->getSingleValue("ProviderType");
    if (!value)
    {
        CERR << "No ProviderType defined." << ENDL;
        return false;
    }
    auto type = value->getEnumerator();
    if (!type)
    {
        CERR << "Invalid ProviderType." << ENDL;
        return false;
    }
    m_providerType = type->getName();
    if (m_providerType != "Server" && m_providerType != "Client")
    {
        CERR << "Invalid ProviderType." << ENDL;
        return false;
    }

    // get and validate RTE
    value = provider->getSingleValue("RTE");
    if (!value)
    {
        CERR << "No RTE defined." << ENDL;
        return false;
    }
    type = value->getEnumerator();
    if (!type)
    {
        CERR << "Invalid RTE." << ENDL;
        return false;
    }
    m_rte = type->getName();
    if (m_rte != "BareMetal" && m_rte != "Posix" && m_rte != "LinuxKernel" && m_rte != "RTOS")
    {
        CERR << "Invalid RTE." << ENDL;
        return false;
    }

    // get and validate SrcEndID
    value = provider->getSingleValue("SrcEndID");
    if (!value)
    {
        CERR << "No SrcEndID defined." << ENDL;
        return false;
    }
    type = value->getEnumerator();
    if (!type)
    {
        CERR << "Invalid SrcEndID." << ENDL;
        return false;
    }
    m_srcEndID = type->getName();
    if (m_srcEndID != "CPU_0" && m_srcEndID != "CPU_1" && m_srcEndID != "CPU_2" && m_srcEndID != "CPU_3" &&
        m_srcEndID != "CPU_4" && m_srcEndID != "CPU_5" && m_srcEndID != "CPU_6" && m_srcEndID != "CPU_7" &&
        m_srcEndID != "CPUMP2_0" && m_srcEndID != "CPUMP2_1" && m_srcEndID != "ISPCV_0" && m_srcEndID != "ISPCV_1" &&
        m_srcEndID != "ISPCV_2" && m_srcEndID != "ISPCV_3" && m_srcEndID != "ISPCV_4" && m_srcEndID != "NET_0" &&
        m_srcEndID != "DMA_0" && m_srcEndID != "DMA_1" && m_srcEndID != "SWITCH_0" && m_srcEndID != "SWITCH_1" &&
        m_srcEndID != "SWITCH_2" && m_srcEndID != "SWITCH_3" && m_srcEndID != "SWITCH_4" && m_srcEndID != "SWITCH_5" &&
        m_srcEndID != "SECURE_0" && m_srcEndID != "SECURE_1" && m_srcEndID != "SAFETY_0" && m_srcEndID != "SAFETY_1" &&
        m_srcEndID != "REALTIME_0" && m_srcEndID != "REALTIME_1" && m_srcEndID != "REALTIME_2" &&
        m_srcEndID != "REALTIME_3" && m_srcEndID != "REALTIME_4" && m_srcEndID != "REALTIME_5" &&
        m_srcEndID != "MEDIA_0")
    {
        CERR << "Invalid SrcEndID." << ENDL;
        return false;
    }

    // get and validate FID
    value = provider->getSingleValue("FID");
    if (!value)
    {
        CERR << "No FID defined." << ENDL;
        return false;
    }
    type = value->getEnumerator();
    if (!type)
    {
        CERR << "Invalid FID." << ENDL;
        return false;
    }
    m_fid = type->getName();
    if (m_srcEndID == "CPU_0" || m_srcEndID == "CPU_1" || m_srcEndID == "CPU_2" || m_srcEndID == "CPU_3" ||
        m_srcEndID == "CPU_4" || m_srcEndID == "CPU_5" || m_srcEndID == "CPU_6" || m_srcEndID == "CPU_7")
    {
        if (m_fid != "DEF" && m_fid != "F1" && m_fid != "F2" && m_fid != "F3" && m_fid != "F4" && m_fid != "F5" &&
            m_fid != "F6" && m_fid != "F7")
        {
            CERR << "Invalid FID." << ENDL;
            return false;
        }
    }
    else if (m_srcEndID == "CPUMP2_0" || m_srcEndID == "CPUMP2_1")
    {
        if (m_fid != "DEF" && m_fid != "F1")
        {
            CERR << "Invalid FID." << ENDL;
            return false;
        }
    }
    else
    {
        if (m_fid != "DEF" && m_fid != "F1" && m_fid != "F2" && m_fid != "F3")
        {
            CERR << "Invalid FID." << ENDL;
            return false;
        }
    }

    // get and validate SID.
    value = provider->getSingleValue("SID");
    if (!value)
    {
        CERR << "No SID defined." << ENDL;
        return false;
    }
    if (!value->isInteger())
    {
        CERR << "Invalid SID." << ENDL;
        return false;
    }
    m_sid = value->getInteger();
    if (m_sid < 0 || m_sid > 15)
    {
        CERR << "Invalid SID." << ENDL;
        return false;
    }

    return true;
}

bool MsgBoxGenerator::generateStubHeader(const std::shared_ptr<FDExtensionRoot> &provider)
{
    if (!provider)
        return false;

    std::string content = R"($LICENSE
$VERSION_COMMENT
#ifndef $HEADER_MACRO
#define $HEADER_MACRO

$INCLUDES
#ifdef __cplusplus
extern "C" {
#endif

struct _$PROVIDER_NAME_t {
$INS_SERVERS
#ifdef IPC_RTE_BAREMETAL
	/**
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
	int32_t (*dispatch_message)(void);
#else
	/**
	 * Start the message router.
	 *
	 * @return 0 if success, negative if fail.
	 */
	int32_t (*start)(void);

	/**
	 * Stop the message router.
	 *
	 * @return 0 if success, negative if fail.
	 */
	int32_t (*stop)(void);
#endif
};
#define $PROVIDER_NAME_t struct _$PROVIDER_NAME_t


/**
 *  The internal data used by the server.
 *  Users should define an instance and pass it to the server initialization function.
 */
struct _$PROVIDER_NAME_data_t {
	com_server_data_t com_data;
	$PROVIDER_NAME_t server;
	$INS_SERVER_EXTS
};
#define $PROVIDER_NAME_data_t struct _$PROVIDER_NAME_data_t

/**
 * Initializes the server.
 *
 * @param ins The instance to be initialized.
 * @return A pointer to the initialized server, NULL if fail.
 */
$PROVIDER_NAME_t *$PROVIDER_NAME_init($PROVIDER_NAME_data_t *ins);
/**
 * Destroys the server.
 *
 * @return 0 if success, negetive if fail.
 */
int32_t $PROVIDER_NAME_destroy(void);

#ifdef __cplusplus
}
#endif

#endif // $HEADER_MACRO
)";

    std::string providerName = provider->getName();
    std::string headerMacro = toUpper(providerName) + "_H";
    std::string includes;
    std::string servers;
    std::string serverExts;

    for(auto e : m_infs)
    {
        includes.append("#include \"" + e + "_server.h\"\n");
        servers.append("\t" + e + "_server_t " + e + "_server;\n");
        serverExts.append(e + "_server_ext_t " + e + "_ext;\n\t");
    }
    trim(serverExts);

    // check file
    auto filename = m_folderPath + "/" + providerName + ".h";
    if (std::ifstream(filename))
    {
        //file already exists.
        providerName.append("_provider");
        filename = m_folderPath + "/" + providerName + ".h";
        CERR << "Provider name already used, append with provider" << ENDL;
    }

    replace_all(content, "$LICENSE", getLicense());
    replace_all(content, "$VERSION_COMMENT", getVersionComment());
    replace_all(content, "$HEADER_MACRO", headerMacro);
    replace_all(content, "$INCLUDES", includes);
    replace_all(content, "$INS_SERVERS", servers);
    replace_all(content, "$INS_SERVER_EXTS", serverExts);
    replace_all(content, "$PROVIDER_NAME", providerName);
    replace_all(content, "    ", "\t");
    replace_all(content, "\r\n", "\n");
    replace_all(content, "\t\n", "\n");

    //write file
    std::ofstream ofs(filename, std::ofstream::out | std::ios::binary);
    ofs << content;
    ofs.close();

    return true;
}
bool MsgBoxGenerator::generateStubSource(const std::shared_ptr<FDExtensionRoot> &provider)
{
    if (!provider)
        return false;

    std::string content = R"($LICENSE
$VERSION_COMMENT
#include "$PROVIDER_NAME.h"
#ifdef IPC_RTE_KERNEL
#include <bst/ipc_trans_common.h>
#include <bst/ipc_trans_layer.h>
#else
#include "ipc_trans_common.h"
#include "ipc_trans_layer.h"
#endif

#define PID $PID_VALUE
#define FID $FID_VALUE
#define SID $SID_VALUE

static $PROVIDER_NAME_data_t *s_ins = NULL;

// receive messages
static int32_t receive_message(void)
{
	com_server_data_t *data = (com_server_data_t *)s_ins;

	if (!data)
		return -ERR_APP_PARAM;

	return ipc_trans_layer_query_info(data->pid, data->handle);
}

// dispatch messages
static int32_t dispatch_message(void)
{
	int32_t ret = 0;
	serdes_t *ser = NULL;
	serdes_t *des = NULL;
    com_server_data_t *data = (com_server_data_t *)s_ins;

	if (!data)
		return -ERR_APP_PARAM;
	ser = &data->serializer;
	des = &data->deserializer;

	while (ipc_trans_layer_stub_get_method_msg(data->pid, data->handle, des) >= 0) {
		bool need_reply = true;
$INS_SVR_CASES
		if (need_reply) {
            (void)ipc_ser_init(ser);
            ret = ipc_ser_put_32(ser, (uint32_t *)&ret);
			ser->header = des->header;
			ser->header.cid = des->header.pid;
			ser->header.pid = data->pid;
			ser->header.typ = MSGBX_MSG_TYPE_REPLY;
			if (ret >= 0)
				ret = ipc_ser_finish(ser);
			if (ret >= 0)
				ret = send_reply(data, ser);
			if (ret < 0)
				IPC_LOG_ERR("send reply fail %" PRId32 ".\n", ret);
		}
	}
	return ret;
}
#ifndef IPC_RTE_BAREMETAL
#if defined IPC_RTE_KERNEL
static int router_func(void *arg)
#else
static void router_func(void *arg)
#endif
{
	int32_t ret = 0;

#if defined IPC_RTE_POSIX || defined IPC_RTE_RTOS
	while (s_ins && s_ins->com_data.bRunning) {
#elif defined IPC_RTE_KERNEL
	while (unlikely(!kthread_should_stop())) {
#endif
		ret = receive_message();
		if (ret != 0)
			continue;

		ret = dispatch_message();
		if (ret < 0)
			continue;
	}
#if defined IPC_RTE_KERNEL
	return RESULT_SUCCESS;
#else
	return;
#endif
}

// start message router
static int32_t start(void)
{
#if defined IPC_RTE_POSIX || defined IPC_RTE_RTOS
	int32_t ret = 0;
#endif
	com_server_data_t *data = &s_ins->com_data;

	if (!data)
		return ERR_APP_PARAM;

	if (data->bRunning)
		return RESULT_SUCCESS;

	data->bRunning = true;
#if defined IPC_RTE_POSIX
	ret = pthread_create(&data->route_task, NULL, router_func, NULL);
	if (ret != 0) {
#elif defined IPC_RTE_RTOS
	TaskCreate(router_func, "$PROVIDER_NAME_thread", 0x1000, NULL, 6 ,NULL,NULL);
	if (ret != 0) {
#elif defined IPC_RTE_KERNEL
	data->route_task = kthread_run(router_func, NULL, "$PROVIDER_NAME_thread");
	if (unlikely(!data->route_task)) {
#endif
		data->bRunning = false;
		return -ERR_APP_START;
	}

	return RESULT_SUCCESS;
}

// stop message router.
static int32_t stop(void)
{
	int32_t ret = 0;
	com_server_data_t *data = &s_ins->com_data;

	if (!data)
		return ERR_APP_PARAM;

	if (!data->bRunning)
		return RESULT_SUCCESS;

#if defined IPC_RTE_POSIX
	sleep(1);
	data->bRunning = false;
	ipc_trans_layer_release_recv_wait(data->pid, data->handle);
	ret = pthread_join(data->route_task, NULL);
	if (ret != 0)
		return -ERR_APP_STOP;
#elif defined IPC_RTE_RTOS
	Msleep(1000);
	data->bRunning = false;
	ipc_trans_layer_release_recv_wait(data->pid, data->handle);
    TaskDelete(router_func);
#elif defined IPC_RTE_KERNEL
	msleep(1000);
	if (likely(data->route_task)) {
		ipc_trans_layer_release_recv_wait(data->pid, data->handle);
		ret = kthread_stop(data->route_task);
		if (unlikely(ret))
			return -ERR_APP_STOP;
	}
	data->bRunning = false;
#endif

	return RESULT_SUCCESS;
}
#endif
// $PROVIDER_NAME_init
$PROVIDER_NAME_t *$PROVIDER_NAME_init($PROVIDER_NAME_data_t *ins)
{
	int32_t ret = 0;
	com_server_data_t *data = (com_server_data_t *)ins;

	if (!data)
		return NULL;
	if (s_ins && s_ins->com_data.initialized) {
		IPC_LOG_ERR("already initialized.\n");
		return &s_ins->server;
	}

	s_ins = ins;

	data->pid = data->pid == 0 ? PID : data->pid;
	data->fid = data->fid == 0 ? FID : data->fid;
	data->sid = data->sid == 0 ? SID : data->sid;

	// create server handle.
	ret = ipc_trans_layer_stub_create_handle(data->pid, data->fid, data->sid,
                data->pid, &data->handle);
	if (ret < 0)
	{
		IPC_LOG_ERR("create handle fail %" PRId32 ".\n", ret);
		return NULL;
	}

	// init servers
$INS_SVR_INIT
#ifdef IPC_RTE_BAREMETAL
	ins->server.receive_message = receive_message;
	ins->server.dispatch_message = dispatch_message;
#else
	ins->server.start = start;
	ins->server.stop = stop;
#endif
    IPC_MUTEX_INIT(&data->send_mtx);
	data->initialized = true;
	return &ins->server;
}

// $PROVIDER_NAME_destroy
int32_t $PROVIDER_NAME_destroy(void)
{
	int32_t ret = 0;
	com_server_data_t *data = (com_server_data_t *)s_ins;

	// if is NULL, just return SUCCESS.
	if (!data)
		return RESULT_SUCCESS;

	ret = ipc_trans_layer_unregister_method(data->pid, data->handle);
	if (ret < 0)
		return ret;
	ret = ipc_trans_layer_destroy_handle(data->pid, data->handle);
	if (ret < 0)
		return ret;

$INS_DESTROY
    IPC_MUTEX_DESTROY(&data->send_mtx);
	ipc_memset(s_ins, 0, sizeof($PROVIDER_NAME_data_t));
	s_ins = NULL;
	return ret;
}
)";

    std::string providerName = provider->getName();
    std::string headerMacro = toUpper(providerName) + "_H";
    std::string includes;
    std::list<std::string> serverCasesList;
    std::string serverCases;
    std::string serverInits;
    std::string insDestroy;

    std::string caseTpl = R"(        ret = s_ins->server.$NAME_server.dispatch_request(des, &need_reply);
)";
    std::string casesDispatchLogTpl = R"(        if (ret < 0)
            IPC_LOG_ERR("Server dispatch request failed %" PRId32 ".\n", ret);
)";
    std::string initTpl = R"(    ret = $NAME_server_init(data, &ins->server.$NAME_server, &ins->$NAME_ext);
	if (ret < 0) {
		(void)ipc_trans_layer_unregister_method(data->pid, data->handle);
		(void)ipc_trans_layer_destroy_handle(data->pid, data->handle);
		return NULL;
	}
)";
    std::string dtorTpl = R"(	$NAME_server_destroy();
)";
    for(auto e : m_infs)
    {
        std::string svrCase = caseTpl;
        replace_all(svrCase, "$NAME", e);
        serverCasesList.emplace_back(svrCase);

        std::string svrInit = initTpl;
        replace_all(svrInit, "$NAME", e);
        serverInits.append(svrInit);

        std::string dtor = dtorTpl;
        replace_all(dtor, "$NAME", e);
        insDestroy.append(dtor);
    }
    if (!serverCasesList.empty())
    {
        serverCases = join(serverCasesList, "        if (ret < 0)\n\t");
        serverCases.append(casesDispatchLogTpl);
    }

    // check file
    auto filename = m_folderPath + "/" + providerName + ".c";
    if (std::ifstream(filename))
    {
        //file already exists.
        providerName.append("_provider");
        filename = m_folderPath + "/" + providerName + ".c";
        CERR << "Provider name already used, append with provider" << ENDL;
    }

    replace_all(content, "$LICENSE", getLicense());
    // replace license comment style to pass checkpatch.pl
    replace_one(content, "/* SPDX-License-Identifier: GPL-2.0 OR Apache 2.0\n *", "// SPDX-License-Identifier: GPL-2.0 OR Apache 2.0\n/*");
    replace_all(content, "$VERSION_COMMENT", getVersionComment());
    if (m_rte == "LinuxKernel")
        replace_one(content, "static $PROVIDER_NAME_data_t *s_ins = NULL;", "static $PROVIDER_NAME_data_t *s_ins;");

    replace_all(content, "$PID_VALUE", m_srcEndID);
    replace_all(content, "$FID_VALUE", m_fid);
    replace_all(content, "$SID_VALUE", std::to_string(m_sid).append("U"));
    replace_all(content, "$INS_SVR_CASES", serverCases);
    replace_all(content, "$INS_SVR_INIT", serverInits);
    replace_all(content, "$INS_DESTROY", insDestroy);
    replace_all(content, "$PROVIDER_NAME", providerName);
    replace_all(content, "    ", "\t");
    replace_all(content, "\r\n", "\n");
    replace_all(content, "\t\n", "\n");
    // write file
    std::ofstream ofs(filename, std::ofstream::out | std::ios::binary);
    ofs << content;
    ofs.close();

    return true;
}

bool MsgBoxGenerator::generateProxyHeader(const std::shared_ptr<FDExtensionRoot> &provider)
{
    if (!provider)
        return false;

    std::string content = R"($LICENSE
$VERSION_COMMENT
#ifndef $HEADER_MACRO
#define $HEADER_MACRO

$INCLUDES
#ifdef __cplusplus
extern "C" {
#endif

struct _$PROVIDER_NAME_t {
$INS_CLIENTS
#ifdef IPC_RTE_BAREMETAL
	/**
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
	int32_t (*dispatch_message)(void);
#else
	/**
	 * Start the message router.
	 *
	 * @return 0 if success, negative if fail.
	 */
	int32_t (*start)(void);

	/**
	 * Stop the message router.
	 *
	 * @return 0 if success, negative if fail.
	 */
	int32_t (*stop)(void);
#endif
};
#define $PROVIDER_NAME_t struct _$PROVIDER_NAME_t

/**
 *  The internal data used by the client.
 *  Users should define an instance and pass it to the initialization function.
 */
struct _$PROVIDER_NAME_data_t {
	com_client_data_t com_data;
	$PROVIDER_NAME_t client;
	$INS_CLIENT_EXTS
};
#define $PROVIDER_NAME_data_t struct _$PROVIDER_NAME_data_t

/**
 * Initializes the client.
 *
 * @param data The data to be used by the client.
 * @return A pointer to the initialized client, NULL if fail.
 */
$PROVIDER_NAME_t *$PROVIDER_NAME_init($PROVIDER_NAME_data_t *ins);

/**
 * Destroys the client.
 *
 * @return 0 if success, negetive if fail.
 */
int32_t $PROVIDER_NAME_destroy(void);

#ifdef __cplusplus
}
#endif

#endif // $HEADER_MACRO
)";

    std::string providerName = provider->getName();
    std::string headerMacro = toUpper(providerName) + "_H";
    std::string includes;
    std::string servers;
    std::string serverExts;

    for(auto e : m_infs)
    {
        includes.append("#include \"" + e + "_client.h\"\n");
        servers.append("\t" + e + "_client_t " + e + "_client;\n");
        serverExts.append(e + "_client_ext_t " + e + "_ext;\n\t");
    }
    trim(serverExts);

    // check file
    auto filename = m_folderPath + "/" + providerName + ".h";
    if (std::ifstream(filename))
    {
        //file already exists.
        providerName.append("_provider");
        filename = m_folderPath + "/" + providerName + ".h";
        CERR << "Provider name already used, append with provider" << ENDL;
    }

    replace_all(content, "$LICENSE", getLicense());
    replace_all(content, "$VERSION_COMMENT", getVersionComment());
    replace_all(content, "$HEADER_MACRO", headerMacro);
    replace_all(content, "$INCLUDES", includes);
    replace_all(content, "$INS_CLIENTS", servers);
    replace_all(content, "$INS_CLIENT_EXTS", serverExts);
    replace_all(content, "$PROVIDER_NAME", providerName);
    replace_all(content, "    ", "\t");
    replace_all(content, "\r\n", "\n");
    replace_all(content, "\t\n", "\n");
    // write file
    std::ofstream ofs(filename, std::ofstream::out | std::ios::binary);
    ofs << content;
    ofs.close();

    return true;
}
bool MsgBoxGenerator::generateProxySource(const std::shared_ptr<FDExtensionRoot> &provider)
{
    if (!provider)
        return false;

    std::string content = R"($LICENSE
$VERSION_COMMENT
#include "$PROVIDER_NAME.h"

#define PID $PID_VALUE
#define FID $FID_VALUE
#define SID $SID_VALUE

static $PROVIDER_NAME_data_t *s_ins = NULL;

// receive messages
static int32_t receive_message(void)
{
	int32_t ret = 0;
	com_client_data_t *data = (com_client_data_t *)s_ins;

	if (!data)
		return -ERR_APP_PARAM;

	ret = ipc_trans_layer_query_info(data->pid, data->handle);

	// check if availability changed
	if (data->avail_changed_cb) {
		if (ret == QUERY_INFO_DST_STS_OFFLINE)
			data->avail_changed_cb(false, data->avail_ext);
		else if (ret == QUERY_INFO_DST_STS_ONLINE)
			data->avail_changed_cb(true, data->avail_ext);
	}

	return ret;
}

// dispatch messages
static int32_t dispatch_message(void)
{
	int32_t ret = 0;
	serdes_t *des = NULL;
	bool has_message = false;
	com_client_data_t *data = (com_client_data_t *)s_ins;

	if (!data)
		return -ERR_APP_PARAM;
	des = &data->deserializer;

	while (true) {
		has_message = false;
		if (ipc_trans_layer_proxy_get_broadcast_msg(data->pid, data->handle, des) >= 0) {
			has_message = true;
            $INS_CASES1
			if (ret < 0)
				IPC_LOG_ERR("Unexpected broadcast message from ID %u.\n", des->header.pid);
		}
		if (ipc_trans_layer_proxy_get_reply_msg(data->pid, data->handle, des) >= 0) {
			has_message = true;
            $INS_CASES2
			if (ret < 0)
			    IPC_LOG_ERR("Unexpected reply message from ID %u.\n", des->header.pid);
		}
		if (!has_message)
			break;
	}
	return ret;
}
#ifndef IPC_RTE_BAREMETAL
#if defined IPC_RTE_KERNEL
static int router_func(void *arg)
#else
static void router_func(void *arg)
#endif
{
	int32_t ret = 0;

#if defined IPC_RTE_POSIX || defined IPC_RTE_RTOS
	while (s_ins && s_ins->com_data.bRunning) {
#elif defined IPC_RTE_KERNEL
	while (unlikely(!kthread_should_stop())) {
#endif
		ret = receive_message();
		if (ret != 0)
			continue;

		ret = dispatch_message();
		if (ret < 0)
			continue;
	}
#if defined IPC_RTE_KERNEL
	return RESULT_SUCCESS;
#else
	return;
#endif
}

// start message router
static int32_t start(void)
{
#if defined IPC_RTE_POSIX || defined IPC_RTE_RTOS
	int32_t ret = 0;
#endif
	com_client_data_t *data = &s_ins->com_data;

	if (!data)
		return ERR_APP_PARAM;

	if (data->bRunning)
		return RESULT_SUCCESS;

	data->bRunning = true;
#if defined IPC_RTE_POSIX
	ret = pthread_create(&data->route_task, NULL, router_func, NULL);
	if (ret != 0) {
#if defined IPC_RTE_RTOS
	TaskCreate(router_func, "$PROVIDER_NAME_thread", 0x1000, NULL, 6 ,NULL,NULL);
	if (ret != 0) {
#elif defined IPC_RTE_KERNEL
	data->route_task = kthread_run(router_func, NULL, "$PROVIDER_NAME_thread");
	if (unlikely(!data->route_task)) {
#endif
		data->bRunning = false;
		return -ERR_APP_START;
	}

	return RESULT_SUCCESS;
}

// stop message router.
static int32_t stop(void)
{
	int32_t ret = 0;
	com_client_data_t *data = &s_ins->com_data;

	if (!data)
		return ERR_APP_PARAM;

	if (!data->bRunning)
		return RESULT_SUCCESS;

#if defined IPC_RTE_POSIX
	sleep(1);
	data->bRunning = false;
	ipc_trans_layer_release_recv_wait(data->pid, data->handle);
	ret = pthread_join(data->route_task, NULL);
	if (ret != 0)
		return -ERR_APP_STOP;
#elif defined IPC_RTE_RTOS
	Msleep(1000);
	data->bRunning = false;
	ipc_trans_layer_release_recv_wait(data->pid, data->handle);
    TaskDelete(router_func);
#elif defined IPC_RTE_KERNEL
	msleep(1000);
	if (likely(data->route_task)) {
		ipc_trans_layer_release_recv_wait(data->pid, data->handle);
		ret = kthread_stop(data->route_task);
		if (unlikely(ret))
			return -ERR_APP_STOP;
	}
	data->bRunning = false;
#endif

	return RESULT_SUCCESS;
}
#endif
// $PROVIDER_NAME_init
$PROVIDER_NAME_t *$PROVIDER_NAME_init($PROVIDER_NAME_data_t *ins)
{
	int32_t ret = 0;
	com_client_data_t *data = (com_client_data_t *)ins;

	if (!data)
		return NULL;
	if (s_ins && s_ins->com_data.initialized) {
		IPC_LOG_ERR("already initialized.\n");
		return &s_ins->client;
	}

	s_ins = ins;

	data->pid = data->pid == 0 ? PID : data->pid;
	data->fid = data->fid == 0 ? FID : data->fid;
	data->sid = data->sid == 0 ? SID : data->sid;

	// init clients
$INS_INIT
	// create client handle.
	ret = ipc_trans_layer_proxy_create_handle(data->pid, data->fid, data->sid, $DST, &data->handle);
	if (ret < 0)
	{
		IPC_LOG_ERR("create handle fail %" PRId32 ".\n", ret);
		return NULL;
	}

#ifdef IPC_RTE_BAREMETAL
	ins->client.receive_message = receive_message;
	ins->client.dispatch_message = dispatch_message;
#else
	ins->client.start = start;
	ins->client.stop = stop;
#endif
    IPC_MUTEX_INIT(&data->send_mtx);
	init_registry_list(data->method_registry, IPC_TOKEN_NUM);
	data->initialized = true;
	return &ins->client;
}

// $PROVIDER_NAME_destroy
int32_t $PROVIDER_NAME_destroy(void)
{
	int32_t ret = 0;
	com_client_data_t *data = (com_client_data_t *)s_ins;

	// if is NULL, just return SUCCESS.
	if (!data)
		return RESULT_SUCCESS;

	ret = ipc_trans_layer_destroy_handle(data->pid, data->handle);
	if (ret < 0)
		return ret;

$INS_DESTROY
    IPC_MUTEX_DESTROY(&data->send_mtx);
	destroy_registry_list(data->method_registry, IPC_TOKEN_NUM);
	ipc_memset(s_ins, 0, sizeof($PROVIDER_NAME_data_t));
	s_ins = NULL;
	return ret;
}
)";

    std::string providerName = provider->getName();
    std::string headerMacro = toUpper(providerName) + "_H";
    std::string includes;
    std::string insCases;
    std::string insInits;
    std::string insDestroy;

    std::string caseTpl = R"(			if (des->header.pid == s_ins->$NAME_ext.cid)
				ret = s_ins->client.$NAME_client.dispatch_broadcast(des);
)";
    std::string initTpl = R"(	ret = $NAME_client_init(data, &ins->client.$NAME_client, &ins->$NAME_ext);
	if (ret < 0)
		return NULL;
)";
    std::string dtorTpl = R"(	$NAME_client_destroy();
)";
    for(auto e : m_infs)
    {
        std::string svrCase = caseTpl;
        replace_all(svrCase, "$NAME", e);
        insCases.append(svrCase);

        std::string svrInit = initTpl;
        replace_all(svrInit, "$NAME", e);
        insInits.append(svrInit);

        std::string dtor = dtorTpl;
        replace_all(dtor, "$NAME", e);
        insDestroy.append(dtor);
    }
    trim(insCases);

    if (m_infs.size() == 1)
    {
        std::string dst = "ins->" + m_infs.front() + "_ext.cid";
        replace_all(content, "$DST", dst);
    }
    else 
    {
        replace_all(content, "$DST", "0");
        replace_all(insCases, "_client.dispatch_broadcast(des);\n			if (des->header.pid ",
                    "_client.dispatch_broadcast(des);\n			if (ret < 0 && des->header.pid ");
    }
    // check file
    auto filename = m_folderPath + "/" + providerName + ".c";
    if (std::ifstream(filename))
    {
        //file already exists.
        providerName.append("_provider");
        filename = m_folderPath + "/" + providerName + ".c";
        CERR << "Provider name already used, append with provider" << ENDL;
    }

    replace_all(content, "$LICENSE", getLicense());
    // replace license comment style to pass checkpatch.pl
    replace_one(content, "/* SPDX-License-Identifier: GPL-2.0 OR Apache 2.0\n *", "// SPDX-License-Identifier: GPL-2.0 OR Apache 2.0\n/*");

    replace_all(content, "$VERSION_COMMENT", getVersionComment());

    if (m_rte == "LinuxKernel")
        replace_one(content, "static $PROVIDER_NAME_data_t *s_ins = NULL;", "static $PROVIDER_NAME_data_t *s_ins;");

    replace_all(content, "$PID_VALUE", m_srcEndID);
    replace_all(content, "$FID_VALUE", m_fid);
    replace_all(content, "$SID_VALUE", std::to_string(m_sid).append("U"));
    replace_all(content, "$INS_CASES1", insCases);
    replace_all(insCases, "dispatch_broadcast", "dispatch_reply");
    replace_all(content, "$INS_CASES2", insCases);
    replace_all(content, "$INS_INIT", insInits);
    replace_all(content, "$INS_DESTROY", insDestroy);
    replace_all(content, "$PROVIDER_NAME", providerName);
    replace_all(content, "    ", "\t");
    replace_all(content, "\r\n", "\n");
    replace_all(content, "\t\n", "\n");
    // write file
    std::ofstream ofs(filename, std::ofstream::out | std::ios::binary);
    ofs << content;
    ofs.close();

    return true;
}

std::string MsgBoxGenerator::getVersionComment()
{
    std::string ret =  R"(
/* This file is auto generated for message box v$VERSION.
 * All manual modifications will be LOST by next generation.
 * It is recommended NOT modify it.
 * Generator Version: francaidl $GEN_VER1 msgbx_ipc $GEN_VER2
 */
)";
    replace_all(ret, "$VERSION", MsgBoxGenerator::version());
    std::string gen_ver;
    gen_ver = COMMIT_HASH;
    replace_all(ret, "$GEN_VER1", gen_ver);
    replace_all(ret, "$GEN_VER2", COMMIT_HASH2);
    return ret;
}

} // namespace BstIdl