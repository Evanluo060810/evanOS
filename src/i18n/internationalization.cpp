// Copyright 2026 Evanluo
// Licensed under the Apache License, Version 2.0

#include "i18n/internationalization.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>

namespace evan {

// 静态单例实例
static Internationalization* instance = nullptr;

/**
 * 获取单例实例
 * @return 国际化管理实例
 */
Internationalization& Internationalization::getInstance() {
    if (instance == nullptr) {
        instance = new Internationalization();
    }
    return *instance;
}

/**
 * 私有构造函数（单例模式）
 */
Internationalization::Internationalization() {
    // 默认语言为英语
    currentLanguage = Language::ENGLISH;
    
    // 初始化语言名称映射
    languageNames[Language::ENGLISH] = "English";
    languageNames[Language::CHINESE] = "Chinese";
    languageNames[Language::SPANISH] = "Spanish";
    languageNames[Language::FRENCH] = "French";
    languageNames[Language::GERMAN] = "German";
    languageNames[Language::JAPANESE] = "Japanese";
    
    // 初始化默认翻译
    initializeDefaultTranslations();
}

/**
 * 初始化默认翻译
 */
void Internationalization::initializeDefaultTranslations() {
    // 英语翻译
    registerTranslation(Language::ENGLISH, "system_performance", "System Performance");
    registerTranslation(Language::ENGLISH, "system_memory", "System Memory");
    registerTranslation(Language::ENGLISH, "total_memory", "Total Memory");
    registerTranslation(Language::ENGLISH, "each_process", "Each Process");
    registerTranslation(Language::ENGLISH, "hardware_info", "Hardware Info");
    registerTranslation(Language::ENGLISH, "gpu_info", "GPU Info");
    registerTranslation(Language::ENGLISH, "advanced_gpu_info", "Advanced GPU Info");
    registerTranslation(Language::ENGLISH, "help", "Help");
    registerTranslation(Language::ENGLISH, "copyright", "Copyright");
    registerTranslation(Language::ENGLISH, "license", "License");
    registerTranslation(Language::ENGLISH, "cpu_architecture", "CPU Architecture");
    registerTranslation(Language::ENGLISH, "number_of_processors", "Number of Processors");
    registerTranslation(Language::ENGLISH, "processor_level", "Processor Level");
    registerTranslation(Language::ENGLISH, "processor_revision", "Processor Revision");
    registerTranslation(Language::ENGLISH, "cpu_brand", "CPU Brand");
    registerTranslation(Language::ENGLISH, "page_size", "Page Size");
    registerTranslation(Language::ENGLISH, "active_processor_mask", "Active Processor Mask");
    registerTranslation(Language::ENGLISH, "total_physical_memory", "Total Physical Memory");
    registerTranslation(Language::ENGLISH, "used_physical_memory", "Used Physical Memory");
    registerTranslation(Language::ENGLISH, "free_physical_memory", "Free Physical Memory");
    registerTranslation(Language::ENGLISH, "memory_usage", "Memory Usage");
    registerTranslation(Language::ENGLISH, "gpu_name", "GPU Name");
    registerTranslation(Language::ENGLISH, "gpu_vendor", "GPU Vendor");
    registerTranslation(Language::ENGLISH, "driver_version", "Driver Version");
    registerTranslation(Language::ENGLISH, "memory_total", "Total Memory");
    registerTranslation(Language::ENGLISH, "memory_used", "Used Memory");
    registerTranslation(Language::ENGLISH, "memory_free", "Free Memory");
    registerTranslation(Language::ENGLISH, "utilization", "Utilization");
    registerTranslation(Language::ENGLISH, "temperature", "Temperature");
    registerTranslation(Language::ENGLISH, "power_usage", "Power Usage");
    registerTranslation(Language::ENGLISH, "clock_core", "Core Clock");
    registerTranslation(Language::ENGLISH, "clock_memory", "Memory Clock");
    
    // 中文翻译
    registerTranslation(Language::CHINESE, "system_performance", "系统性能");
    registerTranslation(Language::CHINESE, "system_memory", "系统内存");
    registerTranslation(Language::CHINESE, "total_memory", "总内存");
    registerTranslation(Language::CHINESE, "each_process", "每个进程");
    registerTranslation(Language::CHINESE, "hardware_info", "硬件信息");
    registerTranslation(Language::CHINESE, "gpu_info", "GPU信息");
    registerTranslation(Language::CHINESE, "advanced_gpu_info", "高级GPU信息");
    registerTranslation(Language::CHINESE, "help", "帮助");
    registerTranslation(Language::CHINESE, "copyright", "版权");
    registerTranslation(Language::CHINESE, "license", "许可证");
    registerTranslation(Language::CHINESE, "cpu_architecture", "CPU架构");
    registerTranslation(Language::CHINESE, "number_of_processors", "处理器数量");
    registerTranslation(Language::CHINESE, "processor_level", "处理器级别");
    registerTranslation(Language::CHINESE, "processor_revision", "处理器修订");
    registerTranslation(Language::CHINESE, "cpu_brand", "CPU品牌");
    registerTranslation(Language::CHINESE, "page_size", "页大小");
    registerTranslation(Language::CHINESE, "active_processor_mask", "活动处理器掩码");
    registerTranslation(Language::CHINESE, "total_physical_memory", "总物理内存");
    registerTranslation(Language::CHINESE, "used_physical_memory", "已用物理内存");
    registerTranslation(Language::CHINESE, "free_physical_memory", "可用物理内存");
    registerTranslation(Language::CHINESE, "memory_usage", "内存使用率");
    registerTranslation(Language::CHINESE, "gpu_name", "GPU名称");
    registerTranslation(Language::CHINESE, "gpu_vendor", "GPU厂商");
    registerTranslation(Language::CHINESE, "driver_version", "驱动版本");
    registerTranslation(Language::CHINESE, "memory_total", "总显存");
    registerTranslation(Language::CHINESE, "memory_used", "已用显存");
    registerTranslation(Language::CHINESE, "memory_free", "可用显存");
    registerTranslation(Language::CHINESE, "utilization", "使用率");
    registerTranslation(Language::CHINESE, "temperature", "温度");
    registerTranslation(Language::CHINESE, "power_usage", "功耗");
    registerTranslation(Language::CHINESE, "clock_core", "核心频率");
    registerTranslation(Language::CHINESE, "clock_memory", "显存频率");
}

/**
 * 设置当前语言
 * @param lang 语言类型
 */
void Internationalization::setLanguage(Language lang) {
        currentLanguage = lang;
    }

/**
 * 获取当前语言
 * @return 当前语言类型
 */
Language Internationalization::getCurrentLanguage() const {
        return currentLanguage;
    }

/**
 * 获取当前语言名称
 * @return 当前语言名称字符串
 */
std::string Internationalization::getCurrentLanguageName() const {
        auto it = languageNames.find(currentLanguage);
        if (it != languageNames.end()) {
            return it->second;
        }
        return "Unknown";
    }

/**
 * 翻译字符串
 * @param key 翻译键
 * @return 翻译后的字符串
 */
std::string Internationalization::translate(const std::string& key) {
        // 检查当前语言是否有对应的翻译
        auto langIt = translations.find(currentLanguage);
        if (langIt != translations.end()) {
            auto keyIt = langIt->second.find(key);
            if (keyIt != langIt->second.end()) {
                return keyIt->second;
            }
        }
        
        // 如果当前语言没有翻译，尝试使用英语作为回退
        auto englishIt = translations.find(Language::ENGLISH);
        if (englishIt != translations.end()) {
            auto keyIt = englishIt->second.find(key);
            if (keyIt != englishIt->second.end()) {
                return keyIt->second;
            }
        }
        
        // 如果都没有翻译，返回原始键
        return key;
    }

/**
 * 注册翻译
 * @param lang 语言类型
 * @param key 翻译键
 * @param value 翻译值
 */
void Internationalization::registerTranslation(Language lang, const std::string& key, const std::string& value) {
        translations[lang][key] = value;
    }

/**
 * 加载翻译文件
 * @param lang 语言类型
 * @param filePath 翻译文件路径
 * @return 是否加载成功
 */
bool Internationalization::loadTranslationFile(Language lang, const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        // 跳过空行和注释行
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        // 查找等号分隔符
        size_t equalsPos = line.find('=');
        if (equalsPos == std::string::npos) {
            continue;
        }
        
        // 提取键和值
        std::string key = line.substr(0, equalsPos);
        std::string value = line.substr(equalsPos + 1);
        
        // 去除前后空格
        key.erase(0, key.find_first_not_of(" 	"));
        key.erase(key.find_last_not_of(" 	") + 1);
        value.erase(0, value.find_first_not_of(" 	"));
        value.erase(value.find_last_not_of(" 	") + 1);
        
        // 注册翻译
        registerTranslation(lang, key, value);
    }
    
    file.close();
    return true;
}

/**
 * 获取支持的语言列表
 * @return 支持的语言名称列表
 */
std::vector<std::string> Internationalization::getSupportedLanguages() const {
        std::vector<std::string> languages;
        for (const auto& pair : languageNames) {
            languages.push_back(pair.second);
        }
        
        return languages;
    }

/**
 * 检查是否支持某种语言
 * @param lang 语言类型
 * @return 是否支持
 */
bool Internationalization::isLanguageSupported(Language lang) const {
        return languageNames.find(lang) != languageNames.end();
    }

} // namespace evan
