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
#include "msgbox-tools/MsgBoxInfGenerator.h"
#include "FDModel/FDEnumerator.h"
#include "FDModel/FDModelManager.h"
#include "msgbox-tools/transformer/typedecl.h"
#include "msgbox-tools/transformer/typename.h"
#include "msgbox-tools/transformer/deserialize.h"
#include "msgbox-tools/transformer/serialize.h"
#include "msgbox-tools/transformer/serdes.h"
#include "msgbox-tools/transformer/outasgn.h"
#include "msgbox-tools/transformer/replaceEnum.h"
#include "utilities/string_utility.h"
#include "msgbox-tools/MsgBoxGenerator.h"
#include <fstream>

namespace BstIdl
{

MsgBoxInfGenerator::MsgBoxInfGenerator(const std::string &dest_dir, const std::string &src_id, const std::shared_ptr<FDExtensionElement> &instance)
        : AbstractGenerator(dest_dir), m_srcEndID(src_id), m_instance(instance)
{
    setLicense(R"(/* SPDX-License-Identifier: GPL-2.0 OR Apache 2.0
 *
 * Copyright (c) 2024 Black Sesame Technologies
 *
 * This program is also distributed under the terms of the Apache 2.0
 * License.
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
 */)");
}

bool MsgBoxInfGenerator::generate()
{
    if (!validate(m_bCheckDstEndID))
    {
        CERR << "Validation failed." << ENDL;
        return false;
    }

    auto path = "/";
    m_folderPath = mk_dir(m_destDir, path);
    if (m_folderPath.empty())
        return false;

    bool ret = generateTypeHeader();
    if (!ret)
    {
        CERR << "Generate datatype header failed." << ENDL;
        return false;
    }

    ret = generateHeader();
    if (!ret)
    {
        CERR << "Generate header failed." << ENDL;
        return false;
    }

    ret = generateSource();
    if (!ret)
    {
        CERR << "Generate source failed." << ENDL;
        return false;
    }

    return true;
}

bool MsgBoxInfGenerator::generateTypeHeader()
{
    std::string content = R"($LICENSE
$VERSION_COMMENT
#ifndef $HEADER_MACRO
#define $HEADER_MACRO

#ifdef IPC_RTE_KERNEL
#include <bst/ipc_app_common.h>
#else
#include "ipc_app_common.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

// user defined types
$TYPES_DECL

// constants
$CONSTANT_DECL

// type serialize / deserialize functions
$TYPE_SERDES_FUNC

#ifdef __cplusplus
}
#endif

#endif
)";

    std::string typeName = m_infName + "_datatype" ;
    std::string headerMacro = toUpper(typeName) + "_H";
    std::string typesDecl;
    std::string constantDecl;
    std::string serdesFunc;

    auto inf = m_interface->getTarget();
    for (const auto &t : inf->getTypes())
    {
        typesDecl.append(Transformer::MsgBoxGen::getTypeDecl(t));
        serdesFunc.append(Transformer::MsgBoxGen::serdes(t));
    }
    for (const auto &m : inf->getMethods())
    {
        if (auto ptr = m->getErrors())
        {
            typesDecl.append(Transformer::MsgBoxGen::getTypeDecl(ptr));
            serdesFunc.append(Transformer::MsgBoxGen::serdes(ptr));
        }
    }
    trim(typesDecl);

    for (const auto &c : inf->getConstants())
    {
        constantDecl.append(Transformer::MsgBoxGen::getTypeDecl(c));
    }
    trim(constantDecl);

    replace_all(content, "$LICENSE", getLicense());
    replace_all(content, "$VERSION_COMMENT", getVersionComment());
    replace_all(content, "$HEADER_MACRO", headerMacro);
    replace_all(content, "$TYPES_DECL", typesDecl);
    replace_all(content, "$CONSTANT_DECL", constantDecl);
    replace_all(content, "$TYPE_SERDES_FUNC", serdesFunc);
    replace_all(content, "    ", "\t");
    replace_all(content, "\r\n", "\n");
    replace_all(content, "\t\n", "\n");
    // write file
    auto filename = m_folderPath + "/" + typeName + ".h";
    std::ofstream ofs(filename, std::ofstream::out | std::ios::binary);
    ofs << content;
    ofs.close();

    return true;
}

bool MsgBoxInfGenerator::validate(bool checkDstEndID)
{
    if (!m_instance || m_instance->getTag() != "instance" || !m_instance->getTarget())
        return false;

    m_infName.clear();
    m_fid.clear();
    m_sid = 255;
    m_interface = nullptr;
    std::shared_ptr<FDValue> value = nullptr;
    std::shared_ptr<FDEnumerator> type = nullptr;

    // get and validate DstEndID
    if (checkDstEndID)
    {
        value = m_instance->getSingleValue("DstEndID");
        if (!value)
        {
            CERR << "No DstEndID defined." << ENDL;
            return false;
        }
        type = value->getEnumerator();
        if (!type)
        {
            CERR << "Invalid DstEndID." << ENDL;
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
            CERR << "Invalid DstEndID." << ENDL;
            return false;
        }
    }

    // find FDInterface
    auto &fdmodels = BstIdl::FDModelManager::getInstance();
    if (fdmodels.size() == 0)
    {
        CERR << "No valid models!" << ENDL;
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
        CERR << "No valid inteface!" << ENDL;
        return false;
    }

    // get stub name
    auto infName = m_interface->getName();
    m_infName = infName.substr(infName.find_last_of('.') + 1);

    // get Boolean EnableSection
    bool ret = getBooleanProperty(m_interface, "EnableSection", &m_bEnableSection);
    if (!ret)
    {
        CERR << "Invalid EnableSection." << ENDL;
        return false;
    }

    // get MaxMethodNum, MaxBroadcastNum, minSubscribeID
    int32_t minSubscribeID = 0;
    if (!m_bEnableSection)
    {
        // 设置为默认值
        m_maxMethodNum = 255;
        m_maxBroadcastNum = 255;
        minSubscribeID = 0;
    }
    else
    {
        // get MaxMethodNum
        bool ret = getIntegerProperty(m_interface, "MaxMethodNum", &m_maxMethodNum);
        if (!ret || m_maxMethodNum < 0 || m_maxMethodNum > 255)
        {
            CERR << "Invalid MaxMethodNum." << ENDL;
            return false;
        }

        // get MaxBroadcastNum
        ret = getIntegerProperty(m_interface, "MaxBroadcastNum", &m_maxBroadcastNum);
        if (!ret || m_maxBroadcastNum < 0 || m_maxBroadcastNum > 255)
        {
            CERR << "Invalid MaxBroadcastNum." << ENDL;
            return false;
        }

        // set SubscribeMethodMinID which is equal to UnSubscribeMethodMinID
        minSubscribeID = m_maxMethodNum;
    }

    // check validation and duplication for MethodID
    std::list<int32_t> checker;
    for (const auto &m : m_interface->getMethods())
    {
        auto method = m->getTarget();
        int32_t id = -1;
        auto ret = getIntegerProperty(m, "MethodID", &id);
        if (!ret || id < 0 || id >= m_maxMethodNum)
        {
            CERR << "Invalid MethodID for method " << m->getName() << ENDL;
            return false;
        }
        if (!addListItem(checker, id))
        {
            CERR << "Duplicate MethodID for method " << m->getName() << ENDL;
            return false;
        }
        if (!method->getErrorType() && !method->isFireAndForget())
        {
            CERR << "Missing error definition for method " << m->getName() << ENDL;
            return false;
        }

    }

    // check validation and duplication for SubscribeMethodID, UnsubscribeMethodID, BroadcastID
    std::list<int32_t> checker2;
    for (const auto &b : m_interface->getBroadcasts())
    {
        auto name = b->getTarget()->getName();
        auto upperName = toUpper(name);
        int32_t id = -1;
        auto ret = getIntegerProperty(b, "SubscribeMethodID", &id);
        if (!ret || id < minSubscribeID || id > 255)
        {
            CERR << "Invalid SubscribeMethodID for broadcast " << b->getName() <<
                ". value should greater than or equal to " << minSubscribeID << ", and less than or equal to 255" << ENDL;
            return false;
        }
        if (!addListItem(checker, id))
        {
            CERR << "Duplicate SubscribeMethodID for broadcast " << b->getName() << ENDL;
            return false;
        }
        ret = getIntegerProperty(b, "UnsubscribeMethodID", &id);
        if (!ret || id < minSubscribeID || id > 255)
        {
            CERR << "Invalid UnsubscribeMethodID for broadcast " << b->getName() <<
                ". value should greater than or equal to " << minSubscribeID << ", and less than or equal to 255" << ENDL;
            return false;
        }
        if (!addListItem(checker, id))
        {
            CERR << "Duplicate UnsubscribeMethodID for broadcast " << b->getName() << ENDL;
            return false;
        }

        ret = getIntegerProperty(b, "BroadcastID", &id);
        if (!ret || id < 0 || id >= m_maxBroadcastNum)
        {
            CERR << "Invalid BroadcastID for " << b->getName() << ENDL;
            return false;
        }
        if (!addListItem(checker2, id))
        {
            CERR << "Duplicate BroadcastID for broadcast " << b->getName() << ENDL;
            return false;
        }
    }
    checker.clear();
    checker2.clear();

    // get GenerateReplyMethod.
    value = m_instance->getSingleValue("GenerateReplyMethod");
    if (!value)
    {
        CERR << "No GenerateReplyMethod defined." << ENDL;
        return false;
    }
    if (!value->isBoolean())
    {
        CERR << "Invalid GenerateReplyMethod." << ENDL;
        return false;
    }
    m_bSeparateReply = value->getBoolean();

    // get NotUseSwitchCase.
    value = m_instance->getSingleValue("NotUseSwitchCase");
    if (!value)
    {
        CERR << "No NotUseSwitchCase defined." << ENDL;
        return false;
    }
    if (!value->isBoolean())
    {
        CERR << "Invalid NotUseSwitchCase." << ENDL;
        return false;
    }
    m_bNotUseSwitchCase = value->getBoolean();

    // check anonymous array in struct fields
    auto& types = m_interface->getTarget()->getTypes();
    for (auto iter = types.begin(); iter != types.end(); ++iter)
    {
        if (auto ptr = std::dynamic_pointer_cast<BstIdl::FStructType>(*iter))
        {
            for(auto& e : ptr->getElements())
            {
                if (e->isArray())
                {
                    auto newType = convertImplicitArray(e);
                    if (!newType)
                        return false;
                    newType->setContainer(m_interface->getTarget());
                    if (!containsByName(types, newType))
                        iter = types.emplace(iter, newType);
                }
            }
        }
        if (auto ptr = std::dynamic_pointer_cast<BstIdl::FArrayType>(*iter))
        {
            if (ptr->isFixedSize() && ptr->getFixedSize())
            {
                auto size = ptr->getFixedSize();
                auto size_str = size->toString();
                if (!isWord(size_str))
                {
                    CERR << "Invalid Fixed Size in " << ptr->getName() << ENDL;
                    return false;
                }
            }
        }
    }

    // check anonymous array in arguments.
    std::list<std::shared_ptr<FArgument>> arg_list;
    for (auto m : m_interface->getTarget()->getMethods())
    {
        auto in = m->getInArgs();
        arg_list.splice(arg_list.end(), std::move(in));
        auto out = m->getOutArgs();
        arg_list.splice(arg_list.end(), std::move(out));
    }
    for (auto b : m_interface->getTarget()->getBroadcasts())
    {
        auto out = b->getOutArgs();
        arg_list.splice(arg_list.end(), std::move(out));
    }
    for (auto arg : arg_list)
    {
        if (arg->isArray())
        {
            auto newType = convertImplicitArray(arg);
            if (!newType)
                return false;
            newType->setContainer(m_interface->getTarget());
            addListItemByName(types, newType);
        }
    }

    // replace enumeration inside array, struct, union
    for (auto t : m_interface->getTarget()->getTypes())
        (void)Transformer::MsgBoxGen::replaceEnum(t);
    return true;
}

