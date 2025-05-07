#include "friendservice.h"
#include "userservice.h"
#include "logger.h"

int main(int argc, char *argv[])
{
    // 日志
    LOG_INFO("main function start");
    LOG_ERROR("%s: %s: %d", __FILE__, __FUNCTION__, __LINE__);


    // 1. 初始化框架
    MprpcApplication::Init(argc, argv);

    // 2. 创建 provider 并注册服务
    RpcProvider provider;
    provider.NotifyService(new FriendService()); // 注册并发布服务, 这里的UserService是个本地服务, 不是rpc服务
    provider.NotifyService(new UserService());

    // 3. 启动rpc服务发布 节点
    provider.Run(); // 启动服务, 这里的Run是阻塞状态, 一直监听rpc调用请求

    return 0;
}