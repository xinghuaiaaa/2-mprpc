# 设置cmake最低版本 和 项目名称
cmake_minimum_required(VERSION 3.0)
project(mprpc-hzh)

# set(CMAKE_CXX_STANDARD 11)  # 设置C++标准为C++11
set(CMAKE_BUILD_TYPE "Debug")  # 设置 Debug 模式并开启调试信息：
# -g
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11")  # 设置编译选项



# 设置项目可执行文件输出的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# 设置项目库文件输出的路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
# 设置编译头文件搜索路径 -I
include_directories(${PROJECT_SOURCE_DIR}/src/include)
include_directories(${PROJECT_SOURCE_DIR}/src)
# 设置proto的头文件路径
include_directories(${PROJECT_SOURCE_DIR}/example)


# 设置项目库文件搜索路径 -L
link_directories(${PROJECT_SOURCE_DIR}/lib)

# 包含所有mprpc框架的源文件和头文件
add_subdirectory(src) 
# 放 使用 rpc服务的 使用者 caller和 消费者callee
add_subdirectory(example)   
