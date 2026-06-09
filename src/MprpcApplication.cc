#include "MprpcApplication.h"

#include <iostream>
#include <unistd.h>

MprpcConfig MprpcApplication::_config;

void showArgsHelp()
{
    std::cout << "format: command -i <configfile>" << std::endl;
}

void MprpcApplication::Init(int argc, char **argv)
{
    if (argc < 2)
    {
        showArgsHelp();
        exit(EXIT_FAILURE);
    }

    int c = 0;
    std::string configFile;
    while((c = getopt(argc, argv, ":i:")) != -1)
    {
        switch (c)
        {
        case 'i':
            configFile = optarg;
            break;
        case '?':                   // 非法选项 
            showArgsHelp();
            exit(EXIT_FAILURE);
        case ':':                   // 缺少参数（特殊情况）
            showArgsHelp();
            exit(EXIT_FAILURE);
        default:
            showArgsHelp();
            exit(EXIT_FAILURE);
        }
    }

    // 开始加载配置文件了 rpcserver_ip=  rpcserver_port   zookeeper_ip=  zookepper_port=
    _config.LoadConfigFile(configFile.c_str());

    // std::cout << "rpcserverip:" << _config.Load("rpcServerIp") << std::endl;
    // std::cout << "rpcserverport:" << _config.Load("rpcServerPort") << std::endl;
    // std::cout << "zookeeperip:" << _config.Load("zookeeperIp") << std::endl;
    // std::cout << "zookeeperport:" << _config.Load("zookeeperPort") << std::endl;
}

MprpcApplication& MprpcApplication::GetInstance()
{
    static MprpcApplication app;
    return app;
}

MprpcConfig& MprpcApplication::GetConfig()
{
    return _config;
}