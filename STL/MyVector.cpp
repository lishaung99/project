#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <stdexcept>

template<typename T>


class Vector{
private:
    // 容器类的属性有容器的容量、容器中元素的个数
    size_t capacity;
    size_t size;
    T *elements;     // 指向动态数组的指针
    
public:

    // 1.构造函数
    // Vector(): elements(nullptr), capacity(0)，size(0){ }
    Vector() : elements(nullptr), capacity(0), size(0) {}
    
    // 2.析构函数
    ~Vector(){
        delete[] elements;
    }
    
    //3.拷贝构造函数
    Vector(const Vector &other) : capacity(other.capacity), size(other.size){
        elements = new T[capacity];
        std::copy(other.elements, other.elements + size, elements);
        // 起始位置 结束位置 迭代器（元素指针）
    }
    

    /* 
    const修饰符：表示这个参数是常量，即不能通过这个参数去修改所引用的对象。
                 这确保了在函数内部不会意外地修改传入的对象，增加了函数的安全性和可预测性。
    Vector&：这是对Vector类对象的引用。引用类似于别名，
             它允许函数直接操作原始对象而不是对象的副本，避免了不必要的拷贝开销，提高了效率。
    other：仅仅是这个参数的名称，可以通过这个名称在函数内部访问传入的对象
    */
    // 4.拷贝赋值操作符
    Vector& operator=(const Vector& other){
        if (this != &other) { // 如果不是自赋值
            delete[] elements; // 1.释放内存
            capacity = other.capacity; // 2.重新赋值
            size = other.size;
            elements = new T[capacity];
            std::copy(other.elements, other.elements + size, elements); // 赋值所有元素
        }
    return *this;
        
    }
    
    // 5.push_back函数： 在容器末尾添加一个新元素 
    // 只需要将元素加入进去，不需要返回 
    // input ：容器  数值 
    void push_back(const T& value) {
        if (size == capacity) { // 如果当前没有足够的容量来存储新元素
            reserve(capacity == 0 ? 1 : 2 * capacity); // 扩展数组的容量
        }
        elements[size++] = value; // 将新元素添加到数组的末尾，并递增 size
    }
    
    // 6.获取容器存储数据长度
    size_t getSize() const {
        return size;
    }
    // 7.获取容器容量
    size_t getCapacity() const {
        return capacity;
    }
    
    // 8.下标操作符 ：允许通过下标来访问 Vector 中的元素
    // 分为 const 和 non-const 版本
    // const：它可以在const对象上被调用，并且在函数内部不会修改调用它的对象的状态。
    // non-const: 在函数内部可以修改调用它的对象的状态
    T& operator[](size_t index) { 
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return elements[index];
    }
    const T& operator[](size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return elements[index];
    }
    
    // 9.插入函数
    // 在容器制定位置插入一个元素
    // input： index value
    void insert(size_t index, const T& value){
        if (index > size) {
            throw std::out_of_range("Index out of range");
        }
        if (size == capacity) { // 需要扩展一个位置
            reserve(capacity == 0 ? 1 : capacity * 2);
        }
        // 开始插入：将 index 之后的所有元素向后移动一个位置，为新元素腾出空间
        for (size_t i = size; i > index; --i) { 
            elements[i] = elements[i - 1];
        }
        elements[index] = value;
        ++size;
    }
    
    //10.pop_back函数 取出元素
    // 删除 Vector 末尾的元素；
    // 减少 Vector 的 size；
    // 不实际释放内存，这意味着即使调用了 pop_back，capacity 保持不变
    void pop_back() {
        if (size > 0) {
        --size;
        }
    }
    
    // 清空数组：不释放容器空间（内存）
    void clear()
    {
        size = 0; 
    }

    // 使用迭代器遍历数组的开始位置
    T *begin()
    {
        return elements;
    }
    const T *begin() const
    {   // 使用迭代器遍历数组的开始位置（const）
        return elements;
    }

    // 使用迭代器遍历数组的结束位置
    T *end()
    {
        return elements + size;
    }
    const T *end() const
    {   //  使用迭代器遍历数组的结束位置（const)
        return elements + size;
    }

    // 打印数组中的元素
    void printElements() const
    {
        for (size_t i = 0; i < size; ++i)
        {
            std::cout << elements[i] << " ";
        }
        std::cout << std::endl;
    }

private:
    // 扩展数组容量
    void reserve(size_t newCapacity)
    {
        if (newCapacity > capacity)
        {
            T *newElements = new T[newCapacity];
            std::copy(elements, elements + size, newElements);
            delete[] elements;
            elements = newElements;
            capacity = newCapacity;
        }
    }
    
};


int main()
{
    // 创建一个 Vector 对象
    Vector<int> myVector;

    int N;
    std::cin >> N;
    // 读走回车
    getchar();

    std::string line;
    for (int i = 0; i < N; i++)
    {
        // 读取整行
        std::getline(std::cin, line);
        std::istringstream iss(line);
        std::string command;
        iss >> command;

        if (command == "push")
        {
            int value;
            iss >> value;
            myVector.push_back(value);
        }
        else if (command == "print")
        {
            if (myVector.getSize() == 0) {
                std::cout << "empty" << std::endl;
                continue;
            }
            myVector.printElements();
        }
        else if (command == "size")
        {
            std::cout << myVector.getSize() << std::endl;
        }
        else if (command == "get")
        {
            int index;
            iss >> index;
            std::cout << myVector[index] << std::endl;
        }
        else if (command == "insert")
        {
            int index, value;
            iss >> index >> value;
            myVector.insert(index, value);
        }
        else if (command == "pop")
        {
            myVector.pop_back();
        }
        else if (command == "iterator")
        {
            if (myVector.getSize() == 0)
            {
                std::cout << "empty" << std::endl;
                continue;
            }
            for (auto it = myVector.begin(); it != myVector.end(); ++it)
            {
                std::cout << *it << " ";
            }
            std::cout << std::endl;
        }
        else if (command == "foreach")
        {
            if (myVector.getSize() == 0)
            {
                std::cout << "empty" << std::endl;
                continue;
            }
            for (const auto &element : myVector)
            {
                std::cout << element << " ";
            }
            std::cout << std::endl;
        }
        else if (command == "clear")
        {
            myVector.clear();
        }
    }
    return 0;
}