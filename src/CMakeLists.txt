aux_source_directory(. SRC_LIST)
set(SRC_LIST mprpcconfig.cc mprpcapplication.cc rpcprovider.cc)  # 添加源文件列表
add_library(mprpc SHARED ${SRC_LIST})  # 生成动态库