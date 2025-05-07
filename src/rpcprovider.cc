#include "rpcprovider.h"
#include "mprpcapplication.h"
#include <google/protobuf/descriptor.h> // 这个头文件里有 ServiceDescriptor

#include "rpcheader.pb.h" // 这个头文件里有 RpcHeader

#include "logger.h" // 日志打印

// 这里是框架提供给外部使用的, 可以发布rpc方法的函数接口
void RpcProvider::NotifyService(google::protobuf::Service *server)
{
    ServiceInfo service_info; // 服务对象信息

    // 获取服务对象描述信息---- 去看看 pb.h 和 pb.cc 这个接口, 是const!!
    const google::protobuf::ServiceDescriptor *pserviceDesc = server->GetDescriptor();

    // 获取服务名称
    std::string service_name = pserviceDesc->name(); // 获取服务名称

    // std::cout << "service_name: " << service_name << std::endl;
    LOG_INFO("service_name: %s", service_name.c_str()); // 打印服务名称

    // 获取服务方法数量
    int method_count = pserviceDesc->method_count(); // 获取服务方法数量

    // 获取服务对象指定下标的方法描述信息
    for (int i = 0; i < method_count; ++i)
    {
        const google::protobuf::MethodDescriptor *pMethodDesc = pserviceDesc->method(i); // 获取服务对象指定下标的方法描述信息

        // 获取方法名称
        std::string method_name = pMethodDesc->name(); // 获取方法名称

        service_info.m_methodMap.insert({method_name, pMethodDesc}); // 将方法名称和方法描述对象的映射关系存入容器

        // std::cout << "method_name: " << method_name << std::endl;
        LOG_INFO("method_name: %s", method_name.c_str()); // 打印方法名称
    }
    service_info.m_service = server; // 将服务对象存入容器
    // 将服务名称和服务对象存入容器
    m_serviceMap.insert({service_name, service_info});
}

// 启动rpc服务发布 节点, 开始提供rpc远程网络调用服务
void RpcProvider::Run()
{
    std::string ip = MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());

    muduo::net::InetAddress addr(ip, port); // 绑定ip和端口号

    // 创建TcpServer对象
    muduo::net::TcpServer server(&m_eventLoop, addr, "RpcProvider");

    // 设置线程数量
    server.setThreadNum(4);

    // 设置连接回调函数
    server.setConnectionCallback(std::bind(&RpcProvider::OnConnection, this, std::placeholders::_1));

    // 设置消息回调函数
    server.setMessageCallback(std::bind(&RpcProvider::OnMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    std::cout << "RpcProvider start service " << std::endl;

    // 启动服务
    server.start();
    // 事件循环
    m_eventLoop.loop();
}

// 实现连接回调函数
void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr &conn)
{
    if (!conn->connected()) // 如果连接断开
    {
        conn->shutdown(); // 关闭连接
        // exit(EXIT_FAILURE);
    }
}

/*
在框架内部, rpcprovider 和 rpcconsumer 协商好 之间的协议, 约定好数据的格式
service_name method_name args
UserServiceLoginzhang san123456   传这样的 肯定不行,需要拆分
定义 proto 的 message 类型, 进行 数据头的 序列化和 反序列化\

加上头部长度
16UserServiceLoginzhang san123456
16 4个字节的头部长度
从 4个子节后 取出 16
UserServiceLoginzhang san123456
*/

