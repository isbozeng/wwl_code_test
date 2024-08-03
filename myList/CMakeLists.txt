cmake_minimum_required(VERSION 3.0)
project(MyListLib)

# 设置 C++ 标准为 C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加动态库的头文件和源文件
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(myList SHARED myList.cpp)

# 设置输出库的名字
# set_target_properties(MyList PROPERTIES OUTPUT_NAME "mylistlib")
# 指定安装路径（可选）
# 如果需要将生成的动态库安装到系统目录，可以取消注释下面的行，并根据需要修改路径
# install(TARGETS myList LIBRARY DESTINATION /usr/local/lib)

# 如果需要安装头文件，取消下面的注释并修改路径
# install(FILES myList.h DESTINATION /usr/local/include)
