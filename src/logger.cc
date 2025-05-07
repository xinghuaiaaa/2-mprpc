#include "logger.h"
#include <time.h>
#include <iostream>

Logger &Logger::GetInstance()
{
    static Logger logger; // 懒汉式单例
    return logger;
}

// 因为是 单例 只有一个对象, 进行一次构造
// 区别 那个 往缓冲区 写日志的 函数
// 这个是 写到文件, 磁盘io
Logger::Logger()
{
    // 启动一个专门的写线程, 进行异步写日志
    std::thread writeThread([&]()
                            {
        for(;;)
        {
            // 获取当前时间, 然后取日志信息, 写入相应的日志文件中
        time_t now = time(nullptr);
        // tm *nowtm = localtime(&now);  
        // 线程安全版本
        tm *nowtm = localtime_r(&now, new tm());

        char fileName[128] = {0};
        sprintf(fileName, "log_%d-%d-%d.txt",
               nowtm->tm_year + 1900, nowtm->tm_mon + 1, nowtm->tm_mday);

        FILE *fp = fopen(fileName, "a+");
        if (fp == nullptr)
        {
            std::cout<<"logger file : "<<fileName<<" open failed!"<<std::endl;
            exit(EXIT_FAILURE);
        }

        std::string logMessage = m_logQueue.Pop(); // 弹出日志信息

        // 把时间也写入
        char timeBuf[64] = {0};
        sprintf(timeBuf, "%d-%d-%d %d:%d:%d ===>[%s]",
                nowtm->tm_year + 1900, nowtm->tm_mon + 1, nowtm->tm_mday,
                nowtm->tm_hour, nowtm->tm_min, nowtm->tm_sec, m_logLevel == INFO ? "INFO" : "ERROR");
        logMessage.insert(0, timeBuf); // 在日志信息前面插入时间
        logMessage.append("\n"); // 添加换行符

        fputs(logMessage.c_str(), fp); // 写入文件

        fclose(fp); // 关闭文件
        } });

    writeThread.detach(); // 分离线程
}

void Logger::SetLogLevel(LogLevel level)
{
    m_logLevel = level;
}

// 把日志信息 写入 缓冲区
void Logger::Log(const std::string &message)
{
    m_logQueue.Push(message); // 异步写日志
}
