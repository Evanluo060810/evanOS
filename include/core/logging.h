// Copyright 2026 Evanluo
// Licensed under the Apache License, Version 2.0

#pragma once

#include <string>
#include <fstream>
#include <mutex>
#include <ctime>

namespace evan {

// 日志级别枚举
enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

// 日志类
class Logger {
public:
    // 获取单例实例
    static Logger& getInstance();
    
    // 设置日志级别
    void setLogLevel(LogLevel level);
    
    // 设置日志文件
    void setLogFile(const std::string& filename);
    
    // 日志输出函数
    void log(LogLevel level, const std::string& message);
    
    // 便捷日志函数
    void debug(const std::string& message);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
    void fatal(const std::string& message);
    
private:
    // 私有构造函数，防止外部实例化
    Logger();
    
    // 私有析构函数
    ~Logger();
    
    // 禁止复制和赋值
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    // 获取当前时间字符串
    std::string getCurrentTime() const;
    
    // 将日志级别转换为字符串
    std::string levelToString(LogLevel level) const;
    
    // 日志级别
    LogLevel currentLevel;
    
    // 日志文件流
    std::ofstream logFile;
    

    
    // 是否输出到控制台
    bool consoleOutput;
};

// 全局日志宏
#define EVAN_LOG_DEBUG(msg) evan::Logger::getInstance().debug(msg)
#define EVAN_LOG_INFO(msg) evan::Logger::getInstance().info(msg)
#define EVAN_LOG_WARNING(msg) evan::Logger::getInstance().warning(msg)
#define EVAN_LOG_ERROR(msg) evan::Logger::getInstance().error(msg)
#define EVAN_LOG_FATAL(msg) evan::Logger::getInstance().fatal(msg)

} // namespace evan
