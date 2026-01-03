// Copyright 2026 Evanluo
// Licensed under the Apache License, Version 2.0

#pragma once

#include <string>
#include <map>
#include <vector>

namespace evan {

// 支持的语言类型
enum class Language {
    ENGLISH,
    CHINESE,
    SPANISH,
    FRENCH,
    GERMAN,
    JAPANESE
};

// 国际化管理类
class Internationalization {
public:
    /**
     * 获取单例实例
     * @return 国际化管理实例
     */
    static Internationalization& getInstance();
    
    /**
     * 设置当前语言
     * @param lang 语言类型
     */
    void setLanguage(Language lang);
    
    /**
     * 获取当前语言
     * @return 当前语言类型
     */
    Language getCurrentLanguage() const;
    
    /**
     * 获取当前语言名称
     * @return 当前语言名称字符串
     */
    std::string getCurrentLanguageName() const;
    
    /**
     * 翻译字符串
     * @param key 翻译键
     * @return 翻译后的字符串
     */
    std::string translate(const std::string& key);
    
    /**
     * 注册翻译
     * @param lang 语言类型
     * @param key 翻译键
     * @param value 翻译值
     */
    void registerTranslation(Language lang, const std::string& key, const std::string& value);
    
    /**
     * 加载翻译文件
     * @param lang 语言类型
     * @param filePath 翻译文件路径
     * @return 是否加载成功
     */
    bool loadTranslationFile(Language lang, const std::string& filePath);
    
    /**
     * 获取支持的语言列表
     * @return 支持的语言名称列表
     */
    std::vector<std::string> getSupportedLanguages() const;
    
    /**
     * 检查是否支持某种语言
     * @param lang 语言类型
     * @return 是否支持
     */
    bool isLanguageSupported(Language lang) const;
    
private:
    // 私有构造函数（单例模式）
    Internationalization();
    
    // 禁止复制和赋值
    Internationalization(const Internationalization&) = delete;
    Internationalization& operator=(const Internationalization&) = delete;
    
    // 初始化默认翻译
    void initializeDefaultTranslations();
    
    // 当前语言
    Language currentLanguage;
    
    // 翻译映射
    std::map<Language, std::map<std::string, std::string>> translations;
    
    // 语言名称映射
    std::map<Language, std::string> languageNames;
    

};

// 翻译宏（方便使用）
#define _T(key) evan::Internationalization::getInstance().translate(key)

} // namespace evan
