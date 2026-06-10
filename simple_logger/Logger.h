#pragma once

#include "LockQueue.h"

#include <string>
#include <thread>

// 定义宏 LOG_INFO("xxx %d %s", 20, "xxxx")
#define LOG_INFO(logMsgFormat, ...) \
    do  \
    {   \
        Logger::getInstance().setLogLevel(INFO); \
        char c[1024] = {0}; \
        snprintf(c, sizeof(c), logMsgFormat, ##__VA_ARGS__); \
        Logger::getInstance().log(c); \
    } while (0) \

#define LOG_ERROR(logMsgFormat, ...) \
    do  \
    {   \
        Logger::getInstance().setLogLevel(ERROR); \
        char c[1024] = {0}; \
        snprintf(c, sizeof(c), logMsgFormat, ##__VA_ARGS__); \
        Logger::getInstance().log(c); \
    } while (0) \
    

// 定义日志级别
enum LogLevel
{
    INFO,   // 普通信息
    ERROR,  // 错误信息
};

// Mprpc框架提供的日志系统
class Logger
{
public:
    // 获取日志的单例
    static Logger& getInstance();
    // 设置日志级别 
    void setLogLevel(LogLevel level);
    // 写日志
    void log(std::string);
private:
    Logger();
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;

    LockQueue<std::string> _lockQueue;  // 日志缓冲队列
    LogLevel _level;    // 记录日志级别
};