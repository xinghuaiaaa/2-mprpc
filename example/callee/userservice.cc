#include <iostream>
#include <string>
#include "user.pb.h"
// using namespace hzhrpc;
#include "mprpcapplication.h"
#include "rpcprovider.h"
/*
UserService 原来是一个 本地服务, 提供了 两个进程内的本地方法, Login和GetFriendLists
*/
class UserService : public hzhrpc::UserServiceRpc  // 这个rpc类使用在 rpc服务发布端(rpc服务提供者) 
{
    public:
    bool  Login(const std::string username, const std::string password) 
    {
        std::cout << "doing local service : Login" << std::endl;
        std::cout<<"username: " << username << " password: " << password << std::endl;
        return true;
    }


    // 重写  UserServiceRpc 里的 Login方法
    // 1. caller ====> 发起请请求 Login(request)  ==> 通过 muduo  ==> callee
    // 2. callee ====> 收到请求 Login(request)  ==> 交到下面重写的 Login方法
    void Login(::google::protobuf::RpcController* controller,   // 这里命名空间不写hzhrpc::, 直接写google::protobuf   特别注意
        const ::hzhrpc::LoginRequest* request,
        ::hzhrpc::LoginResponse* response,
        ::google::protobuf::Closure* done)
        {

            // 1. 框架给业务上报了请求参数LoginRequest, 应用 获取相应数据做本地业务
            std::string username = request->name();
            std::string password = request->pwd();


            // 2. 做本地业务
            bool login_result = Login(username, password); 

            // 3. 业务处理完毕, 设置响应参数
            // 包括 设置返回码, 提示信息, 以及业务处理结果
            hzhrpc::ResultCode* code = response->mutable_result();  // 可修改的
            code->set_errcode(0);  // 0表示成功
            code->set_errmsg("success");  // 成功的提示信息
            response->set_success(login_result);  // 设置成功与否的标志

            // 4. 执行回调函数
            // 通过查看 Closure 类, run是个 纯虚函数, 即回调函数, 需要重写
            // 这里的done是个回调函数, 由框架提供, 业务上不需要关心
            // 任务: 执行响应对象数据的序列化和网络发送 (都是由 框架来完成的)
           done->Run();  

        }
};

int main(int argc, char* argv[])
{
    // 1. 初始化框架
    MprpcApplication::Init(argc, argv);

    // 2. 创建 provider 并注册服务
    RpcProvider provider;
    provider.NotifyService(new UserService());  // 注册并发布服务, 这里的UserService是个本地服务, 不是rpc服务

    // 3. 启动rpc服务发布 节点
    provider.Run();  // 启动服务, 这里的Run是阻塞状态, 一直监听rpc调用请求

    return 0;
}