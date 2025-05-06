
# set(SRC_LIST userservice.cc ../user.pb.cc)
# add_executable(provider ${SRC_LIST})
# target_link_libraries(provider mprpc protobuf)  # 链接动态库

# set(FRIEND_LIST friendservice.cc ../friend.pb.cc)
# add_executable(friendlist ${FRIEND_LIST})
# target_link_libraries(friendlist mprpc protobuf)  # 链接动态库

set(SRC_LIST main.cc ../user.pb.cc ../friend.pb.cc)
add_executable(provider ${SRC_LIST})
target_link_libraries(provider mprpc protobuf)  # 链接动态库
