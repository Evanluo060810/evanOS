// Copyright 2026 Evanluo
// Licensed under the Apache License, Version 2.0

#include "core/configuration.h"
#include <cstdio>

namespace evan {
    // 静态常量定义
    const char Configuration::BTYPE_NAME[5] = {0, 'K', 'M', 'G', 'T'};
    const unsigned long Configuration::DIV = 1024;
    
    /**
     * 默认构造函数
     * 创建默认配置：自动单位模式（单位字符为0，除数为1）
     */
    Configuration::Configuration() : 
        divByte(std::make_pair(0, 1)) {
    }
    
    /**
     * 构造函数
     * @param unitChar 单位字符 ('K', 'M', 'G'等，0表示自动模式)
     * @param divisor 除数 (1024, 1024*1024等，1表示自动模式)
     */
    Configuration::Configuration(char unitChar, unsigned long divisor) : 
        divByte(std::make_pair(unitChar, divisor)) {
    }
    
    /**
     * 设置字节显示类型
     * @param type 单位类型索引 (0=自动, 1=KB, 2=MB, 3=GB)
     */
    void Configuration::config_byte_unit_set(int type) {
        char unitChar = BTYPE_NAME[type];
        unsigned long divisor = 1;
        
        // 计算除数 (1024^type)
        for (int i = 0; i < type; ++i) {
            divisor *= DIV;
        }
        
        divByte = std::make_pair(unitChar, divisor);
    }
    
    /**
     * 获取字节转换配置
     * @return 包含单位字符和除数的pair
     */
    std::pair<char, unsigned long> Configuration::config_byte_unit_get() const {
        return divByte;
    }
    
    /**
     * 将字节数转换为可读字符串
     * @param byte 字节数
     * @return 转换后的字符串
     */
    std::string Configuration::config_byte_to_str(unsigned long long byte) const {
        auto currentDivByte = divByte;
        
        if (currentDivByte.second == 0 || currentDivByte.second == 1) {
            // 自动模式
            return autoConvertBytes(byte);
        } else {
            // 固定单位模式
            if (currentDivByte.second == 0) {
                return std::to_string(byte);
            }
            return std::to_string(byte / currentDivByte.second) + currentDivByte.first;
        }
    }
    
    /**
     * 重置为默认配置
     */
    void Configuration::config_reset() {
        divByte = std::make_pair(0, 1);
    }
    
    /**
     * 自动转换字节数
     * @param byte 字节数
     * @return 转换后的字符串
     */
    std::string Configuration::autoConvertBytes(unsigned long long byte) const {
        char buffer[100] = {0};
        const char *units[] = {"B", "KB", "MB", "GB", "TB", "PB"};
        int unitIndex = 0;
        unsigned long long size = byte;
        
        // 计算合适的单位
        while (size >= 1024 && unitIndex < (sizeof(units) / sizeof(units[0]) - 1)) {
            size /= 1024;
            unitIndex++;
        }
        
        // 格式化输出
        if (size >= 1024 && unitIndex < (sizeof(units) / sizeof(units[0]) - 1)) {
            // 超过MB时保留一位小数
            sprintf(buffer, "%lu.%01lu %s", 
                    static_cast<unsigned long>(size / 1024), 
                    static_cast<unsigned long>((size % 1024) * 10 / 1024), 
                    units[unitIndex + 1]);
        } else {
            sprintf(buffer, "%lu %s", static_cast<unsigned long>(size), units[unitIndex]);
        }
        
        return std::string(buffer);
    }
} // namespace evan