std::shared_ptr<FArrayType> MsgBoxInfGenerator::convertImplicitArray(const std::shared_ptr<FTypedElement> &element)
{
    if (!element || !element->isArray())
        return nullptr;

    // make and set the new array type.
    auto newType = std::make_shared<BstIdl::FArrayType>();
    newType->setElementType(element->getType());
    if (element->isFixedSize())
    {
        auto size_str = element->getFixedSize()->toString();
        if (!isWord(size_str))
        {
            CERR << "Invalid Fixed Size in " << element->getFQN() << ENDL;
            return nullptr;
        }
        newType->setName(element->getType()->getName() + "Array" + element->getFixedSize()->toString());
    }
    else
    {
        newType->setName(element->getType()->getName() + "Array");
    }
    newType->setFixedSize(element->getFixedSize());

    // set the element type to the new array type.
    auto typeref = std::make_shared<BstIdl::FTypeRef>();
    typeref->setDerived(newType);
    element->setType(typeref);
    element->setArray(false);
    // element->setFixedSize(nullptr);

    return newType;
}

bool MsgBoxInfGenerator::getIntegerProperty(const std::shared_ptr<FDElement> &element, const std::string &property, int32_t *value)
{
    if (!element || !value)
    {
        CERR << "Invalid element." << ENDL;
        return false;
    }
    auto val = element->getSingleValue(property);
    if (!val)
    {
        CERR << "No " << property << " defined." << ENDL;
        return false;
    }
    if (!val->isInteger())
    {
        CERR << "Invalid " << property << ENDL;
        return false;
    }
    *value = val->getInteger();
    return true;
}

bool MsgBoxInfGenerator::getBooleanProperty(const std::shared_ptr<FDElement> &element, const std::string &property, bool *value)
{
    if (!element || !value)
    {
        CERR << "Invalid element." << ENDL;
        return false;
    }
    auto val = element->getSingleValue(property);
    if (!val)
    {
        CERR << "No " << property << " defined." << ENDL;
        return false;
    }
    if (!val->isBoolean())
    {
        CERR << "Invalid " << property << ENDL;
        return false;
    }
    *value = val->getBoolean();
    return true;
}

std::string MsgBoxInfGenerator::getVersionString()
{
    auto ver = m_interface->getTarget()->getVersion();
    std::string version = "#define MAJOR $MAJORU\n#define MINOR $MINORU\n";
    replace_all(version, "$MAJOR", std::to_string(ver->getMajor()));
    replace_all(version, "$MINOR", std::to_string(ver->getMinor()));
    return version;
}

std::string MsgBoxInfGenerator::getStubMethodSignature(const std::shared_ptr<FMethod> &method)
{
    if (!method)
        return "";

    std::string ret = R"(/**
 * Stub function for method $NAME.
 *
$ARG_COMMENT
 */
typedef void (*$METHOD_TYPE)(
                $ARGS
                );
)";
    replace_all(ret, "$METHOD_TYPE", Transformer::MsgBoxGen::getTypeName(method));
    std::list<std::string> arg_list;
    std::list<std::string> comments;
    for (const auto &a : method->getInArgs())
    {
        if (isFixedDerived(a->getType()))
            arg_list.emplace_back("const " + Transformer::MsgBoxGen::getTypeName(a->getType()) + " *" + a->getName());
        else
            arg_list.emplace_back("const " + Transformer::MsgBoxGen::getTypeName(a->getType()) + " " + a->getName());
        comments.emplace_back(replace_all(" * @param $ARG_NAME The input argument of method $NAME.", "$ARG_NAME", a->getName()));
    }
    if (m_bSeparateReply && !method->isFireAndForget())
    {
        arg_list.emplace_back("const uint64_t context");
        comments.emplace_back(" * @param context The message context, to be passed to the reply function.");
    }
    else
    {
        for (const auto &a : method->getOutArgs())
        {
            arg_list.emplace_back(Transformer::MsgBoxGen::getTypeName(a->getType()) + " *" + a->getName());
            comments.emplace_back(replace_all(" * @param $ARG_NAME The output argument of method $NAME.", "$ARG_NAME", a->getName()));
        }
        if (auto ptr = method->getErrorType())
        {
            arg_list.emplace_back(Transformer::MsgBoxGen::getTypeName(ptr) + " *err");
            comments.emplace_back(" * @param err The error code returned to the client.");
        }
    }
    arg_list.emplace_back("const ext_info_t *info");
    comments.emplace_back(" * @param info The extended information, containing uuid and timestamp.");
    auto args = join(arg_list, ",\n\t\t\t\t");
    replace_all(args, " * ", " *");
    replace_all(ret, "$ARGS", args);
    replace_all(ret, "$ARG_COMMENT", join(comments, "\n"));
    replace_all(ret, "$NAME", method->getName());
    return ret;
}

std::string MsgBoxInfGenerator::getStubMethodRegisterSignature(const std::shared_ptr<FMethod> &method)
{
    if (!method)
        return "";

    std::string ret = R"(
/**
 * Register stub function for method $NAME.
 *
 * @param func The stub function to be registered.
 * @return 0 if success, negative if fail.
 */
int32_t (*register_$NAME)($TYPE func);
)";
    replace_all(ret, "$NAME", method->getName());
    replace_all(ret, "$TYPE", Transformer::MsgBoxGen::getTypeName(method));
    return ret;
}

std::string MsgBoxInfGenerator::getStubMethodRegisterImpl(const std::shared_ptr<FMethod> &method)
{
    if (!method)
        return "";

    std::string ret = R"(
static int32_t register_$NAME($METHOD_TYPE func)
{
    if (!s_ext)
        return -ERR_APP_PARAM;
    s_ext->$NAME_ptr = func;
    return RESULT_SUCCESS;
}
)";
    replace_all(ret, "$METHOD_TYPE", Transformer::MsgBoxGen::getTypeName(method));
    replace_all(ret, "$NAME", method->getName());
    return ret;
}

std::string MsgBoxInfGenerator::getStubMethodReplySignature(const std::shared_ptr<FMethod> &method)
{
    if (!method || !m_bSeparateReply || method->isFireAndForget())
        return "";

    std::string ret = R"(
/**
 * Send the reply message of method $NAME.
 *
$ARG_COMMENT
 * @return 0 if success, negative if fail.
 */
int32_t (*reply_$NAME)(
                $ARGS);
)";
    replace_all(ret, "$NAME", method->getName());
    std::list<std::string> arg_list;
    std::list<std::string> comments;
    for (const auto &a : method->getOutArgs())
    {
        if (isFixedDerived(a->getType()))
            arg_list.emplace_back("const " + Transformer::MsgBoxGen::getTypeName(a->getType()) + " *" + a->getName());
        else
            arg_list.emplace_back("const " + Transformer::MsgBoxGen::getTypeName(a->getType()) + " " + a->getName());
        comments.emplace_back(replace_all(" * @param $ARG_NAME The output argument of method $NAME.", "$ARG_NAME", a->getName()));
    }
    if (auto ptr = method->getErrorType())
    {
        arg_list.emplace_back("const " + Transformer::MsgBoxGen::getTypeName(ptr) + " err");
        comments.emplace_back(" * @param err The error code returned to the client.");
    }
    arg_list.emplace_back("const uint64_t context");
    comments.emplace_back(" * @param context The message context, got from the method stub function.");
    auto args = join(arg_list, ",\n\t\t\t\t");
    replace_all(args, " * ", " *");
    replace_all(ret, "$ARGS", args);
    replace_all(ret, "$ARG_COMMENT", join(comments, "\n"));
    replace_all(ret, "$NAME", method->getName());
    return ret;
}

std::string MsgBoxInfGenerator::getStubMethodPtrInit(const std::shared_ptr<FMethod> &method)
{
    if (!method)
        return "";

    std::string ret = "ext->$NAME_ptr = NULL;\n\tserver->register_$NAME = register_$NAME;\n\t";
    if (m_bSeparateReply && !method->isFireAndForget())
        ret.append("server->reply_$NAME = reply_$NAME;\n\t");
    replace_all(ret, "$NAME", method->getName());
    return ret;
}

std::string MsgBoxInfGenerator::getStubMethodPtrDestroy(const std::shared_ptr<FMethod> &method)
{
    if (!method)
        return "";

    std::string ret = "s_data->$NAME_ptr = NULL;\n\ts_data->server.register_$NAME = NULL;\n\t";
    if (m_bSeparateReply)
        ret.append("s_data->server.reply_$NAME = NULL;\n\t");
    replace_all(ret, "$NAME", method->getName());
    return ret;
}

std::string MsgBoxInfGenerator::getStubMethodCallFunc(const std::shared_ptr<FMethod> &method)
{
    if (!method)
        return "";

    std::string name = method->getName();
    std::string ret = getStubMethodCallFuncTpl();
    if (method->getInArgs().empty())
    {
        replace_all(ret, "$DESERIALIZE", "");
        replace_all(ret, "$DES_VARS\n\t", "");
    }
    else
    {
        std::string desTpl = R"(
    buf = &data->des_buf;
    clear_des_buf(buf);
	len = ipc_des_get_all(des, (uint8_t *)buf->data_buf);
	if (len <= 0)
		return -ERR_APP_SERDES;
	buf->unavail_data_size = IPC_MAX_DATA_SIZE - len;

    $DESERIALIZE
    if (ret < 0)
        return -ERR_APP_SERDES;
)";
        replace_all(ret, "$DESERIALIZE", desTpl);
        std::string desVarTpl = R"(int32_t ret = 0;
    des_buf_t *buf = NULL;
	uint32_t len = 0;)";
        replace_all(ret, "$DES_VARS", desVarTpl);
    }


    if (m_bSeparateReply && !method->isFireAndForget())
    {
        std::string reply_func = getStubMethodReplyFuncTpl();
        std::string reply_func_name = getStubMethodReplySignature(method);
        reply_func_name = reply_func_name.substr(reply_func_name.find("*/") + 2, -1);
        trim(reply_func_name);
        replace_all(reply_func_name, "(*reply_" + name + ")", "reply_" + name);
        remove_all(reply_func_name, ";");
        replace_all(reply_func_name, "(*reply_" + name + ")", "reply_" + name);
        replace_all(reply_func, "$REPLY_FUNC", reply_func_name);
        ret.append(reply_func);
    }
    else
    {
        remove_all(ret, "    uintptr_t pcon = 0;\n");
        remove_all(ret, "    pcon = (uintptr_t)&des->header;\n");
    }

    std::string initVars;
    std::string des;
    std::string init;
    std::string ser;
    std::list<std::string> arg_list;
    for (const auto &a : method->getInArgs())
    {
        initVars.append(BstIdl::Transformer::MsgBoxGen::initVar(a));
        des.append(BstIdl::Transformer::MsgBoxGen::deserialize(a));
        arg_list.emplace_back(a->getName());
    }
    replace_all(initVars, "\n", "\n\t");
    replace_all(des, "\n", "\n\t");
    auto err = method->getErrorType();
    if (err)
    {
        auto errArg = method->getErrorArg();
        auto errVar = BstIdl::Transformer::MsgBoxGen::initVar(errArg);
        init.append(errVar);
        auto errSer = BstIdl::Transformer::MsgBoxGen::serialize(errArg);
        ser.append(errSer);
    }
    for (const auto &a : method->getOutArgs())
    {
        init.append(BstIdl::Transformer::MsgBoxGen::initVar(a));
        if (!m_bSeparateReply)
            arg_list.emplace_back("&" + a->getName());
        ser.append(BstIdl::Transformer::MsgBoxGen::serialize(a));
    }
    if (err && !m_bSeparateReply)
        arg_list.emplace_back("&err");
    if (m_bSeparateReply && !method->isFireAndForget())
        arg_list.emplace_back("*(uint64_t *)pcon");
    arg_list.emplace_back("&data->info");
    replace_all(init, "\n", "\n\t");
    replace_all(ser, "\n", "\n\t");

    replace_all(ret, "$DESERIALIZE", des);
    replace_all(ret, "$INIT_VARS", initVars);
    replace_all(ret, "$INIT", init);
    replace_all(ret, "$NAME", method->getName());
    replace_all(ret, "$ARGS", join(arg_list, ", "));
    replace_all(ret, "$SERIALIZE", ser);

    return ret;
}

