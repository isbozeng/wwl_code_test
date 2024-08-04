#include "myList.h"

// 构造函数
MyList::MyList() : head(nullptr), tail(nullptr), list_size(0) {}

MyList::MyList(const std::string& str) : head(nullptr), tail(nullptr), list_size(0) {
    std::istringstream iss(str);
    int val;
    while (iss >> val) {
        push_back(val);
    }
}

MyList::MyList(const MyList& other) : head(nullptr), tail(nullptr), list_size(0) {
    copy_from(other);
}

MyList::MyList(MyList&& other) noexcept : head(other.head), tail(other.tail), list_size(other.list_size) {
    other.head = nullptr;
    other.tail = nullptr;
    other.list_size = 0;
}

// 析构函数
MyList::~MyList() {
    clear();
}

// 拷贝赋值运算符
MyList& MyList::operator=(const MyList& other) {
    if (this != &other) {
        clear();
        copy_from(other);
    }
    return *this;
}

// 移动赋值运算符
MyList& MyList::operator=(MyList&& other) noexcept {
    if (this != &other) {
        clear();
        head = other.head;
        tail = other.tail;
        list_size = other.list_size;
        other.head = nullptr;
        other.tail = nullptr;
        other.list_size = 0;
    }
    return *this;
}

// 成员函数
void MyList::push_front(int val) {
    ListNode* newNode = new ListNode(val, head);
    head = newNode;
    if (tail == nullptr) {
        tail = newNode;
    }
    ++list_size;
}

void MyList::push_back(int val) {
    ListNode* newNode = new ListNode(val);
    if (tail) {
        tail->next = newNode;
        tail = newNode;
    } else {
        head = tail = newNode;
    }
    ++list_size;
}

void MyList::insert(int index, int val) {
    if (index < 0 || index > list_size) return;
    if (index == 0) {
        push_front(val);
    } else if (index == list_size) {
        push_back(val);
    } else {
        ListNode* prev = head;
        for (int i = 0; i < index - 1; ++i) {
            prev = prev->next;
        }
        ListNode* newNode = new ListNode(val, prev->next);
        prev->next = newNode;
        ++list_size;
    }
}

void MyList::remove(int val) {
    if (head == nullptr) return;
    if (head->val == val) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
        --list_size;
        if (head == nullptr) {
            tail = nullptr;
        }
        return;
    }

    ListNode* prev = head;
    ListNode* curr = head->next;
    while (curr) {
        if (curr->val == val) {
            prev->next = curr->next;
            if (curr == tail) {
                tail = prev;
            }
            delete curr;
            --list_size;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void MyList::pop_front() {
    if (head == nullptr) return;
    ListNode* temp = head;
    head = head->next;
    delete temp;
    --list_size;
    if (head == nullptr) {
        tail = nullptr;
    }
}

void MyList::pop_back() {
    if (head == nullptr) return;
    if (head == tail) {
        delete head;
        head = tail = nullptr;
    } else {
        ListNode* prev = head;
        while (prev->next != tail) {
            prev = prev->next;
        }
        delete tail;
        tail = prev;
        tail->next = nullptr;
    }
    --list_size;
}

int MyList::size() const {
    return list_size;
}

bool MyList::empty() const {
    return head == nullptr;
}

void MyList::clear() {
    while (head) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
    head = tail = nullptr;
    list_size = 0;
}

void MyList::print() const {
    ListNode* current = head;
    while (current) {
        std::cout << current->val << " ";
        current = current->next;
    }
    std::cout << std::endl;
}

// 辅助拷贝函数
void MyList::copy_from(const MyList& other) {
    ListNode* current = other.head;
    while (current) {
        push_back(current->val);
        current = current->next;
    }
}

// 重载操作符
MyList& MyList::operator<<(const std::string& str) {
    clear();
    std::istringstream iss(str);
    int val;
    while (iss >> val) {
        push_back(val);
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const MyList& list) {
    ListNode* current = list.head;
    while (current) {
        os << current->val;
        current = current->next;
        if (current) {
           os << " ";
        }
    }
    return os;
}

MyList MyList::operator+(const MyList& other) const {
    MyList result;
    ListNode* current = head;
    while (current) {
        result.push_back(current->val);
        current = current->next;
    }
    current = other.head;
    while (current) {
        result.push_back(current->val);
        current = current->next;
    }
    return result;
}

MyList MyList::operator-() const {
    MyList result;
    ListNode* current = head;
    while (current) {
        result.push_front(current->val);
        current = current->next;
    }
    return result;
}

bool MyList::operator==(const MyList& other) const {
    if (list_size != other.list_size) {
        return false;
    }

    ListNode* current1 = head;
    ListNode* current2 = other.head;
    while (current1 && current2) {
        if (current1->val != current2->val) {
            return false;
        }
        current1 = current1->next;
        current2 = current2->next;
    }
    return current1 == nullptr && current2 == nullptr;
}

int& MyList::operator[](std::size_t index) const {
    ListNode* temp = head;
    while (index-- > 0 && temp != nullptr) {
        temp = temp->next;
    } 
    if (temp == nullptr) {
        throw std::out_of_range("Index out of range");
    }
    return temp->val;
}

// 迭代器类
int& MyList::Iterator::operator*() const {
    return current->val;
}

MyList::Iterator& MyList::Iterator::operator++() {
    current = current->next;
    return *this;
}

MyList::Iterator MyList::Iterator::operator++(int) {// 后置递增
    Iterator temp = *this;
    current = current->next;
    return temp;
}



bool MyList::Iterator::operator!=(const Iterator& other) const {
    return current != other.current;
}

bool MyList::Iterator::operator==(const Iterator& other) const {
    return current == other.current;
}

MyList::Iterator MyList::begin() const {
    return Iterator(head);
}

MyList::Iterator MyList::end() const {
    return Iterator(nullptr);
}

