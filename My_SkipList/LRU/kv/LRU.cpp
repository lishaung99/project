#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <fstream>
#include <mutex>
#include <unordered_map>
#include <chrono>
#include <thread>

#define STORE_FILE "store/dumpFile"

std::mutex mtx;
std::string delimiter = ":";

// Node类：节点
template <typename K, typename V>
class Node {
public:
    Node() {}
    Node(K k, V v, int level, long long expire_time = 0);
    ~Node();
    K get_key() const;
    V get_value() const;
    void set_value(V);
    Node<K, V> **forward;
    int node_level;
    long long expire_time; // 过期时间
    Node<K, V>* prev; // 双链表的前驱指针
    Node<K, V>* next; // 双链表的后继指针
private:
    K key;
    V value;
};

template <typename K, typename V>
Node<K, V>::Node(const K k, const V v, int level, long long expire_time)
    : key(k), value(v), node_level(level), expire_time(expire_time), prev(nullptr), next(nullptr) {
    forward = new Node<K, V>*[level + 1];
    memset(forward, 0, sizeof(Node<K, V>*) * (level + 1));
}

template <typename K, typename V>
Node<K, V>::~Node() {
    delete[] forward;
}

template <typename K, typename V>
K Node<K, V>::get_key() const {
    return key;
}

template <typename K, typename V>
V Node<K, V>::get_value() const {
    return value;
}

template <typename K, typename V>
void Node<K, V>::set_value(V value) {
    this->value = value;
}

// SkipList 类： 一个组织和管理存储引擎 Node 类的类
template<typename K, typename V>
class SkipList {
private:
    int _max_level;
    int _skip_list_level;
    Node<K, V>* _header;
    int _element_count;
    std::ofstream _file_writer;
    std::ifstream _file_reader;
    std::unordered_map<K, Node<K, V>*> _map; // 哈希表
    Node<K, V>* _head; // 双链表的头节点
    Node<K, V>* _tail; // 双链表的尾节点
    std::thread _expire_thread; // 定期删除线程
    bool _running; // 是否运行定期删除线程

public:
    SkipList(int max_level);
    ~SkipList();
    int get_random_level();
    Node<K, V>* create_node(K, V, int, long long expire_time = 0);
    bool search_element(K);
    int insert_element(K, V, long long expire_time = 0);
    void delete_element(K);
    void display_list();
    bool modify_value(K, V);
    void dump_file();
    void load_file();
    void clear(Node<K, V>*);
    int size();
    void check_expire(); // 定期删除过期节点
    void move_to_head(Node<K, V>* node); // 将节点移动到链表头部
    void remove_from_list(Node<K, V>* node); // 从链表中移除节点

private:
    void get_key_value_from_string(const std::string& str, std::string* key, std::string* value);
    bool is_valid_string(const std::string& str);
};

template <typename K, typename V>
SkipList<K, V>::SkipList(int max_level)
    : _max_level(max_level), _skip_list_level(0), _element_count(0), _header(nullptr), _head(nullptr), _tail(nullptr), _running(true) {
    K k;
    V v;
    _header = new Node<K, V>(k, v, _max_level);
    _head = new Node<K, V>(k, v, 0);
    _tail = new Node<K, V>(k, v, 0);
    _head->next = _tail;
    _tail->prev = _head;
    _expire_thread = std::thread(&SkipList<K, V>::check_expire, this);
}

template <typename K, typename V>
SkipList<K, V>::~SkipList() {
    if (_file_writer.is_open()) {
        _file_writer.close();
    }
    if (_file_reader.is_open()) {
        _file_reader.close();
    }
    _running = false;
    _expire_thread.join();
    clear(_header->forward[0]);
    delete _header;
    delete _head;
    delete _tail;
}

template <typename K, typename V>
void SkipList<K, V>::clear(Node<K, V>* cur) {
    if (cur->forward[0] != nullptr) {
        clear(cur->forward[0]);
    }
    delete cur;
}

template <typename K, typename V>
int SkipList<K, V>::get_random_level() {
    int k = 1;
    while (rand() % 2) {
        k++;
    }
    k = (k < _max_level) ? k : _max_level;
    return k;
}

template <typename K, typename V>
Node<K, V>* SkipList<K, V>::create_node(const K k, const V v, int level, long long expire_time) {
    Node<K, V>* n = new Node<K, V>(k, v, level, expire_time);
    return n;
}

