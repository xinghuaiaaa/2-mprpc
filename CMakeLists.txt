# 设置cmake最低版本 和 项目名称
cmake_minimum_required(VERSION 3.0)
project(mprpc-hzh)

# 设置项目可执行文件输出的路径
set(EXECUTABLE_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH}/bin)
# 设置项目库文件输出的路径
set(LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_DIRECTORY}/lib)
# 设置编译头文件搜索路径 -I
include_directories(${PROJECT_SOURCE_DIR}/src/include)
# 设置proto生成的头文件路径
include_directories(${PROJECT_SOURCE_DIR}/example)


# 设置项目库文件搜索路径 -L
link_directories(${PROJECT_SOURCE_DIR}/lib)

# 包含所有mprpc框架的源文件和头文件
add_subdirectory(src) 
# 放 使用 rpc服务的 使用者 caller和 消费者callee
add_subdirectory(example)   
