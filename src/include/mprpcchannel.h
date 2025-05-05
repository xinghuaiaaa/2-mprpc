#pragma once

#include <google/protobuf/service.h>

class MpRpcChannel : public google::protobuf::RpcChannel
{
public:
    // 重写父类的函数
    // 所有通过stub代理对象调用的rpc方法, 都会走到这里, 统一做rpc方法调用的数据序列化和网络发送
    void CallMethod(const google::protobuf::MethodDescriptor *method,
                    google::protobuf::RpcController *controller,
                    const google::protobuf::Message *request,
                    google::protobuf::Message *response,
                    google::protobuf::Closure *done) override;

private:
};