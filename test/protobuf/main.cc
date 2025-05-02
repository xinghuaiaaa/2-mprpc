#include "test.pb.h"
#include <iostream>
#include <string>
using namespace hzhpro;  // 实际开发 要少用命名空间


int main1()
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

int main()
{
    // LoginResponse rsp;
    // ResultCode *rc = rsp.mutable_result();
    // rc->set_errcode(1);
    // rc->set_errmsg("登录处理失败");

    GetFriendListsResponse rsp;
    ResultCode *rc = rsp.mutable_result();
    rc->set_errcode(0);
    User *user1 = rsp.add_friend_list();
    user1->set_name("zhang san");
    user1->set_age(26);
    user1->set_sex(User::MAN);

    User *user2 = rsp.add_friend_list();
    user2->set_name("zhang san-2");
    user2->set_age(26);
    user2->set_sex(User::MAN);

    User *user3 = rsp.add_friend_list();
    user3->set_name("zhang san-3");
    user3->set_age(26);
    user3->set_sex(User::WOMAN);
    
    std::cout<<rsp.friend_list_size()<<std::endl;

    User user = rsp.friend_list(2);
    std::string userstr;
    if(user.SerializeToString(&userstr))
    {
        // std::cout<<userstr.c_str()<<std::endl;// 这个有问题, 序列化后是二进制数据流, 本身是 字符串能打印出来, 要是有别的 类型, 就不好说了
    }

    User userB;
    if(userB.ParseFromString(userstr))
    {
        std::cout<<userB.name()<<std::endl;
        std::cout<<userB.age()<<std::endl;
        std::cout<<userB.sex()<<std::endl;
    }

    return 0;
}