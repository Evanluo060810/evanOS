# evanOS 

## 项目概述

evanOS是本人设计的一个电脑类监控系统 可以用作操作系统的作业 maybe？

## 项目技术栈

- **编程语言**: C++11
- **开发环境**: Windows系统 (支持Windows 10/11)
- **编译器**: GCC/Clang (交叉编译支持), MSVC
- **构建系统**: CMake 3.10+
- **核心库**: Windows API, 标准C++库
- **架构模式**: 分层架构、插件化设计、依赖注入

## 项目结构

```
evanOS/
├── src/
│   ├── core/               # 核心功能模块
│   │   ├── main.cpp       # 主程序入口点
│   │   ├── system_monitor.cpp # 系统监控功能实现
│   │   └── configuration.cpp # 配置管理模块
│   ├── ui/                 # 用户界面模块
│   │   ├── console_ui.cpp  # 控制台界面
│   │   └── display_utils.cpp # 显示工具
│   └── utils/              # 工具模块
│       ├── command_line.cpp # 命令行解析
│       └── string_utils.cpp # 字符串工具
├── include/                # 头文件目录
│   ├── core/              # 核心模块头文件
│   ├── ui/                # UI模块头文件
│   └── utils/             # 工具模块头文件
├── tests/                 # 测试代码
├── docs/                  # 项目文档
├── CMakeLists.txt         # 构建配置文件
├── README.md              # 项目说明文档
└── LICENSE                # 许可证文件
```

## 功能特性

### 系统监控
- 实时CPU使用率监控
- 内存使用情况统计
- 系统性能指标展示
- 进程列表和详细信息

### 硬件信息
- CPU架构和品牌信息
- 内存容量和使用情况
- GPU信息监控（支持NVIDIA、AMD、Intel）
- 网络接口状态

### 命令行界面
- 支持多种命令行参数
- 灵活的输出格式
- 可配置的显示选项
- 实时数据更新

## 构建指南

### 前提条件

- CMake 3.10或更高版本
- C++11兼容的编译器
- Windows SDK（用于Windows API）

### 构建步骤

1. 克隆仓库
   ```bash
   git clone <repository-url>
   cd evanOS
   ```

2. 创建构建目录
   ```bash
   mkdir build
   cd build
   ```

3. 配置CMake
   ```bash
   cmake ..
   ```

4. 编译项目
   ```bash
   cmake --build . --config Release
   ```

5. 运行程序
   ```bash
   ..\bin\evanOS.exe --help
   ```

## 使用说明

### 命令行参数

```
evanOS [options]

Options:
  -p, --perf         show system performance value info.
  -s, --sys          show system memory info.
  -t, --total        show total memory usage.
  -e, --each         show each process info.
  -w, --hardware     show PC hardware information.
  -g, --gpu          show GPU information.
  -G, --gpu-advanced show advanced GPU information.
  -z, --gpu-balance  show GPU load balance suggestions.
  -v, --gpu-power    show GPU power analysis.
  -u, --gpu-tuning   show GPU frequency tuning suggestions.
  -n, --network      show network connections info.
  -f, --traffic      show network traffic statistics.
  -o, --ports        show port usage info.
  -B, --net-bandwidth show network bandwidth statistics.
  -C, --net-chart    show network traffic chart.
  -A, --net-connection show network connection analysis.
  -I, --net-interfaces show network interfaces status.
  -i, --inquire      Inquire the selected process info.
  -l, --loop         loop this program from [1-65535] second.
  -y, --type         Set the show byte type[0=B,1=KB,2=MB,3=GB],Auto decide if not use this.
  -?, --help         show help message.
  -c, --copyright    show copyright and license information.
  -a, --all          show all info.
```

### 示例

显示所有系统信息：
```bash
evanOS --all
```

显示系统性能信息：
```bash
evanOS --perf
```

显示GPU信息：
```bash
evanOS --gpu
```

## 许可证

Apache License 2.0

## 版本历史

### v1.0.0 (2026-01-02)
- 初始版本发布
- 支持基本的系统监控功能
- GPU监控支持
- 命令行界面
- 分层架构设计
