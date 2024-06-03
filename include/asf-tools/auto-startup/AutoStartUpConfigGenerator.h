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
#ifndef AUTO_START_UP_CONFIG_GENERATOR
#define AUTO_START_UP_CONFIG_GENERATOR

#include "FDModel/FDExtensionRoot.h"
#include "asf-tools/ASFPropertyAccessor.h"
#include "generator/AbstractGenerator.h"
#include "nlohmann/json.hpp"
#include "model/FInterface.h"
#include <string>

namespace BstASF
{
class AutoStartUpConfigGenerator : public BstIdl::AbstractGenerator
{

public:
    static AutoStartUpConfigGenerator &getInstance()
    {
        static AutoStartUpConfigGenerator instance;
        return instance;
    }
    bool generate() override
    {
        return true;
    }
    void generateConfigFiles(const std::shared_ptr<BstIdl::FDExtensionRoot> &provider,
                             const std::shared_ptr<ASFPropertyAccessor> &_accessor, std::string &dir);

    const std::string auto_startup_systemd_service =
        R"(__UNIT__
__SERVICE__
__INSTALL__)";
    const std::string auto_startup_systemd_service_unit =
        R"([Unit]
__DESCRIPTION__
__DEPENDS&&SEQUENCE__
)";
    const std::string auto_startup_systemd_service_service =
        R"([Service]
__TYPE__
__ENVIRONMENT__
__EXEC__
)";
    const std::string auto_startup_systemd_service_install =
        R"([Install]
__WANTBY__)";

    const std::string auto_startup_slm_xml =
        R"(<SLM:system>
__SLM_COMPONENTS__
__SLM_REUSE_INCLUSIONS__
</SLM:system>)";

    const std::string auto_startup_slm_xml_component =
        R"(
    <SLM:component name="__PROCESS_NAME__">
        <SLM:stdout iomode="w">/dev/null</SLM:stdout>
        <SLM:stderr iomode="w+">/tmp/slmlog</SLM:stderr>
        <SLM:stdin iomode="r+"></SLM:stdin> 
__COMPONENT_ATTRIBUTES__
        <SLM:repair>none</SLM:repair>
    </SLM:component>
    )";

    const std::string auto_startup_slm_xml_envvar = R"(        <SLM:envvar>__ENVIRONMENTS__</SLM:envvar>)";

    const std::string auto_startup_slm_xml_depend_session =
        R"(        <SLM:depend [state="session"]>__DEPEND_PROCESS__</SLM:depend>)";

    const std::string auto_startup_slm_xml_depend_stateless =
        R"(        <SLM:depend [state="stateless"]>__DEPEND_PROCESS__</SLM:depend>)";

    const std::string auto_startup_slm_xml_command = R"(        <SLM:command>__COMMAND__</SLM:command>)";

    const std::string auto_startup_slm_xml_args = R"(        <SLM:args>__ARGS__</SLM:args>)";

    const std::string auto_startup_slm_xml_waitfor = R"(        <SLM:waitfor wait="__WAIT">/dev/socket</SLM:waitfor>)";

    const std::string auto_startup_slm_xml_dbus_daemon = R"(
    <SLM:component name="dbus-daemon">
        <SLM:command launch="builtin">no_op</SLM:command>
        <SLM:waitfor wait="pathname">/usr/bin/dbus-daemon</SLM:waitfor>
    </SLM:component>)";

    const std::string auto_startup_slm_xml_doctype = R"(
<!DOCTYPE SLM_system [
__ENTITIES__
]>
    )";
    const std::string auto_startup_slm_xml_doctype_entity =
        R"(    <!ENTITY __INCLUSION_NAME__ SYSTEM '__FILE_NAME__'>)";

private:
    class module
    {
    public:
        module(const std::string &_name, const bool &_startup, const bool &_restart, const std::string &_start_action,
               const std::string &_start_dtc, const std::string &_version, const std::string &_special_para,
               const std::string &_label) :name(_name),
            auto_startup(_startup), auto_restart(_restart), max_start_action(_start_action), max_start_dtc(_start_dtc),
            version(_version), special_para(_special_para), label(_label)
        {
        }
        ~module() = default;

        void toJson(nlohmann::ordered_json &j)
        {
            j = {{"module_name", name},
                 {"module_auto_startup", auto_startup},
                 {"module_auto_restart", auto_restart},
                 {"module_max_start_action", max_start_action},
                 {"module_max_start_dtc", max_start_dtc},
                 {"module_version", version},
                 {"module_special_para", special_para},
                 {"module_label", label}};
        }

    private:
        std::string name;
        bool auto_startup;
        bool auto_restart;
        std::string max_start_action;
        std::string max_start_dtc;
        std::string version;
        std::string special_para;
        std::string label;
    };

    AutoStartUpConfigGenerator() : AbstractGenerator()
    {
    }
    virtual ~AutoStartUpConfigGenerator() = default;
    AutoStartUpConfigGenerator(const AutoStartUpConfigGenerator &) = default;
    AutoStartUpConfigGenerator(AutoStartUpConfigGenerator &&) noexcept = default;
    AutoStartUpConfigGenerator &operator=(const AutoStartUpConfigGenerator &) = default;
    AutoStartUpConfigGenerator &operator=(AutoStartUpConfigGenerator &&) = default;

    nlohmann::ordered_json generateConfigJson(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                              const std::shared_ptr<ASFPropertyAccessor> &_accessor);
    std::string generateSystemdConfigService(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                             const std::shared_ptr<ASFPropertyAccessor> &_accessor);
    std::string generateSLMConfigXML(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                     const std::shared_ptr<ASFPropertyAccessor> &_accessor);
    void moduleListToJson(nlohmann::ordered_json &j, const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                          const std::shared_ptr<ASFPropertyAccessor> &_accessor);
    void moduleToJson(nlohmann::ordered_json &j, const module &obj);
    std::string getUnitDefination(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                  const std::shared_ptr<ASFPropertyAccessor> &_accessor);
    std::string getServiceDefination(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                     const std::shared_ptr<ASFPropertyAccessor> &_accessor);
    std::string getInstallDefination(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                     const std::shared_ptr<ASFPropertyAccessor> &_accessor);
    std::string getSLMComponentAttributes(const std::shared_ptr<BstIdl::FDExtensionRoot> &_provider,
                                          const std::shared_ptr<ASFPropertyAccessor> &_accessor);

protected:
};

} // namespace BstASF
#endif
