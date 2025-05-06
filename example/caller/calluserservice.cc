#include <iostream>
#include "mprpcapplication.h"
#include "user.pb.h"
#include "mprpcchannel.h"

int main(int argc, char* argv[])
{
    // 1. 初始化框架----必须的, 但只初始化一次
    MprpcApplication::Init(argc, argv);

    hzhrpc::UserServiceRpc_Stub stub(new MpRpcChannel()); // 创建rpc服务对象
    hzhrpc::LoginRequest request; // 创建请求对象
    request.set_name("xing huai"); // 设置请求参数
    request.set_pwd("123456"); // 设置请求参数

    hzhrpc::LoginResponse response; // 创建响应对象
    
    stub.Login(nullptr, &request, &response, nullptr); // rpcchannel->rpcchannel::callMethod 集中来做所有rpc方法调用的参数序列化和网络发送

    // 此时, stub.Login()方法已经返回, response对象中已经有了值
    if(response.result().errcode()==0)  // proto生成的messgae成员, 想访问得是函数 
    {
        std::cout << "login success: "<< response.success() << std::endl;
    }
    else
    {
        std::cout << "login failed, errcode: " << response.result().errcode() << ", errmsg: " << response.result().errmsg() << std::endl;
    }


    // 请求调用 该服务的 注册方法
    hzhrpc::RegisterRequest register_request;
    register_request.set_id(10001);
    register_request.set_name("huai xing");
    register_request.set_pwd("123456");
    hzhrpc::RegisterResponse register_response;
    stub.Register(nullptr, &register_request, &register_response, nullptr);
    if(register_response.result().errcode()==0)
    {
        std::cout << "register success: " << register_response.success() << std::endl;
    }
    else
    {
        std::cout << "register failed, errcode: " << register_response.result().errcode() << ", errmsg: " << register_response.result().errmsg() << std::endl;
    }

    return 0;
}

