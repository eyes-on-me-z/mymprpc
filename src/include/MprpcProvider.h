#pragma once

#include <mymuduo/EventLoop.h>
#include <google/protobuf/service.h>
#include <unordered_map>

// 框架提供的专门发布rpc服务的网络对象类
class RpcProvider
{
public:
    // 这里是框架提供给外部使用的，可以发布rpc方法的函数接口
    void NotifyService(google::protobuf::Service *service);

    // 启动rpc服务节点，开始提供rpc远程网络调用服务
    void Run();

private:
    // 新的socket连接回调
    void onConnection(const TcpConnectionPtr&);
    // 已建立连接用户的读写事件回调
    void onMessage(const TcpConnectionPtr&, Buffer*, Timestamp);
    // Closure的回调操作，用于序列化rpc的响应和网络发送
    void SendRpcResponse(const TcpConnectionPtr&, google::protobuf::Message*);

    // service服务类型信息
    struct ServiceInfo
    {
        google::protobuf::Service *_service;    // 保存服务对象
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor*> _methodMap;    // 保存服务方法
    };

    // 存储注册成功的服务对象和其服务方法的所有信息
    std::unordered_map<std::string, ServiceInfo> _serviceMap;
    EventLoop _loop;
};