std::string MsgBoxInfGenerator::getStubMethodCase(const std::shared_ptr<FMethod> &method)
{
    if (!method)
        return "";

    std::string ret = getStubDispatchMessageCaseTpl(method->isFireAndForget());
    replace_all(ret, "$NAME", method->getName());
    replace_all(ret, "$UPPER_NAME", toUpper(method->getName()));
    replace_all(ret, "\n", "\n\t");
    return ret;
}

std::string MsgBoxInfGenerator::getStubMethodVars(const std::shared_ptr<FMethod> &method)
{
    if (!method)
        return "";

    std::string ret = "static $METHOD_TYPE s_data->$NAME_ptr = NULL;\n";
    replace_all(ret, "$METHOD_TYPE", Transformer::MsgBoxGen::getTypeName(method));
    replace_all(ret, "$NAME", method->getName());
    return ret;
}

std::string MsgBoxInfGenerator::getStubMethodInternalVarDecls(const std::shared_ptr<FMethod> &method)
{
    if (!method)
        return "";

    std::string ret = "$METHOD_TYPE $NAME_ptr;\n";
    replace_all(ret, "$METHOD_TYPE", Transformer::MsgBoxGen::getTypeName(method));
    replace_all(ret, "$NAME", method->getName());
    return ret;
}

std::string MsgBoxInfGenerator::getMethodCmds(const std::shared_ptr<FDMethod> &method)
{
    if (!method)
        return "";

    std::string ret = "#define CMD_METHOD_$UPPER_NAME $IDU\n";
    replace_all(ret, "$UPPER_NAME", toUpper(method->getTarget()->getName()));
    int32_t id = -1;
    (void)getIntegerProperty(method, "MethodID", &id);
    replace_all(ret, "$ID", std::to_string(id));
    return ret;
}

std::string MsgBoxInfGenerator::getStubBroadcastSignature(const std::shared_ptr<FBroadcast> &broadcast)
{
    if (!broadcast)
        return "";

    std::string ret = R"(/**
 * Send broadcast $NAME to all subscribers.
 *
$ARG_COMMENT
 */
int32_t (*$NAME)($ARGS);

/**
 * Register subscribed callback function for broadcast $NAME
 *
 * @param func The callback function to be registered.
 * @return 0 if success, negative if fail.
 */
int32_t (*register_$NAME_subcribed)(broadcast_sub_t func);

/**
 * Register unsubscribed callback function for broadcast $NAME
 *
 * @param func The callback function to be registered.
 * @return 0 if success, negative if fail.
 */
int32_t (*register_$NAME_unsubcribed)(broadcast_sub_t func);
)";

    std::list<std::string> arg_list;
    std::list<std::string> comments;
    for (const auto &a : broadcast->getOutArgs())
    {
        if (isFixedDerived(a->getType()))
            arg_list.emplace_back("const " + Transformer::MsgBoxGen::getTypeName(a->getType()) + " *" + a->getName());
        else    
            arg_list.emplace_back("const " + Transformer::MsgBoxGen::getTypeName(a->getType()) + " " + a->getName());
        comments.emplace_back(replace_all(" * @param $ARG_NAME The output argument of broadcast $NAME.", "$ARG_NAME", a->getName()));
    }
    if (broadcast->isSelective())
    {
        arg_list.emplace_back("broadcast_reg_entry_t *entries");
        comments.emplace_back(" * @param entries The selected entries to broadcast.");
        arg_list.emplace_back("uint32_t size");
        comments.emplace_back(" * @param size The size of the entries.");
    }
    replace_all(ret, "$ARGS", join(arg_list, ", "));
    replace_all(ret, "$ARG_COMMENT", join(comments, "\n"));
    replace_all(ret, "$NAME", broadcast->getName());

    return ret;
}

std::string MsgBoxInfGenerator::getStubBroadcastCallFunc(const std::shared_ptr<FBroadcast> &broadcast)
{
    if (!broadcast)
        return "";

    std::string ret = R"(
static int32_t register_$NAME_subcribed(broadcast_sub_t func)
{
    if (!s_ext)
        return -ERR_APP_PARAM;
    s_ext->$NAME_sub_ptr = func;
    return RESULT_SUCCESS;
}

static int32_t register_$NAME_unsubcribed(broadcast_sub_t func)
{
    if (!s_ext)
        return -ERR_APP_PARAM;
    s_ext->$NAME_unsub_ptr = func;
    return RESULT_SUCCESS;
}

static int32_t $NAME($ARGS)
{
    int32_t ret = 0;
    int32_t send_ret = 0;
    int32_t index = 0;
    rw_msg_header_t header = { 0 };
    broadcast_reg_entry_t *entry = NULL;
	broadcast_registry_t *reg = NULL;
#ifdef IPC_RTE_BAREMETAL
	serdes_t *ser = NULL;
#else
	serdes_t serdes = { 0 };
	serdes_t *ser = &serdes;
#endif
	com_server_data_t *data = s_data;

	if (!data || !s_ext)
		return -ERR_APP_PARAM;
#ifdef IPC_RTE_BAREMETAL
	ser = &data->serializer;
#endif
	header.pid = data->pid;
    header.cmd = CMD_BROADCAST_$UPPER_NAME;
    header.typ = MSGBX_MSG_TYPE_BROADCAST;
    ret = ipc_ser_init(ser);
    $SERIALIZE
    if (ret < 0)
	    return -ERR_APP_SERDES;

	reg = &s_ext->$NAME_registry;
	entry = reg->entries + reg->start;
	for (index = reg->start; index < reg->end; ++index, ++entry) {
        if (entry->pid != 0) {
            header.cid = entry->pid;
            header.fid = entry->fid;
            header.sid = entry->sid;
            header.tok = data->token;
            ipc_ser_set_header(ser, header);
            ipc_ser_finish(ser);
            send_ret = ipc_trans_layer_stub_send_broadcast(data->pid, data->handle, ser);
            if (send_ret < 0)
                IPC_LOG_ERR("send broadcast fail %d.\n", send_ret);
            else
                ++ret;
        }
    }
    increase_token(data);
    return ret;
})";

    std::string name = broadcast->getName();
    std::string ser;
    std::list<std::string> arg_list;
    for (const auto &a : broadcast->getOutArgs())
    {
        ser.append(BstIdl::Transformer::MsgBoxGen::serialize(a));
        if (isFixedDerived(a->getType()))
            arg_list.emplace_back("const " + Transformer::MsgBoxGen::getTypeName(a->getType()) + " *" + a->getName());
        else
            arg_list.emplace_back("const " + Transformer::MsgBoxGen::getTypeName(a->getType()) + " " + a->getName());
    }

    if (broadcast->isSelective())
    {
        arg_list.emplace_back("broadcast_reg_entry_t *entries");
        arg_list.emplace_back("uint32_t size");
        replace_one(ret, "broadcast_reg_entry_t *entry = NULL;", "broadcast_reg_entry_t *entry = entries;");
        remove_all(ret, "	broadcast_registry_t *reg = NULL;\n");
        // replace_one(ret, "if (!data || !s_ext)", "if (!data || !s_ext || !entry || size == 0)");
        replace_one(ret, "	reg = &s_ext->$NAME_registry;\n\tentry = reg->entries + reg->start;\n", 
        R"(	if (!entry || size == 0) {
		broadcast_registry_t *reg = &s_ext->$NAME_registry;
		entry = reg->entries + reg->start;
		size = reg->end - reg->start;
	}
)");
        replace_one(ret, "for (index = reg->start; index < reg->end; ++index, ++entry) {", "for (index = 0; index < size; ++index, ++entry) {");
    }
    
    replace_all(ret, "$NAME", name);
    replace_all(ret, "$UPPER_NAME", toUpper(name));
    replace_all(ser, "\n", "\n\t");
    replace_all(ret, "$SERIALIZE", ser);
    replace_all(ret, "$ARGS", join(arg_list, ", "));
    return ret;
}

std::string MsgBoxInfGenerator::getStubBroadcastCase(const std::shared_ptr<FBroadcast> &broadcast)
{
    if (!broadcast)
        return "";

    std::string ret;
    if (m_bNotUseSwitchCase)
        ret = R"(if (des->header.cmd == CMD_METHOD_SUB_$UPPER_NAME) {
    if (s_ext->$NAME_sub_ptr)
        ret = (*s_ext->$NAME_sub_ptr)((uint8_t)des->header.pid,
                (uint8_t)des->header.fid, (uint8_t)des->header.sid);
    if (ret >= 0)
        ret = add_registration(&s_ext->$NAME_registry,
                (uint8_t)des->header.pid, (uint8_t)des->header.fid,
                (uint8_t)des->header.sid);
    *reply = true;
    return ret;
}
if (des->header.cmd == CMD_METHOD_UNSUB_$UPPER_NAME) {
    if (s_ext->$NAME_unsub_ptr)
        ret = (*s_ext->$NAME_unsub_ptr)((uint8_t)des->header.pid,
                (uint8_t)des->header.fid, (uint8_t)des->header.sid);
    if (ret >= 0)
        ret = remove_registration(&s_ext->$NAME_registry,
                (uint8_t)des->header.pid, (uint8_t)des->header.fid,
                (uint8_t)des->header.sid);
    *reply = true;
    return ret;
}
)";
    else
        ret = R"(case CMD_METHOD_SUB_$UPPER_NAME:
    if (s_ext->$NAME_sub_ptr)
        ret = (*s_ext->$NAME_sub_ptr)((uint8_t)des->header.pid,
                (uint8_t)des->header.fid, (uint8_t)des->header.sid);
    if (ret >= 0)
        ret = add_registration(&s_ext->$NAME_registry,
                (uint8_t)des->header.pid, (uint8_t)des->header.fid,
                (uint8_t)des->header.sid);
    *reply = true;
    return ret;
