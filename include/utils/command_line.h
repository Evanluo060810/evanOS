// Copyright 2026 Evanluo
// Licensed under the Apache License, Version 2.0

#pragma once

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <sstream>

namespace cmdline {
    // 命令行参数解析器类
    class parser {
    public:
        parser();
        ~parser();
        
        // 设置程序名称
        void set_program_name(const std::string& name);
        
        // 添加布尔参数
        void add(const std::string& name, char short_name, const std::string& description);
        
        // 添加带默认值的参数
        template <typename T>
        void add(const std::string& name, char short_name, const std::string& description, 
                 bool required = false, const T& default_value = T());
        
        // 解析命令行参数
        bool parse(int argc, char** argv);
        
        // 检查参数是否存在
        bool exist(const std::string& name) const;
        
        // 获取参数值
        template <typename T>
        T get(const std::string& name) const;
        
        // 获取使用说明
        std::string usage() const;
        
        // 获取错误信息
        std::string error() const;
        
    private:
        // 参数基类
        class parameter {
        public:
            parameter(const std::string& name, char short_name, const std::string& description,
                      bool required = false);
            virtual ~parameter();
            
            std::string name;
            char short_name;
            std::string description;
            bool required;
            bool has_value;
            
            virtual void set_value(const std::string& value) = 0;
            virtual std::string get_value_as_string() const = 0;
        };
        
        // 具体参数类型
        template <typename T>
        class typed_parameter : public parameter {
        public:
            typed_parameter(const std::string& name, char short_name, const std::string& description,
                           bool required = false, const T& default_value = T());
            
            void set_value(const std::string& value) override;
            std::string get_value_as_string() const override;
            
            T value;
            T default_value;
        };
        
        std::string program_name;
        std::map<std::string, parameter*> parameters;
        std::map<char, std::string> short_to_long;
        std::string error_message;
        
        // 辅助函数：将字符串转换为指定类型
        template <typename T>
        static T convert(const std::string& str);
    };
    
    // 范围验证器
    template <typename T>
    class range {
    public:
        range(T min, T max);
        bool operator()(const std::string& str) const;
        
    private:
        T min;
        T max;
    };
    
    // 实现模板方法
    template <typename T>
    parser::typed_parameter<T>::typed_parameter(const std::string& name, char short_name, 
                                               const std::string& description, bool required,
                                               const T& default_value)
        : parameter(name, short_name, description, required), default_value(default_value), value(default_value) {
    }
    
    template <typename T>
    void parser::typed_parameter<T>::set_value(const std::string& str_value) {
        value = convert<T>(str_value);
        has_value = true;
    }
    
    template <typename T>
    std::string parser::typed_parameter<T>::get_value_as_string() const {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }
    
    template <typename T>
    T parser::get(const std::string& name) const {
        auto it = parameters.find(name);
        if (it == parameters.end()) {
            throw std::runtime_error("Parameter not found: " + name);
        }
        
        typed_parameter<T>* typed_param = dynamic_cast<typed_parameter<T>*>(it->second);
        if (!typed_param) {
            throw std::runtime_error("Parameter type mismatch: " + name);
        }
        
        return typed_param->value;
    }
    
    template <typename T>
    void parser::add(const std::string& name, char short_name, const std::string& description,
                    bool required, const T& default_value) {
        parameters[name] = new typed_parameter<T>(name, short_name, description, required, default_value);
        if (short_name != 0) {
            short_to_long[short_name] = name;
        }
    }
    
    // 基本类型转换
    template <>
    inline int parser::convert<int>(const std::string& str) {
        return std::stoi(str);
    }
    
    template <>
    inline unsigned int parser::convert<unsigned int>(const std::string& str) {
        return std::stoul(str);
    }
    
    template <>
    inline long parser::convert<long>(const std::string& str) {
        return std::stol(str);
    }
    
    template <>
    inline unsigned long parser::convert<unsigned long>(const std::string& str) {
        return std::stoul(str);
    }
    
    template <>
    inline std::string parser::convert<std::string>(const std::string& str) {
        return str;
    }
    
    template <>
    inline bool parser::convert<bool>(const std::string& str) {
        if (str == "true" || str == "1" || str == "yes" || str == "y") {
            return true;
        }
        return false;
    }
    
    // 范围验证器实现
    template <typename T>
    range<T>::range(T min, T max) : min(min), max(max) {
    }
    
    template <typename T>
    bool range<T>::operator()(const std::string& str) const {
        try {
            T value = parser::convert<T>(str);
            return value >= min && value <= max;
        } catch (...) {
            return false;
        }
    }
}
