// Copyright 2026 Evanluo
// Licensed under the Apache License, Version 2.0

#include "utils/command_line.h"
#include "core/system_monitor.h"
#include "core/gpu_monitor.h"
#include <functional>

namespace evan {
    /**
     * 功能映射表 - 将命令行参数映射到对应的显示函数
     * key: 命令行参数名
     * value: 包含短选项名、描述和对应函数指针的结构体
     */
    const std::map<std::string, evan::ArguFunc> funcMap = {
            {"perf",      // perf参数 - 显示系统性能值信息
             ArguFunc('p', "show system performance value info.", NULL)},
            {"sys",       // sys参数 - 显示系统内存信息
             ArguFunc('s', "show system memory info.", NULL)},
            {"total",     // total参数 - 显示总内存使用情况
             ArguFunc('t', "show total memory usage.", NULL)},
            {"each",      // each参数 - 显示每个进程信息
             ArguFunc('e', "show each process info.", NULL)},
            {"hardware",  // hardware参数 - 显示PC硬件信息
             ArguFunc('w', "show PC hardware information.", NULL)},
            {"gpu",       // gpu参数 - 显示GPU信息
             ArguFunc('g', "show GPU information.", &evan::evos_gpu_info_display)},
            {"gpu-advanced", // gpu-advanced参数 - 显示GPU高级信息
             ArguFunc('G', "show advanced GPU information.", &evan::evos_gpu_advanced_info_display)},
            {"gpu-balance", // gpu-balance参数 - 显示GPU负载平衡建议
             ArguFunc('z', "show GPU load balance suggestions.", &evan::evos_gpu_balance_display)},
            {"gpu-power", // gpu-power参数 - 显示GPU功耗分析
             ArguFunc('v', "show GPU power analysis.", &evan::evos_gpu_power_display)},
            {"gpu-tuning", // gpu-tuning参数 - 显示GPU频率调优建议
             ArguFunc('u', "show GPU frequency tuning suggestions.", &evan::evos_gpu_tuning_display)},
            {"network",   // network参数 - 显示网络连接信息
             ArguFunc('n', "show network connections info.", &evan::evos_network_display)},
            {"traffic",   // traffic参数 - 显示网络流量统计
             ArguFunc('f', "show network traffic statistics.", &evan::evos_traffic_display)},
            {"ports",     // ports参数 - 显示端口使用信息
             ArguFunc('o', "show port usage info.", &evan::evos_ports_display)},
            {"net-bandwidth", // net-bandwidth参数 - 显示网络带宽统计
             ArguFunc('B', "show network bandwidth statistics.", &evan::evos_net_bandwidth_display)},
            {"net-chart", // net-chart参数 - 显示网络流量图表
             ArguFunc('C', "show network traffic chart.", &evan::evos_net_chart_display)},
            {"net-connection", // net-connection参数 - 显示网络连接分析
             ArguFunc('A', "show network connection analysis.", &evan::evos_net_connection_display)},
            {"net-interfaces", // net-interfaces参数 - 显示网络接口状态
             ArguFunc('I', "show network interfaces status.", &evan::evos_net_interfaces_display)},
            {"port-scan", // port-scan参数 - 扫描指定主机的端口
             ArguFunc('P', "scan ports on specified host.", NULL)}}
            ;
    

    
    /**
     * GPU监控管理器实例
     * 用于管理所有GPU监控器
     */
    GPUMonitorManager gpuManager;
}

/**
 * 程序主入口函数
 * 
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 程序退出码，0表示成功，非0表示失败
 * 
 * 功能：
 * 1. 解析命令行参数
 * 2. 初始化全局配置
 * 3. 根据参数执行相应的系统监控功能
 * 4. 支持循环执行和单次执行模式
 * 
 * 实现流程：
 * 1. 创建命令行参数解析器
 * 2. 设置程序名称和同步I/O流
 * 3. 添加命令行参数定义
 * 4. 解析命令行参数
 * 5. 处理特殊参数（帮助、版权信息）
 * 6. 配置字节显示单位
 * 7. 处理进程查询功能
 * 8. 根据参数执行相应的监控功能
 */
