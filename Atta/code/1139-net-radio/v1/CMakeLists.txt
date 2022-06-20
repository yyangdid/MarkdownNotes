# 指定 CMake 的最低版本
cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

# 指定项目名称
project(demo C)

# 设置 C 语言版本
set(CMAKE_C_STANDARD 11)

# 追加编译参数
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c11")

# 指定头文件目录
include_directories(./src/include)

# 把项目目录下的所有源码文件赋值给变量 DIR_MAIN
#aux_source_directory(. DIR_MAIN)

# 把动态链接目录下的所有源码文件赋值给变量 DIR_LIBD
#aux_source_directory(src/libd DIR_LIBD)

#把静态链接目录下的所有源码文件赋值给变量 DIR_LIBS
#aux_source_directory(src/libs DIR_LIBS)

# DIR_LIBD 目录下源码文件生成动态链接库
#add_library(libd SHARED ${DIR_LIBD})

# DIR_LIBS 目录下源码文件生成静态链接库
#add_library(libs STATIC ${DIR_LIBS})

# 设置生成可执行文件目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/${PROJECT_NAME}/bin)

# 设置生成链接库目录
#set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/${PROJECT_NAME}/libs)

# 生成可执行文件
#add_executable(${PROJECT_NAME} ${DIR_MAIN})

# 将动态库和静态库链接到可执行文件
#target_link_libraries(${PROJECT_NAME} libd libs)
#target_link_libraries(${PROJECT_NAME} libd)
#target_link_libraries(${PROJECT_NAME} libs)

# 将 math 库链接到可执行文件
#target_link_libraries(${PROJECT_NAME} m)

# 遍历项目根目录下所有的 .c 文件, 并为每个文件设置编译目标
# 如何在 clion 运行多个 main 函数
# https://blog.cugxuan.cn/2019/11/01/Software/multi-main-func-run-in-clion/
# https://github.com/anonymouss/my-leetcode-cpp/blob/master/CMakeLists.txt
#file(GLOB files *.c)
#foreach (file ${files})
#    string(REGEX REPLACE ".+/(.+)\\..*" "\\1" exe ${file})
#    add_executable(${exe} ${file})
#    message(\ \ \ \ --\ src/${exe}.c\ will\ be\ compiled\ to\ bin/${exe})
#endforeach ()

# 添加子项目
add_subdirectory(src/client)
add_subdirectory(src/server)
