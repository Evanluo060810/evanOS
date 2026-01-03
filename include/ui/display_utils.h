// Copyright 2026 Evanluo
// Licensed under the Apache License, Version 2.0

#pragma once

#include <string>
#include <vector>
#include <map>
#include <utility>

namespace evan {

// 显示工具类
class DisplayUtils {
public:
    /**
     * 格式化字节数为可读字符串
     * @param bytes 字节数
     * @return 格式化后的字符串
     */
    static std::string formatBytes(unsigned long long bytes);
    
    /**
     * 格式化百分比
     * @param value 百分比值（0-100）
     * @param width 输出宽度
     * @return 格式化后的字符串
     */
    static std::string formatPercent(float value, int width = 5);
    
    /**
     * 格式化温度
     * @param temperature 温度值（摄氏度）
     * @return 格式化后的字符串
     */
    static std::string formatTemperature(float temperature);
    
    /**
     * 格式化频率
     * @param frequency 频率值（MHz）
     * @return 格式化后的字符串
     */
    static std::string formatFrequency(float frequency);
    
    /**
     * 创建表格
     * @param headers 表头
     * @param rows 行数据
     * @return 表格字符串
     */
    static std::string createTable(const std::vector<std::string>& headers,
                                  const std::vector<std::vector<std::string>>& rows);
    
    /**
     * 显示进度条
     * @param progress 进度值（0-100）
     * @param width 进度条宽度
     */
    static void displayProgressBar(float progress, int width = 50);
    
    /**
     * 居中对齐字符串
     * @param str 原始字符串
     * @param width 总宽度
     * @return 居中对齐后的字符串
     */
    static std::string centerString(const std::string& str, int width);
    
    /**
     * 左对齐字符串
     * @param str 原始字符串
     * @param width 总宽度
     * @return 左对齐后的字符串
     */
    static std::string leftAlignString(const std::string& str, int width);
    
    /**
     * 右对齐字符串
     * @param str 原始字符串
     * @param width 总宽度
     * @return 右对齐后的字符串
     */
    static std::string rightAlignString(const std::string& str, int width);
    
    /**
     * 截断字符串
     * @param str 原始字符串
     * @param max_length 最大长度
     * @return 截断后的字符串
     */
    static std::string truncateString(const std::string& str, size_t max_length);
    
    /**
     * 生成分隔线
     * @param length 分隔线长度
     * @param ch 分隔字符
     * @return 分隔线字符串
     */
    static std::string generateSeparator(int length = 50, char ch = '-');
};

} // namespace evan
