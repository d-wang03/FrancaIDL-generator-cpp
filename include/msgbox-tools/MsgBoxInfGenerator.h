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
#ifndef MSG_BOX_INTERFACE_GENERATOR_H
#define MSG_BOX_INTERFACE_GENERATOR_H

#include "generator/AbstractGenerator.h"
#include "FDModel/FDExtensionRoot.h"
#include "FDModel/FDInterface.h"

namespace BstIdl
{

class MsgBoxInfGenerator : public AbstractGenerator
{
public:
    MsgBoxInfGenerator(const std::string &dest_dir, const std::string &src_id, const std::shared_ptr<FDExtensionElement> &instance);
    ~MsgBoxInfGenerator() override = default;

    bool generate() override;

    void setRteStr(std::string rte) { m_rtestr = rte; }
    std::string getInfName()const { return m_infName; }

protected:
    virtual bool validate(bool checkDstEndID = false);
    virtual bool generateHeader() = 0;
    virtual bool generateSource() = 0;
    virtual bool generateTypeHeader();
    bool getIntegerProperty(const std::shared_ptr<FDElement> &element, const std::string &property, int32_t *value);
    bool getBooleanProperty(const std::shared_ptr<FDElement> &element, const std::string &property, bool *value);
    std::shared_ptr<FArrayType> convertImplicitArray(const std::shared_ptr<FTypedElement> &element);

    std::string getVersionString();
    std::string getStubMethodSignature(const std::shared_ptr<FMethod> &method);
    std::string getStubMethodRegisterSignature(const std::shared_ptr<FMethod> &method);
    std::string getStubMethodRegisterImpl(const std::shared_ptr<FMethod> &method);
    std::string getStubMethodReplySignature(const std::shared_ptr<FMethod> &method);
    std::string getStubMethodPtrInit(const std::shared_ptr<FMethod> &method);
    std::string getStubMethodPtrDestroy(const std::shared_ptr<FMethod> &method);
    std::string getStubMethodCallFunc(const std::shared_ptr<FMethod> &method);
    std::string getStubMethodCase(const std::shared_ptr<FMethod> &method);
    std::string getStubMethodVars(const std::shared_ptr<FMethod> &method);
    std::string getStubMethodInternalVarDecls(const std::shared_ptr<FMethod> &method);
    std::string getMethodCmds(const std::shared_ptr<FDMethod> &method);    
    std::string getStubBroadcastSignature(const std::shared_ptr<FBroadcast> &broadcast);
    std::string getStubBroadcastCallFunc(const std::shared_ptr<FBroadcast> &broadcast);
    std::string getStubBroadcastCase(const std::shared_ptr<FBroadcast> &broadcast);
    std::string getStubBroadcastPtrInit(const std::shared_ptr<FBroadcast> &broadcast);
    std::string getStubBroadcastPtrDestroy(const std::shared_ptr<FBroadcast> &broadcast);
    std::string getStubBroadcastVars(const std::shared_ptr<FBroadcast> &broadcast);
    std::string getStubBroadcastInternalVarDecls(const std::shared_ptr<FBroadcast> &broadcast);
    std::string getBroadcastCmds(const std::shared_ptr<FDBroadcast> &broadcast);
    std::string getStubRegistryMapTpl();
    std::string getStubBroadcastSubscribedSize(const std::shared_ptr<FBroadcast> &broadcast);
    std::string getStubBroadcastExportRegistry(const std::shared_ptr<FBroadcast> &broadcast);
    std::string getStubBroadcastRegistryCase(const std::shared_ptr<FBroadcast> &broadcast);
    std::string getStubBroadcastRegistryCaseTpl();
    std::string getStubDispatchMessageTpl();
    std::string getStubDispatchMessageCaseTpl(bool fireAndForget);
    std::string getStubMethodCallFuncTpl();
    std::string getStubMethodReplyFuncTpl();
    std::string getStubCmdRegStr(const std::string &name);

