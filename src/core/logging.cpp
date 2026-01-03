// Copyright 2026 Evanluo
// Licensed under the Apache License, Version 2.0

#include "core/logging.h"
#include <iostream>
#include <iomanip>

namespace evan {
    // 静态实例初始化
    Logger& Logger::getInstance() {
        // 局部静态变量，确保线程安全（C++11及以上）
        static Logger instance;
        return instance;
    }
    
    // 构造函数
    Logger::Logger() : 
        currentLevel(LogLevel::INFO), // 默认日志级别为INFO
        consoleOutput(true) { // 默认输出到控制台
        // 初始化日志文件，默认不打开
    }
    
    // 析构函数
    Logger::~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }
    
    // 设置日志级别
    void Logger::setLogLevel(LogLevel level) {
        currentLevel = level;
    }
    
    // 设置日志文件
    void Logger::setLogFile(const std::string& filename) {
        if (logFile.is_open()) {
            logFile.close();
        }
        
        logFile.open(filename, std::ios::app);
        if (!logFile.is_open()) {
            std::cerr << "Failed to open log file: " << filename << std::endl;
        }
    }
    
    // 日志输出函数
    void Logger::log(LogLevel level, const std::string& message) {
        if (level < currentLevel) {
            return; // 低于当前日志级别，不输出
        }
        
        std::string logMessage = getCurrentTime() + " [" + levelToString(level) + "] " + message;
        
        // 输出到控制台
        if (consoleOutput) {
            // 根据日志级别设置控制台颜色
            switch (level) {
                case LogLevel::DEBUG:
                    std::cout << logMessage << std::endl;
                    break;
                case LogLevel::INFO:
                    std::cout << logMessage << std::endl;
                    break;
                case LogLevel::WARNING:
                    std::cout << "\033[1;33m" << logMessage << "\033[0m" << std::endl; // 黄色
                    break;
                case LogLevel::ERROR:
                    std::cout << "\033[1;31m" << logMessage << "\033[0m" << std::endl; // 红色
                    break;
                case LogLevel::FATAL:
                    std::cout << "\033[1;37;41m" << logMessage << "\033[0m" << std::endl; // 白色背景红色
                    break;
            }
        }
        
        // 输出到文件
        if (logFile.is_open()) {
            logFile << logMessage << std::endl;
            logFile.flush(); // 立即刷新，确保日志及时写入
        }
    }
    
    // 便捷日志函数
    void Logger::debug(const std::string& message) {
        log(LogLevel::DEBUG, message);
    }
    
    void Logger::info(const std::string& message) {
        log(LogLevel::INFO, message);
    }
    
    void Logger::warning(const std::string& message) {
        log(LogLevel::WARNING, message);
    }
    
    void Logger::error(const std::string& message) {
        log(LogLevel::ERROR, message);
    }
    
    void Logger::fatal(const std::string& message) {
        log(LogLevel::FATAL, message);
    }
    
    // 获取当前时间字符串
    std::string Logger::getCurrentTime() const {
        std::time_t now = std::time(nullptr);
        
        // 使用std::localtime，虽然不是线程安全，但在单线程环境下可以使用
        // 或者可以添加互斥锁来保证线程安全
        std::tm* localTimePtr = std::localtime(&now);
        if (localTimePtr == nullptr) {
            return "Unknown Time";
        }
        
        char buffer[30];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTimePtr);
        
        return std::string(buffer);
    }
    
    // 将日志级别转换为字符串
    std::string Logger::levelToString(LogLevel level) const {
        switch (level) {
            case LogLevel::DEBUG:
                return "DEBUG";
            case LogLevel::INFO:
                return "INFO";
            case LogLevel::WARNING:
                return "WARNING";
            case LogLevel::ERROR:
                return "ERROR";
            case LogLevel::FATAL:
                return "FATAL";
            default:
                return "UNKNOWN";
        }
    }
} // namespace evan
