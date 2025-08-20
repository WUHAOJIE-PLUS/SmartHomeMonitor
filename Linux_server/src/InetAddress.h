#ifndef __INETADRESS_H
#define __INETADRESS_H

#include <header.h>
#include <string>
using std::string;

namespace wd{

class InetAdress{
public:
    InetAdress(unsigned short port,const string &ip = "0.0.0.0");
    InetAdress(struct sockaddr_in &);

    string ip() const;
    unsigned short port() const;

    struct sockaddr_in *getInetAdressPtr();
private:
    struct sockaddr_in _addr;
};

}//end of namespace wd

#endif

