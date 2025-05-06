#include <iostream>
#include "mprpcapplication.h"
#include "friend.pb.h"
#include "mprpcchannel.h"

int main(int argc, char* argv[])
{
    // 1. 初始化框架----必须的, 但只初始化一次
    MprpcApplication::Init(argc, argv);

    hzhrpcfriend::FriendServiceRpc_Stub stub(new MpRpcChannel()); // 创建rpc服务对象
    hzhrpcfriend::FriendListRequest request; // 创建请求对象
    request.set_id(10001); // 设置请求参数

    hzhrpcfriend::FriendListResponse response; // 创建响应对象
    
    stub.GetFriendList_rpc(nullptr, &request, &response, nullptr); // rpcchannel->rpcchannel::callMethod 集中来做所有rpc方法调用的参数序列化和网络发送

    // 此时, stub.Login()方法已经返回, response对象中已经有了值
    if(response.result().errcode()==0)  // proto生成的messgae成员, 想访问得是函数 
    {
        std::cout << "friend list success: "  <<std::endl;
        for(int i=0; i<response.friends_size(); ++i)
        {
            std::cout << response.friends(i) << std::endl;
        }
    }
    else
    {
        std::cout << "friend list failed, errcode: " << response.result().errcode() << ", errmsg: " << response.result().errmsg() << std::endl;
    }


    return 0;
}

