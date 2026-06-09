#include "MprpcApplication.h"
#include "user.pb.h"
#include "MprpcChannel.h"

int main(int argc, char **argv)
{
    // 整个程序启动以后，想使用mprpc框架来享受rpc服务调用，一定需要先调用框架的初始化函数（只初始化一次）
    MprpcApplication::Init(argc, argv);
    
    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());
    fixbug::LoginRequest request;
    request.set_name("zg");
    request.set_pwd("123456");
    fixbug::LoginResponse response;
    stub.Login(nullptr, &request, &response, nullptr);  // channel->CallMethod()

    if (response.result().errcode() == 0)
    {
        std::cout << "rpc login response success: " << response.success() << std::endl;
    }
    else
    {
        std::cout << "rpc login response error: " << response.result().errmsg() << std::endl;
    }

    return 0;
}