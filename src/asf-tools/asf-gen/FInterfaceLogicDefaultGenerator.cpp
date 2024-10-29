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
#include "asf-tools/asf-gen/FInterfaceLogicDefaultGenerator.h"
#include "asf-tools/GeneralServiceGeneratorExtensions.h"
#include "capicxx-core-tools/FInterfaceStubGenerator.h"
#include "model/FModelManager.h"
#include "utilities/string_utility.h"
#include <fstream>
#include <iostream>
#include <map>
namespace BstASF
{
void FInterfaceLogicDefaultGenerator::generateLogicDefault(
    const std::shared_ptr<BstIdl::FInterface> &fInterface,
    const std::shared_ptr<BstCommonAPI::CommonapiPropertyAccessor> &deploymentAccessor, const std::string &dir)
{
    auto &gen = BstASF::GeneralServiceGeneratorExtensions::getInstance();
    auto instances = gen.getServer(fInterface);
    if (instances.empty())
        return;

    // write loglicDefaultHeader
    auto loglicDefaultHeaderPath = gen.getLogicDefaultHeaderPath(fInterface);
    auto d_h_path = mk_dir(dir + "/src-gen", "/" + loglicDefaultHeaderPath);
    if (d_h_path.empty())
        return;
    d_h_path += "/" + gen.getLogicDefaultHeaderFile(fInterface);
    std::ofstream OsWriteH(d_h_path, std::ofstream::out);
    auto loglicDefaultHeader = generateLogicDefaultHeader(fInterface, deploymentAccessor);
    OsWriteH << replace_all(loglicDefaultHeader, "\t", "    ");
    OsWriteH.close();

    // write loglicDefaultSource
    auto loglicDefaultSourcePath = gen.getLogicDefaultSourcePath(fInterface);
    auto d_s_path = mk_dir(dir + "/src-gen", "/" + loglicDefaultSourcePath);
    if (d_s_path.empty())
        return;
    d_s_path += "/" + gen.getLogicDefaultSourceFile(fInterface);
    std::ofstream OsWriteS(d_s_path, std::ofstream::out);
    auto loglicDefaultSource = generateLogicDefaultSource(fInterface, deploymentAccessor);
    OsWriteS << replace_all(loglicDefaultSource, "\t", "    ");
    OsWriteS.close();

    // for instances of the fInterface
    // write logicSource
    for (const auto &instance : instances)
    {
        if (gen.hasUserLogic(instance))
            continue;
        // write logicSource
        auto loglicHeaderPath = gen.getLogicHeaderPath(instance);
        auto d_lh_path = mk_dir(dir, loglicHeaderPath);
        if (d_lh_path.empty())
            return;
        d_lh_path += "/" + gen.getLogicHeaderFile(instance);
        std::ofstream OsWriteLH(d_lh_path, std::ofstream::out);
        auto loglicHeader = generateLogicHeader(instance, deploymentAccessor);
        OsWriteLH << replace_all(loglicHeader, "\t", "    ");
        OsWriteLH.close();

        // write logicHeader
        auto loglicSourcePath = gen.getLogicSourcePath(instance);
        auto d_ls_path = mk_dir(dir, loglicSourcePath);
        if (d_ls_path.empty())
            return;
        d_ls_path += "/" + gen.getLogicSourceFile(instance);
        std::ofstream OsWriteLS(d_ls_path, std::ofstream::out);
        auto loglicSource = generateLogicSource(instance, deploymentAccessor);
        OsWriteLS << replace_all(loglicSource, "\t", "    ");
        OsWriteLS.close();
    }
}

std::string FInterfaceLogicDefaultGenerator::generateLogicDefaultHeader(
    const std::shared_ptr<BstIdl::FInterface> &fInterface,
    const std::shared_ptr<BstCommonAPI::CommonapiPropertyAccessor> &deploymentAccessor)
{
    if (!fInterface || !fInterface->getContainer())
        return "";

    auto &gen = BstASF::GeneralServiceGeneratorExtensions::getInstance();
    std::string header;
    header += getLicense();
    header += "\n" + gen.generateASFLicenseHeader();
    auto class_name = gen.getLogicDefaultClassName(fInterface);
    auto define_name = gen.isProjectMode() ? gen.getProjectDefineName(fInterface) : gen.getDefineName(fInterface);
    std::transform(define_name.begin(), define_name.end(), define_name.begin(), ::toupper);

    header += "\n#ifndef " + define_name + "_SERVER_LOGIC_DEFAULT_H_";
    header += "\n#define " + define_name + "_SERVER_LOGIC_DEFAULT_H_\n";
    header += "\n#include \"logic/tcapilogic.h\"\n#include \"capiconfig.h\"";
    header += "\n\nusing namespace " + fInterface->getNameSpace() + ";";
    auto model = std::dynamic_pointer_cast<BstIdl::FModel>(fInterface->getContainer());
    header += "\n\nnamespace AdvancedServiceFramework {\n";
    // class def
    header += "\nclass " + class_name + " : public TCAPILogic\n{";
    header += "\n\tT_OBJECT\n\tT_DISABLE_COPY(" + class_name + ")\n\tT_DISABLE_MOVE(" + class_name + ")\n";
    header += gen.getSlots(fInterface);
    // public
    std::string server_type("");
    gen.getServer(fInterface).size() > 1 ? server_type = gen.getStubDefaultClassName(fInterface)
                                         : server_type = gen.getStubImplClassName(fInterface);
    header += "\n\n  public :";
    header += "\n\t" + class_name + "(const std::string &name);";
    header += "\n\tvirtual ~" + class_name + "()override;";
    header += "\n\n\tvoid setService(std::weak_ptr<" + server_type + "> &&service)\n\t{";
    header += "\n\t\tm_service = service;\n\t}";

    // add addtional methods and variables.
    auto baseInterfaces = BstIdl::FModelManager::getInstance().getBaseInterfaces(fInterface);
    std::list<std::string> attrMethods;
    std::list<std::string> MethodsDecl;
    std::list<std::string> BroadcastsDecl;
    for (const auto &inf : baseInterfaces)
    {
        for (const auto &md : inf->getMethods())
        {
            if (gen.isRecordAndSimulateInterface(inf) && contains(gen.record_simulate_methods, md->getName()))
                continue;

            MethodsDecl.emplace_back(gen.getMethodDecl(md));
            // if is override-method or overrided-method
            if (!md->getSelector().empty() || (containsNumByName(fInterface->getMethods(), md) > 1))
            {
                MethodsDecl.emplace_back(gen.getMethodSeletcorFuncPtrDecl(md));
            }
        }

        for (const auto &bc : inf->getBroadcasts())
        {
            BroadcastsDecl.emplace_back(gen.getBroadcastDecl(bc));
        }

        for (const auto &attr : inf->getAttributes())
        {
            attrMethods.emplace_back(gen.getSetMethodDeclStr(attr));
            attrMethods.emplace_back(gen.getGetMethodDeclStr(attr));
        }
    }
    if (!attrMethods.empty())
        header.append("\n\n\t" + join(attrMethods, "\n\t"));
    // protected
    header += "\n\n  protected :\n";
    header.append("\t" + join(MethodsDecl, "\n\t") + "\n");
    header.append("\t" + join(BroadcastsDecl, "\n\t") + "\n");
    header += "\n\tstd::weak_ptr<" + server_type + "> m_service;";
    header += "\n\n};";
    // namespace}
    header += "\n} // namespace AdvancedServiceFramework";
    header += "\n#endif // " + define_name + "_SERVER_LOGIC_DEFAULT_H_";
    return header;
}

std::string FInterfaceLogicDefaultGenerator::generateLogicDefaultSource(
    const std::shared_ptr<BstIdl::FInterface> &fInterface,
    const std::shared_ptr<BstCommonAPI::CommonapiPropertyAccessor> &deploymentAccessor)
{
    auto &gen = BstASF::GeneralServiceGeneratorExtensions::getInstance();
    if (!fInterface || !fInterface->getContainer())
        return "";

    std::string src;
    src += getLicense();
    src += "\n" + gen.generateASFLicenseHeader();
    auto class_name = gen.getLogicDefaultClassName(fInterface);

    src += "\n#include \"typeconversion.h\"\n";
    src += "#include <" + gen.getLogicDefaultHeaderPath(fInterface) + ">\n";
    src += "\nusing namespace " + fInterface->getNameSpace() + ";\n";
    // namespace
    auto model = std::dynamic_pointer_cast<BstIdl::FModel>(fInterface->getContainer());
    src += "\nnamespace AdvancedServiceFramework {\n\n";

    src += class_name + "::" + class_name + "(const std::string &name):TCAPILogic(name)\n{";
    src += "\n}\n";
    src += class_name + "::~" + class_name + "()\n{\n}\n";
    // on_TCAPIMsg_ DECL_FUNC
    src += gen.getSlotFunctions(fInterface) + "\n";

    auto baseInterfaces = BstIdl::FModelManager::getInstance().getBaseInterfaces(fInterface);
    std::list<std::string> attrMethodsDef;
    std::list<std::string> MethodsDef;
    std::list<std::string> BroadcastsDef;
    for (const auto &inf : baseInterfaces)
    {
        for (const auto &md : inf->getMethods())
        {
            if (gen.isRecordAndSimulateInterface(inf) && contains(gen.record_simulate_methods, md->getName()))
                continue;
            MethodsDef.emplace_back(gen.getMethodDefine(md, class_name));
        }

        for (const auto &bc : inf->getBroadcasts())
        {
            BroadcastsDef.emplace_back(gen.getBroadcastDefine(bc, class_name));
        }
        for (const auto &attr : inf->getAttributes())
        {
            attrMethodsDef.emplace_back(gen.getGetMethodDefineStr(attr, class_name));
            attrMethodsDef.emplace_back(gen.getSetMethodDefineStr(attr, class_name));
        }
    }
    src.append(join(MethodsDef, "\n") + "\n");
    src.append(join(BroadcastsDef, "\n") + "\n");
    src.append(join(attrMethodsDef, "\n") + "\n");
    // namespace }
    src += "\n} // namespace AdvancedServiceFramework";
    return src;
}

std::string FInterfaceLogicDefaultGenerator::generateLogicHeader(
    const std::shared_ptr<BstIdl::FDExtensionElement> &instance,
    const std::shared_ptr<BstCommonAPI::CommonapiPropertyAccessor> &deploymentAccessor)
{
    auto &gen = BstASF::GeneralServiceGeneratorExtensions::getInstance();
    auto fInterface = std::dynamic_pointer_cast<BstIdl::FInterface>(instance->getTarget());
    if (!fInterface || !fInterface->getContainer())
        return "";

    std::string header;
    header += getLicense();
    header += "\n" + gen.generateASFLicenseHeader();
    replace_one(header, "* All manual modifications will be LOST by next generation.", "* All manual modifications will be LOST by next generation.\n * Please add your own logic to implement the functions in this class.");
    auto class_name = gen.getLogicClassName(instance);
    auto define_name = gen.getProjectDefineName(instance);
    std::transform(define_name.begin(), define_name.end(), define_name.begin(), ::toupper);

    header += "\n#ifndef " + define_name + "_SERVER_LOGIC_H_";
    header += "\n#define " + define_name + "_SERVER_LOGIC_H_\n";
    header += "\n#include <" + gen.getLogicDefaultHeaderPath(fInterface) + ">\n";
    header += "\nnamespace AdvancedServiceFramework {\n";
    header += "\nclass " + class_name + " : public " + gen.getLogicDefaultClassName(fInterface) + "\n{";

    // public
    header += "\n  public : ";
    header += "\n\t" + class_name + "();";
    header += "\n\tvirtual ~" + class_name + "()override;\n";
    auto baseInterfaces = BstIdl::FModelManager::getInstance().getBaseInterfaces(fInterface);
    std::list<std::string> attrMethods;
    std::list<std::string> MethodsDecl;
    std::list<std::string> BroadcastsDecl;
    for (const auto &inf : baseInterfaces)
    {
        for (const auto &attr : inf->getAttributes())
        {
            auto set_decl = gen.getSetMethodDeclStr(attr);
            auto get_decl = gen.getGetMethodDeclStr(attr);
            attrMethods.emplace_back(gen.TransToOverrideDeclStr(set_decl));
            attrMethods.emplace_back(gen.TransToOverrideDeclStr(get_decl));
        }
        for (const auto &md : inf->getMethods())
        {
            if (gen.isRecordAndSimulateInterface(inf) && contains(gen.record_simulate_methods, md->getName()))
                continue;
            auto md_decl = gen.getMethodDecl(md);
            MethodsDecl.emplace_back(gen.TransToOverrideDeclStr(md_decl));
        }

        for (const auto &bc : inf->getBroadcasts())
        {
            auto bc_decl = gen.getBroadcastDecl(bc);
            BroadcastsDecl.emplace_back(gen.TransToOverrideDeclStr(bc_decl));
        }
    }

    header.append("\t" + join(attrMethods, "\n\t") + "\n");
    // protected
    header += "\n  protected :\n";
    // add addtional methods and variables.
    header.append("\t" + join(MethodsDecl, "\n\t") + "\n");
    header.append("\t" + join(BroadcastsDecl, "\n\t") + "\n");
    // private
    header += "\n};";
    // namespace}
    header += "\n\n} // namespace AdvancedServiceFramework";
    header += "\n#endif // " + define_name + "_SERVER_LOGIC_H_";
    return header;
}
std::string FInterfaceLogicDefaultGenerator::generateLogicSource(
    const std::shared_ptr<BstIdl::FDExtensionElement> &instance,
    const std::shared_ptr<BstCommonAPI::CommonapiPropertyAccessor> &deploymentAccessor)
{
    auto &gen = BstASF::GeneralServiceGeneratorExtensions::getInstance();
    auto fInterface = std::dynamic_pointer_cast<BstIdl::FInterface>(instance->getTarget());
    if (!fInterface || !fInterface->getContainer())
        return "";

    std::string src("");
    src += getLicense();
    src += "\n" + gen.generateASFLicenseHeader();
    replace_one(src, "* All manual modifications will be LOST by next generation.", "* All manual modifications will be LOST by next generation.\n * Please add your own logic to implement the functions in this class.");
    auto class_name = gen.getLogicClassName(instance);
    auto default_name = gen.getLogicDefaultClassName(fInterface);
    src += "\n#include \"" + class_name + ".h\"\n#include <Bstlog/log.h>\n";
    // namespace
    src += "\nnamespace AdvancedServiceFramework {\n";
    src += class_name + "::" + class_name + "():" + default_name + "(__func__)\n{";
    src += "\n}\n";
    src += class_name + "::~" + class_name + "()\n{\n}\n";

    auto baseInterfaces = BstIdl::FModelManager::getInstance().getBaseInterfaces(fInterface);
    std::list<std::string> attrMethodsDef;
    std::list<std::string> MethodsDef;
    std::list<std::string> BroadcastsDef;
    for (const auto &inf : baseInterfaces)
    {
        for (const auto &md : inf->getMethods())
        {
            if (gen.isRecordAndSimulateInterface(inf) && contains(gen.record_simulate_methods, md->getName()))
                continue;
            MethodsDef.emplace_back(gen.getLogicMethodDefine(md, class_name));
        }

        for (const auto &bc : inf->getBroadcasts())
        {
            BroadcastsDef.emplace_back(gen.getBroadcastDefine(bc, class_name));
        }
        for (const auto &attr : inf->getAttributes())
        {
            attrMethodsDef.emplace_back(gen.getGetMethodDefineStr(attr, class_name));
            attrMethodsDef.emplace_back(gen.getSetMethodDefineStr(attr, class_name));
        }
    }
    src.append(join(MethodsDef, "\n") + "\n");
    src.append(join(BroadcastsDef, "\n") + "\n");
    src.append(join(attrMethodsDef, "\n") + "\n");

    // namespace }
    src += "\n\n} // namespace AdvancedServiceFramework";
    return src;
}
} // namespace BstASF