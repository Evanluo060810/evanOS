// Copyright 2026 Evanluo
// Licensed under the Apache License, Version 2.0

#include "core/system_monitor.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <psapi.h>
#include <cstring>

// 链接到必要的网络库
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "psapi.lib")

namespace evan {
    /**
     * CPU架构映射表
     * 将Windows API返回的架构代码映射为可读字符串
     */
    const std::map<WORD, std::string> archList = {
        {9, "x64"},              ///< x64架构
        {5, "ARM"},              ///< ARM架构
        {12, "ARM64"},           ///< ARM64架构
        {6, "Intel Itanium"},    ///< Intel Itanium架构
        {0, "x86"},              ///< x86架构
        {0xffff, "Unknown"}      ///< 未知架构
    };
    
    /**
     * 内存状态映射表
     * 将Windows API返回的内存状态代码映射为可读字符串
     */
    const std::map<DWORD, std::string> mbiStateList = {
        {MEM_COMMIT, "Committed"}, ///< 已提交内存
        {MEM_FREE, "Free"},        ///< 空闲内存
        {MEM_RESERVE, "Reserved"}  ///< 保留内存
    };
    
    /**
     * 内存保护映射表
     * 将Windows API返回的内存保护代码映射为可读字符串
     */
    const std::map<DWORD, std::string> mbiProtectList = {
        {PAGE_NOACCESS, "No Access"},                  ///< 无访问权限
        {PAGE_READONLY, "Read Only"},                  ///< 只读
        {PAGE_READWRITE, "Read/Write"},                ///< 读写
        {PAGE_WRITECOPY, "Write Copy"},               ///< 写时复制
        {PAGE_EXECUTE, "Execute"},                    ///< 可执行
        {PAGE_EXECUTE_READ, "Execute/Read"},          ///< 可执行/只读
        {PAGE_EXECUTE_READWRITE, "Execute/Read/Write"}, ///< 可执行/读写
        {PAGE_EXECUTE_WRITECOPY, "Execute/Write Copy"}, ///< 可执行/写时复制
        {PAGE_GUARD, "Guard Page"},                   ///< 保护页
        {PAGE_NOCACHE, "No Cache"},                   ///< 无缓存
        {PAGE_WRITECOMBINE, "Write Combine"}          ///< 写合并
    };
    
    /**
     * 内存类型映射表
     * 将Windows API返回的内存类型代码映射为可读字符串
     */
    const std::map<DWORD, std::string> mbiTypeList = {
        {MEM_IMAGE, "Image"},      ///< 映像内存（可执行文件）
        {MEM_MAPPED, "Mapped"},    ///< 映射内存（内存映射文件）
        {MEM_PRIVATE, "Private"}   ///< 私有内存（进程私有）
    };
    
    /**
     * 根据架构代码获取架构名称
     * @param arch 架构代码
     * @return 架构名称字符串
     */
    const char* getArchName(WORD arch) {
        auto it = archList.find(arch);
        if (it != archList.end()) {
            return it->second.c_str();
        }
        return "Unknown";
    }
    
    /**
     * 根据内存状态代码获取内存状态名称
     * @param state 内存状态代码
     * @return 内存状态名称字符串
     */
    const char* getMbiStateName(DWORD state) {
        auto it = mbiStateList.find(state);
        if (it != mbiStateList.end()) {
            return it->second.c_str();
        }
        return "Unknown";
    }
    
    /**
     * 根据内存保护代码获取内存保护名称
     * @param protect 内存保护代码
     * @return 内存保护名称字符串
     */
    const char* getMbiProtectName(DWORD protect) {
        auto it = mbiProtectList.find(protect);
        if (it != mbiProtectList.end()) {
            return it->second.c_str();
        }
        return "Unknown";
    }
    