case CMD_METHOD_UNSUB_$UPPER_NAME:
    if (s_ext->$NAME_unsub_ptr)
        ret = (*s_ext->$NAME_unsub_ptr)((uint8_t)des->header.pid,
                (uint8_t)des->header.fid, (uint8_t)des->header.sid);
    if (ret >= 0)
        ret = remove_registration(&s_ext->$NAME_registry,
                (uint8_t)des->header.pid, (uint8_t)des->header.fid,
                (uint8_t)des->header.sid);
    *reply = true;
    return ret;
)";

    replace_all(ret, "\n", "\n\t");
    replace_all(ret, "$NAME", broadcast->getName());
    replace_all(ret, "$UPPER_NAME", toUpper(broadcast->getName()));
    return ret;
}

std::string MsgBoxInfGenerator::getStubBroadcastPtrInit(const std::shared_ptr<FBroadcast> &broadcast)
{
    if (!broadcast)
        return "";

    std::string ret = R"(server->$NAME = $NAME;
    server->register_$NAME_subcribed = register_$NAME_subcribed;
    server->register_$NAME_unsubcribed = register_$NAME_unsubcribed;
    )";
    replace_all(ret, "$NAME", broadcast->getName());
    return ret;
}

std::string MsgBoxInfGenerator::getStubBroadcastPtrDestroy(const std::shared_ptr<FBroadcast> &broadcast)
{
    if (!broadcast)
        return "";

    std::string ret = R"(s_data->server.$NAME = NULL;
    s_data->server.register_$NAME_subcribed = NULL;
    s_data->server.register_$NAME_unsubcribed = NULL;
    )";
    replace_all(ret, "$NAME", broadcast->getName());
    return ret;
}

std::string MsgBoxInfGenerator::getStubBroadcastVars(const std::shared_ptr<FBroadcast> &broadcast)
{
    if (!broadcast)
        return "";

    std::string ret = R"(static broadcast_sub_t s_data->$NAME_sub_ptr = NULL;
static broadcast_sub_t s_data->$NAME_unsub_ptr = NULL;
static broadcast_registry s_data->$NAME_registry = { 0 };
)";
    replace_all(ret, "$NAME", broadcast->getName());
    return ret;
}

std::string MsgBoxInfGenerator::getStubBroadcastInternalVarDecls(const std::shared_ptr<FBroadcast> &broadcast)
{
    if (!broadcast)
        return "";

    std::string ret = R"(broadcast_sub_t $NAME_sub_ptr;
broadcast_sub_t $NAME_unsub_ptr;
broadcast_registry_t $NAME_registry;
)";
    replace_all(ret, "$NAME", broadcast->getName());
    return ret;
}

std::string MsgBoxInfGenerator::getBroadcastCmds(const std::shared_ptr<FDBroadcast> &broadcast)
{
    if (!broadcast)
        return "";

    std::string ret = R"(#define CMD_METHOD_SUB_$UPPER_NAME $ID1U
#define CMD_METHOD_UNSUB_$UPPER_NAME $ID2U
#define CMD_BROADCAST_$UPPER_NAME $ID3U
)";
    replace_all(ret, "$UPPER_NAME", toUpper(broadcast->getTarget()->getName()));
    int32_t id = -1;
    (void)getIntegerProperty(broadcast, "SubscribeMethodID", &id);
    replace_all(ret, "$ID1", std::to_string(id));
    (void)getIntegerProperty(broadcast, "UnsubscribeMethodID", &id);
    replace_all(ret, "$ID2", std::to_string(id));
    (void)getIntegerProperty(broadcast, "BroadcastID", &id);
    replace_all(ret, "$ID3", std::to_string(id));
    return ret;
}


std::string MsgBoxInfGenerator::getStubDispatchMessageTpl()
{
    std::string ret;
    if (m_bNotUseSwitchCase)
        ret = R"(
// dispatch_request
static int32_t dispatch_request(serdes_t *des, bool *reply)
{
	int32_t ret = 0;

	if (!des)
		return -ERR_APP_PARAM;

    $METHOD_CASE
    $BROADCAST_CASE
	ret = -ERR_APP_UNKNOWN_CMD;
	*reply = false;
	return ret;
}
)";
    else
        ret = R"(
// dispatch_request
static int32_t dispatch_request(serdes_t *des, bool *reply)
{
	int32_t ret = 0;

	if (!des)
		return -ERR_APP_PARAM;

	switch (des->header.cmd) {
    $METHOD_CASE
    $BROADCAST_CASE
	default:
		break;
	}
	ret = -ERR_APP_UNKNOWN_CMD;
	*reply = false;
	return ret;
}
)";

    return ret;
}

std::string MsgBoxInfGenerator::getStubDispatchMessageCaseTpl(bool fireAndForget)
{
    std::string ret;
    if (m_bNotUseSwitchCase)
        ret = "if (des->header.cmd == CMD_METHOD_$UPPER_NAME) {\n\t";
    else
        ret = "case CMD_METHOD_$UPPER_NAME:\n\t";
    if (m_bSeparateReply)
    {
        if (fireAndForget)
        {
            ret +=
R"(ret = call_$NAME(des);
    *reply = false;
    return ret;
)";
        }
        else
        {
            ret +=
R"(ret = call_$NAME(des);
    if (ret >= 0)
        *reply = false;
    else
        *reply = true;
    return ret;
)";
        }
    }
    else
    {
        ret +=
R"(ret = call_$NAME(des, ser);
    *reply = true;
    return ret;
)";
    }
    if (m_bNotUseSwitchCase)
        ret += "}\n";

    return ret;
}

std::string MsgBoxInfGenerator::getStubMethodCallFuncTpl()
{
    std::string ret;
    if (m_bSeparateReply)
    {
        ret = R"(
static int32_t call_$NAME(serdes_t *des)
{
    $DES_VARS
	com_server_data_t *data = s_data;
    uintptr_t pcon = 0;
    $INIT_VARS
    if (!des || !data || !s_ext || !s_ext->$NAME_ptr)
        return -ERR_APP_PARAM;
    $DESERIALIZE
	data->info.uuid = ipc_msg_get_uuid(des->header);
    data->info.timestamp = des->recv_end_time;
    pcon = (uintptr_t)&des->header;
    (*s_ext->$NAME_ptr)($ARGS);

    return RESULT_SUCCESS;
}
)";
    }
    else
    {
        ret = R"(
static int32_t call_$NAME(serdes_t *des, serdes_t *ser)
{
    $DES_VARS
	com_server_data_t *data = s_data;
    $INIT_VARS
    if (!des || !data || !s_ext || !s_ext->$NAME_ptr)
        return -ERR_APP_PARAM;
    $DESERIALIZE
    $INIT
	data->info.uuid = ipc_msg_get_uuid(des->header);
    (*s_ext->$NAME_ptr)($ARGS);

    $SERIALIZE
    if (ret >= 0)
        return RESULT_SUCCESS;
    else
        return -ERR_APP_SERDES;
}
)";
    }

    return ret;
}

std::string MsgBoxInfGenerator::getStubMethodReplyFuncTpl()
{
    std::string ret = R"(
static $REPLY_FUNC
{
	int32_t ret = 0;
#ifdef IPC_RTE_BAREMETAL
	serdes_t *ser = NULL;
#else
	serdes_t serdes = { 0 };
	serdes_t *ser = &serdes;
#endif
	com_server_data_t *data = s_data;
    uintptr_t pcon = (uintptr_t)&context;

	if (!data)
		return -ERR_APP_PARAM;
#ifdef IPC_RTE_BAREMETAL
	ser = &data->serializer;
#endif
	ret = ipc_ser_init(ser);
    $SERIALIZE
	if (ret < 0) {
        int32_t _err = -1;
        (void)ipc_ser_init(ser);
        ret = ipc_ser_put_32(ser, (uint32_t *)&_err);
        IPC_LOG_ERR("serialization failed.\n");
	}

	if (ret >= 0) {
		ser->header = *(rw_msg_header_t *)pcon;
		ser->header.cid = ser->header.pid;
		ser->header.pid = data->pid;
		ser->header.typ = MSGBX_MSG_TYPE_REPLY;
		ret = ipc_ser_finish(ser);
	}

	if (ret >= 0)
		ret = ipc_trans_layer_stub_send_reply_msg(data->pid, data->handle, ser);

	if (ret < 0) {
		IPC_LOG_ERR("send reply fail %d.\n", ret);
		return ret;
	}

	return RESULT_SUCCESS;
}
)";

    return ret;
}

std::string MsgBoxInfGenerator::getStubCmdRegStr(const std::string &name)
{
    std::string ret = R"(ret = ipc_trans_layer_register_method(data->pid, data->handle, CMD_METHOD_$UPPER_NAME);
    if (ret < 0)
        return -1;
    )";
    replace_all(ret, "$UPPER_NAME", toUpper(name));
    return ret;
}

std::string MsgBoxInfGenerator::getProxyMethodAsyncArgs(const std::shared_ptr<FMethod> &method)
{
    if (!method)
        return "";

    std::list<std::string> arg_list;
    for (const auto &a : method->getInArgs())
    {
        if (isFixedDerived(a->getType()))
            arg_list.emplace_back("const " + Transformer::MsgBoxGen::getTypeName(a->getType()) + " *" + a->getName());
        else
            arg_list.emplace_back("const " + Transformer::MsgBoxGen::getTypeName(a->getType()) + " " + a->getName());
    }
    if (!method->isFireAndForget())
    {
        arg_list.emplace_back(m_infName + "_" + method->getName() + "_callback_t cb");
        arg_list.emplace_back("void *ext");
        arg_list.emplace_back("des_buf_t *ext_buf");
    }
    auto ret = join(arg_list, ",\n\t\t\t\t");
    replace_all(ret, " * ", " *");
    return ret;
}

std::string MsgBoxInfGenerator::getProxyMethodCallbackArgs(const std::shared_ptr<FMethod> &method)
{
    if (!method)
        return "";

    std::list<std::string> arg_list;

    for (const auto &a : method->getOutArgs())
    {
        if (isFixedDerived(a->getType()))
            arg_list.emplace_back("const " + Transformer::MsgBoxGen::getTypeName(a->getType()) + " *" + a->getName());
        else
            arg_list.emplace_back("const " + Transformer::MsgBoxGen::getTypeName(a->getType()) + " " + a->getName());
    }
    if (auto ptr = method->getErrorType())
    {
        arg_list.emplace_back("const " + Transformer::MsgBoxGen::getTypeName(ptr) + " err");
    }
    arg_list.emplace_back("void *ext");
    arg_list.emplace_back("const ext_info_t *info");
    auto ret = join(arg_list, ",\n\t\t\t\t");
    replace_all(ret, " * ", " *");
    return ret;
}

