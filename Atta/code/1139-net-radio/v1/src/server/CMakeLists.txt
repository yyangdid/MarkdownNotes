# 指定 CMake 的最低版本
cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

# 指定项目名称
project(server C)

# 设置 C 语言版本
set(CMAKE_C_STANDARD 11)

# 追加编译参数
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c11 -pthread")

# 指定头文件目录
include_directories(../include)

# 把项目目录下的所有源码文件赋值给变量 DIR_MAIN
aux_source_directory(. DIR_MAIN)

# 生成可执行文件
add_executable(${PROJECT_NAME} ${DIR_MAIN})
