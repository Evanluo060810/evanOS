// Copyright 2026 Evanluo
// Licensed under the Apache License, Version 2.0

#include "core/gpu_monitor.h"
#include <iostream>
#include <stdexcept>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

// GPU监控管理器实现
GPUMonitorManager::GPUMonitorManager() : is_initialized(false) {
}

GPUMonitorManager::~GPUMonitorManager() {
    cleanup();
}

bool GPUMonitorManager::initialize() {
    // 创建并初始化各厂商GPU监控器
    NVIDIA_GPUMonitor* nvidia_monitor = new NVIDIA_GPUMonitor();
    if (nvidia_monitor->initialize()) {
        gpu_monitors.push_back(nvidia_monitor);
    } else {
        delete nvidia_monitor;
    }
    
    AMD_GPUMonitor* amd_monitor = new AMD_GPUMonitor();
    if (amd_monitor->initialize()) {
        gpu_monitors.push_back(amd_monitor);
    } else {
        delete amd_monitor;
    }
    
    Intel_GPUMonitor* intel_monitor = new Intel_GPUMonitor();
    if (intel_monitor->initialize()) {
        gpu_monitors.push_back(intel_monitor);
    } else {
        delete intel_monitor;
    }
    
    is_initialized = !gpu_monitors.empty();
    return is_initialized;
}

std::vector<IGPUMonitor*> GPUMonitorManager::getGPUMonitors() {
    return gpu_monitors;
}

bool GPUMonitorManager::getAllGPUInfo(std::vector<GPUInfo>& gpu_info_list) {
    if (!is_initialized) {
        return false;
    }
    
    for (auto monitor : gpu_monitors) {
        std::vector<GPUInfo> temp_list;
        if (monitor->getAllGPUInfo(temp_list)) {
            gpu_info_list.insert(gpu_info_list.end(), temp_list.begin(), temp_list.end());
        }
    }
    
    return !gpu_info_list.empty();
}

void GPUMonitorManager::cleanup() {
    for (auto monitor : gpu_monitors) {
        monitor->cleanup();
        delete monitor;
    }
    gpu_monitors.clear();
    is_initialized = false;
}

// NVIDIA GPU监控实现
NVIDIA_GPUMonitor::NVIDIA_GPUMonitor() : 
    nvml_lib(nullptr), 
    is_initialized(false), 
    device_count(0) {
    // 初始化函数指针为null
    nvmlInit = nullptr;
    nvmlShutdown = nullptr;
    nvmlDeviceGetCount = nullptr;
    nvmlDeviceGetHandleByIndex = nullptr;
    nvmlDeviceGetName = nullptr;
    nvmlDeviceGetMemoryInfo = nullptr;
    nvmlDeviceGetUtilizationRates = nullptr;
    nvmlDeviceGetTemperature = nullptr;
    nvmlDeviceGetPowerUsage = nullptr;
    nvmlDeviceGetClockInfo = nullptr;
    nvmlSystemGetDriverVersion = nullptr;
}

NVIDIA_GPUMonitor::~NVIDIA_GPUMonitor() {
    cleanup();
}

