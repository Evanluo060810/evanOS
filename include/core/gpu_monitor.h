// Copyright 2026 Evanluo
// Licensed under the Apache License, Version 2.0

#ifndef GPU_MONITOR_H
#define GPU_MONITOR_H

#include <string>
#include <vector>

// GPU信息结构体
typedef struct {
    std::string name;           // GPU名称
    std::string vendor;         // GPU厂商
    std::string driver_version; // 驱动版本
    unsigned int memory_total;  // 总显存（MB）
    unsigned int memory_used;   // 已用显存（MB）
    unsigned int memory_free;   // 可用显存（MB）
    float utilization;          // 使用率（%）
    float temperature;          // 温度（℃）
    float power_usage;          // 功耗（W）
    float clock_core;           // 核心频率（MHz）
    float clock_memory;         // 显存频率（MHz）
} GPUInfo;

// GPU监控抽象接口
class IGPUMonitor {
public:
    virtual ~IGPUMonitor() {}
    
    // 初始化GPU监控
    virtual bool initialize() = 0;
    
    // 获取GPU信息
    virtual bool getGPUInfo(GPUInfo& gpu_info) = 0;
    
    // 获取所有GPU信息
    virtual bool getAllGPUInfo(std::vector<GPUInfo>& gpu_info_list) = 0;
    
    // 清理资源
    virtual void cleanup() = 0;
    
    // 获取厂商名称
    virtual std::string getVendorName() const = 0;
};

// NVIDIA GPU监控实现
class NVIDIA_GPUMonitor : public IGPUMonitor {
public:
    NVIDIA_GPUMonitor();
    ~NVIDIA_GPUMonitor();
    
    bool initialize() override;
    bool getGPUInfo(GPUInfo& gpu_info) override;
    bool getAllGPUInfo(std::vector<GPUInfo>& gpu_info_list) override;
    void cleanup() override;
    std::string getVendorName() const override { return "NVIDIA"; }
    
private:
    void* nvml_lib; // NVML库句柄
    // NVML函数指针声明
    typedef int (*nvmlInit_t)();
    typedef int (*nvmlShutdown_t)();
    typedef int (*nvmlDeviceGetCount_t)(unsigned int* count);
    typedef int (*nvmlDeviceGetHandleByIndex_t)(unsigned int index, void** device);
    typedef int (*nvmlDeviceGetName_t)(void* device, char* name, unsigned int length);
    typedef int (*nvmlDeviceGetMemoryInfo_t)(void* device, void* memory_info);
    typedef int (*nvmlDeviceGetUtilizationRates_t)(void* device, void* utilization);
    typedef int (*nvmlDeviceGetTemperature_t)(void* device, int sensor_type, unsigned int* temp);
    typedef int (*nvmlDeviceGetPowerUsage_t)(void* device, unsigned int* power_usage);
    typedef int (*nvmlDeviceGetClockInfo_t)(void* device, int clock_type, unsigned int* clock);
    typedef int (*nvmlSystemGetDriverVersion_t)(char* version, unsigned int length);
    
    // NVML函数指针
    nvmlInit_t nvmlInit;
    nvmlShutdown_t nvmlShutdown;
    nvmlDeviceGetCount_t nvmlDeviceGetCount;
    nvmlDeviceGetHandleByIndex_t nvmlDeviceGetHandleByIndex;
    nvmlDeviceGetName_t nvmlDeviceGetName;
    nvmlDeviceGetMemoryInfo_t nvmlDeviceGetMemoryInfo;
    nvmlDeviceGetUtilizationRates_t nvmlDeviceGetUtilizationRates;
    nvmlDeviceGetTemperature_t nvmlDeviceGetTemperature;
    nvmlDeviceGetPowerUsage_t nvmlDeviceGetPowerUsage;
    nvmlDeviceGetClockInfo_t nvmlDeviceGetClockInfo;
    nvmlSystemGetDriverVersion_t nvmlSystemGetDriverVersion;
    
    bool is_initialized;
    unsigned int device_count;
};

// AMD GPU监控实现
class AMD_GPUMonitor : public IGPUMonitor {
public:
    AMD_GPUMonitor();
    ~AMD_GPUMonitor();
    
    bool initialize() override;
    bool getGPUInfo(GPUInfo& gpu_info) override;
    bool getAllGPUInfo(std::vector<GPUInfo>& gpu_info_list) override;
    void cleanup() override;
    std::string getVendorName() const override { return "AMD"; }
    
private:
    bool is_initialized;
};

// Intel GPU监控实现
class Intel_GPUMonitor : public IGPUMonitor {
public:
    Intel_GPUMonitor();
    ~Intel_GPUMonitor();
    
    bool initialize() override;
    bool getGPUInfo(GPUInfo& gpu_info) override;
    bool getAllGPUInfo(std::vector<GPUInfo>& gpu_info_list) override;
    void cleanup() override;
    std::string getVendorName() const override { return "Intel"; }
    
private:
    bool is_initialized;
};

// GPU监控管理器
class GPUMonitorManager {
public:
    GPUMonitorManager();
    ~GPUMonitorManager();
    
    // 初始化GPU监控
    bool initialize();
    
    // 获取所有可用GPU监控器
    std::vector<IGPUMonitor*> getGPUMonitors();
    
    // 获取所有GPU信息
    bool getAllGPUInfo(std::vector<GPUInfo>& gpu_info_list);
    
    // 清理资源
    void cleanup();
    
private:
    std::vector<IGPUMonitor*> gpu_monitors;
    bool is_initialized;
};

#endif // GPU_MONITOR_H