template <typename K, typename V>
bool SkipList<K, V>::search_element(K key) {
    std::cout << "search_element-----------------" << std::endl;
    Node<K, V>* current = _header;
    for (int i = _skip_list_level; i >= 0; i--) {
        while (current->forward[i] && current->forward[i]->get_key() < key) {
            current = current->forward[i];
        }
    }
    current = current->forward[0];
    if (current && current->get_key() == key) {
        if (current->expire_time > 0 && current->expire_time < std::chrono::system_clock::now().time_since_epoch().count()) {
            delete_element(key);
            std::cout << "Key " << key << " has expired and been deleted." << std::endl;
            return false;
        }
        move_to_head(current);
        std::cout << "Found key: " << key << ", value: " << current->get_value() << std::endl;
        return true;
    }
    std::cout << "Not Found Key:" << key << std::endl;
    return false;
}

template <typename K, typename V>
int SkipList<K, V>::insert_element(const K key, const V value, long long expire_time) {
    mtx.lock();
    Node<K, V>* current = _header;
    Node<K, V>* update[_max_level + 1];
    memset(update, 0, sizeof(Node<K, V>*) * (_max_level + 1));
    for (int i = _skip_list_level; i >= 0; i--) {
        while (current->forward[i] && current->forward[i]->get_key() < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    current = current->forward[0];
    if (current && current->get_key() == key) {
        current->set_value(value);
        current->expire_time = expire_time;
        move_to_head(current);
        mtx.unlock();
        return 1;
    }
    int random_level = get_random_level();
    if (random_level > _skip_list_level) {
        for (int i = _skip_list_level + 1; i <= random_level; i++) {
            update[i] = _header;
        }
        _skip_list_level = random_level;
    }
    Node<K, V>* inserted_node = create_node(key, value, random_level, expire_time);
    for (int i = 0; i < random_level; i++) {
        inserted_node->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = inserted_node;
    }
    inserted_node->prev = _head;
    inserted_node->next = _head->next;
    _head->next->prev = inserted_node;
    _head->next = inserted_node;
    _map[key] = inserted_node;
    std::cout << "Successfully inserted key:" << key << ", value:" << value << std::endl;
    _element_count++;
    mtx.unlock();
    return 0;
}

template <typename K, typename V>
void SkipList<K, V>::delete_element(K key) {
    mtx.lock();
    Node<K, V>* current = _header;
    Node<K, V>* update[_max_level + 1];
    memset(update, 0, sizeof(Node<K, V>*) * (_max_level + 1));
    for (int i = _skip_list_level; i >= 0; i--) {
        while (current->forward[i] && current->forward[i]->get_key() < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    current = current->forward[0];
    if (current && current->get_key() == key) {
        for (int i = 0; i <= _skip_list_level; i++) {
            if (update[i]->forward[i] != current) break;
            update[i]->forward[i] = current->forward[i];
        }
        while (_skip_list_level > 0 && _header->forward[_skip_list_level] == 0) {
            _skip_list_level--;
        }
        remove_from_list(current);
        _map.erase(key);
        std::cout << "Successfully deleted key " << key << std::endl;
        delete current;
        _element_count--;
    }
    mtx.unlock();
}

template <typename K, typename V>
void SkipList<K, V>::display_list() {
    std::cout << "Skip List:" << std::endl;
    for (int i = _skip_list_level; i >= 0; i--) {
        Node<K, V>* node = _header->forward[i];
        std::cout << "Level " << i << ": ";
        while (node != nullptr) {
            std::cout << node->get_key() << ":" << node->get_value() << "; ";
            node = node->forward[i];
        }
        std::cout << std::endl;
    }
    std::cout << "LRU List: ";
    Node<K, V>* node = _head->next;
    while (node != _tail) {
        std::cout << node->get_key() << ":" << node->get_value() << "; ";
        node = node->next;
    }
    std::cout << std::endl;
}

template <typename K, typename V>
bool SkipList<K, V>::modify_value(K key, V new_value) {
    mtx.lock();
    Node<K, V>* current = _header;
    for (int i = _skip_list_level; i >= 0; i--) {
        while (current->forward[i] && current->forward[i]->get_key() < key) {
            current = current->forward[i];
        }
    }
    current = current->forward[0];
    if (current && current->get_key() == key) {
        current->set_value(new_value);
        move_to_head(current);
        std::cout << "Successfully modified key " << key << " to new value " << new_value << std::endl;
        mtx.unlock();
        return true;
    } else {
        std::cout << "Key " << key << " not found, cannot modify value." << std::endl;
        mtx.unlock();
        return false;
    }
}

template <typename K, typename V>
void SkipList<K, V>::dump_file() {
    std::cout << "dump_file-----------------" << std::endl;
    _file_writer.open(STORE_FILE);
    Node<K, V>* node = _header->forward[0];
    while (node != nullptr) {
        _file_writer << node->get_key() << ":" << node->get_value() << "\n";
        std::cout << node->get_key() << ":" << node->get_value() << ";\n";
        node = node->forward[0];
    }
    _file_writer.flush();
    _file_writer.close();
}

template <typename K, typename V>
bool SkipList<K, V>::is_valid_string(const std::string& str) {
    if (str.empty()) {
        return false;
    }
    if (str.find(delimiter) == std::string::npos) {
        return false;
    }
    return true;
}

template <typename K, typename V>
void SkipList<K, V>::get_key_value_from_string(const std::string& str, std::string* key, std::string* value) {
    if (!is_valid_string(str)) {
        return;
    }
    *key = str.substr(0, str.find(delimiter));
    *value = str.substr(str.find(delimiter) + 1, str.length());
}

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
        insert_element(stoi(*key), *value);
        std::cout << "key:" << *key << "value:" << *value << std::endl;
    }
    delete key;
    delete value;
    _file_reader.close();
}

template <typename K, typename V>
int SkipList<K, V>::size() {
    return _element_count;
}

template <typename K, typename V>
void SkipList<K, V>::check_expire() {
    while (_running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        mtx.lock();
        Node<K, V>* node = _tail->prev;
        while (node != _head) {
            if (node->expire_time > 0 && node->expire_time < std::chrono::system_clock::now().time_since_epoch().count()) {
                Node<K, V>* prev = node->prev;
                delete_element(node->get_key());
                node = prev;
            } else {
                break;
            }
        }
        mtx.unlock();
    }
}

template <typename K, typename V>
void SkipList<K, V>::move_to_head(Node<K, V>* node) {
    remove_from_list(node);
    node->prev = _head;
    node->next = _head->next;
    _head->next->prev = node;
    _head->next = node;
}

template <typename K, typename V>
void SkipList<K, V>::remove_from_list(Node<K, V>* node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

int main() {
    SkipList<int, std::string> skipList(6);

    skipList.insert_element(1, "hello", std::chrono::system_clock::now().time_since_epoch().count() + 10000);
    skipList.insert_element(2, "mine", std::chrono::system_clock::now().time_since_epoch().count() + 20000);
    skipList.insert_element(3, "fist", std::chrono::system_clock::now().time_since_epoch().count() + 30000);
    skipList.insert_element(4, "skip list", std::chrono::system_clock::now().time_since_epoch().count() + 40000);
    skipList.insert_element(5, "sucess!", std::chrono::system_clock::now().time_since_epoch().count() + 50000);

    std::cout << "Skip List after insertion:" << std::endl;
    skipList.display_list();

    int key_to_search = 3;
    if (skipList.search_element(key_to_search)) {
        std::cout << "Element with key " << key_to_search << " found." << std::endl;
    } else {
        std::cout << "Element with key " << key_to_search << " not found." << std::endl;
    }

    skipList.modify_value(2, "my");

    int key_to_delete = 4;
    skipList.delete_element(key_to_delete);
    std::cout << "Skip List after deletion of key " << key_to_delete << ":" << std::endl;
    skipList.display_list();

    skipList.dump_file();

    skipList.load_file();
    std::cout << "Skip List after loading from file:" << std::endl;
    skipList.display_list();

    return 0;
}


/*
LRU缓存机制：
    使用双链表维护节点的访问顺序，最近访问的节点放在链表头部，最久未访问的节点放在链表尾部。
    使用哈希表快速查找节点。
过期时间管理：
    为每个节点添加expire_time字段，表示节点的过期时间。
    实现定期删除策略，通过后台线程定期检查并删除过期节点。
    实现惰性删除策略，在访问节点时检查其是否过期，如果过期则删除。
周期性数据存盘策略：
    定期调用dump_file方法将数据保存到磁盘。
    在程序启动时调用load_file方法从磁盘加载数据。
注意事项：
    过期时间的单位是毫秒，可以通过std::chrono::system_clock::now().time_since_epoch().count()获取当前时间戳。
    定期删除线程通过std::this_thread::sleep_for实现定期检查。
    在多线程环境下，需要使用互斥锁mtx来保护共享数据。
*/