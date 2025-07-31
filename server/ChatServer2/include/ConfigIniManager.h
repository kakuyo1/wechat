#pragma once
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <string>
#include <map>
#include <iostream>
#include <filesystem>
#include <spdlog/spdlog.h>

/***************************************************************
 * Boost.PropertyTree is used to parse INI files.
 * SectionPairs class represents a section in the INI file,
 * containing key-value pairs.
 * boost::property_tree::ptree 是一个支持嵌套的字符串映射容器，
 * 适用于读写结构化配置文件（INI、JSON、XML 等），语法直观
 * struct ptree {
    std::string data;               // 节点自身的数据
    std::vector<ptree> children;    // 子节点
};
 *****************************************************************/

class SectionPairs {
public:
    SectionPairs() = default;
    SectionPairs(const SectionPairs& other);
    SectionPairs& operator=(const SectionPairs& other);
    ~SectionPairs();
    std::string& operator[](const std::string& key); // 用来写
    const std::string& operator[](const std::string &key) const; // 用来读
    const std::map<std::string, std::string>& GetPair() const;
private:
    std::map<std::string, std::string> pairs;
};

class ConfigIniManager {
public:
    ~ConfigIniManager();
    ConfigIniManager(const ConfigIniManager&) = delete;
    ConfigIniManager& operator=(const ConfigIniManager&) = delete;
    SectionPairs& operator[](const std::string& section_name);
    static ConfigIniManager& Instance();
private:
    ConfigIniManager();
    void PrintConfig() const;
    void LoadConfig(const std::string& file_path);
private:
    std::map<std::string, SectionPairs> _config; // Map of section names to SectionPairs
    std::string _config_file_path; // Path to the configuration file
    boost::property_tree::ptree _ptree; // Property tree for parsing INI files
};