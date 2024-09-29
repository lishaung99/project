/*
List：底层实现逻辑。双向链表

优点：便于数据的插入与删除，迭代器不会失效

缺点：不合适用来查找元素，因为需要迭代器去访问
*/

// #pragma once

#include <iostream>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>


template <typename T>
class List{
private:
    // 1.数据量 2.头节点指针 3.尾结点指针
    
    //1）节点 定义节点的结构体 包括 节点的val 节点的双向指针
    struct Node{
        T data;
        Node *next;
        Node *prev;
        
        // 构造函数
        Node(const T &value, Node *nextNode = nullptr, Node *prevNode = nullptr)
        : data(value), next(nextNode), prev(prevNode){}
    };
    
    // 2）双向链表的属性
    Node *head;
    Node *tail;
    size_t size;

public:
    
    // 1.构造函数
    List():head(nullptr), tail(nullptr), size(0){}
    
    //2.析构函数
    ~List(){
        clear();
    }
    void clear() {
        while (head) {
            // 从头节点开始，依次删除节点
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        // 更新尾节点指针和链表大小
        tail = nullptr;
        size = 0;
    }
    
    // 3.尾部添加元素
    void push_back(const T &value){
        Node* newnode = new Node(value, nullptr, tail); 
        // 创建新节点的时候，将新节点的prev指针设置为链表的tail指针
        if(tail!=nullptr){
            // 尾指针不是空指针，即链表不为空
            tail->next = newnode;
        }else{
            // 链表为空 插入的元素就是头节点
            head = newnode;
        }
        tail = newnode;
        size++;
    }
    
    //4.在头部插入元素
    void push_front(const T &value){
        Node* newnode = new Node(value, head, nullptr); 
        if (head) {
            // 如果链表非空，将头节点的 prev 指针指向新节点
            head->prev = newnode;
        } else {
            // 如果链表为空，新节点同时也是尾节点
            tail = newnode;
        }
    
        // 更新头节点指针和链表大小
        head = newnode;
        size++;
    }
    
    //5.获取链表的数据量 get_size
    size_t getSize(){
        return size;
    }
    
    //6.通过索引访问链表中的元素
    /*
    ·处理越界的情况, 此处的处理是抛出异常。
    ·需要注意返回的是引用, 否则就不能通过[]更新元素
    */
    T &operator[](size_t index){
        // 从头结点开始访问数据
        Node* current = head;
        for(size_t i =0; i < index; i++){
            if (!current) {
                // 如果 index 超出链表长度，则抛出异常
                throw std::out_of_range("Index out of range");
            }
            current = current->next;
        }
        return current->data;
    }
    // const版本的访问链表中的元素
    const T &operator[](size_t index) const
    {
        // 从头节点开始遍历链表，找到第 index 个节点
        Node *current = head;
        for (size_t i = 0; i < index; ++i)
        {
            if (!current)
            {
                // 如果 index 超出链表长度，则抛出异常
                throw std::out_of_range("Index out of range");
            }
            current = current->next;
        }

        // 返回节点中的数据
        return current->data;
    }
    
    //7.删除尾部的元素
    void pop_back(){
        if(size > 0){// 列表中存在元素
            Node* newtail = tail->prev;
            delete tail;
            tail = newtail;
            if (tail != nullptr) {
                tail->next = nullptr;
            } else {
                head = nullptr;  // 如果链表为空，头节点也置为空 即只有一个节点的情况
            }
            size--;
        }
    }
    
    //8.删除头部的元素
    void pop_front(){
        if(size > 0){
            Node* newhead = head->next;
            delete head;
            head = newhead;
            if(head != nullptr){
                head->prev = nullptr;
            }else{
                tail = nullptr; // 如果链表为空，尾节点也置为空
            }
        }
        size--;
    }
    
    // 9.迭代器
    Node* begin() {
        return head;
    }
    const Node* begin()const{
        return head;
    }
    Node* end(){
        return nullptr;
    }
    const Node* end()const{
        return nullptr;
    }
    
    // 10.获取指定值的节点
    Node *getNode(const T &val)
    {
        Node *node = head;
        while (node != nullptr && node->data != val)
        {
            node = node->next;
        }

        return node;
    }


    // 11. 
    T *find(const T &val)
    {
        Node *node = getNode(val);
        if (node == nullptr)
        {
            return nullptr;
        }
        return &node->data;
    }

    // 12.删除指定值的节点
    void remove(const T &val)
    {
        Node *node = head;
        while (node != nullptr && node->data != val)
        {
            node = node->next;
        }

        if (node == nullptr)
        {
            // 没有找到
            return;
        }
        if (node != head && node != tail)
        {
            // 既不是头结点也不是尾结点
            node->prev->next = node->next;
            node->next->prev = node->prev;
        }
        else if (node == head && node == tail)
        {
            // 既是头结点也是尾结点
            head = nullptr;
            node = nullptr;
        }
        else if (node == head)
        {
            // 是头结点
            head = node->next;
            head->prev = nullptr;
        }
        else
        {
            // 是尾结点
            tail = node->prev;
            tail->next = nullptr;
        }

        --size;

        delete node;
        node = nullptr;
    }
    
    bool empty() { return size == 0; }
    
    //10.打印列表
    void printElements() const {
        for (Node* current = head; current; current = current->next) {
            std::cout << current->data << " ";
        }
        std::cout << std::endl;
    }
    
};
// 重载 << 运算符
template <typename T>
std::ostream &operator<<(std::ostream &os, const List<T> &pt)
{
    for (typename List<T>::Node *current = pt.head; current;
         current = current->next)
    {
        os << " " << current->data;
    }
    os << std::endl;
    return os;
}
int main() {
    // 创建一个 List 对象
    List<int> myList;

    int N;
    std::cin >> N;
    // 读走回车
    getchar();
    std::string line;
    // 接收命令
    for (int i = 0; i < N; i++) {
        std::getline(std::cin, line);
        std::istringstream iss(line);
        std::string command;
        iss >> command;
        int value;

        if (command == "push_back") {
            iss >> value;
            myList.push_back(value);
        }

        if (command == "push_front") {
            iss >> value;
            myList.push_front(value);
        }

        if (command == "pop_back") {
            myList.pop_back();
        }

        if (command == "pop_front") {
            myList.pop_front();
        }

        if (command == "remove") {
            iss >> value;
            myList.remove(value);
        }

        if (command == "clear") {
            myList.clear();
        }

        if (command == "size") {
            std::cout << myList.getSize() << std::endl;
        }

        if (command == "get") {
            iss >> value;
            std::cout << myList[value] << std::endl;
        }

        if (command == "print") {
            if (myList.getSize() == 0) {
                std::cout << "empty" << std::endl;
            } else {
                myList.printElements();
            }
        }
    }
    return 0;
}