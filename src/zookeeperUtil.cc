#include "zookeeperUtil.h"
#include "MprpcApplication.h"

#include <semaphore.h>
#include <iostream>


// 全局的watcher观察器   客户端连接服务器后，zkserver给zkclient的通知
void globalWatcher(zhandle_t *zh, int type,
        int state, const char *path, void *watcherCtx)
{
    if (type == ZOO_SESSION_EVENT)  // 回调的消息类型是和会话相关的消息类型
    {
        if (state == ZOO_CONNECTED_STATE)   // zkclient和zkserver连接成功
        {
            sem_t *sem = (sem_t*)zoo_get_context(zh);
            sem_post(sem);
        }
    }
}

ZkClient::ZkClient() : _zhandle(nullptr)
{}

ZkClient::~ZkClient()
{
    if (_zhandle != nullptr)
    {
        zookeeper_close(_zhandle);  // 关闭句柄，释放资源  MySQL_Conn
    }
}

// zkclient启动连接zkserver
void ZkClient::start()
{
    std::string host = MprpcApplication::GetConfig().Load("zookeeperIp");
    std::string port = MprpcApplication::GetConfig().Load("zookeeperPort");
    std::string connStr = host + ":" + port;

    /*
	zookeeper_mt：多线程版本
	zookeeper的API客户端程序提供了三个线程
	API调用线程 
	网络I/O线程  pthread_create  poll
	watcher回调线程 pthread_create
	*/
    // ZooKeeper服务器地址(127.0.0.1:8000)   Watcher回调函数   会话超时时间(ms)
    // 已有会话ID（重连时使用）  会话密码（重连时使用）   标志位
    // ZooKeeper 的连接是异步建立的。调用 zookeeper_init() 后并不意味着已经连接成功，而是：
    // 1.创建客户端对象；2.启动内部网络线程；3.开始连接 ZooKeeper；4.连接成功后触发：
    _zhandle = zookeeper_init(connStr.c_str(), globalWatcher, 30000, nullptr, nullptr, 0);
    if (_zhandle == nullptr)    // 客户端创建失败
    {
        std::cout << "zookeeper_init error!" << std::endl;
        exit(EXIT_FAILURE);
    }

    sem_t sem;
    sem_init(&sem, 0, 0);
    zoo_set_context(_zhandle, &sem);    // 把信号量地址保存到 ZooKeeper 的上下文中，以后在 Watcher 中可以取出来：

    sem_wait(&sem); // 当 ZooKeeper 连接成功时触发 Watcher。将信号量从 0 变成 1。
    std::cout << "zookeeper_init success!" << std::endl;
}

// 在zkserver上根据指定的path创建znode节点
void ZkClient::create(const char *path, const char *data, int dataLen, int state)
{
    char pathBuffer[128];
    int bufferLen = sizeof pathBuffer;
    int flag;

    // 先判断path表示的znode节点是否存在，如果存在，就不再重复创建了(编译时定义 THREADED)
    // #ifdef THREADED
    // ZOOAPI int zoo_exists(...)
    // #endif
    flag = zoo_exists(_zhandle, path, 0, nullptr);
    if (flag == ZNONODE)    // 表示path的znode节点不存在
    {
        // 创建指定path的znode节点了
        // &ZOO_OPEN_ACL_UNSAFE表示任何客户端都能读写   持久节点(state=0)
        // pathBuffer: ZooKeeper返回最终创建的路径。
        flag = zoo_create(_zhandle, path, data, dataLen,
            &ZOO_OPEN_ACL_UNSAFE, state, pathBuffer, bufferLen);
        if (flag == ZOK)
        {
            std::cout << "znode create success... path: " << path << std::endl;
        }
        else
        {
            std::cout << "flag: " << flag << std::endl;
            std::cout << "znode create error... path: " << path << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

// 根据参数指定的znode节点路径，或者znode节点的值
std::string ZkClient::getData(const char *path)
{
    char buf[64] = {0};
    int bufferLen = sizeof buf;
    // 0表示不设置watch，buf：接收节点数据的缓冲区，bufflen：输入时表示缓冲区大小，输出时表示实际数据长度
    // nullptr：不需要获取Stat信息
    int flag = zoo_get(_zhandle, path, 0, buf, &bufferLen, nullptr);
    if (flag != ZOK)
    {
        std::cout << "get znode error... path: " << path << std::endl;
        return "";
    }
    else
    {
        return buf;
    }
}