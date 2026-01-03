// Copyright 2026 Evanluo
// Licensed under the Apache License, Version 2.0

#include "utils/command_line.h"
#include <algorithm>
#include <sstream>
#include <cctype>

namespace cmdline {
    /**
     * parameter基类构造函数
     * @param name 参数名称（长选项名）
     * @param short_name 短选项名（如 'h' 对应 '--help'）
     * @param description 参数描述
     * @param required 是否为必填参数
     * 
     * 功能：初始化parameter基类对象，设置参数的基本属性
     */
    parser::parameter::parameter(const std::string& name, char short_name, const std::string& description,
                              bool required)
        : name(name), short_name(short_name), description(description),
          required(required), has_value(false) {
    }
    
    /**
     * parameter基类析构函数
     * 
     * 功能：释放parameter基类对象的资源
     */
    parser::parameter::~parameter() {
    }
    
    /**
     * parser构造函数
     * 
     * 功能：初始化parser对象，设置默认程序名称
     */
    parser::parser() : program_name("program") {
    }
    
    /**
     * parser析构函数
     * 
     * 功能：释放parser对象的资源，包括所有添加的参数对象
     */
    parser::~parser() {
        // 遍历所有参数，释放内存
        for (auto& pair : parameters) {
            delete pair.second;
        }
    }
    
    /**
     * 设置程序名称
     * @param name 程序名称
     * 
     * 功能：设置命令行解析器中使用的程序名称
     */
    void parser::set_program_name(const std::string& name) {
        program_name = name;
    }
    
    /**
     * 添加布尔参数
     * @param name 参数名称（长选项名）
     * @param short_name 短选项名
     * @param description 参数描述
     * 
     * 功能：添加一个布尔类型的命令行参数，默认值为false
     */
    void parser::add(const std::string& name, char short_name, const std::string& description) {
        add<bool>(name, short_name, description, false, false);
    }
    
    /**
     * 解析命令行参数
     * @param argc 命令行参数个数
     * @param argv 命令行参数数组
     * @return 解析成功返回true，失败返回false
     * 
     * 功能：解析命令行参数，将参数值存储到对应的参数对象中
     */
    bool parser::parse(int argc, char** argv) {
        error_message.clear();
        
        // 记录已处理的参数，用于后续检查必填参数
        std::vector<std::string> processed_params;
        
        // 遍历所有命令行参数
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            
            if (arg.empty()) {
                continue;
            }
            
            // 处理长选项（如 --help, --verbose）
            if (arg.size() > 2 && arg.substr(0, 2) == "--") {
                std::string param_name = arg.substr(2);
                
                // 查找参数是否存在
                auto it = parameters.find(param_name);
                if (it == parameters.end()) {
                    error_message = "Unknown parameter: --" + param_name;
                    return false;
                }
                
                // 标记参数为已处理
                processed_params.push_back(param_name);
                
                // 检查是否为布尔参数
                if (dynamic_cast<typed_parameter<bool>*>(it->second)) {
                    // 布尔参数，直接设置为true
                    it->second->set_value("true");
                } else {
                    // 非布尔参数，需要获取值
                    if (i + 1 >= argc) {
                        error_message = "Parameter --" + param_name + " requires a value";
                        return false;
                    }
                    
                    // 设置参数值，i自增以跳过已处理的值
                    it->second->set_value(argv[++i]);
                }
            } 
            // 处理短选项（如 -h, -v）
            else if (arg.size() > 1 && arg[0] == '-') {
                // 遍历短选项字符串中的每个字符
                for (size_t j = 1; j < arg.size(); ++j) {
                    char short_name = arg[j];
                    
                    // 查找短选项对应的长选项名
                    auto it = short_to_long.find(short_name);
                    if (it == short_to_long.end()) {
                        error_message = "Unknown parameter: -" + std::string(1, short_name);
                        return false;
                    }
                    
                    std::string param_name = it->second;
                    auto param_it = parameters.find(param_name);
                    
                    // 标记参数为已处理
                    processed_params.push_back(param_name);
                    
                    // 检查是否为布尔参数
                    if (dynamic_cast<typed_parameter<bool>*>(param_it->second)) {
                        // 布尔参数，直接设置为true
                        param_it->second->set_value("true");
                    } else {
                        // 非布尔参数，需要获取值
                        if (j + 1 < arg.size()) {
                            // 值紧跟在短选项后面（如 -nvalue）
                            std::string value = arg.substr(j + 1);
                            param_it->second->set_value(value);
                            break; // 结束当前短选项处理
                        } else {
                            // 值在下一个参数中
                            if (i + 1 >= argc) {
                                error_message = "Parameter -" + std::string(1, short_name) + " requires a value";
                                return false;
                            }
                            
                            // 设置参数值，i自增以跳过已处理的值
                            param_it->second->set_value(argv[++i]);
                            break; // 结束当前短选项处理
                        }
                    }
                }
            }
        }
        
        // 检查所有必填参数是否都已处理
        for (const auto& pair : parameters) {
            if (pair.second->required) {
                auto it = std::find(processed_params.begin(), processed_params.end(), pair.first);
                if (it == processed_params.end()) {
                    error_message = "Required parameter missing: --" + pair.first;
                    return false;
                }
            }
        }
        
        return true;
    }
    
    /**
     * 检查参数是否存在
     * @param name 参数名称
     * @return 参数存在且有值返回true，否则返回false
     * 
     * 功能：检查指定名称的参数是否存在且已设置值
     */
    bool parser::exist(const std::string& name) const {
        auto it = parameters.find(name);
        if (it == parameters.end()) {
            return false;
        }
        
        return it->second->has_value;
    }
    
    /**
     * 获取使用说明
     * @return 使用说明字符串
     * 
     * 功能：生成并返回命令行参数的使用说明
     */
    std::string parser::usage() const {
        std::ostringstream oss;
        
        // 输出基本使用格式
        oss << "Usage: " << program_name << " [options]\n\nOptions:\n";
        
        // 查找最长的参数名称，用于对齐显示
        size_t max_name_len = 0;
        for (const auto& pair : parameters) {
            max_name_len = std::max(max_name_len, pair.first.size());
        }
        
        // 生成选项列表
        for (const auto& pair : parameters) {
            const parameter* param = pair.second;
            
            // 输出短选项和长选项
            oss << "  -" << param->short_name << ", --" << param->name;
            
            // 计算并添加对齐空格
            size_t padding = max_name_len - param->name.size();
            oss << std::string(padding, ' ');
            
            // 输出参数描述
            oss << "  " << param->description;
            
            oss << "\n";
        }
        
        return oss.str();
    }
    
    /**
     * 获取错误信息
     * @return 错误信息字符串
     * 
     * 功能：返回解析过程中产生的错误信息
     */
    std::string parser::error() const {
        return error_message.empty() ? "" : error_message + "\n";
    }
}
