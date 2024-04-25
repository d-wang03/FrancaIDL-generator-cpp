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
#ifndef LIST_UTILITY_H
#define LIST_UTILITY_H

#include <algorithm>
#include <list>
#include <string>

template <typename T1, typename T2> bool addListItem(std::list<T1> &list, T2 &&t)
{
    auto iter = std::find_if(list.cbegin(), list.cend(), [&t](auto item) { return item == t; });
    if (iter != list.cend())
        return false;
    list.emplace_back(std::forward<T2>(t));
    return true;
}

template <typename T1, typename T2> bool addListItemByName(std::list<T1> &list, T2 &&t)
{
    auto iter = std::find_if(list.cbegin(), list.cend(), [&t](auto item) { return item->getName() == t->getName(); });
    if (iter != list.cend())
        return false;
    list.emplace_back(std::forward<T2>(t));
    return true;
}

template <typename T1, typename T2> bool contains(const std::list<T1> &list, T2 &&t)
{
    auto iter = std::find(list.cbegin(), list.cend(), t);
    return iter != list.cend();
}

template <typename T1, typename T2> bool containsByName(const std::list<T1> &list, T2 &&t)
{
    auto iter = std::find_if(list.cbegin(), list.cend(), [&t](auto item) { return item->getName() == t->getName(); });
    return iter != list.cend();
}

// check the override method or broadcast
template <typename T1, typename T2> int containsNumByName(const std::list<T1> &list, T2 &&t)
{
    int num = 0;
    auto iter = list.cbegin();
    while ((iter = std::find_if(iter, list.cend(), [&t](auto item) { return item->getName() == t->getName(); })) !=
           list.cend())
    {
        ++iter;
        ++num;
    }
    return num;
}

template <typename T>
bool exists(const std::list<std::tuple<T, std::string>> &list, const T &t, const std::string &name)
{
    auto iter = std::find_if(list.cbegin(), list.cend(),
                             [&t, &name](auto item) { return std::get<0>(item) == t && std::get<1>(item) == name; });
    return iter != list.cend();
}

template <typename T, typename T2>
bool exists(const std::list<std::tuple<T, std::string, T2>> &list, const T &t, const std::string &name,
            const T2 &belonged)
{
    auto iter = std::find_if(list.cbegin(), list.cend(), [&t, &name, &belonged](auto item) {
        return std::get<0>(item) == t && std::get<1>(item) == name && std::get<2>(item) == belonged;
    });
    return iter != list.cend();
}

template <typename T> void spliceBack(std::list<T> &list_res, const std::list<T> &list)
{
    for (const T item : list)
        addListItem(list_res, item);
}
#endif