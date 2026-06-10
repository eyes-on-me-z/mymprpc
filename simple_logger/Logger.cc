#include "Logger.h"

#include <iostream>

Logger::Logger()
{
    // 启动专门的写日志线程
    std::thread writeLogTask([&](){
        for (;;)
        {
            // 获取当前的日期，然后取日志信息，写入相应的日志文件当中 a+
            time_t now = time(nullptr);
            tm *nowtm = localtime(&now);

            char fileName[128] = {0};
            sprintf(fileName, "%d-%d-%d-log.txt", nowtm->tm_year + 1900,
                                                nowtm->tm_mon + 1,
                                                nowtm->tm_mday);

            FILE *pf = fopen(fileName, "a+");
            if (pf == nullptr)
            {
                std::cout << "logger file: " << fileName << "open error!" << std::endl;
                exit(EXIT_FAILURE);
            }

            std::string msg = _lockQueue.pop();

            char timeBuf[128] = {0};
            sprintf(timeBuf, "%d:%d:%d =>[%s]",
                nowtm->tm_hour, nowtm->tm_min, nowtm->tm_sec, (_level == INFO ? "INFO" : "ERROR"));

            msg .insert(0, timeBuf);
            msg.append("\n");

            fputs(msg.c_str(), pf);
            fclose(pf);
        }
    });
    // 设置分离线程，守护线程
    writeLogTask.detach();
}

// 获取日志的单例
Logger& Logger::getInstance()
{
    static Logger logger;
    return logger;
}

// 设置日志级别 
void Logger::setLogLevel(LogLevel level)
{
    _level = level;
}

// 写日志, 把日志信息写入lockqueue缓冲区当中
void Logger::log(std::string msg)
{
    _lockQueue.push(msg);
}