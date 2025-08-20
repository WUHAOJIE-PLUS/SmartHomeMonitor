#ifndef __TCPSERVER_H
#define __TCPSERVER_H

#include "Acceptor.h"
#include "EventLoop.h"
#include "TcpConnection.h"
#include <string>

using std::string;

namespace wd{

class TcpServer{
public:
    TcpServer(unsigned short port,const string &ip = "0.0.0.0");
    void setAllCallbacks(TcpConnecitonCallback &&cb1,
                         TcpConnecitonCallback &&cb2,
                         TcpConnecitonCallback &&cb3);
    void start();
    void stop();
private:
    Acceptor _acceptor;
    EventLoop _loop;
};

}//end of namespace wd

#endif

