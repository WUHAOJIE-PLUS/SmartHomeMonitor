#include "InetAddress.h"

namespace wd{

InetAdress::InetAdress(unsigned short port,const string &ip){
    memset(&_addr,0,sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = inet_addr(ip.c_str());
}

InetAdress::InetAdress(struct sockaddr_in &addr)
:_addr(addr)
{

}

string InetAdress::ip() const{
    return inet_ntoa(_addr.sin_addr);
}

unsigned short InetAdress::port() const{
    return ntohs(_addr.sin_port);
}

struct sockaddr_in *InetAdress::getInetAdressPtr(){
    return &_addr;
}

}//end of namespace wd
