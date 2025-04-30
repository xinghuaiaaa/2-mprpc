#include "test.pb.h"
#include <iostream>
#include <string>
using namespace hzhpro;  // 实际开发 要少用命名空间

int main()
{
    // 封装了login请求对象的数据
    LoginRequest req;
    req.set_name("zhang san");
    req.set_pwd("123123");

    // 对象数据序列化=>char*
    std::string send_buf;   
    if(req.SerializeToString(&send_buf))
    {
        std::cout<< send_buf.c_str()<<std::endl;
    }

    // 从send_buf反序列化
    LoginRequest reqB;
    if(reqB.ParseFromString(send_buf))
    {
        std::cout<<reqB.name()<<std::endl;
        std::cout<<reqB.pwd()<<std::endl;
    }

    return 0;
}