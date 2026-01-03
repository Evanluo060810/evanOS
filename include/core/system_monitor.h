// Copyright 2026 Evanluo
// Licensed under the Apache License, Version 2.0

#pragma once

#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include <atomic>
#include <mutex>
#include <unordered_map>
#include <map>
#include <string>
#include <stdexcept>
#include <typeinfo>
#include <algorithm>
#include <iomanip>
#include <functional>
#include <exception>

#include <cstring>
#include <cstdlib>
#include <ctime>

// Windows系统API头文件
#include <windows.h>
#include <tchar.h>
#include <psapi.h>
#include <tlhelp32.h>

// 链接到必要的网络库
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

// 引入配置管理类
#include "core/configuration.h"

// 常量定义
namespace evan {
    /**
     * PID显示宽度
     */
    const int PID_SIZE = 10;
    
    /**
     * 进程名称显示宽度
     */
    const int PNAME_SIZE = 20;
    
    /**
     * 工作集大小显示宽度
     */
    const int PWORKSET_SIZE = 15;
    
    /**
     * 数字显示宽度
     */
    const int NUM_WIDTH = 15;
    
    /**
     * 默认字节显示类型（自动）
     */
    const int DEFAULT_BTYPE = 0;
    
    /**
     * 最小进程ID
     */
    const unsigned long PID_MIN = 1;
    
    /**
     * 最大进程ID
     */
    const unsigned long PID_MAX = 0xFFFF;
    
    /**
     * 最小循环时间（秒）
     */
    const unsigned int MIN_TIME = 1;
    
    /**
     * 最大循环时间（秒）
     */
    const unsigned int MAX_TIME = 65535;
    
    /**
     * 字节单位名称数组
     * 索引0: 自动模式
     * 索引1: KB
     * 索引2: MB
     * 索引3: GB
     * 索引4: TB
     */
    const char BTYPE_NAME[5] = {0, 'K', 'M', 'G', 'T'};
    
    /**
     * 除法因子（1024）
     */
    const unsigned long DIV = 1024;
}

namespace evan {
    /**
     * 函数指针类型定义
     * 用于指向无参数、无返回值的显示函数
     */
    typedef void (*FuncPtr)();
    
    /**
     * 参数函数结构体
     * 用于存储命令行参数与对应显示函数的映射关系
     */
    struct ArguFunc {
        char shortName;       ///< 短选项名（如 'p' 对应 '--perf'）
        std::string desc;     ///< 参数描述信息
        FuncPtr func;         ///< 对应的函数指针
        
        /**
         * 构造函数
         * @param s 短选项名
         * @param d 参数描述
         * @param f 函数指针
         */
        ArguFunc(char s, const std::string& d, FuncPtr f) : 
            shortName(s), desc(d), func(f) {}
    };
    
    /**
     * 错误打印函数
     * @param funcName 发生错误的函数名
     */
    void evos_error_print(const std::string& funcName);
    
    /**
     * 显示总内存使用情况
     * @param config 配置实例，用于格式化输出
     */
    void evos_memory_total_display(const Configuration& config);
    
    /**
     * 显示系统基本信息
     * @param config 配置实例，用于格式化输出
     */
    void evos_system_info_display(const Configuration& config);
    
    /**
     * 显示系统性能信息
     * @param config 配置实例，用于格式化输出
     */
    void evos_system_performance_display(const Configuration& config);
    
    /**
     * 显示每个进程的详细信息
     * @param config 配置实例，用于格式化输出
     */
    void evos_process_enum_display(const Configuration& config);
    
    /**
     * 显示硬件信息
     * @param config 配置实例，用于格式化输出
     */
    void evos_hardware_info_display(const Configuration& config);
    
    /**
     * 显示指定进程的详细信息
     * @param pid 进程ID
     * @param config 配置实例，用于格式化输出
     */
    void evos_process_info_display(DWORD pid, const Configuration& config);
    
    /**
     * 显示GPU基本信息
     */
    void evos_gpu_info_display();
    
    /**
     * 显示GPU高级信息
     */
    void evos_gpu_advanced_info_display();
    
    /**
     * 显示GPU负载平衡建议
     */
    void evos_gpu_balance_display();
    
    /**
     * 显示GPU功耗分析
     */
    void evos_gpu_power_display();
    
    /**
     * 显示GPU频率调优建议
     */
    void evos_gpu_tuning_display();
    
    /**
     * 显示网络连接信息
     */
    void evos_network_display();
    
    /**
     * 显示网络流量统计
     */
    void evos_traffic_display();
    
    /**
     * 显示端口使用信息
     */
    void evos_ports_display();
    
    /**
     * 显示网络带宽统计
     */
    void evos_net_bandwidth_display();
    
    /**
     * 显示网络流量图表
     */
    void evos_net_chart_display();
    
    /**
     * 显示网络连接分析
     */
    void evos_net_connection_display();
    
    /**
     * 显示网络接口状态
     */
    void evos_net_interfaces_display();
    
    /**
     * 显示端口开放扫描结果
     * @param host 目标主机IP或域名
     * @param start_port 起始端口号
     * @param end_port 结束端口号
     */
    void evos_port_scan_display(const std::string& host, int start_port, int end_port);
    
    /**
     * 获取CPU使用率
     * @return CPU使用率百分比
     */
    DWORD evos_cpu_usage_get();
    
    /**
     * 全局配置实例
     * 用于存储和访问系统配置参数
     */
    extern Configuration globalConfig;
}
