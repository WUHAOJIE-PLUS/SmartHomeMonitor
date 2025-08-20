#ifndef __SOCKET_H
#define __SOCKET_H

#include "Noncopyable.h"
#include <header.h>

namespace wd{

class Socket
:Noncopyable
{
public:
    Socket();
    Socket(int);
    ~Socket();

    int fd()const;
    void shutdownWrite();

private:
    int _fd;
};
}//end of namespace wd

#endif

