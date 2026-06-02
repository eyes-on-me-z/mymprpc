#include <iostream>
#include <string>
#include "test.pb.h"

// 编译command：g++ main.cc test.pb.cc     $(pkg-config --cflags --libs protobuf)
int main()
{
    // fixbug::LoginResponse rsp;
    // fixbug::ResultCode *rc = rsp.mutable_result();
    // rc->set_errcode(1);
    // rc->set_errmsg("登录失败");

    fixbug::GetFriendListsResponse rsp;
    fixbug::ResultCode *rc = rsp.mutable_result();
    rc->set_errcode(0);

    fixbug::User *user1 = rsp.add_friend_list();
    user1->set_name("zg");
    user1->set_age(20);
    user1->set_sex(fixbug::User::MALE);

    fixbug::User *user2 = rsp.add_friend_list();
    user2->set_name("cm");
    user2->set_age(20);
    user2->set_sex(fixbug::User::FAMALE);

    std::cout << rsp.friend_list_size() << std::endl;

    return 0;
}


int main1()
{
    // 封装了login请求对象的数据
    fixbug::LoginRequest req;
    req.set_name("zg");
    req.set_pwd("123456");

    // 对象数据序列化 =》 char*
    std::string send_str;
    if (req.SerializeToString(&send_str))
    {
        std::cout << send_str << std::endl;
    }

    // 从send_str反序列化一个login请求对象
    fixbug::LoginRequest reqB;
    if (reqB.ParseFromString(send_str))
    {
        std::cout << reqB.name() << std::endl;
        std::cout << reqB.pwd() << std::endl;
    }

    return 0;
}