bool NVIDIA_GPUMonitor::initialize() {
    // 加载NVML库
    #ifdef _WIN32
        nvml_lib = LoadLibrary(TEXT("nvidia-ml.dll"));
        if (nvml_lib == nullptr) {
            // 尝试从系统目录加载
            nvml_lib = LoadLibrary(TEXT("C:\\Windows\\System32\\nvidia-ml.dll"));
            if (nvml_lib == nullptr) {
                return false;
            }
        }
    #else
        nvml_lib = dlopen("libnvidia-ml.so.1", RTLD_LAZY);
        if (nvml_lib == nullptr) {
            return false;
        }
    #endif
    
    // 获取NVML函数指针
    #ifdef _WIN32
        nvmlInit = (nvmlInit_t)GetProcAddress((HMODULE)nvml_lib, "nvmlInit");
        nvmlShutdown = (nvmlShutdown_t)GetProcAddress((HMODULE)nvml_lib, "nvmlShutdown");
        nvmlDeviceGetCount = (nvmlDeviceGetCount_t)GetProcAddress((HMODULE)nvml_lib, "nvmlDeviceGetCount");
        nvmlDeviceGetHandleByIndex = (nvmlDeviceGetHandleByIndex_t)GetProcAddress((HMODULE)nvml_lib, "nvmlDeviceGetHandleByIndex");
        nvmlDeviceGetName = (nvmlDeviceGetName_t)GetProcAddress((HMODULE)nvml_lib, "nvmlDeviceGetName");
        nvmlDeviceGetMemoryInfo = (nvmlDeviceGetMemoryInfo_t)GetProcAddress((HMODULE)nvml_lib, "nvmlDeviceGetMemoryInfo");
        nvmlDeviceGetUtilizationRates = (nvmlDeviceGetUtilizationRates_t)GetProcAddress((HMODULE)nvml_lib, "nvmlDeviceGetUtilizationRates");
        nvmlDeviceGetTemperature = (nvmlDeviceGetTemperature_t)GetProcAddress((HMODULE)nvml_lib, "nvmlDeviceGetTemperature");
        nvmlDeviceGetPowerUsage = (nvmlDeviceGetPowerUsage_t)GetProcAddress((HMODULE)nvml_lib, "nvmlDeviceGetPowerUsage");
        nvmlDeviceGetClockInfo = (nvmlDeviceGetClockInfo_t)GetProcAddress((HMODULE)nvml_lib, "nvmlDeviceGetClockInfo");
        nvmlSystemGetDriverVersion = (nvmlSystemGetDriverVersion_t)GetProcAddress((HMODULE)nvml_lib, "nvmlSystemGetDriverVersion");
    #else
        nvmlInit = (nvmlInit_t)dlsym(nvml_lib, "nvmlInit");
        nvmlShutdown = (nvmlShutdown_t)dlsym(nvml_lib, "nvmlShutdown");
        nvmlDeviceGetCount = (nvmlDeviceGetCount_t)dlsym(nvml_lib, "nvmlDeviceGetCount");
        nvmlDeviceGetHandleByIndex = (nvmlDeviceGetHandleByIndex_t)dlsym(nvml_lib, "nvmlDeviceGetHandleByIndex");
        nvmlDeviceGetName = (nvmlDeviceGetName_t)dlsym(nvml_lib, "nvmlDeviceGetName");
        nvmlDeviceGetMemoryInfo = (nvmlDeviceGetMemoryInfo_t)dlsym(nvml_lib, "nvmlDeviceGetMemoryInfo");
        nvmlDeviceGetUtilizationRates = (nvmlDeviceGetUtilizationRates_t)dlsym(nvml_lib, "nvmlDeviceGetUtilizationRates");
        nvmlDeviceGetTemperature = (nvmlDeviceGetTemperature_t)dlsym(nvml_lib, "nvmlDeviceGetTemperature");
        nvmlDeviceGetPowerUsage = (nvmlDeviceGetPowerUsage_t)dlsym(nvml_lib, "nvmlDeviceGetPowerUsage");
        nvmlDeviceGetClockInfo = (nvmlDeviceGetClockInfo_t)dlsym(nvml_lib, "nvmlDeviceGetClockInfo");
        nvmlSystemGetDriverVersion = (nvmlSystemGetDriverVersion_t)dlsym(nvml_lib, "nvmlSystemGetDriverVersion");
    #endif
    
    // 检查所有必要函数是否获取成功
    if (!nvmlInit || !nvmlShutdown || !nvmlDeviceGetCount || !nvmlDeviceGetHandleByIndex ||
        !nvmlDeviceGetName || !nvmlDeviceGetMemoryInfo || !nvmlDeviceGetUtilizationRates ||
        !nvmlDeviceGetTemperature || !nvmlDeviceGetPowerUsage || !nvmlDeviceGetClockInfo ||
        !nvmlSystemGetDriverVersion) {
        return false;
    }
    
    // 初始化NVML
    if (nvmlInit() != 0) {
        return false;
    }
    
    // 获取设备数量
    if (nvmlDeviceGetCount(&device_count) != 0) {
        nvmlShutdown();
        return false;
    }
    
    is_initialized = true;
    return true;
}

