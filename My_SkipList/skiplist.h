#include<iostream>
#include<cstdlib>   // 随机函数
#include<cstring>   // memset
#include <cmath>    
#include <fstream>  // 引入文件操作
#include <mutex>    // 引入互斥锁

// 存储文件路径
#define STORE_FILE "store/dumpFile"  

std::mutex mtx; // 定义互斥锁 : 对插入节点成员函数和删除节点成员函数进行加锁
std::string delimiter = ":";


// Node类：节点
template <typename K, typename V>
class Node {
public:
    Node() {}
    Node(K k, V v, int);
    ~Node();
    K get_key() const;
    V get_value() const;
    void set_value(V);
    Node<K, V> ** forward; // 每一个节点都有一个指针数组，用来保存指向的下一个数据位置
    int node_level; // 节点处于的层数
private:
    K key; //节点检索的标志，应该存储为节点的数值 ，这个根据需要获取
    V value; 
};
 
// // 不同节点同层跳转的数据结构基于链表 也就是单层找位置
// class LinkList{
//     int data;
//     LinkList* next;
     
//     // 遍历链表
//     void travasel(LinkList* head){
//         LinkList* current = head;
//         while(current->next != nullptr){
//             current = current->next;
//         }
//     }
// };
 
 
// 1.Node类拥有的构造函数
template <typename K, typename V>
Node< K, V>::Node(const K k, const V v, int level){
    this->key = k;
    this->value = v;
    this->node_level = level;
    this->forward = new Node< K, V> *[level + 1];
    
    // Fill forward array with 0(NULL) 
    memset( this->forward, 0, sizeof(Node<K, V> *) * (level + 1));
};
 
// 2.Node类析构函数
template <typename K, typename V>
Node<K, V>::~Node(){
    delete[] forward;
};
 
// 3.Node类拥有的获取 key 成员函数
template <typename K, typename V>
K Node<K, V>::get_key() const {
    return key;
};
 
// 4.Node类拥有的获取 value 成员函数
template <typename K, typename V>
V Node<K, V>::get_value() const {
    return value;
};
 
// 5.Node类拥有的设置 value 成员函数
template <typename K, typename V>
void Node<K, V>::set_value(V value){
    this->value = value;
};

// SkipList 类： 一个组织和管理存储引擎 Node 类的类
template<typename K, typename V>
class SkipList{
private:
    int _max_level; // 最大层数
    int _skip_list_level; // 当前层数
    Node<K, V>* _header;         // 跳表的头节点
    int _element_count;          // 跳表中组织的所有节点的数量
    std::ofstream _file_writer;  // 文件写入器
    std::ifstream _file_reader;  // 文件读取器
    
public:
    SkipList(int);          // 构造函数
    ~SkipList();            // 类拥有的析构函数
    int get_random_level();             // 获取节点的随机层级
    Node<K, V> *create_node(K, V, int); // 节点创建
    bool search_element(K);             // 搜索节点
    int insert_element(K, V);           // 插入节点
    void delete_element(K);             // 删除节点
    void display_list();                // 展示节点
    void dump_file();                   // 持久化数据到文件
    void load_file();                   // 从文件加载数据
    void clear(Node<K, V> *);           // 递归删除节点
    int size();                         // 跳表中的节点个数
private:
    void get_key_value_from_string( const std::string& str, std::string* key, std::string* value);  // 将字符串分割键值对
    bool is_valid_string(const std::string& str); // 验证字符串合法性
    
};

/*
1.SkipList构造函数
    设置最大层级：根据预设值确定跳表的最大层级
    初始化成员变量：设置跳表当前层级为 0，节点计数为 0
    创建头节点：初始化一个头节点，其层级等于跳表的最大层级
*/
template <typename K, typename V>
SkipList<K, V>::SkipList(int max_level)
    :_max_level{max_level},_skip_list_level{0},_element_count{0},_header{nullptr}
{
    // this->_max_level = max_level;
    // this->_skip_list_level = 0;
    // this->_element_count = 0;
    K k;
    V v;
    //  创建头节点，并初始化键值为默认值
    try{
        _header = new Node<K, V>(k, v, _max_level);
    }catch(std::bad_alloc& e){
        std::cerr << "Bad allocation for SkipList: " << e.what() << std::endl;
        _header = nullptr;
        throw;
    }
    
    // this->_header = new Node<K, V>(k, v, _max_level);
};
// 2.SkipList类拥有的析构函数
template<typename K, typename V> 
SkipList<K, V>::~SkipList() {
   if(_file_writer.is_open()){
       _file_writer.close();
   }
   if(_file_reader.is_open()){
       _file_reader.close();
   }
   
   // 递归删除节点
   if(_header->forward[0] != nullptr){
       clear(_header->forward[0]);
   }
   delete(_header);
    
}

template <typename K, typename V>
void SkipList<K, V>::clear(Node<K, V> * cur)
{
    if(cur->forward[0]!=nullptr){
        clear(cur->forward[0]);
    }
    delete(cur);
}

