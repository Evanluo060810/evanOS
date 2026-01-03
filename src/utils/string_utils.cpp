// Copyright 2026 Evanluo
// Licensed under the Apache License, Version 2.0

#include "utils/string_utils.h"

namespace evan {
namespace utils {
    /**
     * StringUtils类 - 字符串工具类
     * 
     * 功能：提供各种字符串处理功能，包括大小写转换、修剪、分割、连接、替换等
     * 设计：采用静态成员函数设计，无需实例化即可使用
     * 用途：用于处理系统中各种字符串操作需求
     */
    
    /**
     * 将字符串转换为小写
     * 
     * @param str 输入字符串
     * @return 转换后的小写字符串
     * 
     * 功能：将输入字符串中的所有大写字符转换为小写字符
     * 实现：使用std::transform函数和::tolower函数实现
     */
    std::string StringUtils::toLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }
    
    /**
     * 将字符串转换为大写
     * @param str 输入字符串
     * @return 转换后的大写字符串
     */
    std::string StringUtils::toUpper(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }
    
    /**
     * 去除字符串首尾空白字符
     * @param str 输入字符串
     * @return 处理后的字符串
     */
    std::string StringUtils::trim(const std::string& str) {
        size_t start = str.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) {
            return "";
        }
        
        size_t end = str.find_last_not_of(" \t\r\n");
        return str.substr(start, end - start + 1);
    }
    
    /**
     * 按分隔符分割字符串
     * @param str 输入字符串
     * @param delimiter 分隔符
     * @return 分割后的字符串向量
     */
    std::vector<std::string> StringUtils::split(const std::string& str, char delimiter) {
        std::vector<std::string> result;
        std::istringstream iss(str);
        std::string item;
        
        while (std::getline(iss, item, delimiter)) {
            result.push_back(item);
        }
        
        return result;
    }
    
    /**
     * 按分隔符分割字符串
     * @param str 输入字符串
     * @param delimiter 分隔符字符串
     * @return 分割后的字符串向量
     */
    std::vector<std::string> StringUtils::split(const std::string& str, const std::string& delimiter) {
        std::vector<std::string> result;
        size_t pos = 0;
        size_t prev = 0;
        
        while ((pos = str.find(delimiter, prev)) != std::string::npos) {
            result.push_back(str.substr(prev, pos - prev));
            prev = pos + delimiter.length();
        }
        
        // 添加最后一个子串
        result.push_back(str.substr(prev));
        
        return result;
    }
    
    /**
     * 连接字符串向量
     * @param vec 字符串向量
     * @param delimiter 连接符
     * @return 连接后的字符串
     */
    std::string StringUtils::join(const std::vector<std::string>& vec, const std::string& delimiter) {
        std::string result;
        
        for (size_t i = 0; i < vec.size(); ++i) {
            if (i > 0) {
                result += delimiter;
            }
            result += vec[i];
        }
        
        return result;
    }
    
    /**
     * 替换字符串中的子串
     * @param str 原始字符串
     * @param from 要替换的子串
     * @param to 替换后的子串
     * @return 替换后的字符串
     */
    std::string StringUtils::replace(const std::string& str, const std::string& from, const std::string& to) {
        std::string result = str;
        size_t pos = 0;
        
        while ((pos = result.find(from, pos)) != std::string::npos) {
            result.replace(pos, from.length(), to);
            pos += to.length();
        }
        
        return result;
    }
    
    /**
     * 检查字符串是否以指定前缀开头
     * @param str 输入字符串
     * @param prefix 前缀
     * @return 是否以指定前缀开头
     */
    bool StringUtils::startsWith(const std::string& str, const std::string& prefix) {
        if (str.length() < prefix.length()) {
            return false;
        }
        return str.substr(0, prefix.length()) == prefix;
    }
    
    /**
     * 检查字符串是否以指定后缀结尾
     * @param str 输入字符串
     * @param suffix 后缀
     * @return 是否以指定后缀结尾
     */
    bool StringUtils::endsWith(const std::string& str, const std::string& suffix) {
        if (str.length() < suffix.length()) {
            return false;
        }
        return str.substr(str.length() - suffix.length()) == suffix;
    }
    
    /**
     * 检查字符串是否包含指定子串
     * @param str 输入字符串
     * @param substr 子串
     * @return 是否包含指定子串
     */
    bool StringUtils::contains(const std::string& str, const std::string& substr) {
        return str.find(substr) != std::string::npos;
    }
} // namespace utils
} // namespace evan
