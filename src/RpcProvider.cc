#include "RpcProvider.h"
#include "MprpcApplication.h"

#include <iostream>
#include <mymuduo/TcpServer.h>

// 这里是框架提供给外部使用的，可以发布rpc方法的函数接口
void RpcProvider::NotifyService(google::protobuf::Service *service)
{

}

// 启动rpc服务节点，开始提供rpc远程网络调用服务
void RpcProvider::Run()
{
    // 读取配置文件rpcserver的信息
    std::string ip = MprpcApplication::GetConfig().Load("rpcServerIp");
    uint16_t port = atoi(MprpcApplication::GetConfig().Load("rpcServerPort").c_str());

    InetAddress addr(port, ip);
    TcpServer server(&_loop, addr, "RpcProvider");

    // 绑定连接回调和消息读写回调方法  分离了网络代码和业务代码
    server.setConnectionCallback(
        std::bind(&RpcProvider::onConnection, this, std::placeholders::_1)
    );
    server.setMessageCallback(
        std::bind(&RpcProvider::onMessage, this, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3)
    );

    // 设置muduo库的线程数量
    server.setThreadNum(4);

    // rpc服务端准备启动，打印信息
    std::cout << "RpcProvider start service at ip: " << ip << " port: " << port << std::endl;

    // 启动网络服务
    server.start();
    _loop.loop();
}

void RpcProvider::onConnection(const TcpConnectionPtr&)
{

}
void RpcProvider::onMessage(const TcpConnectionPtr&, Buffer*, Timestamp)
{

}