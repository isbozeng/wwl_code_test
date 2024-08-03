cmake_minimum_required(VERSION 3.0)
project(MyListTest)

# 设置 C++ 标准为 C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Google Test 包
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
# 添加 myList 的头文件路径
include_directories(${PROJECT_SOURCE_DIR})
# 添加 myList 动态库
# 假设动态库 myList.so 位于 build 目录中
# 在实际构建过程中，可能需要根据具体路径调整
link_directories(${CMAKE_SOURCE_DIR})
# 添加测试源文件
add_executable(MyListTest myListTest.cpp)

# 链接 Google Test 和动态库
target_link_libraries(MyListTest ${GTEST_LIBRARIES} pthread myList)
