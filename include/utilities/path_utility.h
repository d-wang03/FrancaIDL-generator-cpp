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
#ifndef PATH_UTILITY_H
#define PATH_UTILITY_H

#include <experimental/filesystem>
#include <stdlib.h>
namespace fs = std::experimental::filesystem;

class CurrentAppPath
{
public:
    static CurrentAppPath &getInstance()
    {
        static CurrentAppPath _ins;
        return _ins;
    }

    fs::path absolute() const
    {
        return fs::system_complete(_currentAppPath);
    }

    fs::path parent_path() const
    {
        return absolute().parent_path();
    }

    void currentAppPath(fs::path path)
    {
        auto real_path = findAppRealPath(path);
        _currentAppPath = fs::canonical(std::move(real_path));
    }

    fs::path findAppRealPath(fs::path path)
    {
        // find in current_path
        auto real_path = isRealPath(fs::current_path(), path);
        if (!real_path.empty())
            return real_path;

        // find in system conditional variable PATH
        std::string env = getenv("PATH");
#ifdef _WIN32
        auto path_list = split(env, ";");
#else
        auto path_list = split(env, ":");
#endif
        for (const auto &p : path_list)
        {
            real_path = isRealPath(fs::path(p), path);
            if (!real_path.empty())
                return real_path;
        }

        return path;
    }

    fs::path isRealPath(fs::path curr_path, fs::path path)
    {
        std::error_code ec;
#ifdef _WIN32
        std::string separator("\\");
#else
        std::string separator("/");
#endif
        fs::path exe_file(curr_path.string() + separator + path.string());
        if (fs::is_regular_file(exe_file, ec))
        {
            exe_file = fs::canonical(exe_file);
            fs::path plugin_folder(exe_file.parent_path().parent_path().string() + separator + "plugin");
            if (fs::is_directory(plugin_folder))
            {
                return exe_file;
            }
        }
        return fs::path("");
    }

private:
    CurrentAppPath() = default;
    ~CurrentAppPath() = default;
    CurrentAppPath(const CurrentAppPath &) = delete;
    CurrentAppPath(CurrentAppPath &&) = delete;
    fs::path _currentAppPath;
};

#endif