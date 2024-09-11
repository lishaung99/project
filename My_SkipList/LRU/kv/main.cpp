/*
 * @Description: 
 * @Author: lishuang
 * @Date: 2024-09-11 09:37:38
 * @FilePath: \c++project\kv\main.cpp
 * @LastEditTime: 2024-09-11 10:32:19
 * @LastEditors: lishuang
 */

#include <iostream>
#include "skiplist.h"
#define FILE_PATH "./store/dumpFile"

int main() {
    // 使用默认的比较函数
    SkipList<int, std::string> skipList(6);

    skipList.insert_element(1, "value1");
    skipList.insert_element(2, "value2");
    skipList.insert_element(3, "value3");

    skipList.modify_value(2, "new_value2");

    skipList.display_list();

    // 使用自定义的比较函数
    auto customComp = [](const std::string& a, const std::string& b) {
        return a.length() < b.length();
    };

    SkipList<std::string, std::string, decltype(customComp)> customSkipList(6, customComp);

    customSkipList.insert_element("short", "value1");
    customSkipList.insert_element("longer", "value2");
    customSkipList.insert_element("verylongstring", "value3");

    customSkipList.modify_value("longer", "new_value2");

    customSkipList.display_list();

    return 0;
}