std::string MsgBoxInfGenerator::getProxyMethodSyncArgs(const std::shared_ptr<FMethod> &method)
{
    if (!method)
        return "";

    std::list<std::string> arg_list;
    for (const auto &a : method->getInArgs())
    {
        if (isFixedDerived(a->getType()))
            arg_list.emplace_back("const " + Transformer::MsgBoxGen::getTypeName(a->getType()) + " *" + a->getName());
        else
            arg_list.emplace_back("const " + Transformer::MsgBoxGen::getTypeName(a->getType()) + " " + a->getName());
    }
    for (const auto &a : method->getOutArgs())
    {
        if (isFixedDerived(a->getType()))
            arg_list.emplace_back(Transformer::MsgBoxGen::getTypeName(a->getType()) + " **" + a->getName());
        else
            arg_list.emplace_back(Transformer::MsgBoxGen::getTypeName(a->getType()) + " *" + a->getName());
    }
    if (auto ptr = method->getErrorType())
    {
        arg_list.emplace_back(Transformer::MsgBoxGen::getTypeName(ptr) + " *err");
    }
    arg_list.emplace_back("int64_t timeout_ms");
    arg_list.emplace_back("des_buf_t *ext_buf");

    auto ret = join(arg_list, ",\n\t\t\t\t");
    replace_all(ret, " * ", " *");
    return ret;
}

std::string MsgBoxInfGenerator::getProxyMethodAsyncSignature(const std::shared_ptr<FMethod> &method)
{
    if (!method)
        return "";

    std::string ret;
    if (method->isFireAndForget())
        ret = R"(
/**
 * Fire and forget call to the no_reply_method.
 * This is one way method call. The server will NOT return.
 *
$ARG_COMMENT
 * @return 0 if success, negative if fail.
 * @note This is unreliable transmission, be used ONLY if message losing is accepted.
 */
int32_t (*$NAME_fire_and_forget)(
                $ARGS
                );
)";
    else
        ret = R"(
/**
 * Asynchronously call the $NAME method.
 *
$ARG_COMMENT
 * @param cb The callback function to be called when the method returns.
 * @param ext The user-defined data passed to the method.
 * @param ext_buf The buffer to store the user-defined data.
 * @return 0 if success, negative if fail.
 */
int32_t (*$NAME_async)(
                $ARGS
                );
)";

    std::list<std::string> comments;
    for (const auto &a : method->getInArgs())
    {
        std::string commentTpl = " * @param $ARG_NAME The input argument of method $NAME.";
        comments.emplace_back(replace_all(commentTpl, "$ARG_NAME", a->getName()));
    }

    replace_all(ret, "$ARG_COMMENT", join(comments, "\n"));
    replace_all(ret, "$ARGS", getProxyMethodAsyncArgs(method));
    replace_all(ret, "$NAME", method->getName());
    return ret;
}

std::string MsgBoxInfGenerator::getProxyMethodSyncSignature(const std::shared_ptr<FMethod> &method)
{
    if (!method || method->isFireAndForget())
        return "";

    std::string ret = R"(
#ifndef IPC_RTE_BAREMETAL
/**
 * Synchronously call the hello method.
 *
$IN_ARG_COMMENT
$OUT_ARG_COMMENT
 * @param err The error code returned by the method.
 * @param timeout_ms The timeout for the method call in milliseconds, less or equal to 0 means wait forever.
 * @param ext_buf The buffer to store the user-defined data.
 * @return 0 if success, negative if fail.
 */
int32_t (*$NAME_sync)(
                $ARGS
                );
#endif
)";
    std::list<std::string> comments;
    for (const auto &a : method->getInArgs())
    {
        std::string commentTpl = " * @param $ARG_NAME The input argument of method $NAME.";
        comments.emplace_back(replace_all(commentTpl, "$ARG_NAME", a->getName()));
    }
    replace_all(ret, "$IN_ARG_COMMENT", join(comments, "\n"));

    comments.clear();
    for (const auto &a : method->getOutArgs())
    {
        std::string commentTpl = " * @param $ARG_NAME The output argument of method $NAME.";
        comments.emplace_back(replace_all(commentTpl, "$ARG_NAME", a->getName()));
    }
    replace_all(ret, "$OUT_ARG_COMMENT", join(comments, "\n"));
    replace_all(ret, "$ARGS", getProxyMethodSyncArgs(method));
    replace_all(ret, "$NAME", method->getName());
    return ret;
}

std::string MsgBoxInfGenerator::getProxyMethodOutStruct(const std::shared_ptr<FMethod> &method)
{
    if (!method || method->isFireAndForget())
        return "";

    std::string ret = R"(
struct _$NAME_out_t {
    $OUT_TYPES
};
#define $NAME_out_t struct _$NAME_out_t
)";
    std::string outTypes;
    for (const auto &a : method->getOutArgs())
    {
        if (isFixedDerived(a->getType()))
            outTypes.append(Transformer::MsgBoxGen::getTypeName(a->getType()) + " **" + a->getName() + ";\n\t");
        else
            outTypes.append(Transformer::MsgBoxGen::getTypeName(a->getType()) + " *" + a->getName() + ";\n\t");
    }
    if (auto err = method->getErrorType())
        outTypes.append(Transformer::MsgBoxGen::getTypeName(err) + " *err;");

    replace_all(ret, "$OUT_TYPES", outTypes);
    replace_all(ret, "$NAME", method->getName());
    replace_all(ret, "* *", "**");
    return ret;
}

std::string MsgBoxInfGenerator::getProxyMethodAsyncCallbackSignature(const std::shared_ptr<FMethod> &method)
{
    if (!method || method->isFireAndForget())
        return "";

    std::string ret = R"(
/**
 * Callback function for $NAME_async method.
 *
$ARG_COMMENT
 * @param err The error code returned by the method.
 * @param ext The user-defined data passed to the method.
 * @param info The extended information, containing uuid and timestamp.
 * @note all the data are stored in ext_buf passed to async call.
 * If ext_buf is NULL, internal buffer will be used.
 * Please note that, the internal buffer is shared by all callbacks.
 * The data MAY CHANGED after leaving the callback function.
 */
typedef void (*$FULLNAME_callback_t)(
                $ARGS
                );
)";
    std::list<std::string> comments;
    for (const auto &a : method->getOutArgs())
    {
        std::string commentTpl = " * @param $ARG_NAME The output argument returned by $NAME_async.";
        comments.emplace_back(replace_all(commentTpl, "$ARG_NAME", a->getName()));
    }
    replace_all(ret, "$ARG_COMMENT", join(comments, "\n"));
    replace_all(ret, "$ARGS", getProxyMethodCallbackArgs(method));
    replace_all(ret, "$FULLNAME", m_infName + "_" + method->getName());
    replace_all(ret, "$NAME", method->getName());
    return ret;
}

std::string MsgBoxInfGenerator::getProxyMethodSyncCallbackImpl(const std::shared_ptr<FMethod> &method)
{
    if (!method || method->isFireAndForget())
        return "";

    std::string ret = R"(
static void $NAME_sync_callback(
                $ARGS
                )
{
	$NAME_out_t *out = ($NAME_out_t *)ext;

	if (!out)
		return;
    $OUT_ASGN
}
)";
    std::string outasgn;
    for (const auto &a : method->getOutArgs())
        outasgn.append(BstIdl::Transformer::MsgBoxGen::outAsgn(a));
    if (auto err = method->getErrorType())
        outasgn.append("*out->err = err;");
    replace_all(outasgn, "\n", "\n\t");
    replace_all(ret, "$OUT_ASGN", outasgn);
    replace_all(ret, "$ARGS", getProxyMethodCallbackArgs(method));
    replace_all(ret, "$NAME", method->getName());
    return ret;
}

std::string MsgBoxInfGenerator::getProxyMethodSerializeImpl(const std::shared_ptr<FMethod> &method)
{
    if (!method)
        return "";

    if (method->getInArgs().empty())
        return "";

    std::string ret = R"(
static inline int32_t serialize_$NAME(
                $ARGS
                )
{
    int32_t ret = 0;

    $SERIALIZE
    if (ret < 0)
        return -ERR_APP_SERDES;
    else
        return RESULT_SUCCESS;
}
)";
    std::string ser;
    std::list<std::string> ser_arg_list;
    ser_arg_list.emplace_back("serdes_t *ser");
    for (const auto &a : method->getInArgs())
    {
        if (isFixedDerived(a->getType()))
            ser_arg_list.emplace_back("const " + Transformer::MsgBoxGen::getTypeName(a->getType()) + " *" + a->getName());
        else
            ser_arg_list.emplace_back("const " + Transformer::MsgBoxGen::getTypeName(a->getType()) + " " + a->getName());
        ser.append(BstIdl::Transformer::MsgBoxGen::serialize(a));
    }
    auto args = join(ser_arg_list, ",\n\t\t\t\t");
    replace_all(args, " * ", " *");
    replace_all(ser, "\n", "\n\t");
    replace_all(ret, "$ARGS", args);
    replace_all(ret, "$SERIALIZE", ser);
    replace_all(ret, "$NAME", method->getName());
    replace_all(ret, "$UPPER_NAME", toUpper(method->getName()));

    return ret;
}

std::string MsgBoxInfGenerator::getProxyMethodSyncImpl(const std::shared_ptr<FMethod> &method)
{
    if (!method)
        return "";

    std::string ret = R"(
static int32_t call_$NAME_sync($ARGS)
{
    int32_t ret = 0;
    serdes_t serdes = { 0 };
    serdes_t *ser = NULL;
    $NAME_out_t out = {$OUT_INIT};
    callback_registration_t *reg = NULL;
	com_client_data_t *data = s_data;

    if (!data || !s_ext)
        return -ERR_APP_PARAM;
    ser = &serdes;
    (void)ipc_ser_init(ser);
    $SERIALIZE
	// send request
	ret = send_request(data, ser, s_ext->cid, CMD_METHOD_$UPPER_NAME,
                $NAME_sync_callback, &out, ext_buf);
    if (ret < 0 || ret >= IPC_TOKEN_NUM) {
        IPC_LOG_ERR("send method fail %d.\n", ret);
        return ret;
    }

    //wait for reply
    reg = &data->method_registry[ret];
    reg->disable_gc = true;
    if (timeout_ms <= 0)
        ret = wait_on_registry(reg);
    else
        ret = timedwait_on_registry(reg, timeout_ms);
    if (ret < 0) {
        clear_registry(reg);
        IPC_LOG_ERR("wait timeout\n");
    }

    return ret;
}
)";
    if (method->getInArgs().empty())
    {
        replace_all(ret, "$SERIALIZE", "");
    }
    else
    {
        std::string serTpl = R"(
    ret = $SERIALIZE_CALL;
	if (ret != 0) {
		IPC_LOG_ERR("serialize fail.\n");
		return -ERR_APP_SERDES;
	}
)";
        replace_all(ret, "$SERIALIZE", serTpl);
    }
    std::string ser;
    std::list<std::string> out_init_arg_list;
    for (const auto &a : method->getOutArgs())
    {
        std::string name = a->getName();
        out_init_arg_list.emplace_back("." + name + " = " + name);
    }
    if (method->getErrorType())
        out_init_arg_list.emplace_back(".err = err");
    replace_all(ret, "$ARGS", getProxyMethodSyncArgs(method));
    replace_all(ret, "$SERIALIZE_CALL", getProxyMethodSerializeCallStr(method));
    replace_all(ret, "$OUT_INIT", join(out_init_arg_list, ",\n\t\t\t\t"));
    replace_all(ret, "$NAME", method->getName());
    replace_all(ret, "$UPPER_NAME", toUpper(method->getName()));
    return ret;
}

