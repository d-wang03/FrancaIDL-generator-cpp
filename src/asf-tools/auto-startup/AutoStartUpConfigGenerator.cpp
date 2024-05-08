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
#include "asf-tools/auto-startup/AutoStartUpConfigGenerator.h"
#include "asf-tools/GeneralServiceGeneratorExtensions.h"
#include <fstream>

namespace BstASF
{

void AutoStartUpConfigGenerator::generateConfigFiles(const std::shared_ptr<BstIdl::FDExtensionRoot> &provider,
                                                     const std::shared_ptr<ASFPropertyAccessor> &_accessor,
                                                     std::string &dir)
{
    if (!provider || !_accessor || dir.empty())
        return;
    auto config_type = _accessor->getConfigType(provider);
    if (config_type == "none" || config_type.empty() || (config_type != "systemd" && config_type != "slm"))
        return;
    // generate json config
    auto &gen = GeneralServiceGeneratorExtensions::getInstance();
    auto json_file = gen.getStarUpConfigJsonFile(provider);
    auto config_path = mk_dir(dir, "/" + json_file);
    if (config_path.empty())
        return;
    auto json_path = config_path + "/" + json_file;
    std::ofstream OsWriteJson(json_path, std::ofstream::out);
    auto jsonConfig = generateConfigJson(provider, _accessor);
    OsWriteJson << jsonConfig.dump(4);
    OsWriteJson.close();

    // generate .service or .xml
    if (config_type.compare("systemd") == 0)
    {
        auto service_file = gen.getStarUpSystemdConfigFile(provider);
        auto config_path = mk_dir(dir, "/" + service_file);
        if (config_path.empty())
            return;
        auto service_path = config_path + "/" + service_file;
        std::ofstream OsWriteService(service_path, std::ofstream::out);
        auto serviceConfig = generateSystemdConfigService(provider, _accessor);
        OsWriteService << serviceConfig;
        OsWriteService.close();
    }
    else
    {
        auto xml_file = gen.getStarUpSLMConfigFile(provider);
        auto config_path = mk_dir(dir, "/" + xml_file);
        if (config_path.empty())
            return;
        auto xml_path = config_path + "/" + xml_file;
        std::ofstream OsWriteXML(xml_path, std::ofstream::out);
        auto xmlConfig = generateSLMConfigXML(provider, _accessor);
        OsWriteXML << xmlConfig;
        OsWriteXML.close();
    }
}

nlohmann::ordered_json AutoStartUpConfigGenerator::generateConfigJson(
    const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider, const std::shared_ptr<ASFPropertyAccessor> &_accessor)
{
    if (!_provider || !_accessor)
        return nullptr;

    nlohmann::ordered_json json_config = {{"safety_config_major_version", 1}, {"safety_config_minor_version", 1}};
    // get module_list
    moduleListToJson(json_config, _provider, _accessor);
    if (json_config.empty())
    {
        std::cerr << "--ERRORS : The module's eight parameters should have been all defined for every module!"
                  << std::endl;
        std::cerr << "--WARNING & RECOMMEND : If do not need a parameter in a module like module_special_para, just "
                     "make it an empty string \"\" !"
                  << std::endl;
    }
    return json_config;
}

void AutoStartUpConfigGenerator::moduleListToJson(nlohmann::ordered_json &j,
                                                  const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                                  const std::shared_ptr<ASFPropertyAccessor> &_accessor)
{
    if (!_provider || !_accessor)
        return;

    // get and check module parameters
    auto name = _accessor->getProcessName(_provider);
    if (name.empty())
    {
        j.clear();
        return;
    }

    auto auto_startup = _accessor->getModuleAutoStartup(_provider);
    auto auto_restart = _accessor->getModuleAutoRestart(_provider);
    auto max_start_action = _accessor->getModuleMaxStartAction(_provider);
    auto max_start_dtc = _accessor->getModuleMaxStartDtc(_provider);
    auto version = _accessor->getModuleVersion(_provider);
    auto special_para = _accessor->getModuleSpecialPara(_provider);
    auto label = _accessor->getModuleLabel(_provider);

    // push back ordered_json object
    module tmp(name, auto_startup, auto_restart, max_start_action, max_start_dtc, version, special_para, label);
    nlohmann::ordered_json module_json;
    tmp.toJson(module_json);
    j["module_list"].push_back(module_json);
}

std::string AutoStartUpConfigGenerator::generateSystemdConfigService(
    const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider, const std::shared_ptr<ASFPropertyAccessor> &_accessor)
{
    auto content = auto_startup_systemd_service;
    // unit
    auto unit = getUnitDefination(_provider, _accessor);
    auto service = getServiceDefination(_provider, _accessor);
    auto install = getInstallDefination(_provider, _accessor);
    replace_one(content, "__UNIT__", unit);
    replace_one(content, "__SERVICE__", service);
    replace_one(content, "__INSTALL__", install);
    // install
    return content;
}

std::string AutoStartUpConfigGenerator::getUnitDefination(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                                          const std::shared_ptr<ASFPropertyAccessor> &_accessor)
{
    auto unit = auto_startup_systemd_service_unit;
    // __DESCRIPTION__
    std::string description(_accessor->getDescription(_provider));
    if (!description.empty())
        description = "Description=" + description;
    replace_one(unit, "__DESCRIPTION__", description);

    // __DEPENDS&&SEQUENCE__
    std::list<std::string> dependsAndSequnce;
    auto before_list = _accessor->getBefore(_provider);
    auto after_list = _accessor->getAfter(_provider);
    auto wants = _accessor->getWants(_provider);
    auto requires = _accessor->getRequire(_provider);
    if (!before_list.empty())
        dependsAndSequnce.emplace_back("Before=" + join(before_list, " "));
    if (!after_list.empty())
        dependsAndSequnce.emplace_back("After=" + join(after_list, " "));
    if (!wants.empty())
        dependsAndSequnce.emplace_back("Wants=" + join(wants, " "));
    if (!requires.empty())
        dependsAndSequnce.emplace_back("Require=" + join(requires, " "));
    replace_one(unit, "__DEPENDS&&SEQUENCE__", join(dependsAndSequnce, "\n"));

    return unit;
}

std::string AutoStartUpConfigGenerator::getServiceDefination(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                                             const std::shared_ptr<ASFPropertyAccessor> &_accessor)
{
    auto service = auto_startup_systemd_service_service;
    // __TYPE__
    std::string type_command;
    std::string type(_accessor->getServiceType(_provider));
    if (!type.empty())
        type_command = "Type=" + type;
    if (type.compare("dbus") == 0)
        type_command.append("\nBusName=" + _accessor->getServiceBusName(_provider));
    replace_one(service, "__TYPE__", type_command);

    // __ENVIRONMENT__
    std::string environment;
    auto envr = _accessor->getEnvironmentVar(_provider);
    if (!envr.empty())
        environment.append("Environment=\"" + join(envr, "\" \"") + "\"");
    replace_one(service, "__ENVIRONMENT__", environment);

    // __EXEC__: start ExecStartPre ExecStopPost
    std::list<std::string> commands = _accessor->getServiceExecCommand(_provider);
    std::list<std::string> args = _accessor->getExecArgs(_provider);
    std::list<std::string> ExecStartPres(_accessor->getServiceExecStartPre(_provider));
    std::list<std::string> ExecStopPosts(_accessor->getServiceExecStopPost(_provider));
    std::list<std::string> Execs;

    if (!ExecStartPres.empty())
        Execs.emplace_back("ExecStartPre=" + join(ExecStartPres, "; "));

    if (type.compare("oneshot") == 0)
    {
        int num = commands.size();
        std::list<std::string> exec_commands;
        for (int i = 0; i < num; ++i)
        {
            exec_commands.emplace_back(commands.front() + " " + args.front());
            commands.pop_front();
            args.pop_front();
        }
        if (!exec_commands.empty())
            Execs.emplace_back("ExecStart=" + join(exec_commands, "; "));
    }
    else if (!commands.empty())
    {
        auto argStr = args.empty() ? "" : " " + args.front();
        Execs.emplace_back("ExecStart=" + commands.front() + argStr);
    }
    else
    {
        std::cerr << "WARNING: Service configuration file for Systemd have no command!"
                  << "\nIf not your original intension, the parameter \"ServiceExecCommand\" should be filled in.\n";
    }

    if (!ExecStopPosts.empty())
        Execs.emplace_back("ExecStopPost=" + join(ExecStopPosts, "; "));
    replace_one(service, "__EXEC__", join(Execs, "\n"));
    return service;
}

std::string AutoStartUpConfigGenerator::getInstallDefination(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                                             const std::shared_ptr<ASFPropertyAccessor> &_accessor)
{
    // __WANTBY__
    std::string wantby(_accessor->getServiceWantedBy(_provider));
    if (wantby.empty())
        return std::string();
    wantby = "WantedBy=" + wantby;
    auto install = auto_startup_systemd_service_install;
    replace_one(install, "__WANTBY__", wantby);
    return install;
}
std::string AutoStartUpConfigGenerator::generateSLMConfigXML(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                                             const std::shared_ptr<ASFPropertyAccessor> &_accessor)
{
    auto inclusion_names = _accessor->getReuseSLMSystemInclusionName(_provider);
    auto file_names = _accessor->getReuseSLMSystemFileName(_provider);
    if (inclusion_names.size() != file_names.size())
    {
        std::cerr << "DEPL ERROR: The paras of asf provider defination which named ReuseSLMSystemInclusionName.size() "
                     "not equal to getReuseSLMSystemFileName.size()!"
                  << std::endl;
    }
    auto inclusion_refs = inclusion_names;
    std::list<std::string> doctype_entities;
    while (!inclusion_names.empty() && !file_names.empty())
    {
        auto entity = auto_startup_slm_xml_doctype_entity;
        auto i_name = inclusion_names.front();
        auto f_name = file_names.front();
        file_names.pop_front();
        inclusion_names.pop_front();
        replace_one(entity, "__INCLUSION_NAME__", i_name);
        replace_one(entity, "__FILE_NAME__", f_name);
        doctype_entities.emplace_back(entity);
    }
    std::string doctype;
    if (!doctype_entities.empty())
    {
        doctype = auto_startup_slm_xml_doctype;
        replace_one(doctype, "__ENTITIES__", join(doctype_entities, "\n"));
    }
    auto slm_xml = doctype + auto_startup_slm_xml;
    auto component = auto_startup_slm_xml_component;
    replace_one(component, "__COMPONENT_ATTRIBUTES__", getSLMComponentAttributes(_provider, _accessor));
    replace_one(component, "__PROCESS_NAME__", _accessor->getProcessName(_provider));
    component.append(auto_startup_slm_xml_dbus_daemon);
    replace_one(slm_xml, "__SLM_COMPONENTS__", component);
    if (!inclusion_refs.empty())
        replace_one(slm_xml, "__SLM_REUSE_INCLUSIONS__", "\n\t&" + join(inclusion_refs, ";\n\t&") + ";");
    else
        replace_one(slm_xml, "__SLM_REUSE_INCLUSIONS__", "");
    return slm_xml;
}

std::string AutoStartUpConfigGenerator::getSLMComponentAttributes(
    const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider, const std::shared_ptr<ASFPropertyAccessor> &_accessor)
{
    std::list<std::string> slm_component_attributes;
    // depend
    auto wants = _accessor->getWants(_provider);
    auto requires = _accessor->getRequire(_provider);
    for (const auto want : wants)
    {
        if (want.empty())
            continue;
        auto depend = auto_startup_slm_xml_depend_stateless;
        replace_one(depend, "__DEPEND_PROCESS__", want);
        slm_component_attributes.emplace_back(depend);
    }
    for (const auto require : requires)
    {
        if (require.empty())
            continue;
        auto depend = auto_startup_slm_xml_depend_session;
        replace_one(depend, "__DEPEND_PROCESS__", require);
        slm_component_attributes.emplace_back(depend);
    }
    // envvar
    auto envvars = _accessor->getEnvironmentVar(_provider);
    for (const auto var : envvars)
    {
        if (var.empty())
            continue;
        auto env = auto_startup_slm_xml_envvar;
        replace_one(env, "__ENVIRONMENTS__", var);
        slm_component_attributes.emplace_back(env);
    }
    // command && args
    std::string command = auto_startup_slm_xml_command;
    replace_one(command, "__COMMAND__", _accessor->getSLMExecCommand(_provider));
    slm_component_attributes.emplace_back(command);

    auto args = _accessor->getExecArgs(_provider);
    if (!args.empty())
    {
        std::string args_templ = auto_startup_slm_xml_args;
        replace_one(args_templ, "__ARGS__", args.front());
        slm_component_attributes.emplace_back(args_templ);
    }
    return join(slm_component_attributes, "\n");
}
} // namespace BstASF
