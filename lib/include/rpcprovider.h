#pragma once
#include "google/protobuf/service.h" // 这个头文件里有 RpcController, Closure, Service

#include <memory>                // shared_ptr
#include <muduo/net/TcpServer.h> // muduo库的TcpServer
#include <muduo/net/EventLoop.h> // muduo库的EventLoop
#include <muduo/net/InetAddress.h> // muduo库的InetAddress
#include <muduo/net/TcpConnection.h> // muduo库的TcpConnection

// 框架提供的专门服务发布 rpc 服务的类对象
class RpcProvider
{
public:
    // 这里是框架提供给外部使用的, 可以发布rpc方法的函数接口
    void NotifyService(google::protobuf::Service *server); // 注册服务
    // 传参那边是 new UserService() 这个对象, 这里是个指针, 传递给框架, 不能用引用

    // 启动rpc服务发布 节点, 开始提供rpc远程网络调用服务
    void Run();

private:
    // // 组合TcpServer ----- 不写成成员了, 只有 run 会访问
    // std::unique_ptr<muduo::net::TcpServer> m_pTcpServer; // 智能指针
    // 组合 EventLoop
    muduo::net::EventLoop m_eventLoop; // 事件循环

    void OnConnection(const muduo::net::TcpConnectionPtr &conn); // 连接回调函数

    void OnMessage(const muduo::net::TcpConnectionPtr &conn, // 消息回调函数
                   muduo::net::Buffer *buffer,
                   muduo::Timestamp time);
    // service 服务类型信息----服务名及方法名
    struct ServiceInfo
    {
        google::protobuf::Service *m_service; // 服务对象
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor *> m_methodMap; // 方法名和方法描述对象的映射关系

    };
    
    // 存储服务对象的容器
    std::unordered_map<std::string, ServiceInfo> m_serviceMap; // 服务名和服务对象的映射关系

    // 设置rpc回调函数 --- closuer 回调操作, 用于 序列化和网络发送
    void SendRpcResponse(const muduo::net::TcpConnectionPtr &conn, // 连接对象
                         google::protobuf::Message *response); // 响应对象
};
