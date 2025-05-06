#pragma once
#include "lockqueue.h"

enum LogLevel
{
    INFO,
    ERROR,
};

// mprpc框架提供的 日志系统-----单例
class Logger
{
public:
    static Logger &GetInstance();

    // 设置日志级别
    void SetLogLevel(LogLevel level);

    // 写日志
    void Log(const std::string &message);

private:
    int m_logLevel;                    // 日志级别
    LockQueue<std::string> m_logQueue; // 日志缓冲队列

    Logger(); // 构造函数私有化
    Logger(const Logger &) = delete; // 禁止拷贝构造
    Logger(Logger &&) = delete; // 禁止移动构造
    Logger &operator=(const Logger &) = delete; // 禁止赋值构造

};

// 定义宏 使用:  LOG_INFO("hello %s %d", "world", 3);
#define LOG_INFO(logmsgformat, ...) \
    do { \
        Logger &logger = Logger::GetInstance(); \
        Logger.SetLogLevel(INFO); \
        char logmsg[1024] = {0}; \
        snprintf(logmsg, sizeof(logmsg), logmsgformat, ##__VA_ARGS__); \
        logger.Log(logmsg); \
    } while (0)

#define LOG_ERROR(logmsgformat, ...) \
    do { \
        char logmsg[1024] = {0}; \
        snprintf(logmsg, sizeof(logmsg), logformat, ##__VA_ARGS__); \
        Logger::GetInstance().Log(logmsg); \
    } while (0)