bool NVIDIA_GPUMonitor::getGPUInfo(GPUInfo& gpu_info) {
    if (!is_initialized || device_count == 0) {
        return false;
    }
    
    // 获取第一个GPU的信息
    void* device;
    if (nvmlDeviceGetHandleByIndex(0, &device) != 0) {
        return false;
    }
    
    // 获取GPU名称
    char name[256];
    if (nvmlDeviceGetName(device, name, sizeof(name)) != 0) {
        return false;
    }
    gpu_info.name = name;
    gpu_info.vendor = "NVIDIA";
    
    // 获取驱动版本
    char driver_version[256];
    if (nvmlSystemGetDriverVersion(driver_version, sizeof(driver_version)) != 0) {
        gpu_info.driver_version = "Unknown";
    } else {
        gpu_info.driver_version = driver_version;
    }
    
    // 获取显存信息
    struct NVMLMemoryInfo {
        unsigned long long total;  // 总显存字节数
        unsigned long long free;  // 可用显存字节数
        unsigned long long used;  // 已用显存字节数
    } memory_info;
    
    if (nvmlDeviceGetMemoryInfo(device, &memory_info) != 0) {
        return false;
    }
    
    gpu_info.memory_total = static_cast<unsigned int>(memory_info.total / (1024 * 1024));
    gpu_info.memory_used = static_cast<unsigned int>(memory_info.used / (1024 * 1024));
    gpu_info.memory_free = static_cast<unsigned int>(memory_info.free / (1024 * 1024));
    
    // 获取使用率
    struct NVMLUtilizationRates {
        unsigned int gpu;    // GPU使用率
        unsigned int memory; // 显存使用率
    } utilization;
    
    if (nvmlDeviceGetUtilizationRates(device, &utilization) != 0) {
        return false;
    }
    
    gpu_info.utilization = static_cast<float>(utilization.gpu);
    
    // 获取温度
    unsigned int temp;
    if (nvmlDeviceGetTemperature(device, 0, &temp) != 0) {
        return false;
    }
    gpu_info.temperature = static_cast<float>(temp);
    
    // 获取功耗
    unsigned int power_usage;
    if (nvmlDeviceGetPowerUsage(device, &power_usage) != 0) {
        gpu_info.power_usage = 0.0f;
    } else {
        gpu_info.power_usage = static_cast<float>(power_usage) / 1000.0f; // 转换为瓦
    }
    
    // 获取核心频率
    unsigned int clock_core;
    if (nvmlDeviceGetClockInfo(device, 0, &clock_core) != 0) {
        gpu_info.clock_core = 0.0f;
    } else {
        gpu_info.clock_core = static_cast<float>(clock_core);
    }
    
    // 获取显存频率
    unsigned int clock_memory;
    if (nvmlDeviceGetClockInfo(device, 1, &clock_memory) != 0) {
        gpu_info.clock_memory = 0.0f;
    } else {
        gpu_info.clock_memory = static_cast<float>(clock_memory);
    }
    
    return true;
}

