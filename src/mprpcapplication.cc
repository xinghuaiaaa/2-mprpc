#include "include/mprpcapplication.h"  // 这个头文件里有 Init函数
#include <iostream>  
#include <string>
#include <unistd.h>  

MprpcConfig MprpcApplication::m_config;  // 静态成员变量, 需要在类外初始化

void ShowArgsHelp()
{
    std::cout << "error: format: command -i <config_file>" << std::endl;
}

void MprpcApplication::Init(int argc, char **argv)
{
    if (argc < 3)
    {
        // std::cout << "error: argc < 3" << std::endl;
        // exit(1);
        ShowArgsHelp();
        exit(EXIT_FAILURE); // EXIT_FAILURE 是一个宏，表示程序异常退出
    }

    std::string config_file; // 配置文件
    // 解析命令行参数
    int c = 0;
    while ((c = getopt(argc, argv, "i:")) != -1) // getopt函数解析命令行参数
    {
        switch (c)
        {
        case 'i':
            config_file = optarg; // optarg是一个全局变量，存储当前选项的参数
            break;
        case '?':  // '?'表示没有找到对应的选项
            ShowArgsHelp();
            exit(EXIT_FAILURE);
        case ':':  // ':'表示选项缺少参数
            ShowArgsHelp();
            exit(EXIT_FAILURE);
        default:
            ShowArgsHelp();
            exit(EXIT_FAILURE);
        }

        m_config.LoadConfigFile(config_file.c_str()); // 加载配置文件, config_file是一个std::string类型的变量, 文件名字
        
        // std::cout<<"rpcserverip:"<<m_config.Load("rpcserverip")<<std::endl;
        // std::cout<<"rpcserverport:"<<m_config.Load("rpcserverport")<<std::endl;
        // std::cout<<"zookeeperip:"<<m_config.Load("zookeeperip")<<std::endl;
        // std::cout<<"zookeeperport:"<<m_config.Load("zookeeperport")<<std::endl;
       
    } 

    // 读取配置文件----单独写 .h和.cc文件----解耦-且 代码 也不是很少
    // rpcserver_ip   rpcserver_port  zookeeper_ip zookeeper_port 

}


MprpcApplication& MprpcApplication::GetInstance()
{
    static MprpcApplication app;  // 静态局部变量, 只会被初始化一次
    return app;
}