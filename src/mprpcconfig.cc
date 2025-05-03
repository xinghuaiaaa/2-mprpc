#include "mprpcconfig.h"
#include <iostream>

// 负责解析加载配置文件
void MprpcConfig::LoadConfigFile(const char *config_file)
{
    FILE *pf = fopen(config_file, "r");
    if (pf == nullptr)
    {
        std::cout << "error: config file is not exist" << std::endl;
        exit(EXIT_FAILURE);
    }

    while (!feof(pf)) // feof函数判断文件是否到达末尾
    {
        char buffer[512] = {0};            // 定义一个字符数组, 用于存储一行数据
        fgets(buffer, sizeof(buffer), pf); // 读取一行数据

        // 去掉多余空格
        std::string read_buf(buffer); // 将字符数组转换为字符串
        Trim(read_buf);              // 去掉前后空格

        // 去掉注释
        if (read_buf[0] == '#' || read_buf.empty()) // 如果是注释或空行
        {
            continue; // 跳过 这一行
        }

        // 解析配置项
        int idx = read_buf.find('='); // 查找第一个等号的位置
        if (idx != std::string::npos)
        {
            std::string key = read_buf.substr(0, idx);                              // 截取键
            Trim(key); // 去掉前后空格

            // 先去\n
            int endidx = read_buf.find_last_not_of("\r\n", read_buf.size()-1); // 查找最后一个不是回车或换行的位置
            std::string value;
            if (endidx != std::string::npos)
            {
                value = read_buf.substr(idx+1, endidx-idx); // 截取字符串
            }
            Trim(value); // 再去掉前后空格

            // 下面这段不对, \n本身就是最后一个, 要是 \n之前紧挨着空格呢
            /*
            // 还有换行
            idx = value.find_last_not_of("\r\n"); // 查找最后一个不是回车或换行的位置
            if (idx != std::string::npos)
            {
                value = value.substr(0, idx); // 截取值
            }
            */

            // 存储配置项
            m_configMap.insert({key, value}); // 将键值对插入到map中
        }
    }
}

// 查询配置项
std::string MprpcConfig::Load(const std::string &key)
{
    // return m_configMap[key]; // 错误的, 不要用中括号, 不存在 会自动插入一个空值

    auto it = m_configMap.find(key); // 查找键
    if (it == m_configMap.end())     // 如果没有找到
    {
        std::cout << "error: key is not exist" << std::endl;
        return "";
    }
    return it->second; // 返回值
}

// 去除前后空格
void MprpcConfig::Trim(std::string &src_buf)
{
    int idx = src_buf.find_first_not_of(" "); // 查找第一个不是空格的位置
    if (idx != std::string::npos)
    {
        src_buf = src_buf.substr(idx, src_buf.size() - idx); // 截取字符串
    }
    idx = src_buf.find_last_not_of(" "); // 查找最后一个不是空格的位置
    if (idx != std::string::npos)
    {
        src_buf = src_buf.substr(0, idx + 1); // 截取字符串
    }
}
