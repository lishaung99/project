/*
 * @Description: 
 * @Author: lishuang
 * @Date: 2024-09-10 18:24:47
 * @FilePath: \c++projectd:\LearningPro\project\My_SkipList\main.cpp
 * @LastEditTime: 2024-09-11 11:17:17
 * @LastEditors: lishuang
 */
#include <iostream>
#include "skiplist.h"
#define FILE_PATH "./store/dumpFile"

int main() {

    // 键值中的key用int型，如果用其他类型，需要自定义比较函数
    // 而且如果修改key的类型，同时需要修改skipList.load_file函数
    SkipList<int, std::string> skipList(6);
	skipList.insert_element(1, "第"); 
	skipList.insert_element(3, "一个"); 
	skipList.insert_element(7, "个人"); 
	skipList.insert_element(8, "跳表"); 
	skipList.insert_element(9, "学习"); 
	skipList.insert_element(19, "实现"); 
	skipList.insert_element(19, "完成！"); 

    std::cout << "skipList size:" << skipList.size() << std::endl;

    skipList.dump_file();

    // skipList.load_file();

    skipList.search_element(9);
    skipList.search_element(18);

    skipList.modify_value(3, "一个实现的");

    skipList.display_list();

    skipList.delete_element(3);
    skipList.delete_element(7);

    std::cout << "skipList size:" << skipList.size() << std::endl;

    skipList.display_list();
}