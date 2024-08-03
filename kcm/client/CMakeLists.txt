# CMake 最低版本要求
cmake_minimum_required(VERSION 3.0)

# 项目名称和语言
project(MykcmApp C)

# 添加可执行文件
add_executable(mykcm_app client.c shm_rwlock.c)

# 链接pthread库，因为使用了多线程的函数pthread_rwlock_*
find_package(Threads REQUIRED)
target_link_libraries(mykcm_app PRIVATE rt Threads::Threads)

