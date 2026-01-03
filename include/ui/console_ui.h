// Copyright 2026 Evanluo
// Licensed under the Apache License, Version 2.0

#pragma once

#include <string>
#include <vector>

namespace evan {

// 控制台UI类
class ConsoleUI {
public:
    /**
     * 显示系统性能信息
     */
    static void displaySystemPerformance();
    
    /**
     * 显示系统内存信息
     */
    static void displaySystemMemory();
    
    /**
     * 显示总内存使用情况
     */
    static void displayTotalMemory();
    
    /**
     * 显示每个进程信息
     */
    static void displayEachProcess();
    
    /**
     * 显示硬件信息
     */
    static void displayHardwareInfo();
    
    /**
     * 显示GPU信息
     */
    static void displayGPUInfo();
    
    /**
     * 显示高级GPU信息
     */
    static void displayAdvancedGPUInfo();
    
    /**
     * 显示帮助信息
     */
    static void displayHelp();
    
    /**
     * 显示版权信息
     */
    static void displayCopyright();
    
    /**
     * 清屏
     */
    static void clearScreen();
    
    /**
     * 显示分隔线
     */
    static void displaySeparator();
};

} // namespace evan
