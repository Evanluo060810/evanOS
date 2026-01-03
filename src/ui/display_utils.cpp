// Copyright 2026 Evanluo
// Licensed under the Apache License, Version 2.0

#include "ui/display_utils.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <iostream>

namespace evan {

/**
 * 格式化字节数为可读字符串
 * @param bytes 字节数
 * @return 格式化后的字符串
 */
std::string DisplayUtils::formatBytes(unsigned long long bytes) {
    const char* units[] = {"B", "KB", "MB", "GB", "TB", "PB"};
    int unitIndex = 0;
    unsigned long long size = bytes;
    
    while (size >= 1024 && unitIndex < 5) {
        size /= 1024;
        unitIndex++;
    }
    
    std::stringstream ss;
    if (unitIndex == 0) {
        ss << size << " " << units[unitIndex];
    } else {
        ss << std::fixed << std::setprecision(1) << size << " " << units[unitIndex];
    }
    
    return ss.str();
}

/**
 * 格式化百分比
 * @param value 百分比值（0-100）
 * @param width 输出宽度
 * @return 格式化后的字符串
 */
std::string DisplayUtils::formatPercent(float value, int width) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << std::setw(width) << value << "%";
    return ss.str();
}

/**
 * 格式化温度
 * @param temperature 温度值（摄氏度）
 * @return 格式化后的字符串
 */
std::string DisplayUtils::formatTemperature(float temperature) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << temperature << "°C";
    return ss.str();
}

/**
 * 格式化频率
 * @param frequency 频率值（MHz）
 * @return 格式化后的字符串
 */
std::string DisplayUtils::formatFrequency(float frequency) {
    std::stringstream ss;
    
    if (frequency >= 1000) {
        ss << std::fixed << std::setprecision(1) << frequency / 1000.0 << " GHz";
    } else {
        ss << std::fixed << std::setprecision(0) << frequency << " MHz";
    }
    
    return ss.str();
}

/**
 * 创建表格
 * @param headers 表头
 * @param rows 行数据
 * @return 表格字符串
 */
std::string DisplayUtils::createTable(const std::vector<std::string>& headers,
                                     const std::vector<std::vector<std::string>>& rows) {
    if (headers.empty()) {
        return "";
    }
    
    // 计算每列的最大宽度
    std::vector<int> columnWidths(headers.size(), 0);
    
    // 考虑表头
    for (size_t i = 0; i < headers.size(); ++i) {
        columnWidths[i] = headers[i].length();
    }
    
    // 考虑行数据
    for (const auto& row : rows) {
        for (size_t i = 0; i < row.size() && i < columnWidths.size(); ++i) {
            if (row[i].length() > columnWidths[i]) {
                columnWidths[i] = row[i].length();
            }
        }
    }
    
    std::stringstream ss;
    
    // 显示表头
    for (size_t i = 0; i < headers.size(); ++i) {
        ss << std::setw(columnWidths[i] + 2) << std::left << headers[i];
    }
    ss << std::endl;
    
    // 显示分隔线
    for (size_t i = 0; i < columnWidths.size(); ++i) {
        ss << std::string(columnWidths[i] + 2, '-');
    }
    ss << std::endl;
    
    // 显示行数据
    for (const auto& row : rows) {
        for (size_t i = 0; i < row.size() && i < columnWidths.size(); ++i) {
            ss << std::setw(columnWidths[i] + 2) << std::left << row[i];
        }
        ss << std::endl;
    }
    
    return ss.str();
}

/**
 * 显示进度条
 * @param progress 进度值（0-100）
 * @param width 进度条宽度
 */
void DisplayUtils::displayProgressBar(float progress, int width) {
    // 确保进度在0-100之间
    progress = std::max(0.0f, std::min(100.0f, progress));
    
    int filledWidth = static_cast<int>((progress / 100.0f) * width);
    
    std::cout << "[";
    for (int i = 0; i < width; ++i) {
        if (i < filledWidth) {
            std::cout << "█";
        } else {
            std::cout << "-";
        }
    }
    std::cout << "] " << std::fixed << std::setprecision(1) << progress << "%\r";
    std::cout.flush();
}

/**
 * 居中对齐字符串
 * @param str 原始字符串
 * @param width 总宽度
 * @return 居中对齐后的字符串
 */
std::string DisplayUtils::centerString(const std::string& str, int width) {
    if (str.length() >= static_cast<size_t>(width)) {
        return str;
    }
    
    int padding = width - static_cast<int>(str.length());
    int leftPadding = padding / 2;
    int rightPadding = padding - leftPadding;
    
    return std::string(leftPadding, ' ') + str + std::string(rightPadding, ' ');
}

/**
 * 左对齐字符串
 * @param str 原始字符串
 * @param width 总宽度
 * @return 左对齐后的字符串
 */
std::string DisplayUtils::leftAlignString(const std::string& str, int width) {
    if (str.length() >= static_cast<size_t>(width)) {
        return str;
    }
    
    return str + std::string(width - str.length(), ' ');
}

/**
 * 右对齐字符串
 * @param str 原始字符串
 * @param width 总宽度
 * @return 右对齐后的字符串
 */
std::string DisplayUtils::rightAlignString(const std::string& str, int width) {
    if (str.length() >= static_cast<size_t>(width)) {
        return str;
    }
    
    return std::string(width - str.length(), ' ') + str;
}

/**
 * 截断字符串
 * @param str 原始字符串
 * @param max_length 最大长度
 * @return 截断后的字符串
 */
std::string DisplayUtils::truncateString(const std::string& str, size_t max_length) {
    if (str.length() <= max_length) {
        return str;
    }
    
    if (max_length <= 3) {
        return str.substr(0, max_length);
    }
    
    return str.substr(0, max_length - 3) + "...";
}

/**
 * 生成分隔线
 * @param length 分隔线长度
 * @param ch 分隔字符
 * @return 分隔线字符串
 */
std::string DisplayUtils::generateSeparator(int length, char ch) {
    return std::string(length, ch);
}

} // namespace evan
