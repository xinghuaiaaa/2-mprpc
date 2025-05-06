#include "mprpcchannel.h"
#include "google/protobuf/descriptor.h"
#include <google/protobuf/message.h>
#include "rpcheader.pb.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#include "mprpcapplication.h"


/*
服务端收到的数据格式为:
header_size + service_name + method_name + args_size + args
*/
// 重写
void MpRpcChannel::CallMethod(const google::protobuf::MethodDescriptor *method,
                              google::protobuf::RpcController *controller,
                              const google::protobuf::Message *request,
                              google::protobuf::Message *response,
                              google::protobuf::Closure *done)
{
    const google::protobuf::ServiceDescriptor *service = method->service(); // 获取该方法对应的 服务

    std::string service_name = service->name(); // 获取服务名称
    std::string method_name = method->name(); // 获取方法名称

    // 获取请求参数的 序列化后的 字符串的长度
    uint32_t args_size = 0; // 获取请求参数的序列化字符串的长度
    std::string args_str; // 请求参数序列化后的字符串

    if(!request->SerializeToString(&args_str)) // 将请求参数序列化为字符串
    {
        // 序列化失败
        std::cout << "request serialize error" << std::endl;
        return;
    }
    else
    {
        args_size = args_str.size(); // 获取请求参数的序列化字符串的长度
    }

    // 定义rpc请求头header
    mprpc::RpcHeader rpc_header; // 定义rpc请求头
    rpc_header.set_service_name(service_name); // 设置服务名称
    rpc_header.set_method_name(method_name); // 设置方法名称
    rpc_header.set_args_size(args_size); // 设置请求参数的序列化字符串的长度

    std::string rpc_header_str; // 定义rpc请求头的字符串
    uint32_t header_len = 0; // 获取rpc请求头的长度
    if(!rpc_header.SerializeToString(&rpc_header_str)) // 将rpc请求头序列化为字符串
    {
        // 序列化失败
        std::cout << "rpc_header serialize error" << std::endl;
        return;
    }
    else
    {
        header_len = rpc_header_str.size(); // 获取rpc请求头的长度
    }

    // 组织待发送的 rpc 请求字符串
    std::string send_rpc_str; // 定义待发送的字符串
    send_rpc_str.insert(0, (const char *)&header_len, sizeof(uint32_t)); // 插入头部长度
    send_rpc_str += rpc_header_str; // 插入rpc请求头
    send_rpc_str += args_str; // 插入请求参数的序列化字符串

    // 打印调试信息
    std::cout<<"============================="<<std::endl;
    std::cout<<"header_len: "<<header_len<<std::endl;
    std::cout<<"rpc_header_str: "<<rpc_header_str<<std::endl;
    std::cout<<"service_name: "<<service_name<<std::endl;
    std::cout<<"method_name: "<<method_name<<std::endl;
    std::cout<<"args_size: "<<args_size<<std::endl;
    std::cout<<"args_str: "<<args_str<<std::endl;   // 数字在序列化后 是二进制数据
    std::cout<<"============================="<<std::endl;

    // 发送数据
    // 通过网络发送数据---不需要高并发, 相当于 客户端

    int clientfd = socket(AF_INET, SOCK_STREAM, 0); // 创建socket
    if(clientfd == -1) // 创建socket失败
    {
        std::cout << "client socket error" << std::endl;
        exit(EXIT_FAILURE);
    }

    // 获取配置文件对象
    MprpcConfig config = MprpcApplication::GetConfig(); // 获取配置文件对象
    std::string server_ip = config.Load("rpcserverip"); // 获取服务器ip
    uint16_t server_port = atoi(config.Load("rpcserverport").c_str()); // 获取服务器端口号

    // 设置服务器地址
    struct sockaddr_in server_addr; // 定义服务器地址
    memset(&server_addr, 0, sizeof(server_addr)); // 清空服务器地址
    server_addr.sin_family = AF_INET; // 设置地址族
    server_addr.sin_port = htons(server_port); // 设置端口号
    server_addr.sin_addr.s_addr = inet_addr(server_ip.c_str()); // 设置ip地址

    // 连接服务器
    if(connect(clientfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) // 连接服务器失败
    {
        std::cout << "client connect error" << std::endl;
        close(clientfd); // 关闭socket
        exit(EXIT_FAILURE);
    }

    // 连接成功
    int len = send(clientfd, send_rpc_str.c_str(), send_rpc_str.size(), 0); // 发送数据
    if(len == -1) // 发送数据失败
    {
        std::cout << "client send error" << std::endl;
        close(clientfd); // 关闭socket
        exit(EXIT_FAILURE);
    }
    else if(len < (int)send_rpc_str.size()) // 发送数据不完整
    {
        std::cout << "client send incomplete" << std::endl;
        close(clientfd); // 关闭socket
        return; // 直接返回  结束一次 rpc调用
    }
    else
    {
        std::cout << "client send success" << std::endl;
    }

    // 接收数据
    char buffer[1024] = {0}; // 定义接收缓冲区
    memset(buffer, 0, sizeof(buffer)); // 清空接收缓冲区
    int recv_len = recv(clientfd, buffer, sizeof(buffer), 0); // 接收数据
    if(recv_len == -1) // 接收数据失败
    {
        std::cout << "client recv error" << std::endl;
        close(clientfd); // 关闭socket
        exit(EXIT_FAILURE);
    }
    else if(recv_len < (int)sizeof(uint32_t)) // 接收数据不完整
    {
        std::cout << "client recv incomplete" << std::endl;
        close(clientfd); // 关闭socket
        exit(EXIT_FAILURE);
    }
    else
    {
        std::cout << "client recv success" << std::endl;
    }

    // 解析接收数据
    // std::string response_str(buffer, 0, recv_len); // 将接收的数据转换为字符串
    // // 反序列化
    // if(!response->ParseFromString(response_str)) // 直接填入response对象
    if(!response->ParseFromArray(buffer, recv_len))
    {
        // 反序列化失败
        std::cout << "response parse error" << std::endl;
        close(clientfd); // 关闭socket
        return;
    }
    close(clientfd); // 关闭socket
}