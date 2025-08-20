#ifndef __BUSINESSSERVICE_H
#define __BUSINESSSERVICE_H

//#include "TcpServer.h"
#include "Message.h"
#include "TcpConnection.h"


namespace wd{

class UserLoginSection1{
public:
    UserLoginSection1(wd::TcpConnectionPtr conn,const wd::Packet &p)
        :_conn(conn)
         ,_packet(p){}
    void process();

private:
    wd::TcpConnectionPtr _conn;
    wd::Packet _packet;
};

class UserLoginSection2{
public:
    UserLoginSection2(wd::TcpConnectionPtr conn,const wd::Packet &p)
        :_conn(conn)
         ,_packet(p){}
    void process();

private:
    wd::TcpConnectionPtr _conn;
    wd::Packet _packet;
};

class UserRegistreSection1{
public:
    UserRegistreSection1(wd::TcpConnectionPtr conn,const wd::Packet &p)
        :_conn(conn)
         ,_packet(p){}
    void process();
    char *GenRandomString(int length);

private:
    wd::TcpConnectionPtr _conn;
    wd::Packet _packet;
};

class UserRegistreSection2{
public:
    UserRegistreSection2(wd::TcpConnectionPtr conn,const wd::Packet &p)
        :_conn(conn)
         ,_packet(p){}
    void process();

private:
    wd::TcpConnectionPtr _conn;
    wd::Packet _packet;
};

}//end of namespace wd

#endif

