// Copyright 2026 Evanluo
// Licensed under the Apache License, Version 2.0

#include "ui/console_ui.h"
#include "core/system_monitor.h"
#include "core/gpu_monitor.h"
#include <iostream>

namespace evan {

/**
 * ConsoleUI类 - 控制台用户界面类
 * 
 * 功能：提供控制台界面的各种显示功能，包括系统信息、性能数据、GPU信息等
 * 设计：采用静态成员函数设计，无需实例化即可使用
 */

/**
 * 显示系统性能信息
 * 
 * 功能：显示系统性能相关信息，包括CPU使用率、内存使用情况等
 * 实现：调用系统监控模块的evos_system_performance_display函数
 * 依赖：globalConfig全局配置对象
 */
void ConsoleUI::displaySystemPerformance() {
    evos_system_performance_display(globalConfig);
}

/**
 * 显示系统内存信息
 * 
 * 功能：显示系统内存的详细信息，包括物理内存、虚拟内存等
 * 实现：调用系统监控模块的evos_system_info_display函数
 * 依赖：globalConfig全局配置对象
 */
void ConsoleUI::displaySystemMemory() {
    evos_system_info_display(globalConfig);
}

/**
 * 显示总内存使用情况
 * 
 * 功能：显示系统总内存的使用情况，包括已用内存、可用内存和使用率
 * 实现：调用系统监控模块的evos_memory_total_display函数
 * 依赖：globalConfig全局配置对象
 */
void ConsoleUI::displayTotalMemory() {
    evos_memory_total_display(globalConfig);
}

/**
 * 显示每个进程信息
 * 
 * 功能：显示系统中每个进程的详细信息，包括PID、进程名、内存使用等
 * 实现：调用系统监控模块的evos_process_enum_display函数
 * 依赖：globalConfig全局配置对象
 */
void ConsoleUI::displayEachProcess() {
    evos_process_enum_display(globalConfig);
}

/**
 * 显示硬件信息
 * 
 * 功能：显示系统硬件信息，包括CPU、内存、主板等
 * 实现：调用系统监控模块的evos_hardware_info_display函数
 * 依赖：globalConfig全局配置对象
 */
void ConsoleUI::displayHardwareInfo() {
    evos_hardware_info_display(globalConfig);
}

/**
 * 显示GPU基本信息
 * 
 * 功能：显示系统中所有GPU的基本信息，包括名称、厂商、驱动版本、内存使用情况等
 * 实现：
 * 1. 创建GPU监控管理器实例
 * 2. 初始化GPU监控管理器
 * 3. 获取所有GPU信息
 * 4. 遍历并输出每个GPU的基本信息
 * 5. 清理GPU监控管理器资源
 */
void ConsoleUI::displayGPUInfo() {
    // 创建GPU监控管理器实例
    GPUMonitorManager gpu_manager;
    
    // 初始化GPU监控管理器
    if (gpu_manager.initialize()) {
        std::vector<GPUInfo> gpu_info_list;  ///< 存储GPU信息的向量
        
        // 获取所有GPU信息
        if (gpu_manager.getAllGPUInfo(gpu_info_list)) {
            // 遍历所有GPU信息并输出
            for (const auto& gpu_info : gpu_info_list) {
                std::cout << "GPU名称: " << gpu_info.name << std::endl;
                std::cout << "厂商: " << gpu_info.vendor << std::endl;
                std::cout << "驱动版本: " << gpu_info.driver_version << std::endl;
                std::cout << "总显存: " << gpu_info.memory_total << " MB" << std::endl;
                std::cout << "已用显存: " << gpu_info.memory_used << " MB" << std::endl;
                std::cout << "使用率: " << gpu_info.utilization << "%" << std::endl;
                std::cout << "温度: " << gpu_info.temperature << "°C" << std::endl;
                std::cout << "功耗: " << gpu_info.power_usage << " W" << std::endl;
                std::cout << std::endl;
            }
        } else {
            std::cout << "无法获取GPU信息" << std::endl;
        }
        
        // 清理GPU监控管理器资源
        gpu_manager.cleanup();
    } else {
        std::cout << "无法初始化GPU监控" << std::endl;
    }
}

/**
 * 显示GPU高级信息
 * 
 * 功能：显示系统中所有GPU的高级详细信息，包括显存信息、性能信息、频率信息等
 * 实现：
 * 1. 创建GPU监控管理器实例
 * 2. 初始化GPU监控管理器
 * 3. 获取所有GPU信息
 * 4. 遍历并输出每个GPU的高级信息
 * 5. 清理GPU监控管理器资源
 */
void ConsoleUI::displayAdvancedGPUInfo() {
    // 创建GPU监控管理器实例
    GPUMonitorManager gpu_manager;
    
    // 初始化GPU监控管理器
    if (gpu_manager.initialize()) {
        std::vector<GPUInfo> gpu_info_list;  ///< 存储GPU信息的向量
        
        // 获取所有GPU信息
        if (gpu_manager.getAllGPUInfo(gpu_info_list)) {
            // 遍历所有GPU信息并输出高级信息
            for (const auto& gpu_info : gpu_info_list) {
                std::cout << "=== 高级GPU信息 ===" << std::endl;
                std::cout << "GPU名称: " << gpu_info.name << std::endl;
                std::cout << "厂商: " << gpu_info.vendor << std::endl;
                std::cout << "驱动版本: " << gpu_info.driver_version << std::endl;
                std::cout << "显存信息: " << std::endl;
                std::cout << "  总显存: " << gpu_info.memory_total << " MB" << std::endl;
                std::cout << "  已用显存: " << gpu_info.memory_used << " MB" << std::endl;
                std::cout << "  可用显存: " << gpu_info.memory_free << " MB" << std::endl;
                std::cout << "性能信息: " << std::endl;
                std::cout << "  使用率: " << gpu_info.utilization << "%" << std::endl;
                std::cout << "  温度: " << gpu_info.temperature << "°C" << std::endl;
                std::cout << "  功耗: " << gpu_info.power_usage << " W" << std::endl;
                std::cout << "频率信息: " << std::endl;
                std::cout << "  核心频率: " << gpu_info.clock_core << " MHz" << std::endl;
                std::cout << "  显存频率: " << gpu_info.clock_memory << " MHz" << std::endl;
                std::cout << std::endl;
            }
        } else {
            std::cout << "无法获取GPU信息" << std::endl;
        }
        
        // 清理GPU监控管理器资源
        gpu_manager.cleanup();
    } else {
        std::cout << "无法初始化GPU监控" << std::endl;
    }
}

/**
 * 显示帮助信息
 * 
 * 功能：显示程序的使用帮助和命令行选项说明
 * 实现：直接输出格式化的帮助信息到控制台
 */
void ConsoleUI::displayHelp() {
    std::cout << "evanOS - 企业级系统监控与运维平台" << std::endl;
    std::cout << "使用方法: evanOS [选项]" << std::endl;
    std::cout << std::endl;
    std::cout << "选项:" << std::endl;
    std::cout << "  -p, --perf        显示系统性能信息" << std::endl;
    std::cout << "  -s, --sys         显示系统内存信息" << std::endl;
    std::cout << "  -t, --total       显示总内存使用情况" << std::endl;
    std::cout << "  -e, --each        显示每个进程信息" << std::endl;
    std::cout << "  -w, --hardware    显示硬件信息" << std::endl;
    std::cout << "  -g, --gpu         显示GPU信息" << std::endl;
    std::cout << "  -G, --gpu-advanced 显示高级GPU信息" << std::endl;
    std::cout << "  -a, --all         显示所有信息" << std::endl;
    std::cout << "  -y, --type        设置显示字节类型[0=B,1=KB,2=MB,3=GB]" << std::endl;
    std::cout << "  -?, --help        显示帮助信息" << std::endl;
    std::cout << "  -c, --copyright   显示版权信息" << std::endl;
}

/**
 * 显示版权信息
 * 
 * 功能：显示程序的版权信息和许可证条款
 * 实现：直接输出格式化的版权信息到控制台
 */
void ConsoleUI::displayCopyright() {
    std::cout << "[LICENSE]:" << std::endl;
    std::cout << std::endl;
    std::cout << "Copyright 2026 Evanluo" << std::endl;
    std::cout << std::endl;
    std::cout << "Licensed under the Apache License, Version 2.0 (the \"License\");" << std::endl;
    std::cout << "you may not use this file except in compliance with the License." << std::endl;
    std::cout << "You may obtain a copy of the License at" << std::endl;
    std::cout << std::endl;
    std::cout << "http://www.apache.org/licenses/LICENSE-2.0" << std::endl;
    std::cout << std::endl;
    std::cout << "Unless required by applicable law or agreed to in writing, software" << std::endl;
    std::cout << "distributed under the License is distributed on an \"AS IS\" BASIS," << std::endl;
    std::cout << "WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied." << std::endl;
    std::cout << "See the License for the specific language governing permissions and" << std::endl;
    std::cout << "limitations under the License." << std::endl;
    std::cout << std::endl;
}

/**
 * 清屏
 * 
 * 功能：清除控制台屏幕内容
 * 实现：根据不同操作系统调用不同的系统命令
 * 支持：Windows系统使用cls命令，其他系统使用clear命令
 */
void ConsoleUI::clearScreen() {
#ifdef _WIN32
    system("cls");  ///< Windows系统清屏命令
#else
    system("clear");  ///< Linux/Mac系统清屏命令
#endif
}

/**
 * 显示分隔线
 * 
 * 功能：在控制台输出一条分隔线，用于分隔不同的信息区块
 * 实现：直接输出格式化的分隔线字符串到控制台
 */
void ConsoleUI::displaySeparator() {
    std::cout << "-----------------------------------------------" << std::endl;
}

} // namespace evan
