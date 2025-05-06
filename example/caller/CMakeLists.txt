
set(SRC_LIST calluserservice.cc ../user.pb.cc)
add_executable(consumer ${SRC_LIST})
target_link_libraries(consumer mprpc protobuf)  # 链接动态库

set(FRIEND_LIST callfriendservice.cc ../friend.pb.cc)
add_executable(friend_consumer ${FRIEND_LIST})
target_link_libraries(friend_consumer mprpc protobuf)  # 链接动态库