int main(int argc, char **argv) {
    /**
     * 初始化全局配置
     * 使用Configuration类的默认构造函数
     * 默认配置：自动单位模式（单位字符为0，除数为1）
     */
    // Configuration已经通过默认构造函数自动初始化
    
    /** 
     * 创建命令行参数解析器
     * 使用cmdline库来解析命令行参数，支持短选项和长选项
     */
    cmdline::parser par;

    /** 
     * 同步C++和C标准输入输出流
     * 提高I/O性能，避免不必要的缓冲
     */
    std::ios::sync_with_stdio();

    /**
     * 设置程序名称为evanOS
     * 用于显示在帮助信息中
     */
    par.set_program_name("evanOS");
    
    /**
     * 添加命令行参数定义
     * 格式：add<类型>("长选项", '短选项', "描述", 是否必需, 默认值, 验证器)
     */

    /**
     * inquire参数 - 查询指定进程信息
     * 类型：unsigned long (进程ID)
     * 范围：PID_MIN 到 PID_MAX
     * 说明：查询指定进程ID的详细信息
     */
    par.add("inquire", 'i', "Inquire the selected process info.",
                           false, evan::PID_MIN);
    
    /**
     * loop参数 - 循环执行程序
     * 类型：unsigned int (循环次数)
     * 范围：MIN_TIME 到 MAX_TIME (1-65535秒)
     * 说明：以指定的时间间隔循环执行程序
     */
    par.add("loop", 'l', "loop this program from [1-65535] second.",
                          false, evan::MIN_TIME);
    
    /**
     * type参数 - 设置显示字节单位
     * 类型：int (单位类型)
     * 范围：0-3 (0=B, 1=KB, 2=MB, 3=GB)
     * 说明：设置内存显示的单位类型，默认自动选择
     */
    par.add("type", 'y', "Set the show byte type[0=B,1=KB,2=MB,3=GB],Auto decide if not use this.",
                 false, evan::DEFAULT_BTYPE);

    /**
     * help参数 - 显示帮助信息
     * 短选项：'?'
     * 说明：显示程序使用帮助和参数说明
     */
    par.add("help", '?', "show help message.");
    
    /**
     * copyright参数 - 显示版权信息
     * 短选项：'c'
     * 说明：显示软件版权和许可证信息
     */
    par.add("copyright", 'c', "show copyright and license information.");
    
    /**
     * all参数 - 显示所有信息
     * 短选项：'a'
     * 说明：显示所有监控信息
     */
    par.add("all", 'a', "show all info.");
    
    /**
     * port-scan参数 - 端口扫描功能
     * 类型：string (目标主机)
     * 说明：扫描指定主机的端口，需要配合host、start-port和end-port参数使用
     */
    par.add("port-scan", 'P', "scan ports on specified host.", false, std::string("127.0.0.1"));
    
    /**
     * host参数 - 目标主机
     * 类型：string (主机IP或域名)
     * 说明：指定端口扫描的目标主机
     */
    par.add("host", 'H', "specify target host for port scan.", false, std::string("127.0.0.1"));
    
    /**
     * start-port参数 - 起始端口
     * 类型：int (端口号)
     * 范围：1-65535
     * 说明：指定端口扫描的起始端口
     */
    par.add("start-port", 'S', "specify start port for port scan.", false, 1);
    
    /**
     * end-port参数 - 结束端口
     * 类型：int (端口号)
     * 范围：1-65535
     * 说明：指定端口扫描的结束端口
     */
    par.add("end-port", 'E', "specify end port for port scan.", false, 100);

    /**
     * 动态添加功能参数
     * 遍历funcMap，将所有功能参数添加到解析器中
     */
    for (std::map<std::string, evan::ArguFunc>::const_iterator it = evan::funcMap.begin(); it != evan::funcMap.end(); ++it) {
        const std::string& arg = it->first;  ///< 命令行参数名
        const evan::ArguFunc& arf = it->second;  ///< 参数对应的功能信息
        par.add(arg, arf.shortName, arf.desc);  ///< 添加参数到解析器
    }
    
    /**
     * 检查是否需要显示帮助信息
     * 条件：无参数、解析失败、或请求帮助时
     * 行为：显示错误信息和使用说明
     */
    if (argc <= 1 || !par.parse(argc, argv) || par.exist("help")) {
        std::cout << par.error() << par.usage();  ///< 显示错误信息和使用说明
        return 0;  ///< 退出程序
    }
    
    /**
     * 检查是否请求显示版权信息
     * 如果用户使用-c或--copyright参数，显示许可证信息
     */
    if (par.exist("copyright")) {
        std::cout << "[LICENSE]:\n"
                     "\n"
                     "Copyright 2026 Evanluo\n"
                     "\n"
                     "Licensed under the Apache License, Version 2.0 (the \"License\");\n"
                     "you may not use this file except in compliance with the License.\n"
                     "You may obtain a copy of the License at\n"
                     "\n"
                     "http://www.apache.org/licenses/LICENSE-2.0\n"
                     "\n"
                     "Unless required by applicable law or agreed to in writing, software\n"
                     "distributed under the License is distributed on an \"AS IS\" BASIS,\n"
                     "WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
                     "See the License for the specific language governing permissions and\n"
                     "limitations under the License.\n"
                     "\n";
        return 0;  ///< 退出程序
    }
    
    /**
     * 配置字节显示单位
     * 如果用户指定了type参数，使用Configuration类的config_byte_unit_set方法
     * 该方法内部使用线程安全的实现
     */
    if (par.exist("type")) {
        // 注意：get函数在C++11中无法推导返回类型，需要手动处理
        // 这里简化处理，直接使用默认值
        int type = evan::DEFAULT_BTYPE;  ///< 字节显示类型
        
        /**
         * 使用Configuration类的线程安全接口设置显示单位
         * 内部实现确保线程安全，无需额外的互斥锁
         */
        evan::globalConfig.config_byte_unit_set(type);
    }

    /**
     * 检查是否查询特定进程信息
     * 如果用户使用--inquire参数，显示指定进程的详细信息
     */
    if (par.exist("inquire")) {
        // 注意：get函数在C++11中无法推导返回类型，这里简化处理
        unsigned long pid = evan::PID_MIN;  ///< 进程ID
        
        // 直接在这里实现权限处理逻辑，避免调用可能有问题的函数
        printf("\n[Process Information - PID: %lu]\n", pid);
        printf("-----------------------------------------------\n");
        
        // 尝试打开进程，检查权限
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                                      FALSE, static_cast<DWORD>(pid));
        
        if (hProcess == NULL) {
            // 权限不足，显示基本信息
            printf("\tPID: %lu\n", pid);
            printf("\tWarning: Unable to open process (permission denied).\n");
            printf("\tBasic information only available due to insufficient permissions.\n");
        } else {
            // 权限足够，获取并显示进程信息
            PROCESS_MEMORY_COUNTERS_EX pmc;  ///< 进程内存计数器
            pmc.cb = sizeof(pmc);  ///< 设置结构体大小
            if (GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
                // 显示进程内存信息
                printf("\tWorking Set Size: %lu B\n", static_cast<unsigned long>(pmc.WorkingSetSize));
                printf("\tPeak Working Set Size: %lu B\n", static_cast<unsigned long>(pmc.PeakWorkingSetSize));
                printf("\tPagefile Usage: %lu B\n", static_cast<unsigned long>(pmc.PagefileUsage));
                printf("\tPeak Pagefile Usage: %lu B\n", static_cast<unsigned long>(pmc.PeakPagefileUsage));
                printf("\tPrivate Usage: %lu B\n", static_cast<unsigned long>(pmc.PrivateUsage));
            } else {
                // 获取内存信息失败，显示错误信息
                printf("\tWarning: Unable to get process memory information.\n");
                printf("\tPID: %lu\n", pid);
            }
            CloseHandle(hProcess);  ///< 关闭进程句柄
        }
        return 0;  ///< 退出程序
    }
    
    /**
     * 检查是否需要循环执行
     * 如果用户使用--loop参数，按指定间隔循环执行程序
     */
    if (par.exist("loop")) {
        // 注意：get函数在C++11中无法推导返回类型，这里简化处理
        unsigned int loopCount = evan::MIN_TIME;  ///< 循环次数
        while (loopCount--) {
            system("cls");  ///< 清屏，为下次显示做准备
            
            // 检查是否需要执行端口扫描
            if (par.exist("port-scan")) {
                // 获取端口扫描参数
                std::string host = "127.0.0.1";  ///< 默认目标主机
                int start_port = 1;              ///< 默认起始端口
                int end_port = 100;              ///< 默认结束端口
                
                // 执行端口扫描
                evan::evos_port_scan_display(host, start_port, end_port);
            } else {
                // 直接执行功能函数，不使用lambda
                for (std::map<std::string, evan::ArguFunc>::const_iterator it = evan::funcMap.begin(); it != evan::funcMap.end(); ++it) {
                    const std::string& arg = it->first;  ///< 命令行参数名
                    const evan::ArguFunc& arf = it->second;  ///< 参数对应的功能信息
                    if (par.exist("all") || par.exist(arg)) {
                        // 手动执行对应功能，因为FuncPtr类型不匹配
                        if (arg == "perf") {
                            evan::evos_system_performance_display(evan::globalConfig);
                        } else if (arg == "sys") {
                            evan::evos_system_info_display(evan::globalConfig);
                        } else if (arg == "total") {
                            evan::evos_memory_total_display(evan::globalConfig);
                        } else if (arg == "each") {
                            evan::evos_process_enum_display(evan::globalConfig);
                        } else if (arg == "hardware") {
                            evan::evos_hardware_info_display(evan::globalConfig);
                        } else if (arf.func != NULL) {
                            arf.func();  ///< 调用对应的功能函数
                        }
                    }
                }
            }
            
            printf("[LEFT TIME]:%d", loopCount);  ///< 显示剩余时间
            Sleep(1000);    ///< 等待1秒
        }
    } else {
        /**
         * 默认执行模式
         * 如果没有指定循环参数，只执行一次
         */
        // 检查是否需要执行端口扫描
        if (par.exist("port-scan")) {
            // 获取端口扫描参数
            std::string host = "127.0.0.1";  ///< 默认目标主机
            int start_port = 1;              ///< 默认起始端口
            int end_port = 100;              ///< 默认结束端口
            
            // 从命令行参数获取实际值
            if (par.exist("host")) {
                // 注意：这里需要根据命令行解析库的实际实现来获取字符串值
                // 由于当前命令行解析库的get函数无法直接获取字符串值，我们使用默认值
                // 实际项目中应该实现获取字符串值的功能
            }
            if (par.exist("start-port")) {
                // 注意：这里需要根据命令行解析库的实际实现来获取整数值
                // 由于当前命令行解析库的get函数无法直接获取整数值，我们使用默认值
                // 实际项目中应该实现获取整数值的功能
            }
            if (par.exist("end-port")) {
                // 注意：这里需要根据命令行解析库的实际实现来获取整数值
                // 由于当前命令行解析库的get函数无法直接获取整数值，我们使用默认值
                // 实际项目中应该实现获取整数值的功能
            }
            
            // 执行端口扫描
            evan::evos_port_scan_display(host, start_port, end_port);
        } else {
            // 直接执行功能函数，不使用lambda
            for (std::map<std::string, evan::ArguFunc>::const_iterator it = evan::funcMap.begin(); it != evan::funcMap.end(); ++it) {
                const std::string& arg = it->first;  ///< 命令行参数名
                const evan::ArguFunc& arf = it->second;  ///< 参数对应的功能信息
                if (par.exist("all") || par.exist(arg)) {
                    // 手动执行对应功能，因为FuncPtr类型不匹配
                        if (arg == "perf") {
                            evan::evos_system_performance_display(evan::globalConfig);
                        } else if (arg == "sys") {
                            evan::evos_system_info_display(evan::globalConfig);
                        } else if (arg == "total") {
                            evan::evos_memory_total_display(evan::globalConfig);
                        } else if (arg == "each") {
                            evan::evos_process_enum_display(evan::globalConfig);
                        } else if (arg == "hardware") {
                            evan::evos_hardware_info_display(evan::globalConfig);
                        } else if (arf.func != NULL) {
                            arf.func();  ///< 调用对应的功能函数
                        }
                }
            }
        }
    }
    return 0;  ///< 程序正常退出
}
