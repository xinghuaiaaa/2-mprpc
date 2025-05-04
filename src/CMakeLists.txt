aux_source_directory(. SRC_LIST)
add_library(mprpc ${SRC_LIST})  # 生成动态库

# 添加 muduo 编译选项
target_link_libraries(mprpc  muduo_net muduo_base pthread)