bool NVIDIA_GPUMonitor::getAllGPUInfo(std::vector<GPUInfo>& gpu_info_list) {
    if (!is_initialized || device_count == 0) {
        return false;
    }
    
    for (unsigned int i = 0; i < device_count; ++i) {
        GPUInfo gpu_info;
        
        // 获取GPU句柄
        void* device;
        if (nvmlDeviceGetHandleByIndex(i, &device) != 0) {
            continue;
        }
        
        // 获取GPU名称
        char name[256];
        if (nvmlDeviceGetName(device, name, sizeof(name)) != 0) {
            continue;
        }
        gpu_info.name = name;
        gpu_info.vendor = "NVIDIA";
        
        // 获取驱动版本
        char driver_version[256];
        if (nvmlSystemGetDriverVersion(driver_version, sizeof(driver_version)) != 0) {
            gpu_info.driver_version = "Unknown";
        } else {
            gpu_info.driver_version = driver_version;
        }
        
        // 获取显存信息
        struct NVMLMemoryInfo {
            unsigned long long total;  // 总显存字节数
            unsigned long long free;  // 可用显存字节数
            unsigned long long used;  // 已用显存字节数
        } memory_info;
        
        if (nvmlDeviceGetMemoryInfo(device, &memory_info) != 0) {
            continue;
        }
        
        gpu_info.memory_total = static_cast<unsigned int>(memory_info.total / (1024 * 1024));
        gpu_info.memory_used = static_cast<unsigned int>(memory_info.used / (1024 * 1024));
        gpu_info.memory_free = static_cast<unsigned int>(memory_info.free / (1024 * 1024));
        
        // 获取使用率
        struct NVMLUtilizationRates {
            unsigned int gpu;    // GPU使用率
            unsigned int memory; // 显存使用率
        } utilization;
        
        if (nvmlDeviceGetUtilizationRates(device, &utilization) != 0) {
            continue;
        }
        
        gpu_info.utilization = static_cast<float>(utilization.gpu);
        
        // 获取温度
        unsigned int temp;
        if (nvmlDeviceGetTemperature(device, 0, &temp) != 0) {
            continue;
        }
        gpu_info.temperature = static_cast<float>(temp);
        
        // 获取功耗
        unsigned int power_usage;
        if (nvmlDeviceGetPowerUsage(device, &power_usage) != 0) {
            gpu_info.power_usage = 0.0f;
        } else {
            gpu_info.power_usage = static_cast<float>(power_usage) / 1000.0f; // 转换为瓦
        }
        
        // 获取核心频率
        unsigned int clock_core;
        if (nvmlDeviceGetClockInfo(device, 0, &clock_core) != 0) {
            gpu_info.clock_core = 0.0f;
        } else {
            gpu_info.clock_core = static_cast<float>(clock_core);
        }
        
        // 获取显存频率
        unsigned int clock_memory;
        if (nvmlDeviceGetClockInfo(device, 1, &clock_memory) != 0) {
            gpu_info.clock_memory = 0.0f;
        } else {
            gpu_info.clock_memory = static_cast<float>(clock_memory);
        }
        
        gpu_info_list.push_back(gpu_info);
    }
    
    return !gpu_info_list.empty();
}

void NVIDIA_GPUMonitor::cleanup() {
    if (is_initialized && nvmlShutdown) {
        nvmlShutdown();
    }
    
    if (nvml_lib) {
        #ifdef _WIN32
            FreeLibrary((HMODULE)nvml_lib);
        #else
            dlclose(nvml_lib);
        #endif
        nvml_lib = nullptr;
    }
    
    is_initialized = false;
    device_count = 0;
}

// AMD GPU监控实现
AMD_GPUMonitor::AMD_GPUMonitor() : is_initialized(false) {
}

AMD_GPUMonitor::~AMD_GPUMonitor() {
    cleanup();
}

bool AMD_GPUMonitor::initialize() {
    // 目前AMD GPU监控尚未实现，返回false
    // TODO: 实现AMD GPU监控
    return false;
}

bool AMD_GPUMonitor::getGPUInfo(GPUInfo& gpu_info) {
    return false;
}

bool AMD_GPUMonitor::getAllGPUInfo(std::vector<GPUInfo>& gpu_info_list) {
    return false;
}

void AMD_GPUMonitor::cleanup() {
    is_initialized = false;
}

// Intel GPU监控实现
Intel_GPUMonitor::Intel_GPUMonitor() : is_initialized(false) {
}

Intel_GPUMonitor::~Intel_GPUMonitor() {
    cleanup();
}

bool Intel_GPUMonitor::initialize() {
    // 目前Intel GPU监控尚未实现，返回false
    // TODO: 实现Intel GPU监控
    return false;
}

bool Intel_GPUMonitor::getGPUInfo(GPUInfo& gpu_info) {
    return false;
}

bool Intel_GPUMonitor::getAllGPUInfo(std::vector<GPUInfo>& gpu_info_list) {
    return false;
}

void Intel_GPUMonitor::cleanup() {
    is_initialized = false;
}
