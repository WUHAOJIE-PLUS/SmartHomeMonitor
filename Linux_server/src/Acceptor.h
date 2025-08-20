#ifndef __ACCEPTOR_H
#define __ACCEPTOR_H

#include "Socket.h"
#include "InetAddress.h"
#include <header.h>
#include <string>

using std::string;

namespace wd{

class Acceptor{
public:
    Acceptor(unsigned short port,const string &ip = "0.0.0.0");
    void ready();
    int accept();
    int fd()const;
private:
    void setReuseAddr(bool);//服务器可以不用等待Time_Wait
    void setReusePort(bool);//可以开多个服务器
    void bind();
    void listen();
private:
    InetAdress _addr;
    Socket _sock;
};

}//end of namespace wd

#endif

