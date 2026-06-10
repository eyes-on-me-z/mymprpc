#include "MprpcApplication.h"
#include "MprpcProvider.h"
#include "friend.pb.h"

#include <vector>
#include <string>

class FriendService : public fixbug::FriendServiceRpc
{
public:
    std::vector<std::string> GetFriendLists(uint32_t userid)
    {
        std::cout << "do GetFriendLists success! userid: " << userid << std::endl;
        std::vector<std::string> vec;
        vec.push_back("zeng guang");
        vec.push_back("chen mian");

        return vec;
    }

    // 重写基类方法
    void GetFriendLists(::google::protobuf::RpcController* controller,
                        const ::fixbug::GetFriendListsRequest* request,
                        ::fixbug::GetFriendListsResponse* response,
                        ::google::protobuf::Closure* done)
    {
        uint32_t id = request->userid();
        std::vector<std::string> friendList = GetFriendLists(id);
        for (const auto &fri : friendList)
        {
            std::string *p = response->add_friends();
            *p = fri;
        }
        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");
        done->Run();
    }
};

int main(int argc, char **argv)
{
    // 调用框架的初始化操作
    MprpcApplication::Init(argc, argv);

    // provider是一个rpc网络服务对象。把UserService对象发布到rpc节点上
    RpcProvider provider;
    provider.NotifyService(new FriendService());

    // 启动一个rpc服务发布节点   Run以后，进程进入阻塞状态，等待远程的rpc调用请求
    provider.Run();

    return 0;
}