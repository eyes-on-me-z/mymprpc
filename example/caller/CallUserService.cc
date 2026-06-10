#include "MprpcApplication.h"
#include "user.pb.h"
#include "MprpcChannel.h"

int main(int argc, char **argv)
{
    // 整个程序启动以后，想使用mprpc框架来享受rpc服务调用，一定需要先调用框架的初始化函数（只初始化一次）
    MprpcApplication::Init(argc, argv);
    
    // 演示调用远程发布的rpc方法Login
    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());
    // rpc方法的请求参数
    fixbug::LoginRequest request;
    request.set_name("zg");
    request.set_pwd("123456");
    // rpc方法的响应
    fixbug::LoginResponse response;
    // 发起rpc方法的调用  同步的rpc调用过程  MprpcChannel::callmethod
    // RpcChannel->RpcChannel::callMethod 集中来做所有rpc方法调用的参数序列化和网络发送
    stub.Login(nullptr, &request, &response, nullptr);

    // 一次rpc调用完成，读调用的结果
    if (response.result().errcode() == 0)
    {
        std::cout << "rpc login response success: " << response.success() << std::endl;
    }
    else
    {
        std::cout << "rpc login response error: " << response.result().errmsg() << std::endl;
    }

    // 演示调用远程发布的rpc方法Login
    // rpc方法的请求参数
    fixbug::RegisterRequest regisReq;
    regisReq.set_id(2000);
    regisReq.set_name("cm");
    regisReq.set_pwd("123456");
    // rpc方法的响应
    fixbug::RegisterResponse regisResp;
    // 以同步的方式发起rpc调用请求，等待返回结果
    stub.Register(nullptr, &regisReq, &regisResp, nullptr);

    // 一次rpc调用完成，读调用的结果
    if (regisResp.result().errcode() == 0)
    {
        std::cout << "rpc login response success: " << regisResp.success() << std::endl;
    }
    else
    {
        std::cout << "rpc login response error: " << regisResp.result().errmsg() << std::endl;
    }

    return 0;
}