std::string MsgBoxInfGenerator::getProxyMethodAsyncImpl(const std::shared_ptr<FMethod> &method)
{
    if (!method || method->isFireAndForget())
        return "";

    std::string ret = R"(
static int32_t call_$NAME_async($ARGS)
{
	int32_t ret = 0;
#ifdef IPC_RTE_BAREMETAL
	serdes_t *ser = NULL;
#else
	serdes_t serdes = { 0 };
	serdes_t *ser = &serdes;
#endif
	com_client_data_t *data = s_data;

	if (!data || !s_ext)
		return -ERR_APP_PARAM;
#ifdef IPC_RTE_BAREMETAL
	ser = &data->serializer;
#endif
	(void)ipc_ser_init(ser);
    $SERIALIZE
	// send request
	ret = send_request(data, ser, s_ext->cid, CMD_METHOD_$UPPER_NAME,
                cb, ext, ext_buf);
    if (ret < 0 || ret >= IPC_TOKEN_NUM) {
        IPC_LOG_ERR("send method fail %d.\n", ret);
        return ret;
    }

    return RESULT_SUCCESS;
}
)";
    if (method->getInArgs().empty())
    {
        replace_all(ret, "$SERIALIZE", "");
    }
    else
    {
        std::string serTpl = R"(
    ret = $SERIALIZE_CALL;
	if (ret != 0) {
		IPC_LOG_ERR("serialize fail.\n");
		return -ERR_APP_SERDES;
	}
)";
        replace_all(ret, "$SERIALIZE", serTpl);
    }
    replace_all(ret, "$ARGS", getProxyMethodAsyncArgs(method));
    replace_all(ret, "$SERIALIZE_CALL", getProxyMethodSerializeCallStr(method));
    replace_all(ret, "$NAME", method->getName());
    replace_all(ret, "$UPPER_NAME", toUpper(method->getName()));
    return ret;
}

std::string MsgBoxInfGenerator::getProxyMethodFireAndForgetfImpl(const std::shared_ptr<FMethod> &method)
{
    if (!method || !method->isFireAndForget())
        return "";

    std::string ret = R"(
static int32_t call_$NAME_fire_and_forget($ARGS)
{
	int32_t ret = 0;
#ifdef IPC_RTE_BAREMETAL
	serdes_t *ser = NULL;
#else
	serdes_t serdes = { 0 };
	serdes_t *ser = &serdes;
#endif
	com_client_data_t *data = s_data;

	if (!data || !s_ext)
		return -ERR_APP_PARAM;
#ifdef IPC_RTE_BAREMETAL
	ser = &data->serializer;
#endif
	(void)ipc_ser_init(ser);

    ret = $SERIALIZE_CALL;
	if (ret != 0) {
		IPC_LOG_ERR("serialize fail.\n");
		return -ERR_APP_SERDES;
	}

	// send request
	ret = send_fire_and_forget_request(data, ser, s_ext->cid, CMD_METHOD_$UPPER_NAME);
    if (ret < 0) {
        IPC_LOG_ERR("send method fail %d.\n", ret);
        return ret;
    }

    return RESULT_SUCCESS;
}
)";

    replace_all(ret, "$ARGS", getProxyMethodAsyncArgs(method));
    replace_all(ret, "$SERIALIZE_CALL", getProxyMethodSerializeCallStr(method));
    replace_all(ret, "$NAME", method->getName());
    replace_all(ret, "$UPPER_NAME", toUpper(method->getName()));
    return ret;
}

std::string MsgBoxInfGenerator::getProxyMethodSerializeCallStr(const std::shared_ptr<FMethod> &method)
{
    if (!method)
        return "";

    std::string ret = "serialize_$NAME($SER_CALL_ARGS)";
    std::list<std::string> ser_call_arg_list;
    ser_call_arg_list.emplace_back("ser");

    for (const auto &a : method->getInArgs())
        ser_call_arg_list.emplace_back(a->getName());
    replace_all(ret, "$SER_CALL_ARGS", join(ser_call_arg_list, ", "));
    replace_all(ret, "$NAME", method->getName());
    return ret;
}

std::string MsgBoxInfGenerator::getProxyMethodInvokeCallbackImpl(const std::shared_ptr<FMethod> &method)
{
    if (!method || method->isFireAndForget())
        return "";

    std::string ret = R"(
static inline int32_t call_$NAME_callback(serdes_t *des)
{
    int32_t ret = 0;
    callback_registration_t *reg = NULL;
    des_buf_t *buf = NULL;
	uint32_t len = 0;
	com_client_data_t *data = s_data;
    $FULLNAME_callback_t cb = NULL;
    $INIT_VARS

    if (!des || !data)
        return -ERR_APP_PARAM;

    reg = &data->method_registry[des->header.tok];
    if (!reg->busy || reg->cmd != CMD_METHOD_$UPPER_NAME) {
        IPC_LOG_ERR("callback registry is invalid.\n");
        return -ERR_APP_TOK;
    }
    buf = reg->ext_buf ? reg->ext_buf : &data->des_buf;
    clear_des_buf(buf);
    data->info.uuid = ipc_msg_get_uuid(des->header);
    data->info.timestamp = des->recv_end_time;

    // deserialize arguments
	len = ipc_des_get_all(des, (uint8_t *)buf->data_buf);
	if (len <= 0)
		return -ERR_APP_SERDES;
	buf->unavail_data_size = IPC_MAX_DATA_SIZE - len;

    $ERR_DES
    if (ret < 0)
        return -ERR_APP_SERDES;
    if (err == $INF_UPPER_NAME_NO_ERROR) {
        $DESERIALIZE
        if (ret < 0)
            return -ERR_APP_SERDES;
    }

    // call callback function
    cb = ($FULLNAME_callback_t)(reg->cb);
    if (cb)
        cb($CB_CALL_ARGS);
#ifndef IPC_RTE_BAREMETAL
    notify_callback_registry(reg);
#endif
    clear_registry(reg);

    return RESULT_SUCCESS;
}
)";

    std::string des;
    std::string initvars;
    std::list<std::string> cb_call_arg_list;
    for (const auto &a : method->getOutArgs())
    {
        // if (
        //     (std::dynamic_pointer_cast<FStructType>(a->getType()->getDerived())
        //     && BstIdl::Transformer::MsgBoxGen::isFixed(a) == "true"))
        //     cb_call_arg_list.emplace_back("*" + a->getName());
        // else
            cb_call_arg_list.emplace_back(a->getName());
        des.append(BstIdl::Transformer::MsgBoxGen::deserialize(a));
        initvars.append(BstIdl::Transformer::MsgBoxGen::initVar(a));
    }
    if (auto err = method->getErrorType())
    {
        auto errArg = method->getErrorArg();
        auto errdes = BstIdl::Transformer::MsgBoxGen::deserialize(errArg);
        replace_all(errdes, "\n", "\n\t");
        replace_all(ret, "$ERR_DES", errdes);
        cb_call_arg_list.emplace_back("err");
        initvars.append(BstIdl::Transformer::MsgBoxGen::initVar(errArg));
    }
    cb_call_arg_list.emplace_back("reg->ext");
    cb_call_arg_list.emplace_back("&data->info");
    trim(des);
    replace_all(des, "\n", "\n\t\t");
    replace_all(initvars, "\n", "\n\t");
    replace_all(ret, "$DESERIALIZE", des);
    replace_all(ret, "$INIT_VARS", initvars);
    replace_all(ret, "$CB_CALL_ARGS", join(cb_call_arg_list, ", "));
    replace_all(ret, "$NAME", method->getName());
    replace_all(ret, "$UPPER_NAME", toUpper(method->getName()));
    replace_all(ret, "$FULLNAME", m_infName + "_" + method->getName());

    return ret;
}

std::string MsgBoxInfGenerator::getProxyMethodCase(const std::shared_ptr<FMethod> &method)
{
    if (!method || method->isFireAndForget())
        return "";

    std::string ret;
    if (m_bNotUseSwitchCase)
        ret = R"(if (des->header.cmd == CMD_METHOD_$UPPER_NAME) {
    ret = call_$NAME_callback(des);
    return ret;
}
)";
    else
        ret = R"(case CMD_METHOD_$UPPER_NAME:
    ret = call_$NAME_callback(des);
    break;
)";
    replace_all(ret, "$NAME", method->getName());
    replace_all(ret, "$UPPER_NAME", toUpper(method->getName()));
    replace_all(ret, "\n", "\n\t");
    return ret;
}

std::string MsgBoxInfGenerator::getProxyMethodSyncPtrInit(const std::shared_ptr<FMethod> &method)
{
    if (!method || method->isFireAndForget())
        return "";

    std::string ret = R"(#ifndef IPC_RTE_BAREMETAL
    client->$NAME_sync = call_$NAME_sync;
#endif
)";
    replace_all(ret, "$NAME", method->getName());
    return ret;
}

std::string MsgBoxInfGenerator::getProxyMethodSyncPtrDestroy(const std::shared_ptr<FMethod> &method)
{
    if (!method)
        return "";

    std::string ret = R"(s_data->client.$NAME_sync = NULL;
    )";
    replace_all(ret, "$NAME", method->getName());
    return ret;
}

std::string MsgBoxInfGenerator::getProxyMethodAsyncPtrInit(const std::shared_ptr<FMethod> &method)
{
    if (!method)
        return "";

    std::string ret;
    if (method->isFireAndForget())
        ret = "\tclient->$NAME_fire_and_forget = call_$NAME_fire_and_forget;\n";
    else
        ret = "\tclient->$NAME_async = call_$NAME_async;\n";
    replace_all(ret, "$NAME", method->getName());
    return ret;
}

std::string MsgBoxInfGenerator::getProxyMethodAsyncPtrDestroy(const std::shared_ptr<FMethod> &method)
{
    if (!method)
        return "";

    std::string ret = R"(s_data->client.$NAME_async = NULL;
    )";
    replace_all(ret, "$NAME", method->getName());
    return ret;
}

std::string MsgBoxInfGenerator::getProxyBroadcastCallbackSignature(const std::shared_ptr<FBroadcast> &broadcast)
{
    if (!broadcast)
        return "";

    std::string ret = R"(/**
 * Callback function for broadcast $NAME.
 *
$ARG_COMMENT
 * @param ext The user-defined data passed to the method.
 * @param info The extended information, containing uuid and timestamp.
 * @note all the data are stored in ext_buf passed to async call.
 * If ext_buf is NULL, internal buffer will be used.
 * Please note that, the internal buffer is shared by all callbacks.
 * The data MAY CHANGED after leaving the callback function.
 */
typedef void (*$FULLNAME_callback_t)(
                $ARGS
                );
)";
    std::string name = broadcast->getName();
    std::string fullname = m_infName + "_" + name;
    std::list<std::string> arg_list;
    std::list<std::string> comments;
    for (const auto &a : broadcast->getOutArgs())
    {
        std::string commentTpl = " * @param $ARG_NAME The output argument returned by broadcast $NAME.";
        comments.emplace_back(replace_all(commentTpl, "$ARG_NAME", a->getName()));
        if (isFixedDerived(a->getType()))
            arg_list.emplace_back("const " + Transformer::MsgBoxGen::getTypeName(a->getType()) + " *" + a->getName());
        else
            arg_list.emplace_back("const " + Transformer::MsgBoxGen::getTypeName(a->getType()) + " " + a->getName());
    }
    arg_list.emplace_back("void *ext");
    arg_list.emplace_back("const ext_info_t *info");
    auto args = join(arg_list, ",\n\t\t\t\t");
    replace_all(args, " * ", " *");
    replace_all(ret, "$ARG_COMMENT", join(comments, "\n"));
    replace_all(ret, "$ARGS", args);
    replace_all(ret, "$FULLNAME", fullname);
    replace_all(ret, "$NAME", name);
    return ret;
}