    /**
     * 错误打印函数
     * @param funcName 发生错误的函数名
     * 
     * 功能：打印Windows API调用错误信息
     * 实现：使用FormatMessage函数从系统获取错误描述，然后输出到控制台
     */
    void evos_error_print(const std::string& funcName) {
        DWORD errorCode = GetLastError();  ///< 获取当前线程的最后一个错误码
        LPVOID lpMsgBuf = nullptr;         ///< 用于存储错误信息的缓冲区
        
        // 从系统获取错误信息
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,                          ///< 不使用特定的消息源
            errorCode,                     ///< 错误码
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  ///< 语言ID（中性语言）
            (LPTSTR)&lpMsgBuf,             ///< 输出缓冲区
            0,                             ///< 最小缓冲区大小（由系统分配）
            NULL                           ///< 可变参数（未使用）
        );
        
        // 打印错误信息
        printf("Error in %s: %s\n", funcName.c_str(), (LPTSTR)lpMsgBuf);
        
        // 释放系统分配的缓冲区
        LocalFree(lpMsgBuf);
    }
    
    /**
     * 获取系统内存状态信息（辅助函数）
     * @param memoryStatus [out] 用于存储内存状态信息的结构体
     * @return 成功返回true，失败返回false
     * 
     * 功能：封装GlobalMemoryStatusEx调用，减少重复代码
     */
    bool getMemoryStatus(MEMORYSTATUSEX& memoryStatus) {
        memoryStatus.dwLength = sizeof(memoryStatus);
        return GlobalMemoryStatusEx(&memoryStatus);
    }
    
    /**
     * 显示总内存信息
     * @param config 配置实例，用于格式化输出
     * 
     * 功能：显示系统总内存、已用内存、可用内存和内存使用率
     * 实现：使用GlobalMemoryStatusEx函数获取系统内存状态，仅调用一次API
     */
    void evos_memory_total_display(const Configuration& config) {
        MEMORYSTATUSEX memoryStatus;
        
        if (!getMemoryStatus(memoryStatus)) {
            printf("Error: Failed to retrieve memory status.\n");
            evos_error_print("GlobalMemoryStatusEx");
            return;
        }
        
        // 计算已用物理内存
        const unsigned long long usedPhys = memoryStatus.ullTotalPhys - memoryStatus.ullAvailPhys;
        // 计算内存使用率
        const double memoryUsage = static_cast<double>(usedPhys) / memoryStatus.ullTotalPhys * 100;
        
        // 输出内存信息
        printf("\n[Total Memory Information]\n");
        printf("-----------------------------------------------\n");
        printf("\tTotal Physical Memory: %s.\n", config.config_byte_to_str(memoryStatus.ullTotalPhys).c_str());
        printf("\tUsed Physical Memory: %s.\n", config.config_byte_to_str(usedPhys).c_str());
        printf("\tFree Physical Memory: %s.\n", config.config_byte_to_str(memoryStatus.ullAvailPhys).c_str());
        printf("\tMemory Usage: %.2f%%.\n", memoryUsage);
    }
    
    /**
     * 显示系统基本信息
     * @param config 配置实例，用于格式化输出
     * 
     * 功能：显示系统基本信息，包括处理器架构、内存等
     * 实现：使用GetSystemInfo获取系统硬件信息，使用getMemoryStatus获取内存信息
     */
    void evos_system_info_display(const Configuration& config) {
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        
        MEMORYSTATUSEX memoryStatus;
        
        if (!getMemoryStatus(memoryStatus)) {
            printf("Error: Failed to retrieve memory status.\n");
            evos_error_print("GlobalMemoryStatusEx");
            return;
        }
        
        // 输出系统信息
        printf("\n[System Information]\n");
        printf("-----------------------------------------------\n");
        printf("\tProcessor Architecture: %s.\n", getArchName(sysInfo.wProcessorArchitecture));
        printf("\tNumber of Processors: %lu.\n", sysInfo.dwNumberOfProcessors);
        printf("\tPage Size: %lu bytes.\n", sysInfo.dwPageSize);
        printf("\tMinimum Application Address: 0x%p.\n", sysInfo.lpMinimumApplicationAddress);
        printf("\tMaximum Application Address: 0x%p.\n", sysInfo.lpMaximumApplicationAddress);
        printf("\tActive Processor Mask: 0x%lx.\n", sysInfo.dwActiveProcessorMask);
        printf("\tTotal Physical Memory: %s.\n", config.config_byte_to_str(memoryStatus.ullTotalPhys).c_str());
    }
    
    /**
     * 显示系统性能信息
     * @param config 配置实例，用于格式化输出
     * 
     * 功能：显示系统性能信息，包括内存使用、页面文件等
     * 实现：使用getMemoryStatus获取内存信息，预计算所有需要的值以提高性能
     */
    void evos_system_performance_display(const Configuration& config) {
        MEMORYSTATUSEX memoryStatus;
        
        if (!getMemoryStatus(memoryStatus)) {
            printf("Error: Failed to retrieve memory status.\n");
            evos_error_print("GlobalMemoryStatusEx");
            return;
        }
        
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        
        // 预计算所有需要的值，避免重复计算
        const unsigned long totalAppPages = static_cast<unsigned long>(memoryStatus.ullTotalPageFile / sysInfo.dwPageSize);
        const unsigned long availPages = static_cast<unsigned long>(memoryStatus.ullAvailPageFile / sysInfo.dwPageSize);
        const unsigned long totalPhysPages = static_cast<unsigned long>(memoryStatus.ullTotalPhys / sysInfo.dwPageSize);
        const unsigned long availPhysPages = static_cast<unsigned long>(memoryStatus.ullAvailPhys / sysInfo.dwPageSize);
        const unsigned long long usedVirtual = memoryStatus.ullTotalVirtual - memoryStatus.ullAvailVirtual;
        
        // 输出性能信息
        printf("\n[Performance Information]\n");
        printf("-----------------------------------------------\n");
        printf("\tPage Size: %lu bytes.\n", sysInfo.dwPageSize);
        printf("\tTotal Application Pages: %lu.\n", totalAppPages);
        printf("\tAvailable Pages: %lu.\n", availPages);
        printf("\tTotal Physical Pages: %lu.\n", totalPhysPages);
        printf("\tAvailable Physical Pages: %lu.\n", availPhysPages);
        printf("\tSystem Cache Size: %s.\n", config.config_byte_to_str(usedVirtual).c_str());
        printf("\tFree System Memory: %s.\n", config.config_byte_to_str(memoryStatus.ullAvailVirtual).c_str());
        printf("\tMemory Usage: %lu%%.\n", memoryStatus.dwMemoryLoad);
    }
    
    // 显示每个进程信息
    void evos_process_enum_display(const Configuration& config) {
        // 使用CreateToolhelp32Snapshot获取进程快照，这是获取系统所有进程的高效方式
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE) {
            printf("Error: Unable to create process snapshot.\n");
            evos_error_print("CreateToolhelp32Snapshot");
            return;
        }
        
        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);
        
        // 获取第一个进程信息
        if (!Process32First(hSnapshot, &pe32)) {
            printf("Error: Unable to get first process.\n");
            evos_error_print("Process32First");
            CloseHandle(hSnapshot);
            return;
        }
        
        // 输出表头
        printf("\n[Process Information]\n");
        printf("-----------------------------------------------\n");
        printf("%-*s %-*s %*s %*s\n",
               PID_SIZE, "PID",
               PNAME_SIZE, "Process Name",
               PWORKSET_SIZE, "Working Set",
               NUM_WIDTH, "Page File(KB)");
        
        // 遍历所有进程
        do {
            // 只请求必要的PROCESS_QUERY_LIMITED_INFORMATION权限，减少权限请求开销
            // 对于PROCESS_MEMORY_COUNTERS，PROCESS_QUERY_LIMITED_INFORMATION权限已经足够
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION,
                                          FALSE, pe32.th32ProcessID);
            if (hProcess == NULL) {
                // 无法打开进程，可能是权限问题，只显示基本信息
                printf("%-*lu %-*s %*s %*s\n",
                   PID_SIZE, pe32.th32ProcessID,
                   PNAME_SIZE, pe32.szExeFile,
                   PWORKSET_SIZE, "-",
                   NUM_WIDTH, "-");
                continue;
            }
            
            // 初始化PROCESS_MEMORY_COUNTERS结构体
            PROCESS_MEMORY_COUNTERS pmc;
            pmc.cb = sizeof(pmc);
            
            // 获取进程内存信息
            if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
                // 格式化输出进程信息
                printf("%-*lu %-*s %*s %*ld\n",
                   PID_SIZE, pe32.th32ProcessID,
                   PNAME_SIZE, pe32.szExeFile,
                   PWORKSET_SIZE, config.config_byte_to_str(pmc.WorkingSetSize).c_str(),
                   NUM_WIDTH, static_cast<long>(pmc.PagefileUsage / 1024));
            } else {
                // 获取失败，显示基本信息
                printf("%-*lu %-*s %*s %*s\n",
                       PID_SIZE, pe32.th32ProcessID,
                       PNAME_SIZE, pe32.szExeFile,
                       PWORKSET_SIZE, "-",
                       NUM_WIDTH, "-");
            }
            
            // 及时关闭进程句柄，释放资源
            CloseHandle(hProcess);
        } while (Process32Next(hSnapshot, &pe32));
        
        // 关闭进程快照句柄
        CloseHandle(hSnapshot);
    }
    
    // 显示硬件信息
    void evos_hardware_info_display(const Configuration& config) {
        printf("\n[Hardware Information]\n");
        printf("-----------------------------------------------\n");
        
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        printf("\tProcessor Architecture: %s.\n", 
               getArchName(sysInfo.wProcessorArchitecture));
        printf("\tNumber of Processors: %lu.\n", sysInfo.dwNumberOfProcessors);
        printf("\tProcessor Level: %d.\n", sysInfo.wProcessorLevel);
        printf("\tProcessor Revision: %04x.\n", sysInfo.wProcessorRevision);
        
        // CPU品牌信息，使用简单的实现
        printf("\tCPU Brand: Unknown CPU Brand.\n");
        printf("\tPage Size: %lu bytes.\n", sysInfo.dwPageSize);
        printf("\tActive Processor Mask: 0x%lx.\n", sysInfo.dwActiveProcessorMask);
    }
    
    // 显示特定进程信息
    void evos_process_info_display(DWORD pid, const Configuration& config) {
        printf("\n[Process Information - PID: %lu]\n", pid);
        printf("-----------------------------------------------\n");
        
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                                      FALSE, pid);
        if (hProcess == NULL) {
            // 权限不足，显示基本信息
            printf("\tPID: %lu\n", pid);
            printf("\tWarning: Unable to open process (permission denied).\n");
            printf("\tBasic information only available due to insufficient permissions.\n");
            return;
        }
        
        // 尝试获取进程内存信息
            PROCESS_MEMORY_COUNTERS_EX pmc;
            pmc.cb = sizeof(pmc);
            if (GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
                printf("\tWorking Set Size: %s.\n", 
                       config.config_byte_to_str(pmc.WorkingSetSize).c_str());
                printf("\tPeak Working Set Size: %s.\n", 
                       config.config_byte_to_str(pmc.PeakWorkingSetSize).c_str());
                printf("\tPagefile Usage: %s.\n", 
                       config.config_byte_to_str(pmc.PagefileUsage).c_str());
                printf("\tPeak Pagefile Usage: %s.\n", 
                       config.config_byte_to_str(pmc.PeakPagefileUsage).c_str());
                printf("\tPrivate Usage: %s.\n", 
                       config.config_byte_to_str(pmc.PrivateUsage).c_str());
            } else {
            // 获取内存信息失败，显示基本信息
            printf("\tPID: %lu\n", pid);
            printf("\tWarning: Unable to get process memory information.\n");
        }
        
        CloseHandle(hProcess);
    }
    
    /**
     * 获取CPU使用率
     * 
     * @return DWORD CPU使用率百分比
     * @note 简化实现，返回固定值
     */
    DWORD evos_cpu_usage_get() {
        // 简化实现，返回固定值
        return 0;
    }
    
    // 显示GPU基本信息
    void evos_gpu_info_display() {
        printf("\n[GPU Information - Placeholder]\n");
        printf("-----------------------------------------------\n");
        printf("\tGPU monitoring is not fully implemented yet.\n");
        printf("\tThis is a placeholder implementation.\n");
        printf("\tPlease check back in future releases for full GPU monitoring.\n");
    }
    
    // 显示GPU高级信息
    void evos_gpu_advanced_info_display() {
        printf("\n[Advanced GPU Information - Placeholder]\n");
        printf("-----------------------------------------------\n");
        printf("\tAdvanced GPU monitoring is not fully implemented yet.\n");
        printf("\tThis is a placeholder implementation.\n");
    }
    
    // 显示GPU负载平衡建议
    void evos_gpu_balance_display() {
        printf("\n[GPU Load Balance Suggestions - Placeholder]\n");
        printf("-----------------------------------------------\n");
        printf("\tGPU load balance suggestions are not fully implemented yet.\n");
        printf("\tThis is a placeholder implementation.\n");
    }
    
    // 显示GPU功耗分析
    void evos_gpu_power_display() {
        printf("\n[GPU Power Analysis - Placeholder]\n");
        printf("-----------------------------------------------\n");
        printf("\tGPU power analysis is not fully implemented yet.\n");
        printf("\tThis is a placeholder implementation.\n");
    }
    
    // 显示GPU频率调优建议
    void evos_gpu_tuning_display() {
        printf("\n[GPU Frequency Tuning Suggestions - Placeholder]\n");
        printf("-----------------------------------------------\n");
        printf("\tGPU frequency tuning suggestions are not fully implemented yet.\n");
        printf("\tThis is a placeholder implementation.\n");
    }
    
    // 显示网络连接信息
    void evos_network_display() {
        printf("\n[Network Connection Information - Placeholder]\n");
        printf("-----------------------------------------------\n");
        printf("\tNetwork connection monitoring is not fully implemented yet.\n");
        printf("\tThis is a placeholder implementation.\n");
    }
    
    // 显示网络流量统计
    void evos_traffic_display() {
        printf("\n[Network Traffic Statistics - Placeholder]\n");
        printf("-----------------------------------------------\n");
        printf("\tNetwork traffic statistics are not fully implemented yet.\n");
        printf("\tThis is a placeholder implementation.\n");
    }
    
    // 显示端口使用信息
    void evos_ports_display() {
        printf("\n[Port Usage Information - Placeholder]\n");
        printf("-----------------------------------------------\n");
        printf("\tPort usage monitoring is not fully implemented yet.\n");
        printf("\tThis is a placeholder implementation.\n");
    }
    
    // 显示网络带宽统计
    void evos_net_bandwidth_display() {
        printf("\n[Network Bandwidth Statistics - Placeholder]\n");
        printf("-----------------------------------------------\n");
        printf("\tNetwork bandwidth statistics are not fully implemented yet.\n");
        printf("\tThis is a placeholder implementation.\n");
    }
    
    // 显示网络流量图表
    void evos_net_chart_display() {
        printf("\n[Network Traffic Chart - Placeholder]\n");
        printf("-----------------------------------------------\n");
        printf("\tNetwork traffic charts are not fully implemented yet.\n");
        printf("\tThis is a placeholder implementation.\n");
    }
    
    // 显示网络连接分析
    void evos_net_connection_display() {
        printf("\n[Network Connection Analysis - Placeholder]\n");
        printf("-----------------------------------------------\n");
        printf("\tNetwork connection analysis is not fully implemented yet.\n");
        printf("\tThis is a placeholder implementation.\n");
    }
    
    // 显示网络接口状态
    void evos_net_interfaces_display() {
        printf("\n[Network Interfaces Status - Placeholder]\n");
        printf("-----------------------------------------------\n");
        printf("\tNetwork interfaces status monitoring is not fully implemented yet.\n");
        printf("\tThis is a placeholder implementation.\n");
    }
    
    /**
     * 显示端口开放扫描结果
     * @param host 目标主机IP或域名
     * @param start_port 起始端口号
     * @param end_port 结束端口号
     * 
     * 功能：扫描指定主机的指定端口范围，显示开放的端口
     * 实现：使用TCP连接方式扫描端口，连接成功则端口开放
     */
    void evos_port_scan_display(const std::string& host, int start_port, int end_port) {
        printf("\n[Port Scan Results]\n");
        printf("-----------------------------------------------\n");
        printf("Scanning host: %s\n", host.c_str());
        printf("Port range: %d - %d\n", start_port, end_port);
        printf("-----------------------------------------------\n");
        
        // 初始化Winsock
        WSADATA wsaData;
        int wsaerr = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (wsaerr != 0) {
            printf("Error: Failed to initialize Winsock. Error code: %d\n", wsaerr);
            return;
        }
        
        // 创建socket
        SOCKET scanSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (scanSocket == INVALID_SOCKET) {
            printf("Error: Failed to create socket. Error code: %d\n", WSAGetLastError());
            WSACleanup();
            return;
        }
        
        // 设置socket为非阻塞模式
        u_long mode = 1; // 1 = non-blocking mode
        if (ioctlsocket(scanSocket, FIONBIO, &mode) != NO_ERROR) {
            printf("Error: Failed to set socket to non-blocking. Error code: %d\n", WSAGetLastError());
            closesocket(scanSocket);
            WSACleanup();
            return;
        }
        
        // 解析主机名
        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        
        // 尝试将主机名转换为IP地址
        struct hostent* host_entry = gethostbyname(host.c_str());
        if (host_entry == NULL) {
            // 尝试将字符串直接转换为IP地址
            if (inet_addr(host.c_str()) == INADDR_NONE) {
                printf("Error: Failed to resolve host: %s\n", host.c_str());
                closesocket(scanSocket);
                WSACleanup();
                return;
            } else {
                serverAddr.sin_addr.s_addr = inet_addr(host.c_str());
            }
        } else {
            // 提取IP地址
            serverAddr.sin_addr = *((struct in_addr*)host_entry->h_addr);
        }
        
        // 显示目标IP
        printf("Target IP: %s\n", inet_ntoa(serverAddr.sin_addr));
        printf("Scanning...\n");
        printf("-----------------------------------------------\n");
        
        // 扫描端口
        std::vector<int> open_ports;
        
        for (int port = start_port; port <= end_port; ++port) {
            serverAddr.sin_port = htons(port);
            
            // 尝试连接
            int connect_result = connect(scanSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
            
            if (connect_result == 0) {
                // 连接成功，端口开放
                open_ports.push_back(port);
            } else {
                // 连接失败，检查错误码
                int error = WSAGetLastError();
                if (error != WSAEWOULDBLOCK && error != WSAECONNREFUSED) {
                    // 其他错误，可能是网络问题
                    continue;
                }
            }
            
            // 短暂延迟，避免扫描过快
            Sleep(10);
        }
        
        // 恢复socket为阻塞模式（可选）
        mode = 0; // 0 = blocking mode
        ioctlsocket(scanSocket, FIONBIO, &mode);
        
        // 关闭socket
        closesocket(scanSocket);
        
        // 清理Winsock
        WSACleanup();
        
        // 显示结果
        if (open_ports.empty()) {
            printf("No open ports found in range %d-%d\n", start_port, end_port);
        } else {
            printf("Found %d open port(s):\n", open_ports.size());
            for (int port : open_ports) {
                printf("  Port %d: OPEN\n", port);
            }
        }
        
        printf("-----------------------------------------------\n");
    }
    
    // 全局配置实例
    Configuration globalConfig;
}
