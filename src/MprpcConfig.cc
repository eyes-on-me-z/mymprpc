#include "MprpcConfig.h"
#include "json.hpp"

#include <fstream>
#include <iostream>

using json = nlohmann::json;

// 框架读取配置文件类
void MprpcConfig::LoadConfigFile(const char *configFile)
{
    std::ifstream ifs(configFile);  // 离开作用域时，ifs自动析构并关闭文件
    if (!ifs.is_open())
    {
        std::cerr << "open config file failed: " << configFile << std::endl;
        exit(EXIT_FAILURE);
    }

    json j;
    ifs >> j;
    for (auto it = j.begin(); it != j.end(); ++it)
    {
        if (it.value().is_string())
        {
            _configMap[it.key()] = it.value();
        }
        else
        {
            _configMap[it.key()] = it.value().dump();
        }
    }
}

// 获取配置项
std::string MprpcConfig::Load(const std::string &key)
{
    auto it = _configMap.find(key);
    if (it == _configMap.end())
    {
        return "";
    }

    return it->second;
}