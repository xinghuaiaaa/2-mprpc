#include <iostream>
#include <string>
#include "friend.pb.h"
// using namespace hzhrpcfriend;
#include "mprpcapplication.h"
#include "rpcprovider.h"
#include <vector>

/*
UserService 原来是一个 本地服务, 提供了 两个进程内的本地方法, Login和GetFriendLists
*/
class FriendService : public hzhrpcfriend::FriendServiceRpc // 这个rpc类使用在 rpc服务发布端(rpc服务提供者)
{
public:

    // 本地服务函数-返回好友列表
    std::vector<std::string> GetFriendLists(uint32_t id)
    {
        std::cout<<"GetFriendLists: " << id << std::endl;
        std::vector<std::string> friends;
        friends.push_back("zhang san");
        friends.push_back("li si");
        friends.push_back("wang wu");
        return friends;
    }



    
    void GetFriendList_rpc(::google::protobuf::RpcController *controller, // 这里命名空间不写hzhrpcfriend::, 直接写google::protobuf   特别注意
               const ::hzhrpcfriend::FriendListRequest *request,
               ::hzhrpcfriend::FriendListResponse *response,
               ::google::protobuf::Closure *done)
    {

        // 1. 框架给业务上报了请求参数LoginRequest, 应用 获取相应数据做本地业务
        uint32_t id = request->id(); // 获取请求参数

        // 2. 做本地业务
        std::vector<std::string> friends = GetFriendLists(id); // 获取好友列表

        // 3. 业务处理完毕, 设置响应参数
        // 包括 设置返回码, 提示信息, 以及业务处理结果
        hzhrpcfriend::ResultCode *code = response->mutable_result(); // 可修改的
        code->set_errcode(0);                                  // 0表示成功   -- 这里有问题, 不能根据业务处理结果来设置返回码, 需要根据业务处理结果来设置返回码,  否则, 一直是登录成功
        code->set_errmsg("friend list error");                       // 成功的提示信息   涉及错误复现--笔记
        for(auto &friend_name : friends) // 遍历好友列表
        {
            response->add_friends(friend_name); // 添加好友列表
        }



        // 4. 执行回调函数
        done->Run();
    }

    
};