// 3.SkipList类 获取节点的随机层级
template <typename K, typename V>
int SkipList<K, V>::get_random_level() {
   // 初始化层级：每个节点至少出现在第一层。
   int k = 1;
   // 随机层级增加：使用 rand() % 2 实现抛硬币效果，决定是否升层。
   while (rand() % 2) {
      k++;
   }
   // 层级限制：确保节点层级不超过最大值 _max_level。
   k = (k < _max_level) ? k : _max_level;
   // 返回层级：返回确定的层级值，决定节点插入的层。
   return k;
};

/*
 4.SkipList类：创建一个新节点,节点实例化并返回新节点
    @param k 节点的键
    @param v 节点的值
    @param level 节点的层级
    @return 新创建的节点指针
 */
template <typename K, typename V>
Node<K, V>* SkipList<K, V>::create_node(const K k, const V v, int level) {
    // 实例化新节点，并为其分配指定的键、值和层级
    Node<K, V> *n = new Node<K, V>(k, v, level);
    return n; // 返回新创建的节点
}

// 5. 搜索指定的键值是否存在于跳表中
// params： k值
template<typename K, typename V>
bool SkipList<K, V>::search_element(K key){
    std::cout << "search_element-----------------" << std::endl;
    // 与链表有关
    Node<K, V>* current = _header; // 当前头节点
    // 从最高层开始搜索
    for(int i = _skip_list_level; i >= 0; i--){
        // 遍历当前层级，直到下一个节点的键值大于或等于待查找的键值
        // 注意这里的while是会在当前层级一直寻找key值
        while( current->forward[i] && current->forward[i]->get_key()< key){
            // 移动到当前层的下一个节点
            current = current->forward[i];
        }
        // 当前节点的下一个节点的键值大于待查找的键值时，进行下沉到下一层
        // 下沉操作通过循环的 i-- 实现
    }
    // 检查当前层（最底层）的下一个节点的键值是否为待查找的键值
    current = current->forward[0];
    if(current && current->get_key() == key){
        // 找到匹配值
        std::cout << "Found key: " << key << ", value: " << current->get_value() << std::endl;
        return true;
    }
    std::cout << "Not Found Key:" << key << std::endl;
    return false;
}

template<typename K, typename V>
int SkipList<K, V>::insert_element(const K key, const V value ){
    mtx.lock();  // 在函数第一句加锁
    Node<K, V> *current =this-> _header;
    // 用于在各层更新指针的数组
    Node<K, V> *update[_max_level + 1];  // 用于记录每层中待更新指针的节点
    // 分配内存
    memset(update, 0, sizeof(Node<K, V>*) * (_max_level + 1));
    // 从最高层向下搜索插入位置
    for(int i = _skip_list_level; i >= 0; i--){
        // 寻找当前层中最接近且小于 key 的节点
        while(current->forward[i] != nullptr && current->forward[i]->get_key() < key){
            current = current->forward[i];
        }
        // 保存每层中该节点，以便后续插入时更新指针
        update[i] = current;
    }
     // 移动到最底层的下一节点，准备插入操作
    current = current->forward[0]; 
    // 检查待插入的节点的键是否已存在 
    if (current != NULL && current->get_key() == key) {
        // 键已存在，取消插入
        std::cout << "key: " << key << ", exists" << std::endl;
        // 在此处需要提前 return，所以提前解锁
        mtx.unlock();
        return 1;
    }
    // 检查待插入的节点是否已存在于跳表中  不存在
    if (current == NULL || current->get_key() != key) {
        // 通过随机函数决定新节点的层级高度
        int random_level = get_random_level(); // 决定节点插入的层
        if(random_level > _skip_list_level){
            // 对所有新的更高层级，将头节点设置为它们的前驱节点
            for (int i = _skip_list_level + 1; i <= random_level; i++) {
                update[i] = _header;
            }
            // 更新跳表的当前最高层级为新节点的层级
            _skip_list_level = random_level;
        }
        // 创建插入节点
        Node<K, V>* inserted_node = create_node(key, value, random_level);
        // 在各层插入新节点，同时更新前驱节点的forward指针
        for(int i = 0; i < random_level; i++){
            inserted_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = inserted_node;
        }
        std::cout << "Successfully inserted key:" << key << ", value:" << value << std::endl;
        _element_count++;
    }
    mtx.unlock();  // 函数执行完毕后解锁
    return 0;
}

