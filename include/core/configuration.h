// Copyright 2026 Evanluo
// Licensed under the Apache License, Version 2.0

#pragma once

#include <string>
#include <mutex>

namespace evan {

/**
 * 配置管理类
 * 用于管理系统的各种配置参数
 * 提供线程安全的配置访问和修改接口
 */
class Configuration {
public:
    /**
     * 默认构造函数
     * 创建默认配置
     */
    Configuration();
    
    /**
     * 构造函数
     * @param unitChar 单位字符 ('K', 'M', 'G'等，0表示自动模式)
     * @param divisor 除数 (1024, 1024*1024等，1表示自动模式)
     */
    Configuration(char unitChar, unsigned long divisor);
    
    /**
     * 设置字节显示类型
     * @param type 单位类型索引 (0=自动, 1=KB, 2=MB, 3=GB)
     */
    void config_byte_unit_set(int type);
    
    /**
     * 获取字节转换配置
     * @return 包含单位字符和除数的pair
     */
    std::pair<char, unsigned long> config_byte_unit_get() const;
    
    /**
     * 将字节数转换为可读字符串
     * @param byte 字节数
     * @return 转换后的字符串
     */
    std::string config_byte_to_str(unsigned long long byte) const;
    
    /**
     * 重置为默认配置
     */
    void config_reset();
    
private:
    /**
     * 自动转换字节数
     * @param byte 字节数
     * @return 转换后的字符串
     */
    std::string autoConvertBytes(unsigned long long byte) const;
    
    /**
     * 字节转换配置
     * first: 单位字符 ('K', 'M', 'G'等，0表示自动模式)
     * second: 除数 (1024^n)
     */
    std::pair<char, unsigned long> divByte;
    
    /**
     * 字节单位名称
     */
    static const char BTYPE_NAME[5];
    
    /**
     * 除法因子
     */
    static const unsigned long DIV;
};

} // namespace evan
