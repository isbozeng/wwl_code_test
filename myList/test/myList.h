#ifndef MYLIST_H
#define MYLIST_H

#include <iostream>
#include <string>
#include <sstream>

struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

class MyList {
public:
    MyList();
    MyList(const std::string& str);
    MyList(const MyList& other);  // 拷贝构造函数
    MyList(MyList&& other) noexcept;  // 移动构造函数
    ~MyList();

    MyList& operator=(const MyList& other);  // 拷贝赋值运算符
    MyList& operator=(MyList&& other) noexcept;  // 移动赋值运算符
    

    void push_front(int val);
    void push_back(int val);
    void insert(int index, int val);
    void remove(int val);
    void pop_front();
    void pop_back();
    int size() const;
    bool empty() const;
    void clear();
    void print() const;

    MyList& operator<<(const std::string& str);
    friend std::ostream& operator<<(std::ostream& os, const MyList& list);
    MyList operator+(const MyList& other) const;
    MyList operator-() const;
    int& operator[](std::size_t index) const; 
    bool operator==(const MyList& other) const;

    class Iterator {
    public:
        Iterator(ListNode* node = nullptr) : current(node) {}
        int& operator*() const;
        Iterator& operator++();    // 前置递增
        Iterator operator++(int);  // 后置递增
        bool operator!=(const Iterator& other) const;
        bool operator==(const Iterator& other) const;
    private:
        ListNode* current;
    };

    Iterator begin() const;
    Iterator end() const;

private:
    ListNode* head;
    ListNode* tail;
    int list_size;

    void copy_from(const MyList& other);  // 辅助拷贝函数
};

#endif // MYLIST_H
