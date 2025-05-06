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

    bool Register(const uint32_t id, const std::string username, const std::string password) 
    {
        // 仅演示
        std::cout << "doing local service : Register" << std::endl;
        std::cout << "id: " << id << " username: " << username << " password: " << password << std::endl;
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
            code->set_errcode(0);  // 0表示成功   -- 这里有问题, 不能根据业务处理结果来设置返回码, 需要根据业务处理结果来设置返回码,  否则, 一直是登录成功
            code->set_errmsg("login error");  // 成功的提示信息   涉及错误复现--笔记
            response->set_success(login_result);  // 设置成功与否的标志

            // 4. 执行回调函数
            // 通过查看 Closure 类, run是个 纯虚函数, 即回调函数, 需要重写
            // 这里的done是个回调函数, 由框架提供, 业务上不需要关心
            // 任务: 执行响应对象数据的序列化和网络发送 (都是由 框架来完成的)
           done->Run();  

        }



    // 重写 UserServiceRpc 里的 Register方法
    void Register(::google::protobuf::RpcController* controller,
        const ::hzhrpc::RegisterRequest* request,
        ::hzhrpc::RegisterResponse* response,
        ::google::protobuf::Closure* done)
        {
            // 1. 框架给业务上报了请求参数RegisterRequest, 应用 获取相应数据做本地业务
            uint32_t id = request->id();
            std::string username = request->name();
            std::string password = request->pwd();

            // 2. 做本地业务
            bool register_result = Register(id, username, password); 

            // 3. 业务处理完毕, 设置响应参数
            // 包括 设置返回码, 提示信息, 以及业务处理结果
            hzhrpc::ResultCode* code = response->mutable_result();  // 可修改的
            code->set_errcode(0);  // 0表示成功   -- 这里有问题, 不能根据业务处理结果来设置返回码, 需要根据业务处理结果来设置返回码,  否则, 一直是登录成功
            code->set_errmsg("register error");  // 成功的提示信息   涉及错误复现--笔记
            response->set_success(register_result);  // 设置成功与否的标志

            // 4. 执行回调函数
           done->Run();  
        }
};

