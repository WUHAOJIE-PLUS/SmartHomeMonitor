#ifndef __SMARTHOMEMONITORSERVER_H
#define __SMARTHOMEMONITORSERVER_H

#include "TcpServer.h"
#include "ThreadPool.h"
#include <iostream>
#include <string>
#include <thread>

using std::cout;
using std::endl;
using std::string;
using std::thread;

namespace wd{
//把组合设计模式用到极致
class SmartHomeMonitorServer{
public:
    SmartHomeMonitorServer(int threadNum, int taskSize, unsigned short port, const string & ip = "0.0.0.0");

    void start();
    void stop();
private:
    void onConnection(wd::TcpConnectionPtr conn);

    void onMessage(wd::TcpConnectionPtr conn);

    void onClose(wd::TcpConnectionPtr conn);

private:
    wd::ThreadPool _threadpool;
    wd::TcpServer _server;
};

}//end of namespace wd

#endif

