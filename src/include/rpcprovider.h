#pragma one
#include "google/protobuf/service.h" // 这个头文件里有 RpcController, Closure, Service

// 框架提供的专门服务发布 rpc 服务的类对象
class RpcProvider {
    public:

    // 这里是框架提供给外部使用的, 可以发布rpc方法的函数接口
        void NotifyService(google::protobuf::Service *server); // 注册服务  
        // 传参那边是 new UserService() 这个对象, 这里是个指针, 传递给框架, 不能用引用

        // 启动rpc服务发布 节点, 开始提供rpc远程网络调用服务
        void Run(); 
    };
    