#include <iostream>
#include <string>
#include "test.pb.h"


// 编译command：g++ main.cc test.pb.cc     $(pkg-config --cflags --libs protobuf)
int main()
{
    fixbug::LoginRequest req;
    req.set_name("zhang san");
    req.set_pwd("123456");
    std::string send_str;
    if (req.SerializeToString(&send_str))
    {
        std::cout << send_str << std::endl;
    }

    fixbug::LoginRequest reqB;
    if (reqB.ParseFromString(send_str))
    {
        std::cout << reqB.name() << std::endl;
        std::cout << reqB.pwd() << std::endl;
    }

    return 0;
}