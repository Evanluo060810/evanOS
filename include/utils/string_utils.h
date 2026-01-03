// Copyright 2026 Evanluo
// Licensed under the Apache License, Version 2.0

#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

namespace evan {
namespace utils {

/**
 * 字符串工具类
 * 提供各种字符串处理功能
 */
class StringUtils {
public:
    /**
     * 将字符串转换为小写
     * @param str 输入字符串
     * @return 转换后的小写字符串
     */
    static std::string toLower(const std::string& str);
    
    /**
     * 将字符串转换为大写
     * @param str 输入字符串
     * @return 转换后的大写字符串
     */
    static std::string toUpper(const std::string& str);
    
    /**
     * 去除字符串首尾空白字符
     * @param str 输入字符串
     * @return 处理后的字符串
     */
    static std::string trim(const std::string& str);
    
    /**
     * 按分隔符分割字符串
     * @param str 输入字符串
     * @param delimiter 分隔符
     * @return 分割后的字符串向量
     */
    static std::vector<std::string> split(const std::string& str, char delimiter);
    
    /**
     * 按分隔符分割字符串
     * @param str 输入字符串
     * @param delimiter 分隔符字符串
     * @return 分割后的字符串向量
     */
    static std::vector<std::string> split(const std::string& str, const std::string& delimiter);
    
    /**
     * 连接字符串向量
     * @param vec 字符串向量
     * @param delimiter 连接符
     * @return 连接后的字符串
     */
    static std::string join(const std::vector<std::string>& vec, const std::string& delimiter);
    
    /**
     * 替换字符串中的子串
     * @param str 原始字符串
     * @param from 要替换的子串
     * @param to 替换后的子串
     * @return 替换后的字符串
     */
    static std::string replace(const std::string& str, const std::string& from, const std::string& to);
    
    /**
     * 检查字符串是否以指定前缀开头
     * @param str 输入字符串
     * @param prefix 前缀
     * @return 是否以指定前缀开头
     */
    static bool startsWith(const std::string& str, const std::string& prefix);
    
    /**
     * 检查字符串是否以指定后缀结尾
     * @param str 输入字符串
     * @param suffix 后缀
     * @return 是否以指定后缀结尾
     */
    static bool endsWith(const std::string& str, const std::string& suffix);
    
    /**
     * 检查字符串是否包含指定子串
     * @param str 输入字符串
     * @param substr 子串
     * @return 是否包含指定子串
     */
    static bool contains(const std::string& str, const std::string& substr);
    
    /**
     * 将数值转换为字符串
     * @param value 数值
     * @return 转换后的字符串
     */
    template <typename T>
    static std::string toString(T value);
    
    /**
     * 将字符串转换为数值
     * @param str 字符串
     * @param defaultValue 默认值
     * @return 转换后的数值
     */
    template <typename T>
    static T toNumber(const std::string& str, T defaultValue = T());
    
    /**
     * 格式化字符串
     * @param format 格式字符串
     * @param args 可变参数
     * @return 格式化后的字符串
     */
    template <typename... Args>
    static std::string format(const std::string& format, Args&&... args);
};

// 模板方法实现

/**
 * 将数值转换为字符串
 * @param value 数值
 * @return 转换后的字符串
 */
template <typename T>
std::string StringUtils::toString(T value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

/**
 * 将字符串转换为数值
 * @param str 字符串
 * @param defaultValue 默认值
 * @return 转换后的数值
 */
template <typename T>
T StringUtils::toNumber(const std::string& str, T defaultValue) {
    T result = defaultValue;
    std::istringstream iss(str);
    iss >> result;
    return result;
}

/**
 * 格式化字符串
 * @param format 格式字符串
 * @param args 可变参数
 * @return 格式化后的字符串
 */
template <typename... Args>
std::string StringUtils::format(const std::string& format, Args&&... args) {
    // 计算所需缓冲区大小
    int size = snprintf(nullptr, 0, format.c_str(), std::forward<Args>(args)...);
    if (size <= 0) {
        return "";
    }
    
    // 分配缓冲区
    std::vector<char> buffer(size + 1);
    
    // 格式化字符串
    snprintf(buffer.data(), buffer.size(), format.c_str(), std::forward<Args>(args)...);
    
    return std::string(buffer.data());
}

} // namespace utils
} // namespace evan
