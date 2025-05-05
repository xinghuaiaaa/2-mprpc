#include <iostream>
#include "mprpcapplication.h"
#include "user.pb.h"
#include "mprpcchannel.h"

int main(int argc, char* argv[])
{
    // 1. 初始化框架----必须的, 但只初始化一次
    MprpcApplication::Init(argc, argv);

    hzhrpc::UserServiceRpc_Stub stub(new MpRpcChannel()); // 创建rpc服务对象
    // stub.Login(); // rpcchannel->rpcchannel::callMethod 集中来做所有rpc方法调用的参数序列化和网络发送

    return 0;
}

