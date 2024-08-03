cmake_minimum_required(VERSION 3.10)

# 项目名称
project(CircularQueueApp C)

# 设置 C 标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 添加可执行文件
add_executable(circular_queue_app main.c circular_queue.c)

# 链接线程库（在 Linux 下）
find_package(Threads REQUIRED)
target_link_libraries(circular_queue_app PRIVATE Threads::Threads)