    std::string getProxyMethodAsyncArgs(const std::shared_ptr<FMethod> &method);
    std::string getProxyMethodSyncArgs(const std::shared_ptr<FMethod> &method);
    std::string getProxyMethodCallbackArgs(const std::shared_ptr<FMethod> &method);
    std::string getProxyMethodAsyncSignature(const std::shared_ptr<FMethod> &method);
    std::string getProxyMethodSyncSignature(const std::shared_ptr<FMethod> &method);
    std::string getProxyMethodOutStruct(const std::shared_ptr<FMethod> &method);
    std::string getProxyMethodAsyncCallbackSignature(const std::shared_ptr<FMethod> &method);
    std::string getProxyMethodSyncCallbackImpl(const std::shared_ptr<FMethod> &method);
    std::string getProxyMethodSerializeImpl(const std::shared_ptr<FMethod> &method);
    std::string getProxyMethodSyncImpl(const std::shared_ptr<FMethod> &method);
    std::string getProxyMethodAsyncImpl(const std::shared_ptr<FMethod> &method);
    std::string getProxyMethodFireAndForgetfImpl(const std::shared_ptr<FMethod> &method);
    std::string getProxyMethodSerializeCallStr(const std::shared_ptr<FMethod> &method);
    std::string getProxyMethodInvokeCallbackImpl(const std::shared_ptr<FMethod> &method);
    std::string getProxyMethodCase(const std::shared_ptr<FMethod> &method);
    std::string getProxyMethodSyncPtrInit(const std::shared_ptr<FMethod> &method);
    std::string getProxyMethodSyncPtrDestroy(const std::shared_ptr<FMethod> &method);
    std::string getProxyMethodAsyncPtrInit(const std::shared_ptr<FMethod> &method);
    std::string getProxyMethodAsyncPtrDestroy(const std::shared_ptr<FMethod> &method);

    std::string getProxyBroadcastCallbackSignature(const std::shared_ptr<FBroadcast> &broadcast);
    std::string getProxyBroadcastRegisterSignature(const std::shared_ptr<FBroadcast> &broadcast);
    std::string getProxyBroadcastVars(const std::shared_ptr<FBroadcast> &broadcast);
    std::string getProxyBroadcastInternalVarDecls(const std::shared_ptr<FBroadcast> &broadcast);
    std::string getProxyBroadcastSubImpl(const std::shared_ptr<FBroadcast> &broadcast);
    std::string getProxyBroadcastUnsubImpl(const std::shared_ptr<FBroadcast> &broadcast);
    std::string getProxyBroadcastInvokeCallbackImpl(const std::shared_ptr<FBroadcast> &broadcast);
    std::string getProxyBroadcastCase(const std::shared_ptr<FBroadcast> &broadcast);
    std::string getProxyBroadcastReplyCase(const std::shared_ptr<FBroadcast> &broadcast);
    std::string getProxyBroadcastPtrInit(const std::shared_ptr<FBroadcast> &broadcast);
    std::string getProxyBroadcastPtrDestroy(const std::shared_ptr<FBroadcast> &broadcast);

    std::string getProxyDispatchMessageTpl();
    std::string getProxyHeaderTpl();
    std::string getProxySourceTpl();
    std::string getStubHeaderTpl();
    std::string getStubSourceTpl();
    std::string getVersionComment();
    std::string getMethodComment(const std::shared_ptr<BstIdl::FMethod> &element);
    std::string getBroadcastComment(const std::shared_ptr<BstIdl::FBroadcast> &element);
    std::string getArgumentComment(const std::shared_ptr<BstIdl::FArgument> &arg, std::string &defaut_comment);
    std::string generateComments(const std::shared_ptr<BstIdl::FModelElement> &model, bool inline_);
    std::string breaktext(std::string text, const BstIdl::FAnnotationType &annotation);

    bool isFixedDerived(const std::shared_ptr<FTypeRef> &type);

    std::shared_ptr<FDExtensionElement> m_instance;
    std::string broadcastCases;
    std::shared_ptr<FDInterface> m_interface;
    std::string m_infName;
    std::string m_srcEndID;
    std::string m_dstEndID;
    std::string m_fid;
    std::string m_folderPath;
    std::string m_rtestr;
    uint8_t m_sid;
    int32_t m_maxMethodNum;
    int32_t m_maxBroadcastNum;
    bool m_bCheckDstEndID = false;
    bool m_bSeparateReply = false;
    bool m_bNotUseSwitchCase = false;
    bool m_bEnableSection = false;
};
} // namespace BstIdl

#endif