// 实现消息回调函数
// 已建立连接用户的 读写事件回调, 如果远程有 rpc服务的 调用请求, 那么onMessage 就会被调用
void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr &conn, // 消息回调函数
                            muduo::net::Buffer *buffer,
                            muduo::Timestamp time)
{
    // 网络上接收的远程rpc调用请求的字符流
    std::string recv_buf = buffer->retrieveAllAsString(); // 获取接收的字符流

    uint32_t header_len = 0;                  // 定义头部长度
    recv_buf.copy((char *)&header_len, 4, 0); // 本项目 自定义 是 4个字节的头部长度

    // 根据头部长度, 获取到数据头的内容
    // recv_buf 是一个字符串, 头部长度是4个字节, 所以从第4个字节开始, 取header_len长度的内容
    std::string rpc_header_str = recv_buf.substr(4, header_len);
    // 反序列化数据, 得到rpc请求的 详细信息

    mprpc::RpcHeader rpc_header;                    // 定义rpc请求头对象
    std::string service_name;                       // 定义服务名称
    std::string method_name;                        // 定义方法名称
    uint32_t args_size = 0;                         // 调用方法所要的参数大小
    if (rpc_header.ParseFromString(rpc_header_str)) // 反序列化数据, 得到rpc请求的 详细信息
    {
        service_name = rpc_header.service_name(); // 获取服务名称
        method_name = rpc_header.method_name();   // 获取方法名称
        args_size = rpc_header.args_size();       // 获取方法参数大小
    }
    else
    {
        std::cout << "rpc_header parse error" << std::endl;
        return;
    }

    // 获取 rpc的方法参数 的字符流数据
    std::string args_str = recv_buf.substr(4 + header_len, recv_buf.size() - 4 - header_len); // 获取方法参数的字符流数据

    // // 打印调试信息
    // std::cout<<"============================="<<std::endl;
    // std::cout<<"header_len: "<<header_len<<std::endl;
    // std::cout<<"rpc_header_str: "<<rpc_header_str<<std::endl;
    // std::cout<<"service_name: "<<service_name<<std::endl;
    // std::cout<<"method_name: "<<method_name<<std::endl;
    // std::cout<<"args_size: "<<args_str.size()<<std::endl;
    // std::cout<<"args_str: "<<args_str<<std::endl;
    // std::cout<<"============================="<<std::endl;

    auto it = m_serviceMap.find(service_name); // 查找服务名称
    if (it == m_serviceMap.end())              // 如果服务名称不存在
    {
        std::cout << "service_name: " << service_name << " not exist" << std::endl;
        return;
    }

    auto mit = it->second.m_methodMap.find(method_name); // 查找方法名称
    if (mit == it->second.m_methodMap.end())             // 如果方法名称不存在
    {
        std::cout << "method_name: " << method_name << " not exist" << std::endl;
        return;
    }

    // 获取服务对象  new UserService
    google::protobuf::Service *service = it->second.m_service; 

    // 获取方法描述对象 Login
    const google::protobuf::MethodDescriptor *method = mit->second;

    // 生成rpc方法调用的 request和response对象
    // 用于 从 request对象中获取参数 传给collee的本地业务--并不知道proto文件message类型指针, 所以用基类 指针 代替
    google::protobuf::Message *request = service->GetRequestPrototype(method).New(); // New 是 生成一个新对象 

    if(!request->ParseFromString(args_str)) // 反序列化数据, 得到rpc请求的 详细信息
    {
        std::cout << "request parse error" << std::endl;
        return;
    }


    // collee的本地业务 需要将结果返回给response对象
    google::protobuf::Message *response = service->GetResponsePrototype(method).New(); 

    // 给下面的method 方法的调用, 绑定一个 Closure 的回调函数
    google::protobuf::Closure *done = google::protobuf::NewCallback<RpcProvider, const muduo::net::TcpConnectionPtr &, google::protobuf::Message *>(this, &RpcProvider::SendRpcResponse, conn, response);

    // 在框架上根据远端rpc调用请求, 调用当前rpc节点上发布的方法
    // 相当于 UserService::Login(request, response, nullptr);
    // 这是框架, 不能写死, 而是 要根据远端rpc调用请求, 动态的调用
    service->CallMethod(method, nullptr, request, response, done); 
    

}

// 设置rpc回调函数 --- closuer 回调操作, 用于 序列化和网络发送
void RpcProvider::SendRpcResponse(const muduo::net::TcpConnectionPtr &conn, // 连接对象
                                  google::protobuf::Message *response) // 响应对象
{
    // 1. 序列化响应对象
    std::string response_str;
    if (!response->SerializeToString(&response_str)) // 序列化数据, 得到rpc请求的 详细信息
    {
        std::cout << "response_str serialize error!" << std::endl;
        conn->shutdown();  
    }
    
    conn->send(response_str);                           // 发送数据
    conn->shutdown();    // 模拟http的短连接, 发送完数据就主动关闭连接
}
