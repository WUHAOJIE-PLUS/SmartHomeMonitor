#include "Acceptor.h"

namespace wd{

Acceptor::Acceptor(unsigned short port,const string &ip)
    :_addr(port,ip)
     ,_sock()
{

}

void Acceptor::ready(){
    setReuseAddr(true);
    setReusePort(true);
    bind();
    listen();
}

int Acceptor::accept(){
    int net_fd = ::accept(_sock.fd(),NULL,NULL);
    if(net_fd < 0){
        perror("accept");
    }
    return net_fd;
}

//服务器可以不用等待Time_Wait
void Acceptor::setReuseAddr(bool on){
    int reuse = on;
    int ret =  setsockopt(_sock.fd(),SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
    if(ret < 0){
        perror("setsockopt");
    }
}

//可以开多个服务器
void Acceptor::setReusePort(bool on){
    int reuse = on;
    int ret =  setsockopt(_sock.fd(),SOL_SOCKET,SO_REUSEPORT,&reuse,sizeof(reuse));
    if(ret < 0){
        perror("setsockopt");
    }
}

void Acceptor::bind(){
    //使用匿名命名空间访问Linux下的bind函数
    int ret = ::bind(_sock.fd(),(struct sockaddr *)_addr.getInetAdressPtr(),sizeof(*_addr.getInetAdressPtr()));
    if(ret < 0){
        perror("bind");
    }
}

void Acceptor::listen(){
    int ret = ::listen(_sock.fd(),10000);
    if(ret < 0){
        perror("listen");
    }
}  
int Acceptor::fd()const{
    return  _sock.fd();
}

}//end of namespace wd
