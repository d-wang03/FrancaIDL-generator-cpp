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
#ifndef STRING_UTILITY_H
#define STRING_UTILITY_H

#include <cctype>
#include <cwctype>
#include <list>
#include <string>

#define CERR std::cerr << "\033[1;31m" // Light Red
#define CWRN std::cout << "\033[1;33m" // Light Yello
#define ENDL "\033[0m" << std::endl    // Default

/**
 * Returns a string that has whitespace removed from the start and the end.
 *
 * Whitespace means any character for which QChar::isSpace() returns true.
 * This includes the ASCII characters '\t', '\n', '\v', '\f', '\r', and ' '.
 */
template <typename T> std::basic_string<T> &trim(std::basic_string<T> &s)
{
    if (s.empty())
    {
        return s;
    }

    typename std::basic_string<T>::iterator c;
    // Erase whitespace before the string

    for (c = s.begin(); c != s.end() && iswspace(*c++);)
        ;
    s.erase(s.begin(), --c);

    // Erase whitespace after the string

    for (c = s.end(); c != s.begin() && iswspace(*--c);)
        ;
    s.erase(++c, s.end());

    return s;
}

template <typename T> std::basic_string<T> trim(std::basic_string<T> &&s)
{
    if (s.empty())
    {
        return s;
    }

    typename std::basic_string<T>::iterator c;
    // Erase whitespace before the string

    for (c = s.begin(); c != s.end() && iswspace(*c++);)
        ;
    s.erase(s.begin(), --c);

    // Erase whitespace after the string

    for (c = s.end(); c != s.begin() && iswspace(*--c);)
        ;
    s.erase(++c, s.end());

    return s;
}

template <typename T> std::basic_string<T> simplify(std::basic_string<T> &&s)
{
    if (s.empty())
    {
        return s;
    }
    typename std::basic_string<T>::iterator c;
    bool dupSpace = false;

    // Erase duplicated whitespace
    for (c = s.begin(); c != s.end();)
    {
        if (dupSpace)
        {
            if (iswspace(*c))
            {
                c = s.erase(c);
                continue;
            }
            dupSpace = false;
        }

        if (iswspace(*c))
            dupSpace = true;
        ++c;
    }
    if (iswspace(*s.begin()))
        s.erase(s.begin());

    if (iswspace(*s.rbegin()))
        s.pop_back();

    return s;
}
template <typename T> std::basic_string<T> simplifyPoint(std::basic_string<T> &&s)
{
    if (s.empty())
    {
        return s;
    }
    typename std::basic_string<T>::iterator c;
    bool dupPoint = false;

    // Erase duplicated '.' from unnamed overWrite
    for (c = s.begin(); c != s.end();)
    {
        if (dupPoint)
        {
            if (*c == '.')
            {
                c = s.erase(c);
                continue;
            }
            dupPoint = false;
        }
        if (*c == '.')
            dupPoint = true;
        ++c;
    }
    if (*s.begin() == '.')
        s.erase(s.begin());

    if (*s.rbegin() == '.')
        s.pop_back();

    return s;
}
static inline std::string replace_one(std::string &inout, const std::string &what, const std::string &with)
{
    std::string::size_type pos{};
    if (inout.npos != (pos = inout.find(what.data(), pos, what.length())))
        inout.replace(pos, what.length(), with.data(), with.length());

    return inout;
}

static inline std::string replace_all(std::string &inout, const std::string &what, const std::string &with)
{
    for (std::string::size_type pos{}; inout.npos != (pos = inout.find(what.data(), pos, what.length()));
         pos += with.length())
    {
        inout.replace(pos, what.length(), with.data(), with.length());
    }
    return inout;
}
static inline std::string replace_all(std::string &&inout, const std::string &what, const std::string &with)
{
    return replace_all(inout, what, with);
}

static inline void remove_all(std::string &inout, const std::string &what)
{
    replace_all(inout, what, "");
}

static inline std::string remove_last(std::string inout, size_t n = 1)
{
    if (n >= inout.size())
        inout.clear();
    else
        inout.erase(inout.size() - n);
    return inout;
}

static inline std::string join(const std::list<std::string> &list, const std::string &separator)
{
    std::string ret;
    for (const auto &item : list)
        ret.append(separator + item);
    ret.erase(0, separator.size());
    return ret;
}

static inline bool contains(const std::string &str, const std::string &what)
{
    if (str.npos != str.find(what.data(), 0, what.length()))
        return true;
    else
        return false;
}

static inline uint32_t count(const std::string &str, const std::string &what)
{
    uint32_t ret = 0;
    for (std::string::size_type pos{}; str.npos != (pos = str.find(what.data(), pos, what.length()));
         pos += what.length())
        ++ret;
    return ret;
}

static inline std::string toFirstUpper(std::string str)
{
    str[0] = toupper(str[0]);
    return str;
}

static inline std::string toFirstLower(std::string str)
{
    str[0] = tolower(str[0]);
    return str;
}

static inline std::string toUpper(std::string str)
{
    for (auto &c : str)
        c = (char)toupper(c);
    return str;
}

static inline std::string toLower(std::string str)
{
    for (auto &c : str)
        c = (char)tolower(c);
    return str;
}

static inline std::list<std::string> split(const std::string &input, const std::string &splitter)
{
    size_t start_pos = 0;
    size_t end_pos = std::string::npos;
    std::list<std::string> ret;
    while ((end_pos = input.find(splitter, start_pos)) != std::string::npos)
    {
        ret.emplace_back(input.substr(start_pos, end_pos - start_pos));
        start_pos = end_pos + 1;
    }
    ret.emplace_back(input.substr(start_pos));
    return ret;
}

static inline bool isWord(const std::string &name)
{
    if (name.empty())
    {
        return false;
    }
    for (char c : name)
    {
        if (!std::isalnum(c) && c != '_')
        {
            return false;
        }
    }
    return true;
}

#endif