#include "ConfigIniManager.h"
#include <spdlog/spdlog.h>
SectionPairs::SectionPairs(const SectionPairs &other)
{
    pairs = other.pairs;
}

SectionPairs &SectionPairs::operator=(const SectionPairs &other)
{
    if (this == &other) {
        return *this; // Handle self-assignment
    }
    pairs.clear(); // Clear existing pairs
    pairs = other.pairs;
    return *this;
}

SectionPairs::~SectionPairs()
{
    pairs.clear();
}

std::string& SectionPairs::operator[](const std::string &key) //插入版本：用于加载配置（允许插入）
{
    return pairs[key]; // 使用 std::map 的原始行为

    /*不允许插入新 key，而是抛异常(先检测是否存在，都没插入，肯定不存在！！！！！故下面的代码应该写成一个只读函数)：
    auto it = pairs.find(key);
    if (it == pairs.end()) {
        throw std::runtime_error("Configuration key not found: " + key);
    }
    return it->second;
    */
}

const std::map<std::string, std::string> &SectionPairs::GetPair() const
{
    return pairs;
}

ConfigIniManager::~ConfigIniManager()
{
    _config.clear();
    _ptree.clear();
}

SectionPairs& ConfigIniManager::operator[](const std::string &section_name)
{
    auto it = _config.find(section_name);
    if (it == _config.end()) {
        throw std::runtime_error("Configuration section not found: " + section_name);
    }
    return it->second;
}

ConfigIniManager &ConfigIniManager::Instance()
{
    static ConfigIniManager instance; // Singleton instance
    return instance;
}

ConfigIniManager::ConfigIniManager()
{
    // Load the default configuration file if it exists
    auto current_path = std::filesystem::current_path();
    std::string default_config_path = (current_path / "config.ini").string(); // Adjust the path as needed
    if (std::filesystem::exists(default_config_path)) {
        LoadConfig(default_config_path);
    } else {
        std::cerr << "Default config file not found: " << default_config_path << std::endl;
        throw std::runtime_error("Config file not found");
    }
    // debug: print the loaded config
    // PrintConfig();
}

void ConfigIniManager::LoadConfig(const std::string &file_path)
{
    _config_file_path = file_path;
    try {
        boost::property_tree::ini_parser::read_ini(file_path, _ptree);
        for (const auto &section : _ptree) {
            SectionPairs section_pairs;
            for (const auto &pair : section.second) {// pair.first is the section name, pair.second is a ptree representing key-value pairs
                section_pairs[pair.first] = pair.second.get_value<std::string>(); // as std::string(8080)
            } //section_pairs[pair.first]调用了std::string& SectionPairs::operator[](const std::string &key)来插入
            _config[section.first] = section_pairs;
        }
    } catch (const std::exception &e) {
        spdlog::error("Failed to load config file {}: {}", file_path, e.what());
    }
}

void ConfigIniManager::PrintConfig() const
{
    for (const auto &section : _config) {
        std::cout << "[" << section.first << "]" << std::endl;
        for (const auto &pair : section.second.GetPair()) {
            std::cout << pair.first << " = " << pair.second << std::endl;
        }
    }
}

const std::string& SectionPairs::operator[](const std::string &key) const // 只读版本：用于获取配置（不允许插入新 key）
{
    auto it = pairs.find(key);
    if (it == pairs.end()) {
        throw std::runtime_error("Key not found: " + key);
    }
    return it->second;
}