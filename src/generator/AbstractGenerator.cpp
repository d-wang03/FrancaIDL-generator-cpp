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
#include "generator/AbstractGenerator.h"
#include <experimental/filesystem>
#include <iostream>

namespace fs = std::experimental::filesystem;
namespace BstIdl
{
std::string AbstractGenerator::mk_dir(const std::string &dir, const std::string &interface_path)
{
    auto size = interface_path.find_last_of("/");
    if (size == interface_path.npos)
    {
        std::cerr << "unexpected dir,interface_path:" << interface_path << std::endl;
        return std::string();
    }

    auto whole_path = dir + "/" + interface_path.substr(0, size);
    std::error_code ec;
    fs::create_directories(whole_path, ec);
    if (ec.value() != 0)
    {
        std::cerr << "create directory failed. " << whole_path << " error is " << ec.message() << std::endl;
        return std::string();
    }

    return whole_path;
}
std::string AbstractGenerator::commit_hash()
{
    return COMMIT_HASH;
}

} // namespace BstIdl