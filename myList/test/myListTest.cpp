#include <gtest/gtest.h>
#include "myList.h"

// 测试 MyList 的构造函数
TEST(MyListTest, DefaultConstructor) {
    MyList list;
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);
}

TEST(MyListTest, StringConstructor) {
    MyList list("1 2 3 4 5");
    EXPECT_EQ(list.size(), 5);
    EXPECT_FALSE(list.empty());
    std::ostringstream oss;
    oss << list;
    EXPECT_EQ(oss.str(), "1 2 3 4 5 ");
}

TEST(MyListTest, CopyConstructor) {
    MyList original("1 2 3");
    MyList copy(original);
    EXPECT_EQ(copy.size(), 3);
    EXPECT_FALSE(copy.empty());
    std::ostringstream oss;
    oss << copy;
    EXPECT_EQ(oss.str(), "1 2 3 ");
}

TEST(MyListTest, MoveConstructor) {
    MyList original("1 2 3");
    MyList moved(std::move(original));
    EXPECT_EQ(moved.size(), 3);
    EXPECT_TRUE(original.empty());
    EXPECT_EQ(moved.size(), 3);
    std::ostringstream oss;
    oss << moved;
    EXPECT_EQ(oss.str(), "1 2 3 ");
}

// 测试 MyList 的拷贝赋值运算符
TEST(MyListTest, CopyAssignment) {
    MyList list1("1 2 3");
    MyList list2;
    list2 = list1;
    EXPECT_EQ(list2.size(), 3);
    EXPECT_FALSE(list2.empty());
    std::ostringstream oss;
    oss << list2;
    EXPECT_EQ(oss.str(), "1 2 3 ");
}

// 测试 MyList 的移动赋值运算符
TEST(MyListTest, MoveAssignment) {
    MyList list1("1 2 3");
    MyList list2;
    list2 = std::move(list1);
    EXPECT_TRUE(list1.empty());
    EXPECT_EQ(list2.size(), 3);
    std::ostringstream oss;
    oss << list2;
    EXPECT_EQ(oss.str(), "1 2 3 ");
}

// 测试 push_back 和 push_front
TEST(MyListTest, PushOperations) {
    MyList list;
    list.push_back(1);
    list.push_back(2);
    list.push_front(0);
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.begin().operator*(), 0);  // 检查头节点
    EXPECT_EQ((++list.begin()).operator*(), 1);  // 检查第二个节点
}

// 测试 insert 和 remove
TEST(MyListTest, InsertAndRemove) {
    MyList list("1 3 4");
    list.insert(1, 2);  // 插入 2 在索引 1
    EXPECT_EQ(list.size(), 4);
    std::ostringstream oss;
    oss << list;
    EXPECT_EQ(oss.str(), "1 2 3 4 ");

    list.remove(2);
    EXPECT_EQ(list.size(), 3);
    oss.str("");  // 清空ostringstream
    oss << list;
    EXPECT_EQ(oss.str(), "1 3 4 ");
}

// 测试 pop_front 和 pop_back
TEST(MyListTest, PopOperations) {
    MyList list("1 2 3 4");
    list.pop_front();
    EXPECT_EQ(list.size(), 3);
    std::ostringstream oss;
    oss << list;
    EXPECT_EQ(oss.str(), "2 3 4 ");

    list.pop_back();
    EXPECT_EQ(list.size(), 2);
    oss.str("");  // 清空ostringstream
    oss << list;
    EXPECT_EQ(oss.str(), "2 3 ");
}

// 测试 == 操作符
TEST(MyListTest, EqualityOperator) {
    MyList list1("1 2 3");
    MyList list2("1 2 3");
    MyList list3("1 2 4");
    MyList list4("1 2 3 4");

    EXPECT_TRUE(list1 == list2);
    EXPECT_FALSE(list1 == list3);
    EXPECT_FALSE(list1 == list4);
}

// 测试 + 运算符
TEST(MyListTest, PlusOperator) {
    MyList list1("1 2 3");
    MyList list2("4 5 6");
    MyList result = list1 + list2;
    EXPECT_EQ(result.size(), 6);
    std::ostringstream oss;
    oss << result;
    EXPECT_EQ(oss.str(), "1 2 3 4 5 6 ");
}

// 测试 - 运算符
TEST(MyListTest, MinusOperator) {
    MyList list("1 2 3 4 5");
    MyList result = -list;
    EXPECT_EQ(result.size(), 5);
    std::ostringstream oss;
    oss << result;
    EXPECT_EQ(oss.str(), "5 4 3 2 1 ");
}

// 测试迭代器
TEST(MyListTest, Iterator) {
    MyList list("1 2 3 4");
    int index = 1;
    for (auto it = list.begin(); it != list.end(); ++it) {
        EXPECT_EQ(*it, index++);
    }
}

// 测试空链表
TEST(MyListTest, EmptyListOperations) {
    MyList list;
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);

    // 测试对空链表的 pop_front 和 pop_back 操作
    EXPECT_NO_THROW(list.pop_front());
    EXPECT_NO_THROW(list.pop_back());

    // 测试迭代器对空链表的行为
    auto it = list.begin();
    EXPECT_EQ(it, list.end());
}

// 测试超出范围的索引
TEST(MyListTest, OutOfBoundsAccess) {
    MyList list("1 2 3");
    EXPECT_THROW(list[-1], std::out_of_range); // 负索引
    EXPECT_THROW(list[3], std::out_of_range); // 超过范围
}

// 性能测试
TEST(MyListTest, PerformanceTest) {
    MyList list;
    for (int i = 0; i < 10000; ++i) {
        list.push_back(i);
    }
    EXPECT_EQ(list.size(), 10000);

    MyList another_list = list;
    EXPECT_EQ(another_list.size(), 10000);
}

// 自定义迭代器测试
TEST(MyListTest, IteratorIntegrity) {
    MyList list("1 2 3 4");
    auto it = list.begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_EQ(*it, 3);
    ++it;
    EXPECT_EQ(*it, 4);
    ++it;
    EXPECT_EQ(it, list.end());
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