template<typename K, typename V>
void SkipList<K, V>::delete_element(K key){
    mtx.lock();  // 加锁
    Node<K, V>* current = _header;
    Node<K, V>* update[_max_level + 1]; //用于记录每层中待更新指针的节点
    memset(update, 0, sizeof(Node<K, V>*) * (_max_level + 1));
    // 从最高层开始向下搜索待删除节点
    for(int i = _skip_list_level; i >= 0; i--){
        // 寻找当前层中最接近且小于 key 的节点
        while(current->forward[i] != nullptr && current->forward[i]->get_key() < key){
            current = current->forward[i];
        }
        update[i] = current; // 记录每层待删除节点的前驱
    }
    current = current->forward[0];
    // 确认找到了待删除的节点
    if (current != NULL && current->get_key() == key) {
        // 逐层更新指针，删除节点
        for(int i = 0; i <= _skip_list_level; i++){
            if(update[i]->forward[i] != current) 
                break;
            update[i]->forward[i] = current->forward[i];
        }
        // 调整跳表的层级
        while (_skip_list_level > 0 && _header->forward[_skip_list_level] == 0) {
            _skip_list_level--;
        }
        std::cout << "Successfully deleted key "<< key << std::endl;
        delete current; // 释放节点占用的内存
        _element_count--; // 节点计数减一
    }
    mtx.unlock();  // 解锁
    return;
}

template<typename K, typename V>
void SkipList<K, V>::display_list(){
    std::cout << "Successfully inserted key:" << key << ", value:" << value << std::endl;
    // 从最上层开始向下遍历所有层
    for(int i = _skip_list_level; i >=0; i++){
        Node<K,V> *node = this->_header->forward[i]; // 获取当前层的头节点
        std::cout << "Level " << i << ": ";
        // 遍历当前层的所有节点
        while (node != nullptr){
            std::cout<< node->get_key() << ":"<<node->gey_value()<<";"; // 当前节点k和v
            node = node->forward[i]; //向下一个节点移动
        }
        std::cout<< std::endl;
    }
}

// 数据保存
template<typename K, typename V>
void SkipList<K, V>::dump_file(){
    std::cout << "dump_file-----------------" << std::endl;
    // STORE_FILE 这个根据需要获取
    _file_reader.open(STORE_FILE);// 打开文件
    Node<K, V> *node = this->_header->forward[0];// 从头节点开始遍历
    
    while (node != nullptr) {
        _file_writer << node->get_key() << ":" << node->get_value() << "\n"; // 写入键值对
        std::cout << node->get_key() << ":" << node->get_value() << ";\n";
        node = node->forward[0]; // 移动到下一个节点
    }
    _file_writer.flush(); // 刷新缓冲区，确保数据完全写入
    _file_writer.close(); // 关闭文件
    return ;
}

// 验证字符串合法性
template<typename K, typename V>
bool SkipList<K, V>::is_valid_string(const std::string& str) {

    if (str.empty()) {
        return false;
    }
    if (str.find(delimiter) == std::string::npos) {
        return false;
    }
    return true;
}

// 将字符串分割为键和值
template <typename K, typename V>
void SkipList<K, V>::get_key_value_from_string(const std::string &str, std::string *key, std::string *value) {
    if (!is_valid_string(str)) {
        return;
    }
    *key = str.substr(0, str.find(delimiter));
    *value = str.substr(str.find(delimiter) + 1, str.length());
}
// Load data from disk
template <typename K, typename V>
void SkipList<K, V>::load_file() {
    _file_reader.open(STORE_FILE);
    std::cout << "load_file-----------------" << std::endl;
    std::string line;
    std::string* key = new std::string();
    std::string* value = new std::string();

    while (getline(_file_reader, line)) {
        get_key_value_from_string(line, key, value);
        if (key->empty() || value->empty()) {
            continue;
        }
        // Define key as int type
        insert_element(stoi(*key), *value);
        std::cout << "key:" << *key << "value:" << *value << std::endl;
    }

    delete key;
    delete value;
    _file_reader.close();
}

// Get current SkipList size
template<typename K, typename V> 
int SkipList<K, V>::size() { 
    return _element_count;
}

// int main() {
//     // 创建一个最大层级为 6 的跳表
//     SkipList<int, int> skipList(6);

//     // 插入一些元素
//     skipList.insert_element(1, 100);
//     skipList.insert_element(2, 20);
//     skipList.insert_element(3, 30);
//     skipList.insert_element(4, 40);
//     skipList.insert_element(5, 50);

//     // 显示跳表内容
//     std::cout << "Skip List after insertion:" << std::endl;
//     skipList.display_list();

//     // 搜索元素
//     int key_to_search = 3;
//     if (skipList.search_element(key_to_search)) {
//         std::cout << "Element with key " << key_to_search << " found." << std::endl;
//     } else {
//         std::cout << "Element with key " << key_to_search << " not found." << std::endl;
//     }

//     // 删除元素
//     int key_to_delete = 4;
//     skipList.delete_element(key_to_delete);
//     std::cout << "Skip List after deletion of key " << key_to_delete << ":" << std::endl;
//     skipList.display_list();

//     // 持久化数据到文件
//     skipList.dump_file();

//     // 从文件加载数据
//     skipList.load_file();
//     std::cout << "Skip List after loading from file:" << std::endl;
//     skipList.display_list();

//     return 0;
// }