std::string MsgBoxInfGenerator::getProxyBroadcastRegisterSignature(const std::shared_ptr<FBroadcast> &broadcast)
{
    if (!broadcast)
        return "";

    std::string ret = R"(/**
 * Subscribe to the $NAME broadcast.
 *
 * @param cb The callback function called when the broadcast received.
 * @param ext The user-defined data passed to the broadcast callback.
 * @param ext_buf The buffer to store the user-defined data.
 * @param cb2 The callback function called when the subscription is complete.
 * @param ext2 The user-defined data passed to the subscription callback.
 * @note all the data are stored in ext_buf passed to async call.
 * If ext_buf is NULL, internal buffer will be used.
 * Please note that, the internal buffer is shared by all callbacks.
 * The data MAY CHANGED after leaving the callback function.
 * @return 0 if success, negative if fail.
 */
int32_t (*$NAME_sub)(
                $FULLNAME_callback_t cb,
                void *ext,
                des_buf_t *ext_buf,
                broadcast_sub_unsub_callback_t cb2,
                void *ext2
                );

/**
 * Unsubscribe from the $NAME broadcast.
 *
 * @param cb The callback function called when the unsubscription is complete.
 * @param ext The user-defined data passed to the callback.
 * @return 0 if success, negative if fail.
 */
int32_t (*$NAME_unsub)(broadcast_sub_unsub_callback_t cb, void *ext);
)";
    std::string fullname = m_infName + "_" + broadcast->getName();
    replace_all(ret, "$FULLNAME", fullname);
    replace_all(ret, "$NAME", broadcast->getName());
    return ret;
}

std::string MsgBoxInfGenerator::getProxyBroadcastVars(const std::shared_ptr<FBroadcast> &broadcast)
{
    if (!broadcast)
        return "";

    std::string ret = "static callback_registration_t s_data->$NAME_registry = { 0 };\n";
    replace_all(ret, "$NAME", broadcast->getName());
    return ret;
}

std::string MsgBoxInfGenerator::getProxyBroadcastInternalVarDecls(const std::shared_ptr<FBroadcast> &broadcast)
{
    if (!broadcast)
        return "";

    std::string ret = "callback_registration_t $NAME_registry;\n";
    replace_all(ret, "$NAME", broadcast->getName());
    return ret;
}

std::string MsgBoxInfGenerator::getProxyBroadcastSubImpl(const std::shared_ptr<FBroadcast> &broadcast)
{
    if (!broadcast)
        return "";

    std::string ret = R"(
// subscribe $NAME
static int32_t subscribe_$NAME(
                $FULLNAME_callback_t cb,
                void *ext,
                des_buf_t *ext_buf,
                broadcast_sub_unsub_callback_t cb2,
                void *ext2
                )
{
    int32_t ret = 0;
    serdes_t *ser = NULL;
    com_client_data_t *data = s_data;

	if (!data || !s_ext)
        return -ERR_APP_PARAM;

    ser = &data->serializer;

	// send request
	ret = send_request(data, ser, s_ext->cid, CMD_METHOD_SUB_$UPPER_NAME,
                cb2, ext2, NULL);
    if (ret < 0 || ret >= IPC_TOKEN_NUM) {
        IPC_LOG_ERR("send fail %d.\n", ret);
        return ret;
    }

	// set registry
	s_ext->$NAME_registry.busy = true;
	(void)add_registry(&s_ext->$NAME_registry, (void *)cb, ext, ext_buf);

    return RESULT_SUCCESS;
}
)";
    std::string fullname = m_infName + "_" + broadcast->getName();
    replace_all(ret, "$FULLNAME", fullname);
    replace_all(ret, "$NAME", broadcast->getName());
    replace_all(ret, "$UPPER_NAME", toUpper(broadcast->getName()));

    return ret;
}

std::string MsgBoxInfGenerator::getProxyBroadcastUnsubImpl(const std::shared_ptr<FBroadcast> &broadcast)
{
    if (!broadcast)
        return "";

    std::string ret = R"(
// unsubscribe $NAME
static int32_t unsubscribe_$NAME(broadcast_sub_unsub_callback_t cb, void *ext)
{
    int32_t ret = 0;
    serdes_t *ser = NULL;
    com_client_data_t *data = s_data;

    if (!data)
        return -ERR_APP_PARAM;

    ser = &data->serializer;

	// send request
	ret = send_request(data, ser, s_ext->cid, CMD_METHOD_UNSUB_$UPPER_NAME,
                cb, ext, NULL);
    if (ret < 0 || ret >= IPC_TOKEN_NUM) {
        IPC_LOG_ERR("send fail %d.\n", ret);
        return ret;
    }

	return RESULT_SUCCESS;
}
)";
    std::string fullname = m_infName + "_" + broadcast->getName();
    replace_all(ret, "$FULLNAME", fullname);
    replace_all(ret, "$NAME", broadcast->getName());
    replace_all(ret, "$UPPER_NAME", toUpper(broadcast->getName()));

    return ret;
}

std::string MsgBoxInfGenerator::getProxyBroadcastInvokeCallbackImpl(const std::shared_ptr<FBroadcast> &broadcast)
{
    if (!broadcast)
        return "";

    std::string ret = R"(
static inline int32_t call_$NAME_callback(serdes_t *des)
{
    $DES_VARS
	com_client_data_t *data = s_data;
    callback_registration_t *reg = NULL;
    $FULLNAME_callback_t cb = NULL;
    $INIT_VARS
    if (!des || !data || !s_ext)
        return -ERR_APP_PARAM;

    reg = &s_ext->$NAME_registry;
    if (!reg->busy || !reg->cb) {
        IPC_LOG_ERR("callback registry is invalid.\n");
        return RESULT_SUCCESS;
    }

    data->info.uuid = ipc_msg_get_uuid(des->header);
    data->info.timestamp = des->recv_end_time;
    $DESERIALIZE
    cb = ($FULLNAME_callback_t)(reg->cb);
    cb($ARGS);

    return RESULT_SUCCESS;
}
)";
    if (broadcast->getOutArgs().empty())
    {
        replace_all(ret, "$DESERIALIZE", "");
        replace_all(ret, "$DES_VARS\n\t", "");
    }
    else
    {
        std::string desTpl = R"(buf = reg->ext_buf ? reg->ext_buf : &data->des_buf;
    clear_des_buf(buf);
	len = ipc_des_get_all(des, (uint8_t *)buf->data_buf);
	if (len <= 0)
		return -ERR_APP_SERDES;
	buf->unavail_data_size = IPC_MAX_DATA_SIZE - len;

    $DESERIALIZE
    if (ret < 0)
        return -ERR_APP_SERDES;
)";
        replace_all(ret, "$DESERIALIZE", desTpl);

        std::string desVarTpl = R"(int32_t ret = 0;
    des_buf_t *buf = NULL;
	uint32_t len = 0;)";
        replace_all(ret, "$DES_VARS", desVarTpl);
    }
    std::string des;
    std::string initVars;
    std::list<std::string> arg_list;
    for (const auto &a : broadcast->getOutArgs())
    {
        arg_list.emplace_back(a->getName());
        initVars.append(BstIdl::Transformer::MsgBoxGen::initVar(a));
        des.append(BstIdl::Transformer::MsgBoxGen::deserialize(a));
    }
    arg_list.emplace_back("reg->ext");
    arg_list.emplace_back("&data->info");
    replace_all(des, "\n", "\n\t");
    replace_all(initVars, "\n", "\n\t");

    replace_all(ret, "$INIT_VARS", initVars);
    replace_all(ret, "$DESERIALIZE", des);
    replace_all(ret, "$ARGS", join(arg_list, ", "));
    replace_all(ret, "$FULLNAME", m_infName + "_" + broadcast->getName());
    replace_all(ret, "$NAME", broadcast->getName());

    return ret;
}

std::string MsgBoxInfGenerator::getProxyBroadcastCase(const std::shared_ptr<FBroadcast> &broadcast)
{
    if (!broadcast)
        return "";

    std::string ret;
    if (m_bNotUseSwitchCase)
        ret = R"(if (des->header.cmd == CMD_BROADCAST_$UPPER_NAME) {
    ret = call_$NAME_callback(des);
    return ret;
}
)";
    else
        ret = R"(case CMD_BROADCAST_$UPPER_NAME:
    ret = call_$NAME_callback(des);
    break;
)";
    replace_all(ret, "\n", "\n\t");
    replace_all(ret, "$NAME", broadcast->getName());
    replace_all(ret, "$UPPER_NAME", toUpper(broadcast->getName()));
    return ret;
}

std::string MsgBoxInfGenerator::getProxyBroadcastReplyCase(const std::shared_ptr<FBroadcast> &broadcast)
{
    if (!broadcast)
        return "";

    std::string ret;
    if (m_bNotUseSwitchCase)
        ret = R"(if (des->header.cmd == CMD_METHOD_SUB_$UPPER_NAME) {
    ret = call_broadcast_sub_unsub_callback(data, des);
    return ret;
}
if (des->header.cmd == CMD_METHOD_UNSUB_$UPPER_NAME) {
    ret = call_broadcast_sub_unsub_callback(data, des);
    if (ret >= 0)
        clear_registry(&s_ext->$NAME_registry);
    return ret;
}
)";
    else
        ret = R"(case CMD_METHOD_SUB_$UPPER_NAME:
    ret = call_broadcast_sub_unsub_callback(data, des);
    break;
case CMD_METHOD_UNSUB_$UPPER_NAME:
    ret = call_broadcast_sub_unsub_callback(data, des);
    if (ret >= 0)
        clear_registry(&s_ext->$NAME_registry);
    break;
)";
    std::string name = broadcast->getName();
    std::string fullname = m_infName + "_" + name;
    replace_all(ret, "$NAME", name);
    replace_all(ret, "$FULLNAME", fullname);
    replace_all(ret, "$UPPER_NAME", toUpper(name));
    replace_all(ret, "\n", "\n\t");
    return ret;
}

std::string MsgBoxInfGenerator::getProxyBroadcastPtrInit(const std::shared_ptr<FBroadcast> &broadcast)
{
    if (!broadcast)
        return "";

    std::string ret = R"(client->$NAME_sub = subscribe_$NAME;
    client->$NAME_unsub = unsubscribe_$NAME;
	(void)init_registry(&ext->$NAME_registry);
    )";
    replace_all(ret, "$NAME", broadcast->getName());
    return ret;
}

