#pragma once

#include "mprpcconfig.h"

// mprpc框架的基础类, 负责框架的初始化工作
class MprpcApplication {
    public:
        static void Init(int argc, char **argv);
        static MprpcApplication& GetInstance();
       
    
    private:
        MprpcApplication() {}                         // 禁止外部构造
        MprpcApplication(const MprpcApplication&) = delete; // 禁止拷贝构造
        MprpcApplication& operator=(const MprpcApplication&) = delete; // 禁止赋值构造

        static MprpcConfig m_config; // 配置文件对象
    
    };
    