std::string MsgBoxInfGenerator::getProxyBroadcastPtrDestroy(const std::shared_ptr<FBroadcast> &broadcast)
{
    if (!broadcast)
        return "";

    std::string ret = R"(destroy_registry(&s_ext->$NAME_registry);
    )";
    replace_all(ret, "$NAME", broadcast->getName());
    return ret;
}

std::string MsgBoxInfGenerator::getProxyDispatchMessageTpl()
{
    std::string ret;
    if (m_bNotUseSwitchCase)
        ret = R"(// dispatch_broadcast
static inline int32_t dispatch_broadcast(serdes_t *des)
{
	int32_t ret = 0;

	if (!des || des->header.pid != s_ext->cid)
		return -ERR_APP_PARAM;

    $BROADCAST_CASES
    IPC_LOG_ERR("unknown broadcast message %d.\n", des->header.cmd);

	return -ERR_APP_UNKNOWN_CMD;
}

// dispatch_reply
static inline int32_t dispatch_reply(serdes_t *des)
{
	int32_t ret = 0;
	com_client_data_t *data = s_data;

	if (!des || des->header.pid != s_ext->cid)
		return -ERR_APP_PARAM;

    $METHOD_REPLY_CASES
    $BROADCAST_REPLY_CASES
    IPC_LOG_ERR("unknown reply message %d.\n", des->header.cmd);

	return -ERR_APP_UNKNOWN_CMD;
}
)";
    else
        ret = R"(// dispatch_broadcast
static inline int32_t dispatch_broadcast(serdes_t *des)
{
	int32_t ret = 0;

	if (!des || des->header.pid != s_ext->cid)
		return -ERR_APP_PARAM;

	switch (des->header.cmd) {
	$BROADCAST_CASES
	default:
        ret = -ERR_APP_UNKNOWN_CMD;
        IPC_LOG_ERR("unknown broadcast message %d.\n", des->header.cmd);
		break;
	}

	return ret;
}

// dispatch_reply
static inline int32_t dispatch_reply(serdes_t *des)
{
	int32_t ret = 0;
	com_client_data_t *data = s_data;

	if (!des || des->header.pid != s_ext->cid)
		return -ERR_APP_PARAM;

	switch (des->header.cmd) {
    $METHOD_REPLY_CASES
    $BROADCAST_REPLY_CASES
    default:
        ret = -ERR_APP_UNKNOWN_CMD;
        IPC_LOG_ERR("unknown reply message %d.\n", des->header.cmd);
		break;
	}

	return ret;
}
)";
    if (m_interface->getBroadcasts().empty())
        remove_all(ret, "\tcom_client_data_t *data = s_data;\n");
    return ret;
}

std::string MsgBoxInfGenerator::getProxyHeaderTpl()
{
    std::string ret = R"($LICENSE
$VERSION_COMMENT
#ifndef $HEADER_MACRO
#define $HEADER_MACRO

#define $RTE_DEFINE
#ifdef IPC_RTE_KERNEL
#include <bst/ipc_app_client_utils.h>
#else
#include "ipc_app_client_utils.h"
#endif
#include "$INF_NAME_datatype.h"

#ifdef __cplusplus
extern "C" {
#endif
$METHOD_TYPE
$BROADCAST_TYPE
// Interface client
struct _$CLIENT_NAME_t {
    /**
     * Get the version of the interface.
     *
     * @return The version struct, containing major and minor.
     */
    ipc_inf_version_t (*version)(void);

    /**
     * Register server availability changed callback.
     *
     * @param cb The callback function to be registered.
     * @return 0 if success, negative if fail.
     */
    int32_t (*register_avail_changed)(avail_changed_callback_t cb,
                    void *ext);
    $METHODS
    $BROADCASTS
	/**
	 * Dispatch broadcast messages.
	 *
	 * @param des The received message package.
	 * @return 0 if success, negative if fail.
	 */
	int32_t (*dispatch_broadcast)(serdes_t *des);

	/**
	 * Dispatch reply messages.
	 *
	 * @param des The received message package.
	 * @return 0 if success, negative if fail.
	 */
	int32_t (*dispatch_reply)(serdes_t *des);
};
#define $CLIENT_NAME_t struct _$CLIENT_NAME_t

/**
 *  The extend data used by the client.
 */
struct _$CLIENT_NAME_ext_t {
	uint8_t cid;
	uint8_t res[7];
    $BROADCAST_VAR_DECLS
};
#define $CLIENT_NAME_ext_t struct _$CLIENT_NAME_ext_t

/**
 * Initializes the client.
 *
 * @param data The data for com_client_data_t
 * @param client The data for $CLIENT_NAME_t
 * @param ext The data for $CLIENT_NAME_ext_t
 * @return 0 if success, negative if fail.
 */
int32_t $CLIENT_NAME_init(com_client_data_t *data,
            $CLIENT_NAME_t *client,
            $CLIENT_NAME_ext_t *ext);

/**
 * Destroys the client.
 */
void $CLIENT_NAME_destroy(void);

#ifdef __cplusplus
}
#endif

#endif // $HEADER_MACRO
)";
    return ret;
}

std::string MsgBoxInfGenerator::getProxySourceTpl()
{
    std::string ret = R"($LICENSE
$VERSION_COMMENT
#include "$CLIENT_NAME.h"

// macro definitions
#define CID $CID_VALUE
$VERSION

$METHOD_ID_DEF
$BROADCAST_ID_DEF
// local variables
static com_client_data_t *s_data = NULL;
static $CLIENT_NAME_ext_t *s_ext = NULL;

#ifndef IPC_RTE_BAREMETAL
$METHOD_SYNC_OUT
#endif
// interface implementation
// get interface version
static ipc_inf_version_t get_ipc_inf_version(void)
{
	ipc_inf_version_t ret = { .major = MAJOR, .minor = MINOR };

	return ret;
}

// method
$METHOD_DEFS
// broadcast
$BROADCAST_DEFS
$DISPATCH_MESSAGE
// register availablity changed callback function
static int32_t register_avail_changed_cb(avail_changed_callback_t cb, void *ext)
{
    return reg_avail_changed_cb(s_data, cb, ext);
}

// initialize client
int32_t $CLIENT_NAME_init(com_client_data_t *data, $CLIENT_NAME_t *client,
            $CLIENT_NAME_ext_t *ext)
{
	if (!data || !client || !ext)
		return -1;

	s_data = data;
	s_ext = ext;

    // set client
    client->version = get_ipc_inf_version;
    client->register_avail_changed = register_avail_changed_cb;
$METHOD_REG_INIT
    $BROADCAST_REG_INIT
	client->dispatch_broadcast = dispatch_broadcast;
	client->dispatch_reply = dispatch_reply;

	// set ext
	if (ext->cid == 0)
		ext->cid = CID;

    return 0;
}
// destroy client
void $CLIENT_NAME_destroy(void)
{
    $BROADCAST_DESTROY
    s_data = NULL;
    s_ext = NULL;
}
)";
    return ret;
}

std::string MsgBoxInfGenerator::getStubHeaderTpl()
{
    std::string ret = R"($LICENSE
$VERSION_COMMENT
#ifndef $HEADER_MACRO
#define $HEADER_MACRO

#define $RTE_DEFINE
#ifdef IPC_RTE_KERNEL
#include <bst/ipc_app_svr_utils.h>
#else
#include "ipc_app_svr_utils.h"
#endif
#include "$INF_NAME_datatype.h"

#ifdef __cplusplus
extern "C" {
#endif

$METHOD_TYPE
// Interface server
struct _$SERVER_NAME_t {
    /**
     * Get the version of the interface.
     *
     * @return The version struct, containing major and minor.
     */
    ipc_inf_version_t (*version)(void);
    $METHOD_REGS
    $BROADCASTS
	/**
	 * Dispatch server messages.
	 *
	 * @return 0 if success, negative if fail.
	 */
	int32_t (*dispatch_request)(serdes_t *des, bool *reply);
};
#define $SERVER_NAME_t struct _$SERVER_NAME_t

/**
 *  The extend data used by the server.
 */
struct _$SERVER_NAME_ext_t {
    $METHOD_VAR_DECLS
    $BROADCAST_VAR_DECLS
};
#define $SERVER_NAME_ext_t struct _$SERVER_NAME_ext_t

/**
 * Initializes the server.
 *
 * @param data The data for com_server_data_t
 * @param server The data for test_server_t
 * @param ext The data for test_server_ext_t
 * @return 0 if success, negative if fail.
 */
int32_t $SERVER_NAME_init(com_server_data_t *data,
            $SERVER_NAME_t *server,
            $SERVER_NAME_ext_t *ext);

/**
 * Destroys the test client.
 */
void $SERVER_NAME_destroy(void);

#ifdef __cplusplus
}
#endif

#endif // $HEADER_MACRO
)";
    return ret;
}

std::string MsgBoxInfGenerator::getStubSourceTpl()
{
    std::string ret = R"($LICENSE
$VERSION_COMMENT
#include "$SERVER_NAME.h"
#ifdef IPC_RTE_KERNEL
#include <bst/ipc_trans_common.h>
#include <bst/ipc_trans_layer.h>
#else
#include "ipc_trans_common.h"
#include "ipc_trans_layer.h"
#endif

// macro definitions
$VERSION
#define MAX_METHOD_NUM $MAX_METHOD_NUM
#define MAX_BROADCAST_NUM $MAX_BROADCAST_NUM

$METHOD_ID_DEF
$BROADCAST_ID_DEF
// local variables
static com_server_data_t *s_data = NULL;
static $SERVER_NAME_ext_t *s_ext = NULL;

// interface implementation
// get interface version
static ipc_inf_version_t get_ipc_inf_version(void)
{
	ipc_inf_version_t ret = { .major = MAJOR, .minor = MINOR };

	return ret;
}

// method
$METHOD_DEF
// broadcast
$BROADCAST_DEF
$DISPATCH_MESSAGE
// initialize server
int32_t $SERVER_NAME_init(com_server_data_t *data, $SERVER_NAME_t *server,
            $SERVER_NAME_ext_t *ext)
{
    int32_t ret = 0;

	if (!data || !server || !ext)
		return -1;

	s_data = data;
	s_ext = ext;

    // register CMDs.
    $CMD_REG
    // set server    
    server->version = get_ipc_inf_version;
    $METHOD_REG_INIT
    $BROADCAST_REG_INIT
    server->dispatch_request = dispatch_request;

    return 0;
}

// destroy client
void $SERVER_NAME_destroy(void)
{
	s_data = NULL;
	s_ext = NULL;
}
)";
    return ret;
}

std::string MsgBoxInfGenerator::getVersionComment()
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

bool MsgBoxInfGenerator::isFixedDerived(const std::shared_ptr<FTypeRef> &type)
{
    if (!type)
        return false;
    auto t = type->getDerived();
    if (!t)
        return false;
    // bypass union
    auto u = std::dynamic_pointer_cast<FUnionType>(t);
    if (u)
        return false;
    // bypass enumeration
    auto e = std::dynamic_pointer_cast<FEnumerationType>(t);
    if (e)
        return false;
    return BstIdl::Transformer::MsgBoxGen::isFixed(type) == "true";
}

} // namespace BstIdl
